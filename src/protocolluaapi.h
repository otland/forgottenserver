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

#ifndef FS_LUAAPI_H_
#define FS_LUAAPI_H_

#include "networkmessage.h"
#include "protocol.h"
#include "luascript.h"

class ProtocolLuaApi final : public Protocol
{
	public:
		// lua protocol API
		enum {server_sends_first = false};
		enum {protocol_identifier = 200};
		enum {use_checksum = false};
		static const char* protocol_name() {
			return "lua protocol";
		}

		explicit ProtocolLuaApi(Connection_ptr connection) : Protocol(connection) {}

		void onRecvFirstMessage(NetworkMessage& msg) override;

	private:
};

#endif
