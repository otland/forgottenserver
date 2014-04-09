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

#ifndef FS_CHAT_H_F1574642D0384ABFAB52B7ED906E5628
#define FS_CHAT_H_F1574642D0384ABFAB52B7ED906E5628

#include "const.h"
#include "luascript.h"

class Party;
class Player;

typedef std::map<uint32_t, Player*> UsersMap;
typedef std::map<uint32_t, Player*> InvitedMap;

class ChatChannel
{
	public:
		ChatChannel() {}
		ChatChannel(uint16_t channelId, const std::string& channelName) : name(channelName), canJoinEvent(-1), onJoinEvent(-1), onLeaveEvent(-1), onSpeakEvent(-1), id(channelId), publicChannel(false) {}
		virtual ~ChatChannel() {}

		bool addUser(Player& player);
		bool removeUser(const Player& player);

		bool talk(const Player& fromPlayer, SpeakClasses type, const std::string& text);
		void sendToAll(const std::string& message, SpeakClasses type);

		const std::string& getName() const {
			return name;
		}
		uint16_t getId() const {
			return id;
		}
		const UsersMap& getUsers() const {
			return users;
		}
		virtual const InvitedMap* getInvitedUsersPtr() const {
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

		int32_t canJoinEvent;
		int32_t onJoinEvent;
		int32_t onLeaveEvent;
		int32_t onSpeakEvent;

		uint16_t id;
		bool publicChannel;

	friend class Chat;
};

class PrivateChatChannel : public ChatChannel
{
	public:
		PrivateChatChannel(uint16_t channelId, const std::string& channelName);
		virtual ~PrivateChatChannel() {}

		virtual uint32_t getOwner() const {
			return m_owner;
		}
		void setOwner(uint32_t id) {
			m_owner = id;
		}

		bool isInvited(const Player& player) const;

		void invitePlayer(const Player& player, Player& invitePlayer);
		void excludePlayer(const Player& player, Player& excludePlayer);

		bool addInvited(Player& player);
		bool removeInvited(const Player& player);

		void closeChannel();

		const InvitedMap& getInvitedUsers() const {
			return m_invites;
		}

		const InvitedMap* getInvitedUsersPtr() const {
			return &m_invites;
		}

	protected:
		InvitedMap m_invites;
		uint32_t m_owner;
};

typedef std::list<ChatChannel*> ChannelList;

class Chat
{
	public:
		Chat();
		~Chat();

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
			return &m_scriptInterface;
		}

	private:
		std::map<uint16_t, ChatChannel> normalChannels;
		std::map<uint16_t, PrivateChatChannel*> privateChannels;
		std::map<Party*, ChatChannel*> partyChannels;
		std::map<uint32_t, ChatChannel*> guildChannels;

		LuaScriptInterface m_scriptInterface;

		ChatChannel* dummyPrivate;
};

#endif
