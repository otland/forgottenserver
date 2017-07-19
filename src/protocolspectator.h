/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2017  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_PROTOCOLSPECTATOR_H_FACA2A2D1A9348B78E8FD7E8003EBB87
#define FS_PROTOCOLSPECTATOR_H_FACA2A2D1A9348B78E8FD7E8003EBB87

#include "protocolgame.h"

extern Game g_game;

class ProtocolSpectator final : public ProtocolGameBase
{
	public:
		// static protocol information
		enum { server_sends_first = true };
		enum { protocol_identifier = 0 }; // Not required as we send first
		enum { use_checksum = true };
		static const char* protocol_name() {
			return "spectator protocol";
		}

		explicit ProtocolSpectator(Connection_ptr connection) : ProtocolGameBase(connection) {}

	private:
		ProtocolSpectator_ptr getThis() {
			return std::static_pointer_cast<ProtocolSpectator>(shared_from_this());
		}

		void login(const std::string& character, const std::string& password);
		void release() final;

		void disconnectClient(const std::string& message) const;
		void writeToOutputBuffer(const NetworkMessage& msg, bool = true) final;

		void parsePacket(NetworkMessage& msg) final;
		void onRecvFirstMessage(NetworkMessage& msg) final;

		// parse functions
		void parseSay(NetworkMessage& msg);
		void parseCloseChannel(NetworkMessage& msg);

		// send functions
		void sendCastChannel();

		// syncing functions
		void syncKnownCreatures();
		void syncChannels();
		void syncContainers();

		bool isBanned();
		bool isMuted();

		void reOpenCastChannel();

		friend class ProtocolGame;

		uint32_t lastSpeak = 0;

		std::string name; // spectator name
		ProtocolGame* casterProtocol = nullptr;
};

#endif
