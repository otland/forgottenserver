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

#ifndef FS_PROTOCOLOLD_H_5487B862FE144AE0904D098A3238E161
#define FS_PROTOCOLOLD_H_5487B862FE144AE0904D098A3238E161

#include "protocol.h"

class NetworkMessage;
class OutputMessage;

class ProtocolOld : public Protocol
{
	public:
		// static protocol information
		enum {server_sends_first = false};
		enum {use_checksum = false};

		ProtocolOld(Connection_ptr connection) : Protocol(connection) {}

		virtual ~ProtocolOld() {}

		virtual void onRecvFirstMessage(NetworkMessage& msg);

	protected:
		void dispatchedDisconnectClient(const std::string& message);
		void disconnectClient(const std::string& message);
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
