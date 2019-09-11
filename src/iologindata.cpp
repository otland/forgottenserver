/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019  Mark Samman <mark.samman@gmail.com>
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

#include "iologindata.h"
#include "configmanager.h"
#include "game.h"

extern ConfigManager g_config;
extern Game g_game;
PlayerCacheManager g_playerCacheManager;

Account IOLoginData::loadAccount(uint32_t accno)
{
	Account account;

	std::ostringstream query;
	query << "SELECT `id`, `name`, `password`, `type`, `premdays`, `lastday` FROM `accounts` WHERE `id` = " << accno;
	DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return account;
	}

	account.id = result->getNumber<uint32_t>("id");
	account.name = result->getString("name");
	account.accountType = static_cast<AccountType_t>(result->getNumber<int32_t>("type"));
	account.premiumDays = result->getNumber<uint16_t>("premdays");
	account.lastDay = result->getNumber<time_t>("lastday");
	return account;
}

bool IOLoginData::saveAccount(const Account& acc)
{
	std::ostringstream query;
	query << "UPDATE `accounts` SET `premdays` = " << acc.premiumDays << ", `lastday` = " << acc.lastDay << " WHERE `id` = " << acc.id;
	return Database::getInstance().executeQuery(query.str());
}

std::string decodeSecret(const std::string& secret)
{
	// simple base32 decoding
	std::string key;
	key.reserve(10);

	uint32_t buffer = 0, left = 0;
	for (const auto& ch : secret) {
		buffer <<= 5;
		if (ch >= 'A' && ch <= 'Z') {
			buffer |= (ch & 0x1F) - 1;
		} else if (ch >= '2' && ch <= '7') {
			buffer |= ch - 24;
		} else {
			// if a key is broken, return empty and the comparison
			// will always be false since the token must not be empty
			return {};
		}

		left += 5;
		if (left >= 8) {
			left -= 8;
			key.push_back(static_cast<char>(buffer >> left));
		}
	}

	return key;
}

bool IOLoginData::loginserverAuthentication(const std::string& name, const std::string& password, Account& account)
{
	Database& db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `id`, `name`, `password`, `secret`, `type`, `premdays`, `lastday` FROM `accounts` WHERE `name` = " << db.escapeString(name);
	DBResult_ptr result = db.storeQuery(query.str());
	if (!result) {
		return false;
	}

	if (transformToSHA1(password) != result->getString("password")) {
		return false;
	}

	account.id = result->getNumber<uint32_t>("id");
	account.name = result->getString("name");
	account.key = decodeSecret(result->getString("secret"));
	account.accountType = static_cast<AccountType_t>(result->getNumber<int32_t>("type"));
	account.premiumDays = result->getNumber<uint16_t>("premdays");
	account.lastDay = result->getNumber<time_t>("lastday");

	query.str(std::string());
	query << "SELECT `name`, `deletion` FROM `players` WHERE `account_id` = " << account.id;
	result = db.storeQuery(query.str());
	if (result) {
		do {
			if (result->getNumber<uint64_t>("deletion") == 0) {
				account.characters.push_back(result->getString("name"));
			}
		} while (result->next());
		std::sort(account.characters.begin(), account.characters.end());
	}
	return true;
}

uint32_t IOLoginData::gameworldAuthentication(const std::string& accountName, const std::string& password, std::string& characterName, std::string& token, uint32_t tokenTime)
{
	Database& db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `id`, `password`, `secret` FROM `accounts` WHERE `name` = " << db.escapeString(accountName);
	DBResult_ptr result = db.storeQuery(query.str());
	if (!result) {
		return 0;
	}

	std::string secret = decodeSecret(result->getString("secret"));
	if (!secret.empty()) {
		if (token.empty()) {
			return 0;
		}

		bool tokenValid = token == generateToken(secret, tokenTime) || token == generateToken(secret, tokenTime - 1) || token == generateToken(secret, tokenTime + 1);
		if (!tokenValid) {
			return 0;
		}
	}

	if (transformToSHA1(password) != result->getString("password")) {
		return 0;
	}

	uint32_t accountId = result->getNumber<uint32_t>("id");

	query.str(std::string());
	query << "SELECT `account_id`, `name`, `deletion` FROM `players` WHERE `name` = " << db.escapeString(characterName);
	result = db.storeQuery(query.str());
	if (!result) {
		return 0;
	}

	if (result->getNumber<uint32_t>("account_id") != accountId || result->getNumber<uint64_t>("deletion") != 0) {
		return 0;
	}
	characterName = result->getString("name");
	return accountId;
}

AccountType_t IOLoginData::getAccountType(uint32_t accountId)
{
	std::ostringstream query;
	query << "SELECT `type` FROM `accounts` WHERE `id` = " << accountId;
	DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return ACCOUNT_TYPE_NORMAL;
	}
	return static_cast<AccountType_t>(result->getNumber<uint16_t>("type"));
}

void IOLoginData::setAccountType(uint32_t accountId, AccountType_t accountType)
{
	std::ostringstream query;
	query << "UPDATE `accounts` SET `type` = " << static_cast<uint16_t>(accountType) << " WHERE `id` = " << accountId;
	Database::getInstance().executeQuery(query.str());
}

void IOLoginData::updateOnlineStatus(uint32_t guid, bool login)
{
	if (g_config.getBoolean(ConfigManager::ALLOW_CLONES)) {
		return;
	}

	std::ostringstream query;
	if (login) {
		query << "INSERT INTO `players_online` VALUES (" << guid << ')';
	} else {
		query << "DELETE FROM `players_online` WHERE `player_id` = " << guid;
	}
	Database::getInstance().executeQuery(query.str());
}

bool IOLoginData::preloadPlayer(Player* player, const std::string& name)
{
	Database& db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `id`, `account_id`, `group_id`, `deletion`, (SELECT `type` FROM `accounts` WHERE `accounts`.`id` = `account_id`) AS `account_type`";
	if (!g_config.getBoolean(ConfigManager::FREE_PREMIUM)) {
		query << ", (SELECT `premdays` FROM `accounts` WHERE `accounts`.`id` = `account_id`) AS `premium_days`";
	}
	query << " FROM `players` WHERE `name` = " << db.escapeString(name);
	DBResult_ptr result = db.storeQuery(query.str());
	if (!result) {
		return false;
	}

	if (result->getNumber<uint64_t>("deletion") != 0) {
		return false;
	}

	player->setGUID(result->getNumber<uint32_t>("id"));
	Group* group = g_game.groups.getGroup(result->getNumber<uint16_t>("group_id"));
	if (!group) {
		std::cout << "[Error - IOLoginData::preloadPlayer] " << player->name << " has Group ID " << result->getNumber<uint16_t>("group_id") << " which doesn't exist." << std::endl;
		return false;
	}
	player->setGroup(group);
	player->accountNumber = result->getNumber<uint32_t>("account_id");
	player->accountType = static_cast<AccountType_t>(result->getNumber<uint16_t>("account_type"));
	if (!g_config.getBoolean(ConfigManager::FREE_PREMIUM)) {
		player->premiumDays = result->getNumber<uint16_t>("premium_days");
	} else {
		player->premiumDays = std::numeric_limits<uint16_t>::max();
	}
	return true;
}

