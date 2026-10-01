// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "ban.h"
#include "configmanager.h"
#include "connection.h"
#include "outputmessage.h"
#include "protocol.h"
#include "scheduler.h"
#include "server.h"

extern ConfigManager g_config;
extern Ban g_bans;

namespace proxy_protocol = tfs::net::proxy_protocol;

Connection_ptr ConnectionManager::createConnection(boost::asio::io_service& io_service, ConstServicePort_ptr servicePort)
{
	std::lock_guard<std::mutex> lockClass(connectionManagerLock);

	auto connection = std::make_shared<Connection>(io_service, servicePort);
	connections.insert(connection);
	return connection;
}

void ConnectionManager::releaseConnection(const Connection_ptr& connection)
{
	std::lock_guard<std::mutex> lockClass(connectionManagerLock);

	connections.erase(connection);
}

void ConnectionManager::closeAll()
{
	std::lock_guard<std::mutex> lockClass(connectionManagerLock);

	for (const auto& connection : connections) {
		try {
			boost::system::error_code error;
			connection->socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
			connection->socket.close(error);
		} catch (boost::system::system_error&) {
		}
	}
	connections.clear();
}

// Connection

void Connection::close(bool force)
{
	//any thread
	ConnectionManager::getInstance().releaseConnection(shared_from_this());

	std::lock_guard<std::recursive_mutex> lockClass(connectionLock);
	if (closed) {
		return;
	}
	closed = true;

	if (protocol) {
		g_dispatcher.addTask(
			createTask(std::bind(&Protocol::release, protocol)));
	}

	if (messageQueue.empty() || force) {
		closeSocket();
	} else {
		//will be closed by the destructor or onWriteOperation
	}
}

void Connection::closeSocket()
{
	if (socket.is_open()) {
		try {
			readTimer.cancel();
			writeTimer.cancel();
			boost::system::error_code error;
			socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
			socket.close(error);
		} catch (boost::system::system_error& e) {
			std::cout << "[Network error - Connection::closeSocket] " << e.what() << std::endl;
		}
	}
}

Connection::~Connection()
{
	closeSocket();
}

void Connection::accept(Protocol_ptr protocol)
{
	this->protocol = protocol;
	g_dispatcher.addTask(createTask(std::bind(&Protocol::onConnect, protocol)));

	accept();
}

void Connection::resolveRemoteAddress()
{
	boost::system::error_code error;
	const boost::asio::ip::tcp::endpoint endpoint = socket.remote_endpoint(error);
	if (!error && endpoint.address().is_v4()) {
		remoteAddress = endpoint.address().to_v4();
	}
}

void Connection::accept()
{
	std::lock_guard<std::recursive_mutex> lockClass(connectionLock);
	try {
		readTimer.expires_from_now(std::chrono::seconds(CONNECTION_READ_TIMEOUT));
		readTimer.async_wait(std::bind(&Connection::handleTimeout, std::weak_ptr<Connection>(shared_from_this()), std::placeholders::_1));

		// Read size of the first packet
		asyncRead(msg.getBuffer(), NetworkMessage::HEADER_LENGTH,
		          std::bind(&Connection::parseHeader, shared_from_this(), std::placeholders::_1));
	} catch (boost::system::system_error& e) {
		std::cout << "[Network error - Connection::accept] " << e.what() << std::endl;
		close(FORCE_CLOSE);
	}
}

void Connection::parseHeader(const boost::system::error_code& error)
{
	std::lock_guard<std::recursive_mutex> lockClass(connectionLock);
	readTimer.cancel();

	if (error) {
		close(FORCE_CLOSE);
		return;
	} else if (closed) {
		return;
	}

	if (!receivedFirstHeader) {
		receivedFirstHeader = true;

		// Only a proxy running on the same host is trusted to announce the original client address. Only the two
		// bytes of a regular packet header have been read at this point, so this is a probe: the rest of the
		// signature is checked once the full header is in, and a mismatch there hands the bytes back to this flow
		if (proxy_protocol::isTrustedPeer(remoteAddress)) {
			if (proxy_protocol::matchesSignature(msg.getBuffer(), NetworkMessage::HEADER_LENGTH)) {
				readProxyHeader();
				return;
			}

			// Not relayed by a proxy, apply the connection limit that ServicePort defers for local peers
			if (!g_bans.acceptConnection(getIP())) {
				close(FORCE_CLOSE);
				return;
			}
		}
	}

	uint32_t timePassed = std::max<uint32_t>(1, (time(nullptr) - timeConnected) + 1);
	if ((++packetsSent / timePassed) > static_cast<uint32_t>(g_config.getNumber(ConfigManager::MAX_PACKETS_PER_SECOND))) {
		std::cout << convertIPToString(getIP()) << " disconnected for exceeding packet per second limit." << std::endl;
		close();
		return;
	}

	if (timePassed > 2) {
		timeConnected = time(nullptr);
		packetsSent = 0;
	}

	uint16_t size = msg.getLengthHeader();
	if (size == 0 || size >= NETWORKMESSAGE_MAXSIZE - 16) {
		close(FORCE_CLOSE);
		return;
	}

	try {
		readTimer.expires_from_now(std::chrono::seconds(CONNECTION_READ_TIMEOUT));
		readTimer.async_wait(std::bind(&Connection::handleTimeout, std::weak_ptr<Connection>(shared_from_this()),
		                                    std::placeholders::_1));

		// Read packet content
		msg.setLength(size + NetworkMessage::HEADER_LENGTH);
		asyncRead(msg.getBodyBuffer(), size,
		          std::bind(&Connection::parsePacket, shared_from_this(), std::placeholders::_1));
	} catch (boost::system::system_error& e) {
		std::cout << "[Network error - Connection::parseHeader] " << e.what() << std::endl;
		close(FORCE_CLOSE);
	}
}

