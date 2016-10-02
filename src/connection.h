/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2016  Mark Samman <mark.samman@gmail.com>
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
typedef std::shared_ptr<const ServicePort> ConstServicePort_ptr;

class ConnectionManager
{
	public:
		static ConnectionManager& getInstance() {
			static ConnectionManager instance;
			return instance;
		}

		Connection_ptr createConnection(boost::asio::io_service& io_service, ConstServicePort_ptr servicePort);
		void releaseConnection(const Connection_ptr& connection);
		void closeAll();

	protected:
		ConnectionManager() = default;

		std::unordered_set<Connection_ptr> connections;
		std::mutex connectionManagerLock;
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

		enum { FORCE_CLOSE = true };

		Connection(boost::asio::io_service& io_service,
		           ConstServicePort_ptr service_port) :
			readTimer(io_service),
			writeTimer(io_service),
			service_port(std::move(service_port)),
			socket(io_service) {
			connectionState = CONNECTION_STATE_OPEN;
			receivedFirst = false;
			packetsSent = 0;
			timeConnected = time(nullptr);
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

		void onWriteOperation(const boost::system::error_code& error);

		static void handleTimeout(ConnectionWeak_ptr connectionWeak, const boost::system::error_code& error);

		void closeSocket();
		void internalSend(const OutputMessage_ptr& msg);

		boost::asio::ip::tcp::socket& getSocket() {
			return socket;
		}
		friend class ServicePort;

		NetworkMessage msg;

		boost::asio::deadline_timer readTimer;
		boost::asio::deadline_timer writeTimer;

		std::recursive_mutex connectionLock;

		std::list<OutputMessage_ptr> messageQueue;

		ConstServicePort_ptr service_port;
		Protocol_ptr protocol;

		boost::asio::ip::tcp::socket socket;

		time_t timeConnected;
		uint32_t packetsSent;

		bool connectionState;
		bool receivedFirst;
};

#endif
