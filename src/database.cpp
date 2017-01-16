/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2016  Mark Samman <mark.samman@gmail.com>
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
#include "databasemysql.h"
#include "databasepgsql.h"
#include "database.h"

Database* Database::instance;

Database* Database::getInstance() 		
{
	if (!instance) {
		if (g_config.getString(ConfigManager::SQL_TYPE) == "mysql"){
			instance = new DatabaseMYsql();
		}else if (g_config.getString(ConfigManager::SQL_TYPE) == "odbc"){
				//instance = new DatabaseODBC();
		}else if (g_config.getString(ConfigManager::SQL_TYPE) == "sqlite"){
				//instance = new DatabaseSQLite();
		}else if (g_config.getString(ConfigManager::SQL_TYPE) == "pgsql"){
			instance = new DatabasePGsql();
		}
	}
	return instance;
}

bool Database::executeQuery(const std::string& query)
{
	return false;
}

DBResult_ptr Database::storeQuery(const std::string& query)
{
	return nullptr;
}

std::string Database::escapeString(const std::string& s) const {
	return "''";
}

std::string Database::escapeBlob(const char* s, uint32_t length) const {
	return "''";
}

uint64_t Database::getLastInsertId(){
	return 0;
}

std::string Database::getClientVersion()
{
	return "''";
}

bool Database::beginTransaction()
{
	return executeQuery("BEGIN");
}

bool Database::rollback()
{
	return executeQuery("ROLLBACK");
}

bool Database::commit()
{
	return executeQuery("COMMIT");
}

/***************/

std::string DBResult::getString(const std::string & s) const
{
	return std::string();
}

const char * DBResult::getStream(const std::string & s, uint64_t & size) const
{
	return nullptr;
}

bool DBResult::hasNext()
{
	return false;
}

bool DBResult::next() 
{
	return false;
}

int64_t DBResult::getNumberAny(std::string const & s) const
{
	return int64_t();
}

/***************/

DBInsert::DBInsert(std::string query) : query(query)
{
	this->length = this->query.length();
}

bool DBInsert::addRow(const std::string& row)
{
	
	// adds new row to buffer
	const size_t rowLength = row.length();
	length += rowLength;	
	if (length > Database::getInstance()->getMaxPacketSize() && !execute()) {
		return false;
	}

	if (values.empty()) {
		values.reserve(rowLength + 2);
		values.push_back('(');
		values.append(row);
		values.push_back(')');
	}
	else {
		values.reserve(values.length() + rowLength + 3);
		values.push_back(',');
		values.push_back('(');
		values.append(row);
		values.push_back(')');
	}
	return true;
}

bool DBInsert::addRow(std::ostringstream& row)
{
	bool ret = addRow(row.str());
	row.str(std::string());
	return ret;
}

bool DBInsert::execute()
{
	if (values.empty()) {
		return true;
	}

	// executes buffer
	bool res = Database::getInstance()->executeQuery(query + values);
	values.clear();
	length = query.length();
	return res;
}