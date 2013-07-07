//////////////////////////////////////////////////////////////////////
// The Forgotten Server - a server application for the MMORPG Tibia
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

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

class ChatChannel
{
	public:
		ChatChannel(uint16_t channelId, const std::string& channelName);
		virtual ~ChatChannel() {}

		bool addUser(Player* player);
		bool removeUser(Player* player);

		bool talk(Player* fromPlayer, SpeakClasses type, const std::string& text);
		void sendToAll(const std::string& message, SpeakClasses type);

		const std::string& getName() const {
			return m_name;
		}
		uint16_t getId() const {
			return m_id;
		}
		const UsersMap& getUsers() const {
			return m_users;
		}

		virtual uint32_t getOwner() {
			return 0;
		}

	protected:
		UsersMap m_users;
		std::string m_name;
		uint16_t m_id;
};

class PrivateChatChannel : public ChatChannel
{
	public:
		PrivateChatChannel(uint16_t channelId, const std::string& channelName);
		virtual ~PrivateChatChannel() {}

		virtual uint32_t getOwner() {
			return m_owner;
		}
		void setOwner(uint32_t id) {
			m_owner = id;
		}

		bool isInvited(const Player* player);

		void invitePlayer(Player* player, Player* invitePlayer);
		void excludePlayer(Player* player, Player* excludePlayer);

		bool addInvited(Player* player);
		bool removeInvited(Player* player);

		void closeChannel();

		const InvitedMap& getInvitedUsers() {
			return m_invites;
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
		ChatChannel* createChannel(Player* player, uint16_t channelId);
		bool deleteChannel(Player* player, uint16_t channelId);

		ChatChannel* addUserToChannel(Player* player, uint16_t channelId);
		bool removeUserFromChannel(Player* player, uint16_t channelId);
		void removeUserFromAllChannels(Player* player);

		bool talkToChannel(Player* player, SpeakClasses type, const std::string& text, uint16_t channelId);

		std::string getChannelName(Player* player, uint16_t channelId);
		ChannelList getChannelList(Player* player);

		ChatChannel* getChannel(Player* player, uint16_t channelId);
		ChatChannel* getChannelById(uint16_t channelId);
		ChatChannel* getGuildChannelById(uint32_t guildId);
		PrivateChatChannel* getPrivateChannel(Player* player);

	private:
		typedef std::map<uint16_t, ChatChannel*> NormalChannelMap;
		typedef std::map<uint16_t, PrivateChatChannel*> PrivateChannelMap;
		typedef std::map<Party*, ChatChannel*> PartyChannelMap;
		typedef std::map<uint32_t, ChatChannel*> GuildChannelMap;
		NormalChannelMap m_normalChannels;
		PrivateChannelMap m_privateChannels;
		PartyChannelMap m_partyChannels;
		GuildChannelMap m_guildChannels;

		ChatChannel* dummyPrivate;
};

#endif
