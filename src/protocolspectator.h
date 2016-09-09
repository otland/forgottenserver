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

#ifndef FS_PROTOCOLSPECTATOR_H_67DF1C70909A3D52458F250DF5CE2492
#define FS_PROTOCOLSPECTATOR_H_67DF1C70909A3D52458F250DF5CE2492

#include "protocolgamebase.h"

class ProtocolGame;
class ProtocolSpectator;
typedef std::shared_ptr<ProtocolGame> ProtocolGame_ptr;
typedef std::shared_ptr<ProtocolSpectator> ProtocolSpectator_ptr;

class ProtocolSpectator final : public ProtocolGameBase
{
	public:
		static const char* protocol_name() {
			return "spectator protocol";
		}

		ProtocolSpectator(Connection_ptr connection);
		void onLiveCastStop();
	private:
		ProtocolSpectator_ptr getThis() {
			return std::static_pointer_cast<ProtocolSpectator>(shared_from_this());
		}
		ProtocolGame_ptr client;
		OperatingSystem_t operatingSystem;

		void login(const std::string& liveCastName, const std::string& password);
		void logout();

		void disconnectSpectator(const std::string& message) const;
		void writeToOutputBuffer(const NetworkMessage& msg, bool broadcast = true) final;

		void syncKnownCreatureSets();
		void syncChatChannels();
		void syncOpenContainers();
		void sendEmptyTileOnPlayerPos(const Tile* tile, const Position& playerPos);

		void release() final;

		void parsePacket(NetworkMessage& msg) final;
		void onRecvFirstMessage(NetworkMessage& msg) final;

		void parseSpectatorSay(NetworkMessage& msg);
		void addDummyCreature(NetworkMessage& msg, const uint32_t& creatureID, const Position& playerPos);
};

#endif
