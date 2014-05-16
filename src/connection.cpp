/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2014  Mark Samman <mark.samman@gmail.com>
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
	std::lock_guard<std::recursive_mutex> lockClass(m_connectionManagerLock);

	Connection_ptr connection = Connection_ptr(new Connection(socket, io_service, servicer));
	m_connections.insert(connection);
	return connection;
}

void ConnectionManager::releaseConnection(Connection_ptr connection)
{
	std::lock_guard<std::recursive_mutex> lockClass(m_connectionManagerLock);

	m_connections.erase(connection);
}

void ConnectionManager::closeAll()
{
	std::lock_guard<std::recursive_mutex> lockClass(m_connectionManagerLock);

	for (const Connection_ptr& connection : m_connections) {
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

void Connection::closeConnection()
{
	//any thread
	std::lock_guard<std::recursive_mutex> lockClass(m_connectionLock);

	if (m_connectionState == CONNECTION_STATE_CLOSING || m_connectionState == CONNECTION_STATE_CLOSED || m_connectionState == CONNECTION_STATE_REQUEST_CLOSE) {
		return;
	}

	m_connectionState = CONNECTION_STATE_REQUEST_CLOSE;

	g_dispatcher.addTask(
	    createTask(std::bind(&Connection::closeConnectionTask, this)));
}

void Connection::closeConnectionTask()
{
	//dispatcher thread
	m_connectionLock.lock();

	if (m_connectionState != CONNECTION_STATE_REQUEST_CLOSE) {
		std::cout << "Error: [Connection::closeConnectionTask] m_connectionState = " << m_connectionState << std::endl;
		m_connectionLock.unlock();
		return;
	}

	if (m_protocol) {
		m_protocol->setConnection(Connection_ptr());
		m_protocol->releaseProtocol();
		m_protocol = nullptr;
	}

	m_connectionState = CONNECTION_STATE_CLOSING;

	if (m_pendingWrite == 0 || m_writeError) {
		closeSocket();
		releaseConnection();
		m_connectionState = CONNECTION_STATE_CLOSED;
	} else {
		//will be closed by onWriteOperation/handleWriteTimeout/handleReadTimeout instead
	}

	m_connectionLock.unlock();
}

void Connection::closeSocket()
{
	m_connectionLock.lock();

	if (m_socket->is_open()) {
		m_pendingRead = 0;
		m_pendingWrite = 0;

		try {
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

	m_connectionLock.unlock();
}

void Connection::releaseConnection()
{
	if (m_refCount > 0) {
		//Reschedule it and try again.
		g_scheduler.addEvent(createSchedulerTask(SCHEDULER_MINTICKS, std::bind(&Connection::releaseConnection, this)));
	} else {
		deleteConnectionTask();
	}
}

void Connection::onStopOperation()
{
	//io_service thread
	m_connectionLock.lock();

	m_readTimer.cancel();
	m_writeTimer.cancel();

	try {
		if (m_socket->is_open()) {
			boost::system::error_code error;
			m_socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
			m_socket->close();
		}
	} catch (boost::system::system_error&) {
		//
	}

	delete m_socket;
	m_socket = nullptr;

	m_connectionLock.unlock();
	ConnectionManager::getInstance()->releaseConnection(shared_from_this());
}

void Connection::deleteConnectionTask()
{
	//dispather thread
	assert(m_refCount == 0);

	try {
		m_io_service.dispatch(std::bind(&Connection::onStopOperation, this));
	} catch (boost::system::system_error& e) {
		if (m_logError) {
			std::cout << "[Network error - Connection::deleteConnectionTask] " << e.what() << std::endl;
			m_logError = false;
		}
	}
}

void Connection::acceptConnection(Protocol* protocol)
{
	m_protocol = protocol;
	g_dispatcher.addTask(createTask(std::bind(&Protocol::onConnect, m_protocol)));

	acceptConnection();
}

void Connection::acceptConnection()
{
	try {
		++m_pendingRead;
		m_readTimer.expires_from_now(boost::posix_time::seconds(Connection::read_timeout));
		m_readTimer.async_wait(std::bind(&Connection::handleReadTimeout, std::weak_ptr<Connection>(shared_from_this()), std::placeholders::_1));

		// Read size of the first packet
		boost::asio::async_read(getHandle(),
		                        boost::asio::buffer(m_msg.getBuffer(), NetworkMessage::header_length),
		                        std::bind(&Connection::parseHeader, shared_from_this(), std::placeholders::_1));
	} catch (boost::system::system_error& e) {
		if (m_logError) {
			std::cout << "[Network error - Connection::acceptConnection] " << e.what() << std::endl;
			m_logError = false;
		}

		closeConnection();
	}
}

void Connection::parseHeader(const boost::system::error_code& error)
{
	m_connectionLock.lock();
	m_readTimer.cancel();

	int32_t size = m_msg.decodeHeader();
	if (error || size <= 0 || size >= NETWORKMESSAGE_MAXSIZE - 16) {
		handleReadError(error);
	}

	if (m_connectionState != CONNECTION_STATE_OPEN || m_readError) {
		closeConnection();
		m_connectionLock.unlock();
		return;
	}

	uint32_t timePassed = std::max<uint32_t>(1, (time(nullptr) - m_timeConnected) + 1);
	if ((++m_packetsSent / timePassed) > (uint32_t)g_config.getNumber(ConfigManager::MAX_PACKETS_PER_SECOND)) {
		std::cout << convertIPToString(getIP()) << " disconnected for exceeding packet per second limit." << std::endl;
		closeConnection();
		m_connectionLock.unlock();
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
		m_msg.setMessageLength(size + NetworkMessage::header_length);
		boost::asio::async_read(getHandle(), boost::asio::buffer(m_msg.getBodyBuffer(), size),
		                        std::bind(&Connection::parsePacket, shared_from_this(), std::placeholders::_1));
	} catch (boost::system::system_error& e) {
		if (m_logError) {
			std::cout << "[Network error - Connection::parseHeader] " << e.what() << std::endl;
			m_logError = false;
		}

		closeConnection();
	}

	m_connectionLock.unlock();
}

void Connection::parsePacket(const boost::system::error_code& error)
{
	m_connectionLock.lock();
	m_readTimer.cancel();

	if (error) {
		handleReadError(error);
	}

	if (m_connectionState != CONNECTION_STATE_OPEN || m_readError) {
		closeConnection();
		m_connectionLock.unlock();
		return;
	}

	//Check packet checksum
	uint32_t checksum;
	int32_t len = m_msg.getMessageLength() - m_msg.getReadPos() - 4;
	if (len > 0) {
		checksum = adlerChecksum(m_msg.getBuffer() + m_msg.getReadPos() + 4, len);
	} else {
		checksum = 0;
	}

	uint32_t recvChecksum = m_msg.get<uint32_t>();
	if (recvChecksum != checksum) {
		// it might not have been the checksum, step back
		m_msg.SkipBytes(-4);
	}

	if (!m_receivedFirst) {
		// First message received
		m_receivedFirst = true;

		if (!m_protocol) {
			// Game protocol has already been created at this point
			m_protocol = m_service_port->make_protocol(recvChecksum == checksum, m_msg);
			if (!m_protocol) {
				closeConnection();
				m_connectionLock.unlock();
				return;
			}

			m_protocol->setConnection(shared_from_this());
		} else {
			m_msg.GetByte();    // Skip protocol ID
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
		boost::asio::async_read(getHandle(),
		                        boost::asio::buffer(m_msg.getBuffer(), NetworkMessage::header_length),
		                        std::bind(&Connection::parseHeader, shared_from_this(), std::placeholders::_1));
	} catch (boost::system::system_error& e) {
		if (m_logError) {
			std::cout << "[Network error - Connection::parsePacket] " << e.what() << std::endl;
			m_logError = false;
		}

		closeConnection();
	}

	m_connectionLock.unlock();
}

bool Connection::send(OutputMessage_ptr msg)
{
	m_connectionLock.lock();

	if (m_connectionState != CONNECTION_STATE_OPEN || m_writeError) {
		m_connectionLock.unlock();
		return false;
	}

	if (m_pendingWrite == 0) {
		msg->getProtocol()->onSendMessage(msg);
		internalSend(msg);
	} else {
		OutputMessagePool::getInstance()->addToAutoSend(msg);
	}

	m_connectionLock.unlock();
	return true;
}

void Connection::internalSend(OutputMessage_ptr msg)
{
	try {
		++m_pendingWrite;
		m_writeTimer.expires_from_now(boost::posix_time::seconds(Connection::write_timeout));
		m_writeTimer.async_wait( std::bind(&Connection::handleWriteTimeout, std::weak_ptr<Connection>(shared_from_this()),
		                                     std::placeholders::_1));

		boost::asio::async_write(getHandle(),
		                         boost::asio::buffer(msg->getOutputBuffer(), msg->getMessageLength()),
		                         std::bind(&Connection::onWriteOperation, shared_from_this(), msg, std::placeholders::_1));
	} catch (boost::system::system_error& e) {
		if (m_logError) {
			std::cout << "[Network error - Connection::internalSend] " << e.what() << std::endl;
			m_logError = false;
		}
	}
}

uint32_t Connection::getIP() const
{
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
	m_connectionLock.lock();
	m_writeTimer.cancel();

	msg.reset();

	if (error) {
		handleWriteError(error);
	}

	if (m_connectionState != CONNECTION_STATE_OPEN || m_writeError) {
		closeSocket();
		closeConnection();
		m_connectionLock.unlock();
		return;
	}

	--m_pendingWrite;
	m_connectionLock.unlock();
}

void Connection::handleReadError(const boost::system::error_code& error)
{
	std::lock_guard<std::recursive_mutex> lockClass(m_connectionLock);

	if (error == boost::asio::error::operation_aborted) {
		// Operation aborted because connection will be closed
		// Do NOT call closeConnection() from here
	} else {
		/**
		 * error == boost::asio::error::eof:
		 *  No more to read
		 * error == boost::asio::error::connection_reset || error == boost::asio::error::connection_aborted:
		 *  Connection closed remotely
		 */
		closeConnection();
	}

	m_readError = true;
}

void Connection::onReadTimeout()
{
	std::lock_guard<std::recursive_mutex> lockClass(m_connectionLock);

	if (m_pendingRead > 0 || m_readError) {
		closeSocket();
		closeConnection();
	}
}

void Connection::onWriteTimeout()
{
	std::lock_guard<std::recursive_mutex> lockClass(m_connectionLock);

	if (m_pendingWrite > 0 || m_writeError) {
		closeSocket();
		closeConnection();
	}
}

void Connection::handleReadTimeout(std::weak_ptr<Connection> weak_conn, const boost::system::error_code& error)
{
	if (error == boost::asio::error::operation_aborted) {
		return;
	}

	if (weak_conn.expired()) {
		return;
	}

	if (Connection_ptr connection = weak_conn.lock()) {
		connection->onReadTimeout();
	}
}

void Connection::handleWriteError(const boost::system::error_code& error)
{
	std::lock_guard<std::recursive_mutex> lockClass(m_connectionLock);

	if (error == boost::asio::error::operation_aborted) {
		// Operation aborted because connection will be closed
		// Do NOT call closeConnection() from here
	} else {
		/**
		 * error == boost::asio::error::eof:
		 *  No more to read
		 * error == boost::asio::error::connection_reset || error == boost::asio::error::connection_aborted:
		 *  Connection closed remotely
		 */
		closeConnection();
	}

	m_writeError = true;
}

void Connection::handleWriteTimeout(std::weak_ptr<Connection> weak_conn, const boost::system::error_code& error)
{
	if (error == boost::asio::error::operation_aborted) {
		return;
	}

	if (weak_conn.expired()) {
		return;
	}

	if (Connection_ptr connection = weak_conn.lock()) {
		connection->onWriteTimeout();
	}
}
