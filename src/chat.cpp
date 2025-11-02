// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "chat.h"

#include "game.h"
#include "pugicast.h"
#include "scheduler.h"

extern Chat* g_chat;
extern Game g_game;

bool PrivateChatChannel::isInvited(uint32_t guid) const
{
	if (guid == getOwner()) {
		return true;
	}
	return invites.find(guid) != invites.end();
}

bool PrivateChatChannel::removeInvite(uint32_t guid) { return invites.erase(guid) != 0; }

void PrivateChatChannel::invitePlayer(const std::shared_ptr<const Player>& player,
                                      const std::shared_ptr<Player>& invitePlayer)
{
	auto result = invites.emplace(invitePlayer->getGUID(), invitePlayer);
	if (!result.second) {
		return;
	}

	invitePlayer->sendTextMessage(MESSAGE_INFO_DESCR,
	                              fmt::format("{:s} invites you to {:s} private chat channel.", player->getName(),
	                                          player->getSex() == PLAYERSEX_FEMALE ? "her" : "his"));

	player->sendTextMessage(MESSAGE_INFO_DESCR, fmt::format("{:s} has been invited.", invitePlayer->getName()));

	for (const auto& it : users) {
		it.second->sendChannelEvent(id, invitePlayer->getName(), CHANNELEVENT_INVITE);
	}
}

void PrivateChatChannel::excludePlayer(const std::shared_ptr<const Player>& player,
                                       const std::shared_ptr<Player>& excludePlayer)
{
	if (!removeInvite(excludePlayer->getGUID())) {
		return;
	}

	removeUser(excludePlayer);

	player->sendTextMessage(MESSAGE_INFO_DESCR, fmt::format("{:s} has been excluded.", excludePlayer->getName()));

	excludePlayer->sendClosePrivate(id);

	for (const auto& it : users) {
		it.second->sendChannelEvent(id, excludePlayer->getName(), CHANNELEVENT_EXCLUDE);
	}
}

void PrivateChatChannel::closeChannel() const
{
	for (const auto& it : users) {
		it.second->sendClosePrivate(id);
	}
}

bool ChatChannel::addUser(const std::shared_ptr<Player>& player)
{
	if (users.find(player->getID()) != users.end()) {
		return false;
	}

	if (!executeOnJoinEvent(player)) {
		return false;
	}

	// TODO: Move to script when guild channels can be scripted
	if (id == CHANNEL_GUILD) {
		if (const auto& guild = player->getGuild(); !guild->getMotd().empty()) {
			g_scheduler.addEvent(
			    createSchedulerTask(150, [playerID = player->getID()]() { g_game.sendGuildMotd(playerID); }));
		}
	}

	if (!publicChannel) {
		for (const auto& it : users) {
			it.second->sendChannelEvent(id, player->getName(), CHANNELEVENT_JOIN);
		}
	}

	users[player->getID()] = player;
	return true;
}

bool ChatChannel::removeUser(const std::shared_ptr<const Player>& player)
{
	auto iter = users.find(player->getID());
	if (iter == users.end()) {
		return false;
	}

	users.erase(iter);

	if (!publicChannel) {
		for (const auto& it : users) {
			it.second->sendChannelEvent(id, player->getName(), CHANNELEVENT_LEAVE);
		}
	}

	executeOnLeaveEvent(player);
	return true;
}

bool ChatChannel::hasUser(const std::shared_ptr<const Player>& player)
{
	return users.find(player->getID()) != users.end();
}

void ChatChannel::sendToAll(const std::string& message, SpeakClasses type) const
{
	for (const auto& it : users) {
		it.second->sendChannelMessage("", message, type, id);
	}
}

bool ChatChannel::talk(const std::shared_ptr<const Player>& fromPlayer, SpeakClasses type, const std::string& text)
{
	if (users.find(fromPlayer->getID()) == users.end()) {
		return false;
	}

	for (const auto& it : users) {
		it.second->sendToChannel(fromPlayer, type, text, id);
	}
	return true;
}

