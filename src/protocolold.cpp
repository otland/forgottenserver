// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "protocolold.h"

#include "game.h"
#include "outputmessage.h"
#include "tools.h"

extern Game g_game;

ProtocolMessage ProtocolOld::onRecvFirstMessage(NetworkMessage& msg)
{
	if (g_game.getGameState() == GAME_STATE_SHUTDOWN) {
		throw std::exception("Server is shutting down...");
	}

	/*uint16_t clientOS =*/msg.get<uint16_t>();
	version = msg.get<uint16_t>();
	msg.skipBytes(12);

	if (version <= 760) {
		return PROTOCOLMESSAGE_INVALID_PROTOCOL_VERSION;
	}

	if (!Protocol::RSA_decrypt(msg)) {
		throw std::exception("RSA decrypt fails...");
	}

	xtea::key key;
	key[0] = msg.get<uint32_t>();
	key[1] = msg.get<uint32_t>();
	key[2] = msg.get<uint32_t>();
	key[3] = msg.get<uint32_t>();
	enableXTEAEncryption();
	setXTEAKey(std::move(key));

	if (version <= 822) {
		setChecksumMode(CHECKSUM_DISABLED);
	}

	return PROTOCOLMESSAGE_INVALID_PROTOCOL_VERSION;
}
