#include "../otpch.h"

#include "../configmanager.h"

#include "../luascript.h"
#include "luaregister.h"

#define registerEnumIn(lsi, tableName, value) \
	{ \
		std::string_view enumName = #value; \
		lsi.registerVariable(tableName, enumName.substr(enumName.find_last_of(':') + 1), value); \
	}

void tfs::lua::registerConfigManager(LuaScriptInterface& lsi)
{
	// configKeys
	lsi.registerTable("configKeys");

	registerEnumIn(lsi, "configKeys", ConfigManager::ALLOW_CHANGEOUTFIT);
	registerEnumIn(lsi, "configKeys", ConfigManager::ONE_PLAYER_ON_ACCOUNT);
	registerEnumIn(lsi, "configKeys", ConfigManager::AIMBOT_HOTKEY_ENABLED);
	registerEnumIn(lsi, "configKeys", ConfigManager::REMOVE_RUNE_CHARGES);
	registerEnumIn(lsi, "configKeys", ConfigManager::REMOVE_WEAPON_AMMO);
	registerEnumIn(lsi, "configKeys", ConfigManager::REMOVE_WEAPON_CHARGES);
	registerEnumIn(lsi, "configKeys", ConfigManager::REMOVE_POTION_CHARGES);
	registerEnumIn(lsi, "configKeys", ConfigManager::EXPERIENCE_FROM_PLAYERS);
	registerEnumIn(lsi, "configKeys", ConfigManager::FREE_PREMIUM);
	registerEnumIn(lsi, "configKeys", ConfigManager::REPLACE_KICK_ON_LOGIN);
	registerEnumIn(lsi, "configKeys", ConfigManager::ALLOW_CLONES);
	registerEnumIn(lsi, "configKeys", ConfigManager::BIND_ONLY_GLOBAL_ADDRESS);
	registerEnumIn(lsi, "configKeys", ConfigManager::OPTIMIZE_DATABASE);
	registerEnumIn(lsi, "configKeys", ConfigManager::MARKET_PREMIUM);
	registerEnumIn(lsi, "configKeys", ConfigManager::EMOTE_SPELLS);
	registerEnumIn(lsi, "configKeys", ConfigManager::STAMINA_SYSTEM);
	registerEnumIn(lsi, "configKeys", ConfigManager::WARN_UNSAFE_SCRIPTS);
	registerEnumIn(lsi, "configKeys", ConfigManager::CONVERT_UNSAFE_SCRIPTS);
	registerEnumIn(lsi, "configKeys", ConfigManager::CLASSIC_EQUIPMENT_SLOTS);
	registerEnumIn(lsi, "configKeys", ConfigManager::CLASSIC_ATTACK_SPEED);
	registerEnumIn(lsi, "configKeys", ConfigManager::SERVER_SAVE_NOTIFY_MESSAGE);
	registerEnumIn(lsi, "configKeys", ConfigManager::SERVER_SAVE_NOTIFY_DURATION);
	registerEnumIn(lsi, "configKeys", ConfigManager::SERVER_SAVE_CLEAN_MAP);
	registerEnumIn(lsi, "configKeys", ConfigManager::SERVER_SAVE_CLOSE);
	registerEnumIn(lsi, "configKeys", ConfigManager::SERVER_SAVE_SHUTDOWN);
	registerEnumIn(lsi, "configKeys", ConfigManager::ONLINE_OFFLINE_CHARLIST);
	registerEnumIn(lsi, "configKeys", ConfigManager::CHECK_DUPLICATE_STORAGE_KEYS);

	registerEnumIn(lsi, "configKeys", ConfigManager::MAP_NAME);
	registerEnumIn(lsi, "configKeys", ConfigManager::HOUSE_RENT_PERIOD);
	registerEnumIn(lsi, "configKeys", ConfigManager::SERVER_NAME);
	registerEnumIn(lsi, "configKeys", ConfigManager::OWNER_NAME);
	registerEnumIn(lsi, "configKeys", ConfigManager::OWNER_EMAIL);
	registerEnumIn(lsi, "configKeys", ConfigManager::URL);
	registerEnumIn(lsi, "configKeys", ConfigManager::LOCATION);
	registerEnumIn(lsi, "configKeys", ConfigManager::IP);
	registerEnumIn(lsi, "configKeys", ConfigManager::WORLD_TYPE);
	registerEnumIn(lsi, "configKeys", ConfigManager::MYSQL_HOST);
	registerEnumIn(lsi, "configKeys", ConfigManager::MYSQL_USER);
	registerEnumIn(lsi, "configKeys", ConfigManager::MYSQL_PASS);
	registerEnumIn(lsi, "configKeys", ConfigManager::MYSQL_DB);
	registerEnumIn(lsi, "configKeys", ConfigManager::MYSQL_SOCK);
	registerEnumIn(lsi, "configKeys", ConfigManager::DEFAULT_PRIORITY);
	registerEnumIn(lsi, "configKeys", ConfigManager::MAP_AUTHOR);

	registerEnumIn(lsi, "configKeys", ConfigManager::SQL_PORT);
	registerEnumIn(lsi, "configKeys", ConfigManager::MAX_PLAYERS);
	registerEnumIn(lsi, "configKeys", ConfigManager::PZ_LOCKED);
	registerEnumIn(lsi, "configKeys", ConfigManager::DEFAULT_DESPAWNRANGE);
	registerEnumIn(lsi, "configKeys", ConfigManager::DEFAULT_DESPAWNRADIUS);
	registerEnumIn(lsi, "configKeys", ConfigManager::DEFAULT_WALKTOSPAWNRADIUS);
	registerEnumIn(lsi, "configKeys", ConfigManager::REMOVE_ON_DESPAWN);
	registerEnumIn(lsi, "configKeys", ConfigManager::RATE_EXPERIENCE);
	registerEnumIn(lsi, "configKeys", ConfigManager::RATE_SKILL);
	registerEnumIn(lsi, "configKeys", ConfigManager::RATE_LOOT);
	registerEnumIn(lsi, "configKeys", ConfigManager::RATE_MAGIC);
	registerEnumIn(lsi, "configKeys", ConfigManager::RATE_SPAWN);
	registerEnumIn(lsi, "configKeys", ConfigManager::HOUSE_PRICE);
	registerEnumIn(lsi, "configKeys", ConfigManager::KILLS_TO_RED);
	registerEnumIn(lsi, "configKeys", ConfigManager::KILLS_TO_BLACK);
	registerEnumIn(lsi, "configKeys", ConfigManager::MAX_MESSAGEBUFFER);
	registerEnumIn(lsi, "configKeys", ConfigManager::ACTIONS_DELAY_INTERVAL);
	registerEnumIn(lsi, "configKeys", ConfigManager::EX_ACTIONS_DELAY_INTERVAL);
	registerEnumIn(lsi, "configKeys", ConfigManager::KICK_AFTER_MINUTES);
	registerEnumIn(lsi, "configKeys", ConfigManager::PROTECTION_LEVEL);
	registerEnumIn(lsi, "configKeys", ConfigManager::DEATH_LOSE_PERCENT);
	registerEnumIn(lsi, "configKeys", ConfigManager::STATUSQUERY_TIMEOUT);
	registerEnumIn(lsi, "configKeys", ConfigManager::FRAG_TIME);
	registerEnumIn(lsi, "configKeys", ConfigManager::WHITE_SKULL_TIME);
	registerEnumIn(lsi, "configKeys", ConfigManager::GAME_PORT);
	registerEnumIn(lsi, "configKeys", ConfigManager::LOGIN_PORT);
	registerEnumIn(lsi, "configKeys", ConfigManager::STATUS_PORT);
	registerEnumIn(lsi, "configKeys", ConfigManager::STAIRHOP_DELAY);
	registerEnumIn(lsi, "configKeys", ConfigManager::MARKET_OFFER_DURATION);
	registerEnumIn(lsi, "configKeys", ConfigManager::CHECK_EXPIRED_MARKET_OFFERS_EACH_MINUTES);
	registerEnumIn(lsi, "configKeys", ConfigManager::MAX_MARKET_OFFERS_AT_A_TIME_PER_PLAYER);
	registerEnumIn(lsi, "configKeys", ConfigManager::EXP_FROM_PLAYERS_LEVEL_RANGE);
	registerEnumIn(lsi, "configKeys", ConfigManager::MAX_PACKETS_PER_SECOND);
	registerEnumIn(lsi, "configKeys", ConfigManager::PLAYER_CONSOLE_LOGS);
	registerEnumIn(lsi, "configKeys", ConfigManager::TWO_FACTOR_AUTH);
	registerEnumIn(lsi, "configKeys", ConfigManager::MANASHIELD_BREAKABLE);
	registerEnumIn(lsi, "configKeys", ConfigManager::STAMINA_REGEN_MINUTE);
	registerEnumIn(lsi, "configKeys", ConfigManager::STAMINA_REGEN_PREMIUM);
	registerEnumIn(lsi, "configKeys", ConfigManager::HOUSE_DOOR_SHOW_PRICE);
	registerEnumIn(lsi, "configKeys", ConfigManager::MONSTER_OVERSPAWN);
}
