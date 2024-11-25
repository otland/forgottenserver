// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "database.h"

#include "configmanager.h"

#include <mysql/errmsg.h>

namespace {

std::recursive_mutex database_lock;
tfs::db::detail::Mysql_ptr handle = nullptr;
uint64_t packet_size = 1048576;

// Do not retry queries if we are in the middle of a transaction
bool retry_queries = true;

bool is_lost_connection_error(const unsigned error)
{
	return error == CR_SERVER_LOST || error == CR_SERVER_GONE_ERROR || error == CR_CONN_HOST_ERROR ||
	       error == 1053 /*ER_SERVER_SHUTDOWN*/ || error == CR_CONNECTION_ERROR;
}

tfs::db::detail::Mysql_ptr connect_to_database(const bool retryIfError)
{
	auto is_first_attempt_to_connect = true;

retry:
	if (!is_first_attempt_to_connect) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	is_first_attempt_to_connect = false;

	tfs::db::detail::Mysql_ptr handle{mysql_init(nullptr)};
	if (!handle) {
		std::cout << std::endl << "Failed to initialize MySQL connection handle." << std::endl;
		goto error;
	}

	// connects to database
	if (!mysql_real_connect(handle.get(), getString(ConfigManager::MYSQL_HOST).c_str(),
	                        getString(ConfigManager::MYSQL_USER).c_str(), getString(ConfigManager::MYSQL_PASS).c_str(),
	                        getString(ConfigManager::MYSQL_DB).c_str(), getNumber(ConfigManager::SQL_PORT),
	                        getString(ConfigManager::MYSQL_SOCK).c_str(), 0)) {
		std::cout << std::endl << "MySQL Error Message: " << mysql_error(handle.get()) << std::endl;
		goto error;
	}
	return handle;

error:
	if (retryIfError) {
		goto retry;
	}
	return nullptr;
}

bool execute_query_handle(tfs::db::detail::Mysql_ptr& handle, std::string_view query, const bool retryIfLostConnection)
{
	while (mysql_real_query(handle.get(), query.data(), query.length()) != 0) {
		std::cout << "[Error - mysql_real_query] Query: " << query.substr(0, 256) << std::endl
		          << "Message: " << mysql_error(handle.get()) << std::endl;
		const unsigned error = mysql_errno(handle.get());
		if (!is_lost_connection_error(error) || !retryIfLostConnection) {
			return false;
		}
		handle = connect_to_database(true);
	}
	return true;
}

} // namespace

bool tfs::db::connect()
{
	auto new_handle = connect_to_database(false);
	if (!new_handle) {
		return false;
	}

	handle = std::move(new_handle);
	if (auto result = store_query("SHOW VARIABLES LIKE 'max_allowed_packet'")) {
		packet_size = result->getNumber<uint64_t>("Value");
	}
	return true;
}

bool tfs::db::execute_query(std::string_view query)
{
	std::lock_guard<std::recursive_mutex> lockGuard(database_lock);
	auto success = ::execute_query_handle(handle, query, retry_queries);

	// execute_query_handle can be called with command that produces result (e.g. SELECT)
	// we have to store that result, even though we do not need it, otherwise handle will get blocked
	auto store_result = mysql_store_result(handle.get());
	mysql_free_result(store_result);

	return success;
}

DBResult_ptr tfs::db::store_query(std::string_view query)
{
	std::lock_guard<std::recursive_mutex> lockGuard(database_lock);

retry:
	if (!::execute_query_handle(handle, query, retry_queries) && !retry_queries) {
		return nullptr;
	}

	// we should call that every time as someone would call execute_query_handle('SELECT...')
	// as it is described in MySQL manual: "it doesn't hurt" :P
	tfs::db::detail::MysqlResult_ptr res{mysql_store_result(handle.get())};
	if (!res) {
		std::cout << "[Error - mysql_store_result] Query: " << query << std::endl
		          << "Message: " << mysql_error(handle.get()) << std::endl;
		const unsigned error = mysql_errno(handle.get());
		if (!is_lost_connection_error(error) || !retry_queries) {
			return nullptr;
		}
		goto retry;
	}

	// retrieving results of query
	auto result = std::make_shared<DBResult>(std::move(res));
	if (!result->hasNext()) {
		return nullptr;
	}
	return result;
}

std::string tfs::db::escape_string(std::string_view s) { return escape_blob(s.data(), s.length()); }

std::string tfs::db::escape_blob(const char* s, uint32_t length)
{ // the worst case is 2n + 1
	size_t max_length = (length * 2) + 1;

	std::string escaped;
	escaped.reserve(max_length + 2);
	escaped.push_back('\'');

	if (length != 0) {
		char* output = new char[max_length];
		mysql_real_escape_string(handle.get(), output, s, length);
		escaped.append(output);
		delete[] output;
	}

	escaped.push_back('\'');
	return escaped;
}

uint64_t tfs::db::last_insert_id() { return static_cast<uint64_t>(mysql_insert_id(handle.get())); }

const char* tfs::db::client_version() { return mysql_get_client_info(); }

uint64_t tfs::db::max_packet_size() { return packet_size; }

bool tfs::db::transaction::begin()
{
	database_lock.lock();
	const auto result = tfs::db::execute_query("START TRANSACTION");
	retry_queries = !result;
	if (!result) {
		database_lock.unlock();
	}
	return result;
}

bool tfs::db::transaction::rollback()
{
	const auto result = tfs::db::execute_query("ROLLBACK");
	retry_queries = true;
	database_lock.unlock();
	return result;
}

bool tfs::db::transaction::commit()
{
	const auto result = tfs::db::execute_query("COMMIT");
	retry_queries = true;
	database_lock.unlock();
	return result;
}

DBResult::DBResult(tfs::db::detail::MysqlResult_ptr&& res) : handle{std::move(res)}
{
	size_t i = 0;

	MYSQL_FIELD* field = mysql_fetch_field(handle.get());
	while (field) {
		listNames[field->name] = i++;
		field = mysql_fetch_field(handle.get());
	}

	row = mysql_fetch_row(handle.get());
}

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

	auto size = mysql_fetch_lengths(handle.get())[it->second];
	return {row[it->second], size};
}

bool DBResult::hasNext() const { return row; }

bool DBResult::next()
{
	row = mysql_fetch_row(handle.get());
	return row;
}

DBInsert::DBInsert(std::string query) : query(std::move(query)) { this->length = this->query.length(); }

bool DBInsert::addRow(const std::string& row)
{
	// adds new row to buffer
	const size_t row_length = row.length();
	length += row_length;

	if (length > tfs::db::max_packet_size() && !execute()) {
		return false;
	}

	if (values.empty()) {
		values.reserve(row_length + 2);
		values.push_back('(');
		values.append(row);
		values.push_back(')');
	} else {
		values.reserve(values.length() + row_length + 3);
		values.push_back(',');
		values.push_back('(');
		values.append(row);
		values.push_back(')');
	}
	return true;
}

bool DBInsert::addRow(std::ostringstream& row)
{
	auto result = addRow(row.str());
	row.str(std::string());
	return result;
}

bool DBInsert::execute()
{
	if (values.empty()) {
		return true;
	}

	// executes buffer
	auto result = tfs::db::execute_query(query + values);
	values.clear();
	length = query.length();
	return result;
}
