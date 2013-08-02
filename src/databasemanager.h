/**
 * The Forgotten Server - a server application for the MMORPG Tibia
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

#ifndef __DATABASE_MANAGER__
#define __DATABASE_MANAGER__
#include "database.h"

class DatabaseManager
{
	public:
		DatabaseManager() {}
		virtual ~DatabaseManager() {}

		static DatabaseManager* getInstance() {
			static DatabaseManager instance;
			return &instance;
		}

		bool tableExists(const std::string& table);
		bool triggerExists(const std::string& trigger);

		int32_t getDatabaseVersion();
		bool isDatabaseSetup();

		bool optimizeTables();
		void updateDatabase();

		bool getDatabaseConfig(const std::string& config, int32_t& value);
		void registerDatabaseConfig(const std::string& config, int32_t value);

		bool getDatabaseConfig(const std::string& config, std::string& value);
		void registerDatabaseConfig(const std::string& config, const std::string& value);

		void checkEncryption();
		void checkTriggers();
};
#endif
