// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_LOGGER_H
#define FS_LOGGER_H

#include "position.h"

#include <chrono>
#include <fmt/format.h>

enum class LogLevel
{
	TRACE,
	DEBUG,
	INFO,
	WARNING,
	ERRORR,
	CRITICAL
};

class Logger
{
public:
	virtual ~Logger() = default;

	virtual void setLevel(LogLevel level) = 0;
	virtual LogLevel getLevel() const = 0;
	virtual bool isEnabled(LogLevel level) const = 0;

	void trace([[maybe_unused]] std::string_view msg)
	{
#if !defined(NDEBUG) || defined(DEBUG_LOG)
		if (isEnabled(LogLevel::TRACE)) {
			log(LogLevel::TRACE, msg);
		}
#endif
	}

	void debug([[maybe_unused]] std::string_view msg)
	{
#if !defined(NDEBUG) || defined(DEBUG_LOG)
		if (isEnabled(LogLevel::DEBUG)) {
			log(LogLevel::DEBUG, msg);
		}
#endif
	}

	void info(std::string_view msg)
	{
		if (isEnabled(LogLevel::INFO)) {
			log(LogLevel::INFO, msg);
		}
	}

	void warn(std::string_view msg)
	{
		if (isEnabled(LogLevel::WARNING)) {
			log(LogLevel::WARNING, msg);
		}
	}

	void error(std::string_view msg)
	{
		if (isEnabled(LogLevel::ERRORR)) {
			log(LogLevel::ERRORR, msg);
		}
	}

	void critical(std::string_view msg)
	{
		if (isEnabled(LogLevel::CRITICAL)) {
			log(LogLevel::CRITICAL, msg);
		}
	}

	template <typename... Args>
	void trace(fmt::format_string<Args...> fmt, Args&&... args)
	{
#if !defined(NDEBUG) || defined(DEBUG_LOG)
		if (isEnabled(LogLevel::TRACE)) {
			log(LogLevel::TRACE, fmt::format(fmt, std::forward<Args>(args)...));
		}
#endif
	}

	template <typename... Args>
	void debug(fmt::format_string<Args...> fmt, Args&&... args)
	{
#if !defined(NDEBUG) || defined(DEBUG_LOG)
		if (isEnabled(LogLevel::DEBUG)) {
			log(LogLevel::DEBUG, fmt::format(fmt, std::forward<Args>(args)...));
		}
#endif
	}

	template <typename... Args>
	void info(fmt::format_string<Args...> fmt, Args&&... args)
	{
		if (isEnabled(LogLevel::INFO)) {
			log(LogLevel::INFO, fmt::format(fmt, std::forward<Args>(args)...));
		}
	}

	template <typename... Args>
	void warn(fmt::format_string<Args...> fmt, Args&&... args)
	{
		if (isEnabled(LogLevel::WARNING)) {
			log(LogLevel::WARNING, fmt::format(fmt, std::forward<Args>(args)...));
		}
	}

	template <typename... Args>
	void error(fmt::format_string<Args...> fmt, Args&&... args)
	{
		if (isEnabled(LogLevel::ERRORR)) {
			log(LogLevel::ERRORR, fmt::format(fmt, std::forward<Args>(args)...));
		}
	}

	template <typename... Args>
	void critical(fmt::format_string<Args...> fmt, Args&&... args)
	{
		if (isEnabled(LogLevel::CRITICAL)) {
			log(LogLevel::CRITICAL, fmt::format(fmt, std::forward<Args>(args)...));
		}
	}

	template <typename F>
	auto profile(std::string_view name, F&& func)
	{
		if (!isEnabled(LogLevel::INFO)) {
			return std::forward<F>(func)();
		}

		auto start = std::chrono::steady_clock::now();
		try {
			auto result = std::forward<F>(func)();
			auto end = std::chrono::steady_clock::now();
			info("{} took {} ms", name, std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
			return result;
		}

		catch (...) {
			auto end = std::chrono::steady_clock::now();
			error("{} failed after {} ms", name,
			      std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
			throw;
		}
	}

protected:
	virtual void log(LogLevel level, std::string_view message) = 0;
};

Logger& g_logger();
bool initLogger(LogLevel level = LogLevel::INFO, std::string_view filePath = "data/logs/server.log",
                size_t rotateSize = 5 * 1024 * 1024, size_t rotateFiles = 3);
void shutdownLogger();
bool isLoggerInitialized();
LogLevel parseLogLevel(std::string_view level);

// Signal-safe shutdown functions
void setupLoggerSignalHandlers();
void loggerSignalHandler(int signal);

#define LOG_TRACE(...) \
	do { \
		if (isLoggerInitialized()) g_logger().trace(__VA_ARGS__); \
	} while (0)
#define LOG_DEBUG(...) \
	do { \
		if (isLoggerInitialized()) g_logger().debug(__VA_ARGS__); \
	} while (0)
#define LOG_INFO(...) \
	do { \
		if (isLoggerInitialized()) g_logger().info(__VA_ARGS__); \
	} while (0)
#define LOG_WARN(...) \
	do { \
		if (isLoggerInitialized()) g_logger().warn(__VA_ARGS__); \
	} while (0)
#define LOG_ERROR(...) \
	do { \
		if (isLoggerInitialized()) g_logger().error(__VA_ARGS__); \
	} while (0)
#define LOG_CRITICAL(...) \
	do { \
		if (isLoggerInitialized()) g_logger().critical(__VA_ARGS__); \
	} while (0)

template <typename T>
concept EnumLike = std::is_enum_v<T>;

template <EnumLike T>
struct fmt::formatter<T> : fmt::formatter<int>
{
	template <typename FormatContext>
	auto format(T value, FormatContext& ctx)
	{
		return fmt::formatter<int>::format(static_cast<int>(value), ctx);
	}
};

template <>
struct fmt::formatter<Position>
{
	constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const Position& pos, FormatContext& ctx) const
	{
		return fmt::format_to(ctx.out(), "({}, {}, {})", pos.x, pos.y, pos.z);
	}
};

#endif // FS_LOGGER_H
