// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_DATABASE_H
#define FS_DATABASE_H

#include "pugicast.h"

class DBResult;
using DBResult_ptr = std::shared_ptr<DBResult>;

class Database
{
public:
	Database() = default;
	~Database();

	// non-copyable
	Database(const Database&) = delete;
	Database& operator=(const Database&) = delete;

	/**
	 * Singleton implementation.
	 *
	 * @return database connection handler singleton
	 */
	static Database& getInstance()
	{
		static Database instance;
		return instance;
	}

	/**
	 * Connects to the database
	 *
	 * @return true on successful connection, false on error
	 */
	bool connect();

	/**
	 * Executes command.
	 *
	 * Executes query which doesn't generates results (eg. INSERT, UPDATE,
	 * DELETE...).
	 *
	 * @param query command
	 * @return true on success, false on error
	 */
	bool executeQuery(const std::string& query);

	/**
	 * Queries database.
	 *
	 * Executes query which generates results (mostly SELECT).
	 *
	 * @return results object (nullptr on error)
	 */
	DBResult_ptr storeQuery(std::string_view query);

	/**
	 * Escapes string for query.
	 *
	 * Prepares string to fit SQL queries including quoting it.
	 *
	 * @param s string to be escaped
	 * @return quoted string
	 */
	std::string escapeString(std::string_view s) const { return escapeBlob(s.data(), s.length()); }

	/**
	 * Escapes binary stream for query.
	 *
	 * Prepares binary stream to fit SQL queries.
	 *
	 * @param s binary stream
	 * @param length stream length
	 * @return quoted string
	 */
	std::string escapeBlob(const char* s, uint32_t length) const;

	/**
	 * Retrieve id of last inserted row
	 *
	 * @return id on success, 0 if last query did not result on any rows with
	 * auto_increment keys
	 */
	uint64_t getLastInsertId() const { return static_cast<uint64_t>(mysql_insert_id(handle)); }

	/**
	 * Get database engine version
	 *
	 * @return the database engine version
	 */
	static const char* getClientVersion() { return mysql_get_client_info(); }

	uint64_t getMaxPacketSize() const { return maxPacketSize; }

private:
	/**
	 * Transaction related methods.
	 *
	 * Methods for starting, committing and rolling back transaction. Each of
	 * the returns boolean value.
	 *
	 * @return true on success, false on error
	 */
	bool beginTransaction();
	bool rollback();
	bool commit();

	MYSQL* handle = nullptr;
	std::recursive_mutex databaseLock;
	uint64_t maxPacketSize = 1048576;
	// Do not retry queries if we are in the middle of a transaction
	bool retryQueries = true;

	friend class DBTransaction;
};

class DBResult
{
public:
	explicit DBResult(MYSQL_RES* res);
	~DBResult();

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
	MYSQL_RES* handle;
	MYSQL_ROW row;

	std::map<std::string_view, size_t> listNames;

	friend class Database;
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
			Database::getInstance().rollback();
		}
	}

	// non-copyable
	DBTransaction(const DBTransaction&) = delete;
	DBTransaction& operator=(const DBTransaction&) = delete;

	bool begin()
	{
		state = STATE_START;
		return Database::getInstance().beginTransaction();
	}

	bool commit()
	{
		if (state != STATE_START) {
			return false;
		}

		state = STATE_COMMIT;
		return Database::getInstance().commit();
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
