// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "protocol.h"

#include "outputmessage.h"
#include "rsa.h"
#include "xtea.h"

namespace {

void XTEA_encrypt(OutputMessage& msg, const xtea::round_keys& key)
{
	// The message must be a multiple of 8
	size_t paddingBytes = msg.getLength() % 8u;
	if (paddingBytes != 0) {
		msg.addPaddingBytes(8 - paddingBytes);
	}

	uint8_t* buffer = msg.getOutputBuffer();
	xtea::encrypt(buffer, msg.getLength(), key);
}

bool XTEA_decrypt(NetworkMessage& msg, const xtea::round_keys& key)
{
	if (((msg.getLength() - 6) & 7) != 0) {
		return false;
	}

	uint8_t* buffer = msg.getRemainingBuffer();
	xtea::decrypt(buffer, msg.getLength() - 6, key);

	uint16_t innerLength = msg.get<uint16_t>();
	if (innerLength + 8 > msg.getLength()) {
		return false;
	}

	msg.setLength(innerLength);
	return true;
}

} // namespace

Protocol::~Protocol()
{
	const auto zlibEndResult = deflateEnd(&zstream);
	if (zlibEndResult == Z_DATA_ERROR) {
		std::cout << "ZLIB discarded pending output or unprocessed input while cleaning up stream state" << std::endl;
	} else if (zlibEndResult == Z_STREAM_ERROR) {
		std::cout << "ZLIB encountered an error while cleaning up stream state" << std::endl;
	}
}

void Protocol::onSendMessage(const OutputMessage_ptr& msg)
{
	if (!rawMessages) {
		if (encryptionEnabled && checksumMode == CHECKSUM_SEQUENCE) {
			uint32_t compressionChecksum = 0;
			if (msg->getLength() >= 128 && deflateMessage(*msg)) {
				compressionChecksum = 0x80000000;
			}

			msg->setSequenceId(compressionChecksum | getNextSequenceId());
		}

		msg->writeMessageLength();

		if (encryptionEnabled) {
			XTEA_encrypt(*msg, key);
			msg->addCryptoHeader(checksumMode);
		}
	}
}

void Protocol::onRecvMessage(NetworkMessage& msg)
{
	if (encryptionEnabled && !XTEA_decrypt(msg, key)) {
		return;
	}

	parsePacket(msg);
}

OutputMessage_ptr Protocol::getOutputBuffer(int32_t size)
{
	// dispatcher thread
	if (!outputBuffer) {
		outputBuffer = tfs::net::make_output_message();
	} else if ((outputBuffer->getLength() + size) > NetworkMessage::MAX_PROTOCOL_BODY_LENGTH) {
		send(outputBuffer);
		outputBuffer = tfs::net::make_output_message();
	}
	return outputBuffer;
}

bool Protocol::RSA_decrypt(NetworkMessage& msg)
{
	if (msg.getRemainingBufferLength() < RSA_BUFFER_LENGTH) {
		return false;
	}

	tfs::rsa::decrypt(msg.getRemainingBuffer(), RSA_BUFFER_LENGTH);
	return msg.getByte() == 0;
}

bool Protocol::deflateMessage(OutputMessage& msg)
{
	static thread_local std::vector<uint8_t> buffer(NETWORKMESSAGE_MAXSIZE);

	zstream.next_in = msg.getOutputBuffer();
	zstream.avail_in = msg.getLength();
	zstream.next_out = buffer.data();
	zstream.avail_out = buffer.size();

	const auto result = deflate(&zstream, Z_FINISH);
	if (result != Z_OK && result != Z_STREAM_END) {
		std::cout << "Error while deflating packet data error: " << (zstream.msg ? zstream.msg : "unknown")
		          << std::endl;
		return false;
	}

	const auto size = zstream.total_out;
	deflateReset(&zstream);

	if (size <= 0) {
		std::cout << "Deflated packet data had invalid size: " << size
		          << " error: " << (zstream.msg ? zstream.msg : "unknown") << std::endl;
		return false;
	}

	msg.reset();
	msg.addBytes(reinterpret_cast<const char*>(buffer.data()), size);

	return true;
}

Connection::Address Protocol::getIP() const
{
	if (auto connection = getConnection()) {
		return connection->getIP();
	}

	return {};
}

bool Protocol::isOtcProxy() const
{
	if (auto connection = getConnection()) {
		return connection->isOtcProxy();
	}

	return false;
}

bool Protocol::isHaProxy() const
{
	if (auto connection = getConnection()) {
		return connection->isHaProxy();
	}

	return false;
}
