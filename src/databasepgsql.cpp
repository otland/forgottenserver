#include "otpch.h"
#include "databasepgsql.h"
#include <boost/algorithm/string/replace.hpp>
#include <boost/bind.hpp>

DatabasePGsql::DatabasePGsql()
{
	m_handle = NULL;

	std::stringstream dns;
	dns << "host=" << g_config.getString(ConfigManager::SQL_HOST) << " dbname=" << g_config.getString(ConfigManager::SQL_DB) << " user=" << g_config.getString(ConfigManager::SQL_USER) << " password=" << g_config.getString(ConfigManager::SQL_PASS) << " port=" << g_config.getNumber(ConfigManager::SQL_PORT) << "";
	m_handle = PQconnectdb(dns.str().c_str());

	if (PQstatus(m_handle) != CONNECTION_OK)
	{
		std::clog << "Failed to estabilish PostgreSQL database connection: " << PQerrorMessage(m_handle) << std::endl;
		PQerrorMessage(m_handle);
	}
	m_connected = true;
}

DatabasePGsql::~DatabasePGsql()
{
	if (m_handle != NULL) {
		PQfinish(m_handle);
	}
}

bool DatabasePGsql::executeQuery(const std::string& query)
{
	if (!m_connected)
		return false;
	databaseLock.lock();

	std::string _query = (std::string)query;
	boost::replace_all(_query, "`", "");
	bool success = true;
	PGresult* res;
	// executes query
	
		res = PQexec(m_handle, _query.c_str());
		ExecStatusType stat = PQresultStatus(res);
			if (stat != PGRES_COMMAND_OK) {
				std::cout << "PQexec() internalQuery-PQresStatus: " << _query << " : " << PQresStatus(stat) << std::endl;
				std::cout << "PQexec() internalQuery-PQresultStatus: " << PQresultStatus(res) << std::endl;
				std::cout << "PQexec() internalQuery-PQresultErrorMessage: " << PQresultErrorMessage(res) << std::endl;
			}
	databaseLock.unlock();

	// everything went fine
	PQclear(res);
	return true;
}

DBResult_ptr DatabasePGsql::storeQuery(const std::string& query)
{
	databaseLock.lock();

	std::string _query = (std::string)query;
	boost::replace_all(_query, "`", "");

	// executes query	
	PGresult* res = PQexec(m_handle, _query.c_str());
	ExecStatusType stat = PQresultStatus(res);
		if (stat != PGRES_COMMAND_OK && stat != PGRES_TUPLES_OK) {
			std::cout << "PQexec() internalSelectQuery-PQresStatus: " << _query << " : " << PQresStatus(stat) << std::endl;
			std::cout << "PQexec() internalSelectQuery-PQresultStatus: " << PQresultStatus(res) << std::endl;
			std::cout << "PQexec() internalSelectQuery-PQresultErrorMessage: " << PQresultErrorMessage(res) << std::endl;

			PQclear(res);
			return false;	
		}

	databaseLock.unlock();
	// retrieving results of query
	DBResult_ptrPGSQL result = std::make_shared<PgSQLDBResult>(res);
	if (!result->hasNext()) {
		return nullptr;
	}
	return result;
}


std::string DatabasePGsql::getClientVersion() {
	return "PgSQL - " + std::to_string(PQlibVersion());
}

uint64_t DatabasePGsql::getLastInsertId()
{
	if (!m_connected)
		return 0;

	PGresult* res = PQexec(m_handle, "SELECT LASTVAL() as last;");
	ExecStatusType stat = PQresultStatus(res);

	if (stat != PGRES_COMMAND_OK && stat != PGRES_TUPLES_OK) {
		std::cout << "PQexec(): failed to fetch last row: " << PQresultErrorMessage(res) << std::endl;
		PQclear(res);
		return 0;
	}

	// everything went fine
	uint64_t id = reinterpret_cast<uint64_t>(PQgetvalue(res, 0, PQfnumber(res, "last")));
	PQclear(res);
	return id;
}

std::string DatabasePGsql::escapeString(const std::string &s) const
{
	// remember to quote even empty string!
	if (!s.size())
		return std::string("''");

	//the worst case is 2n + 1
	int32_t error;
	size_t maxLength = s.length() * 2 + 1;

	std::string escaped;
	escaped.reserve(maxLength + 2);
	escaped.push_back('\'');
	char* output = new char[s.length() * 2 + 1];
	PQescapeStringConn(m_handle, output, s.c_str(), s.length(), reinterpret_cast<int*>(&error));

	escaped.append(output);
	delete[] output;

	escaped.push_back('\'');
	return escaped;
}

std::string DatabasePGsql::escapeBlob(const char* s, uint32_t length) const
{
	// remember to quote even empty stream!
	if (!s)
		return std::string("''");

	// quotes escaped string and frees temporary buffer
	size_t maxLength = (length * 2) + 1;

	std::string escaped;
	escaped.reserve(maxLength + 2);
	escaped.push_back('\'');
	char* output = (char*)PQescapeByteaConn(m_handle, (unsigned char*)s, length, &maxLength);
	escaped.append(output);
	PQfreemem(output);
	escaped.push_back('\'');

	return escaped;
}


std::string PgSQLDBResult::getString(const std::string &s) const
{
	return std::string(PQgetvalue(m_handle, m_cursor, PQfnumber(m_handle, s.c_str())));
}

const char* PgSQLDBResult::getStream(const std::string &s, uint64_t &size) const
{
	std::string buf = PQgetvalue(m_handle, m_cursor, PQfnumber(m_handle, s.c_str()));
	unsigned char* temp = PQunescapeBytea((const unsigned char*)buf.c_str(), &size);
	const char* value = reinterpret_cast<const char*>(temp);
	PQfreemem(temp);
	return value;
}

bool PgSQLDBResult::hasNext()
{
	if (m_cursor >= m_rows) {
		return false;
	}
	else {
		m_cursor++;
		return true;
	}
}

bool PgSQLDBResult::next()
{
	if (m_cursor >= m_rows) {
		return false;
	}
	else {
		m_cursor++;
		return true;
	}
}


PgSQLDBResult::PgSQLDBResult(PGresult* results)
{
	m_handle = results;
	m_cursor = -1;
	m_rows = PQntuples(m_handle) - 1;
}

PgSQLDBResult::~PgSQLDBResult()
{
	PQclear(m_handle);
}

int64_t PgSQLDBResult::getNumberAny(std::string const & s) const
{	
		try {
			int64_t data = boost::lexical_cast<int64_t>(PQgetvalue(m_handle, m_cursor, PQfnumber(m_handle, s.c_str())));
			return data;
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
			return 0;
		}
}
