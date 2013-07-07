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
		uint32_t updateDatabase();

		bool getDatabaseConfig(const std::string& config, int32_t& value);
		void registerDatabaseConfig(const std::string& config, int32_t value);

		bool getDatabaseConfig(const std::string& config, std::string& value);
		void registerDatabaseConfig(const std::string& config, const std::string& value);

		void checkEncryption();
		void checkTriggers();
};
#endif
