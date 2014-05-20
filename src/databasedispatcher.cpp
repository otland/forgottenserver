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

#include "tasks.h"
#include "databasedispatcher.h"

DatabaseDispatcher::DatabaseDispatcher()
{
	m_executionThread = std::unique_ptr<std::thread>(new std::thread(&DatabaseDispatcher::processQueue, this));
}

DatabaseDispatcher::~DatabaseDispatcher()
{
}

void DatabaseDispatcher::queueSqlCommand(DBCommand_t type,
										 std::string query,
										 DBCallback callback,
										 DBInsert_ptr insertStmt,
										 DBTransaction_ptr transaction,
										 TransactionFunc_ptr transactionFunc)
{
	DBCommand sqlCommand(type, query, callback, insertStmt, transaction, transactionFunc);

	m_queueMutex.lock();
	m_sqlCommandQueue.push(sqlCommand);
	m_queueMutex.unlock();
}

void DatabaseDispatcher::processQueue()
{
	bool success = false;
	DBResult_ptr result;

	while (true)
	{
		std::unique_lock<std::mutex> waitAdd(m_waitQueueMutex);
		while (m_sqlCommandQueue.empty())
		{
			m_QueueAddCondition.wait(waitAdd);
		}

		m_queueMutex.lock();
		DBCommand sqlCommand = m_sqlCommandQueue.front();
		m_queueMutex.unlock();

		switch(sqlCommand.type)
		{
		case(DBCommand_t::INSERT):
			break;
		case(DBCommand_t::SELECT):
			result = Database::getInstance()->storeQuery(sqlCommand.query);
			if (result != nullptr)
				success = true;
			break;
		case(DBCommand_t::UPDATE):
		case(DBCommand_t::DELETE):
			if (Database::getInstance()->executeQuery(sqlCommand.query))
				success = true;
			break;
		case(DBCommand_t::TRANSACTION):
			break;
		}

		if (!success)
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		else {
			m_sqlCommandQueue.pop();

			if (sqlCommand.type == DBCommand_t::SELECT && sqlCommand.callback != nullptr)
				g_dispatcher.addTask(createTask(std::bind(sqlCommand.callback,result)));
		}
	}
}
