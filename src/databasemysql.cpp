#include "otpch.h"
#include "databasemysql.h"
#include <boost/bind.hpp>

DatabaseMYsql::DatabaseMYsql()
{
	// connection handle initialization
	handle = mysql_init(nullptr);
	if (!handle) {
		std::cout << std::endl << "Failed to initialize MySQL connection handle." << std::endl;
		return;
	}

	// automatic reconnect
	my_bool reconnect = true;
	mysql_options(handle, MYSQL_OPT_RECONNECT, &reconnect);

	// connects to database
	if (!mysql_real_connect(handle, g_config.getString(ConfigManager::SQL_HOST).c_str(), g_config.getString(ConfigManager::SQL_USER).c_str(), g_config.getString(ConfigManager::SQL_PASS).c_str(), g_config.getString(ConfigManager::SQL_DB).c_str(), g_config.getNumber(ConfigManager::SQL_PORT), g_config.getString(ConfigManager::SQL_SOCK).c_str(), 0)) {
		std::cout << std::endl << "MySQL Error Message: " << mysql_error(handle) << std::endl;
		return;
	}
	m_connected = true;
}


DatabaseMYsql::~DatabaseMYsql()
{
	if (handle != nullptr) {
		mysql_close(handle);
	}
}

bool DatabaseMYsql::executeQuery(const std::string & query)
{
	bool success = true;

	// executes the query
	databaseLock.lock();

	while (mysql_real_query(handle, query.c_str(), query.length()) != 0) {
		std::cout << "[Error - mysql_real_query] Query: " << query.substr(0, 256) << std::endl << "Message: " << mysql_error(handle) << std::endl;
		auto error = mysql_errno(handle);
		if (error != CR_SERVER_LOST && error != CR_SERVER_GONE_ERROR && error != CR_CONN_HOST_ERROR && error != 1053/*ER_SERVER_SHUTDOWN*/ && error != CR_CONNECTION_ERROR) {
			success = false;
			break;
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	MYSQL_RES* m_res = mysql_store_result(handle);
	databaseLock.unlock();

	if (m_res) {
		mysql_free_result(m_res);
	}

	return success;
}

DBResult_ptr DatabaseMYsql::storeQuery(const std::string & query)
{
	databaseLock.lock();

retry:
	while (mysql_real_query(handle, query.c_str(), query.length()) != 0) {
		std::cout << "[Error - mysql_real_query] Query: " << query << std::endl << "Message: " << mysql_error(handle) << std::endl;
		auto error = mysql_errno(handle);
		if (error != CR_SERVER_LOST && error != CR_SERVER_GONE_ERROR && error != CR_CONN_HOST_ERROR && error != 1053/*ER_SERVER_SHUTDOWN*/ && error != CR_CONNECTION_ERROR) {
			break;
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	// we should call that every time as someone would call executeQuery('SELECT...')
	// as it is described in MySQL manual: "it doesn't hurt" :P
	MYSQL_RES* res = mysql_store_result(handle);
	if (res == nullptr) {
		std::cout << "[Error - mysql_store_result] Query: " << query << std::endl << "Message: " << mysql_error(handle) << std::endl;
		auto error = mysql_errno(handle);
		if (error != CR_SERVER_LOST && error != CR_SERVER_GONE_ERROR && error != CR_CONN_HOST_ERROR && error != 1053/*ER_SERVER_SHUTDOWN*/ && error != CR_CONNECTION_ERROR) {
			databaseLock.unlock();
			return nullptr;
		}
		goto retry;
	}
	databaseLock.unlock();

	// retrieving results of query
	DBResult_ptrMYSQL result = std::make_shared<MySQLDBResult>(res);
	if (!result->hasNext()) {
		return nullptr;
	}
	return result;
}

std::string DatabaseMYsql::escapeString(const std::string & s) const
{
	return escapeBlob(s.c_str(), s.length());
}

std::string DatabaseMYsql::escapeBlob(const char * s, uint32_t length) const
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

uint64_t DatabaseMYsql::getLastInsertId()
{
	return static_cast<uint64_t>(mysql_insert_id(handle));
}

std::string DatabaseMYsql::getClientVersion()
{
	return "MySQL - " + (std::string)mysql_get_client_info();
}

int64_t MySQLDBResult::getNumberAny(std::string const & s) const
{
	auto it = listNames.find(s);
	if (it == listNames.end()) {
		std::cout << "[Error - DBResult::getNumber] Column '" << s << "' doesn't exist in the result set" << std::endl;
		return 0;
	}

	if (row[it->second] == nullptr) {
		return 0;
	}

	int64_t data;
	try {
		data = boost::lexical_cast<int64_t>(row[it->second]);
		if (data < 0) {
			
		}
	}
	catch (boost::bad_lexical_cast&) {
		data = 0;
	}
	return data;
	//return boost::any();
}

std::string MySQLDBResult::getString(const std::string & s) const
{
	auto it = listNames.find(s);
	if (it == listNames.end()) {
		std::cout << "[Error - DBResult::getString] Column '" << s << "' does not exist in result set." << std::endl;
		return std::string();
	}

	if (row[it->second] == nullptr) {
		return std::string();
	}

	return std::string(row[it->second]);
}

const char * MySQLDBResult::getStream(const std::string & s, uint64_t & size) const
{
	auto it = listNames.find(s);
	if (it == listNames.end()) {
		std::cout << "[Error - DBResult::getStream] Column '" << s << "' doesn't exist in the result set" << std::endl;
		size = 0;
		return nullptr;
	}

	if (row[it->second] == nullptr) {
		size = 0;
		return nullptr;
	}

	size = mysql_fetch_lengths(handle)[it->second];
	return row[it->second];
}

bool MySQLDBResult::hasNext()
{
	return row != nullptr;
}

bool MySQLDBResult::next()
{
	row = mysql_fetch_row(handle);
	return row != nullptr;
}

MySQLDBResult::MySQLDBResult(MYSQL_RES * results)
{
	handle = results;

	size_t i = 0;

	MYSQL_FIELD* field = mysql_fetch_field(handle);
	while (field) {
		listNames[field->name] = i++;
		field = mysql_fetch_field(handle);
	}

	row = mysql_fetch_row(handle);
}

MySQLDBResult::~MySQLDBResult()
{
	mysql_free_result(handle);
}
