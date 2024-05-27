// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "database.h"

#include "configmanager.h"

#include <mysql/errmsg.h>

static bool connectToDatabase(MYSQL*& handle, const bool retryIfError)
{
	bool isFirstAttemptToConnect = true;

retry:
	if (!isFirstAttemptToConnect) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	isFirstAttemptToConnect = false;

	// close the connection handle
	mysql_close(handle);
	// connection handle initialization
	handle = mysql_init(nullptr);
	if (!handle) {
		std::cout << std::endl << "Failed to initialize MySQL connection handle." << std::endl;
		goto error;
	}
	// connects to database
	if (!mysql_real_connect(handle, getString(ConfigManager::MYSQL_HOST).c_str(),
	                        getString(ConfigManager::MYSQL_USER).c_str(), getString(ConfigManager::MYSQL_PASS).c_str(),
	                        getString(ConfigManager::MYSQL_DB).c_str(), getNumber(ConfigManager::SQL_PORT),
	                        getString(ConfigManager::MYSQL_SOCK).c_str(), 0)) {
		std::cout << std::endl << "MySQL Error Message: " << mysql_error(handle) << std::endl;
		goto error;
	}
	return true;

error:
	if (retryIfError) {
		goto retry;
	}
	return false;
}

static bool isLostConnectionError(const unsigned error)
{
	return error == CR_SERVER_LOST || error == CR_SERVER_GONE_ERROR || error == CR_CONN_HOST_ERROR ||
	       error == 1053 /*ER_SERVER_SHUTDOWN*/ || error == CR_CONNECTION_ERROR;
}

static bool executeQuery(MYSQL*& handle, std::string_view query, const bool retryIfLostConnection)
{
	while (mysql_real_query(handle, query.data(), query.length()) != 0) {
		std::cout << "[Error - mysql_real_query] Query: " << query.substr(0, 256) << std::endl
		          << "Message: " << mysql_error(handle) << std::endl;
		const unsigned error = mysql_errno(handle);
		if (!isLostConnectionError(error) || !retryIfLostConnection) {
			return false;
		}
		connectToDatabase(handle, true);
	}
	return true;
}

Database::~Database() { mysql_close(handle); }

bool Database::connect()
{
	if (!connectToDatabase(handle, false)) {
		return false;
	}
	DBResult_ptr result = storeQuery("SHOW VARIABLES LIKE 'max_allowed_packet'");
	if (result) {
		maxPacketSize = result->getNumber<uint64_t>("Value");
	}
	return true;
}

bool Database::beginTransaction()
{
	databaseLock.lock();
	const bool result = executeQuery("START TRANSACTION");
	retryQueries = !result;
	if (!result) {
		databaseLock.unlock();
	}
	return result;
}

bool Database::rollback()
{
	const bool result = executeQuery("ROLLBACK");
	retryQueries = true;
	databaseLock.unlock();
	return result;
}

bool Database::commit()
{
	const bool result = executeQuery("COMMIT");
	retryQueries = true;
	databaseLock.unlock();
	return result;
}

bool Database::executeQuery(const std::string& query)
{
	std::lock_guard<std::recursive_mutex> lockGuard(databaseLock);
	return ::executeQuery(handle, query, retryQueries);
}

DBResult_ptr Database::storeQuery(std::string_view query)
{
	std::lock_guard<std::recursive_mutex> lockGuard(databaseLock);

retry:
	if (!::executeQuery(handle, query, retryQueries) && !retryQueries) {
		return nullptr;
	}

	// we should call that every time as someone would call executeQuery('SELECT...')
	// as it is described in MySQL manual: "it doesn't hurt" :P
	MYSQL_RES* res = mysql_store_result(handle);
	if (!res) {
		std::cout << "[Error - mysql_store_result] Query: " << query << std::endl
		          << "Message: " << mysql_error(handle) << std::endl;
		const unsigned error = mysql_errno(handle);
		if (!isLostConnectionError(error) || !retryQueries) {
			return nullptr;
		}
		goto retry;
	}

	// retrieving results of query
	DBResult_ptr result = std::make_shared<DBResult>(res);
	if (!result->hasNext()) {
		return nullptr;
	}
	return result;
}

std::string Database::escapeBlob(const char* s, uint32_t length) const
{
	// the worst case is 2n + 1
	size_t maxLength = (length * 2) + 1;

	std::string escaped;
	escaped.reserve(maxLength + 2);
	escaped.push_back('\'');

	if (length != 0) {
		char* output = new char[maxLength];
		mysql_real_escape_string(handle, output, s, length);
		escaped.append(output);
		delete[] output;
	}

	escaped.push_back('\'');
	return escaped;
}

DBResult::DBResult(MYSQL_RES* res)
{
	handle = res;

	size_t i = 0;

	MYSQL_FIELD* field = mysql_fetch_field(handle);
	while (field) {
		listNames[field->name] = i++;
		field = mysql_fetch_field(handle);
	}

	row = mysql_fetch_row(handle);
}

DBResult::~DBResult() { mysql_free_result(handle); }

std::string_view DBResult::getString(std::string_view column) const
{
	auto it = listNames.find(column);
	if (it == listNames.end()) {
		std::cout << "[Error - DBResult::getString] Column '" << column << "' does not exist in result set."
		          << std::endl;
		return {};
	}

	if (!row[it->second]) {
		return {};
	}

	auto size = mysql_fetch_lengths(handle)[it->second];
	return {row[it->second], size};
}

bool DBResult::hasNext() const { return row; }

bool DBResult::next()
{
	row = mysql_fetch_row(handle);
	return row;
}

DBInsert::DBInsert(std::string query) : query(std::move(query)) { this->length = this->query.length(); }

bool DBInsert::addRow(const std::string& row)
{
	// adds new row to buffer
	const size_t rowLength = row.length();
	length += rowLength;
	if (length > Database::getInstance().getMaxPacketSize() && !execute()) {
		return false;
	}

	if (values.empty()) {
		values.reserve(rowLength + 2);
		values.push_back('(');
		values.append(row);
		values.push_back(')');
	} else {
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
	bool res = Database::getInstance().executeQuery(query + values);
	values.clear();
	length = query.length();
	return res;
}
