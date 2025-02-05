// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "protocollogin.h"

#include "ban.h"
#include "base64.h"
#include "configmanager.h"
#include "game.h"
#include "iologindata.h"
#include "outputmessage.h"
#include "rsa.h"
#include "tasks.h"

extern Game g_game;

namespace {

std::string decodeSecret(std::string_view secret)
{
	// simple base32 decoding
	std::string key;
	key.reserve(10);

	uint32_t buffer = 0, left = 0;
	for (const auto& ch : secret) {
		buffer <<= 5;
		if (ch >= 'A' && ch <= 'Z') {
			buffer |= (ch & 0x1F) - 1;
		} else if (ch >= '2' && ch <= '7') {
			buffer |= ch - 24;
		} else {
			// if a key is broken, return empty and the comparison will always be false since the token must not be
			// empty
			return {};
		}

		left += 5;
		if (left >= 8) {
			left -= 8;
			key.push_back(static_cast<char>(buffer >> left));
		}
	}

	return key;
}

} // namespace

void ProtocolLogin::disconnectClient(const std::string& message, uint16_t version)
{
	auto output = tfs::net::make_output_message();
	output->addByte(version >= 1076 ? 0x0B : 0x0A);
	output->addString(message);
	send(output);

	disconnect();
}

void ProtocolLogin::getCharacterList(const std::string& accountName, const std::string& password,
                                     const std::string& token, uint16_t version)
{
	Database& db = Database::getInstance();

	DBResult_ptr result = db.storeQuery(fmt::format(
	    "SELECT `id`, UNHEX(`password`) AS `password`, `secret`, `premium_ends_at` FROM `accounts` WHERE `name` = {:s} OR `email` = {:s}",
	    db.escapeString(accountName), db.escapeString(accountName)));
	if (!result) {
		disconnectClient("Account name or password is not correct.", version);
		return;
	}

	if (transformToSHA1(password) != result->getString("password")) {
		disconnectClient("Account name or password is not correct.", version);
		return;
	}

	auto id = result->getNumber<uint32_t>("id");
	auto key = decodeSecret(result->getString("secret"));
	auto premiumEndsAt = result->getNumber<time_t>("premium_ends_at");

	std::vector<std::string> characters = {};
	result = db.storeQuery(fmt::format(
	    "SELECT `name` FROM `players` WHERE `account_id` = {:d} AND `deletion` = 0 ORDER BY `name` ASC", id));
	if (result) {
		do {
			characters.emplace_back(result->getString("name"));
		} while (result->next());
	}

	uint32_t ticks = duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count() /
	                 AUTHENTICATOR_PERIOD;

	auto output = tfs::net::make_output_message();
	if (!key.empty()) {
		if (token.empty() || !(token == generateToken(key, ticks) || token == generateToken(key, ticks - 1) ||
		                       token == generateToken(key, ticks + 1))) {
			output->addByte(0x0D);
			output->addByte(0);
			send(output);
			disconnect();
			return;
		}
		output->addByte(0x0C);
		output->addByte(0);
	}

	// Generate and add session key
	static std::independent_bits_engine<std::default_random_engine, CHAR_BIT, unsigned short> rbe;
	std::string sessionKey(16, '\x00');
	std::generate(sessionKey.begin(), sessionKey.end(), std::ref(rbe));

	output->addByte(0x28);
	output->addString(tfs::base64::encode({sessionKey.data(), sessionKey.size()}));

	if (!db.executeQuery(
	        fmt::format("INSERT INTO `sessions` (`token`, `account_id`, `ip`) VALUES ({:s}, {:d}, INET6_ATON({:s}))",
	                    db.escapeBlob(sessionKey.data(), sessionKey.size()), id,
	                    db.escapeString(getConnection()->getIP().to_string())))) {
		disconnectClient("Failed to create session.\nPlease try again later.", version);
		return;
	}

	// Add char list
	output->addByte(0x64);

	uint8_t size = std::min<size_t>(std::numeric_limits<uint8_t>::max(), characters.size());

	if (getBoolean(ConfigManager::ONLINE_OFFLINE_CHARLIST)) {
		output->addByte(2); // number of worlds

		for (uint8_t i = 0; i < 2; i++) {
			output->addByte(i); // world id
			output->addString(i == 0 ? "Offline" : "Online");
			output->addString(getString(ConfigManager::IP));
			output->add<uint16_t>(getNumber(ConfigManager::GAME_PORT));
			output->addByte(0);
		}
	} else {
		output->addByte(1); // number of worlds
		output->addByte(0); // world id
		output->addString(getString(ConfigManager::SERVER_NAME));
		output->addString(getString(ConfigManager::IP));
		output->add<uint16_t>(getNumber(ConfigManager::GAME_PORT));
		output->addByte(0);
	}

	output->addByte(size);
	for (uint8_t i = 0; i < size; i++) {
		const auto& character = characters[i];
		if (getBoolean(ConfigManager::ONLINE_OFFLINE_CHARLIST)) {
			output->addByte(g_game.getPlayerByName(character) ? 1 : 0);
		} else {
			output->addByte(0);
		}
		output->addString(character);
	}

	// Add premium days
	output->addByte(0);
	if (getBoolean(ConfigManager::FREE_PREMIUM)) {
		output->addByte(1);
		output->add<uint32_t>(0);
	} else {
		output->addByte(premiumEndsAt > time(nullptr) ? 1 : 0);
		output->add<uint32_t>(premiumEndsAt);
	}

	send(output);

	disconnect();
}

