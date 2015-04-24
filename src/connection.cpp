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

#include "otpch.h"

#include "configmanager.h"
#include "connection.h"
#include "outputmessage.h"
#include "protocol.h"
#include "scheduler.h"
#include "server.h"

bool Connection::m_logError = true;

extern ConfigManager g_config;

Connection_ptr ConnectionManager::createConnection(boost::asio::ip::tcp::socket* socket,
        boost::asio::io_service& io_service, ServicePort_ptr servicer)
{
	std::lock_guard<std::mutex> lockClass(m_connectionManagerLock);

	auto connection = std::make_shared<Connection>(socket, io_service, servicer);
	m_connections.insert(connection);
	return connection;
}

void ConnectionManager::releaseConnection(const Connection_ptr& connection)
{
	std::lock_guard<std::mutex> lockClass(m_connectionManagerLock);

	m_connections.erase(connection);
}

void ConnectionManager::closeAll()
{
	std::lock_guard<std::mutex> lockClass(m_connectionManagerLock);

	for (const auto& connection : m_connections) {
		assert(connection.use_count() == 1);
		try {
			boost::system::error_code error;
			connection->m_socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
			connection->m_socket->close(error);
		} catch (boost::system::system_error&) {
		}
	}
	m_connections.clear();
}

// Connection

void Connection::close()
{
	//any thread
	ConnectionManager::getInstance().releaseConnection(shared_from_this());

	std::lock_guard<std::recursive_mutex> lockClass(m_connectionLock);
	if (m_connectionState != CONNECTION_STATE_OPEN) {
		return;
	}
	m_connectionState = CONNECTION_STATE_CLOSED;
	
	if (m_protocol) {
		g_dispatcher.addTask(
			createTask(std::bind(&Protocol::release, m_protocol)));
	}
	
	if (!m_pendingWrite || m_writeError) {
		closeSocket();
	} else {
		//will be closed by onWriteOperation/handleWriteTimeout/handleReadTimeout instead
	}
}

void Connection::closeSocket()
{
	if (m_socket->is_open()) {
		m_pendingRead = false;
		m_pendingWrite = false;

		try {
			m_readTimer.cancel();
			m_writeTimer.cancel();
			boost::system::error_code error;
			m_socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
			m_socket->close(error);
		} catch (boost::system::system_error& e) {
			if (m_logError) {
				std::cout << "[Network error - Connection::closeSocket] " << e.what() << std::endl;
				m_logError = false;
			}
		}
	}
}

Connection::~Connection()
{
	assert(m_connectionState != CONNECTION_STATE_OPEN); //If this fires, no one closed this Connection and we're being deleted - something went wrong
	closeSocket();
}

void Connection::accept(Protocol_ptr protocol)
{
	m_protocol = protocol;
	g_dispatcher.addTask(createTask(std::bind(&Protocol::onConnect, m_protocol)));

	accept();
}

void Connection::accept()
{
	std::lock_guard<std::recursive_mutex> lockClass(m_connectionLock);
	try {
		m_pendingRead = true;
		m_readTimer.expires_from_now(boost::posix_time::seconds(Connection::read_timeout));
		m_readTimer.async_wait(std::bind(&Connection::handleReadTimeout, std::weak_ptr<Connection>(shared_from_this()), std::placeholders::_1));

		// Read size of the first packet
		boost::asio::async_read(*m_socket,
		                        boost::asio::buffer(m_msg.getBuffer(), NetworkMessage::header_length),
		                        std::bind(&Connection::parseHeader, shared_from_this(), std::placeholders::_1));
	} catch (boost::system::system_error& e) {
		if (m_logError) {
			std::cout << "[Network error - Connection::accept] " << e.what() << std::endl;
			m_logError = false;
		}

		close();
	}
}

