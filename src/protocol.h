// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_PROTOCOL_H
#define FS_PROTOCOL_H

#include "connection.h"
#include "xtea.h"

#include <zlib.h>

class Protocol : public std::enable_shared_from_this<Protocol>
{
public:
	explicit Protocol(Connection_ptr connection) : connection(connection)
	{
		if (deflateInit2(&zstream, 6, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
			std::cout << "ZLIB initialization error: " << (zstream.msg ? zstream.msg : "unknown") << std::endl;
		}
	}
	virtual ~Protocol() = default;

	// non-copyable
	Protocol(const Protocol&) = delete;
	Protocol& operator=(const Protocol&) = delete;

	virtual void parsePacket(NetworkMessage&) {}

	virtual void onSendMessage(const OutputMessage_ptr& msg);
	void onRecvMessage(NetworkMessage& msg);
	virtual void onRecvFirstMessage(NetworkMessage& msg) = 0;
	virtual void onConnect() {}

	bool isConnectionExpired() const { return connection.expired(); }

	Connection_ptr getConnection() const { return connection.lock(); }

	Connection::Address getIP() const;

	// Use this function for autosend messages only
	OutputMessage_ptr getOutputBuffer(int32_t size);

	OutputMessage_ptr& getCurrentBuffer() { return outputBuffer; }

	void send(OutputMessage_ptr msg) const
	{
		if (auto connection = getConnection()) {
			connection->send(msg);
		}
	}

	uint32_t getNextSequenceId()
	{
		const auto sequence = ++sequenceNumber;
		if (sequenceNumber >= std::numeric_limits<int32_t>::max()) {
			sequenceNumber = 0;
		}

		return sequence;
	}

protected:
	static constexpr size_t RSA_BUFFER_LENGTH = 128;

	void disconnect() const
	{
		if (auto connection = getConnection()) {
			connection->close();
		}
	}
	void enableXTEAEncryption() { encryptionEnabled = true; }
	void setXTEAKey(const xtea::key& key) { this->key = xtea::expand_key(key); }
	void setChecksumMode(checksumMode_t newMode) { checksumMode = newMode; }

	static bool RSA_decrypt(NetworkMessage& msg);

	bool deflateMessage(OutputMessage& msg);

	void setRawMessages(bool value) { rawMessages = value; }

	virtual void release() {}

private:
	friend class Connection;

	OutputMessage_ptr outputBuffer;

	const ConnectionWeak_ptr connection;
	xtea::round_keys key;
	uint32_t sequenceNumber = 0;
	bool encryptionEnabled = false;
	checksumMode_t checksumMode = CHECKSUM_ADLER;
	bool rawMessages = false;

	z_stream zstream{};
};

#endif // FS_PROTOCOL_H
