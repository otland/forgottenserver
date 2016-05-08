/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2016  Mark Samman <mark.samman@gmail.com>
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

#include "protocol.h"
#include "outputmessage.h"
#include "rsa.h"

extern RSA g_RSA;

void Protocol::onSendMessage(const OutputMessage_ptr& msg) const
{
	if (!rawMessages) {
		msg->writeMessageLength();

		if (encryptionEnabled) {
			XTEA_encrypt(*msg);
			msg->addCryptoHeader(checksumEnabled);
		}
	}
}

void Protocol::onRecvMessage(NetworkMessage& msg)
{
	if (encryptionEnabled && !XTEA_decrypt(msg)) {
		return;
	}

	parsePacket(msg);
}

OutputMessage_ptr Protocol::getOutputBuffer(int32_t size)
{
	//dispatcher thread
	if (!outputBuffer) {
		outputBuffer = OutputMessagePool::getOutputMessage();
	} else if ((outputBuffer->getLength() + size) > NetworkMessage::MAX_PROTOCOL_BODY_LENGTH) {
		send(outputBuffer);
		outputBuffer = OutputMessagePool::getOutputMessage();
	}
	return outputBuffer;
}

void Protocol::XTEA_encrypt(OutputMessage& msg) const
{
	const uint32_t delta = 0x61C88647;

	// The message must be a multiple of 8
	size_t paddingBytes = msg.getLength() % 8;
	if (paddingBytes != 0) {
		msg.addPaddingBytes(8 - paddingBytes);
	}

	uint8_t* buffer = msg.getOutputBuffer();
	const size_t messageLength = msg.getLength();
	size_t readPos = 0;
	const uint32_t k[] = {key[0], key[1], key[2], key[3]};
	while (readPos < messageLength) {
		uint32_t v0;
		memcpy(&v0, buffer + readPos, 4);
		uint32_t v1;
		memcpy(&v1, buffer + readPos + 4, 4);

		uint32_t sum = 0;

		for (int32_t i = 32; --i >= 0;) {
			v0 += ((v1 << 4 ^ v1 >> 5) + v1) ^ (sum + k[sum & 3]);
			sum -= delta;
			v1 += ((v0 << 4 ^ v0 >> 5) + v0) ^ (sum + k[(sum >> 11) & 3]);
		}

		memcpy(buffer + readPos, &v0, 4);
		readPos += 4;
		memcpy(buffer + readPos, &v1, 4);
		readPos += 4;
	}
}

bool Protocol::XTEA_decrypt(NetworkMessage& msg) const
{
	if (((msg.getLength() - 6) & 7) != 0) {
		return false;
	}

	const uint32_t delta = 0x61C88647;

	uint8_t* buffer = msg.getBuffer() + msg.getBufferPosition();
	const size_t messageLength = (msg.getLength() - 6);
	size_t readPos = 0;
	const uint32_t k[] = {key[0], key[1], key[2], key[3]};
	while (readPos < messageLength) {
		uint32_t v0;
		memcpy(&v0, buffer + readPos, 4);
		uint32_t v1;
		memcpy(&v1, buffer + readPos + 4, 4);

		uint32_t sum = 0xC6EF3720;

		for (int32_t i = 32; --i >= 0;) {
			v1 -= ((v0 << 4 ^ v0 >> 5) + v0) ^ (sum + k[(sum >> 11) & 3]);
			sum += delta;
			v0 -= ((v1 << 4 ^ v1 >> 5) + v1) ^ (sum + k[sum & 3]);
		}

		memcpy(buffer + readPos, &v0, 4);
		readPos += 4;
		memcpy(buffer + readPos, &v1, 4);
		readPos += 4;
	}

	int innerLength = msg.get<uint16_t>();
	if (innerLength > msg.getLength() - 8) {
		return false;
	}

	msg.setLength(innerLength);
	return true;
}

bool Protocol::RSA_decrypt(NetworkMessage& msg)
{
	if ((msg.getLength() - msg.getBufferPosition()) < 128) {
		return false;
	}

	g_RSA.decrypt(reinterpret_cast<char*>(msg.getBuffer()) + msg.getBufferPosition()); //does not break strict aliasing
	return msg.getByte() == 0;
}

uint32_t Protocol::getIP() const
{
	if (auto connection = getConnection()) {
		return connection->getIP();
	}

	return 0;
}
