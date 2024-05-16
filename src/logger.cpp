#include "otpch.h"

#include "Logger.h"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>

void Logger::setLogLevel(LogSeverity lvl) { config.minLogLevel = lvl; }

void Logger::setFileNameMaxLenght(size_t fileNameMaxCharacter) { fileNameMaxCharacter = fileNameMaxCharacter; }

void Logger::log(LogSeverity lvl, const char *message, const std::source_location loc)
{
	bool do_signal = false;

	m_mutex.lock();

	if (getState() == THREAD_STATE_RUNNING) {
		do_signal = m_queue.empty();
		if (config.minLogLevel <= lvl) {
			std::string msg = prepareLogMessage(lvl, std::string(message), loc.file_name(), loc.line());
			m_queue.push(msg);
			std::cout << msg << std::endl;
		}
	}
	m_mutex.unlock();

	// send a signal if the list was empty
	if (do_signal) {
		m_cv.notify_one();
	}
}

Logger::Logger() { loadConfig(); }

void Logger::logLua(LogSeverity lvl, const char *message, const std::string &file, int line)
{
	bool do_signal = false;

	m_mutex.lock();

	if (getState() == THREAD_STATE_RUNNING) {
		do_signal = m_queue.empty();
		if (config.minLogLevel <= lvl) {
			std::string msg = prepareLogMessage(lvl, std::string(message), file, line);
			std::cout << msg << std::endl;
			m_queue.push(std::move(msg));
		}
	}
	m_mutex.unlock();

	// send a signal if the list was empty
	if (do_signal) {
		m_cv.notify_one();
	}
}

void Logger::threadMain()
{
	std::queue<std::string> tempLogQueue;
	std::unique_lock<std::mutex> m_mutexUnique(m_mutex, std::defer_lock);

	while (getState() != THREAD_STATE_TERMINATED) {
		m_mutexUnique.lock();
		if (m_queue.empty()) {
			m_cv.wait(m_mutexUnique);
		}
		tempLogQueue.swap(m_queue);

		m_mutexUnique.unlock();

		file.open(config.filename + ".log", std::ios::out | std::ios::app | std::ios::ate);
		if (!file.is_open()) {
			return;
		}

		while (!tempLogQueue.empty()) {
			file << tempLogQueue.front() << std::endl;
			tempLogQueue.pop();
			try {
				auto fileSize = std::filesystem::file_size(config.filename + ".log");
				if (config.maxFileSize < fileSize) {
					file.close();
					rotateLogs();
					file.open(config.filename + ".log", std::ios::out | std::ios::app | std::ios::ate);
				}
			} catch (const std::filesystem::filesystem_error &ex) {
				std::cout << "Fatal error while handling logger files: " << ex.what() << std::endl;
			}
		}

		file.close();
	}
}

std::string Logger::getSeverityString(LogSeverity lvl)
{
	switch (lvl) {
		case LogSeverity::LINFO:
			return "_I_";
		case LogSeverity::LWARNING:
			return "_W_";
		case LogSeverity::LERROR:
			return "_E_";
		case LogSeverity::LFATAL:
			return "_F_";
		case LogSeverity::LDEBUG:
			return "_D_";
		default:
			// undefinied
			return "_U_";
	}
}

std::string Logger::getCurrentTime()
{
	auto timePoint = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(timePoint);
	std::chrono::system_clock::duration tp = timePoint.time_since_epoch();

	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(tp);
	auto fractional_seconds = std::chrono::duration_cast<std::chrono::duration<int, std::milli>>(tp - seconds);

	std::tm timeInfo;
#ifdef _WIN32
	localtime_s(&timeInfo, &time);
#else
	localtime_r(&time, &timeInfo);
#endif

	std::stringstream ss;
	ss << std::put_time(&timeInfo, "%Y:%d  %I:%M:%S") << "." << std::setfill('0') << std::setw(3)
	   << fractional_seconds.count();
	return ss.str();
}

std::string Logger::getCurrentThreadID()
{
	std::stringstream ss;
	ss << std::this_thread::get_id();
	return ss.str();
}