bool IOLoginData::loadPlayerById(Player* player, uint32_t id)
{
	Database& db = Database::getInstance();
	std::ostringstream query;
	query << "SELECT `id`, `name`, `account_id`, `group_id`, `sex`, `vocation`, `experience`, `level`, `maglevel`, `health`, `healthmax`, `blessings`, `mana`, `manamax`, `manaspent`, `soul`, `lookbody`, `lookfeet`, `lookhead`, `looklegs`, `looktype`, `lookaddons`, `posx`, `posy`, `posz`, `cap`, `lastlogin`, `lastlogout`, `lastip`, `conditions`, `skulltime`, `skull`, `town_id`, `balance`, `offlinetraining_time`, `offlinetraining_skill`, `stamina`, `skill_fist`, `skill_fist_tries`, `skill_club`, `skill_club_tries`, `skill_sword`, `skill_sword_tries`, `skill_axe`, `skill_axe_tries`, `skill_dist`, `skill_dist_tries`, `skill_shielding`, `skill_shielding_tries`, `skill_fishing`, `skill_fishing_tries`, `direction` FROM `players` WHERE `id` = " << id;
	return loadPlayer(player, db.storeQuery(query.str()));
}

bool IOLoginData::loadPlayerByName(Player* player, const std::string& name)
{
	Database& db = Database::getInstance();
	std::ostringstream query;
	query << "SELECT `id`, `name`, `account_id`, `group_id`, `sex`, `vocation`, `experience`, `level`, `maglevel`, `health`, `healthmax`, `blessings`, `mana`, `manamax`, `manaspent`, `soul`, `lookbody`, `lookfeet`, `lookhead`, `looklegs`, `looktype`, `lookaddons`, `posx`, `posy`, `posz`, `cap`, `lastlogin`, `lastlogout`, `lastip`, `conditions`, `skulltime`, `skull`, `town_id`, `balance`, `offlinetraining_time`, `offlinetraining_skill`, `stamina`, `skill_fist`, `skill_fist_tries`, `skill_club`, `skill_club_tries`, `skill_sword`, `skill_sword_tries`, `skill_axe`, `skill_axe_tries`, `skill_dist`, `skill_dist_tries`, `skill_shielding`, `skill_shielding_tries`, `skill_fishing`, `skill_fishing_tries`, `direction` FROM `players` WHERE `name` = " << db.escapeString(name);
	return loadPlayer(player, db.storeQuery(query.str()));
}

