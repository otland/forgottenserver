#pragma once
#include "databasetasks.h"

#include <condition_variable>
#include <fstream>
#include <mutex>
#include <queue>
#include <source_location>
#include <string>
#include <thread>

class Logger : public ThreadHolder<Logger>
{
public:
	enum class LogSeverity
	{
		LDEBUG = 0,
		LINFO = 1,
		LWARNING = 2,
		LERROR = 3,
		LFATAL = 4,
	};

	Logger() = default;
	void threadMain();

	void log(Logger::LogSeverity lvl, const char* message,
	         const std::source_location loc = std::source_location::current());

	void setLogLevel(Logger::LogSeverity lvl);
	void setFileNameMaxLenght(size_t fileNameMaxCharacter);

	void shutdown();

private:
	std::string getSeverityString(Logger::LogSeverity lvl);
	std::string getCurrentTime();
	std::string getCurrentThreadID();
	std::string formatFileName(const std::string& file);
	std::string prepareLogMessage(Logger::LogSeverity lvl, const std::string& message, const std::string& file,
	                              int line);
	void rotateLogs();
	void removeFile(std::string name);
	void renameFile(std::string oldName, std::string newName);

	Logger::LogSeverity m_minLogLevel = Logger::LogSeverity::LDEBUG;
	std::string m_filename = "tfs.log";
	std::ofstream m_file;
	std::queue<std::string> m_queue;
	std::mutex m_mutex;
	std::condition_variable m_cv;
	size_t m_fileNameMaxCharacter = 25;
	const size_t m_maxLogFiles = 2;
	const size_t m_maxFileSize = 1024 * 1024;
};

#define LOG_D(message) g_logger.log(Logger::LogSeverity::LDEBUG, message, std::source_location::current())
#define LOG_I(message) g_logger.log(Logger::LogSeverity::LINFO, message, std::source_location::current())
#define LOG_W(message) g_logger.log(Logger::LogSeverity::LWARNING, message, std::source_location::current())
#define LOG_E(message) g_logger.log(Logger::LogSeverity::LERROR, message, std::source_location::current())
#define LOG_F(message) g_logger.log(Logger::LogSeverity::LFATAL, message, std::source_location::current())

extern Logger g_logger;
