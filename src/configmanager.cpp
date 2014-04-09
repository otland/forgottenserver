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

#include "otpch.h"

#include "configmanager.h"
#include "game.h"
#include "tools.h"

#include <stdexcept>

#if LUA_VERSION_NUM >= 502
#define lua_strlen lua_rawlen
#endif

extern Game g_game;

ConfigManager::ConfigManager()
{
	m_isLoaded = false;
}

ConfigManager::~ConfigManager()
{
	//
}

bool ConfigManager::load()
{
	lua_State* L = luaL_newstate();
	if (!L) {
		throw std::runtime_error("Failed to allocate memory");
	}

	luaL_openlibs(L);

	if (luaL_dofile(L, "config.lua")) {
		std::cout << "[Error - ConfigManager::load] " << lua_tostring(L, -1) << std::endl;
		lua_close(L);
		return false;
	}

	//parse config
	if (!m_isLoaded) { //info that must be loaded one time (unless we reset the modules involved)
		m_confBoolean[BIND_ONLY_GLOBAL_ADDRESS] = booleanString(getGlobalString(L, "bindOnlyGlobalAddress", "no"));
		m_confBoolean[OPTIMIZE_DATABASE] = booleanString(getGlobalString(L, "startupDatabaseOptimization", "yes"));

		m_confString[IP] = getGlobalString(L, "ip", "127.0.0.1");
		m_confString[MAP_NAME] = getGlobalString(L, "mapName", "forgotten");
		m_confString[MAP_AUTHOR] = getGlobalString(L, "mapAuthor", "Unknown");
		m_confString[HOUSE_RENT_PERIOD] = getGlobalString(L, "houseRentPeriod", "monthly");
		m_confString[MYSQL_HOST] = getGlobalString(L, "mysqlHost", "localhost");
		m_confString[MYSQL_USER] = getGlobalString(L, "mysqlUser", "root");
		m_confString[MYSQL_PASS] = getGlobalString(L, "mysqlPass", "");
		m_confString[MYSQL_DB] = getGlobalString(L, "mysqlDatabase", "theforgottenserver");
		m_confString[MYSQL_SOCK] = getGlobalString(L, "mysqlSock", "");

		m_confInteger[SQL_PORT] = getGlobalNumber(L, "mysqlPort", 3306);
		m_confInteger[GAME_PORT] = getGlobalNumber(L, "gameProtocolPort", 7172);
		m_confInteger[LOGIN_PORT] = getGlobalNumber(L, "loginProtocolPort", 7171);
		m_confInteger[STATUS_PORT] = getGlobalNumber(L, "statusProtocolPort", 7171);

		m_confInteger[MARKET_OFFER_DURATION] = getGlobalNumber(L, "marketOfferDuration",  30 * 24 * 60 * 60);
	}

	m_confBoolean[ALLOW_CHANGEOUTFIT] = booleanString(getGlobalString(L, "allowChangeOutfit", "yes"));
	m_confBoolean[ONE_PLAYER_ON_ACCOUNT] = booleanString(getGlobalString(L, "onePlayerOnlinePerAccount", "yes"));
	m_confBoolean[CANNOT_ATTACK_SAME_LOOKFEET] = booleanString(getGlobalString(L, "noDamageToSameLookfeet", "no"));
	m_confBoolean[AIMBOT_HOTKEY_ENABLED] = booleanString(getGlobalString(L, "hotkeyAimbotEnabled", "yes"));
	m_confBoolean[REMOVE_AMMO] = booleanString(getGlobalString(L, "removeAmmoWhenUsingDistanceWeapon", "yes"));
	m_confBoolean[REMOVE_RUNE_CHARGES] = booleanString(getGlobalString(L, "removeChargesFromRunes", "yes"));
	m_confBoolean[EXPERIENCE_FROM_PLAYERS] = booleanString(getGlobalString(L, "experienceByKillingPlayers", "no"));
	m_confBoolean[FREE_PREMIUM] = booleanString(getGlobalString(L, "freePremium", "no"));
	m_confBoolean[REPLACE_KICK_ON_LOGIN] = booleanString(getGlobalString(L, "replaceKickOnLogin", "yes"));
	m_confBoolean[ALLOW_CLONES] = booleanString(getGlobalString(L, "allowClones", "no"));
	m_confBoolean[MARKET_PREMIUM] = booleanString(getGlobalString(L, "premiumToCreateMarketOffer", "yes"));
	m_confBoolean[EMOTE_SPELLS] = booleanString(getGlobalString(L, "emoteSpells", "no"));
	m_confBoolean[STAMINA_SYSTEM] = booleanString(getGlobalString(L, "staminaSystem", "yes"));

	m_confString[DEFAULT_PRIORITY] = getGlobalString(L, "defaultPriority", "high");
	m_confString[SERVER_NAME] = getGlobalString(L, "serverName");
	m_confString[OWNER_NAME] = getGlobalString(L, "ownerName");
	m_confString[OWNER_EMAIL] = getGlobalString(L, "ownerEmail");
	m_confString[URL] = getGlobalString(L, "url");
	m_confString[LOCATION] = getGlobalString(L, "location");
	m_confString[MOTD] = getGlobalString(L, "motd");
	m_confString[WORLD_TYPE] = getGlobalString(L, "worldType", "pvp");

	m_confInteger[MAX_PLAYERS] = getGlobalNumber(L, "maxPlayers");
	m_confInteger[PZ_LOCKED] = getGlobalNumber(L, "pzLocked", 0);
	m_confInteger[DEFAULT_DESPAWNRANGE] = getGlobalNumber(L, "deSpawnRange", 2);
	m_confInteger[DEFAULT_DESPAWNRADIUS] = getGlobalNumber(L, "deSpawnRadius", 50);
	m_confInteger[RATE_EXPERIENCE] = getGlobalNumber(L, "rateExp", 1);
	m_confInteger[RATE_SKILL] = getGlobalNumber(L, "rateSkill", 1);
	m_confInteger[RATE_LOOT] = getGlobalNumber(L, "rateLoot", 1);
	m_confInteger[RATE_MAGIC] = getGlobalNumber(L, "rateMagic", 1);
	m_confInteger[RATE_SPAWN] = getGlobalNumber(L, "rateSpawn", 1);
	m_confInteger[HOUSE_PRICE] = getGlobalNumber(L, "housePriceEachSQM", 1000);
	m_confInteger[KILLS_TO_RED] = getGlobalNumber(L, "killsToRedSkull", 3);
	m_confInteger[KILLS_TO_BLACK] = getGlobalNumber(L, "killsToBlackSkull", 6);
	m_confInteger[ACTIONS_DELAY_INTERVAL] = getGlobalNumber(L, "timeBetweenActions", 200);
	m_confInteger[EX_ACTIONS_DELAY_INTERVAL] = getGlobalNumber(L, "timeBetweenExActions", 1000);
	m_confInteger[MAX_MESSAGEBUFFER] = getGlobalNumber(L, "maxMessageBuffer", 4);
	m_confInteger[KICK_AFTER_MINUTES] = getGlobalNumber(L, "kickIdlePlayerAfterMinutes", 15);
	m_confInteger[PROTECTION_LEVEL] = getGlobalNumber(L, "protectionLevel", 1);
	m_confInteger[DEATH_LOSE_PERCENT] = getGlobalNumber(L, "deathLosePercent", -1);
	m_confInteger[STATUSQUERY_TIMEOUT] = getGlobalNumber(L, "statusTimeout", 60000);
	m_confInteger[FRAG_TIME] = getGlobalNumber(L, "timeToDecreaseFrags", 24 * 60 * 60 * 1000);
	m_confInteger[WHITE_SKULL_TIME] = getGlobalNumber(L, "whiteSkullTime", 15 * 60 * 1000);
	m_confInteger[STAIRHOP_DELAY] = getGlobalNumber(L, "stairJumpExhaustion", 2000);
	m_confInteger[EXP_FROM_PLAYERS_LEVEL_RANGE] = getGlobalNumber(L, "expFromPlayersLevelRange", 75);
	m_confInteger[CHECK_EXPIRED_MARKET_OFFERS_EACH_MINUTES] = getGlobalNumber(L, "checkExpiredMarketOffersEachMinutes", 60);
	m_confInteger[MAX_MARKET_OFFERS_AT_A_TIME_PER_PLAYER] = getGlobalNumber(L, "maxMarketOffersAtATimePerPlayer", 100);
	m_confInteger[MAX_PACKETS_PER_SECOND] = getGlobalNumber(L, "maxPacketsPerSecond", 40);

	m_isLoaded = true;
	lua_close(L);
	return true;
}

