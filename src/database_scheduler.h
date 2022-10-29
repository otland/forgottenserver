// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_DATABASETASKS_H
#define FS_DATABASETASKS_H

#include "database.h"
#include "thread_holder_base.h"

struct DatabaseTask
{
	DatabaseTask(std::string&& query, std::function<void(DBResult_ptr, bool)>&& callback, bool store) :
	    query(std::move(query)), callback(std::move(callback)), store(store)
	{}

	std::string query;
	std::function<void(DBResult_ptr, bool)> callback;
	bool store;
};

class DatabaseScheduler : public ThreadHolder<DatabaseScheduler>
{
public:
	DatabaseScheduler() = default;
	void start();
	void flush();
	void shutdown();

	void addTask(std::string query, std::function<void(DBResult_ptr, bool)> callback = nullptr, bool store = false);

	void run();

private:
	void runTask(const DatabaseTask& task);

	Database db;
	std::thread thread;
	std::list<DatabaseTask> tasks;
	std::mutex taskLock;
	std::condition_variable taskSignal;
};

extern DatabaseScheduler g_databaseScheduler;

#endif // FS_DATABASETASKS_H
