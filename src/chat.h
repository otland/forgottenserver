// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_CHAT_H
#define FS_CHAT_H

#include "const.h"
#include "luascript.h"

class Party;
class Player;

using UsersMap = std::map<uint32_t, std::shared_ptr<Player>>;
using InvitedMap = std::map<uint32_t, std::shared_ptr<const Player>>;

class ChatChannel
{
public:
	ChatChannel() = default;
	ChatChannel(uint16_t channelId, std::string channelName) : id{channelId}, name{std::move(channelName)} {}

	virtual ~ChatChannel() = default;

	bool addUser(std::shared_ptr<Player> player);
	bool removeUser(std::shared_ptr<const Player> player);
	bool hasUser(std::shared_ptr<const Player> player);

	bool talk(std::shared_ptr<const Player> fromPlayer, SpeakClasses type, const std::string& text);
	void sendToAll(const std::string& message, SpeakClasses type) const;

	const std::string& getName() const { return name; }
	uint16_t getId() const { return id; }
	const UsersMap& getUsers() const { return users; }
	virtual const InvitedMap* getInvitedUsers() const { return nullptr; }

	virtual uint32_t getOwner() const { return 0; }

	bool isPublicChannel() const { return publicChannel; }

	bool executeOnJoinEvent(std::shared_ptr<const Player> player);
	bool executeCanJoinEvent(std::shared_ptr<const Player> player);
	bool executeOnLeaveEvent(std::shared_ptr<const Player> player);
	bool executeOnSpeakEvent(std::shared_ptr<const Player> player, SpeakClasses& type, const std::string& message);

protected:
	UsersMap users;

	uint16_t id;

private:
	std::string name;

	int32_t canJoinEvent = -1;
	int32_t onJoinEvent = -1;
	int32_t onLeaveEvent = -1;
	int32_t onSpeakEvent = -1;

	bool publicChannel = false;

	friend class Chat;
};

class PrivateChatChannel final : public ChatChannel
{
public:
	PrivateChatChannel(uint16_t channelId, std::string channelName) : ChatChannel(channelId, channelName) {}

	uint32_t getOwner() const override { return owner; }
	void setOwner(uint32_t owner) { this->owner = owner; }

	bool isInvited(uint32_t guid) const;

	void invitePlayer(std::shared_ptr<const Player> player, std::shared_ptr<Player> invitePlayer);
	void excludePlayer(std::shared_ptr<const Player> player, std::shared_ptr<Player> excludePlayer);

	bool removeInvite(uint32_t guid);

	void closeChannel() const;

	const InvitedMap* getInvitedUsers() const override { return &invites; }

private:
	InvitedMap invites;
	uint32_t owner = 0;
};

using ChannelList = std::list<ChatChannel*>;

class Chat
{
public:
	Chat();

	// non-copyable
	Chat(const Chat&) = delete;
	Chat& operator=(const Chat&) = delete;

	bool load();

	ChatChannel* createChannel(std::shared_ptr<const Player> player, uint16_t channelId);
	bool deleteChannel(std::shared_ptr<const Player> player, uint16_t channelId);

	ChatChannel* addUserToChannel(std::shared_ptr<Player> player, uint16_t channelId);
	bool removeUserFromChannel(std::shared_ptr<const Player> player, uint16_t channelId);
	void removeUserFromAllChannels(std::shared_ptr<const Player> player);

	bool talkToChannel(std::shared_ptr<const Player> player, SpeakClasses type, const std::string& text,
	                   uint16_t channelId);

	ChannelList getChannelList(std::shared_ptr<const Player> player);

	ChatChannel* getChannel(std::shared_ptr<const Player> player, uint16_t channelId);
	ChatChannel* getChannelById(uint16_t channelId);
	ChatChannel* getGuildChannelById(uint32_t guildId);
	PrivateChatChannel* getPrivateChannel(std::shared_ptr<const Player> player);

	LuaScriptInterface* getScriptInterface() { return &scriptInterface; }

private:
	std::map<uint16_t, ChatChannel> normalChannels;
	std::map<uint16_t, PrivateChatChannel> privateChannels;
	std::map<Party*, ChatChannel> partyChannels;
	std::map<uint32_t, ChatChannel> guildChannels;

	LuaScriptInterface scriptInterface;

	PrivateChatChannel dummyPrivate;
};

#endif // FS_CHAT_H
