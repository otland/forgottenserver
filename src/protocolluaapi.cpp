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
#include "configmanager.h"
#include "game.h"

extern Scripts* g_scripts;
extern ConfigManager g_config;

/*
* List of Packets used for communication
* --------------------------------------------------------------------
* 
* Server ----> API (structure of packet)
* --------------------------------------------------------------------
* 100 => sending a callback message (string)
* 101 => sending lua error back to API (string)
* 102 => request to exchange lua code, if the API has any
* --------------------------------------------------------------------
* 
* API ----> Server (structure of packet)
* --------------------------------------------------------------------
* 100 => sending raw string lua code with immediate execution (string)
* 101 => force reload scripts
* 102 => save server
* 103 => restart server
* 104 => close server
* 105 => start raid (string)
* --------------------------------------------------------------------
*/

void ProtocolLuaApi::onRecvFirstMessage(NetworkMessage& msg)
{
	// we only allow connection from the ip set in config.lua "luaApiIp = "xx.xxx.x.xx"" or from localhost.
	if (convertIPToString(getIP()) != g_config.getString(ConfigManager::LUA_API_IP) || convertIPToString(getIP()) != "127.0.0.1") {
		std::cout << "IP: " << convertIPToString(getIP()) << " tried to connect." << std::endl;
		disconnect();
		return;
	}

	auto recvbyte = msg.get<uint16_t>();

	switch (recvbyte) {
		case 100: {
			std::string text = msg.getString();
			std::string& returnvalue = g_scripts->executeString(text);
			if (!returnvalue.empty()) {
				g_dispatcher.addTask(createTask(std::bind(&ProtocolLuaApi::sendErrorMessage, std::static_pointer_cast<ProtocolLuaApi>(shared_from_this()),
					returnvalue)));
				return;
			}
			g_dispatcher.addTask(createTask(std::bind(&ProtocolLuaApi::sendCallbackMessage, std::static_pointer_cast<ProtocolLuaApi>(shared_from_this()),
				"successfully executed lua code.")));
			return;
		}
		case 101: {
			g_scripts->executeString("EventCallbackData = {}; for i = 1, EVENT_CALLBACK_LAST do EventCallbackData[i] = {} end; Game.reload(RELOAD_TYPE_SCRIPTS)");
			g_dispatcher.addTask(createTask(std::bind(&ProtocolLuaApi::sendCallbackMessage, std::static_pointer_cast<ProtocolLuaApi>(shared_from_this()),
				"successfully reloaded scripts.")));
			return;
		}
		case 102: {
			g_scripts->executeString("EventCallbackData = {}; for i = 1, EVENT_CALLBACK_LAST do EventCallbackData[i] = {} end; Game.reload(RELOAD_TYPE_SCRIPTS)");
			g_dispatcher.addTask(createTask(std::bind(&ProtocolLuaApi::sendCallbackMessage, std::static_pointer_cast<ProtocolLuaApi>(shared_from_this()),
				"successfully reloaded scripts.")));
			return;
		}

		default:
			std::cout << "unhandled packet: " << recvbyte << std::endl;
			disconnect();
			return;
	}
}

// packet 100
void ProtocolLuaApi::sendCallbackMessage(const std::string& message)
{
	auto output = OutputMessagePool::getOutputMessage();
	output->add<uint16_t>(100);
	output->addString(message);
	send(output);
	disconnect();
}

// packet 101
void ProtocolLuaApi::sendErrorMessage(const std::string& error)
{
	auto output = OutputMessagePool::getOutputMessage();
	output->add<uint16_t>(101);
	output->addString(error);
	send(output);
	disconnect();
}

//packet 102
void ProtocolLuaApi::sendRequestFileExchange()
{
	auto output = OutputMessagePool::getOutputMessage();
	output->add<uint16_t>(102);
	send(output);
	disconnect();
}
