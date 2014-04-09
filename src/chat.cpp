/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2014  Mark Samman <mark.samman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "otpch.h"

#include "chat.h"
#include "game.h"
#include "pugicast.h"
#include "player.h"
#include "scheduler.h"

extern Chat g_chat;
extern Game g_game;

PrivateChatChannel::PrivateChatChannel(uint16_t channelId, const std::string& channelName) :
	ChatChannel(channelId, channelName)
{
	m_owner = 0;
}

bool PrivateChatChannel::isInvited(const Player& player) const
{
	if (player.getGUID() == getOwner()) {
		return true;
	}
	return m_invites.find(player.getGUID()) != m_invites.end();
}

bool PrivateChatChannel::addInvited(Player& player)
{
	auto it = m_invites.find(player.getGUID());
	if (it != m_invites.end()) {
		return false;
	}

	m_invites[player.getGUID()] = &player;
	return true;
}

bool PrivateChatChannel::removeInvited(const Player& player)
{
	auto it = m_invites.find(player.getGUID());
	if (it == m_invites.end()) {
		return false;
	}

	m_invites.erase(it);
	return true;
}

void PrivateChatChannel::invitePlayer(const Player& player, Player& invitePlayer)
{
	if (addInvited(invitePlayer)) {
		std::ostringstream ss;
		ss << player.getName() << " invites you to " << (player.getSex() == PLAYERSEX_FEMALE ? "her" : "his") << " private chat channel.";
		invitePlayer.sendTextMessage(MESSAGE_INFO_DESCR, ss.str());

		ss.str("");
		ss << invitePlayer.getName() << " has been invited.";
		player.sendTextMessage(MESSAGE_INFO_DESCR, ss.str());

		for (const auto& it : users) {
			it.second->sendChannelEvent(id, invitePlayer.getName(), CHANNELEVENT_INVITE);
		}
	}
}

void PrivateChatChannel::excludePlayer(const Player& player, Player& excludePlayer)
{
	if (removeInvited(excludePlayer)) {
		removeUser(excludePlayer);

		std::ostringstream ss;
		ss << excludePlayer.getName() << " has been excluded.";
		player.sendTextMessage(MESSAGE_INFO_DESCR, ss.str());

		excludePlayer.sendClosePrivate(getId());

		for (const auto& it : users) {
			it.second->sendChannelEvent(id, excludePlayer.getName(), CHANNELEVENT_EXCLUDE);
		}
	}
}

void PrivateChatChannel::closeChannel()
{
	for (const auto& it : users) {
		it.second->sendClosePrivate(getId());
	}
}

bool ChatChannel::addUser(Player& player)
{
	if (users.find(player.getID()) != users.end()) {
		return false;
	}

	if (!executeOnJoinEvent(player)) {
		return false;
	}

	// TODO: Move to script when guild channels can be scripted
	if (id == CHANNEL_GUILD) {
		Guild* guild = player.getGuild();
		if (guild && !guild->getMotd().empty()) {
			g_scheduler.addEvent(createSchedulerTask(150, std::bind(&Game::sendGuildMotd, &g_game, player.getID())));
		}
	}

	if (!publicChannel) {
		for (const auto& it : users) {
			it.second->sendChannelEvent(id, player.getName(), CHANNELEVENT_JOIN);
		}
	}

	users[player.getID()] = &player;
	return true;
}

bool ChatChannel::removeUser(const Player& player)
{
	auto iter = users.find(player.getID());
	if (iter == users.end()) {
		return false;
	}

	users.erase(iter);

	if (!publicChannel) {
		for (const auto& it : users) {
			it.second->sendChannelEvent(id, player.getName(), CHANNELEVENT_LEAVE);
		}
	}

	executeOnLeaveEvent(player);
	return true;
}

void ChatChannel::sendToAll(const std::string& message, SpeakClasses type)
{
	for (const auto& it : users) {
		it.second->sendChannelMessage("", message, type, id);
	}
}

bool ChatChannel::talk(const Player& fromPlayer, SpeakClasses type, const std::string& text)
{
	if (users.find(fromPlayer.getID()) == users.end()) {
		return false;
	}

	for (const auto& it : users) {
		it.second->sendToChannel(&fromPlayer, type, text, getId());
	}
	return true;
}

