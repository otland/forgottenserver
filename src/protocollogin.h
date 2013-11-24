/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
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

#ifndef PROTOCOLLOGIN_H
#define PROTOCOLLOGIN_H

#include "protocol.h"

class NetworkMessage;
class OutputMessage;

class ProtocolLogin : public Protocol
{
	public:
		// static protocol information
		enum {server_sends_first = false};
		enum {protocol_identifier = 0x01};
		enum {use_checksum = true};
		static const char* protocol_name() {
			return "login protocol";
		}

#ifdef ENABLE_SERVER_DIAGNOSTIC
		static uint32_t protocolLoginCount;
#endif
		ProtocolLogin(Connection_ptr connection) : Protocol(connection) {
#ifdef ENABLE_SERVER_DIAGNOSTIC
			protocolLoginCount++;
#endif
		}
		virtual ~ProtocolLogin() {
#ifdef ENABLE_SERVER_DIAGNOSTIC
			protocolLoginCount--;
#endif
		}

		virtual int32_t getProtocolId() {
			return 0x01;
		}

		virtual void onRecvFirstMessage(NetworkMessage& msg);

	protected:
		void disconnectClient(uint8_t error, const char* message);

		bool parseFirstPacket(NetworkMessage& msg);
};

#endif
