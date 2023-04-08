// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_PROTOCOLSTATUS_H
#define FS_PROTOCOLSTATUS_H

#include "protocol.h"

class NetworkMessage;

class ProtocolStatus final : public Protocol
{
public:
	// static protocol information
	enum
	{
		server_sends_first = false
	};
	enum
	{
		protocol_identifier = 0xFF
	};
	enum
	{
		use_checksum = false
	};
	static const char* protocol_name() { return "status protocol"; }

	explicit ProtocolStatus(Connection_ptr connection) : Protocol(connection) {}

	void onRecvFirstMessage(NetworkMessage& msg) override;

	void sendStatusString();
	void sendInfo(uint16_t requestedInfo, const std::string& characterName);

	static const uint64_t start;

private:
	static std::map<Connection::Address, int64_t> ipConnectMap;
};

#endif // FS_PROTOCOLSTATUS_H