void Connection::parseHeader(const boost::system::error_code& error)
{
	std::lock_guard<std::recursive_mutex> lockClass(m_connectionLock);
	m_readTimer.cancel();

	int32_t size = m_msg.decodeHeader();
	if (error || size <= 0 || size >= NETWORKMESSAGE_MAXSIZE - 16) {
		handleReadError(error);
	}

	if (m_connectionState != CONNECTION_STATE_OPEN || m_readError) {
		close();
		return;
	}

	uint32_t timePassed = std::max<uint32_t>(1, (time(nullptr) - m_timeConnected) + 1);
	if ((++m_packetsSent / timePassed) > static_cast<uint32_t>(g_config.getNumber(ConfigManager::MAX_PACKETS_PER_SECOND))) {
		std::cout << convertIPToString(getIP()) << " disconnected for exceeding packet per second limit." << std::endl;
		close();
		return;
	}

	if (timePassed > 2) {
		m_timeConnected = time(nullptr);
		m_packetsSent = 0;
	}

	try {
		m_readTimer.expires_from_now(boost::posix_time::seconds(Connection::read_timeout));
		m_readTimer.async_wait( std::bind(&Connection::handleReadTimeout, std::weak_ptr<Connection>(shared_from_this()),
		                                    std::placeholders::_1));

		// Read packet content
		m_msg.setLength(size + NetworkMessage::header_length);
		boost::asio::async_read(*m_socket, boost::asio::buffer(m_msg.getBodyBuffer(), size),
		                        std::bind(&Connection::parsePacket, shared_from_this(), std::placeholders::_1));
	} catch (boost::system::system_error& e) {
		if (m_logError) {
			std::cout << "[Network error - Connection::parseHeader] " << e.what() << std::endl;
			m_logError = false;
		}

		close();
	}
}

void Connection::parsePacket(const boost::system::error_code& error)
{
	std::lock_guard<std::recursive_mutex> lockClass(m_connectionLock);
	m_readTimer.cancel();

	if (error) {
		handleReadError(error);
	}

	if (m_connectionState != CONNECTION_STATE_OPEN || m_readError) {
		close();
		return;
	}

	//Check packet checksum
	uint32_t checksum;
	int32_t len = m_msg.getLength() - m_msg.getBufferPosition() - 4;
	if (len > 0) {
		checksum = adlerChecksum(m_msg.getBuffer() + m_msg.getBufferPosition() + 4, len);
	} else {
		checksum = 0;
	}

	uint32_t recvChecksum = m_msg.get<uint32_t>();
	if (recvChecksum != checksum) {
		// it might not have been the checksum, step back
		m_msg.skipBytes(-4);
	}

	if (!m_receivedFirst) {
		// First message received
		m_receivedFirst = true;

		if (!m_protocol) {
			// Game protocol has already been created at this point
			m_protocol = m_service_port->make_protocol(recvChecksum == checksum, m_msg, shared_from_this());
			if (!m_protocol) {
				close();
				return;
			}

		} else {
			m_msg.skipBytes(1);    // Skip protocol ID
		}

		m_protocol->onRecvFirstMessage(m_msg);
	} else {
		m_protocol->onRecvMessage(m_msg);    // Send the packet to the current protocol
	}

	try {
		m_readTimer.expires_from_now(boost::posix_time::seconds(Connection::read_timeout));
		m_readTimer.async_wait( std::bind(&Connection::handleReadTimeout, std::weak_ptr<Connection>(shared_from_this()),
		                                    std::placeholders::_1));

		// Wait to the next packet
		boost::asio::async_read(*m_socket,
		                        boost::asio::buffer(m_msg.getBuffer(), NetworkMessage::header_length),
		                        std::bind(&Connection::parseHeader, shared_from_this(), std::placeholders::_1));
	} catch (boost::system::system_error& e) {
		if (m_logError) {
			std::cout << "[Network error - Connection::parsePacket] " << e.what() << std::endl;
			m_logError = false;
		}

		close();
	}
}

