//////////////////////////////////////////////////////////////////////
// The Forgotten Server - a server application for the MMORPG Tibia
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#include "otpch.h"

#include "logger.h"
#include "tools.h"

#include <ctime>
#include <iostream>

Logger::Logger()
{
	m_file = fopen("data/logs/otadmin.log", "a");
	if (!m_file) {
		m_file = stderr;
	}
}

Logger::~Logger()
{
	if (m_file) {
		fclose(m_file);
	}
}

void Logger::logMessage(const char* channel, eLogType type, int32_t level, const std::string& message, const char* func)
{
	fprintf(m_file, "%s", formatDate(time(NULL)).c_str());

	if (channel) {
		fprintf(m_file, " [%s] ", channel);
	}

	if (strcmp(func, "") != 0) {
		fprintf(m_file, " %s ", func);
	}

	std::string type_str;

	switch (type) {
		case LOGTYPE_EVENT:
			type_str = "event";
			break;
		case LOGTYPE_WARNING:
			type_str = "warning";
			break;
		case LOGTYPE_ERROR:
			type_str = "error";
			break;
		default:
			type_str = "unknown";
			break;
	}

	fprintf(m_file, " %s:", type_str.c_str());
	fprintf(m_file, " %s\n", message.c_str());
	fflush(m_file);
}