bool ChatChannel::executeCanJoinEvent(const std::shared_ptr<const Player>& player)
{
	if (canJoinEvent == -1) {
		return true;
	}

	// canJoin(player)
	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - CanJoinChannelEvent::execute] Call stack overflow" << std::endl;
		return false;
	}

	LuaScriptInterface* scriptInterface = g_chat->getScriptInterface();
	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(canJoinEvent, scriptInterface);

	lua_State* L = scriptInterface->getLuaState();

	scriptInterface->pushFunction(canJoinEvent);
	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	return scriptInterface->callFunction(1);
}

bool ChatChannel::executeOnJoinEvent(const std::shared_ptr<const Player>& player)
{
	if (onJoinEvent == -1) {
		return true;
	}

	// onJoin(player)
	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - OnJoinChannelEvent::execute] Call stack overflow" << std::endl;
		return false;
	}

	LuaScriptInterface* scriptInterface = g_chat->getScriptInterface();
	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(onJoinEvent, scriptInterface);

	lua_State* L = scriptInterface->getLuaState();

	scriptInterface->pushFunction(onJoinEvent);
	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	return scriptInterface->callFunction(1);
}

bool ChatChannel::executeOnLeaveEvent(const std::shared_ptr<const Player>& player)
{
	if (onLeaveEvent == -1) {
		return true;
	}

	// onLeave(player)
	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - OnLeaveChannelEvent::execute] Call stack overflow" << std::endl;
		return false;
	}

	LuaScriptInterface* scriptInterface = g_chat->getScriptInterface();
	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(onLeaveEvent, scriptInterface);

	lua_State* L = scriptInterface->getLuaState();

	scriptInterface->pushFunction(onLeaveEvent);
	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	return scriptInterface->callFunction(1);
}

bool ChatChannel::executeOnSpeakEvent(const std::shared_ptr<const Player>& player, SpeakClasses& type,
                                      const std::string& message)
{
	if (onSpeakEvent == -1) {
		return true;
	}

	// onSpeak(player, type, message)
	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - OnSpeakChannelEvent::execute] Call stack overflow" << std::endl;
		return false;
	}

	LuaScriptInterface* scriptInterface = g_chat->getScriptInterface();
	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(onSpeakEvent, scriptInterface);

	lua_State* L = scriptInterface->getLuaState();

	scriptInterface->pushFunction(onSpeakEvent);
	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	lua_pushnumber(L, type);
	tfs::lua::pushString(L, message);

	bool result = false;
	int size0 = lua_gettop(L);
	int ret = tfs::lua::protectedCall(L, 3, 1);
	if (ret != 0) {
		reportErrorFunc(nullptr, tfs::lua::popString(L));
	} else if (lua_gettop(L) > 0) {
		if (lua_isboolean(L, -1)) {
			result = tfs::lua::getBoolean(L, -1);
		} else if (lua_isnumber(L, -1)) {
			result = true;
			type = tfs::lua::getNumber<SpeakClasses>(L, -1);
		}
		lua_pop(L, 1);
	}

	if ((lua_gettop(L) + 4) != size0) {
		reportErrorFunc(nullptr, "Stack size changed!");
	}
	tfs::lua::resetScriptEnv();
	return result;
}

Chat::Chat() : scriptInterface("Chat Interface"), dummyPrivate(CHANNEL_PRIVATE, "Private Chat Channel")
{
	scriptInterface.initState();
}

