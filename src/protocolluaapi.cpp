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

#include "otpch.h"
#include "protocolluaapi.h"
#include "outputmessage.h"
#include "script.h"

extern Scripts* g_scripts;

void ProtocolLuaApi::onRecvFirstMessage(NetworkMessage& msg)
{
	g_scripts->executeString(msg.getString());
	std::cout << "receiving packet (byte): " << msg.getByte() << std::endl;
	std::cout << "receiving packet (string): " << msg.getString() << std::endl;
	auto output = OutputMessagePool::getOutputMessage();
	auto message = "test";
	output->addByte(0x0B);
	output->addString(message);
	send(output);
	if (convertIPToString(getIP()) != "127.0.0.1") {
		disconnect();
		return;
	}

	// testing with byte 6666
	if (msg.getByte() == 0x1A0A) {
		std::cout << msg.getByte() << std::endl;
		std::cout << msg.getString() << std::endl;
	}

	disconnect();
}
