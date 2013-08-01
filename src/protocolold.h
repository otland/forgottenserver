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

#ifndef __OTSERV_PROTOCOL_OLD_H__
#define __OTSERV_PROTOCOL_OLD_H__

#include "protocol.h"

class NetworkMessage;
class OutputMessage;

class ProtocolOld : public Protocol
{
	public:
		// static protocol information
		enum {server_sends_first = false};
		enum {use_checksum = false};

#ifdef __ENABLE_SERVER_DIAGNOSTIC__
		static uint32_t protocolOldCount;
#endif

		ProtocolOld(Connection_ptr connection) : Protocol(connection) {
#ifdef __ENABLE_SERVER_DIAGNOSTIC__
			protocolOldCount++;
#endif
		}

		virtual ~ProtocolOld() {
#ifdef __ENABLE_SERVER_DIAGNOSTIC__
			protocolOldCount--;
#endif
		}

		virtual void onRecvFirstMessage(NetworkMessage& msg);

	protected:
		void disconnectClient(uint8_t error, const char* message);

		bool parseFirstPacket(NetworkMessage& msg);
};

class ProtocolOldLogin : public ProtocolOld
{
	public:
		enum {protocol_identifier = 0x01};
		static const char* protocol_name() {
			return "old login protocol";
		}

		ProtocolOldLogin(Connection_ptr connection) : ProtocolOld(connection) {}
};

class ProtocolOldGame : public ProtocolOld
{
	public:
		enum {protocol_identifier = 0x0A};
		static const char* protocol_name() {
			return "old gameworld protocol";
		}

		ProtocolOldGame(Connection_ptr connection) : ProtocolOld(connection) {}
};

#endif
