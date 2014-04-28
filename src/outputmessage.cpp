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

#include "outputmessage.h"
#include "protocol.h"
#include "scheduler.h"

OutputMessage::OutputMessage()
{
	freeMessage();
}

//*********** OutputMessagePool ****************//

OutputMessagePool::OutputMessagePool()
{
	for (uint32_t i = 0; i < OUTPUT_POOL_SIZE; ++i) {
		OutputMessage* msg = new OutputMessage();
		m_outputMessages.push_back(msg);
	}

	m_frameTime = OTSYS_TIME();
}

void OutputMessagePool::startExecutionFrame()
{
	//std::lock_guard<std::recursive_mutex> lockClass(m_outputPoolLock);
	m_frameTime = OTSYS_TIME();
	m_isOpen = true;
}

OutputMessagePool::~OutputMessagePool()
{
	for (OutputMessage* msg : m_outputMessages) {
		delete msg;
	}
}

void OutputMessagePool::send(OutputMessage_ptr msg)
{
	m_outputPoolLock.lock();
	OutputMessage::OutputMessageState state = msg->getState();
	m_outputPoolLock.unlock();

	if (state == OutputMessage::STATE_ALLOCATED_NO_AUTOSEND) {
		Connection_ptr connection = msg->getConnection();
		if (connection && !connection->send(msg)) {
			// Send only fails when connection is closing (or in error state)
			// This call will free the message
			msg->getProtocol()->onSendMessage(msg);
		}
	}
}

void OutputMessagePool::sendAll()
{
	std::lock_guard<std::recursive_mutex> lockClass(m_outputPoolLock);

	const int64_t dropTime = m_frameTime - 10000;
	const int64_t frameTime = m_frameTime - 10;

	for (OutputMessage_ptr omsg : m_toAddQueue) {
		const int64_t msgFrame = omsg->getFrame();
		if (msgFrame >= dropTime) {
			omsg->setState(OutputMessage::STATE_ALLOCATED);

			if (frameTime > msgFrame) {
				m_autoSendOutputMessages.push_front(omsg);
			} else {
				m_autoSendOutputMessages.push_back(omsg);
			}
		} else {
			//drop messages that are older than 10 seconds
			omsg->getProtocol()->onSendMessage(omsg);
		}
	}
	m_toAddQueue.clear();

	for (auto it = m_autoSendOutputMessages.begin(), end = m_autoSendOutputMessages.end(); it != end; it = m_autoSendOutputMessages.erase(it)) {
		OutputMessage_ptr omsg = *it;
		if (frameTime <= omsg->getFrame()) {
			break;
		}

		Connection_ptr connection = omsg->getConnection();
		if (connection && !connection->send(omsg)) {
			// Send only fails when connection is closing (or in error state)
			// This call will free the message
			omsg->getProtocol()->onSendMessage(omsg);
		}
	}
}

void OutputMessagePool::releaseMessage(OutputMessage* msg)
{
	g_dispatcher.addTask(
	    createTask(std::bind(&OutputMessagePool::internalReleaseMessage, this, msg)));
}

void OutputMessagePool::internalReleaseMessage(OutputMessage* msg)
{
	if (msg->getProtocol()) {
		msg->getProtocol()->unRef();
	} else {
		std::cout << "No protocol found." << std::endl;
	}

	if (msg->getConnection()) {
		msg->getConnection()->unRef();
	} else {
		std::cout << "No connection found." << std::endl;
	}

	msg->freeMessage();

	m_outputPoolLock.lock();
	m_outputMessages.push_back(msg);
	m_outputPoolLock.unlock();
}

OutputMessage_ptr OutputMessagePool::getOutputMessage(Protocol* protocol, bool autosend /*= true*/)
{
	if (!m_isOpen) {
		return OutputMessage_ptr();
	}

	std::lock_guard<std::recursive_mutex> lockClass(m_outputPoolLock);

	if (!protocol->getConnection()) {
		return OutputMessage_ptr();
	}

	if (m_outputMessages.empty()) {
		OutputMessage* msg = new OutputMessage();
		m_outputMessages.push_back(msg);
	}

	OutputMessage_ptr outputmessage;
	outputmessage.reset(m_outputMessages.back(),
	                    std::bind(&OutputMessagePool::releaseMessage, this, std::placeholders::_1));

	m_outputMessages.pop_back();

	configureOutputMessage(outputmessage, protocol, autosend);
	return outputmessage;
}

void OutputMessagePool::configureOutputMessage(OutputMessage_ptr msg, Protocol* protocol, bool autosend)
{
	msg->Reset();

	if (autosend) {
		msg->setState(OutputMessage::STATE_ALLOCATED);
		m_autoSendOutputMessages.push_back(msg);
	} else {
		msg->setState(OutputMessage::STATE_ALLOCATED_NO_AUTOSEND);
	}

	Connection_ptr connection = protocol->getConnection();
	assert(connection);

	msg->setProtocol(protocol);
	protocol->addRef();

	msg->setConnection(connection);
	connection->addRef();

	msg->setFrame(m_frameTime);
}

void OutputMessagePool::addToAutoSend(OutputMessage_ptr msg)
{
	m_outputPoolLock.lock();
	m_toAddQueue.push_back(msg);
	m_outputPoolLock.unlock();
}
