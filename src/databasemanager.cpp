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

#include "otpch.h"
#include "enums.h"
#include <iostream>
#include <string>

#include "databasemanager.h"
#include "tools.h"

#include "ban.h"

#include "configmanager.h"
extern ConfigManager g_config;

bool DatabaseManager::optimizeTables()
{
	Database* db = Database::getInstance();
	std::ostringstream query;

	query << "SELECT `TABLE_NAME` FROM `information_schema`.`TABLES` WHERE `TABLE_SCHEMA` = " << db->escapeString(g_config.getString(ConfigManager::MYSQL_DB)) << " AND `DATA_FREE` > 0";
	DBResult* result = db->storeQuery(query.str());
	if (!result) {
		return false;
	}

	do {
		std::string tableName = result->getDataString("TABLE_NAME");
		std::cout << "> Optimizing table " << tableName << "..." << std::flush;

		query.str("");
		query << "OPTIMIZE TABLE `" << tableName << "`";

		if (db->executeQuery(query.str())) {
			std::cout << " [success]" << std::endl;
		} else {
			std::cout << " [failed]" << std::endl;
		}
	} while (result->next());
	db->freeResult(result);
	return true;
}

bool DatabaseManager::triggerExists(const std::string& triggerName)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `TRIGGER_NAME` FROM `information_schema`.`TRIGGERS` WHERE `TRIGGER_SCHEMA` = " << db->escapeString(g_config.getString(ConfigManager::MYSQL_DB)) << " AND `TRIGGER_NAME` = " << db->escapeString(triggerName);

	DBResult* result = db->storeQuery(query.str());
	if (!result) {
		return false;
	}

	db->freeResult(result);
	return true;
}

bool DatabaseManager::tableExists(const std::string& tableName)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `TABLE_NAME` FROM `information_schema`.`tables` WHERE `TABLE_SCHEMA` = " << db->escapeString(g_config.getString(ConfigManager::MYSQL_DB)) << " AND `TABLE_NAME` = " << db->escapeString(tableName);

	DBResult* result = db->storeQuery(query.str());
	if (!result) {
		return false;
	}

	db->freeResult(result);
	return true;
}

bool DatabaseManager::isDatabaseSetup()
{
	Database* db = Database::getInstance();
	std::ostringstream query;
	query << "SELECT `TABLE_NAME` FROM `information_schema`.`tables` WHERE `TABLE_SCHEMA` = " << db->escapeString(g_config.getString(ConfigManager::MYSQL_DB));

	DBResult* result = db->storeQuery(query.str());
	if (!result) {
		return false;
	}

	db->freeResult(result);
	return true;
}

