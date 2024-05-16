#pragma once
#include "databasetasks.h"

#include <condition_variable>
#include <fstream>
#include <mutex>
#include <queue>
#include <source_location>
#include <string>
#include <thread>

enum LogSeverity
{
	LDEBUG = 0,
	LINFO = 1,
	LWARNING = 2,
	LERROR = 3,
	LFATAL = 4,
};

class Logger : public ThreadHolder<Logger>
{
public:
	Logger();
	void threadMain();
	void log(LogSeverity lvl, const char* message, const std::source_location loc = std::source_location::current());
	void logLua(LogSeverity lvl, const char* message, const std::string& file, int line);
	void shutdown();

private:
	struct Config
	{
		const uint8_t logSeveritySize = 3;
		const uint8_t threadIdSize = 6;
		std::string filename = "tfs";
		LogSeverity minLogLevel = LogSeverity::LDEBUG;
		uint32_t maxLogFiles = static_cast<uint32_t>(2);
		uint32_t maxFileSize = static_cast<uint32_t>(1048576);
		uint32_t fileNameMaxCharacter = static_cast<uint32_t>(35);
	};

	std::string getSeverityString(LogSeverity lvl);
	std::string getCurrentTime();
	std::string getCurrentThreadID();
	std::string formatFileName(const std::string& file);
	std::string prepareLogMessage(LogSeverity lvl, const std::string& message, const std::string& file, int line);

	void rotateLogs();
	void removeFile(std::string name);
	void renameFile(std::string oldName, std::string newName);

	void loadConfig();
	void updateConfig(std::map<std::string, std::string>& configMap);

	Config config;
	std::queue<std::string> m_queue;
	std::mutex m_mutex;
	std::condition_variable m_cv;
	std::ofstream file;
};

#define LOG_D(message) g_logger.log(LogSeverity::LDEBUG, message, std::source_location::current())
#define LOG_I(message) g_logger.log(LogSeverity::LINFO, message, std::source_location::current())
#define LOG_W(message) g_logger.log(LogSeverity::LWARNING, message, std::source_location::current())
#define LOG_E(message) g_logger.log(LogSeverity::LERROR, message, std::source_location::current())
#define LOG_F(message) g_logger.log(LogSeverity::LFATAL, message, std::source_location::current())

extern Logger g_logger;