void Connection::send(OutputMessage_ptr msg)
{
	std::lock_guard<std::recursive_mutex> lockClass(m_connectionLock);
	if (m_connectionState != CONNECTION_STATE_OPEN || m_writeError) {
		return;
	}

	if (!m_pendingWrite) {
		internalSend(msg);
	} else {
		messageQueue.push(msg);
	}
}

void Connection::internalSend(OutputMessage_ptr msg)
{
	m_protocol->onSendMessage(msg);
	try {
		m_pendingWrite = true;
		m_writeTimer.expires_from_now(boost::posix_time::seconds(Connection::write_timeout));
		m_writeTimer.async_wait( std::bind(&Connection::handleWriteTimeout, std::weak_ptr<Connection>(shared_from_this()),
		                                     std::placeholders::_1));

		boost::asio::async_write(*m_socket,
		                         boost::asio::buffer(msg->getOutputBuffer(), msg->getLength()),
		                         std::bind(&Connection::onWriteOperation, shared_from_this(), msg, std::placeholders::_1));
	} catch (boost::system::system_error& e) {
		if (m_logError) {
			std::cout << "[Network error - Connection::internalSend] " << e.what() << std::endl;
			m_logError = false;
		}
	}
}

uint32_t Connection::getIP()
{
	std::lock_guard<std::recursive_mutex> lockClass(m_connectionLock);
	//Ip is expressed in network byte order
	boost::system::error_code error;
	const boost::asio::ip::tcp::endpoint endpoint = m_socket->remote_endpoint(error);

	if (error) {
		return 0;
	}

	return htonl(endpoint.address().to_v4().to_ulong());
}

void Connection::onWriteOperation(OutputMessage_ptr msg, const boost::system::error_code& error)
{
	std::lock_guard<std::recursive_mutex> lockClass(m_connectionLock);
	m_writeTimer.cancel();

	if (error) {
		handleWriteError(error);
	}

	if (m_connectionState != CONNECTION_STATE_OPEN || m_writeError) {
		close();
		closeSocket();
		return;
	}

	m_pendingWrite = false;
	if (!messageQueue.empty()) {
		internalSend(std::move(messageQueue.front()));
		messageQueue.pop();
	}
}

void Connection::handleReadError(const boost::system::error_code& error)
{
	std::lock_guard<std::recursive_mutex> lockClass(m_connectionLock);

	if (error != boost::asio::error::operation_aborted) {
		/**
		 * error == boost::asio::error::eof:
		 *  No more to read
		 * error == boost::asio::error::connection_reset || error == boost::asio::error::connection_aborted:
		 *  Connection closed remotely
		 */
		close();
	}

	m_readError = true;
}

void Connection::onReadTimeout()
{
	std::lock_guard<std::recursive_mutex> lockClass(m_connectionLock);

	if (m_pendingRead || m_readError) {
		close();
		closeSocket();
	}
}

void Connection::onWriteTimeout()
{
	std::lock_guard<std::recursive_mutex> lockClass(m_connectionLock);

	if (m_pendingWrite || m_writeError) {
		close();
		closeSocket();
	}
}

void Connection::handleReadTimeout(ConnectionWeak_ptr weak_conn, const boost::system::error_code& error)
{
	if (error == boost::asio::error::operation_aborted) {
		return;
	}

	if (auto connection = weak_conn.lock()) {
		connection->onReadTimeout();
	}
}

void Connection::handleWriteError(const boost::system::error_code& error)
{
	if (error != boost::asio::error::operation_aborted) {
		/**
		 * error == boost::asio::error::eof:
		 *  No more to read
		 * error == boost::asio::error::connection_reset || error == boost::asio::error::connection_aborted:
		 *  Connection closed remotely
		 */
		close();
	}

	m_writeError = true;
}

void Connection::handleWriteTimeout(ConnectionWeak_ptr weak_conn, const boost::system::error_code& error)
{
	if (error == boost::asio::error::operation_aborted) {
		return;
	}

	if (auto connection = weak_conn.lock()) {
		connection->onWriteTimeout();
	}
}
