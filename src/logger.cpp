#include "otpch.h"

#include "Logger.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>

void Logger::setLogLevel(Logger::LogSeverity lvl) { m_minLogLevel = lvl; }

void Logger::setFileNameMaxLenght(size_t fileNameMaxCharacter) { m_fileNameMaxCharacter = fileNameMaxCharacter; }

void Logger::log(Logger::LogSeverity lvl, const char *message, const std::source_location loc)
{
	bool do_signal = false;

	m_mutex.lock();

	if (getState() == THREAD_STATE_RUNNING) {
		do_signal = m_queue.empty();
		if (m_minLogLevel <= lvl) {
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

		m_file.open(m_filename, std::ios::out | std::ios::app | std::ios::ate);
		if (!m_file.is_open()) {
			return;
		}

		while (!tempLogQueue.empty()) {
			m_file << tempLogQueue.front() << std::endl;
			tempLogQueue.pop();
			try {
				auto fileSize = std::filesystem::file_size(m_filename);
				if (m_maxFileSize < fileSize) {
					m_file.close();
					rotateLogs();
					m_file.open(m_filename, std::ios::out | std::ios::app | std::ios::ate);
				}
			} catch (const std::filesystem::filesystem_error &ex) {
			}
		}

		m_file.close();
	}
}

std::string Logger::getSeverityString(Logger::LogSeverity lvl)
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
	int fileNameMaxCharacter = std::max(static_cast<int>(0), static_cast<int>(m_fileNameMaxCharacter - threeDotsSize));

	if (file.length() >= fileNameMaxCharacter) {
		return "..." + file.substr(file.length() - fileNameMaxCharacter);
	} else {
		return file + std::string(fileNameMaxCharacter - file.length(), ' ');
	}
}

std::string Logger::prepareLogMessage(Logger::LogSeverity lvl, const std::string &message, const std::string &file,
                                      int line)
{
	std::stringstream ss;
	ss << std::setw(10) << getCurrentTime() << " | ";
	ss << std::setw(6) << std::left << getCurrentThreadID() << " |";
	ss << std::setw(3) << std::left << getSeverityString(lvl) << " | ";
	ss << std::setw(m_fileNameMaxCharacter) << std::left << formatFileName(file) << ":" << std::setw(5) << line
	   << " | ";
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
	std::string prefix = "tfs";
	std::string afix = ".log";
	removeFile(prefix + std::to_string(m_maxLogFiles -1) + afix);
	for (size_t i = m_maxLogFiles - 1; i > 0; i--) {
		renameFile(prefix + ((i - 1) ? std::to_string(i - 1) : "") + afix, prefix + std::to_string(i) + afix);
	}
	renameFile(m_filename, prefix + std::to_string(1) + afix);
}