void Connection::readProxyHeader()
{
	try {
		readTimer.expires_from_now(std::chrono::seconds(CONNECTION_READ_TIMEOUT));
		readTimer.async_wait(std::bind(&Connection::handleTimeout, std::weak_ptr<Connection>(shared_from_this()),
		                                    std::placeholders::_1));

		// Read the rest of the fixed-size header, the first NetworkMessage::HEADER_LENGTH bytes are already in
		boost::asio::async_read(socket,
		                        boost::asio::buffer(msg.getBuffer() + NetworkMessage::HEADER_LENGTH,
		                                            proxy_protocol::HEADER_LENGTH - NetworkMessage::HEADER_LENGTH),
		                        std::bind(&Connection::parseProxyHeader, shared_from_this(), std::placeholders::_1));
	} catch (boost::system::system_error& e) {
		std::cout << "[Network error - Connection::readProxyHeader] " << e.what() << std::endl;
		close(FORCE_CLOSE);
	}
}

void Connection::parseProxyHeader(const boost::system::error_code& error)
{
	std::lock_guard<std::recursive_mutex> lockClass(connectionLock);
	readTimer.cancel();

	if (error) {
		close(FORCE_CLOSE);
		return;
	} else if (closed) {
		return;
	}

	uint8_t* buffer = msg.getBuffer();
	if (!proxy_protocol::matchesSignature(buffer, proxy_protocol::SIGNATURE.size())) {
		// The first two bytes matched by coincidence: this is an ordinary packet that happens to start with 0x0D 0x0A.
		// Hand everything read so far back to the regular flow, which consumes it before reading from the socket
		pushback.assign(buffer, buffer + proxy_protocol::HEADER_LENGTH);

		// Not relayed by a proxy, apply the connection limit that ServicePort defers for local peers
		if (!g_bans.acceptConnection(getIP())) {
			close(FORCE_CLOSE);
			return;
		}

		accept();
		return;
	}

	auto header = proxy_protocol::parseHeader(buffer);
	if (!header || header->length > NETWORKMESSAGE_MAXSIZE - proxy_protocol::HEADER_LENGTH) {
		std::cout << "[Warning - Connection::parseProxyHeader] Malformed PROXY protocol header from "
		          << remoteAddress.to_string() << std::endl;
		close(FORCE_CLOSE);
		return;
	}

	proxyHeader = *header;
	if (proxyHeader.length == 0) {
		applyProxyHeader();
		return;
	}

	try {
		readTimer.expires_from_now(std::chrono::seconds(CONNECTION_READ_TIMEOUT));
		readTimer.async_wait(std::bind(&Connection::handleTimeout, std::weak_ptr<Connection>(shared_from_this()),
		                                    std::placeholders::_1));

		// Read the address block and any TLVs following it
		boost::asio::async_read(socket,
		                        boost::asio::buffer(msg.getBuffer() + proxy_protocol::HEADER_LENGTH, proxyHeader.length),
		                        std::bind(&Connection::parseProxyAddress, shared_from_this(), std::placeholders::_1));
	} catch (boost::system::system_error& e) {
		std::cout << "[Network error - Connection::parseProxyHeader] " << e.what() << std::endl;
		close(FORCE_CLOSE);
	}
}

void Connection::parseProxyAddress(const boost::system::error_code& error)
{
	std::lock_guard<std::recursive_mutex> lockClass(connectionLock);
	readTimer.cancel();

	if (error) {
		close(FORCE_CLOSE);
		return;
	} else if (closed) {
		return;
	}

	applyProxyHeader();
}