// Character list request
void ProtocolLogin::onRecvFirstMessage(NetworkMessage& msg)
{
	if (g_game.getGameState() == GAME_STATE_SHUTDOWN) {
		disconnect();
		return;
	}

	msg.skipBytes(2); // client OS

	uint16_t version = msg.get<uint16_t>();
	if (version <= 822) {
		setChecksumMode(CHECKSUM_DISABLED);
	}

	if (version <= 760) {
		disconnectClient(fmt::format("Only clients with protocol {:s} allowed!", CLIENT_VERSION_STR), version);
		return;
	}

	if (version >= 971) {
		msg.skipBytes(17);
	} else {
		msg.skipBytes(12);
	}
	/*
	 * Skipped bytes:
	 * 4 bytes: protocolVersion
	 * 12 bytes: dat, spr, pic signatures (4 bytes each)
	 * 1 byte: 0
	 */

	if (!Protocol::RSA_decrypt(msg)) {
		disconnect();
		return;
	}

	xtea::key key;
	key[0] = msg.get<uint32_t>();
	key[1] = msg.get<uint32_t>();
	key[2] = msg.get<uint32_t>();
	key[3] = msg.get<uint32_t>();
	enableXTEAEncryption();
	setXTEAKey(std::move(key));

	if (version < CLIENT_VERSION_MIN || version > CLIENT_VERSION_MAX) {
		disconnectClient(fmt::format("Only clients with protocol {:s} allowed!", CLIENT_VERSION_STR), version);
		return;
	}

	if (g_game.getGameState() == GAME_STATE_STARTUP) {
		disconnectClient("Gameworld is starting up. Please wait.", version);
		return;
	}

	if (g_game.getGameState() == GAME_STATE_MAINTAIN) {
		disconnectClient("Gameworld is under maintenance.\nPlease re-connect in a while.", version);
		return;
	}

	auto connection = getConnection();
	if (!connection) {
		return;
	}

	if (const auto& banInfo = IOBan::getIpBanInfo(connection->getIP())) {
		disconnectClient(fmt::format("Your IP has been banned until {:s} by {:s}.\n\nReason specified:\n{:s}",
		                             formatDateShort(banInfo->expiresAt), banInfo->bannedBy, banInfo->reason),
		                 version);
		return;
	}

	auto accountName = msg.getString();
	if (accountName.empty()) {
		disconnectClient("Invalid account name.", version);
		return;
	}

	auto password = msg.getString();
	if (password.empty()) {
		disconnectClient("Invalid password.", version);
		return;
	}

	// read authenticator token and stay logged in flag from last bytes
	msg.skipBytes(msg.getRemainingBufferLength() - Protocol::RSA_BUFFER_LENGTH);
	if (!Protocol::RSA_decrypt(msg)) {
		disconnectClient("Invalid authentication token.", version);
		return;
	}

	auto authToken = msg.getString();

	g_dispatcher.addTask([=, thisPtr = std::static_pointer_cast<ProtocolLogin>(shared_from_this()),
	                      accountName = std::string{accountName}, password = std::string{password},
	                      authToken = std::string{authToken}]() {
		thisPtr->getCharacterList(accountName, password, authToken, version);
	});
}
