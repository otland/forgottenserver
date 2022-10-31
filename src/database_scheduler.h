// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_DATABASE_SCHEDULER_H
#define FS_DATABASE_SCHEDULER_H

#include "database.h"
#include "scheduler_holder_base.h"

struct DatabaseTask
{
	DatabaseTask(std::string&& query, std::function<void(DBResult_ptr, bool)>&& callback, bool store) :
	    query(std::move(query)), callback(std::move(callback)), store(store)
	{}

	std::string query;
	std::function<void(DBResult_ptr, bool)> callback;
	bool store;
};

class DatabaseScheduler final : virtual public SchedulerHolder
{
public:
	DatabaseScheduler() = default;
	void start();
	void flush();

	void addTask(std::string query, std::function<void(DBResult_ptr, bool)> callback = nullptr, bool store = false);

	void run() final;
	void shutdown() final;

private:
	void runTask(const DatabaseTask& task);

	Database db;

	std::list<DatabaseTask> taskList;
};

extern DatabaseScheduler g_databaseScheduler;

#endif // FS_DATABASE_SCHEDULER_H
