#define BOOST_TEST_MODULE http_login

#include "../../otpch.h"

#include "../../base64.h"
#include "../../database.h"
#include "../../tools.h"
#include "../../vocation.h"
#include "../login.h"

#include <boost/test/unit_test.hpp>

extern Vocations g_vocations;

auto vocationsXml = []() {
	return std::istringstream{R"(<?xml version="1.0" encoding="UTF-8"?>
<vocations>
	<vocation id="0" clientid="0" name="None" description="none" magicshield="0" gaincap="10" gainhp="5" gainmana="5" gainhpticks="6" gainhpamount="1" gainmanaticks="6" gainmanaamount="1" manamultiplier="4.0" attackspeed="2000" basespeed="220" soulmax="100" gainsoulticks="120" allowPvp="0" fromvoc="0">
		<formula meleeDamage="1.0" distDamage="1.0" defense="1.0" armor="1.0" />
		<skill id="0" multiplier="1.5" />
		<skill id="1" multiplier="2.0" />
		<skill id="2" multiplier="2.0" />
		<skill id="3" multiplier="2.0" />
		<skill id="4" multiplier="2.0" />
		<skill id="5" multiplier="1.5" />
		<skill id="6" multiplier="1.1" />
	</vocation>
	<vocation id="1" clientid="3" name="Sorcerer" description="a sorcerer" magicshield="1" gaincap="10" gainhp="5" gainmana="30" gainhpticks="6" gainhpamount="5" gainmanaticks="3" gainmanaamount="5" manamultiplier="1.1" attackspeed="2000" basespeed="220" soulmax="100" gainsoulticks="120" fromvoc="1" noPongKickTime="40">
		<formula meleeDamage="1.0" distDamage="1.0" defense="1.0" armor="1.0" />
		<skill id="0" multiplier="1.5" />
		<skill id="1" multiplier="2.0" />
		<skill id="2" multiplier="2.0" />
		<skill id="3" multiplier="2.0" />
		<skill id="4" multiplier="2.0" />
		<skill id="5" multiplier="1.5" />
		<skill id="6" multiplier="1.1" />
	</vocation>
	<vocation id="2" clientid="4" name="Druid" description="a druid" magicshield="1" gaincap="10" gainhp="5" gainmana="30" gainhpticks="6" gainhpamount="5" gainmanaticks="3" gainmanaamount="5" manamultiplier="1.1" attackspeed="2000" basespeed="220" soulmax="100" gainsoulticks="120" fromvoc="2" noPongKickTime="40">
		<formula meleeDamage="1.0" distDamage="1.0" defense="1.0" armor="1.0" />
		<skill id="0" multiplier="1.5" />
		<skill id="1" multiplier="1.8" />
		<skill id="2" multiplier="1.8" />
		<skill id="3" multiplier="1.8" />
		<skill id="4" multiplier="1.8" />
		<skill id="5" multiplier="1.5" />
		<skill id="6" multiplier="1.1" />
	</vocation>
	<vocation id="3" clientid="2" name="Paladin" description="a paladin" magicshield="0" gaincap="20" gainhp="10" gainmana="15" gainhpticks="4" gainhpamount="5" gainmanaticks="4" gainmanaamount="5" manamultiplier="1.4" attackspeed="2000" basespeed="220" soulmax="100" gainsoulticks="120" fromvoc="3" noPongKickTime="50">
		<formula meleeDamage="1.0" distDamage="1.0" defense="1.0" armor="1.0" />
		<skill id="0" multiplier="1.2" />
		<skill id="1" multiplier="1.2" />
		<skill id="2" multiplier="1.2" />
		<skill id="3" multiplier="1.2" />
		<skill id="4" multiplier="1.1" />
		<skill id="5" multiplier="1.1" />
		<skill id="6" multiplier="1.1" />
	</vocation>
	<vocation id="4" clientid="1" name="Knight" description="a knight" magicshield="0" gaincap="25" gainhp="15" gainmana="5" gainhpticks="3" gainhpamount="5" gainmanaticks="6" gainmanaamount="5" manamultiplier="3.0" attackspeed="2000" basespeed="220" soulmax="100" gainsoulticks="120" fromvoc="4">
		<formula meleeDamage="1.0" distDamage="1.0" defense="1.0" armor="1.0" />
		<skill id="0" multiplier="1.1" />
		<skill id="1" multiplier="1.1" />
		<skill id="2" multiplier="1.1" />
		<skill id="3" multiplier="1.1" />
		<skill id="4" multiplier="1.4" />
		<skill id="5" multiplier="1.1" />
		<skill id="6" multiplier="1.1" />
	</vocation>
	<vocation id="5" clientid="13" name="Master Sorcerer" description="a master sorcerer" magicshield="1" gaincap="10" gainhp="5" gainmana="30" gainhpticks="4" gainhpamount="10" gainmanaticks="2" gainmanaamount="10" manamultiplier="1.1" attackspeed="2000" basespeed="220" soulmax="200" gainsoulticks="15" fromvoc="1" noPongKickTime="40">
		<formula meleeDamage="1.0" distDamage="1.0" defense="1.0" armor="1.0" />
		<skill id="0" multiplier="1.5" />
		<skill id="1" multiplier="2.0" />
		<skill id="2" multiplier="2.0" />
		<skill id="3" multiplier="2.0" />
		<skill id="4" multiplier="2.0" />
		<skill id="5" multiplier="1.5" />
		<skill id="6" multiplier="1.1" />
	</vocation>
	<vocation id="6" clientid="14" name="Elder Druid" description="an elder druid" magicshield="1" gaincap="10" gainhp="5" gainmana="30" gainhpticks="4" gainhpamount="10" gainmanaticks="2" gainmanaamount="10" manamultiplier="1.1" attackspeed="2000" basespeed="220" soulmax="200" gainsoulticks="15" fromvoc="2" noPongKickTime="40">
		<formula meleeDamage="1.0" distDamage="1.0" defense="1.0" armor="1.0" />
		<skill id="0" multiplier="1.5" />
		<skill id="1" multiplier="1.8" />
		<skill id="2" multiplier="1.8" />
		<skill id="3" multiplier="1.8" />
		<skill id="4" multiplier="1.8" />
		<skill id="5" multiplier="1.5" />
		<skill id="6" multiplier="1.1" />
	</vocation>
	<vocation id="7" clientid="12" name="Royal Paladin" description="a royal paladin" magicshield="0" gaincap="20" gainhp="10" gainmana="15" gainhpticks="3" gainhpamount="10" gainmanaticks="3" gainmanaamount="10" manamultiplier="1.4" attackspeed="2000" basespeed="220" soulmax="200" gainsoulticks="15" fromvoc="3" noPongKickTime="50">
		<formula meleeDamage="1.0" distDamage="1.0" defense="1.0" armor="1.0" />
		<skill id="0" multiplier="1.2" />
		<skill id="1" multiplier="1.2" />
		<skill id="2" multiplier="1.2" />
		<skill id="3" multiplier="1.2" />
		<skill id="4" multiplier="1.1" />
		<skill id="5" multiplier="1.1" />
		<skill id="6" multiplier="1.1" />
	</vocation>
	<vocation id="8" clientid="11" name="Elite Knight" description="an elite knight" magicshield="0" gaincap="25" gainhp="15" gainmana="5" gainhpticks="2" gainhpamount="10" gainmanaticks="4" gainmanaamount="10" manamultiplier="3.0" attackspeed="2000" basespeed="220" soulmax="200" gainsoulticks="15" fromvoc="4">
		<formula meleeDamage="1.0" distDamage="1.0" defense="1.0" armor="1.0" />
		<skill id="0" multiplier="1.1" />
		<skill id="1" multiplier="1.1" />
		<skill id="2" multiplier="1.1" />
		<skill id="3" multiplier="1.1" />
		<skill id="4" multiplier="1.4" />
		<skill id="5" multiplier="1.1" />
		<skill id="6" multiplier="1.1" />
	</vocation>
</vocations>)"};
};

