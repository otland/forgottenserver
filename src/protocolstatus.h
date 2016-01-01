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

#ifndef FS_STATUS_H_8B28B354D65B4C0483E37AD1CA316EB4
#define FS_STATUS_H_8B28B354D65B4C0483E37AD1CA316EB4

#include "networkmessage.h"
#include "protocol.h"

class ProtocolStatus final : public Protocol
{
	public:
		// static protocol information
		enum {server_sends_first = false};
		enum {protocol_identifier = 0xFF};
		enum {use_checksum = false};
		static const char* protocol_name() {
			return "status protocol";
		}

		explicit ProtocolStatus(Connection_ptr connection) : Protocol(connection) {}

		void onRecvFirstMessage(NetworkMessage& msg) final;

		void sendStatusString();
		void sendInfo(uint16_t requestedInfo, const std::string& characterName);

		static const uint64_t start;

	protected:
		static std::map<uint32_t, int64_t> ipConnectMap;
};

#endif