void Connection::applyProxyHeader()
{
	// A LOCAL command (e.g. a health check) is the proxy connecting on its own behalf, the real socket endpoints
	// apply and the connection is not treated as relayed
	if (proxyHeader.command == proxy_protocol::Command::PROXY) {
		auto address = proxy_protocol::parseSourceAddress(proxyHeader, msg.getBuffer() + proxy_protocol::HEADER_LENGTH);
		if (!address) {
			// IPv6 clients cannot be represented, keeping the loopback address would exempt them from bans and limits
			std::cout << "[Warning - Connection::applyProxyHeader] PROXY protocol header from "
			          << remoteAddress.to_string() << " announced an IPv6 client, which is not supported" << std::endl;
			close(FORCE_CLOSE);
			return;
		}

		remoteAddress = *address;
		proxied = true;
	}

	// The client address is known now, apply the connection limit that ServicePort defers for local peers
	if (!g_bans.acceptConnection(getIP())) {
		close(FORCE_CLOSE);
		return;
	}

	// Continue with the regular protocol
	accept();
}

void Connection::parsePacket(const boost::system::error_code& error)
{
	std::lock_guard<std::recursive_mutex> lockClass(connectionLock);
	readTimer.cancel();

	if (error) {
		close(FORCE_CLOSE);
		return;
	} else if (closed) {
		return;
	}

	//Check packet checksum
	uint32_t checksum;
	int32_t len = msg.getLength() - msg.getBufferPosition() - NetworkMessage::CHECKSUM_LENGTH;
	if (len > 0) {
		checksum = adlerChecksum(msg.getBuffer() + msg.getBufferPosition() + NetworkMessage::CHECKSUM_LENGTH, len);
	} else {
		checksum = 0;
	}

	uint32_t recvChecksum = msg.get<uint32_t>();
	if (recvChecksum != checksum) {
		// it might not have been the checksum, step back
		msg.skipBytes(-NetworkMessage::CHECKSUM_LENGTH);
	}

	if (!receivedFirst) {
		// First message received
		receivedFirst = true;

		if (!protocol) {
			// Game protocol has already been created at this point
			protocol = service_port->make_protocol(recvChecksum == checksum, msg, shared_from_this());
			if (!protocol) {
				close(FORCE_CLOSE);
				return;
			}
		} else {
			msg.skipBytes(1); // Skip protocol ID
		}

		protocol->onRecvFirstMessage(msg);
	} else {
		protocol->onRecvMessage(msg); // Send the packet to the current protocol
	}

	try {
		readTimer.expires_from_now(std::chrono::seconds(CONNECTION_READ_TIMEOUT));
		readTimer.async_wait(std::bind(&Connection::handleTimeout, std::weak_ptr<Connection>(shared_from_this()),
		                                    std::placeholders::_1));

		// Wait to the next packet
		asyncRead(msg.getBuffer(), NetworkMessage::HEADER_LENGTH,
		          std::bind(&Connection::parseHeader, shared_from_this(), std::placeholders::_1));
	} catch (boost::system::system_error& e) {
		std::cout << "[Network error - Connection::parsePacket] " << e.what() << std::endl;
		close(FORCE_CLOSE);
	}
}

void Connection::send(const OutputMessage_ptr& msg)
{
	std::lock_guard<std::recursive_mutex> lockClass(connectionLock);
	if (closed) {
		return;
	}

	bool noPendingWrite = messageQueue.empty();
	messageQueue.emplace_back(msg);
	if (noPendingWrite) {
		internalSend(msg);
	}
}

void Connection::internalSend(const OutputMessage_ptr& msg)
{
	protocol->onSendMessage(msg);
	try {
		writeTimer.expires_from_now(std::chrono::seconds(CONNECTION_WRITE_TIMEOUT));
		writeTimer.async_wait(std::bind(&Connection::handleTimeout, std::weak_ptr<Connection>(shared_from_this()),
		                                     std::placeholders::_1));

		boost::asio::async_write(socket,
		                         boost::asio::buffer(msg->getOutputBuffer(), msg->getLength()),
		                         std::bind(&Connection::onWriteOperation, shared_from_this(), std::placeholders::_1));
	} catch (boost::system::system_error& e) {
		std::cout << "[Network error - Connection::internalSend] " << e.what() << std::endl;
		close(FORCE_CLOSE);
	}
}

uint32_t Connection::getIP()
{
	std::lock_guard<std::recursive_mutex> lockClass(connectionLock);

	// IP-address is expressed in network byte order
	return htonl(remoteAddress.to_ulong());
}

void Connection::onWriteOperation(const boost::system::error_code& error)
{
	std::lock_guard<std::recursive_mutex> lockClass(connectionLock);
	writeTimer.cancel();
	messageQueue.pop_front();

	if (error) {
		messageQueue.clear();
		close(FORCE_CLOSE);
		return;
	}

	if (!messageQueue.empty()) {
		internalSend(messageQueue.front());
	} else if (closed) {
		closeSocket();
	}
}

void Connection::handleTimeout(ConnectionWeak_ptr connectionWeak, const boost::system::error_code& error)
{
	if (error == boost::asio::error::operation_aborted) {
		//The timer has been manually canceled
		return;
	}

	if (auto connection = connectionWeak.lock()) {
		connection->close(FORCE_CLOSE);
	}
}