void IOLoginData::loadPlayerItems(Player* player, Database* db) {
	std::ostringstream query;
	DBResult_ptr result;

	//load inventory items
	ItemMap itemMap;

	query.str(std::string());
	if (g_config.getBoolean(ConfigManager::BINARY_PLAYER_ITEMS)) {
		query << "SELECT `items` FROM `player_binary_items` WHERE `player_id` = " << player->getGUID() << " AND `type` = " << BINARY_TYPE_ITEMS;
	}
	else {
		query << "SELECT `pid`, `sid`, `itemtype`, `count`, `attributes` FROM `player_items` WHERE `player_id` = " << player->getGUID() << " ORDER BY `sid` DESC";
	}
	if ((result = db->storeQuery(query.str()))) {
		loadItems(itemMap, result);

		for (ItemMap::const_reverse_iterator it = itemMap.rbegin(), end = itemMap.rend(); it != end; ++it) {
			const std::pair<Item *, int32_t> &pair = it->second;
			Item *item = pair.first;
			int32_t pid = pair.second;
			if (pid >= 1 && pid <= 10) {
				player->internalAddThing(pid, item);
			} else {
				ItemMap::const_iterator it2 = itemMap.find(pid);
				if (it2 == itemMap.end()) {
					continue;
				}

				Container *container = it2->second.first->getContainer();
				if (container) {
					container->internalAddThing(item);
				}
			}
		}
	}

	//load depot items
	itemMap.clear();

	query.str(std::string());
	if (g_config.getBoolean(ConfigManager::BINARY_PLAYER_ITEMS)) {
		query << "SELECT `items` FROM `player_binary_items` WHERE `player_id` = " << player->getGUID() << " AND `type` = " << BINARY_TYPE_DEPOTITEMS;
	}
	else {
		query << "SELECT `pid`, `sid`, `itemtype`, `count`, `attributes` FROM `player_depotitems` WHERE `player_id` = " << player->getGUID() << " ORDER BY `sid` DESC";
	}
	if ((result = db->storeQuery(query.str()))) {
		loadItems(itemMap, result);

		for (ItemMap::const_reverse_iterator it = itemMap.rbegin(), end = itemMap.rend(); it != end; ++it) {
			const std::pair<Item *, int32_t> &pair = it->second;
			Item *item = pair.first;

			int32_t pid = pair.second;
			if (pid >= 0 && pid < 100) {
				DepotChest *depotChest = player->getDepotChest(pid, true);
				if (depotChest) {
					depotChest->internalAddThing(item);
				}
			} else {
				ItemMap::const_iterator it2 = itemMap.find(pid);
				if (it2 == itemMap.end()) {
					continue;
				}

				Container *container = it2->second.first->getContainer();
				if (container) {
					container->internalAddThing(item);
				}
			}
		}
	}

	//load inbox items
	itemMap.clear();

	query.str(std::string());
	if (g_config.getBoolean(ConfigManager::BINARY_PLAYER_ITEMS)) {
		query << "SELECT `items` FROM `player_binary_items` WHERE `player_id` = " << player->getGUID() << " AND `type` = " << BINARY_TYPE_INBOX;
	}
	else {
		query << "SELECT `pid`, `sid`, `itemtype`, `count`, `attributes` FROM `player_inboxitems` WHERE `player_id` = " << player->getGUID() << " ORDER BY `sid` DESC";
	}
	if ((result = db->storeQuery(query.str()))) {
		loadItems(itemMap, result);

		for (ItemMap::const_reverse_iterator it = itemMap.rbegin(), end = itemMap.rend(); it != end; ++it) {
			const std::pair<Item *, int32_t> &pair = it->second;
			Item *item = pair.first;
			int32_t pid = pair.second;

			if (pid >= 0 && pid < 100) {
				player->getInbox()->internalAddThing(item);
			} else {
				ItemMap::const_iterator it2 = itemMap.find(pid);

				if (it2 == itemMap.end()) {
					continue;
				}

				Container *container = it2->second.first->getContainer();
				if (container) {
					container->internalAddThing(item);
				}
			}
		}
	}
}
bool IOLoginData::loadPlayer(Player* player, DBResult_ptr result)
{
	if (!result) {
		return false;
	}

	Database& db = Database::getInstance();

	uint32_t accno = result->getNumber<uint32_t>("account_id");
	Account acc = loadAccount(accno);

	player->setGUID(result->getNumber<uint32_t>("id"));
	player->name = result->getString("name");
	player->accountNumber = accno;

	player->accountType = acc.accountType;

	if (g_config.getBoolean(ConfigManager::FREE_PREMIUM)) {
		player->premiumDays = std::numeric_limits<uint16_t>::max();
	} else {
		player->premiumDays = acc.premiumDays;
	}

	Group* group = g_game.groups.getGroup(result->getNumber<uint16_t>("group_id"));
	if (!group) {
		std::cout << "[Error - IOLoginData::loadPlayer] " << player->name << " has Group ID " << result->getNumber<uint16_t>("group_id") << " which doesn't exist" << std::endl;
		return false;
	}
	player->setGroup(group);

	player->bankBalance = result->getNumber<uint64_t>("balance");

	player->setSex(static_cast<PlayerSex_t>(result->getNumber<uint16_t>("sex")));
	player->level = std::max<uint32_t>(1, result->getNumber<uint32_t>("level"));

	uint64_t experience = result->getNumber<uint64_t>("experience");

	uint64_t currExpCount = Player::getExpForLevel(player->level);
	uint64_t nextExpCount = Player::getExpForLevel(player->level + 1);
	if (experience < currExpCount || experience > nextExpCount) {
		experience = currExpCount;
	}

	player->experience = experience;

	if (currExpCount < nextExpCount) {
		player->levelPercent = Player::getPercentLevel(player->experience - currExpCount, nextExpCount - currExpCount);
	} else {
		player->levelPercent = 0;
	}

	player->soul = result->getNumber<uint16_t>("soul");
	player->capacity = result->getNumber<uint32_t>("cap") * 100;
	player->blessings = result->getNumber<uint16_t>("blessings");

	unsigned long conditionsSize;
	const char* conditions = result->getStream("conditions", conditionsSize);
	PropStream propStream;
	propStream.init(conditions, conditionsSize);

	Condition* condition = Condition::createCondition(propStream);
	while (condition) {
		if (condition->unserialize(propStream)) {
			player->storedConditionList.push_front(condition);
		} else {
			delete condition;
		}
		condition = Condition::createCondition(propStream);
	}

	if (!player->setVocation(result->getNumber<uint16_t>("vocation"))) {
		std::cout << "[Error - IOLoginData::loadPlayer] " << player->name << " has Vocation ID " << result->getNumber<uint16_t>("vocation") << " which doesn't exist" << std::endl;
		return false;
	}

	player->mana = result->getNumber<uint32_t>("mana");
	player->manaMax = result->getNumber<uint32_t>("manamax");
	player->magLevel = result->getNumber<uint32_t>("maglevel");

	uint64_t nextManaCount = player->vocation->getReqMana(player->magLevel + 1);
	uint64_t manaSpent = result->getNumber<uint64_t>("manaspent");
	if (manaSpent > nextManaCount) {
		manaSpent = 0;
	}

	player->manaSpent = manaSpent;
	player->magLevelPercent = Player::getPercentLevel(player->manaSpent, nextManaCount);

	player->health = result->getNumber<int32_t>("health");
	player->healthMax = result->getNumber<int32_t>("healthmax");

	player->defaultOutfit.lookType = result->getNumber<uint16_t>("looktype");
	player->defaultOutfit.lookHead = result->getNumber<uint16_t>("lookhead");
	player->defaultOutfit.lookBody = result->getNumber<uint16_t>("lookbody");
	player->defaultOutfit.lookLegs = result->getNumber<uint16_t>("looklegs");
	player->defaultOutfit.lookFeet = result->getNumber<uint16_t>("lookfeet");
	player->defaultOutfit.lookAddons = result->getNumber<uint16_t>("lookaddons");
	player->currentOutfit = player->defaultOutfit;
	player->direction = static_cast<Direction> (result->getNumber<uint16_t>("direction"));

	if (g_game.getWorldType() != WORLD_TYPE_PVP_ENFORCED) {
		const time_t skullSeconds = result->getNumber<time_t>("skulltime") - time(nullptr);
		if (skullSeconds > 0) {
			//ensure that we round up the number of ticks
			player->skullTicks = (skullSeconds + 2);

			uint16_t skull = result->getNumber<uint16_t>("skull");
			if (skull == SKULL_RED) {
				player->skull = SKULL_RED;
			} else if (skull == SKULL_BLACK) {
				player->skull = SKULL_BLACK;
			}
		}
	}

	player->loginPosition.x = result->getNumber<uint16_t>("posx");
	player->loginPosition.y = result->getNumber<uint16_t>("posy");
	player->loginPosition.z = result->getNumber<uint16_t>("posz");

	player->lastLoginSaved = result->getNumber<time_t>("lastlogin");
	player->lastLogout = result->getNumber<time_t>("lastlogout");

	player->offlineTrainingTime = result->getNumber<int32_t>("offlinetraining_time") * 1000;
	player->offlineTrainingSkill = result->getNumber<int32_t>("offlinetraining_skill");

	Town* town = g_game.map.towns.getTown(result->getNumber<uint32_t>("town_id"));
	if (!town) {
		std::cout << "[Error - IOLoginData::loadPlayer] " << player->name << " has Town ID " << result->getNumber<uint32_t>("town_id") << " which doesn't exist" << std::endl;
		return false;
	}

	player->town = town;

	const Position& loginPos = player->loginPosition;
	if (loginPos.x == 0 && loginPos.y == 0 && loginPos.z == 0) {
		player->loginPosition = player->getTemplePosition();
	}

	player->staminaMinutes = result->getNumber<uint16_t>("stamina");

	static const std::string skillNames[] = {"skill_fist", "skill_club", "skill_sword", "skill_axe", "skill_dist", "skill_shielding", "skill_fishing"};
	static const std::string skillNameTries[] = {"skill_fist_tries", "skill_club_tries", "skill_sword_tries", "skill_axe_tries", "skill_dist_tries", "skill_shielding_tries", "skill_fishing_tries"};
	static constexpr size_t size = sizeof(skillNames) / sizeof(std::string);
	for (uint8_t i = 0; i < size; ++i) {
		uint16_t skillLevel = result->getNumber<uint16_t>(skillNames[i]);
		uint64_t skillTries = result->getNumber<uint64_t>(skillNameTries[i]);
		uint64_t nextSkillTries = player->vocation->getReqSkillTries(i, skillLevel + 1);
		if (skillTries > nextSkillTries) {
			skillTries = 0;
		}

		player->skills[i].level = skillLevel;
		player->skills[i].tries = skillTries;
		player->skills[i].percent = Player::getPercentLevel(skillTries, nextSkillTries);
	}

	std::ostringstream query;
	query << "SELECT `guild_id`, `rank_id`, `nick` FROM `guild_membership` WHERE `player_id` = " << player->getGUID();
	if ((result = db.storeQuery(query.str()))) {
		uint32_t guildId = result->getNumber<uint32_t>("guild_id");
		uint32_t playerRankId = result->getNumber<uint32_t>("rank_id");
		player->guildNick = result->getString("nick");

		Guild* guild = g_game.getGuild(guildId);
		if (!guild) {
			guild = IOGuild::loadGuild(guildId);
			g_game.addGuild(guild);
		}

		if (guild) {
			player->guild = guild;
			const GuildRank* rank = guild->getRankById(playerRankId);
			if (!rank) {
				query.str(std::string());
				query << "SELECT `id`, `name`, `level` FROM `guild_ranks` WHERE `id` = " << playerRankId;

				if ((result = db.storeQuery(query.str()))) {
					guild->addRank(result->getNumber<uint32_t>("id"), result->getString("name"), result->getNumber<uint16_t>("level"));
				}

				rank = guild->getRankById(playerRankId);
				if (!rank) {
					player->guild = nullptr;
				}
			}

			player->guildRank = rank;

			IOGuild::getWarList(guildId, player->guildWarVector);

			query.str(std::string());
			query << "SELECT COUNT(*) AS `members` FROM `guild_membership` WHERE `guild_id` = " << guildId;
			if ((result = db.storeQuery(query.str()))) {
				guild->setMemberCount(result->getNumber<uint32_t>("members"));
			}
		}
	}

	query.str(std::string());
	query << "SELECT `player_id`, `name` FROM `player_spells` WHERE `player_id` = " << player->getGUID();
	if ((result = db.storeQuery(query.str()))) {
		do {
			player->learnedInstantSpellList.emplace_front(result->getString("name"));
		} while (result->next());
	}

	if (!g_config.getBoolean(ConfigManager::PLAYER_ITEMS_CACHE) ||
		!g_playerCacheManager.loadCachedPlayer(player->getGUID(), player)) {
		loadPlayerItems(player, &db);
	}

	//load storage map
	query.str(std::string());
	query << "SELECT `key`, `value` FROM `player_storage` WHERE `player_id` = " << player->getGUID();
	if ((result = db.storeQuery(query.str()))) {
		do {
			player->addStorageValue(result->getNumber<uint32_t>("key"), result->getNumber<int32_t>("value"), true);
		} while (result->next());
	}

	//load vip
	query.str(std::string());
	query << "SELECT `player_id` FROM `account_viplist` WHERE `account_id` = " << player->getAccount();
	if ((result = db.storeQuery(query.str()))) {
		do {
			player->addVIPInternal(result->getNumber<uint32_t>("player_id"));
		} while (result->next());
	}

	player->updateBaseSpeed();
	player->updateInventoryWeight();
	player->updateItemsLight(true);
	return true;
}