bool ChatChannel::executeCanJoinEvent(const Player& player)
{
	if (canJoinEvent == -1) {
		return true;
	}

	//canJoin(player)
	LuaScriptInterface* m_scriptInterface = g_chat.getScriptInterface();
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - CanJoinChannelEvent::execute] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(canJoinEvent, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();

	m_scriptInterface->pushFunction(canJoinEvent);
	LuaScriptInterface::pushUserdata(L, &player);
	LuaScriptInterface::setMetatable(L, -1, "Player");

	return m_scriptInterface->callFunction(1);
}

bool ChatChannel::executeOnJoinEvent(const Player& player)
{
	if (onJoinEvent == -1) {
		return true;
	}

	//onJoin(player)
	LuaScriptInterface* m_scriptInterface = g_chat.getScriptInterface();
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - OnJoinChannelEvent::execute] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(onJoinEvent, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();

	m_scriptInterface->pushFunction(onJoinEvent);
	LuaScriptInterface::pushUserdata(L, &player);
	LuaScriptInterface::setMetatable(L, -1, "Player");

	return m_scriptInterface->callFunction(1);
}

bool ChatChannel::executeOnLeaveEvent(const Player& player)
{
	if (onLeaveEvent == -1) {
		return true;
	}

	//onLeave(player)
	LuaScriptInterface* m_scriptInterface = g_chat.getScriptInterface();
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - OnLeaveChannelEvent::execute] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(onLeaveEvent, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();

	m_scriptInterface->pushFunction(onLeaveEvent);
	LuaScriptInterface::pushUserdata(L, &player);
	LuaScriptInterface::setMetatable(L, -1, "Player");

	return m_scriptInterface->callFunction(1);
}

bool ChatChannel::executeOnSpeakEvent(const Player& player, SpeakClasses& type, const std::string& message)
{
	if (onSpeakEvent == -1) {
		return true;
	}

	//onSpeak(player, type, message)
	LuaScriptInterface* m_scriptInterface = g_chat.getScriptInterface();
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - OnSpeakChannelEvent::execute] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(onSpeakEvent, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();

	m_scriptInterface->pushFunction(onSpeakEvent);
	LuaScriptInterface::pushUserdata(L, &player);
	LuaScriptInterface::setMetatable(L, -1, "Player");

	lua_pushnumber(L, type);
	LuaScriptInterface::pushString(L, message);

	bool result = false;
	int32_t size0 = lua_gettop(L);
	int32_t ret = m_scriptInterface->protectedCall(L, 3, 1);
	if (ret != 0) {
		LuaScriptInterface::reportError(nullptr, LuaScriptInterface::popString(L));
	} else if (lua_isboolean(L, -1)) {
		result = LuaScriptInterface::popBoolean(L);
	} else if (lua_gettop(L) > 0 && lua_isnumber(L, -1)) {
		result = true;
		type = static_cast<SpeakClasses>(LuaScriptInterface::popNumber<uint32_t>(L));
	}

	if ((lua_gettop(L) + 4) != size0) {
		LuaScriptInterface::reportError(nullptr, "Stack size changed!");
	}
	m_scriptInterface->resetScriptEnv();
	return result;
}

Chat::Chat()
	: m_scriptInterface("Chat Interface")
{
	m_scriptInterface.initState();
	dummyPrivate = new PrivateChatChannel(CHANNEL_PRIVATE, "Private Chat Channel");
}

Chat::~Chat()
{
	for (const auto& it : guildChannels) {
		delete it.second;
	}

	for (const auto& it : partyChannels) {
		delete it.second;
	}

	for (const auto& it : privateChannels) {
		delete it.second;
	}

	delete dummyPrivate;
}

