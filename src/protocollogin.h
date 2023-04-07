// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_PROTOCOLLOGIN_H
#define FS_PROTOCOLLOGIN_H

#include "protocol.h"

class NetworkMessage;

class ProtocolLogin : public Protocol
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
		use_checksum = true
	};
	static const char* protocol_name() { return "login protocol"; }

	explicit ProtocolLogin(Connection_ptr connection) : Protocol(connection) {}

	void onRecvFirstMessage(NetworkMessage& msg) override;

private:
	void disconnectClient(const std::string& message, uint16_t version);

	void getCharacterList(const std::string& accountName, const std::string& password, const std::string& token,
	                      uint16_t version);
};

#endif // FS_PROTOCOLLOGIN_H
