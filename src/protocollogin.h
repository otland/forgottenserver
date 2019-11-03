/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_PROTOCOLLOGIN_H_1238F4B473074DF2ABC595C29E81C46D
#define FS_PROTOCOLLOGIN_H_1238F4B473074DF2ABC595C29E81C46D

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

		explicit ProtocolLogin(Connection_ptr connection) : Protocol(connection) {}

		void onRecvFirstMessage(NetworkMessage& msg) override;

	private:
		void disconnectClient(const std::string& message, uint16_t version);

		void getCharacterList(const std::string& accountName, const std::string& password, const std::string& token, uint16_t version);
};

#endif