bool IOLoginData::saveItems(uint32_t guid, const ItemBlockList& itemList, DBInsert& query_insert, PropWriteStream& propWriteStream, BinaryPlayerDataType type /* = BINARY_TYPE_NONE */)
{
	if (g_config.getBoolean(ConfigManager::BINARY_PLAYER_ITEMS)) {
		std::list<Container*> listContainer;
		std::ostringstream ss;

		using ContainerBlock = std::pair<Container*, int32_t>;
		std::list<ContainerBlock> queue;

		int32_t runningId = 100;

		PropWriteStream binData;
		for (const auto& it : itemList) {
			int32_t pid = it.first;
			Item* item = it.second;
			++runningId;

			size_t attributesSize;

			propWriteStream.clear();
			item->serializeAttr(propWriteStream);
			const char* attributes = propWriteStream.getStream(attributesSize);

			binData.write<uint32_t>(pid);
			binData.write<uint32_t>(runningId);
			binData.write<uint16_t>(item->getID());
			binData.write<uint16_t>(item->getSubType());
			binData.writeString(std::string(attributes, attributesSize));

			if (Container * container = item->getContainer()) {
				queue.emplace_back(container, runningId);
			}
		}

		while (!queue.empty()) {
			const ContainerBlock& cb = queue.front();
			Container* container = cb.first;
			int32_t parentId = cb.second;
			queue.pop_front();

			for (Item* item : container->getItemList()) {
				++runningId;
				if (Container * sub = item->getContainer()) {
					queue.emplace_back(sub, runningId);
				}

				size_t attributesSize;

				propWriteStream.clear();
				item->serializeAttr(propWriteStream);
				const char* attributes = propWriteStream.getStream(attributesSize);

				binData.write<uint32_t>(parentId);
				binData.write<uint32_t>(runningId);
				binData.write<uint16_t>(item->getID());
				binData.write<uint16_t>(item->getSubType());
				binData.writeString(std::string(attributes, attributesSize));
			}
		}

		size_t itemsSize;
		const char* items = binData.getStream(itemsSize);
		ss << guid << ",  " << type << ", " << Database::getInstance().escapeBlob(items, itemsSize);

		if (!query_insert.addRow(ss)) {
			return false;
		}
	}
	else {
		std::ostringstream ss;

		using ContainerBlock = std::pair<Container*, int32_t>;
		std::list<ContainerBlock> queue;

		int32_t runningId = 100;

		Database& db = Database::getInstance();
		for (const auto& it : itemList) {
			int32_t pid = it.first;
			Item* item = it.second;
			++runningId;

			propWriteStream.clear();
			item->serializeAttr(propWriteStream);

			size_t attributesSize;
			const char* attributes = propWriteStream.getStream(attributesSize);

			ss << guid << ',' << pid << ',' << runningId << ',' << item->getID() << ',' << item->getSubType() << ',' << db.escapeBlob(attributes, attributesSize);
			if (!query_insert.addRow(ss)) {
				return false;
			}

			if (Container * container = item->getContainer()) {
				queue.emplace_back(container, runningId);
			}
		}

		while (!queue.empty()) {
			const ContainerBlock& cb = queue.front();
			Container* container = cb.first;
			int32_t parentId = cb.second;
			queue.pop_front();

			for (Item* item : container->getItemList()) {
				++runningId;

				Container* subContainer = item->getContainer();
				if (subContainer) {
					queue.emplace_back(subContainer, runningId);
				}

				propWriteStream.clear();
				item->serializeAttr(propWriteStream);

				size_t attributesSize;
				const char* attributes = propWriteStream.getStream(attributesSize);

				ss << guid << ',' << parentId << ',' << runningId << ',' << item->getID() << ',' << item->getSubType() << ',' << db.escapeBlob(attributes, attributesSize);
				if (!query_insert.addRow(ss)) {
					return false;
				}
			}
		}
	}

	return query_insert.execute();
}

