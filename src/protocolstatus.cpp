// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "protocolstatus.h"

#include "configmanager.h"
#include "game.h"
#include "outputmessage.h"
#include "tasks.h"

extern Game g_game;

std::map<Connection::Address, int64_t> ProtocolStatus::ipConnectMap;
const uint64_t ProtocolStatus::start = OTSYS_TIME();

enum RequestedInfo_t : uint16_t
{
	REQUEST_BASIC_SERVER_INFO = 1 << 0,
	REQUEST_OWNER_SERVER_INFO = 1 << 1,
	REQUEST_MISC_SERVER_INFO = 1 << 2,
	REQUEST_PLAYERS_INFO = 1 << 3,
	REQUEST_MAP_INFO = 1 << 4,
	REQUEST_EXT_PLAYERS_INFO = 1 << 5,
	REQUEST_PLAYER_STATUS_INFO = 1 << 6,
	REQUEST_SERVER_SOFTWARE_INFO = 1 << 7,
};

void ProtocolStatus::onRecvFirstMessage(NetworkMessage& msg)
{
	const static auto acceptorAddress = boost::asio::ip::make_address(getString(ConfigManager::IP));

	const auto& ip = getIP();

	if (!ip.is_loopback() && ip != acceptorAddress) {
		if (auto it = ipConnectMap.find(ip);
		    it != ipConnectMap.end() && (OTSYS_TIME() < (it->second + getNumber(ConfigManager::STATUSQUERY_TIMEOUT)))) {
			disconnect();
			return;
		}
	}

	ipConnectMap[ip] = OTSYS_TIME();

	switch (msg.getByte()) {
		// XML info protocol
		case 0xFF: {
			if (msg.getString(4) == "info") {
				g_dispatcher.addTask([thisPtr = std::static_pointer_cast<ProtocolStatus>(shared_from_this())]() {
					thisPtr->sendStatusString();
				});
				return;
			}
			break;
		}

		// Another ServerInfo protocol
		case 0x01: {
			uint16_t requestedInfo = msg.get<uint16_t>(); // only a Byte is necessary, though we could add new info here
			std::string characterName;
			if (requestedInfo & REQUEST_PLAYER_STATUS_INFO) {
				characterName = msg.getString();
			}
			g_dispatcher.addTask(
			    [=, thisPtr = std::static_pointer_cast<ProtocolStatus>(shared_from_this()),
			     characterName = std::move(characterName)]() { thisPtr->sendInfo(requestedInfo, characterName); });
			return;
		}

		default:
			break;
	}
	disconnect();
}

void ProtocolStatus::sendStatusString()
{
	auto output = tfs::net::make_output_message();

	setRawMessages(true);

	pugi::xml_document doc;

	pugi::xml_node decl = doc.prepend_child(pugi::node_declaration);
	decl.append_attribute("version") = "1.0";

	pugi::xml_node tsqp = doc.append_child("tsqp");
	tsqp.append_attribute("version") = "1.0";

	pugi::xml_node serverinfo = tsqp.append_child("serverinfo");
	uint64_t uptime = (OTSYS_TIME() - ProtocolStatus::start) / 1000;
	serverinfo.append_attribute("uptime") = std::to_string(uptime).c_str();
	serverinfo.append_attribute("ip") = getString(ConfigManager::IP).c_str();
	serverinfo.append_attribute("servername") = getString(ConfigManager::SERVER_NAME).c_str();
	serverinfo.append_attribute("port") = std::to_string(getNumber(ConfigManager::LOGIN_PORT)).c_str();
	serverinfo.append_attribute("location") = getString(ConfigManager::LOCATION).c_str();
	serverinfo.append_attribute("url") = getString(ConfigManager::URL).c_str();
	serverinfo.append_attribute("server") = STATUS_SERVER_NAME;
	serverinfo.append_attribute("version") = STATUS_SERVER_VERSION;
	serverinfo.append_attribute("client") = CLIENT_VERSION_STR;

	pugi::xml_node owner = tsqp.append_child("owner");
	owner.append_attribute("name") = getString(ConfigManager::OWNER_NAME).c_str();
	owner.append_attribute("email") = getString(ConfigManager::OWNER_EMAIL).c_str();

	pugi::xml_node players = tsqp.append_child("players");

	uint32_t reportableOnlinePlayerCount = 0;
	uint32_t maxPlayersPerIp = getNumber(ConfigManager::STATUS_COUNT_MAX_PLAYERS_PER_IP);
	if (maxPlayersPerIp > 0) {
		std::map<Connection::Address, uint32_t> playersPerIp;
		for (auto&& player : g_game.getPlayers() | tfs::views::lock_weak_ptrs | std::views::as_const) {
			if (!player->getIP().is_unspecified()) {
				++playersPerIp[player->getIP()];
			}
		}

		for (auto& p : playersPerIp | std::views::values) {
			reportableOnlinePlayerCount += std::min(p, maxPlayersPerIp);
		}
	} else {
		reportableOnlinePlayerCount = g_game.getPlayersOnline();
	}

	players.append_attribute("online") = std::to_string(reportableOnlinePlayerCount).c_str();
	players.append_attribute("max") = std::to_string(getNumber(ConfigManager::MAX_PLAYERS)).c_str();
	players.append_attribute("peak") = std::to_string(g_game.getPlayersRecord()).c_str();

	pugi::xml_node monsters = tsqp.append_child("monsters");
	monsters.append_attribute("total") = std::to_string(g_game.getMonstersOnline()).c_str();

	pugi::xml_node npcs = tsqp.append_child("npcs");
	npcs.append_attribute("total") = std::to_string(g_game.getNpcsOnline()).c_str();

	pugi::xml_node rates = tsqp.append_child("rates");
	rates.append_attribute("experience") = std::to_string(getNumber(ConfigManager::RATE_EXPERIENCE)).c_str();
	rates.append_attribute("skill") = std::to_string(getNumber(ConfigManager::RATE_SKILL)).c_str();
	rates.append_attribute("loot") = std::to_string(getNumber(ConfigManager::RATE_LOOT)).c_str();
	rates.append_attribute("magic") = std::to_string(getNumber(ConfigManager::RATE_MAGIC)).c_str();
	rates.append_attribute("spawn") = std::to_string(getNumber(ConfigManager::RATE_SPAWN)).c_str();

	pugi::xml_node map = tsqp.append_child("map");
	map.append_attribute("name") = getString(ConfigManager::MAP_NAME).c_str();
	map.append_attribute("author") = getString(ConfigManager::MAP_AUTHOR).c_str();

	uint32_t mapWidth, mapHeight;
	g_game.getMapDimensions(mapWidth, mapHeight);
	map.append_attribute("width") = std::to_string(mapWidth).c_str();
	map.append_attribute("height") = std::to_string(mapHeight).c_str();

	pugi::xml_node motd = tsqp.append_child("motd");
	motd.text() = "N/A";

	std::ostringstream ss;
	doc.save(ss, "", pugi::format_raw);

	std::string data = ss.str();
	output->addBytes(data.c_str(), data.size());
	send(output);
	disconnect();
}

