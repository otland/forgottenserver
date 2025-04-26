#include "otpch.h"

#include "stats.h"

#include "configmanager.h"
#include "tasks.h"
#include "tools.h"

#include <fstream>

int64_t Stats::DUMP_INTERVAL = 30000;
uint32_t Stats::SLOW_EXECUTION_TIME = 10000000;
uint32_t Stats::VERY_SLOW_EXECUTION_TIME = 50000000;

AutoStatRecursive* AutoStatRecursive::activeStat = nullptr;

void Stats::threadMain()
{
	std::unique_lock<std::mutex> taskLockUnique(statsLock, std::defer_lock);
	bool last_iteration = false;
	lua.lastDump = sql.lastDump = special.lastDump = OTSYS_TIME();
	playersOnline = 0;

	while (true) {
		taskLockUnique.lock();

		DUMP_INTERVAL = getNumber(ConfigManager::STATS_DUMP_INTERVAL) * 1000;
		SLOW_EXECUTION_TIME = getNumber(ConfigManager::STATS_SLOW_LOG_TIME) * 1000000;
		VERY_SLOW_EXECUTION_TIME = getNumber(ConfigManager::STATS_VERY_SLOW_LOG_TIME) * 1000000;

		std::forward_list<Stat*> lua_stats(std::move(lua.queue));
		lua.queue.clear();
		std::forward_list<Stat*> sql_stats(std::move(sql.queue));
		sql.queue.clear();
		std::forward_list<Stat*> special_stats(std::move(special.queue));
		special.queue.clear();
		taskLockUnique.unlock();

		parseLuaQueue(lua_stats);
		parseSqlQueue(sql_stats);
		parseSpecialQueue(special_stats);

		if (lua.lastDump + DUMP_INTERVAL < OTSYS_TIME() || last_iteration) {
			writeStats("lua.log", lua.stats);
			lua.stats.clear();
			lua.lastDump = OTSYS_TIME();
		}
		if (sql.lastDump + DUMP_INTERVAL < OTSYS_TIME() || last_iteration) {
			writeStats("sql.log", sql.stats);
			sql.stats.clear();
			sql.lastDump = OTSYS_TIME();
		}
		if (special.lastDump + DUMP_INTERVAL < OTSYS_TIME() || last_iteration) {
			writeStats("special.log", special.stats);
			special.stats.clear();
			special.lastDump = OTSYS_TIME();
		}

		if (last_iteration) break;
		if (getState() == THREAD_STATE_TERMINATED) {
			last_iteration = true;
			continue;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void Stats::addLuaStats(Stat* stats)
{
	std::lock_guard<std::mutex> lockClass(statsLock);
	lua.queue.push_front(stats);
}

void Stats::addSqlStats(Stat* stats)
{
	std::lock_guard<std::mutex> lockClass(statsLock);
	sql.queue.push_front(stats);
}

void Stats::addSpecialStats(Stat* stats)
{
	std::lock_guard<std::mutex> lockClass(statsLock);
	special.queue.push_front(stats);
}

void Stats::parseLuaQueue(std::forward_list<Stat*>& queue)
{
	for (Stat* stats : queue) {
		auto it = lua.stats.emplace(stats->description, statsData(0, 0, stats->extraDescription)).first;
		it->second.calls += 1;
		it->second.executionTime += stats->executionTime;

		if (VERY_SLOW_EXECUTION_TIME > 0 && stats->executionTime > VERY_SLOW_EXECUTION_TIME) {
			writeSlowInfo("lua_very_slow.log", stats->executionTime, stats->description, stats->extraDescription);
		} else if (SLOW_EXECUTION_TIME > 0 && stats->executionTime > SLOW_EXECUTION_TIME) {
			writeSlowInfo("lua_slow.log", stats->executionTime, stats->description, stats->extraDescription);
		}
		delete stats;
	}
}

void Stats::parseSqlQueue(std::forward_list<Stat*>& queue)
{
	for (Stat* stats : queue) {
		auto it = sql.stats.emplace(stats->description, statsData(0, 0, stats->extraDescription)).first;
		it->second.calls += 1;
		it->second.executionTime += stats->executionTime;

		if (VERY_SLOW_EXECUTION_TIME > 0 && stats->executionTime > VERY_SLOW_EXECUTION_TIME) {
			writeSlowInfo("sql_very_slow.log", stats->executionTime, stats->description, stats->extraDescription);
		} else if (SLOW_EXECUTION_TIME > 0 && stats->executionTime > SLOW_EXECUTION_TIME) {
			writeSlowInfo("sql_slow.log", stats->executionTime, stats->description, stats->extraDescription);
		}
		delete stats;
	}
}

void Stats::parseSpecialQueue(std::forward_list<Stat*>& queue)
{
	for (Stat* stats : queue) {
		auto it = special.stats.emplace(stats->description, statsData(0, 0, stats->extraDescription)).first;
		it->second.calls += 1;
		it->second.executionTime += stats->executionTime;

		if (VERY_SLOW_EXECUTION_TIME > 0 && stats->executionTime > VERY_SLOW_EXECUTION_TIME) {
			writeSlowInfo("special_very_slow.log", stats->executionTime, stats->description, stats->extraDescription);
		} else if (SLOW_EXECUTION_TIME > 0 && stats->executionTime > SLOW_EXECUTION_TIME) {
			writeSlowInfo("special_slow.log", stats->executionTime, stats->description, stats->extraDescription);
		}
		delete stats;
	}
}

void Stats::writeSlowInfo(const std::string& file, uint64_t executionTime, const std::string& description,
                          const std::string& extraDescription)
{
	std::ofstream out(std::string("stats/") + file, std::ofstream::out | std::ofstream::app);
	if (!out.is_open()) {
		std::clog << "Can't open " << std::string("stats/") + file << " (check if directory exists)" << std::endl;
		return;
	}
	out << "[" << formatDate(time(nullptr)) << "] Execution time: " << (executionTime / 1000000) << " ms - "
	    << description << " - " << extraDescription << "\n";
	out.flush();
	out.close();
}

void Stats::writeStats(const std::string& file, const statsMap& stats, const std::string& extraInfo) const
{
	if (DUMP_INTERVAL == 0) {
		return;
	}

	std::ofstream out(std::string("stats/") + file, std::ofstream::out | std::ofstream::app);
	if (!out.is_open()) {
		std::clog << "Can't open " << std::string("stats/") + file << " (check if directory exists)" << std::endl;
		return;
	}
	if (stats.empty()) {
		out.close();
		return;
	}
	out << "[" << formatDate(time(nullptr)) << "] Players online: " << playersOnline << "\n";

	std::vector<std::pair<std::string, statsData>> pairs;
	for (auto& it : stats) pairs.emplace_back(it);

	sort(pairs.begin(), pairs.end(),
	     [=](const std::pair<std::string, statsData>& a, const std::pair<std::string, statsData>& b) {
		     return a.second.executionTime > b.second.executionTime;
	     });

	out << extraInfo;
	float total_time = 0;
	out << std::setw(10) << "Time (ms)" << std::setw(10) << "Calls" << std::setw(15) << "Rel usage " << "%"
	    << std::setw(15) << "Real usage " << "%" << " " << "Description" << "\n";
	for (auto& it : pairs) total_time += it.second.executionTime;
	for (auto& it : pairs) {
		float percent = 100 * static_cast<float>(it.second.executionTime) / total_time;
		float realPercent = static_cast<float>(it.second.executionTime) / (static_cast<float>(DUMP_INTERVAL) * 10000.);
		if (percent > 0.1)
			out << std::setw(10) << it.second.executionTime / 1000000 << std::setw(10) << it.second.calls
			    << std::setw(15) << std::setprecision(5) << std::fixed << percent << "%" << std::setw(15)
			    << std::setprecision(5) << std::fixed << realPercent << "%" << " " << it.first << "\n";
	}
	out << "\n";
	out.flush();
	out.close();
}
