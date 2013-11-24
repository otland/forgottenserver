/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2013  Mark Samman <mark.samman@gmail.com>
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

#ifndef LOGGER_H
#define LOGGER_H

#ifdef __GNUC__
#define __OTSERV_PRETTY_FUNCTION__ __PRETTY_FUNCTION__
#endif
#ifdef _MSC_VER
#define __OTSERV_PRETTY_FUNCTION__ __FUNCDNAME__
#endif

/*
#define LOG_MESSAGE(channel, type, level, message) \
	Logger::getInstance()->logMessage(channel, type, level, message, __OTSERV_PRETTY_FUNCTION__, __LINE__, __FILE__);
*/

#define LOG_MESSAGE(channel, type, level, message) \
	Logger::getInstance()->logMessage(channel, type, level, message, __OTSERV_PRETTY_FUNCTION__);

enum eLogType {
	LOGTYPE_EVENT,
	LOGTYPE_WARNING,
	LOGTYPE_ERROR,
};

class Logger
{
	public:
		~Logger();
		static Logger* getInstance() {
			static Logger instance;
			return &instance;
		}

		void logMessage(const char* channel, eLogType type, int32_t level, const std::string& message, const char* func);

	private:
		FILE* m_file;
		Logger();
};

#endif
