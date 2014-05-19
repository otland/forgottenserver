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
                                         Callback_ptr callback,
                                         DBInsert_ptr insertStmt,
                                         DBTransaction_ptr transaction,
                                         TransactionFunc_ptr transactionFunc, bool mustExecute)
{
    DBCommand sqlCommand(type, query, callback, insertStmt, transaction, transactionFunc, mustExecute);

    m_queueMutex.lock();
    m_sqlCommandQueue.push(sqlCommand);
    m_queueMutex.unlock();
}

void DatabaseDispatcher::processQueue()
{
    while (true)
    {
        std::unique_lock<std::mutex> waitAdd(m_waitQueueMutex);
        while (m_sqlCommandQueue.empty())
        {
            m_QueueAddCondition.wait(waitAdd);
        }

        m_queueMutex.lock();
        DBCommand sqlCommand = m_sqlCommandQueue.front();
        m_sqlCommandQueue.pop();
        m_queueMutex.unlock();

        sqlCommand.tries++;

        switch(sqlCommand.type)
        {
            case(DBCommand_t::INSERT):
                break;
            case(DBCommand_t::SELECT):
                sqlCommand.result = Database::getInstance()->storeQuery(sqlCommand.query);
                if (sqlCommand.result != nullptr)
                    sqlCommand.succeeded = true;
                break;
            case(DBCommand_t::UPDATE):
            case(DBCommand_t::DELETE):
                if (Database::getInstance()->executeQuery(sqlCommand.query))
                    sqlCommand.succeeded = true;
                break;
            case(DBCommand_t::TRANSACTION):
                break;
        }

        if (!sqlCommand.succeeded)
        {
            if (sqlCommand.tries >= 3 && !sqlCommand.mustExecute)
            {
                // TODO: log failure.
                // Give up trying.
            } else {
                m_queueMutex.lock();
                m_sqlCommandQueue.push(sqlCommand); // let's put it in the end of the line.
                m_queueMutex.unlock();
            }
        } else if (sqlCommand.callback != nullptr)
        {
            (*sqlCommand.callback)(sqlCommand.result);
        }
    }
}