bool IOLoginData::savePlayerItems(uint32_t guid, Item** inventory, std::map<uint32_t, DepotChest*>& depotChests,
								  Inbox* inbox, int16_t lastDepotId, Database* db /* = nullptr */) {
	if (!db) {
		db = &Database::getInstance();
	}

	if (g_config.getBoolean(ConfigManager::BINARY_PLAYER_ITEMS)) {
		std::ostringstream query;
		PropWriteStream propWriteStream;
		
		query << "DELETE FROM `player_binary_items` WHERE `player_id` = " << guid << " AND `type` = " << BINARY_TYPE_ITEMS;
		if (!db->executeQuery(query.str())) {
			return false;
		}

		DBInsert itemsQuery("INSERT INTO `player_binary_items` (`player_id`, `type`, `items`) VALUES ", db);

		ItemBlockList itemList;
		for (int32_t slotId = 1; slotId <= 10; ++slotId) {
			Item* item = inventory[slotId];
			if (item) {
				itemList.emplace_back(slotId, item);
			}
		}

		if (!saveItems(guid, itemList, itemsQuery, propWriteStream, BINARY_TYPE_ITEMS)) {
			return false;
		}

		if (lastDepotId != -1) {
			//save depot items
			query.str(std::string());
			query << "DELETE FROM `player_binary_items` WHERE `player_id` = " << guid << " AND `type` = " << BINARY_TYPE_DEPOTITEMS;

			if (!db->executeQuery(query.str())) {
				return false;
			}

			DBInsert depotQuery("INSERT INTO `player_binary_items` (`player_id`, `type`, `items`) VALUES ", db);
			itemList.clear();

			for (const auto& it : depotChests) {
				DepotChest* depotChest = it.second;
				for (Item* item : depotChest->getItemList()) {
					itemList.emplace_back(it.first, item);
				}
			}

			if (!saveItems(guid, itemList, depotQuery, propWriteStream, BINARY_TYPE_DEPOTITEMS)) {
				return false;
			}
		}

		//save inbox items
		query.str(std::string());
		query << "DELETE FROM `player_binary_items` WHERE `player_id` = " << guid << " AND `type` = " << BINARY_TYPE_INBOX;
		if (!db->executeQuery(query.str())) {
			return false;
		}

		DBInsert inboxQuery("INSERT INTO `player_binary_items` (`player_id`, `type`, `items`) VALUES ", db);
		itemList.clear();

		for (Item* item : inbox->getItemList()) {
			itemList.emplace_back(0, item);
		}

		if (!saveItems(guid, itemList, inboxQuery, propWriteStream, BINARY_TYPE_INBOX)) {
			return false;
		}
	}
	else {
		std::ostringstream query;
		PropWriteStream propWriteStream;

		query << "DELETE FROM `player_items` WHERE `player_id` = " << guid;
		if (!db->executeQuery(query.str())) {
			return false;
		}

		DBInsert itemsQuery("INSERT INTO `player_items` (`player_id`, `pid`, `sid`, `itemtype`, `count`, `attributes`) VALUES ", db);

		ItemBlockList itemList;
		for (int32_t slotId = 1; slotId <= 10; ++slotId) {
			Item* item = inventory[slotId];
			if (item) {
				itemList.emplace_back(slotId, item);
			}
		}

		if (!saveItems(guid, itemList, itemsQuery, propWriteStream)) {
			return false;
		}

		if (lastDepotId != -1) {
			//save depot items
			query.str(std::string());
			query << "DELETE FROM `player_depotitems` WHERE `player_id` = " << guid;

			if (!db->executeQuery(query.str())) {
				return false;
			}

			DBInsert depotQuery("INSERT INTO `player_depotitems` (`player_id`, `pid`, `sid`, `itemtype`, `count`, `attributes`) VALUES ", db);
			itemList.clear();

			for (const auto& it : depotChests) {
				DepotChest* depotChest = it.second;
				for (Item* item : depotChest->getItemList()) {
					itemList.emplace_back(it.first, item);
				}
			}

			if (!saveItems(guid, itemList, depotQuery, propWriteStream)) {
				return false;
			}
		}

		//save inbox items
		query.str(std::string());
		query << "DELETE FROM `player_inboxitems` WHERE `player_id` = " << guid;
		if (!db->executeQuery(query.str())) {
			return false;
		}

		DBInsert inboxQuery("INSERT INTO `player_inboxitems` (`player_id`, `pid`, `sid`, `itemtype`, `count`, `attributes`) VALUES ", db);
		itemList.clear();

		for (Item* item : inbox->getItemList()) {
			itemList.emplace_back(0, item);
		}

		if (!saveItems(guid, itemList, inboxQuery, propWriteStream)) {
			return false;
		}
	}

	return true;
}

