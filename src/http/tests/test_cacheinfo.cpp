#define BOOST_TEST_MODULE http_cacheinfo

#include "../../otpch.h"

#include "../../configmanager.h"
#include "../../database.h"
#include "../cacheinfo.h"

#include <boost/test/unit_test.hpp>

using namespace std::chrono;

struct CacheInfoFixture
{
	CacheInfoFixture()
	{
		setString(ConfigManager::MYSQL_HOST, "0.0.0.0");
		setString(ConfigManager::MYSQL_USER, "forgottenserver");
		setString(ConfigManager::MYSQL_PASS, "forgottenserver");
		setString(ConfigManager::MYSQL_DB, "forgottenserver");
		setNumber(ConfigManager::SQL_PORT, 3306);

		db.connect();
		transaction.begin();
	}

	~CacheInfoFixture()
	{
		// `players_online` is a memory table and does not support transactions, so we need to clear it manually
		// do NOT run this test against a running server's database
		db.executeQuery("TRUNCATE `players_online`");
	}

	Database& db = Database::getInstance();
	DBTransaction transaction;

	std::string_view ip = "74.125.224.72";
	seconds now = duration_cast<seconds>(system_clock::now().time_since_epoch());
};

using status = boost::beast::http::status;

BOOST_FIXTURE_TEST_CASE(test_login_success_with_token, CacheInfoFixture)
{
	auto result = db.storeQuery(
	    "INSERT INTO `accounts` (`name`, `email`, `password`, `secret`) VALUES ('foo', 'foo@example.com', SHA1('bar'), UNHEX('')) RETURNING `id`");
	auto id = result->getNumber<uint64_t>("id");

	DBInsert insertSession("INSERT INTO `sessions` (`account_id`, `token`, `expire_at`) VALUES");

	DBInsert insertPlayers("INSERT INTO `players` (`account_id`, `name`) VALUES");
	insertPlayers.addRow(std::format("{:d}, \"{:s}\"", id, "Dejairzin"));
	insertPlayers.addRow(std::format("{:d}, \"{:s}\"", id, "Goraca"));
	insertPlayers.addRow(std::format("{:d}, \"{:s}\"", id, "Bobeek"));
	BOOST_TEST(insertPlayers.execute());

	BOOST_TEST(db.executeQuery(std::format(
	    "INSERT INTO `players_online` (`player_id`) SELECT `id` FROM `players` WHERE `account_id` = {:d}", id)));

	auto&& [status, body] = tfs::http::handle_cacheinfo({{"type", "cacheinfo"}}, ip);

	BOOST_TEST(status == status::ok);
	BOOST_TEST(body.at("playersonline").as_uint64() == 3);
}
