#ifndef FS_DATABASEDISPATCHER_H // no idea of the checksum used for defines here.
#define FS_DATABASEDISPATCHER_H

#include "database.h"

#include <queue>
#include <condition_variable>

enum DBCommand_t {
	INSERT,
	DELETE,
	SELECT,
	UPDATE,
	TRANSACTION
};

class DatabaseDispatcher
{
private:
	typedef std::shared_ptr<std::function<void(DBResult_ptr)>> Callback_ptr;
	typedef std::shared_ptr<std::function<void()>> TransactionFunc_ptr;

	struct DBCommand
	{
		DBCommand_t type;
		std::string query;
		Callback_ptr callback;
		DBInsert_ptr insertStmt;
		DBTransaction_ptr transaction;
		TransactionFunc_ptr transactionFunc;

		DBCommand() {}
		DBCommand(DBCommand_t _type,
				  std::string _query,
				  Callback_ptr _callback,
				  DBInsert_ptr _insertStmt,
				  DBTransaction_ptr _transaction,
				  TransactionFunc_ptr _transactionFunc):
			type(_type), query(_query), callback(_callback),
			insertStmt(_insertStmt), transaction(_transaction),
			transactionFunc(_transactionFunc) {}
	};

	DatabaseDispatcher();
	~DatabaseDispatcher();

	void processQueue();

	std::queue<DBCommand> m_sqlCommandQueue;

	std::recursive_mutex m_queueMutex;
	std::mutex m_waitQueueMutex;
	std::condition_variable m_QueueAddCondition;

	std::unique_ptr<std::thread> m_executionThread;

public:
	static DatabaseDispatcher* getInstance()
	{
		static DatabaseDispatcher instance;
		return &instance;
	}

	void queueSqlCommand(DBCommand_t type,
						 std::string query,
						 Callback_ptr callback = nullptr,
						 DBInsert_ptr insertStmt = nullptr,
						 DBTransaction_ptr transaction = nullptr,
						 TransactionFunc_ptr transactionFunc = nullptr);

	void transactionToExecute(std::unique_ptr<DBTransaction> transaction); // nao pode ser um ponteiro
};

extern DatabaseDispatcher g_databaseDispatcher;
#endif