bool IOLoginData::savePlayer(Player* player)
{
	if (player->getHealth() <= 0) {
		player->changeHealth(1);
	}

	Database& db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `save` FROM `players` WHERE `id` = " << player->getGUID();
	DBResult_ptr result = db.storeQuery(query.str());
	if (!result) {
		return false;
	}

	if (result->getNumber<uint16_t>("save") == 0) {
		query.str(std::string());
		query << "UPDATE `players` SET `lastlogin` = " << player->lastLoginSaved << ", `lastip` = " << player->lastIP << " WHERE `id` = " << player->getGUID();
		return db.executeQuery(query.str());
	}

	//serialize conditions
	PropWriteStream propWriteStream;
	for (Condition* condition : player->conditions) {
		if (condition->isPersistent()) {
			condition->serialize(propWriteStream);
			propWriteStream.write<uint8_t>(CONDITIONATTR_END);
		}
	}

	size_t conditionsSize;
	const char* conditions = propWriteStream.getStream(conditionsSize);

	//First, an UPDATE query to write the player itself
	query.str(std::string());
	query << "UPDATE `players` SET ";
	query << "`level` = " << player->level << ',';
	query << "`group_id` = " << player->group->id << ',';
	query << "`vocation` = " << player->getVocationId() << ',';
	query << "`health` = " << player->health << ',';
	query << "`healthmax` = " << player->healthMax << ',';
	query << "`experience` = " << player->experience << ',';
	query << "`lookbody` = " << static_cast<uint32_t>(player->defaultOutfit.lookBody) << ',';
	query << "`lookfeet` = " << static_cast<uint32_t>(player->defaultOutfit.lookFeet) << ',';
	query << "`lookhead` = " << static_cast<uint32_t>(player->defaultOutfit.lookHead) << ',';
	query << "`looklegs` = " << static_cast<uint32_t>(player->defaultOutfit.lookLegs) << ',';
	query << "`looktype` = " << player->defaultOutfit.lookType << ',';
	query << "`lookaddons` = " << static_cast<uint32_t>(player->defaultOutfit.lookAddons) << ',';
	query << "`maglevel` = " << player->magLevel << ',';
	query << "`mana` = " << player->mana << ',';
	query << "`manamax` = " << player->manaMax << ',';
	query << "`manaspent` = " << player->manaSpent << ',';
	query << "`soul` = " << static_cast<uint16_t>(player->soul) << ',';
	query << "`town_id` = " << player->town->getID() << ',';

	const Position& loginPosition = player->getLoginPosition();
	query << "`posx` = " << loginPosition.getX() << ',';
	query << "`posy` = " << loginPosition.getY() << ',';
	query << "`posz` = " << loginPosition.getZ() << ',';

	query << "`cap` = " << (player->capacity / 100) << ',';
	query << "`sex` = " << static_cast<uint16_t>(player->sex) << ',';

	if (player->lastLoginSaved != 0) {
		query << "`lastlogin` = " << player->lastLoginSaved << ',';
	}

	if (player->lastIP != 0) {
		query << "`lastip` = " << player->lastIP << ',';
	}

	query << "`conditions` = " << db.escapeBlob(conditions, conditionsSize) << ',';

	if (g_game.getWorldType() != WORLD_TYPE_PVP_ENFORCED) {
		int64_t skullTime = 0;

		if (player->skullTicks > 0) {
			skullTime = time(nullptr) + player->skullTicks;
		}
		query << "`skulltime` = " << skullTime << ',';

		Skulls_t skull = SKULL_NONE;
		if (player->skull == SKULL_RED) {
			skull = SKULL_RED;
		} else if (player->skull == SKULL_BLACK) {
			skull = SKULL_BLACK;
		}
		query << "`skull` = " << static_cast<int64_t>(skull) << ',';
	}

	query << "`lastlogout` = " << player->getLastLogout() << ',';
	query << "`balance` = " << player->bankBalance << ',';
	query << "`offlinetraining_time` = " << player->getOfflineTrainingTime() / 1000 << ',';
	query << "`offlinetraining_skill` = " << player->getOfflineTrainingSkill() << ',';
	query << "`stamina` = " << player->getStaminaMinutes() << ',';

	query << "`skill_fist` = " << player->skills[SKILL_FIST].level << ',';
	query << "`skill_fist_tries` = " << player->skills[SKILL_FIST].tries << ',';
	query << "`skill_club` = " << player->skills[SKILL_CLUB].level << ',';
	query << "`skill_club_tries` = " << player->skills[SKILL_CLUB].tries << ',';
	query << "`skill_sword` = " << player->skills[SKILL_SWORD].level << ',';
	query << "`skill_sword_tries` = " << player->skills[SKILL_SWORD].tries << ',';
	query << "`skill_axe` = " << player->skills[SKILL_AXE].level << ',';
	query << "`skill_axe_tries` = " << player->skills[SKILL_AXE].tries << ',';
	query << "`skill_dist` = " << player->skills[SKILL_DISTANCE].level << ',';
	query << "`skill_dist_tries` = " << player->skills[SKILL_DISTANCE].tries << ',';
	query << "`skill_shielding` = " << player->skills[SKILL_SHIELD].level << ',';
	query << "`skill_shielding_tries` = " << player->skills[SKILL_SHIELD].tries << ',';
	query << "`skill_fishing` = " << player->skills[SKILL_FISHING].level << ',';
	query << "`skill_fishing_tries` = " << player->skills[SKILL_FISHING].tries << ',';
	query << "`direction` = " << static_cast<uint16_t> (player->getDirection()) << ',';

	if (!player->isOffline()) {
		query << "`onlinetime` = `onlinetime` + " << (time(nullptr) - player->lastLoginSaved) << ',';
	}
	query << "`blessings` = " << static_cast<uint32_t>(player->blessings);
	query << " WHERE `id` = " << player->getGUID();

	DBTransaction transaction;
	if (!transaction.begin()) {
		return false;
	}

	if (!db.executeQuery(query.str())) {
		return false;
	}

	// learned spells
	query.str(std::string());
	query << "DELETE FROM `player_spells` WHERE `player_id` = " << player->getGUID();
	if (!db.executeQuery(query.str())) {
		return false;
	}

	query.str(std::string());

	DBInsert spellsQuery("INSERT INTO `player_spells` (`player_id`, `name` ) VALUES ");
	for (const std::string& spellName : player->learnedInstantSpellList) {
		query << player->getGUID() << ',' << db.escapeString(spellName);
		if (!spellsQuery.addRow(query)) {
			return false;
		}
	}

	if (!spellsQuery.execute()) {
		return false;
	}

	if (g_config.getBoolean(ConfigManager::PLAYER_ITEMS_CACHE)) {
		g_playerCacheManager.cachePlayer(player->getGUID(), player);
	}
	else {
		savePlayerItems(player->getGUID(),
						player->inventory,
						player->depotChests,
						player->inbox,
						player->lastDepotId,
						&db);
	}

	query.str(std::string());
	query << "DELETE FROM `player_storage` WHERE `player_id` = " << player->getGUID();
	if (!db.executeQuery(query.str())) {
		return false;
	}

	query.str(std::string());

	DBInsert storageQuery("INSERT INTO `player_storage` (`player_id`, `key`, `value`) VALUES ");
	player->genReservedStorageRange();

	for (const auto& it : player->storageMap) {
		query << player->getGUID() << ',' << it.first << ',' << it.second;
		if (!storageQuery.addRow(query)) {
			return false;
		}
	}

	if (!storageQuery.execute()) {
		return false;
	}

	//End the transaction
	return transaction.commit();
}

std::string IOLoginData::getNameByGuid(uint32_t guid)
{
	std::ostringstream query;
	query << "SELECT `name` FROM `players` WHERE `id` = " << guid;
	DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (!result) {
		return std::string();
	}
	return result->getString("name");
}

