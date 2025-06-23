// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "databasemanager.h"

#include "configmanager.h"
#include "luascript.h"

bool DatabaseManager::optimizeTables()
{
	Database& db = Database::getInstance();

	DBResult_ptr result = db.storeQuery(std::format(
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
	lua_State* L = luaL_newstate();
	if (!L) {
		return;
	}

	luaL_openlibs(L);

#ifndef LUAJIT_VERSION
	// bit operations for Lua, based on bitlib project release 24
	// bit.bnot, bit.band, bit.bor, bit.bxor, bit.lshift, bit.rshift
	luaL_register(L, "bit", LuaScriptInterface::luaBitReg);
#endif

	// db table
	luaL_register(L, "db", LuaScriptInterface::luaDatabaseTable);

	// result table
	luaL_register(L, "result", LuaScriptInterface::luaResultTable);

	int32_t version = getDatabaseVersion();
	do {
		if (luaL_dofile(L, std::format("data/migrations/{:d}.lua", version).c_str()) != 0) {
			std::cout << "[Error - DatabaseManager::updateDatabase - Version: " << version << "] "
			          << lua_tostring(L, -1) << std::endl;
			break;
		}

		if (!tfs::lua::reserveScriptEnv()) {
			break;
		}

		lua_getglobal(L, "onUpdateDatabase");
		if (lua_pcall(L, 0, 1, 0) != 0) {
			tfs::lua::resetScriptEnv();
			std::cout << "[Error - DatabaseManager::updateDatabase - Version: " << version << "] "
			          << lua_tostring(L, -1) << std::endl;
			break;
		}

		if (!tfs::lua::getBoolean(L, -1, false)) {
			tfs::lua::resetScriptEnv();
			break;
		}

		version++;
		std::cout << "> Database has been updated to version " << version << '.' << std::endl;
		registerDatabaseConfig("db_version", version);

		tfs::lua::resetScriptEnv();
	} while (true);
	lua_close(L);
}

bool DatabaseManager::getDatabaseConfig(const std::string& config, int32_t& value)
{
	Database& db = Database::getInstance();

	DBResult_ptr result = db.storeQuery(
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
