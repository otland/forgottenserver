// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_DATABASE_H
#define FS_DATABASE_H

#include "pugicast.h"

class DBResult;
using DBResult_ptr = std::shared_ptr<DBResult>;

namespace tfs::db {

/// @brief Connects to the database.
/// @return true on successful connection, false on error.
bool connect();

/// @brief Executes a command that does not return results.
///
/// This is used for queries such as INSERT, UPDATE, DELETE, etc.
/// @param {query} The SQL command to execute.
/// @return true on success, false on error.
bool execute_query(std::string_view query);

/// @brief Queries the database and retrieves results.
///
/// Executes a query that generates results, typically a SELECT statement.
/// @param {query} The SQL query to execute.
/// @return A DBResult_ptr object containing the results, or nullptr on error.
DBResult_ptr store_query(std::string_view query);

/// @brief Escapes a string for use in an SQL query.
///
/// Prepares a string to be safely included in SQL statements, adding necessary
/// escaping and quoting.
/// @param {s} The string to escape.
/// @return The escaped and quoted string.
std::string escape_string(std::string_view s);

/// @brief Escapes a binary stream for use in an SQL query.
///
/// Converts a binary stream into a format suitable for inclusion in SQL
/// statements.
/// @param {s} The binary stream to escape.
/// @param {length} The length of the binary stream.
/// @return The escaped and quoted string.
std::string escape_blob(const char* s, uint32_t length);

/// @brief Retrieves the ID of the last inserted row.
///
/// If the last query involved an auto-increment column, this retrieves the ID
/// of the row that was inserted.
/// @return The last inserted ID, or 0 if no rows were affected.
uint64_t last_insert_id();

/// @brief Gets the version of the database client engine.
///
/// @return A string containing the database engine version.
const char* client_version();

/// @brief Retrieves the maximum packet size allowed by the database.
///
/// @return The maximum packet size in bytes.
uint64_t max_packet_size();

} // namespace tfs::db

namespace tfs::db::transaction {

/// @brief Begins a database transaction.
/// @return true on success, false on error.
bool begin();

/// @brief Rolls back the current database transaction.
/// @return true on success, false on error.
bool rollback();

/// @brief Commits the current database transaction.
/// @return true on success, false on error.
bool commit();

} // namespace tfs::db::transaction

namespace tfs::db::detail {

struct MysqlDeleter
{
	void operator()(MYSQL* handle) const { mysql_close(handle); }
	void operator()(MYSQL_RES* handle) const { mysql_free_result(handle); }
};

using Mysql_ptr = std::unique_ptr<MYSQL, MysqlDeleter>;
using MysqlResult_ptr = std::unique_ptr<MYSQL_RES, MysqlDeleter>;

} // namespace tfs::db::detail

class DBResult
{
public:
	explicit DBResult(tfs::db::detail::MysqlResult_ptr&& res);

	// non-copyable
	DBResult(const DBResult&) = delete;
	DBResult& operator=(const DBResult&) = delete;

	template <typename T>
	T getNumber(std::string_view column) const
	{
		auto it = listNames.find(column);
		if (it == listNames.end()) {
			std::cout << "[Error - DBResult::getNumber] Column '" << column << "' doesn't exist in the result set"
			          << std::endl;
			return {};
		}

		if (!row[it->second]) {
			return {};
		}

		return pugi::cast<T>(row[it->second]);
	}

	std::string_view getString(std::string_view column) const;

	bool hasNext() const;
	bool next();

private:
	tfs::db::detail::MysqlResult_ptr handle;
	MYSQL_ROW row;

	std::map<std::string_view, size_t> listNames;
};

/**
 * INSERT statement.
 */
class DBInsert
{
public:
	explicit DBInsert(std::string query);
	bool addRow(const std::string& row);
	bool addRow(std::ostringstream& row);
	bool execute();

private:
	std::string query;
	std::string values;
	size_t length;
};

class DBTransaction
{
public:
	constexpr DBTransaction() = default;

	~DBTransaction()
	{
		if (state == STATE_START) {
			tfs::db::transaction::rollback();
		}
	}

	// non-copyable
	DBTransaction(const DBTransaction&) = delete;
	DBTransaction& operator=(const DBTransaction&) = delete;

	bool begin()
	{
		state = STATE_START;
		return tfs::db::transaction::begin();
	}

	bool commit()
	{
		if (state != STATE_START) {
			return false;
		}

		state = STATE_COMMIT;
		return tfs::db::transaction::commit();
	}

private:
	enum TransactionStates_t
	{
		STATE_NO_START,
		STATE_START,
		STATE_COMMIT,
	};

	TransactionStates_t state = STATE_NO_START;
};

#endif // FS_DATABASE_H
