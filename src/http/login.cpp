#include "../otpch.h"

#include "login.h"

#include "../base64.h"
#include "../game.h"
#include "error.h"

#include <fmt/format.h>

extern Game g_game;
extern Vocations g_vocations;

namespace beast = boost::beast;
namespace json = boost::json;
using boost::beast::http::status;

namespace {

int getPvpType()
{
	switch (g_game.getWorldType()) {
		case WORLD_TYPE_PVP:
			return 0;
		case WORLD_TYPE_NO_PVP:
			return 1;
		case WORLD_TYPE_PVP_ENFORCED:
			return 2;
	}

	tfs::unreachable();
}

} // namespace

std::pair<status, json::value> tfs::http::handle_login(const json::object& body, std::string_view ip)
{
	using namespace std::chrono;

	auto emailField = body.if_contains("email");
	if (!emailField || !emailField->is_string()) {
		return make_error_response(
		    {.code = 3, .message = "Tibia account email address or Tibia password is not correct."});
	}

	auto passwordField = body.if_contains("password");
	if (!passwordField || !passwordField->is_string()) {
		return make_error_response(
		    {.code = 3, .message = "Tibia account email address or Tibia password is not correct."});
	}

	thread_local auto& db = Database::getInstance();
	auto result = db.storeQuery(fmt::format(
	    "SELECT `id`, `password`, UNHEX(`password`) AS `password_unhex`, `secret`, `premium_ends_at` FROM `accounts` WHERE `email` = {:s}",
	    db.escapeString(emailField->get_string())));
	if (!result) {
		return make_error_response(
		    {.code = 3, .message = "Tibia account email address or Tibia password is not correct."});
	}

	auto passwordUnhex = result->getString("password_unhex");
	if (passwordUnhex != transformToSHA1(passwordField->get_string())) {
		return make_error_response(
		    {.code = 3, .message = "Tibia account email address or Tibia password is not correct."});
	}

	auto now = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();

	auto secret = result->getString("secret");
	if (!secret.empty()) {
		auto token = body.if_contains("token");
		if (!token || !token->is_string()) {
			return make_error_response({.code = 6, .message = "Two-factor token required for authentication."});
		}

		uint64_t ticks = now / AUTHENTICATOR_PERIOD;
		if (token->get_string() != generateToken(secret, ticks) &&
		    token->get_string() != generateToken(secret, ticks - 1) &&
		    token->get_string() != generateToken(secret, ticks + 1)) {
			return make_error_response({.code = 6, .message = "Two-factor token required for authentication."});
		}
	}

	auto accountId = result->getNumber<uint32_t>("id");
	auto premiumEndsAt = result->getNumber<int64_t>("premium_ends_at");

	auto passwordHash = result->getString("password");
	SessionToken sessionToken(accountId, passwordHash, static_cast<uint32_t>(time(nullptr)), ip,
	                          getString(ConfigManager::SESSION_TOKEN_SECRET_KEY));

	result = db.storeQuery(fmt::format(
	    "SELECT `id`, `name`, `level`, `vocation`, `lastlogin`, `sex`, `looktype`, `lookhead`, `lookbody`, `looklegs`, `lookfeet`, `lookaddons` FROM `players` WHERE `account_id` = {:d}",
	    accountId));

	json::array characters;
	uint32_t lastLogin = 0;
	if (result) {
		do {
			auto vocation = g_vocations.getVocation(result->getNumber<uint32_t>("vocation"));
			assert(vocation);

			characters.push_back({
			    {"worldid", 0}, // not implemented
			    {"name", result->getString("name")},
			    {"level", result->getNumber<uint32_t>("level")},
			    {"vocation", vocation->getVocName()},
			    {"lastlogin", result->getNumber<uint64_t>("lastlogin")},
			    {"ismale", result->getNumber<uint16_t>("sex") == PLAYERSEX_MALE},
			    {"ishidden", false},        // not implemented
			    {"ismaincharacter", false}, // not implemented
			    {"tutorial", false},        // not implemented
			    {"outfitid", result->getNumber<uint32_t>("looktype")},
			    {"headcolor", result->getNumber<uint32_t>("lookhead")},
			    {"torsocolor", result->getNumber<uint32_t>("lookbody")},
			    {"legscolor", result->getNumber<uint32_t>("looklegs")},
			    {"detailcolor", result->getNumber<uint32_t>("lookfeet")},
			    {"addonsflags", result->getNumber<uint32_t>("lookaddons")},
			    {"dailyrewardstate", 0}, // not implemented
			});

			lastLogin = std::max(lastLogin, result->getNumber<uint32_t>("lastlogin"));
		} while (result->next());
	}

	json::array worlds{
	    {
	        {"id", 0}, // not implemented
	        {"name", getString(ConfigManager::SERVER_NAME)},
	        {"externaladdressprotected", getString(ConfigManager::IP)},
	        {"externalportprotected", getNumber(ConfigManager::GAME_PORT)},
	        {"externaladdressunprotected", getString(ConfigManager::IP)},
	        {"externalportunprotected", getNumber(ConfigManager::GAME_PORT)},
	        {"previewstate", 0}, // not implemented
	        {"location", getString(ConfigManager::LOCATION)},
	        {"anticheatprotection", false}, // not implemented
	        {"pvptype", getPvpType()},
	    },
	};

	return {
	    status::ok,
	    {
	        {"session",
	         {
	             {"sessionkey", sessionToken.getToken()},
	             {"lastlogintime", lastLogin},
	             {"ispremium", premiumEndsAt >= now},
	             {"premiumuntil", premiumEndsAt},
	             // not implemented
	             {"status", "active"},
	             {"returnernotification", false},
	             {"showrewardnews", true},
	             {"isreturner", true},
	             {"recoverysetupcomplete", true},
	             {"fpstracking", false},
	             {"optiontracking", false},
	         }},
	        {"playdata",
	         {
	             {"worlds", worlds},
	             {"characters", characters},
	         }},
	    },
	};
}