bool Chat::load()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/chatchannels/chatchannels.xml");
	if (!result) {
		std::cout << "[Error - Chat::load] Failed to load data/chatchannels/chatchannels.xml: " << result.description() << std::endl;
		return false;
	}

	std::forward_list<uint16_t> removedChannels;
	for (auto& channelEntry : normalChannels) {
		ChatChannel& channel = channelEntry.second;
		channel.onSpeakEvent = -1;
		channel.canJoinEvent = -1;
		channel.onJoinEvent = -1;
		channel.onLeaveEvent = -1;
		removedChannels.push_front(channelEntry.first);
	}

	for (pugi::xml_node channelNode = doc.child("channels").first_child(); channelNode; channelNode = channelNode.next_sibling()) {
		ChatChannel channel(pugi::cast<uint16_t>(channelNode.attribute("id").value()), channelNode.attribute("name").as_string());
		channel.publicChannel = channelNode.attribute("public").as_bool();

		pugi::xml_attribute scriptAttribute = channelNode.attribute("script");
		if (scriptAttribute) {
			if (m_scriptInterface.loadFile("data/chatchannels/scripts/" + std::string(scriptAttribute.as_string())) == 0) {
				channel.onSpeakEvent = m_scriptInterface.getEvent("onSpeak");
				channel.canJoinEvent = m_scriptInterface.getEvent("canJoin");
				channel.onJoinEvent = m_scriptInterface.getEvent("onJoin");
				channel.onLeaveEvent = m_scriptInterface.getEvent("onLeave");
			} else {
				std::cout << "[Warning - Chat::load] Can not load script: " << scriptAttribute.as_string() << std::endl;
			}
		}

		removedChannels.remove(channel.id);
		normalChannels[channel.id] = channel;
	}

	for (uint16_t channelId : removedChannels) {
		normalChannels.erase(channelId);
	}
	return true;
}

ChatChannel* Chat::createChannel(const Player& player, uint16_t channelId)
{
	if (getChannel(player, channelId)) {
		return nullptr;
	}

	switch (channelId) {
		case CHANNEL_GUILD: {
			Guild* guild = player.getGuild();
			if (guild) {
				ChatChannel* newChannel = new ChatChannel(channelId, guild->getName());
				guildChannels[guild->getId()] = newChannel;
				return newChannel;
			}
			break;
		}

		case CHANNEL_PARTY: {
			Party* party = player.getParty();
			if (party) {
				ChatChannel* newChannel = new ChatChannel(channelId, "Party");
				partyChannels[party] = newChannel;
				return newChannel;
			}
			break;
		}

		case CHANNEL_PRIVATE: {
			//only 1 private channel for each premium player
			if (!player.isPremium() || getPrivateChannel(player)) {
				return nullptr;
			}

			//find a free private channel slot
			for (uint16_t i = 100; i < 10000; ++i) {
				if (privateChannels.find(i) == privateChannels.end()) {
					PrivateChatChannel* newChannel = new PrivateChatChannel(i, player.getName() + "'s Channel");
					newChannel->setOwner(player.getGUID());
					privateChannels[i] = newChannel;
					return newChannel;
				}
			}
			break;
		}

		default:
			break;
	}
	return nullptr;
}

bool Chat::deleteChannel(const Player& player, uint16_t channelId)
{
	switch (channelId) {
		case CHANNEL_GUILD: {
			Guild* guild = player.getGuild();
			if (!guild) {
				return false;
			}

			auto it = guildChannels.find(guild->getId());
			if (it == guildChannels.end()) {
				return false;
			}

			delete it->second;
			guildChannels.erase(it);
			break;
		}

		case CHANNEL_PARTY: {
			Party* party = player.getParty();
			if (!party) {
				return false;
			}

			auto it = partyChannels.find(party);
			if (it == partyChannels.end()) {
				return false;
			}

			delete it->second;
			partyChannels.erase(it);
			break;
		}

		default: {
			auto it = privateChannels.find(channelId);
			if (it == privateChannels.end()) {
				return false;
			}

			it->second->closeChannel();

			delete it->second;
			privateChannels.erase(it);
			break;
		}
	}
	return true;
}

ChatChannel* Chat::addUserToChannel(Player& player, uint16_t channelId)
{
	ChatChannel* channel = getChannel(player, channelId);
	if (channel && channel->addUser(player)) {
		return channel;
	}
	return nullptr;
}

bool Chat::removeUserFromChannel(const Player& player, uint16_t channelId)
{
	ChatChannel* channel = getChannel(player, channelId);
	if (!channel || !channel->removeUser(player)) {
		return false;
	}

	if (channel->getOwner() == player.getGUID()) {
		deleteChannel(player, channelId);
	}
	return true;
}

