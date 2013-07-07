//////////////////////////////////////////////////////////////////////
// The Forgotten Server - a server application for the MMORPG Tibia
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#ifndef __OTSERV_PROTOCOL_LOGIN_H__
#define __OTSERV_PROTOCOL_LOGIN_H__

#include "protocol.h"

class NetworkMessage;
class OutputMessage;

class ProtocolLogin : public Protocol
{
	public:
		// static protocol information
		enum {server_sends_first = false};
		enum {protocol_identifier = 0x01};
		enum {use_checksum = true};
		static const char* protocol_name() {
			return "login protocol";
		}

#ifdef __ENABLE_SERVER_DIAGNOSTIC__
		static uint32_t protocolLoginCount;
#endif
		ProtocolLogin(Connection_ptr connection) : Protocol(connection) {
#ifdef __ENABLE_SERVER_DIAGNOSTIC__
			protocolLoginCount++;
#endif
		}
		virtual ~ProtocolLogin() {
#ifdef __ENABLE_SERVER_DIAGNOSTIC__
			protocolLoginCount--;
#endif
		}

		virtual int32_t getProtocolId() {
			return 0x01;
		}

		virtual void onRecvFirstMessage(NetworkMessage& msg);

	protected:
		void disconnectClient(uint8_t error, const char* message);

		bool parseFirstPacket(NetworkMessage& msg);
};

#endif
