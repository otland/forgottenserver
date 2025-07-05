#include "../otpch.h"

#include "serverinfo.h"

#include "../configmanager.h"
#include "../game.h"
#include "http.h"

namespace beast = boost::beast;
namespace json = boost::json;
using boost::beast::http::status;

std::pair<status, json::value> tfs::http::handle_serverinfo(const json::object&, std::string_view)
{
	uint32_t mapWidth, mapHeight;
	g_game.getMapDimensions(mapWidth, mapHeight);

	return {
	    status::ok,
	    {{
	         "serverinfo",
	         {{"started", std::format("{:%FT%TZ}", round<std::chrono::seconds>(serverStart()))},
	          {"uptime", duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - serverStart()).count()},
	          {"servername", getString(ConfigManager::SERVER_NAME)},
	          {"port", getNumber(ConfigManager::LOGIN_PORT)},
	          {"location", getString(ConfigManager::LOCATION)},
	          {"url", getString(ConfigManager::URL)},
	          {"server", STATUS_SERVER_NAME},
	          {"version", STATUS_SERVER_VERSION},
	          {"client", CLIENT_VERSION_STR}},
	     },
	     {"owner", {{"name", getString(ConfigManager::OWNER_NAME)}, {"email", getString(ConfigManager::OWNER_EMAIL)}}},
	     {"players",
	      {{"online", g_game.getPlayersOnline()},
	       {"max", getNumber(ConfigManager::MAX_PLAYERS)},
	       {"peak", g_game.getPlayersRecord()}}},
	     {"monsters", {{"total", g_game.getMonstersOnline()}}},
	     {"npcs", {{"total", g_game.getNpcsOnline()}}},
	     {"rates",
	      {{"experience", getNumber(ConfigManager::RATE_EXPERIENCE)},
	       {"skill", getNumber(ConfigManager::RATE_SKILL)},
	       {"loot", getNumber(ConfigManager::RATE_LOOT)},
	       {"magic", getNumber(ConfigManager::RATE_MAGIC)},
	       {"spawn", getNumber(ConfigManager::RATE_SPAWN)}}},
	     {"map",
	      {{"name", getString(ConfigManager::MAP_NAME)},
	       {"author", getString(ConfigManager::MAP_AUTHOR)},
	       {"width", mapWidth},
	       {"height", mapHeight}}}}};
}
