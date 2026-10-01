// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_CONNECTION_H_FC8E1B4392D24D27A2F129D8B93A6348
#define FS_CONNECTION_H_FC8E1B4392D24D27A2F129D8B93A6348

#include <unordered_set>

#include "networkmessage.h"
#include "proxyprotocol.h"

static constexpr int32_t CONNECTION_WRITE_TIMEOUT = 30;
static constexpr int32_t CONNECTION_READ_TIMEOUT = 30;

class Protocol;
using Protocol_ptr = std::shared_ptr<Protocol>;
class OutputMessage;
using OutputMessage_ptr = std::shared_ptr<OutputMessage>;
class Connection;
using Connection_ptr = std::shared_ptr<Connection>;
using ConnectionWeak_ptr = std::weak_ptr<Connection>;
class ServiceBase;
using Service_ptr = std::shared_ptr<ServiceBase>;
class ServicePort;
using ServicePort_ptr = std::shared_ptr<ServicePort>;
using ConstServicePort_ptr = std::shared_ptr<const ServicePort>;

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

	private:
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

		enum { FORCE_CLOSE = true };

		Connection(boost::asio::io_service& io_service,
		ConstServicePort_ptr service_port) :
			readTimer(io_service),
			writeTimer(io_service),
			service_port(std::move(service_port)),
			socket(io_service),
			timeConnected(time(nullptr)) {}
		~Connection();

		friend class ConnectionManager;

		void close(bool force = false);
		// Used by protocols that require server to send first
		void accept(Protocol_ptr protocol);
		void accept();

		void send(const OutputMessage_ptr& msg);

		uint32_t getIP();
		// Whether the connection was relayed by a proxy that announced the original client address (PROXY protocol)
		bool isProxied() const {
			return proxied;
		}

	private:
		void resolveRemoteAddress();

		void parseHeader(const boost::system::error_code& error);
		void parsePacket(const boost::system::error_code& error);

		void readProxyHeader();
		void parseProxyHeader(const boost::system::error_code& error);
		void parseProxyAddress(const boost::system::error_code& error);
		void applyProxyHeader();

		// Reads `length` bytes into `buffer`, serving bytes handed back by the PROXY protocol detection before the socket
		template <typename Handler>
		void asyncRead(uint8_t* buffer, size_t length, Handler&& handler);

		void onWriteOperation(const boost::system::error_code& error);

		static void handleTimeout(ConnectionWeak_ptr connectionWeak, const boost::system::error_code& error);

		void closeSocket();
		void internalSend(const OutputMessage_ptr& msg);

		boost::asio::ip::tcp::socket& getSocket() {
			return socket;
		}
		friend class ServicePort;

		NetworkMessage msg;

		boost::asio::steady_timer readTimer;
		boost::asio::steady_timer writeTimer;

		std::recursive_mutex connectionLock;

		std::list<OutputMessage_ptr> messageQueue;

		ConstServicePort_ptr service_port;
		Protocol_ptr protocol;

		boost::asio::ip::tcp::socket socket;

		time_t timeConnected;
		uint32_t packetsSent = 0;

		// Client address, the socket address unless a trusted proxy announced another one (PROXY protocol)
		boost::asio::ip::address_v4 remoteAddress;

		tfs::net::proxy_protocol::Header proxyHeader{};
		// Bytes read while probing for a PROXY protocol header that turned out to be ordinary client data
		std::vector<uint8_t> pushback;

		bool closed = false;
		bool receivedFirst = false;
		bool receivedFirstHeader = false;
		bool proxied = false;
};

template <typename Handler>
void Connection::asyncRead(uint8_t* buffer, size_t length, Handler&& handler)
{
	if (!pushback.empty()) {
		const size_t count = std::min(length, pushback.size());
		std::copy_n(pushback.begin(), count, buffer);
		pushback.erase(pushback.begin(), pushback.begin() + count);
		buffer += count;
		length -= count;

		if (length == 0) {
			// Complete through the executor like a socket read would, so the handler never runs inside the caller
			boost::asio::post(socket.get_executor(), [handler = std::forward<Handler>(handler), count]() mutable {
				handler(boost::system::error_code{}, count);
			});
			return;
		}
	}

	boost::asio::async_read(socket, boost::asio::buffer(buffer, length), std::forward<Handler>(handler));
}

#endif