int32_t DatabaseManager::getDatabaseVersion()
{
	if (!tableExists("server_config")) {
		Database* db = Database::getInstance();
		db->executeQuery("CREATE TABLE `server_config` (`config` VARCHAR(50) NOT NULL, `value` VARCHAR(256) NOT NULL DEFAULT '', UNIQUE(`config`)) ENGINE = InnoDB");
		db->executeQuery("INSERT INTO `server_config` VALUES ('db_version', 0)");
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

#ifndef __LUAJIT__
	//bit operations for Lua, based on bitlib project release 24
	//bit.bnot, bit.band, bit.bor, bit.bxor, bit.lshift, bit.rshift
	luaL_register(L, "bit", LuaScriptInterface::luaBitReg);
#endif

	//db table
	luaL_register(L, "db", LuaScriptInterface::luaDatabaseTable);

	//result table
	luaL_register(L, "result", LuaScriptInterface::luaResultTable);

	int32_t version = getDatabaseVersion();
	do {
		std::ostringstream ss;
		ss << "data/migrations/" << version << ".lua";
		if (luaL_dofile(L, ss.str().c_str()) != 0) {
			std::cout << "[Error - DatabaseManager::updateDatabase - Version: " << version << "] " << lua_tostring(L, -1) << std::endl;
			break;
		}

		lua_getglobal(L, "onUpdateDatabase");
		if (lua_pcall(L, 0, 1, 0) != 0) {
			std::cout << "[Error - DatabaseManager::updateDatabase - Version: " << version << "] " << lua_tostring(L, -1) << std::endl;
			break;
		}

		if (!LuaScriptInterface::popBoolean(L)) {
			break;
		}

		version++;
		std::cout << "> Database has been updated to version " << version << "." << std::endl;
		registerDatabaseConfig("db_version", version);
	} while (true);
	lua_close(L);
}

bool DatabaseManager::getDatabaseConfig(const std::string& config, int32_t& value)
{
	Database* db = Database::getInstance();
	std::ostringstream query;
	query << "SELECT `value` FROM `server_config` WHERE `config` = " << db->escapeString(config);

	DBResult* result = db->storeQuery(query.str());
	if (!result) {
		return false;
	}

	value = atoi(result->getDataString("value").c_str());
	db->freeResult(result);
	return true;
}

bool DatabaseManager::getDatabaseConfig(const std::string& config, std::string& value)
{
	Database* db = Database::getInstance();
	std::ostringstream query;
	query << "SELECT `value` FROM `server_config` WHERE `config` = " << db->escapeString(config);
	DBResult* result = db->storeQuery(query.str());

	if (!result) {
		return false;
	}

	value = result->getDataString("value");
	db->freeResult(result);
	return true;
}

void DatabaseManager::registerDatabaseConfig(const std::string& config, int32_t value)
{
	Database* db = Database::getInstance();
	std::ostringstream query;

	int32_t tmp;

	if (!getDatabaseConfig(config, tmp)) {
		query << "INSERT INTO `server_config` VALUES (" << db->escapeString(config) << ", '" << value << "')";
	} else {
		query << "UPDATE `server_config` SET `value` = '" << value << "' WHERE `config` = " << db->escapeString(config);
	}

	db->executeQuery(query.str());
}

void DatabaseManager::registerDatabaseConfig(const std::string& config, const std::string& value)
{
	Database* db = Database::getInstance();
	std::ostringstream query;

	std::string tmp;

	if (!getDatabaseConfig(config, tmp)) {
		query << "INSERT INTO `server_config` VALUES (" << db->escapeString(config) << ", " << db->escapeString(value) << ")";
	} else {
		query << "UPDATE `server_config` SET `value` = " << db->escapeString(value) << " WHERE `config` = " << db->escapeString(config);
	}

	db->executeQuery(query.str());
}

void DatabaseManager::checkEncryption()
{
	int32_t currentValue = g_config.getNumber(ConfigManager::PASSWORD_TYPE);
	int32_t oldValue = 0;

	if (getDatabaseConfig("encryption", oldValue)) {
		if (currentValue == oldValue) {
			return;
		}

		if (oldValue != PASSWORD_TYPE_PLAIN) {
			std::string oldName;

			if (oldValue == PASSWORD_TYPE_MD5) {
				oldName = "md5";
			} else if (oldValue == PASSWORD_TYPE_SHA1) {
				oldName = "sha1";
			} else {
				oldName = "plain";
			}

			g_config.setNumber(ConfigManager::PASSWORD_TYPE, oldValue);
			std::cout << "> WARNING: Unsupported password hashing switch! Change back passwordType in config.lua to \"" << oldName << "\"!" << std::endl;
			return;
		}

		switch (currentValue) {
			case PASSWORD_TYPE_MD5: {
				Database::getInstance()->executeQuery("UPDATE `accounts` SET `password` = MD5(`password`)");
				std::cout << "> Password type has been updated to MD5." << std::endl;
				break;
			}

			case PASSWORD_TYPE_SHA1: {
				Database::getInstance()->executeQuery("UPDATE `accounts` SET `password` = SHA1(`password`)");
				std::cout << "> Password type has been updated to SHA1." << std::endl;
				break;
			}

			default:
				break;
		}
	}

	registerDatabaseConfig("encryption", currentValue);
}

void DatabaseManager::checkTriggers()
{
	//
}
