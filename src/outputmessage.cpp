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

#include "outputmessage.h"
#include "protocol.h"
#include "scheduler.h"

OutputMessage::OutputMessage()
{
	freeMessage();
}

// OutputMessagePool

OutputMessagePool::OutputMessagePool()
{
	for (uint32_t i = 0; i < OUTPUT_POOL_SIZE; ++i) {
		OutputMessage* msg = new OutputMessage();
		outputMessages.push_back(msg);
	}

	frameTime = OTSYS_TIME();
	m_open = true;
}

void OutputMessagePool::startExecutionFrame()
{
	//std::lock_guard<std::recursive_mutex> lockClass(outputPoolLock);
	frameTime = OTSYS_TIME();
}

OutputMessagePool::~OutputMessagePool()
{
	for (OutputMessage* msg : outputMessages) {
		delete msg;
	}
}

void OutputMessagePool::send(OutputMessage_ptr msg)
{
	outputPoolLock.lock();
	OutputMessage::OutputMessageState state = msg->getState();
	outputPoolLock.unlock();

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
	std::lock_guard<std::recursive_mutex> lockClass(outputPoolLock);

	const int64_t dropTime = frameTime - 10000;
	const int64_t staleTime = frameTime - 10;

	for (OutputMessage_ptr msg : toAddQueue) {
		const int64_t msgFrame = msg->getFrame();
		if (msgFrame >= dropTime) {
			msg->setState(OutputMessage::STATE_ALLOCATED);

			if (msgFrame < staleTime) {
				autoSendOutputMessages.push_front(msg);
			} else {
				autoSendOutputMessages.push_back(msg);
			}
		} else {
			//drop messages that are older than 10 seconds
			msg->getProtocol()->onSendMessage(msg);
		}
	}
	toAddQueue.clear();

	for (auto it = autoSendOutputMessages.begin(), end = autoSendOutputMessages.end(); it != end; it = autoSendOutputMessages.erase(it)) {
		OutputMessage_ptr msg = *it;
		if (staleTime <= msg->getFrame()) {
			break;
		}

		Connection_ptr connection = msg->getConnection();
		if (connection && !connection->send(msg)) {
			// Send only fails when connection is closing (or in error state)
			// This call will free the message
			msg->getProtocol()->onSendMessage(msg);
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

	outputPoolLock.lock();
	outputMessages.push_back(msg);
	outputPoolLock.unlock();
}

OutputMessage_ptr OutputMessagePool::getOutputMessage(Protocol* protocol, bool autosend /*= true*/)
{
	if (!m_open) {
		return OutputMessage_ptr();
	}

	std::lock_guard<std::recursive_mutex> lockClass(outputPoolLock);

	if (!protocol->getConnection()) {
		return OutputMessage_ptr();
	}

	if (outputMessages.empty()) {
		OutputMessage* msg = new OutputMessage();
		outputMessages.push_back(msg);
	}

	OutputMessage_ptr outputmessage;
	outputmessage.reset(outputMessages.back(),
	                    std::bind(&OutputMessagePool::releaseMessage, this, std::placeholders::_1));

	outputMessages.pop_back();

	configureOutputMessage(outputmessage, protocol, autosend);
	return outputmessage;
}

void OutputMessagePool::configureOutputMessage(OutputMessage_ptr msg, Protocol* protocol, bool autosend)
{
	msg->reset();

	if (autosend) {
		msg->setState(OutputMessage::STATE_ALLOCATED);
		autoSendOutputMessages.push_back(msg);
	} else {
		msg->setState(OutputMessage::STATE_ALLOCATED_NO_AUTOSEND);
	}

	Connection_ptr connection = protocol->getConnection();
	assert(connection);

	msg->setProtocol(protocol);
	protocol->addRef();

	msg->setConnection(connection);
	connection->addRef();

	msg->setFrame(frameTime);
}

void OutputMessagePool::addToAutoSend(OutputMessage_ptr msg)
{
	outputPoolLock.lock();
	toAddQueue.push_back(msg);
	outputPoolLock.unlock();
}
