//////////////////////////////////////////////////////////////////////
// The Forgotten Server - a server application for the MMORPG Tibia
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#include "otpch.h"

#include "outputmessage.h"
#include "protocol.h"
#include "scheduler.h"

extern Dispatcher g_dispatcher;

#ifdef __ENABLE_SERVER_DIAGNOSTIC__
uint32_t OutputMessagePool::OutputMessagePoolCount = OUTPUT_POOL_SIZE;
#endif

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
	//boost::recursive_mutex::scoped_lock lockClass(m_outputPoolLock);
	m_frameTime = OTSYS_TIME();
	m_isOpen = true;
}

OutputMessagePool::~OutputMessagePool()
{
	InternalOutputMessageList::iterator it;

	for (it = m_outputMessages.begin(); it != m_outputMessages.end(); ++it) {
		delete *it;
	}

	m_outputMessages.clear();
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
	boost::recursive_mutex::scoped_lock lockClass(m_outputPoolLock);

	const int64_t dropTime = m_frameTime - 10000;
	const int64_t frameTime = m_frameTime - 10;

	for (OutputMessageMessageList::const_iterator it = m_toAddQueue.begin(), end = m_toAddQueue.end(); it != end; ++it) {
		OutputMessage_ptr omsg = *it;
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

	for (OutputMessageMessageList::iterator it = m_autoSendOutputMessages.begin(), end = m_autoSendOutputMessages.end(); it != end; it = m_autoSendOutputMessages.erase(it)) {
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
	    createTask(boost::bind(&OutputMessagePool::internalReleaseMessage, this, msg)));
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

	boost::recursive_mutex::scoped_lock lockClass(m_outputPoolLock);

	if (protocol->getConnection() == NULL) {
		return OutputMessage_ptr();
	}

	if (m_outputMessages.empty()) {
		OutputMessage* msg = new OutputMessage();
		m_outputMessages.push_back(msg);

#ifdef __ENABLE_SERVER_DIAGNOSTIC__
		OutputMessagePoolCount++;
#endif
	}

	OutputMessage_ptr outputmessage;
	outputmessage.reset(m_outputMessages.back(),
	                    boost::bind(&OutputMessagePool::releaseMessage, this, _1));

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
	assert(connection != NULL);

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
