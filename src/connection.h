/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2015  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_CONNECTION_H_FC8E1B4392D24D27A2F129D8B93A6348
#define FS_CONNECTION_H_FC8E1B4392D24D27A2F129D8B93A6348

#include <unordered_set>

#include "networkmessage.h"

class Protocol;
typedef std::shared_ptr<Protocol> Protocol_ptr;
class OutputMessage;
typedef std::shared_ptr<OutputMessage> OutputMessage_ptr;
class Connection;
typedef std::shared_ptr<Connection> Connection_ptr;
typedef std::weak_ptr<Connection> ConnectionWeak_ptr;
class ServiceBase;
typedef std::shared_ptr<ServiceBase> Service_ptr;
class ServicePort;
typedef std::shared_ptr<ServicePort> ServicePort_ptr;

class ConnectionManager
{
	public:
		static ConnectionManager& getInstance() {
			static ConnectionManager instance;
			return instance;
		}

		Connection_ptr createConnection(boost::asio::ip::tcp::socket* socket,
		                                boost::asio::io_service& io_service, ServicePort_ptr servicers);
		void releaseConnection(const Connection_ptr& connection);
		void closeAll();

	protected:
		ConnectionManager() = default;

		std::unordered_set<Connection_ptr> m_connections;
		std::mutex m_connectionManagerLock;
};

class Connection : public std::enable_shared_from_this<Connection>
{
	public:
		// non-copyable
		Connection(const Connection&) = delete;
		Connection& operator=(const Connection&) = delete;

		enum { write_timeout = 30 };
		enum { read_timeout = 30 };

		enum ConnectionState_t {
			CONNECTION_STATE_OPEN,
			CONNECTION_STATE_CLOSED,
		};
		
		enum { FORCE_CLOSE = true};

		Connection(boost::asio::ip::tcp::socket* socket,
		           boost::asio::io_service& io_service,
		           ServicePort_ptr service_port) :
			m_readTimer(io_service),
			m_writeTimer(io_service),
			m_service_port(service_port),
			m_socket(socket),
			m_io_service(io_service) {
			m_pendingWrite = false;
			m_connectionState = CONNECTION_STATE_OPEN;
			m_receivedFirst = false;
			m_packetsSent = 0;
			m_timeConnected = time(nullptr);
		}
		~Connection();

		friend class ConnectionManager;

		void close(bool force = false);
		// Used by protocols that require server to send first
		void accept(Protocol_ptr protocol);
		void accept();

		void send(const OutputMessage_ptr& msg);

		uint32_t getIP();

	private:
		void parseHeader(const boost::system::error_code& error);
		void parsePacket(const boost::system::error_code& error);

		void onWriteOperation(OutputMessage_ptr msg, const boost::system::error_code& error);

		static void handleReadTimeout(ConnectionWeak_ptr connectionWeak, const boost::system::error_code& error);
		static void handleWriteTimeout(ConnectionWeak_ptr connectionWeak, const boost::system::error_code& error);

		void closeSocket();

		void internalSend(const OutputMessage_ptr& msg);

		NetworkMessage m_msg;

		boost::asio::deadline_timer m_readTimer;
		boost::asio::deadline_timer m_writeTimer;

		std::recursive_mutex m_connectionLock;

		std::list<OutputMessage_ptr> messageQueue;

		ServicePort_ptr m_service_port;
		Protocol_ptr m_protocol;

		std::unique_ptr<boost::asio::ip::tcp::socket> m_socket;
		boost::asio::io_service& m_io_service;

		time_t m_timeConnected;
		uint32_t m_packetsSent;

		bool m_pendingWrite;
		bool m_connectionState;
		bool m_receivedFirst;

		static bool m_logError;
};

#endif
