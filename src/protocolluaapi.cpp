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
#include "globalevent.h"

extern Scripts* g_scripts;
extern ConfigManager g_config;
extern GlobalEvents* g_globalEvents;

/*
* List of Packets used for communication
* --------------------------------------------------------------------
* 
* Server ----> API (structure of packet)
* --------------------------------------------------------------------
* 100 => ping
* 101 => sending a callback message (string)
* 102 => sending lua error back to API (string)
* 103 => request to exchange lua code, if the API has any
* --------------------------------------------------------------------
* 
* API ----> Server (structure of packet)
* --------------------------------------------------------------------
* 100 => pong
* 101 => sending raw string lua code with immediate execution (string[name], string[data])
* 102 => save server
* 103 => clean server
* 104 => close server
* 105 => start raid (string)
* 200 => reload scripts
* --------------------------------------------------------------------
*/

void ProtocolLuaApi::onRecvFirstMessage(NetworkMessage& msg)
{
	// we only allow connection from the ip set in config.lua "luaApiIp = "xx.xxx.x.xx"" or from localhost.
	if (convertIPToString(getIP()) != g_config.getString(ConfigManager::LUA_API_IP) && convertIPToString(getIP()) != "127.0.0.1") {
		std::cout << "IP: " << convertIPToString(getIP()) << " tried to connect." << std::endl;
		disconnect();
		return;
	}

	auto recvbyte = msg.get<uint16_t>();
	auto name = msg.getString();

	switch (recvbyte) {
		case 100: {
			setResponse(true);
			g_dispatcher.addTask(createTask(std::bind(&ProtocolLuaApi::sendCallbackMessage, std::static_pointer_cast<ProtocolLuaApi>(shared_from_this()),
				"pong was successful")));
			return;
		}
		case 101: {
			auto data = msg.getString();
			auto returnvalue = g_scripts->executeString(data, name);
			if (!returnvalue.empty()) {
				g_dispatcher.addTask(createTask(std::bind(&ProtocolLuaApi::sendErrorMessage, std::static_pointer_cast<ProtocolLuaApi>(shared_from_this()),
					returnvalue)));
				return;
			}
			g_dispatcher.addTask(createTask(std::bind(&ProtocolLuaApi::sendCallbackMessage, std::static_pointer_cast<ProtocolLuaApi>(shared_from_this()),
				"successfully executed " + name)));
			return;
		}
		default:
			g_dispatcher.addTask(createTask(std::bind(&ProtocolLuaApi::parseHttpRequest, std::static_pointer_cast<ProtocolLuaApi>(shared_from_this()),
				name, msg)));
			g_dispatcher.addTask(createTask(std::bind(&ProtocolLuaApi::sendCallbackMessage, std::static_pointer_cast<ProtocolLuaApi>(shared_from_this()),
				"http request recieved")));
	}
}

void ProtocolLuaApi::parseHttpRequest(std::string& name, NetworkMessage& msg)
{
	std::unordered_map<std::string, std::string> dataMap;
	// now we get a byte which tells us how much http headers we have to parse
	auto headers = msg.get<uint16_t>();
	for (uint16_t i = 0; i < headers; i++) {
		// we get the header string
		auto headerString = msg.getString();
		// now we get the data as string
		auto dataString = msg.getString();
		// add everything to map
		dataMap.emplace(headerString, dataString);
	}

	/* We are searching if we can even find a registered GlobalEvent with the name
	*  This way we could push app names onward from the API to only execute the GlobalEvent which belongs to this app
	*/
	auto& http = g_globalEvents->getEventMap(GLOBALEVENT_HTTPREQUEST);
	if (http.find(name) != http.end()) {
		g_globalEvents->getEventMap(GLOBALEVENT_HTTPREQUEST).at(name).executeHttpRequest(name, dataMap);
	} else {
		std::cout << "[Error - ProtocolLuaApi::parseHttpRequest] Invalid name for global event: " << name << std::endl;
	}
}

// packet 100
void ProtocolLuaApi::sendPing()
{
	setResponse(false);
	auto output = OutputMessagePool::getOutputMessage();
	output->add<uint16_t>(100);
	send(output);
	disconnect();
}

// packet 101
void ProtocolLuaApi::sendCallbackMessage(const std::string& message)
{
	auto output = OutputMessagePool::getOutputMessage();
	output->add<uint16_t>(101);
	output->addString(message);
	send(output);
	disconnect();
}

// packet 102
void ProtocolLuaApi::sendErrorMessage(const std::string& error)
{
	auto output = OutputMessagePool::getOutputMessage();
	output->add<uint16_t>(102);
	output->addString(error);
	send(output);
	disconnect();
}

//packet 103
void ProtocolLuaApi::sendRequestFileExchange()
{
	auto output = OutputMessagePool::getOutputMessage();
	output->add<uint16_t>(103);
	send(output);
	disconnect();
}
