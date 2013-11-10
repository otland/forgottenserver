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

#ifndef __OTSERV_CONNECTION_H__
#define __OTSERV_CONNECTION_H__

#include <set>
#include <memory>

#include "networkmessage.h"

class Protocol;
class OutputMessage;
typedef std::shared_ptr<OutputMessage> OutputMessage_ptr;
class Connection;
typedef std::shared_ptr<Connection> Connection_ptr;
class ServiceBase;
typedef std::shared_ptr<ServiceBase> Service_ptr;
class ServicePort;
typedef std::shared_ptr<ServicePort> ServicePort_ptr;

class ConnectionManager
{
	public:
		~ConnectionManager() {
			closeAll();
		}

		static ConnectionManager* getInstance() {
			static ConnectionManager instance;
			return &instance;
		}

		Connection_ptr createConnection(boost::asio::ip::tcp::socket* socket,
		                                boost::asio::io_service& io_service, ServicePort_ptr servicers);
		void releaseConnection(Connection_ptr connection);
		void closeAll();

	protected:
		ConnectionManager() {
		}

		// TODO: Use unordered_set (we need to specify a hash function)
		std::set<Connection_ptr> m_connections;
		std::recursive_mutex m_connectionManagerLock;
};

class Connection : public std::enable_shared_from_this<Connection>
{
	public:
#ifdef __ENABLE_SERVER_DIAGNOSTIC__
		static uint32_t connectionCount;
#endif

		// non-copyable
		Connection(const Connection&) = delete;
		Connection& operator=(const Connection&) = delete;

		enum { write_timeout = 30 };
		enum { read_timeout = 30 };

		enum ConnectionState_t {
			CONNECTION_STATE_OPEN = 0,
			CONNECTION_STATE_REQUEST_CLOSE = 1,
			CONNECTION_STATE_CLOSING = 2,
			CONNECTION_STATE_CLOSED = 3
		};

	private:
		Connection(boost::asio::ip::tcp::socket* socket,
		           boost::asio::io_service& io_service,
		           ServicePort_ptr service_port) :
			m_socket(socket),
			m_readTimer(io_service),
			m_writeTimer(io_service),
			m_io_service(io_service),
			m_service_port(service_port) {
			m_refCount = 0;
			m_protocol = nullptr;
			m_pendingWrite = 0;
			m_pendingRead = 0;
			m_connectionState = CONNECTION_STATE_OPEN;
			m_receivedFirst = false;
			m_writeError = false;
			m_readError = false;
			m_packetsSent = 0;
			m_timeConnected = time(nullptr);

#ifdef __ENABLE_SERVER_DIAGNOSTIC__
			connectionCount++;
#endif
		}
		friend class ConnectionManager;

	public:
		~Connection() {
#ifdef __ENABLE_SERVER_DIAGNOSTIC__
			connectionCount--;
#endif
		}

		boost::asio::ip::tcp::socket& getHandle() {
			return *m_socket;
		}

		void closeConnection();
		// Used by protocols that require server to send first
		void acceptConnection(Protocol* protocol);
		void acceptConnection();

		bool send(OutputMessage_ptr msg);

		uint32_t getIP() const;

		int32_t addRef() {
			return ++m_refCount;
		}
		int32_t unRef() {
			return --m_refCount;
		}

	private:
		void parseHeader(const boost::system::error_code& error);
		void parsePacket(const boost::system::error_code& error);

		void onWriteOperation(OutputMessage_ptr msg, const boost::system::error_code& error);

		void onStopOperation();
		void handleReadError(const boost::system::error_code& error);
		void handleWriteError(const boost::system::error_code& error);

		static void handleReadTimeout(std::weak_ptr<Connection> weak_conn, const boost::system::error_code& error);
		static void handleWriteTimeout(std::weak_ptr<Connection> weak_conn, const boost::system::error_code& error);

		void closeConnectionTask();
		void deleteConnectionTask();
		void releaseConnection();
		void closeSocket();
		void onReadTimeout();
		void onWriteTimeout();

		void internalSend(OutputMessage_ptr msg);

		NetworkMessage m_msg;
		boost::asio::ip::tcp::socket* m_socket;
		boost::asio::deadline_timer m_readTimer;
		boost::asio::deadline_timer m_writeTimer;
		boost::asio::io_service& m_io_service;
		ServicePort_ptr m_service_port;
		bool m_receivedFirst;
		bool m_writeError;
		bool m_readError;

		int32_t m_pendingWrite;
		int32_t m_pendingRead;
		ConnectionState_t m_connectionState;
		uint32_t m_refCount;
		static bool m_logError;
		std::recursive_mutex m_connectionLock;

		time_t m_timeConnected;
		uint32_t m_packetsSent;

		Protocol* m_protocol;
};

#endif
