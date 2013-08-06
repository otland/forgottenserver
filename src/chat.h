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

#ifndef __OTSERV_CHAT_H__
#define __OTSERV_CHAT_H__

#include <map>
#include <list>
#include <string>

#include "const.h"
#include "definitions.h"
#include "party.h"

class Player;

typedef std::map<uint32_t, Player*> UsersMap;
typedef std::map<uint32_t, Player*> InvitedMap;

class CanJoinChannelEvent : public Event
{
	public:
		CanJoinChannelEvent();

		bool execute(const Player& player);

		bool configureEvent(xmlNodePtr p) {
			return false;
		}

	protected:
		std::string getScriptEventName() {
			return "canJoin";
		}
};

class OnJoinChannelEvent : public Event
{
	public:
		OnJoinChannelEvent();

		bool execute(const Player& player);

		bool configureEvent(xmlNodePtr p) {
			return false;
		}

	protected:
		std::string getScriptEventName() {
			return "onJoin";
		}
};

class OnLeaveChannelEvent : public Event
{
	public:
		OnLeaveChannelEvent();

		bool execute(const Player& player);

		bool configureEvent(xmlNodePtr p) {
			return false;
		}

	protected:
		std::string getScriptEventName() {
			return "onLeave";
		}
};

class OnSpeakChannelEvent : public Event
{
	public:
		OnSpeakChannelEvent();

		bool execute(const Player& player, SpeakClasses& type, const std::string& message);

		bool configureEvent(xmlNodePtr p) {
			return false;
		}

	protected:
		std::string getScriptEventName() {
			return "onSpeak";
		}
};

class ChatChannel
{
	public:
		ChatChannel();
		ChatChannel(uint16_t channelId, const std::string& channelName);
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
			return NULL;
		}

		virtual uint32_t getOwner() const {
			return 0;
		}

		bool isPublicChannel() const { return publicChannel; }

	protected:
		UsersMap users;

		CanJoinChannelEvent* canJoinEvent;
		OnJoinChannelEvent* onJoinEvent;
		OnLeaveChannelEvent* onLeaveEvent;
		OnSpeakChannelEvent* onSpeakEvent;

		std::string name;
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
		virtual ~Chat();

		bool load();

		ChatChannel* createChannel(const Player& player, uint16_t channelId);
		bool deleteChannel(const Player& player, uint16_t channelId);

		ChatChannel* addUserToChannel(Player& player, uint16_t channelId);
		bool removeUserFromChannel(const Player& player, uint16_t channelId);
		void removeUserFromAllChannels(const Player& player);

		bool talkToChannel(const Player& player, SpeakClasses type, const std::string& text, uint16_t channelId);

		std::string getChannelName(const Player& player, uint16_t channelId);
		ChannelList getChannelList(const Player& player);

		ChatChannel* getChannel(const Player& player, uint16_t channelId);
		ChatChannel* getChannelById(uint16_t channelId);
		ChatChannel* getGuildChannelById(uint32_t guildId);
		PrivateChatChannel* getPrivateChannel(const Player& player);

		LuaScriptInterface* getScriptInterface() {
			return &m_scriptInterface;
		}

	private:
		typedef std::map<uint16_t, ChatChannel> NormalChannelMap;
		typedef std::map<uint16_t, PrivateChatChannel*> PrivateChannelMap;
		typedef std::map<Party*, ChatChannel*> PartyChannelMap;
		typedef std::map<uint32_t, ChatChannel*> GuildChannelMap;
		NormalChannelMap normalChannels;
		PrivateChannelMap privateChannels;
		PartyChannelMap partyChannels;
		GuildChannelMap guildChannels;

		LuaScriptInterface m_scriptInterface;

		ChatChannel* dummyPrivate;
};

#endif
