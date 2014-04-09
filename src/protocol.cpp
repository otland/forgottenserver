/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2014  Mark Samman <mark.samman@gmail.com>
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
#include "scheduler.h"
#include "connection.h"
#include "outputmessage.h"
#include "rsa.h"

extern RSA g_RSA;

void Protocol::onSendMessage(OutputMessage_ptr msg)
{
	if (!m_rawMessages) {
		msg->writeMessageLength();

		if (m_encryptionEnabled) {
			XTEA_encrypt(*msg);
			msg->addCryptoHeader(m_checksumEnabled);
		}
	}

	if (msg == m_outputBuffer) {
		m_outputBuffer.reset();
	}
}

void Protocol::onRecvMessage(NetworkMessage& msg)
{
	if (m_encryptionEnabled && !XTEA_decrypt(msg)) {
		return;
	}

	parsePacket(msg);
}

OutputMessage_ptr Protocol::getOutputBuffer(int32_t size)
{
	if (m_outputBuffer && NetworkMessage::max_protocol_body_length >= m_outputBuffer->getMessageLength() + size) {
		return m_outputBuffer;
	} else if (m_connection) {
		m_outputBuffer = OutputMessagePool::getInstance()->getOutputMessage(this);
		return m_outputBuffer;
	}
	return OutputMessage_ptr();
}

void Protocol::releaseProtocol()
{
	if (m_refCount > 0) {
		//Reschedule it and try again.
		g_scheduler.addEvent(createSchedulerTask(SCHEDULER_MINTICKS, std::bind(&Protocol::releaseProtocol, this)));
	} else {
		deleteProtocolTask();
	}
}

void Protocol::deleteProtocolTask()
{
	//dispather thread
	assert(m_refCount == 0);
	setConnection(Connection_ptr());

	delete this;
}

void Protocol::XTEA_encrypt(OutputMessage& msg) const
{
	const uint32_t delta = 0x61C88647;

	// the message must be a multiple of 8
	size_t paddingBytes = msg.getMessageLength() & 7;
	if (paddingBytes != 0) {
		msg.AddPaddingBytes(8 - paddingBytes);
	}

	uint32_t* buffer = (uint32_t*)msg.getOutputBuffer();
	const size_t messageLength = msg.getMessageLength() / 4;
	size_t readPos = 0;
	const uint32_t k[] = {m_key[0], m_key[1], m_key[2], m_key[3]};
	while (readPos < messageLength) {
		uint32_t v0 = buffer[readPos], v1 = buffer[readPos + 1];
		uint32_t sum = 0;

		for (int32_t i = 32; --i >= 0;) {
			v0 += ((v1 << 4 ^ v1 >> 5) + v1) ^ (sum + k[sum & 3]);
			sum -= delta;
			v1 += ((v0 << 4 ^ v0 >> 5) + v0) ^ (sum + k[(sum >> 11) & 3]);
		}

		buffer[readPos++] = v0;
		buffer[readPos++] = v1;
	}
}

bool Protocol::XTEA_decrypt(NetworkMessage& msg) const
{
	if (((msg.getMessageLength() - 6) & 7) != 0) {
		return false;
	}

	const uint32_t delta = 0x61C88647;

	uint32_t* buffer = (uint32_t*)(msg.getBuffer() + msg.getReadPos());
	const size_t messageLength = (msg.getMessageLength() - 6) / 4;
	size_t readPos = 0;
	const uint32_t k[] = {m_key[0], m_key[1], m_key[2], m_key[3]};
	while (readPos < messageLength) {
		uint32_t v0 = buffer[readPos], v1 = buffer[readPos + 1];
		uint32_t sum = 0xC6EF3720;

		for (int32_t i = 32; --i >= 0;) {
			v1 -= ((v0 << 4 ^ v0 >> 5) + v0) ^ (sum + k[(sum >> 11) & 3]);
			sum += delta;
			v0 -= ((v1 << 4 ^ v1 >> 5) + v1) ^ (sum + k[sum & 3]);
		}

		buffer[readPos++] = v0;
		buffer[readPos++] = v1;
	}

	//

	int tmp = msg.get<uint16_t>();
	if (tmp > msg.getMessageLength() - 8) {
		return false;
	}

	msg.setMessageLength(tmp);
	return true;
}

bool Protocol::RSA_decrypt(NetworkMessage& msg)
{
	if (msg.getMessageLength() - msg.getReadPos() != 128) {
		return false;
	}

	g_RSA.decrypt((char*)(msg.getBuffer() + msg.getReadPos()));
	return msg.GetByte() == 0;
}

uint32_t Protocol::getIP() const
{
	if (getConnection()) {
		return getConnection()->getIP();
	}

	return 0;
}