bool Chat::load()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/chatchannels/chatchannels.xml");
	if (!result) {
		printXMLError("Error - Chat::load", "data/chatchannels/chatchannels.xml", result);
		return false;
	}

	for (auto channelNode : doc.child("channels").children()) {
		uint16_t channelId = pugi::cast<uint16_t>(channelNode.attribute("id").value());
		std::string channelName = channelNode.attribute("name").as_string();
		bool isPublic = channelNode.attribute("public").as_bool();
		pugi::xml_attribute scriptAttribute = channelNode.attribute("script");

		auto it = normalChannels.find(channelId);
		if (it != normalChannels.end()) {
			ChatChannel& channel = it->second;
			channel.publicChannel = isPublic;
			channel.name = channelName;

			if (scriptAttribute) {
				if (scriptInterface.loadFile("data/chatchannels/scripts/" + std::string(scriptAttribute.as_string())) ==
				    0) {
					channel.onSpeakEvent = scriptInterface.getEvent("onSpeak");
					channel.canJoinEvent = scriptInterface.getEvent("canJoin");
					channel.onJoinEvent = scriptInterface.getEvent("onJoin");
					channel.onLeaveEvent = scriptInterface.getEvent("onLeave");
				} else {
					std::cout << "[Warning - Chat::load] Can not load script: " << scriptAttribute.as_string()
					          << std::endl;
				}
			}

			UsersMap tempUserMap = std::move(channel.users);
			for (const auto& user : tempUserMap | std::views::values) {
				channel.addUser(user);
			}
			continue;
		}

		ChatChannel channel(channelId, channelName);
		channel.publicChannel = isPublic;

		if (scriptAttribute) {
			if (scriptInterface.loadFile("data/chatchannels/scripts/" + std::string(scriptAttribute.as_string())) ==
			    0) {
				channel.onSpeakEvent = scriptInterface.getEvent("onSpeak");
				channel.canJoinEvent = scriptInterface.getEvent("canJoin");
				channel.onJoinEvent = scriptInterface.getEvent("onJoin");
				channel.onLeaveEvent = scriptInterface.getEvent("onLeave");
			} else {
				std::cout << "[Warning - Chat::load] Can not load script: " << scriptAttribute.as_string() << std::endl;
			}
		}

		normalChannels[channel.id] = channel;
	}
	return true;
}

ChatChannel* Chat::createChannel(const std::shared_ptr<const Player>& player, uint16_t channelId)
{
	if (getChannel(player, channelId)) {
		return nullptr;
	}

	switch (channelId) {
		case CHANNEL_GUILD: {
			if (const auto& guild = player->getGuild()) {
				auto ret =
				    guildChannels.emplace(std::make_pair(guild->getId(), ChatChannel(channelId, guild->getName())));
				return &ret.first->second;
			}
			break;
		}

		case CHANNEL_PARTY: {
			if (Party* party = player->getParty()) {
				auto ret = partyChannels.emplace(std::make_pair(party, ChatChannel(channelId, "Party")));
				return &ret.first->second;
			}
			break;
		}

		case CHANNEL_PRIVATE: {
			// only 1 private channel for each premium player
			if (!player->isPremium() || getPrivateChannel(player)) {
				return nullptr;
			}

			// find a free private channel slot
			for (uint16_t i = 100; i < 10000; ++i) {
				auto [it, inserted] =
				    privateChannels.emplace(std::make_pair(i, PrivateChatChannel(i, player->getName() + "'s Channel")));
				if (inserted) { // second is a bool that indicates that a new channel has been placed in the map
					auto& newChannel = it->second;
					newChannel.setOwner(player->getGUID());
					return &newChannel;
				}
			}
			break;
		}

		default:
			break;
	}
	return nullptr;
}

bool Chat::deleteChannel(const std::shared_ptr<const Player>& player, uint16_t channelId)
{
	switch (channelId) {
		case CHANNEL_GUILD: {
			if (const auto& guild = player->getGuild()) {
				guildChannels.erase(guild->getId());
				return true;
			}
			return false;
		}

		case CHANNEL_PARTY: {
			if (Party* party = player->getParty()) {
				partyChannels.erase(party);
				return true;
			}
			return false;
		}

		default: {
			auto it = privateChannels.find(channelId);
			if (it == privateChannels.end()) {
				return false;
			}

			it->second.closeChannel();

			privateChannels.erase(it);
			break;
		}
	}
	return true;
}

