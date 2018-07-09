/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2018  Mark Samman <mark.samman@gmail.com>
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
#include "xtea.h"

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
	// The message must be a multiple of 8
	size_t paddingBytes = msg.getLength() % 8u;
	if (paddingBytes != 0) {
		msg.addPaddingBytes(8 - paddingBytes);
	}

	uint8_t* buffer = msg.getOutputBuffer();
	xtea::encrypt(buffer, msg.getLength(), key);
}

bool Protocol::XTEA_decrypt(NetworkMessage& msg) const
{
	if (((msg.getLength() - 6) & 7) != 0) {
		return false;
	}

	uint8_t* buffer = msg.getBuffer() + msg.getBufferPosition();
	xtea::decrypt(buffer, msg.getLength() - 6, key);

	uint16_t innerLength = msg.get<uint16_t>();
	if (innerLength + 8 > msg.getLength()) {
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