void ProtocolStatus::sendInfo(uint16_t requestedInfo, const std::string& characterName)
{
	auto output = tfs::net::make_output_message();

	if (requestedInfo & REQUEST_BASIC_SERVER_INFO) {
		output->addByte(0x10);
		output->addString(getString(ConfigManager::SERVER_NAME));
		output->addString(getString(ConfigManager::IP));
		output->addString(std::to_string(getNumber(ConfigManager::LOGIN_PORT)));
	}

	if (requestedInfo & REQUEST_OWNER_SERVER_INFO) {
		output->addByte(0x11);
		output->addString(getString(ConfigManager::OWNER_NAME));
		output->addString(getString(ConfigManager::OWNER_EMAIL));
	}

	if (requestedInfo & REQUEST_MISC_SERVER_INFO) {
		output->addByte(0x12);
		output->addString("N/A"); // MOTD
		output->addString(getString(ConfigManager::LOCATION));
		output->addString(getString(ConfigManager::URL));
		output->add<uint64_t>((OTSYS_TIME() - ProtocolStatus::start) / 1000);
	}

	if (requestedInfo & REQUEST_PLAYERS_INFO) {
		output->addByte(0x20);
		output->add<uint32_t>(g_game.getPlayersOnline());
		output->add<uint32_t>(getNumber(ConfigManager::MAX_PLAYERS));
		output->add<uint32_t>(g_game.getPlayersRecord());
	}

	if (requestedInfo & REQUEST_MAP_INFO) {
		output->addByte(0x30);
		output->addString(getString(ConfigManager::MAP_NAME));
		output->addString(getString(ConfigManager::MAP_AUTHOR));
		uint32_t mapWidth, mapHeight;
		g_game.getMapDimensions(mapWidth, mapHeight);
		output->add<uint16_t>(mapWidth);
		output->add<uint16_t>(mapHeight);
	}

	if (requestedInfo & REQUEST_EXT_PLAYERS_INFO) {
		output->addByte(0x21); // players info - online players list

		const auto& players = g_game.getPlayers() | tfs::views::lock_weak_ptrs | std::ranges::to<std::vector>();
		output->add<uint32_t>(players.size());
		for (const auto& player : players) {
			output->addString(player->getName());
			output->add<uint32_t>(player->getLevel());
		}
	}

	if (requestedInfo & REQUEST_PLAYER_STATUS_INFO) {
		output->addByte(0x22); // players info - online status info of a player
		if (g_game.getPlayerByName(characterName)) {
			output->addByte(0x01);
		} else {
			output->addByte(0x00);
		}
	}

	if (requestedInfo & REQUEST_SERVER_SOFTWARE_INFO) {
		output->addByte(0x23); // server software info
		output->addString(STATUS_SERVER_NAME);
		output->addString(STATUS_SERVER_VERSION);
		output->addString(CLIENT_VERSION_STR);
	}
	send(output);
	disconnect();
}
