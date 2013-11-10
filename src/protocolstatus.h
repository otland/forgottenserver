/**
 * The Forgotten Server - a server application for the MMORPG Tibia
 * Copyright (C) 2013  Mark Samman <mark.samman@gmail.com>
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

#ifndef __OTSERV_STATUS_H
#define __OTSERV_STATUS_H

#include "networkmessage.h"
#include "protocol.h"

class ProtocolStatus : public Protocol
{
	public:
		// static protocol information
		enum {server_sends_first = false};
		enum {protocol_identifier = 0xFF};
		enum {use_checksum = false};
		static const char* protocol_name() {
			return "status protocol";
		}

#ifdef __ENABLE_SERVER_DIAGNOSTIC__
		static uint32_t protocolStatusCount;
#endif
		ProtocolStatus(Connection_ptr connection) : Protocol(connection) {
#ifdef __ENABLE_SERVER_DIAGNOSTIC__
			protocolStatusCount++;
#endif
		}
		virtual ~ProtocolStatus() {
#ifdef __ENABLE_SERVER_DIAGNOSTIC__
			protocolStatusCount--;
#endif
		}

		virtual int32_t getProtocolId() {
			return 0xFF;
		}

		virtual void onRecvFirstMessage(NetworkMessage& msg);

		void sendStatusString();
		void sendInfo(uint16_t requestedInfo, const std::string& characterName);

		static const uint64_t start;

	protected:
		static std::map<uint32_t, int64_t> ipConnectMap;
};

#endif
