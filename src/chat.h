/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2016  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_CHAT_H_F1574642D0384ABFAB52B7ED906E5628
#define FS_CHAT_H_F1574642D0384ABFAB52B7ED906E5628

#include "const.h"
#include "luascript.h"

class Party;
class Player;

typedef std::map<uint32_t, Player*> UsersMap;
typedef std::map<uint32_t, const Player*> InvitedMap;

class ChatChannel
{
	public:
		ChatChannel() = default;
		ChatChannel(uint16_t channelId, std::string channelName):
			name(std::move(channelName)),
			id(channelId) {}

		virtual ~ChatChannel() = default;

		bool addUser(Player& player);
		bool removeUser(const Player& player);

		bool talk(const Player& fromPlayer, SpeakClasses type, const std::string& text);
		void sendToAll(const std::string& message, SpeakClasses type) const;

		const std::string& getName() const {
			return name;
		}
		uint16_t getId() const {
			return id;
		}
		const UsersMap& getUsers() const {
			return users;
		}
		virtual const InvitedMap* getInvitedUsers() const {
			return nullptr;
		}

		virtual uint32_t getOwner() const {
			return 0;
		}

		bool isPublicChannel() const { return publicChannel; }

		bool executeOnJoinEvent(const Player& player);
		bool executeCanJoinEvent(const Player& player);
		bool executeOnLeaveEvent(const Player& player);
		bool executeOnSpeakEvent(const Player& player, SpeakClasses& type, const std::string& message);

	protected:
		UsersMap users;

		std::string name;

		int32_t canJoinEvent = -1;
		int32_t onJoinEvent = -1;
		int32_t onLeaveEvent = -1;
		int32_t onSpeakEvent = -1;

		uint16_t id;
		bool publicChannel = false;

	friend class Chat;
};

class PrivateChatChannel final : public ChatChannel
{
	public:
		PrivateChatChannel(uint16_t channelId, std::string channelName) : ChatChannel(channelId, channelName) {}

		uint32_t getOwner() const final {
			return owner;
		}
		void setOwner(uint32_t owner) {
			this->owner = owner;
		}

		bool isInvited(uint32_t guid) const;

		void invitePlayer(const Player& player, Player& invitePlayer);
		void excludePlayer(const Player& player, Player& excludePlayer);

		bool removeInvite(uint32_t guid);

		void closeChannel() const;

		const InvitedMap* getInvitedUsers() const final {
			return &invites;
		}

	protected:
		InvitedMap invites;
		uint32_t owner = 0;
};

typedef std::list<ChatChannel*> ChannelList;

class Chat
{
	public:
		Chat();

		// non-copyable
		Chat(const Chat&) = delete;
		Chat& operator=(const Chat&) = delete;

		bool load();

		ChatChannel* createChannel(const Player& player, uint16_t channelId);
		bool deleteChannel(const Player& player, uint16_t channelId);

		ChatChannel* addUserToChannel(Player& player, uint16_t channelId);
		bool removeUserFromChannel(const Player& player, uint16_t channelId);
		void removeUserFromAllChannels(const Player& player);

		bool talkToChannel(const Player& player, SpeakClasses type, const std::string& text, uint16_t channelId);

		ChannelList getChannelList(const Player& player);

		ChatChannel* getChannel(const Player& player, uint16_t channelId);
		ChatChannel* getChannelById(uint16_t channelId);
		ChatChannel* getGuildChannelById(uint32_t guildId);
		PrivateChatChannel* getPrivateChannel(const Player& player);

		LuaScriptInterface* getScriptInterface() {
			return &scriptInterface;
		}

	private:
		std::map<uint16_t, ChatChannel> normalChannels;
		std::map<uint16_t, PrivateChatChannel> privateChannels;
		std::map<Party*, ChatChannel> partyChannels;
		std::map<uint32_t, ChatChannel> guildChannels;

		LuaScriptInterface scriptInterface;

		PrivateChatChannel dummyPrivate;
};

#endif