void Chat::removeUserFromAllChannels(const Player& player)
{
	for (auto& it : normalChannels) {
		it.second.removeUser(player);
	}

	for (const auto& it : partyChannels) {
		it.second->removeUser(player);
	}

	for (const auto& it : guildChannels) {
		it.second->removeUser(player);
	}

	for (const auto& it : privateChannels) {
		PrivateChatChannel* channel = it.second;
		channel->removeInvited(player);
		channel->removeUser(player);
		if (channel->getOwner() == player.getGUID()) {
			deleteChannel(player, channel->getId());
		}
	}
}

bool Chat::talkToChannel(const Player& player, SpeakClasses type, const std::string& text, uint16_t channelId)
{
	ChatChannel* channel = getChannel(player, channelId);
	if (!channel) {
		return false;
	}

	if (channelId == CHANNEL_GUILD) {
		if (player.getGuildLevel() > 1) {
			type = TALKTYPE_CHANNEL_O;
		} else if (type != TALKTYPE_CHANNEL_Y) {
			type = TALKTYPE_CHANNEL_Y;
		}
	} else if (type != TALKTYPE_CHANNEL_Y && (channelId == CHANNEL_PRIVATE || channelId == CHANNEL_PARTY)) {
		type = TALKTYPE_CHANNEL_Y;
	}

	if (!channel->executeOnSpeakEvent(player, type, text)) {
		return false;
	}

	return channel->talk(player, type, text);
}

ChannelList Chat::getChannelList(const Player& player)
{
	ChannelList list;
	if (player.getGuild()) {
		ChatChannel* channel = getChannel(player, CHANNEL_GUILD);
		if (channel) {
			list.push_back(channel);
		} else {
			channel = createChannel(player, CHANNEL_GUILD);
			if (channel) {
				list.push_back(channel);
			}
		}
	}

	if (player.getParty()) {
		ChatChannel* channel = getChannel(player, CHANNEL_PARTY);
		if (channel) {
			list.push_back(channel);
		} else {
			channel = createChannel(player, CHANNEL_PARTY);
			if (channel) {
				list.push_back(channel);
			}
		}
	}

	for (const auto& it : normalChannels) {
		ChatChannel* channel = getChannel(player, it.first);
		if (channel) {
			list.push_back(channel);
		}
	}

	bool hasPrivate = false;
	for (const auto& it : privateChannels) {
		if (PrivateChatChannel* channel = it.second) {
			if (channel->isInvited(player)) {
				list.push_back(channel);
			}

			if (channel->getOwner() == player.getGUID()) {
				hasPrivate = true;
			}
		}
	}

	if (!hasPrivate && player.isPremium()) {
		list.push_front(dummyPrivate);
	}
	return list;
}

ChatChannel* Chat::getChannel(const Player& player, uint16_t channelId)
{
	switch (channelId) {
		case CHANNEL_GUILD: {
			Guild* guild = player.getGuild();
			if (guild) {
				auto it = guildChannels.find(guild->getId());
				if (it != guildChannels.end()) {
					return it->second;
				}
			}
			break;
		}

		case CHANNEL_PARTY: {
			Party* party = player.getParty();
			if (party) {
				auto it = partyChannels.find(party);
				if (it != partyChannels.end()) {
					return it->second;
				}
			}
			break;
		}

		default: {
			auto it = normalChannels.find(channelId);
			if (it != normalChannels.end()) {
				ChatChannel& channel = it->second;
				if (!channel.executeCanJoinEvent(player)) {
					return nullptr;
				}
				return &channel;
			} else {
				auto it2 = privateChannels.find(channelId);
				if (it2 != privateChannels.end() && it2->second->isInvited(player)) {
					return it2->second;
				}
			}
			break;
		}
	}
	return nullptr;
}

ChatChannel* Chat::getGuildChannelById(uint32_t guildId)
{
	auto it = guildChannels.find(guildId);
	if (it == guildChannels.end()) {
		return nullptr;
	}
	return it->second;
}

ChatChannel* Chat::getChannelById(uint16_t channelId)
{
	auto it = normalChannels.find(channelId);
	if (it == normalChannels.end()) {
		return nullptr;
	}
	return &it->second;
}

PrivateChatChannel* Chat::getPrivateChannel(const Player& player)
{
	for (const auto& it : privateChannels) {
		if (it.second->getOwner() == player.getGUID()) {
			return it.second;
		}
	}
	return nullptr;
}
