// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "databasemanager.h"

#include "configmanager.h"
#include "database.h"
#include "lua/api.h"
#include "lua/modules.h"
#include "lua/register.h"
#include "lua/script.h"

bool DatabaseManager::optimizeTables()
{
	Database& db = Database::getInstance();

	const auto& result = db.storeQuery(std::format(
	    "SELECT `TABLE_NAME` FROM `information_schema`.`TABLES` WHERE `TABLE_SCHEMA` = {:s} AND `DATA_FREE` > 0",
	    db.escapeString(getString(ConfigManager::MYSQL_DB))));
	if (!result) {
		return false;
	}

	do {
		const auto tableName = result->getString("TABLE_NAME");
		std::cout << "> Optimizing table " << tableName << "..." << std::flush;

		if (db.executeQuery(std::format("OPTIMIZE TABLE `{:s}`", tableName))) {
			std::cout << " [success]" << std::endl;
		} else {
			std::cout << " [failed]" << std::endl;
		}
	} while (result->next());
	return true;
}

bool DatabaseManager::tableExists(const std::string& tableName)
{
	Database& db = Database::getInstance();
	return db
	    .storeQuery(std::format(
	        "SELECT `TABLE_NAME` FROM `information_schema`.`tables` WHERE `TABLE_SCHEMA` = {:s} AND `TABLE_NAME` = {:s} LIMIT 1",
	        db.escapeString(getString(ConfigManager::MYSQL_DB)), db.escapeString(tableName)))
	    .get();
}

bool DatabaseManager::isDatabaseSetup()
{
	Database& db = Database::getInstance();
	return db
	    .storeQuery(std::format("SELECT `TABLE_NAME` FROM `information_schema`.`tables` WHERE `TABLE_SCHEMA` = {:s}",
	                            db.escapeString(getString(ConfigManager::MYSQL_DB))))
	    .get();
}

int32_t DatabaseManager::getDatabaseVersion()
{
	if (!tableExists("server_config")) {
		Database& db = Database::getInstance();
		db.executeQuery(
		    "CREATE TABLE `server_config` (`config` VARCHAR(50) NOT NULL, `value` VARCHAR(256) NOT NULL DEFAULT '', UNIQUE(`config`)) ENGINE = InnoDB");
		db.executeQuery("INSERT INTO `server_config` VALUES ('db_version', 0)");
		return 0;
	}

	int32_t version = 0;
	if (getDatabaseConfig("db_version", version)) {
		return version;
	}
	return -1;
}

void DatabaseManager::updateDatabase()
{
	LuaScriptInterface lsi{"Database Interface"};
	if (!lsi.initState()) {
		return;
	}

	luaL_openlibs(lsi.getLuaState());

	tfs::lua::registerDatabase(lsi);

	int32_t version = getDatabaseVersion();
	do {
		if (luaL_dofile(lsi.getLuaState(), std::format("data/migrations/{:d}.lua", version).c_str()) != 0) {
			std::cout << "[Error - DatabaseManager::updateDatabase - Version: " << version << "] "
			          << lua_tostring(lsi.getLuaState(), -1) << std::endl;
			break;
		}

		if (!tfs::lua::reserveScriptEnv()) {
			break;
		}

		lua_getglobal(lsi.getLuaState(), "onUpdateDatabase");
		if (lua_pcall(lsi.getLuaState(), 0, 1, 0) != 0) {
			tfs::lua::resetScriptEnv();
			std::cout << "[Error - DatabaseManager::updateDatabase - Version: " << version << "] "
			          << lua_tostring(lsi.getLuaState(), -1) << std::endl;
			break;
		}

		if (!tfs::lua::getBoolean(lsi.getLuaState(), -1, false)) {
			tfs::lua::resetScriptEnv();
			break;
		}

		version++;
		std::cout << "> Database has been updated to version " << version << '.' << std::endl;
		registerDatabaseConfig("db_version", version);

		tfs::lua::resetScriptEnv();
	} while (true);
}

bool DatabaseManager::getDatabaseConfig(const std::string& config, int32_t& value)
{
	Database& db = Database::getInstance();

	const auto& result = db.storeQuery(
	    std::format("SELECT `value` FROM `server_config` WHERE `config` = {:s}", db.escapeString(config)));
	if (!result) {
		return false;
	}

	value = result->getNumber<int32_t>("value");
	return true;
}

void DatabaseManager::registerDatabaseConfig(const std::string& config, int32_t value)
{
	Database& db = Database::getInstance();

	int32_t tmp;

	if (!getDatabaseConfig(config, tmp)) {
		db.executeQuery(
		    std::format("INSERT INTO `server_config` VALUES ({:s}, '{:d}')", db.escapeString(config), value));
	} else {
		db.executeQuery(std::format("UPDATE `server_config` SET `value` = '{:d}' WHERE `config` = {:s}", value,
		                            db.escapeString(config)));
	}
}
