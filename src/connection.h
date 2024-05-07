// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_CONNECTION_H
#define FS_CONNECTION_H

#include "networkmessage.h"

enum ConnectionState_t
{
	CONNECTION_STATE_DISCONNECTED,
	CONNECTION_STATE_REQUEST_CHARLIST,
	CONNECTION_STATE_GAMEWORLD_AUTH,
	CONNECTION_STATE_GAME,
	CONNECTION_STATE_PENDING
};

enum checksumMode_t
{
	CHECKSUM_DISABLED,
	CHECKSUM_ADLER,
	CHECKSUM_SEQUENCE
};

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
	static ConnectionManager& getInstance()
	{
		static ConnectionManager instance;
		return instance;
	}

	Connection_ptr createConnection(boost::asio::io_context& io_context, ConstServicePort_ptr servicePort);
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
	using Address = boost::asio::ip::address;
	// non-copyable
	Connection(const Connection&) = delete;
	Connection& operator=(const Connection&) = delete;

	enum
	{
		FORCE_CLOSE = true
	};

	Connection(boost::asio::io_context& io_context, ConstServicePort_ptr service_port);
	~Connection();

	friend class ConnectionManager;

	void close(bool force = false);
	// Used by protocols that require server to send first
	void accept(Protocol_ptr protocol);
	void accept();

	void send(const OutputMessage_ptr& msg);

	const Address& getIP() const { return remoteAddress; };

private:
	void parseHeader(const boost::system::error_code& error);
	void parsePacket(const boost::system::error_code& error);

	void onWriteOperation(const boost::system::error_code& error);

	static void handleTimeout(ConnectionWeak_ptr connectionWeak, const boost::system::error_code& error);

	void closeSocket();
	void internalSend(const OutputMessage_ptr& msg);

	boost::asio::ip::tcp::socket& getSocket() { return socket; }
	friend class ServicePort;

	NetworkMessage msg;

	boost::asio::steady_timer readTimer;
	boost::asio::steady_timer writeTimer;

	std::recursive_mutex connectionLock;

	std::list<OutputMessage_ptr> messageQueue;

	ConstServicePort_ptr service_port;
	Protocol_ptr protocol;

	boost::asio::ip::tcp::socket socket;
	Address remoteAddress;
	time_t timeConnected;
	uint32_t packetsSent = 0;

	ConnectionState_t connectionState = CONNECTION_STATE_PENDING;
	bool receivedFirst = false;
	bool receivedName = false;
	bool receivedLastChar = false;
};

#endif // FS_CONNECTION_H
