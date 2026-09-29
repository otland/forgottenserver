// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "connection.h"

#include "configmanager.h"
#include "outputmessage.h"
#include "protocol.h"
#include "server.h"
#include "tasks.h"

namespace proxy_protocol = tfs::net::proxy_protocol;

Connection_ptr ConnectionManager::createConnection(boost::asio::io_context& io_context,
                                                   ConstServicePort_ptr servicePort)
{
	std::lock_guard<std::mutex> lockClass(connectionManagerLock);

	auto connection = std::make_shared<Connection>(io_context, servicePort);
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

Connection::Connection(boost::asio::io_context& io_context, ConstServicePort_ptr service_port) :
    readTimer(io_context),
    writeTimer(io_context),
    service_port(std::move(service_port)),
    socket(io_context),
    timeConnected(time(nullptr))
{}

void Connection::close(bool force)
{
	// any thread
	ConnectionManager::getInstance().releaseConnection(shared_from_this());

	std::lock_guard<std::recursive_mutex> lockClass(connectionLock);
	connectionState = CONNECTION_STATE_DISCONNECTED;

	if (protocol) {
		g_dispatcher.addTask([protocol = protocol]() { protocol->release(); });
	}

	if (messageQueue.empty() || force) {
		closeSocket();
	} else {
		// will be closed by the destructor or onWriteOperation
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

Connection::~Connection() { closeSocket(); }

void Connection::accept(Protocol_ptr protocol)
{
	this->protocol = protocol;
	g_dispatcher.addTask([=]() { protocol->onConnect(); });
	connectionState = CONNECTION_STATE_GAMEWORLD_AUTH;
	accept();
}

void Connection::resolveRemoteAddress()
{
	boost::system::error_code error;
	if (auto endpoint = socket.remote_endpoint(error); !error) {
		remoteAddress = endpoint.address();
	}
}

void Connection::accept()
{
	if (connectionState == CONNECTION_STATE_PENDING) {
		connectionState = CONNECTION_STATE_REQUEST_CHARLIST;
	}

	std::lock_guard<std::recursive_mutex> lockClass(connectionLock);

	try {
		readTimer.expires_after(std::chrono::seconds(CONNECTION_READ_TIMEOUT));
		readTimer.async_wait(
		    [thisPtr = std::weak_ptr<Connection>(shared_from_this())](const boost::system::error_code& error) {
			    Connection::handleTimeout(thisPtr, error);
		    });

		// Read size of the first packet
		auto bufferLength = !receivedLastChar && receivedName && connectionState == CONNECTION_STATE_GAMEWORLD_AUTH
		                        ? 1
		                        : NetworkMessage::HEADER_LENGTH;
		asyncRead(msg.getBuffer(), bufferLength,
		          [thisPtr = shared_from_this()](const boost::system::error_code& error, auto /*bytes_transferred*/) {
			          thisPtr->parseHeader(error);
		          });
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
	} else if (connectionState == CONNECTION_STATE_DISCONNECTED) {
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
			if (!acceptConnection(remoteAddress)) {
				close(FORCE_CLOSE);
				return;
			}
		}
	}

	uint32_t timePassed = std::max<uint32_t>(1, (time(nullptr) - timeConnected) + 1);
	if ((++packetsSent / timePassed) > static_cast<uint32_t>(getNumber(ConfigManager::MAX_PACKETS_PER_SECOND))) {
		std::cout << getIP() << " disconnected for exceeding packet per second limit." << std::endl;
		close();
		return;
	}

	if (!receivedLastChar && connectionState == CONNECTION_STATE_GAMEWORLD_AUTH) {
		uint8_t* msgBuffer = msg.getBuffer();

		if (!receivedName && msgBuffer[1] == 0x00) {
			receivedLastChar = true;
		} else {
			if (!receivedName) {
				receivedName = true;

				accept();
				return;
			}

			if (msgBuffer[0] == 0x0A) {
				receivedLastChar = true;
			}

			accept();
			return;
		}
	}

	if (receivedLastChar && connectionState == CONNECTION_STATE_GAMEWORLD_AUTH) {
		connectionState = CONNECTION_STATE_GAME;
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
		readTimer.expires_after(std::chrono::seconds(CONNECTION_READ_TIMEOUT));
		readTimer.async_wait(
		    [thisPtr = std::weak_ptr<Connection>(shared_from_this())](const boost::system::error_code& error) {
			    Connection::handleTimeout(thisPtr, error);
		    });

		// Read packet content
		msg.setLength(size + NetworkMessage::HEADER_LENGTH);
		asyncRead(msg.getBodyBuffer(), size,
		          [thisPtr = shared_from_this()](const boost::system::error_code& error, auto /*bytes_transferred*/) {
			          thisPtr->parsePacket(error);
		          });
	} catch (boost::system::system_error& e) {
		std::cout << "[Network error - Connection::parseHeader] " << e.what() << std::endl;
		close(FORCE_CLOSE);
	}
}

void Connection::readProxyHeader()
{
	try {
		readTimer.expires_after(std::chrono::seconds(CONNECTION_READ_TIMEOUT));
		readTimer.async_wait(
		    [thisPtr = std::weak_ptr<Connection>(shared_from_this())](const boost::system::error_code& error) {
			    Connection::handleTimeout(thisPtr, error);
		    });

		// Read the rest of the fixed-size header, the first NetworkMessage::HEADER_LENGTH bytes are already in
		boost::asio::async_read(
		    socket,
		    boost::asio::buffer(msg.getBuffer() + NetworkMessage::HEADER_LENGTH,
		                        proxy_protocol::HEADER_LENGTH - NetworkMessage::HEADER_LENGTH),
		    [thisPtr = shared_from_this()](const boost::system::error_code& error, auto /*bytes_transferred*/) {
			    thisPtr->parseProxyHeader(error);
		    });
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
	} else if (connectionState == CONNECTION_STATE_DISCONNECTED) {
		return;
	}

	uint8_t* buffer = msg.getBuffer();
	if (!proxy_protocol::matchesSignature(buffer, proxy_protocol::SIGNATURE.size())) {
		// The first two bytes matched by coincidence: this is an ordinary packet that happens to start with 0x0D 0x0A.
		// Hand everything read so far back to the regular flow, which consumes it before reading from the socket
		pushback.assign(buffer, buffer + proxy_protocol::HEADER_LENGTH);

		// Not relayed by a proxy, apply the connection limit that ServicePort defers for local peers
		if (!acceptConnection(remoteAddress)) {
			close(FORCE_CLOSE);
			return;
		}

		accept();
		return;
	}

	auto header = proxy_protocol::parseHeader(buffer);
	if (!header || header->length > NETWORKMESSAGE_MAXSIZE - proxy_protocol::HEADER_LENGTH) {
		std::cout << "[Warning - Connection::parseProxyHeader] Malformed PROXY protocol header from " << remoteAddress
		          << std::endl;
		close(FORCE_CLOSE);
		return;
	}

	proxyHeader = *header;
	if (proxyHeader.length == 0) {
		applyProxyHeader();
		return;
	}

	try {
		readTimer.expires_after(std::chrono::seconds(CONNECTION_READ_TIMEOUT));
		readTimer.async_wait(
		    [thisPtr = std::weak_ptr<Connection>(shared_from_this())](const boost::system::error_code& error) {
			    Connection::handleTimeout(thisPtr, error);
		    });

		// Read the address block and any TLVs following it
		boost::asio::async_read(
		    socket, boost::asio::buffer(msg.getBuffer() + proxy_protocol::HEADER_LENGTH, proxyHeader.length),
		    [thisPtr = shared_from_this()](const boost::system::error_code& error, auto /*bytes_transferred*/) {
			    thisPtr->parseProxyAddress(error);
		    });
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
	} else if (connectionState == CONNECTION_STATE_DISCONNECTED) {
		return;
	}

	applyProxyHeader();
}

void Connection::applyProxyHeader()
{
	// A LOCAL command (e.g. a health check) is the proxy connecting on its own behalf, the real socket endpoints
	// apply and the connection is not treated as relayed
	if (proxyHeader.command == proxy_protocol::Command::PROXY) {
		if (auto address =
		        proxy_protocol::parseSourceAddress(proxyHeader, msg.getBuffer() + proxy_protocol::HEADER_LENGTH)) {
			remoteAddress = *address;
		}
		proxied = true;
	}

	// The client address is known now, apply the connection limit that ServicePort defers for local peers
	if (!acceptConnection(remoteAddress)) {
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
	} else if (connectionState == CONNECTION_STATE_DISCONNECTED) {
		return;
	}

	// Read potential checksum bytes
	msg.get<uint32_t>();

	if (!receivedFirst) {
		receivedFirst = true;

		if (!protocol) {
			// Skip deprecated checksum bytes (with clients that aren't using it in mind)
			uint16_t len = msg.getLength();
			if (len < 280 && len != 151) {
				msg.skipBytes(-NetworkMessage::CHECKSUM_LENGTH);
			}

			// Game protocol has already been created at this point
			protocol = service_port->make_protocol(msg, shared_from_this());
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
		readTimer.expires_after(std::chrono::seconds(CONNECTION_READ_TIMEOUT));
		readTimer.async_wait(
		    [thisPtr = std::weak_ptr<Connection>(shared_from_this())](const boost::system::error_code& error) {
			    Connection::handleTimeout(thisPtr, error);
		    });

		// Wait to the next packet
		asyncRead(msg.getBuffer(), NetworkMessage::HEADER_LENGTH,
		          [thisPtr = shared_from_this()](const boost::system::error_code& error, auto /*bytes_transferred*/) {
			          thisPtr->parseHeader(error);
		          });
	} catch (boost::system::system_error& e) {
		std::cout << "[Network error - Connection::parsePacket] " << e.what() << std::endl;
		close(FORCE_CLOSE);
	}
}

void Connection::send(const OutputMessage_ptr& msg)
{
	std::lock_guard<std::recursive_mutex> lockClass(connectionLock);
	if (connectionState == CONNECTION_STATE_DISCONNECTED) {
		return;
	}

	bool noPendingWrite = messageQueue.empty();
	messageQueue.emplace_back(msg);
	if (noPendingWrite) {
		try {
			boost::asio::post(socket.get_executor(),
			                  [thisPtr = shared_from_this(), msg] { thisPtr->internalSend(msg); });
		} catch (const boost::system::system_error& e) {
			std::cout << "[Network error - Connection::send] " << e.what() << std::endl;
			messageQueue.clear();
			close(FORCE_CLOSE);
		}
	}
}

void Connection::internalSend(const OutputMessage_ptr& msg)
{
	protocol->onSendMessage(msg);
	try {
		writeTimer.expires_after(std::chrono::seconds(CONNECTION_WRITE_TIMEOUT));
		writeTimer.async_wait(
		    [thisPtr = std::weak_ptr<Connection>(shared_from_this())](const boost::system::error_code& error) {
			    Connection::handleTimeout(thisPtr, error);
		    });

		boost::asio::async_write(
		    socket, boost::asio::buffer(msg->getOutputBuffer(), msg->getLength()),
		    [thisPtr = shared_from_this()](const boost::system::error_code& error, auto /*bytes_transferred*/) {
			    thisPtr->onWriteOperation(error);
		    });
	} catch (boost::system::system_error& e) {
		std::cout << "[Network error - Connection::internalSend] " << e.what() << std::endl;
		close(FORCE_CLOSE);
	}
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
	} else if (connectionState == CONNECTION_STATE_DISCONNECTED) {
		closeSocket();
	}
}

void Connection::handleTimeout(ConnectionWeak_ptr connectionWeak, const boost::system::error_code& error)
{
	if (error == boost::asio::error::operation_aborted) {
		// The timer has been cancelled manually
		return;
	}

	if (auto connection = connectionWeak.lock()) {
		connection->close(FORCE_CLOSE);
	}
}
