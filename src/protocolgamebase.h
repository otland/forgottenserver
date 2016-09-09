/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2015  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_PROTOCOLGAMEBASE_H_A28CB86652D0AF6760E43DCD9ACED40D
#define FS_PROTOCOLGAMEBASE_H_A28CB86652D0AF6760E43DCD9ACED40D

#include "protocol.h"
#include "creature.h"
#include "outfit.h"
#include "chat.h"

class NetworkMessage;
class Player;
class Game;
class House;
class Container;
class Tile;
class Connection;
class Quest;

/** \brief Contains methods and member variables common to both the game and spectator protocols
 */
class ProtocolGameBase : public Protocol {
	public:
		// static protocol information
		enum {server_sends_first = true};
		enum {protocol_identifier = 0}; // Not required as we send first
		enum {use_checksum = true};

	protected:
		explicit ProtocolGameBase(Connection_ptr connection):
			Protocol(connection),
			player(nullptr),
			eventConnect(0),
			version(CLIENT_VERSION_MIN),
			challengeTimestamp(0),
			challengeRandom(0),
			debugAssertSent(false),
			acceptPackets(false) {}

		virtual void writeToOutputBuffer(const NetworkMessage& msg, bool broadcast = true) = 0;
		void onConnect() final;

		void checkCreatureAsKnown(uint32_t id, bool& known, uint32_t& removedKnown);
		void AddCreature(NetworkMessage& msg, const Creature* creature, bool known, uint32_t remove);
		void AddPlayerStats(NetworkMessage& msg);
		void AddPlayerSkills(NetworkMessage& msg);
		void AddWorldLight(NetworkMessage& msg, const LightInfo& lightInfo);
		void AddCreatureLight(NetworkMessage& msg, const Creature* creature);
		void AddOutfit(NetworkMessage& msg, const Outfit_t& outfit);

		// translate a tile to clientreadable format
		void GetTileDescription(const Tile* tile, NetworkMessage& msg);
		// translate a floor to clientreadable format
		void GetFloorDescription(NetworkMessage& msg, int32_t x, int32_t y, int32_t z,
		                         int32_t width, int32_t height, int32_t offset, int32_t& skip);
		// translate a map area to clientreadable format
		void GetMapDescription(int32_t x, int32_t y, int32_t z,
		                       int32_t width, int32_t height, NetworkMessage& msg);

		static void RemoveTileThing(NetworkMessage& msg, const Position& pos, uint32_t stackpos);

		void sendUpdateTile(const Tile* tile, const Position& pos);
		void sendContainer(uint8_t cid, const Container* container, bool hasParent, uint16_t firstIndex);
		void sendChannel(uint16_t channelId, const std::string& channelName, const UsersMap* channelUsers, const InvitedMap* invitedUsers);
		void sendAddCreature(const Creature* creature, const Position& pos, int32_t stackpos, bool isLogin);
		void sendMagicEffect(const Position& pos, uint8_t type);
		void sendStats();
		void sendBasicData();
		void sendPendingStateEntered();
		void sendEnterWorld();
		//inventory
		void sendInventoryItem(slots_t slot, const Item* item);

		void sendSkills();

		void sendCreatureLight(const Creature* creature);
		void sendWorldLight(const LightInfo& lightInfo);
		void sendMapDescription(const Position& pos);

		void sendVIP(uint32_t guid, const std::string& name, const std::string& description, uint32_t icon, bool notify, VipStatus_t status);
		void sendCancelWalk();

		void sendPing();
		void sendPingBack();

		bool canSee(int32_t x, int32_t y, int32_t z) const;
		bool canSee(const Creature*) const;
		bool canSee(const Position& pos) const;

		Player* player;
		uint32_t eventConnect;
		uint16_t version;

		uint32_t challengeTimestamp;
		uint8_t challengeRandom;

		bool debugAssertSent;
		bool acceptPackets;

		std::unordered_set<uint32_t> knownCreatureSet;
};
#endif