std::string Logger::formatFileName(const std::string &file)
{
	const size_t threeDotsSize = 3;
	int fileNameMaxCharacter = std::max<uint16_t>(0, config.fileNameMaxCharacter - threeDotsSize);

	if (file.length() >= fileNameMaxCharacter) {
		return "..." + file.substr(file.length() - fileNameMaxCharacter);
	} else {
		return file + std::string(fileNameMaxCharacter - file.length(), ' ');
	}
}

std::string Logger::prepareLogMessage(LogSeverity lvl, const std::string &message, const std::string &file, int line)
{
	std::stringstream ss;
	const char *bar = " | ";

	ss << getCurrentTime() << bar;
	ss << std::setw(config.threadIdSize) << std::left << getCurrentThreadID() << bar;
	ss << std::setw(config.logSeveritySize) << std::left << getSeverityString(lvl) << bar;
	ss << std::setw(config.fileNameMaxCharacter) << std::left << formatFileName(file) << ":" << std::setw(5) << line
	   << bar;
	ss << message;

	return ss.str();
}

void Logger::shutdown()
{
	m_mutex.lock();
	setState(THREAD_STATE_TERMINATED);
	m_mutex.unlock();
	m_cv.notify_one();
}

void Logger::removeFile(std::string name)
{
	if (std::filesystem::exists(name)) {
		std::filesystem::remove(name);
	}
}

void Logger::renameFile(std::string oldName, std::string newName)
{
	if (std::filesystem::exists(oldName)) {
		std::rename(oldName.c_str(), newName.c_str());
	}
}

void Logger::rotateLogs()
{
	std::string afix = ".log";
	removeFile(config.filename + std::to_string(config.maxLogFiles - 1) + afix);
	for (size_t i = config.maxLogFiles - 1; i > 0; i--) {
		renameFile(config.filename + ((i - 1) ? std::to_string(i - 1) : "") + afix,
		           config.filename + std::to_string(i) + afix);
	}
	renameFile(config.filename + ".log", config.filename + std::to_string(1) + afix);
}

static std::string trim(const std::string &str)
{
	size_t start = str.find_first_not_of(" \t\n\r");
	size_t end = str.find_last_not_of(" \t\n\r");
	return (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

static bool isValidKeyFormat(const std::string &str)
{
	return std::all_of(str.begin(), str.end(), [](char c) { return std::isalnum(c) || c == '_'; });
}

void Logger::loadConfig()
{
	std::ifstream file("logger.conf");

	if (!file.is_open()) {
		return;
	}

	std::map<std::string, std::string> configs;

	std::string line;
	while (std::getline(file, line)) {
		// skip comments
		if (line.empty() || line[0] == '#') {
			printf(line.c_str());
			continue;
		}

		size_t pos = line.find('=');
		if (pos == std::string::npos) continue;

		std::string key = trim(line.substr(0, pos));
		std::string value = trim(line.substr(pos + 1));

		if (isValidKeyFormat(key)) {
			configs[key] = value;
		}
	}
	updateConfig(configs);
}

LogSeverity getLogLevel(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	if (str == "debug") {
		return LogSeverity::LDEBUG;
	}
	if (str == "info") {
		return LogSeverity::LINFO;
	}
	if (str == "warning") {
		return LogSeverity::LWARNING;
	}
	if (str == "error") {
		return LogSeverity::LERROR;
	}
	if (str == "fatal") {
		return LogSeverity::LFATAL;
	}
	return LogSeverity::LDEBUG;
}

void Logger::updateConfig(std::map<std::string, std::string> &configMap)
{
	try {
		auto itLogLevel = configMap.find("log_level");
		if (itLogLevel != configMap.end()) {
			config.minLogLevel = getLogLevel(itLogLevel->second);
		}

		auto itFileCount = configMap.find("max_log_files");
		if (itFileCount != configMap.end() && std::stoi(itFileCount->second)) {
			config.maxLogFiles = std::stoi(itFileCount->second);
		}

		auto itFileSize = configMap.find("log_file_size");
		if (itFileSize != configMap.end() && std::stoi(itFileSize->second)) {
			config.maxFileSize = std::stoi(itFileSize->second);
		}

		auto itLofFileName = configMap.find("log_file_name");
		if (itLofFileName != configMap.end()) {
			config.filename = itLofFileName->second;
		}
	} catch (const std::exception &ex) {
		std::cout << "Fail when parsing config file: " << ex.what() << std::endl;
	}
}
