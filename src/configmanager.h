//////////////////////////////////////////////////////////////////////
// The Forgotten Server - a server application for the MMORPG Tibia
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#ifndef _CONFIG_MANAGER_H
#define _CONFIG_MANAGER_H

#include <string>

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
			ON_OR_OFF_CHARLIST = 0,
			ALLOW_CHANGEOUTFIT,
			CANNOT_ATTACK_SAME_LOOKFEET,
			ONE_PLAYER_ON_ACCOUNT,
			AIMBOT_HOTKEY_ENABLED,
			SHOW_GAMEMASTERS_ONLINE,
			REMOVE_AMMO,
			REMOVE_RUNE_CHARGES,
			REMOVE_WEAPON_CHARGES,
			EXPERIENCE_FROM_PLAYERS,
			SHUTDOWN_AT_SERVERSAVE,
			CLEAN_MAP_AT_SERVERSAVE,
			SERVERSAVE_ENABLED,
			FREE_PREMIUM,
			ADMIN_LOGS_ENABLED,
			SAVE_GLOBAL_STORAGE,
			REPLACE_KICK_ON_LOGIN,
			OLD_CONDITION_ACCURACY,
			BROADCAST_BANISHMENTS,
			FREE_MEMORY_AT_SHUTDOWN,
			ALLOW_CLONES,
			BIND_ONLY_GLOBAL_ADDRESS,
			OPTIMIZE_DATABASE,
			MARKET_ENABLED,
			MARKET_PREMIUM,
			STAMINA_SYSTEM,
			LAST_BOOLEAN_CONFIG /* this must be the last one */
		};

		enum string_config_t {
			DUMMY_STR = 0,
			CONFIG_FILE,
			MAP_NAME,
			HOUSE_RENT_PERIOD,
			LOGIN_MSG,
			FIRST_MSG,
			SERVER_NAME,
			OWNER_NAME,
			OWNER_EMAIL,
			URL,
			LOCATION,
			IP,
			MOTD,
			WORLD_TYPE,
			MYSQL_HOST,
			MYSQL_USER,
			MYSQL_PASS,
			MYSQL_DB,
			DEFAULT_PRIORITY,
			PASSWORDTYPE,
			MAP_AUTHOR,
			MAP_STORAGE_TYPE,
			LAST_STRING_CONFIG /* this must be the last one */
		};

		enum integer_config_t {
			LOGIN_TRIES = 0,
			RETRY_TIMEOUT,
			LOGIN_TIMEOUT,
			SQL_PORT,
			MAX_PLAYERS,
			PZ_LOCKED,
			DEFAULT_DESPAWNRANGE,
			DEFAULT_DESPAWNRADIUS,
			RATE_EXPERIENCE,
			RATE_SKILL,
			RATE_LOOT,
			RATE_MAGIC,
			RATE_SPAWN,
			SERVERSAVE_H,
			HOUSE_PRICE,
			KILLS_TO_RED,
			KILLS_TO_BLACK,
			KILLS_TO_BAN,
			BAN_DAYS,
			FINAL_BAN_DAYS,
			MAX_MESSAGEBUFFER,
			ACTIONS_DELAY_INTERVAL,
			EX_ACTIONS_DELAY_INTERVAL,
			CRITICAL_HIT_CHANCE,
			KICK_AFTER_MINUTES,
			PROTECTION_LEVEL,
			DEATH_LOSE_PERCENT,
			PASSWORD_TYPE,
			STATUSQUERY_TIMEOUT,
			FRAG_TIME,
			WHITE_SKULL_TIME,
			AUTO_SAVE_EACH_MINUTES,
			ADMIN_PORT,
			GAME_PORT,
			LOGIN_PORT,
			STATUS_PORT,
			STAIRHOP_DELAY,
			MARKET_OFFER_DURATION,
			CHECK_EXPIRED_MARKET_OFFERS_EACH_MINUTES,
			MAX_MARKET_OFFERS_AT_A_TIME_PER_PLAYER,
			EXP_FROM_PLAYERS_LEVEL_RANGE,
			MAX_PACKETS_PER_SECOND,
			LAST_INTEGER_CONFIG /* this must be the last one */
		};

		bool loadFile(const std::string& _filename);
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
