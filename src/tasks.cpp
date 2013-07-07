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

#include "tasks.h"
#include "outputmessage.h"
#include "game.h"

extern Game g_game;

Dispatcher::Dispatcher()
{
	m_taskList.clear();
	m_threadState = STATE_TERMINATED;
}

void Dispatcher::start()
{
	m_threadState = STATE_RUNNING;
	m_thread = boost::thread(boost::bind(&Dispatcher::dispatcherThread, (void*)this));
}

void Dispatcher::dispatcherThread(void* p)
{
	Dispatcher* dispatcher = (Dispatcher*)p;

	OutputMessagePool* outputPool;

	// NOTE: second argument defer_lock is to prevent from immediate locking
	boost::unique_lock<boost::mutex> taskLockUnique(dispatcher->m_taskLock, boost::defer_lock);

	while (dispatcher->m_threadState != STATE_TERMINATED) {
		Task* task = NULL;

		// check if there are tasks waiting
		taskLockUnique.lock();

		if (dispatcher->m_taskList.empty()) {
			//if the list is empty wait for signal
			dispatcher->m_taskSignal.wait(taskLockUnique);
		}

		if (!dispatcher->m_taskList.empty() && (dispatcher->m_threadState != STATE_TERMINATED)) {
			// take the first task
			task = dispatcher->m_taskList.front();
			dispatcher->m_taskList.pop_front();
		}

		taskLockUnique.unlock();

		// finally execute the task...
		if (task) {
			if (!task->hasExpired()) {
				OutputMessagePool::getInstance()->startExecutionFrame();
				(*task)();

				outputPool = OutputMessagePool::getInstance();

				if (outputPool) {
					outputPool->sendAll();
				}

				g_game.clearSpectatorCache();
			}

			delete task;
		}
	}
}

void Dispatcher::addTask(Task* task, bool push_front /*= false*/)
{
	bool do_signal = false;

	m_taskLock.lock();

	if (m_threadState == STATE_RUNNING) {
		do_signal = m_taskList.empty();

		if (push_front) {
			m_taskList.push_front(task);
		} else {
			m_taskList.push_back(task);
		}
	} else {
		delete task;
		task = NULL;
	}

	m_taskLock.unlock();

	// send a signal if the list was empty
	if (do_signal) {
		m_taskSignal.notify_one();
	}
}

void Dispatcher::flush()
{
	Task* task = NULL;

	while (!m_taskList.empty()) {
		task = m_taskList.front();
		m_taskList.pop_front();
		(*task)();
		delete task;
		OutputMessagePool* outputPool = OutputMessagePool::getInstance();

		if (outputPool) {
			outputPool->sendAll();
		}

		g_game.clearSpectatorCache();
	}
}

void Dispatcher::stop()
{
	m_taskLock.lock();
	m_threadState = STATE_CLOSING;
	m_taskLock.unlock();
}

void Dispatcher::shutdown()
{
	m_taskLock.lock();
	m_threadState = STATE_TERMINATED;
	flush();
	m_taskLock.unlock();
}

void Dispatcher::join()
{
	m_thread.join();
}
