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

#include "otpch.h"

#include "protocollogin.h"

#include "outputmessage.h"
#include "connection.h"
#include "rsa.h"

#include "configmanager.h"
#include "tools.h"
#include "iologindata.h"
#include "ban.h"
#include <iomanip>
#include "game.h"

extern ConfigManager g_config;
extern IPList serverIPs;
extern Ban g_bans;
extern Game g_game;

#ifdef __ENABLE_SERVER_DIAGNOSTIC__
uint32_t ProtocolLogin::protocolLoginCount = 0;
#endif

void ProtocolLogin::disconnectClient(uint8_t error, const char* message)
{
	OutputMessage_ptr output = OutputMessagePool::getInstance()->getOutputMessage(this, false);
	if (output) {
		output->AddByte(error);
		output->AddString(message);
		OutputMessagePool::getInstance()->send(output);
	}

	getConnection()->closeConnection();
}

bool ProtocolLogin::parseFirstPacket(NetworkMessage& msg)
{
	if (g_game.getGameState() == GAME_STATE_SHUTDOWN) {
		getConnection()->closeConnection();
		return false;
	}

	uint32_t clientip = getConnection()->getIP();

	/*uint16_t clientos = */
	msg.GetU16();
	uint16_t version = msg.GetU16();

	if (version >= 971) {
		msg.SkipBytes(17);
	} else {
		msg.SkipBytes(12);
	}

	/*
	 * Skipped bytes:
	 * 4 bytes: protocolVersion (only 971+)
	 * 12 bytes: dat, spr, pic signatures (4 bytes each)
	 * 1 byte: 0 (only 971+)
	*/

	if (version <= 760) {
		disconnectClient(0x0A, "Only clients with protocol " CLIENT_VERSION_STR " allowed!");
		return false;
	}

	if (!RSA_decrypt(msg)) {
		getConnection()->closeConnection();
		return false;
	}

	uint32_t key[4];
	key[0] = msg.GetU32();
	key[1] = msg.GetU32();
	key[2] = msg.GetU32();
	key[3] = msg.GetU32();
	enableXTEAEncryption();
	setXTEAKey(key);

	std::string accountName = msg.GetString();
	std::string password = msg.GetString();

	if (version < CLIENT_VERSION_MIN || version > CLIENT_VERSION_MAX) {
		disconnectClient(0x0A, "Only clients with protocol " CLIENT_VERSION_STR " allowed!");
		return false;
	}

	if (g_game.getGameState() == GAME_STATE_STARTUP) {
		disconnectClient(0x0A, "Gameworld is starting up. Please wait.");
		return false;
	}

	if (g_game.getGameState() == GAME_STATE_MAINTAIN) {
		disconnectClient(0x0A, "Gameworld is under maintenance. Please re-connect in a while.");
		return false;
	}

	BanInfo banInfo;
	if (IOBan::getInstance()->isIpBanned(clientip, banInfo)) {
		if (banInfo.reason.empty()) {
			banInfo.reason = "(none)";
		}

		std::ostringstream ss;
		ss << "Your IP has been banned until " << formatDateShort(banInfo.expiresAt) << " by " << banInfo.bannedBy << ".\n\nReason specified:\n" << banInfo.reason;
		disconnectClient(0x0A, ss.str().c_str());
		return false;
	}

	uint32_t serverip = serverIPs[0].first;
	for (uint32_t i = 0; i < serverIPs.size(); i++) {
		if ((serverIPs[i].first & serverIPs[i].second) == (clientip & serverIPs[i].second)) {
			serverip = serverIPs[i].first;
			break;
		}
	}

	if (accountName.empty()) {
		disconnectClient(0x0A, "Invalid account name.");
		return false;
	}

	Account account;
	if (!IOLoginData::getInstance()->loginserverAuthentication(accountName, password, account)) {
		disconnectClient(0x0A, "Account name or password is not correct.");
		return false;
	}

	OutputMessage_ptr output = OutputMessagePool::getInstance()->getOutputMessage(this, false);
	if (output) {
		//Update premium days
		g_game.updatePremium(account);

		//Add MOTD
		output->AddByte(0x14);

		std::ostringstream ss;
		ss << g_game.getMotdNum() << "\n" << g_config.getString(ConfigManager::MOTD);
		output->AddString(ss.str());

		//Add char list
		output->AddByte(0x64);

		if (version > 1010) {
			output->AddByte(1); // number of worlds

			output->AddByte(0); // world id
			output->AddString(g_config.getString(ConfigManager::SERVER_NAME));
			output->AddString(g_config.getString(ConfigManager::IP));
			output->AddU16(g_config.getNumber(ConfigManager::GAME_PORT));
			output->AddByte(0);

			output->AddByte((uint8_t)account.charList.size());
			for (const std::string& characterName : account.charList) {
				output->AddByte(0);
				output->AddString(characterName);
			}
		} else {
			output->AddByte((uint8_t)account.charList.size());
			for (const std::string& characterName : account.charList) {
				output->AddString(characterName);
				output->AddString(g_config.getString(ConfigManager::SERVER_NAME));
				output->AddU32(serverip);
				output->AddU16(g_config.getNumber(ConfigManager::GAME_PORT));
				output->AddByte(0x00);
			}
		}

		//Add premium days
		if (g_config.getBoolean(ConfigManager::FREE_PREMIUM)) {
			output->AddU16(0xFFFF);    //client displays free premium
		} else {
			output->AddU16(account.premiumDays);
		}

		OutputMessagePool::getInstance()->send(output);
	}

	getConnection()->closeConnection();
	return true;
}

void ProtocolLogin::onRecvFirstMessage(NetworkMessage& msg)
{
	parseFirstPacket(msg);
}