using namespace std::chrono;

struct LoginFixture
{
	LoginFixture()
	{
		setString(ConfigManager::SERVER_NAME, "Forgotten");
		setString(ConfigManager::IP, "tfs.example.com");
		setNumber(ConfigManager::GAME_PORT, 7171);
		setString(ConfigManager::LOCATION, "Sweden");
		setString(ConfigManager::WORLD_TYPE, "pvp");

		setString(ConfigManager::MYSQL_HOST, "0.0.0.0");
		setString(ConfigManager::MYSQL_USER, "forgottenserver");
		setString(ConfigManager::MYSQL_PASS, "forgottenserver");
		setString(ConfigManager::MYSQL_DB, "forgottenserver");
		setNumber(ConfigManager::SQL_PORT, 3306);

		auto is = vocationsXml();
		g_vocations.loadFromXml(is, ":memory:");

		db.connect();
		transaction.begin();
	}

	~LoginFixture()
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

BOOST_FIXTURE_TEST_CASE(test_login_missing_email, LoginFixture)
{
	auto&& [status, body] = tfs::http::handle_login({{"type", "login"}, {"password", "bar"}}, ip);

	BOOST_TEST(status == status::ok);
	BOOST_TEST(body.at("errorCode").as_int64() == 3);
}

BOOST_FIXTURE_TEST_CASE(test_login_account_does_not_exist, LoginFixture)
{
	auto&& [status, body] =
	    tfs::http::handle_login({{"type", "login"}, {"email", "k@example.com"}, {"password", "bar"}}, ip);

	BOOST_TEST(status == status::ok);
	BOOST_TEST(body.at("errorCode").as_int64() == 3);
}

BOOST_FIXTURE_TEST_CASE(test_login_missing_password, LoginFixture)
{
	auto&& [status, body] = tfs::http::handle_login({{"type", "login"}, {"email", "foo@example.com"}}, ip);

	BOOST_TEST(status == status::ok);
	BOOST_TEST(body.at("errorCode").as_int64() == 3);
}

BOOST_FIXTURE_TEST_CASE(test_login_invalid_password, LoginFixture)
{
	BOOST_TEST(db.executeQuery(
	    "INSERT INTO `accounts` (`name`, `email`, `password`) VALUES ('abc', 'foo@example.com', SHA1('bar'))"));

	auto&& [status, body] =
	    tfs::http::handle_login({{"type", "login"}, {"email", "foo@example.com"}, {"password", "baz"}}, ip);

	BOOST_TEST(status == status::ok);
	BOOST_TEST(body.at("errorCode").as_int64() == 3);
}

BOOST_FIXTURE_TEST_CASE(test_login_missing_token, LoginFixture)
{
	BOOST_TEST(db.executeQuery(
	    "INSERT INTO `accounts` (`name`, `email`, `password`, `secret`) VALUES ('abcd', 'fooba@example.com', SHA1('bar'), UNHEX('48656c6c6f21dead'))"));

	auto&& [status, body] = tfs::http::handle_login(
	    {
	        {"type", "login"},
	        {"email", "fooba@example.com"},
	        {"password", "bar"},
	    },
	    ip);

	BOOST_TEST(status == status::ok);
	BOOST_TEST(body.at("errorCode").as_int64() == 6);
}

BOOST_FIXTURE_TEST_CASE(test_login_success_no_players, LoginFixture)
{
	BOOST_TEST(db.executeQuery(
	    "INSERT INTO `accounts` (`name`, `email`, `password`) VALUES ('defg', 'foobar@example.com', SHA1('bar'))"));

	auto&& [status, body] =
	    tfs::http::handle_login({{"type", "login"}, {"email", "foobar@example.com"}, {"password", "bar"}}, ip);

	BOOST_TEST(status == status::ok);
	auto& characters = body.at("playdata").at("characters").as_array();
	BOOST_TEST(characters.size() == 0);
}

BOOST_FIXTURE_TEST_CASE(test_login_success, LoginFixture)
{
	auto premiumEndsAt = now + days(30);

	auto result = db.storeQuery(fmt::format(
	    "INSERT INTO `accounts` (`name`, `email`, `password`, `premium_ends_at`) VALUES ('ghij', 'ghij@example.com', SHA1('bar'), {:d}) RETURNING `id`",
	    premiumEndsAt.count()));
	auto id = result->getNumber<uint64_t>("id");

	DBInsert insert(
	    "INSERT INTO `players` (`account_id`, `name`, `level`, `vocation`, `lastlogin`, `sex`, `looktype`, `lookhead`, `lookbody`, `looklegs`, `lookfeet`, `lookaddons`) VALUES");
	insert.addRow(fmt::format("{:d}, \"{:s}\", {:d}, {:d}, {:d}, {:d}, {:d}, {:d}, {:d}, {:d}, {:d}, {:d}", id,
	                          "Test", 2597, 6, 1715719401, 1, 1094, 78, 132, 114, 0, 1));
	BOOST_TEST(insert.execute());

	auto&& [status, body] =
	    tfs::http::handle_login({{"type", "login"}, {"email", "ghij@example.com"}, {"password", "bar"}}, ip);

	BOOST_TEST(status == status::ok);

	auto& session = body.at("session");
	BOOST_TEST(session.at("lastlogintime").as_uint64() == 1715719401);
	BOOST_TEST(session.at("ispremium").as_bool() == true);
	BOOST_TEST(session.at("premiumuntil").as_int64() == premiumEndsAt.count());

	result = db.storeQuery(
	    fmt::format("SELECT `token`, INET6_NTOA(`ip`) AS `ip` FROM `sessions` WHERE `account_id` = {:d}", id));
	BOOST_TEST(result, "Session not found in database.");
	BOOST_TEST(result->getString("token") == tfs::base64::decode(session.at("sessionkey").as_string()));
	BOOST_TEST(result->getString("ip") == ip);

	auto& worlds = body.at("playdata").at("worlds").as_array();
	BOOST_TEST(worlds.size() == 1);
	BOOST_TEST(worlds[0].at("id").as_int64() == 0);
	BOOST_TEST(worlds[0].at("name").as_string() == "Forgotten");
	BOOST_TEST(worlds[0].at("externaladdressprotected").as_string() == "tfs.example.com");
	BOOST_TEST(worlds[0].at("externalportprotected").as_int64() == 7171);
	BOOST_TEST(worlds[0].at("externaladdressunprotected").as_string() == "tfs.example.com");
	BOOST_TEST(worlds[0].at("externalportunprotected").as_int64() == 7171);
	BOOST_TEST(worlds[0].at("location").as_string() == "Sweden");
	BOOST_TEST(worlds[0].at("pvptype").as_int64() == 0);

	auto& characters = body.at("playdata").at("characters").as_array();
	BOOST_TEST(characters.size() == 1);
	BOOST_TEST(characters[0].at("name").as_string() == "Test");
	BOOST_TEST(characters[0].at("level").as_uint64() == 2597);
	BOOST_TEST(characters[0].at("vocation").as_string() == "Elder Druid");
	BOOST_TEST(characters[0].at("lastlogin").as_uint64() == 1715719401);
	BOOST_TEST(characters[0].at("ismale").as_bool() == true);
	BOOST_TEST(characters[0].at("outfitid").as_uint64() == 1094);
	BOOST_TEST(characters[0].at("headcolor").as_uint64() == 78);
	BOOST_TEST(characters[0].at("torsocolor").as_uint64() == 132);
	BOOST_TEST(characters[0].at("legscolor").as_uint64() == 114);
	BOOST_TEST(characters[0].at("detailcolor").as_uint64() == 0);
	BOOST_TEST(characters[0].at("addonsflags").as_uint64() == 1);
}

BOOST_FIXTURE_TEST_CASE(test_login_success_with_token, LoginFixture)
{
	auto result = db.storeQuery(
	    "INSERT INTO `accounts` (`name`, `email`, `password`, `secret`) VALUES ('nbdj', 'nbdj@example.com', SHA1('bar'), UNHEX('')) RETURNING `id`");
	auto id = result->getNumber<uint64_t>("id");

	DBInsert insert("INSERT INTO `players` (`account_id`, `name`, `level`, `vocation`, `lastlogin`) VALUES");
	insert.addRow(fmt::format("{:d}, \"{:s}\", {:d}, {:d}, {:d}", id, "Testtoken", 2597, 6, 1715719401));
	BOOST_TEST(insert.execute());

	auto&& [status, body] = tfs::http::handle_login(
	    {
	        {"type", "login"},
	        {"email", "nbdj@example.com"},
	        {"password", "bar"},
	        {"token", generateToken("", now.count() / AUTHENTICATOR_PERIOD)},
	    },
	    ip);

	BOOST_TEST(status == status::ok);
}