bool ConfigManager::reload()
{
	if (!m_isLoaded) {
		return false;
	}

	bool result = load();
	if (transformToSHA1(getString(ConfigManager::MOTD)) != g_game.getMotdHash()) {
		g_game.incrementMotdNum();
	}
	return result;
}

const std::string& ConfigManager::getString(string_config_t _what) const
{
	if (m_isLoaded && _what < LAST_STRING_CONFIG) {
		return m_confString[_what];
	} else {
		std::cout << "[Warning - ConfigManager::getString] " << _what << std::endl;
		return m_confString[DUMMY_STR];
	}
}

int32_t ConfigManager::getNumber(integer_config_t _what) const
{
	if (m_isLoaded && _what < LAST_INTEGER_CONFIG) {
		return m_confInteger[_what];
	} else {
		std::cout << "[Warning - ConfigManager::getNumber] " << _what << std::endl;
		return 0;
	}
}

bool ConfigManager::getBoolean(boolean_config_t _what) const
{
	if (m_isLoaded && _what < LAST_BOOLEAN_CONFIG) {
		return m_confBoolean[_what];
	} else {
		std::cout << "[Warning - ConfigManager::getBoolean] " << _what << std::endl;
		return false;
	}
}

std::string ConfigManager::getGlobalString(lua_State* _L, const std::string& _identifier, const std::string& _default)
{
	lua_getglobal(_L, _identifier.c_str());

	if (!lua_isstring(_L, -1)) {
		return _default;
	}

	size_t len = lua_strlen(_L, -1);
	std::string ret(lua_tostring(_L, -1), len);
	lua_pop(_L, 1);
	return ret;
}

int32_t ConfigManager::getGlobalNumber(lua_State* _L, const std::string& _identifier, const int32_t _default)
{
	lua_getglobal(_L, _identifier.c_str());

	if (!lua_isnumber(_L, -1)) {
		return _default;
	}

	int32_t val = lua_tonumber(_L, -1);
	lua_pop(_L, 1);
	return val;
}
