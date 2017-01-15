/**
* The Forgotten Server - a free and open-source MMORPG server emulator
* Copyright (C) 2017  Mark Samman <mark.samman@gmail.com>
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

#ifndef DATABASEMYSQL_H
#define DATABASEMYSQL_H

#include "database.h"
#include <mysql.h>

class MySQLDBResult;
using DBResult_ptrMYSQL = std::shared_ptr<MySQLDBResult>;

class DatabaseMYsql : public Database
{
public:
	DatabaseMYsql();
	virtual ~DatabaseMYsql();
	bool executeQuery(const std::string& query);
	DBResult_ptr storeQuery(const std::string& query);
	std::string escapeString(const std::string& s) const;
	std::string escapeBlob(const char* s, uint32_t length) const;
	uint64_t getLastInsertId();
	std::string getClientVersion();

protected:
	MYSQL* handle = nullptr;	
};

class MySQLDBResult : public DBResult
{
public:
	int64_t getNumberAny(std::string const& s) const;
	std::string getString(const std::string& s) const;
	const char* getStream(const std::string& s, uint64_t& size) const;
	bool hasNext();
	bool next();

	MySQLDBResult(MYSQL_RES* results);
	~MySQLDBResult();
protected:
	
	MYSQL_RES* handle;
	MYSQL_ROW row;

	std::map<std::string, size_t> listNames;
	friend class DatabaseMYsql;
};
#endif // DATABASEMYSQL_H