uint32_t IOLoginData::getGuidByName(const std::string& name)
{
	Database& db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `id` FROM `players` WHERE `name` = " << db.escapeString(name);
	DBResult_ptr result = db.storeQuery(query.str());
	if (!result) {
		return 0;
	}
	return result->getNumber<uint32_t>("id");
}

bool IOLoginData::getGuidByNameEx(uint32_t& guid, bool& specialVip, std::string& name)
{
	Database& db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `name`, `id`, `group_id`, `account_id` FROM `players` WHERE `name` = " << db.escapeString(name);
	DBResult_ptr result = db.storeQuery(query.str());
	if (!result) {
		return false;
	}

	name = result->getString("name");
	guid = result->getNumber<uint32_t>("id");
	Group* group = g_game.groups.getGroup(result->getNumber<uint16_t>("group_id"));

	uint64_t flags;
	if (group) {
		flags = group->flags;
	} else {
		flags = 0;
	}

	specialVip = (flags & PlayerFlag_SpecialVIP) != 0;
	return true;
}

bool IOLoginData::formatPlayerName(std::string& name)
{
	Database& db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `name` FROM `players` WHERE `name` = " << db.escapeString(name);

	DBResult_ptr result = db.storeQuery(query.str());
	if (!result) {
		return false;
	}

	name = result->getString("name");
	return true;
}

void IOLoginData::loadItems(ItemMap& itemMap, DBResult_ptr result)
{
	if (g_config.getBoolean(ConfigManager::BINARY_PLAYER_ITEMS)) {
		unsigned long itemsSize;
		const char* items = result->getStream("items", itemsSize);
		PropStream itemsStream;
		itemsStream.init(items, itemsSize);

		while (itemsStream.size() >= 14) {
			uint32_t pid, sid;
			uint16_t type = 0, count = 0;
			std::string attr;
			itemsStream.read<uint32_t>(pid);
			itemsStream.read<uint32_t>(sid);
			itemsStream.read<uint16_t>(type);
			itemsStream.read<uint16_t>(count);
			itemsStream.readString(attr);

			PropStream propStream;
			propStream.init(attr.c_str(), attr.size());

			Item* item = Item::CreateItem(type, count);
			if (item) {
				if (!item->unserializeAttr(propStream)) {
					std::cout << "WARNING: Serialize error in IOLoginData::loadItems" << std::endl;
				}
				
				std::pair<Item*, uint32_t> pair(item, pid);
				itemMap[sid] = pair;
			}
		}
	}
	else {
		do {
			uint32_t sid = result->getNumber<uint32_t>("sid");
			uint32_t pid = result->getNumber<uint32_t>("pid");
			uint16_t type = result->getNumber<uint16_t>("itemtype");
			uint16_t count = result->getNumber<uint16_t>("count");

			unsigned long attrSize;
			const char* attr = result->getStream("attributes", attrSize);

			PropStream propStream;
			propStream.init(attr, attrSize);

			Item* item = Item::CreateItem(type, count);
			if (item) {
				if (!item->unserializeAttr(propStream)) {
					std::cout << "WARNING: Serialize error in IOLoginData::loadItems" << std::endl;
				}

				std::pair<Item*, uint32_t> pair(item, pid);
				itemMap[sid] = pair;
			}
		} while (result->next());
	}
}

void IOLoginData::increaseBankBalance(uint32_t guid, uint64_t bankBalance)
{
	std::ostringstream query;
	query << "UPDATE `players` SET `balance` = `balance` + " << bankBalance << " WHERE `id` = " << guid;
	Database::getInstance().executeQuery(query.str());
}

bool IOLoginData::hasBiddedOnHouse(uint32_t guid)
{
	Database& db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `id` FROM `houses` WHERE `highest_bidder` = " << guid << " LIMIT 1";
	return db.storeQuery(query.str()).get() != nullptr;
}

std::forward_list<VIPEntry> IOLoginData::getVIPEntries(uint32_t accountId)
{
	std::forward_list<VIPEntry> entries;

	std::ostringstream query;
	query << "SELECT `player_id`, (SELECT `name` FROM `players` WHERE `id` = `player_id`) AS `name`, `description`, `icon`, `notify` FROM `account_viplist` WHERE `account_id` = " << accountId;

	DBResult_ptr result = Database::getInstance().storeQuery(query.str());
	if (result) {
		do {
			entries.emplace_front(
				result->getNumber<uint32_t>("player_id"),
				result->getString("name"),
				result->getString("description"),
				result->getNumber<uint32_t>("icon"),
				result->getNumber<uint16_t>("notify") != 0
			);
		} while (result->next());
	}
	return entries;
}

void IOLoginData::addVIPEntry(uint32_t accountId, uint32_t guid, const std::string& description, uint32_t icon, bool notify)
{
	Database& db = Database::getInstance();

	std::ostringstream query;
	query << "INSERT INTO `account_viplist` (`account_id`, `player_id`, `description`, `icon`, `notify`) VALUES (" << accountId << ',' << guid << ',' << db.escapeString(description) << ',' << icon << ',' << notify << ')';
	db.executeQuery(query.str());
}

void IOLoginData::editVIPEntry(uint32_t accountId, uint32_t guid, const std::string& description, uint32_t icon, bool notify)
{
	Database& db = Database::getInstance();

	std::ostringstream query;
	query << "UPDATE `account_viplist` SET `description` = " << db.escapeString(description) << ", `icon` = " << icon << ", `notify` = " << notify << " WHERE `account_id` = " << accountId << " AND `player_id` = " << guid;
	db.executeQuery(query.str());
}

void IOLoginData::removeVIPEntry(uint32_t accountId, uint32_t guid)
{
	std::ostringstream query;
	query << "DELETE FROM `account_viplist` WHERE `account_id` = " << accountId << " AND `player_id` = " << guid;
	Database::getInstance().executeQuery(query.str());
}

void IOLoginData::addPremiumDays(uint32_t accountId, int32_t addDays)
{
	std::ostringstream query;
	query << "UPDATE `accounts` SET `premdays` = `premdays` + " << addDays << " WHERE `id` = " << accountId;
	Database::getInstance().executeQuery(query.str());
}

void IOLoginData::removePremiumDays(uint32_t accountId, int32_t removeDays)
{
	std::ostringstream query;
	query << "UPDATE `accounts` SET `premdays` = `premdays` - " << removeDays << " WHERE `id` = " << accountId;
	Database::getInstance().executeQuery(query.str());
}

PlayerCacheData* PlayerCacheManager::getCachedPlayer(uint32_t guid, bool autoCreate /* = false */)
{
	PlayerCacheData* playerCacheData;

	listLock.lock();
	auto it = playersCache.find(guid);
	if (it == playersCache.end()) {
		if (autoCreate) {
			playerCacheData = new PlayerCacheData();
			playersCache.emplace(guid, playerCacheData);
		}
		else {
			listLock.unlock();
			return nullptr;
		}
	}
	else {
		playerCacheData = it->second;
	}

	listLock.unlock();
	return playerCacheData;
}

