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
* 101 => sending a callback message
* 102 => sending lua error back to API
* 103 => request to exchange lua code, if the API has any
* --------------------------------------------------------------------
* 
* API ----> Server (structure of packet)
* --------------------------------------------------------------------
* 100 => pong
* 101 => sending raw string lua code with immediate execution
* 102 => sending a http request
* --------------------------------------------------------------------
*/

/*
* Examples
*/

/*
* >>> Server ----> API
* ---------------------------------------------------------------------
* >> Ping (100)
*
* uint16_t <2 bytes> (100) recvbyte
* ---------------------------------------------------------------------
* >> Send callback message (101)
*
* uint16_t <2 bytes> (101) recvbyte
* string ("example")
* ---------------------------------------------------------------------
* >> Send error message (102)
*
* uint16_t <2 bytes> (102) recvbyte
* string ("error message")
* ---------------------------------------------------------------------
* >> Send exchange lua files
* 
* uint16_t <2 bytes> (103) recvbyte
* ---------------------------------------------------------------------
*
* >>> API ----> Server
* ---------------------------------------------------------------------
* >> Pong (100)
*
* uint8_t <1 byte> (200) protocol identifier
* uint16_t <2 bytes> (100) recvbyte
* ---------------------------------------------------------------------
* >> Immediatly execute string as lua chunk (101)
*
* uint8_t <1 byte> (200) protocol identifier
* uint16_t <2 bytes> (101) recvbyte
* string ("API") this would refer to the application which is sending the request, so we know where to send back the response
* string ("callback name") we need this incase the code errors out then we know exactly which code it was because of the name
* string ("print("test")") this would now print "test" to the console
* ----------------------------------------------------------------------
* >> http request (102) #1 example (single header & value)
* 
* uint8_t <1 byte> (200) protocol identifier
* uint16_t <2 bytes> (102) recvbyte
* string ("API") this would refer to the application which is sending the request, so we know where to send back the response
* string ("Test") this is important, it has to match the name of the GlobalEvent in order to know which file it has to parse
* uint16_t <2 bytes> (size of headers) in this case it would be 1
* string ("header") this is our header
* string ("value") this is our value to header
* ----------------------------------------------------------------------
* >> http request (102) #2 example (multiple header & value)
*
* uint8_t <1 byte> (200) protocol identifier
* uint16_t <2 bytes> (102) recvbyte
* string ("API") this would refer to the application which is sending the request, so we know where to send back the response
* string ("Test") this is important, it has to match the name of the GlobalEvent in order to know which file it has to parse
* uint16_t <2 bytes> (size of headers) in this case it would be 2
* string ("header1") this is our header1
* string ("value1") this is our value1 to header1
* string ("header2") this is our header2
* string ("value2") this is our value2 to header2
*
* a better explanation gives a look at the parseHttpRequest function a bit further down
*/

void ProtocolLuaApi::onRecvFirstMessage(NetworkMessage& msg)
{
	// we only allow connection from the ip set in config.lua "luaApiIp = "xx.xxx.x.xx"" or from localhost.
	if ((g_config.getString(ConfigManager::LUA_API_IP) != "127.0.0.1" && convertIPToString(getIP()) != g_config.getString(ConfigManager::LUA_API_IP)) || convertIPToString(getIP()) != "127.0.0.1") {
		std::cout << "IP: " << convertIPToString(getIP()) << " tried to connect." << std::endl;
		disconnect();
		return;
	}

	auto recvbyte = msg.get<uint16_t>();
	auto appName = msg.getString();

	switch (recvbyte) {
		// Ping & Pong to ensure that server or api is available and is responding
		case 100: {
			setResponse(true);
			g_dispatcher.addTask(createTask(std::bind(&ProtocolLuaApi::sendCallbackMessage, std::static_pointer_cast<ProtocolLuaApi>(shared_from_this()),
				appName, "pong was successful")));
			return;
		}
		// This will execute whatever data it gets through the string directly as lua code via "dostring" function
		case 101: {
			// We use fileName, to let the lua interface know how this file is named, incase it throws an error so we get a name to it.
			auto fileName = msg.getString();
			auto data = msg.getString();
			auto returnValue = g_scripts->executeString(data, fileName);
			if (!returnValue.empty()) {
				g_dispatcher.addTask(createTask(std::bind(&ProtocolLuaApi::sendErrorMessage, std::static_pointer_cast<ProtocolLuaApi>(shared_from_this()),
					appName, returnValue)));
				return;
			}
			g_dispatcher.addTask(createTask(std::bind(&ProtocolLuaApi::sendCallbackMessage, std::static_pointer_cast<ProtocolLuaApi>(shared_from_this()),
				appName, "successfully executed " + fileName)));
			return;
		}
		// This parses packets received as strings and then creates a lua table representating the data given from the http headers & values
		case 102: {
			auto eventName = msg.getString();
			g_dispatcher.addTask(createTask(std::bind(&ProtocolLuaApi::parseHttpRequest, std::static_pointer_cast<ProtocolLuaApi>(shared_from_this()),
				appName, eventName, msg)));
			g_dispatcher.addTask(createTask(std::bind(&ProtocolLuaApi::sendCallbackMessage, std::static_pointer_cast<ProtocolLuaApi>(shared_from_this()),
				appName, "http request received")));
			return;
		}
		default:
			g_dispatcher.addTask(createTask(std::bind(&ProtocolLuaApi::sendErrorMessage, std::static_pointer_cast<ProtocolLuaApi>(shared_from_this()),
				 appName, "recvbyte: " + std::to_string(recvbyte) + " does not exist!")));
	}
}

void ProtocolLuaApi::parseHttpRequest(std::string& appName, std::string& name, NetworkMessage& msg)
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
		http.find(name)->second.executeHttpRequest(name, dataMap);
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
void ProtocolLuaApi::sendCallbackMessage(const std::string& appName, const std::string& message)
{
	auto output = OutputMessagePool::getOutputMessage();
	output->add<uint16_t>(101);
	output->addString(appName);
	output->addString(message);
	send(output);
	disconnect();
}

// packet 102
void ProtocolLuaApi::sendErrorMessage(const std::string& appName, const std::string& error)
{
	auto output = OutputMessagePool::getOutputMessage();
	output->add<uint16_t>(102);
	output->addString(appName);
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
