/**
 * The Forgotten Server - a server application for the MMORPG Tibia
 * Copyright (C) 2013  Mark Samman <mark.samman@gmail.com>
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

#ifndef _CONFIG_MANAGER_H
#define _CONFIG_MANAGER_H

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

class ConfigManager
{
	public:
		ConfigManager();
		~ConfigManager();

		enum boolean_config_t {
			ALLOW_CHANGEOUTFIT = 0,
			CANNOT_ATTACK_SAME_LOOKFEET = 1,
			ONE_PLAYER_ON_ACCOUNT = 2,
			AIMBOT_HOTKEY_ENABLED = 3,
			REMOVE_AMMO = 4,
			REMOVE_RUNE_CHARGES = 5,
			REMOVE_WEAPON_CHARGES = 6,
			EXPERIENCE_FROM_PLAYERS = 7,
			FREE_PREMIUM = 8,
			ADMIN_LOGS_ENABLED = 9,
			SAVE_GLOBAL_STORAGE = 10,
			REPLACE_KICK_ON_LOGIN = 11,
			ALLOW_CLONES = 12,
			BIND_ONLY_GLOBAL_ADDRESS = 13,
			OPTIMIZE_DATABASE = 14,
			MARKET_PREMIUM = 15,
			STAMINA_SYSTEM = 16,
			LAST_BOOLEAN_CONFIG /* this must be the last one */
		};

		enum string_config_t {
			DUMMY_STR = 0,
			MAP_NAME = 1,
			HOUSE_RENT_PERIOD = 2,
			LOGIN_MSG = 3,
			FIRST_MSG = 4,
			SERVER_NAME = 5,
			OWNER_NAME = 6,
			OWNER_EMAIL = 7,
			URL = 8,
			LOCATION = 9,
			IP = 10,
			MOTD = 11,
			WORLD_TYPE = 12,
			MYSQL_HOST = 13,
			MYSQL_USER = 14,
			MYSQL_PASS = 15,
			MYSQL_DB = 16,
			DEFAULT_PRIORITY = 17,
			MAP_AUTHOR = 18,
			LAST_STRING_CONFIG /* this must be the last one */
		};

		enum integer_config_t {
			LOGIN_TRIES = 0,
			SQL_PORT = 1,
			MAX_PLAYERS = 2,
			PZ_LOCKED = 3,
			DEFAULT_DESPAWNRANGE = 4,
			DEFAULT_DESPAWNRADIUS = 5,
			RATE_EXPERIENCE = 6,
			RATE_SKILL = 7,
			RATE_LOOT = 8,
			RATE_MAGIC = 9,
			RATE_SPAWN = 10,
			HOUSE_PRICE = 11,
			KILLS_TO_RED = 12,
			KILLS_TO_BLACK = 13,
			MAX_MESSAGEBUFFER = 14,
			ACTIONS_DELAY_INTERVAL = 15,
			EX_ACTIONS_DELAY_INTERVAL = 16,
			KICK_AFTER_MINUTES = 17,
			PROTECTION_LEVEL = 18,
			DEATH_LOSE_PERCENT = 19,
			STATUSQUERY_TIMEOUT = 20,
			FRAG_TIME = 21,
			WHITE_SKULL_TIME = 22,
			ADMIN_PORT = 23,
			GAME_PORT = 24,
			LOGIN_PORT = 25,
			STATUS_PORT = 26,
			STAIRHOP_DELAY = 27,
			MARKET_OFFER_DURATION = 28,
			CHECK_EXPIRED_MARKET_OFFERS_EACH_MINUTES = 29,
			MAX_MARKET_OFFERS_AT_A_TIME_PER_PLAYER = 30,
			EXP_FROM_PLAYERS_LEVEL_RANGE = 31,
			MAX_PACKETS_PER_SECOND = 32,
			LAST_INTEGER_CONFIG /* this must be the last one */
		};

		bool load();
		bool reload();

		const std::string& getString(string_config_t _what) const;
		int32_t getNumber(integer_config_t _what) const;
		bool getBoolean(boolean_config_t _what) const;
		bool setNumber(integer_config_t _what, int32_t _value);

	private:
		std::string getGlobalString(lua_State* _L, const std::string& _identifier, const std::string& _default = "");
		int32_t getGlobalNumber(lua_State* _L, const std::string& _identifier, const int32_t _default = 0);
		std::string getGlobalStringField(lua_State* _L, const std::string& _identifier, const int32_t _key, const std::string& _default = "");

		bool m_isLoaded;
		std::string m_confString[LAST_STRING_CONFIG];
		int32_t m_confInteger[LAST_INTEGER_CONFIG];
		bool m_confBoolean[LAST_BOOLEAN_CONFIG];
};

#endif