ChatChannel* Chat::addUserToChannel(const std::shared_ptr<Player>& player, uint16_t channelId)
{
	ChatChannel* channel = getChannel(player, channelId);
	if (channel && channel->addUser(player)) {
		return channel;
	}
	return nullptr;
}

bool Chat::removeUserFromChannel(const std::shared_ptr<const Player>& player, uint16_t channelId)
{
	ChatChannel* channel = getChannel(player, channelId);
	if (!channel || !channel->removeUser(player)) {
		return false;
	}

	if (channel->getOwner() == player->getGUID()) {
		deleteChannel(player, channelId);
	}
	return true;
}

void Chat::removeUserFromAllChannels(const std::shared_ptr<const Player>& player)
{
	for (auto& it : normalChannels) {
		it.second.removeUser(player);
	}

	for (auto& it : partyChannels) {
		it.second.removeUser(player);
	}

	for (auto& it : guildChannels) {
		it.second.removeUser(player);
	}

	auto it = privateChannels.begin();
	while (it != privateChannels.end()) {
		PrivateChatChannel* channel = &it->second;
		channel->removeInvite(player->getGUID());
		channel->removeUser(player);
		if (channel->getOwner() == player->getGUID()) {
			channel->closeChannel();
			it = privateChannels.erase(it);
		} else {
			++it;
		}
	}
}

bool Chat::talkToChannel(const std::shared_ptr<const Player>& player, SpeakClasses type, const std::string& text,
                         uint16_t channelId)
{
	ChatChannel* channel = getChannel(player, channelId);
	if (!channel) {
		return false;
	}

	if (channelId == CHANNEL_GUILD) {
		const auto& rank = player->getGuildRank();
		if (rank && rank->level > 1) {
			type = TALKTYPE_CHANNEL_O;
		} else if (type != TALKTYPE_CHANNEL_Y) {
			type = TALKTYPE_CHANNEL_Y;
		}
	} else if (channelId == CHANNEL_PRIVATE || channelId == CHANNEL_PARTY) {
		type = TALKTYPE_CHANNEL_Y;
	}

	if (!channel->executeOnSpeakEvent(player, type, text)) {
		return false;
	}

	return channel->talk(player, type, text);
}

ChannelList Chat::getChannelList(const std::shared_ptr<const Player>& player)
{
	ChannelList list;
	if (player->getGuild()) {
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

	if (player->getParty()) {
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
	for (auto& it : privateChannels) {
		if (PrivateChatChannel* channel = &it.second) {
			uint32_t guid = player->getGUID();
			if (channel->isInvited(guid)) {
				list.push_back(channel);
			}

			if (channel->getOwner() == guid) {
				hasPrivate = true;
			}
		}
	}

	if (!hasPrivate && player->isPremium()) {
		list.push_front(&dummyPrivate);
	}
	return list;
}

ChatChannel* Chat::getChannel(const std::shared_ptr<const Player>& player, uint16_t channelId)
{
	switch (channelId) {
		case CHANNEL_GUILD: {
			if (const auto& guild = player->getGuild()) {
				auto it = guildChannels.find(guild->getId());
				if (it != guildChannels.end()) {
					return &it->second;
				}
			}
			break;
		}

		case CHANNEL_PARTY: {
			Party* party = player->getParty();
			if (party) {
				auto it = partyChannels.find(party);
				if (it != partyChannels.end()) {
					return &it->second;
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
				if (it2 != privateChannels.end() && it2->second.isInvited(player->getGUID())) {
					return &it2->second;
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
	return &it->second;
}

ChatChannel* Chat::getChannelById(uint16_t channelId)
{
	auto it = normalChannels.find(channelId);
	if (it == normalChannels.end()) {
		return nullptr;
	}
	return &it->second;
}

PrivateChatChannel* Chat::getPrivateChannel(const std::shared_ptr<const Player>& player)
{
	for (auto& it : privateChannels) {
		if (it.second.getOwner() == player->getGUID()) {
			return &it.second;
		}
	}
	return nullptr;
}