bool PlayerCacheManager::loadCachedPlayer(uint32_t guid, Player* player)
{
	PlayerCacheData* playerCacheData = getCachedPlayer(guid);

	if (!playerCacheData) {
		return false;
	}

	playerCacheData->copyDataToPlayer(player);

	return true;
}

void PlayerCacheManager::cachePlayer(uint32_t guid, Player* player)
{
	PlayerCacheData* playerCacheData = getCachedPlayer(guid, true);
	playerCacheData->copyDataFromPlayer(player);
	addToSaveList(guid);
}

void PlayerCacheManager::clear()
{
	listLock.lock();

	for (auto& it : playersCache) {
		delete it.second;
	}
	playersCache.clear();

	listLock.unlock();
}

void PlayerCacheManager::clear(uint32_t guid)
{
	listLock.lock();

	auto it = playersCache.find(guid);
	if (it != playersCache.end()) {
		delete it->second;
		playersCache.erase(it);
	}

	listLock.unlock();
}

void PlayerCacheManager::start()
{
	db.connect();
	ThreadHolder::start();
}

void PlayerCacheManager::threadMain()
{
	std::unique_lock<std::mutex> listLockUnique(listLock, std::defer_lock);

	while (getState() != THREAD_STATE_TERMINATED) {
		listLockUnique.lock();

		if (!toSave.empty()) {
			int64_t currentTime = OTSYS_TIME();
			uint32_t guidToSave = 0;
			for (auto it : toSave) {
				if (it.second + g_config.getNumber(ConfigManager::PLAYER_ITEMS_CACHE_SAVE_THRESHOLD) < currentTime) {
					guidToSave = it.first;
					toSave.erase(it.first);
					break;
				}
			}
			listLockUnique.unlock();

			if (guidToSave > 0 && !saveCachedItems(guidToSave)) {
				std::cout << "Error while saving player items: " << guidToSave << std::endl;
			}
		}
		else {
			listLockUnique.unlock();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void PlayerCacheManager::addToSaveList(uint32_t guid)
{
	listLock.lock();

	toSave[guid] = OTSYS_TIME();

	listLock.unlock();
}

bool PlayerCacheManager::saveCachedItems(uint32_t guid)
{
	PlayerCacheData* playerCacheData = getCachedPlayer(guid);

	if (!playerCacheData) {
		return false;
	}

	PlayerCacheData* playerCacheDataClone = playerCacheData->clone();
	bool saved = IOLoginData::savePlayerItems(guid, playerCacheDataClone->inventory, playerCacheDataClone->depotChests, playerCacheDataClone->inbox, playerCacheDataClone->lastDepotId, &db);
	delete playerCacheDataClone;

	return saved;
}

void PlayerCacheManager::flush()
{
	std::unique_lock<std::mutex> guard{ listLock };
	while (!toSave.empty()) {
		uint32_t guidToSave = 0;
		for (auto it : toSave) {
			guidToSave = it.first;
			toSave.erase(it.first);
			break;
		}
		guard.unlock();

		if (!saveCachedItems(guidToSave)) {
			std::cout << "Error while saving player items: " << guidToSave << std::endl;
		}

		guard.lock();
	}
}

void PlayerCacheManager::shutdown()
{
	listLock.lock();

	setState(THREAD_STATE_TERMINATED);

	listLock.unlock();
}

PlayerCacheData::~PlayerCacheData() {
	clear();
}

PlayerCacheData* PlayerCacheData::clone() {
	dataLock.lock();

	auto clone = new PlayerCacheData();

	for (uint8_t slotId = CONST_SLOT_FIRST; slotId <= CONST_SLOT_LAST; ++slotId) {
		Item* slotItem = inventory[slotId];

		if (slotItem) {
			clone->inventory[slotId] = slotItem->cloneWithoutDecay();
		}
	}

	for (const auto& it : depotChests) {
		auto depotId = it.first;
		DepotChest* depotChest = it.second;

		clone->depotChests[depotId] = (DepotChest*)depotChest->cloneWithoutDecay();
	}

	if (inbox) {
		clone->inbox = (Inbox*)inbox->cloneWithoutDecay();
	}

	clone->lastDepotId = lastDepotId;

	dataLock.unlock();

	return clone;
}

void PlayerCacheData::copyDataFromPlayer(Player* player) {
	clear();

	dataLock.lock();

	for (uint8_t slotId = CONST_SLOT_FIRST; slotId <= CONST_SLOT_LAST; ++slotId) {
		Item* slotItem = player->inventory[slotId];

		if (slotItem) {
			inventory[slotId] = slotItem->cloneWithoutDecay();
			inventory[slotId]->setParent(nullptr);
		}
	}

	for (const auto& it : player->depotChests) {
		auto depotId = it.first;
		DepotChest* depotChest = it.second;

		depotChests[depotId] = (DepotChest*)depotChest->cloneWithoutDecay();
		depotChests[depotId]->setParent(nullptr);
	}

	Inbox* playerInbox = player->inbox;

	if (playerInbox) {
		inbox = (Inbox*)playerInbox->cloneWithoutDecay();
		inbox->setParent(nullptr);
	}

	lastDepotId = player->lastDepotId;

	dataLock.unlock();
}

void PlayerCacheData::copyDataToPlayer(Player* player) {
	dataLock.lock();

	for (uint8_t slotId = CONST_SLOT_FIRST; slotId <= CONST_SLOT_LAST; ++slotId) {
		Item* slotItem = inventory[slotId];
		if (slotItem) {
			player->internalAddThing(slotId, slotItem->cloneWithoutDecay());
		}
	}

	for (const auto& it : depotChests) {
		auto depotId = it.first;
		DepotChest* depotChest = it.second;
		player->depotChests[depotId] = (DepotChest*)depotChest->cloneWithoutDecay();
	}

	if (inbox) {
		player->inbox = (Inbox*)inbox->cloneWithoutDecay();
	}

	dataLock.unlock();
}

void PlayerCacheData::clear() {
	dataLock.lock();

	for (uint8_t slotId = CONST_SLOT_FIRST; slotId <= CONST_SLOT_LAST; ++slotId) {
		if (inventory[slotId]) {
			delete inventory[slotId];
			inventory[slotId] = nullptr;
		}
	}

	for (auto& it : depotChests) {
		delete it.second;
	}
	depotChests.clear();

	if (inbox) {
		delete inbox;
		inbox = nullptr;
	}

	dataLock.unlock();
}
