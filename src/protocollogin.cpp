/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2015  Mark Samman <mark.samman@gmail.com>
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
#include "tasks.h"

#include "configmanager.h"
#include "tools.h"
#include "iologindata.h"
#include "ban.h"
#include "game.h"

extern ConfigManager g_config;
extern Game g_game;

void ProtocolLogin::disconnectClient(uint16_t version, const std::string& message)
{
	OutputMessage_ptr output = OutputMessagePool::getInstance()->getOutputMessage(this, false);
	if (output) {
		if (version >= 1076) {
			output->AddByte(0x0B);
			output->AddString(message);
			OutputMessagePool::getInstance()->send(output);
		} else {
			output->AddByte(0x0A);
			output->AddString(message);
			OutputMessagePool::getInstance()->send(output);
		}
	}

	getConnection()->closeConnection();
}

void ProtocolLogin::getCharacterList(const std::string& accountName, const std::string& password, uint16_t version)
{
	Account account;
	if (!IOLoginData::loginserverAuthentication(accountName, password, account)) {
		disconnectClient(version, "Account name or password is not correct.");
		return;
	}

	OutputMessage_ptr output = OutputMessagePool::getInstance()->getOutputMessage(this, false);
	if (output) {
		//Update premium days
		Game::updatePremium(account);

		//Add MOTD
		output->AddByte(0x14);

		std::ostringstream ss;
		ss << g_game.getMotdNum() << "\n" << g_config.getString(ConfigManager::MOTD);
		output->AddString(ss.str());

		//SessionKey
		output->AddByte(0x28);

		output->AddString(accountName + "\n" + password);

		//Add char list
		output->AddByte(0x64);

		output->AddByte(1); // number of worlds

		output->AddByte(0); // world id
		output->AddString(g_config.getString(ConfigManager::SERVER_NAME));
		output->AddString(g_config.getString(ConfigManager::IP));
		output->Add<uint16_t>(g_config.getNumber(ConfigManager::GAME_PORT));
		output->AddByte(0);

		output->AddByte(static_cast<uint8_t>(account.charList.size()));
		for (const std::string& characterName : account.charList) {
			output->AddByte(0);
			output->AddString(characterName);
		}

		//Add premium days
		if (g_config.getBoolean(ConfigManager::FREE_PREMIUM)) {
			output->Add<uint16_t>(0xFFFF);    //client displays free premium
		} else {
			output->Add<uint16_t>(account.premiumDays);
		}

		OutputMessagePool::getInstance()->send(output);
	}

	getConnection()->closeConnection();
}

void ProtocolLogin::onRecvFirstMessage(NetworkMessage& msg)
{
	if (g_game.getGameState() == GAME_STATE_SHUTDOWN) {
		getConnection()->closeConnection();
		return;
	}

	uint32_t clientip = getConnection()->getIP();

	/*uint16_t clientos = */
	msg.get<uint16_t>();
	uint16_t version = msg.get<uint16_t>();

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

#define dispatchDisconnectClient(ver, err) g_dispatcher.addTask(createTask(std::bind(&ProtocolLogin::disconnectClient, this, ver, err)))

	if (version <= 760) {
		dispatchDisconnectClient(version, "Only clients with protocol " CLIENT_VERSION_STR " allowed!");
		return;
	}

	if (!RSA_decrypt(msg)) {
		getConnection()->closeConnection();
		return;
	}

	uint32_t key[4];
	key[0] = msg.get<uint32_t>();
	key[1] = msg.get<uint32_t>();
	key[2] = msg.get<uint32_t>();
	key[3] = msg.get<uint32_t>();
	enableXTEAEncryption();
	setXTEAKey(key);

	std::string accountName = msg.GetString();
	std::string password = msg.GetString();

	if (version < CLIENT_VERSION_MIN || version > CLIENT_VERSION_MAX) {
		dispatchDisconnectClient(version, "Only clients with protocol " CLIENT_VERSION_STR " allowed!");
		return;
	}

	if (g_game.getGameState() == GAME_STATE_STARTUP) {
		dispatchDisconnectClient(version, "Gameworld is starting up. Please wait.");
		return;
	}

	if (g_game.getGameState() == GAME_STATE_MAINTAIN) {
		dispatchDisconnectClient(version, "Gameworld is under maintenance. Please re-connect in a while.");
		return;
	}

	BanInfo banInfo;
	if (IOBan::isIpBanned(clientip, banInfo)) {
		if (banInfo.reason.empty()) {
			banInfo.reason = "(none)";
		}

		std::ostringstream ss;
		ss << "Your IP has been banned until " << formatDateShort(banInfo.expiresAt) << " by " << banInfo.bannedBy << ".\n\nReason specified:\n" << banInfo.reason;
		dispatchDisconnectClient(version, ss.str());
		return;
	}

	if (accountName.empty()) {
		dispatchDisconnectClient(version, "Invalid account name.");
		return;
	}

#undef dispatchDisconnectClient

	g_dispatcher.addTask(createTask(std::bind(&ProtocolLogin::getCharacterList, this, accountName, password, version)));
}
