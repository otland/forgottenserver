/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2014  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_DATABASE_H_A484B0CDFDE542838F506DCE3D40C693
#define FS_DATABASE_H_A484B0CDFDE542838F506DCE3D40C693

#include <boost/lexical_cast.hpp>

#include <mysql.h>

class DBResult;
typedef std::shared_ptr<DBResult> DBResult_ptr;

class Database
{
	public:
		/**
		 * Singleton implementation.
		 *
		 * @return database connection handler singleton
		 */
		static Database* getInstance()
		{
			static Database instance;
			return &instance;
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
		 * Executes query which doesn't generates results (eg. INSERT, UPDATE, DELETE...).
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
		DBResult_ptr storeQuery(const std::string& query);

		/**
		 * Escapes string for query.
		 *
		 * Prepares string to fit SQL queries including quoting it.
		 *
		 * @param s string to be escaped
		 * @return quoted string
		 */
		std::string escapeString(const std::string& s) const;

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
		 * @return id on success, 0 if last query did not result on any rows with auto_increment keys
		 */
		uint64_t getLastInsertId() const {
			return static_cast<uint64_t>(mysql_insert_id(m_handle));
		}

		/**
		 * Get database engine version
		 *
		 * @return the database engine version
		 */
		static const char* getClientVersion() {
			return mysql_get_client_info();
		}

		uint64_t getMaxPacketSize() const {
			return maxPacketSize;
		}

	protected:
		/**
		 * Transaction related methods.
		 *
		 * Methods for starting, commiting and rolling back transaction. Each of the returns boolean value.
		 *
		 * @return true on success, false on error
		 */
		bool beginTransaction();
		bool rollback();
		bool commit();

	private:
		Database();
		~Database();

		MYSQL* m_handle;
		std::recursive_mutex database_lock;
		uint64_t maxPacketSize;

	friend class DBTransaction;
};

class DBResult
{
	public:
		~DBResult();

		template<typename T>
		T getNumber(const std::string& s) const
		{
			auto it = m_listNames.find(s);
			if (it == m_listNames.end()) {
				std::cout << "[Error - DBResult::getData] Column '" << s << "' does not exist in result set." << std::endl;
				return static_cast<T>(0);
			}

			if (m_row[it->second] == nullptr) {
				return static_cast<T>(0);
			}

			T data;
			try {
				data = boost::lexical_cast<T>(m_row[it->second]);
			} catch (boost::bad_lexical_cast&) {
				data = 0;
			}
			return data;
		}

		int32_t getDataInt(const std::string& s) const;
		std::string getDataString(const std::string& s) const;
		const char* getDataStream(const std::string& s, unsigned long& size) const;

		bool hasNext() const;
		bool next();

	protected:
		DBResult(MYSQL_RES* res);

	private:
		MYSQL_RES* m_handle;
		MYSQL_ROW m_row;

		std::map<std::string, uint32_t> m_listNames;

	friend class Database;
};

/**
 * INSERT statement.
 */
class DBInsert
{
	public:
		DBInsert(const std::string& query);
		bool addRow(const std::string& row);
		bool addRow(std::ostringstream& row);
		bool execute();

	protected:
		std::string query;
		std::string values;
		size_t length;
};

class DBTransaction
{
	public:
		DBTransaction() {
			m_state = STATE_NO_START;
		}

		~DBTransaction() {
			if (m_state == STATE_START) {
				Database::getInstance()->rollback();
			}
		}

		bool begin() {
			m_state = STATE_START;
			return Database::getInstance()->beginTransaction();
		}

		bool commit() {
			if (m_state != STATE_START) {
				return false;
			}

			m_state = STEATE_COMMIT;
			return Database::getInstance()->commit();
		}

	private:
		enum TransactionStates_t {
			STATE_NO_START,
			STATE_START,
			STEATE_COMMIT
		};

		TransactionStates_t m_state;
};

#endif
