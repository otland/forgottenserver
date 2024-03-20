// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_PROTOCOLOLD_H
#define FS_PROTOCOLOLD_H

#include "protocol.h"

class NetworkMessage;

class ProtocolOld final : public Protocol
{
public:
	// static protocol information
	enum
	{
		server_sends_first = false
	};
	enum
	{
		protocol_identifier = 0x01
	};
	enum
	{
		use_checksum = false
	};
	static const char* protocol_name() { return "old login protocol"; }

	explicit ProtocolOld(Connection_ptr connection) : Protocol(connection) {}

	ProtocolMessage onRecvFirstMessage(NetworkMessage& msg) override;
};

#endif // FS_PROTOCOLOLD_H
