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

#ifndef __OTSERV_PROTOCOL_H__
#define __OTSERV_PROTOCOL_H__

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>

class NetworkMessage;
class OutputMessage;
class Connection;
typedef boost::shared_ptr<OutputMessage> OutputMessage_ptr;
typedef boost::shared_ptr<Connection> Connection_ptr;
class RSA;

class Protocol : boost::noncopyable
{
	public:
		Protocol(Connection_ptr connection) {
			m_connection = connection;
			m_encryptionEnabled = false;
			m_checksumEnabled = true;
			m_rawMessages = false;
			m_key[0] = 0;
			m_key[1] = 0;
			m_key[2] = 0;
			m_key[3] = 0;
			m_refCount = 0;
		}

		virtual ~Protocol() {}

		virtual int32_t getProtocolId() {
			return 0x00;
		}

		virtual void parsePacket(NetworkMessage& msg) {}

		virtual void onSendMessage(OutputMessage_ptr msg);
		void onRecvMessage(NetworkMessage& msg);
		virtual void onRecvFirstMessage(NetworkMessage& msg) = 0;
		virtual void onConnect() {}

		Connection_ptr getConnection() {
			return m_connection;
		}
		const Connection_ptr getConnection() const {
			return m_connection;
		}
		void setConnection(Connection_ptr connection) {
			m_connection = connection;
		}

		uint32_t getIP() const;

		int32_t addRef() {
			return ++m_refCount;
		}
		int32_t unRef() {
			return --m_refCount;
		}

		//Use this function for autosend messages only
		OutputMessage_ptr getOutputBuffer(int32_t size);

	protected:
		void enableXTEAEncryption() {
			m_encryptionEnabled = true;
		}
		void disableXTEAEncryption() {
			m_encryptionEnabled = false;
		}
		void setXTEAKey(const uint32_t* key) {
			memcpy(m_key, key, sizeof(uint32_t) * 4);
		}
		void enableChecksum() {
			m_checksumEnabled = true;
		}
		void disableChecksum() {
			m_checksumEnabled = false;
		}

		void XTEA_encrypt(OutputMessage& msg);
		bool XTEA_decrypt(NetworkMessage& msg);
		bool RSA_decrypt(NetworkMessage& msg);
		bool RSA_decrypt(RSA* rsa, NetworkMessage& msg);

		void setRawMessages(bool value) {
			m_rawMessages = value;
		}

		virtual void releaseProtocol();
		virtual void deleteProtocolTask();
		friend class Connection;

		OutputMessage_ptr m_outputBuffer;

	private:
		Connection_ptr m_connection;
		bool m_encryptionEnabled;
		bool m_checksumEnabled;
		bool m_rawMessages;
		uint32_t m_key[4];
		uint32_t m_refCount;
};

#endif
