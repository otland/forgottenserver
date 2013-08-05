/**
 * The Forgotten Server - a server application for the MMORPG Tibia
 * Copyright (C) 2013  Mark Samman <mark.samman@gmail.com>
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
#include "configmanager.h"
#include "player.h"
#include "game.h"
#include "iologindata.h"

extern ConfigManager g_config;
extern Game g_game;
extern Chat g_chat;

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
	InvitedMap::const_iterator it = m_invites.find(player.getGUID());
	if (it != m_invites.end()) {
		return false;
	}

	m_invites[player.getGUID()] = &player;
	return true;
}

bool PrivateChatChannel::removeInvited(const Player& player)
{
	InvitedMap::iterator it = m_invites.find(player.getGUID());
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
		invitePlayer.sendTextMessage(MSG_INFO_DESCR, ss.str());

		ss.str("");
		ss << invitePlayer.getName() << " has been invited.";
		player.sendTextMessage(MSG_INFO_DESCR, ss.str());

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
		player.sendTextMessage(MSG_INFO_DESCR, ss.str());

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

ChatChannel::ChatChannel()
	: Event(g_chat.getScriptInterface())
{
	flags = 0;
}

ChatChannel::ChatChannel(uint16_t channelId, const std::string& channelName)
	: Event(g_chat.getScriptInterface())
{
	id = channelId;
	name = channelName;
	flags = 0;
}

bool ChatChannel::addUser(Player& player)
{
	if (users.find(player.getID()) != users.end()) {
		return false;
	}

	if (hasFlag(CHANNEL_FLAG_LUA_ONJOIN) && !onJoin(player)) {
		return false;
	}

	if (id == CHANNEL_GUILD) {
		Guild* guild = player.getGuild();
		if (guild && !guild->getMotd().empty()) {
			g_scheduler.addEvent(createSchedulerTask(150, boost::bind(&Game::sendGuildMotd, &g_game, player.getID(), guild->getId())));
		}
	}

	if (hasFlag(CHANNEL_FLAG_EVENTS)) {
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

	if (hasFlag(CHANNEL_FLAG_EVENTS)) {
		for (const auto& it : users) {
			it.second->sendChannelEvent(id, player.getName(), CHANNELEVENT_LEAVE);
		}
	}

	if (hasFlag(CHANNEL_FLAG_LUA_ONLEAVE) && !onLeave(player)) {
		return false;
	}
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

bool ChatChannel::canJoin(const Player& player)
{
	return true;
}

bool ChatChannel::onJoin(const Player& player)
{
	return true;
}

bool ChatChannel::onLeave(const Player& player)
{
	return true;
}

bool ChatChannel::onSpeak(const Player& player, SpeakClasses& type, const std::string& message)
{
	return true;
}

Chat::Chat()
	: m_scriptInterface("Chat Interface")
{
	m_scriptInterface.initState();
	dummyPrivate = new PrivateChatChannel(CHANNEL_PRIVATE, "Private Chat Channel");
}

Chat::~Chat()
{
	for (const auto& it : normalChannels) {
		delete it.second;
	}
	normalChannels.clear();

	for (const auto& it : guildChannels) {
		delete it.second;
	}
	guildChannels.clear();

	for (const auto& it : partyChannels) {
		delete it.second;
	}
	partyChannels.clear();

	for (const auto& it : privateChannels) {
		delete it.second;
	}
	privateChannels.clear();

	delete dummyPrivate;
}

bool Chat::load()
{
	xmlDocPtr doc = xmlParseFile("data/chatchannels/chatchannels.xml");
	if (!doc) {
		return false;
	}

	xmlNodePtr root = xmlDocGetRootElement(doc);
	if (xmlStrcmp(root->name, (const xmlChar*)"channel") != 0) {
		xmlFreeDoc(doc);
		return false;
	}

	xmlNodePtr p = root->children;
	while (p) {
		if (xmlStrcmp(p->name, (const xmlChar*)"channel") == 0) {
			ChatChannel* channel = new ChatChannel();
			channel->id = readXMLValue<uint16_t>(p, "id");
			readXMLString(p, "name", channel->name);
			readXMLString(p, "script", channel->scriptname);
			if (readXMLValue<int32_t>(p, "onspeak") != 0) {
				channel->flags |= CHANNEL_FLAG_LUA_ONSPEAK;
			}
			if (readXMLValue<int32_t>(p, "canjoin") != 0) {
				channel->flags |= CHANNEL_FLAG_LUA_CANJOIN;
			}
			if (readXMLValue<int32_t>(p, "onjoin") != 0) {
				channel->flags |= CHANNEL_FLAG_LUA_ONJOIN;
			}
			if (readXMLValue<int32_t>(p, "onleave") != 0) {
				channel->flags |= CHANNEL_FLAG_LUA_ONLEAVE;
			}
			normalChannels[channel->id] = channel;
		}
		p = p->next;
	}

	xmlFreeDoc(doc);
	return true;
}

ChatChannel* Chat::createChannel(const Player& player, uint16_t channelId)
{
	if (getChannel(player, channelId)) {
		return NULL;
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
				return NULL;
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
	return NULL;
}

bool Chat::deleteChannel(const Player& player, uint16_t channelId)
{
	switch (channelId) {
		case CHANNEL_GUILD: {
			Guild* guild = player.getGuild();
			if (!guild) {
				return false;
			}

			GuildChannelMap::iterator it = guildChannels.find(guild->getId());
			if (it == guildChannels.end()) {
				return false;
			}

			delete it->second;
			guildChannels.erase(it);
			return true;
		}

		case CHANNEL_PARTY: {
			Party* party = player.getParty();
			if (!party) {
				return false;
			}

			PartyChannelMap::iterator it = partyChannels.find(party);
			if (it == partyChannels.end()) {
				return false;
			}

			delete it->second;
			partyChannels.erase(it);
			return true;
		}

		default: {
			PrivateChannelMap::iterator it = privateChannels.find(channelId);
			if (it == privateChannels.end()) {
				return false;
			}

			it->second->closeChannel();

			delete it->second;
			privateChannels.erase(it);
			return true;
		}
	}
	return false;
}

ChatChannel* Chat::addUserToChannel(Player& player, uint16_t channelId)
{
	ChatChannel* channel = getChannel(player, channelId);
	if (channel && channel->addUser(player)) {
		return channel;
	}
	return NULL;
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
	for (const auto& it : normalChannels) {
		it.second->removeUser(player);
	}

	for (const auto& it : partyChannels) {
		it.second->removeUser(player);
	}

	for (const auto& it : guildChannels) {
		it.second->removeUser(player);
	}

	for (const auto& it : privateChannels) {
		PrivateChatChannel* channel = it.second;
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

	if (channelId == CHANNEL_GUILD && player.getGuildLevel() > 1) {
		type = SPEAK_CHANNEL_O;
	}

	if (channel->hasFlag(CHANNEL_FLAG_LUA_ONSPEAK) && !channel->onSpeak(player, type, text)) {
		return false;
	}

	return channel->talk(player, type, text);
}

std::string Chat::getChannelName(const Player& player, uint16_t channelId)
{
	ChatChannel* channel = getChannel(player, channelId);
	if (!channel) {
		return "";
	}
	return channel->getName();
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

	for (NormalChannelMap::iterator it = normalChannels.begin(); it != normalChannels.end(); ++it) {
		ChatChannel* channel = getChannel(player, it->first);
		if (channel) {
			list.push_back(it->second);
		}
	}

	bool hasPrivate = false;
	for (PrivateChannelMap::iterator pit = privateChannels.begin(); pit != privateChannels.end(); ++pit) {
		if (PrivateChatChannel* channel = pit->second) {
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
				GuildChannelMap::iterator git = guildChannels.find(guild->getId());
				if (git != guildChannels.end()) {
					return git->second;
				}
			}
			break;
		}

		case CHANNEL_PARTY: {
			Party* party = player.getParty();
			if (party) {
				PartyChannelMap::iterator it = partyChannels.find(party);
				if (it != partyChannels.end()) {
					return it->second;
				}
			}
			break;
		}

		default: {
			NormalChannelMap::iterator nit = normalChannels.find(channelId);
			if (nit != normalChannels.end()) {
				if (nit->second->canJoin(player)) {
					return nit->second;
				}
			} else {
				PrivateChannelMap::iterator pit = privateChannels.find(channelId);
				if (pit != privateChannels.end() && pit->second->isInvited(player)) {
					return pit->second;
				}
			}
			break;
		}
	}
	return NULL;
}

ChatChannel* Chat::getGuildChannelById(uint32_t guildId)
{
	GuildChannelMap::iterator it = guildChannels.find(guildId);
	if (it == guildChannels.end()) {
		return NULL;
	}
	return it->second;
}

ChatChannel* Chat::getChannelById(uint16_t channelId)
{
	NormalChannelMap::iterator it = normalChannels.find(channelId);
	if (it == normalChannels.end()) {
		return NULL;
	}
	return it->second;
}

PrivateChatChannel* Chat::getPrivateChannel(const Player& player)
{
	for (PrivateChannelMap::iterator it = privateChannels.begin(); it != privateChannels.end(); ++it) {
		PrivateChatChannel* channel = it->second;
		if (channel->getOwner() == player.getGUID()) {
			return channel;
		}
	}
	return NULL;
}
