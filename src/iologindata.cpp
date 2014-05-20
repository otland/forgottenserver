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

#include "iologindata.h"
#include "item.h"
#include "configmanager.h"
#include "town.h"
#include "game.h"
#include "vocation.h"
#include "house.h"

extern ConfigManager g_config;
extern Game g_game;

Account IOLoginData::loadAccount(uint32_t accno)
{
	Account account;

	std::ostringstream query;
	query << "SELECT `id`, `name`, `password`, `type`, `premdays`, `lastday` FROM `accounts` WHERE `id` = " << accno;
	DBResult_ptr result = Database::getInstance()->storeQuery(query.str());
	if (!result) {
		return account;
	}

	account.id = result->getDataInt("id");
	account.name = result->getDataString("name");
	account.accountType = static_cast<AccountType_t>(result->getDataInt("type"));
	account.premiumDays = result->getDataInt("premdays");
	account.lastDay = result->getDataInt("lastday");
	return account;
}

bool IOLoginData::saveAccount(const Account& acc)
{
	std::ostringstream query;
	query << "UPDATE `accounts` SET `premdays` = " << acc.premiumDays << ", `lastday` = " << acc.lastDay << " WHERE `id` = " << acc.id;
	return Database::getInstance()->executeQuery(query.str());
}

bool IOLoginData::loginserverAuthentication(const std::string& name, const std::string& password, Account& account)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `id`, `name`, `password`, `type`, `premdays`, `lastday` FROM `accounts` WHERE `name` = " << db->escapeString(name);
	DBResult_ptr result = db->storeQuery(query.str());
	if (!result) {
		return false;
	}

	if (transformToSHA1(password) != result->getDataString("password")) {
		return false;
	}

	account.id = result->getDataInt("id");
	account.name = result->getDataString("name");
	account.accountType = static_cast<AccountType_t>(result->getDataInt("type"));
	account.premiumDays = result->getDataInt("premdays");
	account.lastDay = result->getDataInt("lastday");

	query.str("");
	query << "SELECT `name`, `deletion` FROM `players` WHERE `account_id` = " << account.id;
	result = db->storeQuery(query.str());
	if (result) {
		do {
			if (result->getDataInt("deletion") == 0) {
				account.charList.push_back(result->getDataString("name"));
			}
		} while (result->next());
		account.charList.sort();
	}
	return true;
}

uint32_t IOLoginData::gameworldAuthentication(const std::string& accountName, const std::string& password, std::string& characterName)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `id`, `password` FROM `accounts` WHERE `name` = " << db->escapeString(accountName);
	DBResult_ptr result = db->storeQuery(query.str());
	if (!result) {
		return 0;
	}

	if (transformToSHA1(password) != result->getDataString("password")) {
		return 0;
	}

	int32_t accountId = result->getDataInt("id");

	query.str("");
	query << "SELECT `account_id`, `name`, `deletion` FROM `players` WHERE `name` = " << db->escapeString(characterName);
	result = db->storeQuery(query.str());
	if (!result) {
		return 0;
	}

	if (result->getDataInt("account_id") != accountId || result->getDataInt("deletion") != 0) {
		return 0;
	}
	characterName = result->getDataString("name");
	return accountId;
}

AccountType_t IOLoginData::getAccountType(uint32_t accountId)
{
	std::ostringstream query;
	query << "SELECT `type` FROM `accounts` WHERE `id` = " << accountId;
	DBResult_ptr result = Database::getInstance()->storeQuery(query.str());
	if (!result) {
		return ACCOUNT_TYPE_NORMAL;
	}
	return static_cast<AccountType_t>(result->getDataInt("type"));
}

void IOLoginData::setAccountType(uint32_t accountId, AccountType_t accountType)
{
	std::ostringstream query;
	query << "UPDATE `accounts` SET `type` = " << accountType << " WHERE `id` = " << accountId;
	Database::getInstance()->executeQuery(query.str());
}

bool IOLoginData::updateOnlineStatus(uint32_t guid, bool login)
{
	std::ostringstream query;
	if (login) {
		query << "INSERT INTO `players_online` VALUES (" << guid << ')';
	} else {
		query << "DELETE FROM `players_online` WHERE `player_id` = " << guid;
	}
	return Database::getInstance()->executeQuery(query.str());
}

bool IOLoginData::preloadPlayer(Player* player, const std::string& name)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `id`, `account_id`, `group_id`, `deletion`, (SELECT `type` FROM `accounts` WHERE `accounts`.`id` = `account_id`) AS `account_type`";
	if (!g_config.getBoolean(ConfigManager::FREE_PREMIUM)) {
		query << ", (SELECT `premdays` FROM `accounts` WHERE `accounts`.`id` = `account_id`) AS `premium_days`";
	}
	query << " FROM `players` WHERE `name` = " << db->escapeString(name);
	DBResult_ptr result = db->storeQuery(query.str());
	if (!result) {
		return false;
	}

	if (result->getDataInt("deletion") != 0) {
		return false;
	}

	player->setGUID(result->getDataInt("id"));
	Group* group = g_game.getGroup(result->getDataInt("group_id"));
	if (!group) {
		std::cout << "[Error - IOLoginData::preloadPlayer] " << player->name << " has Group ID " << result->getDataInt("group_id") << " which doesn't exist." << std::endl;
		return false;
	}
	player->setGroup(group);
	player->accountNumber = result->getNumber<uint32_t>("account_id");
	player->accountType = static_cast<AccountType_t>(result->getDataInt("account_type"));
	if (!g_config.getBoolean(ConfigManager::FREE_PREMIUM)) {
		player->premiumDays = result->getDataInt("premium_days");
	} else {
		player->premiumDays = std::numeric_limits<uint16_t>::max();
	}
	return true;
}

bool IOLoginData::loadPlayerById(Player* player, uint32_t id)
{
	std::ostringstream query;
	query << "SELECT `id`, `name`, `account_id`, `group_id`, `sex`, `vocation`, `experience`, `level`, `maglevel`, `health`, `healthmax`, `blessings`, `mana`, `manamax`, `manaspent`, `soul`, `lookbody`, `lookfeet`, `lookhead`, `looklegs`, `looktype`, `lookaddons`, `posx`, `posy`, `posz`, `cap`, `lastlogin`, `lastlogout`, `lastip`, `conditions`, `skulltime`, `skull`, `town_id`, `balance`, `offlinetraining_time`, `offlinetraining_skill`, `stamina`, `skill_fist`, `skill_fist_tries`, `skill_club`, `skill_club_tries`, `skill_sword`, `skill_sword_tries`, `skill_axe`, `skill_axe_tries`, `skill_dist`, `skill_dist_tries`, `skill_shielding`, `skill_shielding_tries`, `skill_fishing`, `skill_fishing_tries` FROM `players` WHERE `id` = " << id;
	return loadPlayer(player, Database::getInstance()->storeQuery(query.str()));
}

bool IOLoginData::loadPlayerByName(Player* player, const std::string& name)
{
	Database* db = Database::getInstance();
	std::ostringstream query;
	query << "SELECT `id`, `name`, `account_id`, `group_id`, `sex`, `vocation`, `experience`, `level`, `maglevel`, `health`, `healthmax`, `blessings`, `mana`, `manamax`, `manaspent`, `soul`, `lookbody`, `lookfeet`, `lookhead`, `looklegs`, `looktype`, `lookaddons`, `posx`, `posy`, `posz`, `cap`, `lastlogin`, `lastlogout`, `lastip`, `conditions`, `skulltime`, `skull`, `town_id`, `balance`, `offlinetraining_time`, `offlinetraining_skill`, `stamina`, `skill_fist`, `skill_fist_tries`, `skill_club`, `skill_club_tries`, `skill_sword`, `skill_sword_tries`, `skill_axe`, `skill_axe_tries`, `skill_dist`, `skill_dist_tries`, `skill_shielding`, `skill_shielding_tries`, `skill_fishing`, `skill_fishing_tries` FROM `players` WHERE `name` = " << db->escapeString(name);
	return loadPlayer(player, db->storeQuery(query.str()));
}

bool IOLoginData::loadPlayer(Player* player, DBResult_ptr result)
{
	if (!result) {
		return false;
	}

	Database* db = Database::getInstance();

	uint32_t accno = result->getDataInt("account_id");
	Account acc = loadAccount(accno);

	player->setGUID(result->getDataInt("id"));
	player->name = result->getDataString("name");
	player->accountNumber = accno;

	player->accountType = acc.accountType;

	if (g_config.getBoolean(ConfigManager::FREE_PREMIUM)) {
		player->premiumDays = std::numeric_limits<uint16_t>::max();
	} else {
		player->premiumDays = acc.premiumDays;
	}

	Group* group = g_game.getGroup(result->getDataInt("group_id"));
	if (!group) {
		std::cout << "[Error - IOLoginData::loadPlayer] " << player->name << " has Group ID " << result->getDataInt("group_id") << " which doesn't exist." << std::endl;
		return false;
	}
	player->setGroup(group);

	player->bankBalance = result->getNumber<uint64_t>("balance");

	player->setSex((PlayerSex_t)result->getDataInt("sex"));
	player->level = std::max<uint32_t>(1, result->getDataInt("level"));

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

	player->soul = result->getDataInt("soul");
	player->capacity = result->getDataInt("cap");
	player->blessings = result->getDataInt("blessings");

	unsigned long conditionsSize;
	const char* conditions = result->getDataStream("conditions", conditionsSize);
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

	if (!player->setVocation(result->getDataInt("vocation"))) {
		std::cout << "[Error - IOLoginData::loadPlayer] " << player->name << " has Vocation ID " << result->getDataInt("vocation") << " which doesn't exist." << std::endl;
		return false;
	}

	player->mana = result->getDataInt("mana");
	player->manaMax = result->getDataInt("manamax");
	player->magLevel = result->getDataInt("maglevel");

	uint64_t nextManaCount = player->vocation->getReqMana(player->magLevel + 1);
	uint64_t manaSpent = result->getNumber<uint64_t>("manaspent");
	if (manaSpent > nextManaCount) {
		manaSpent = 0;
	}

	player->manaSpent = manaSpent;
	player->magLevelPercent = Player::getPercentLevel(player->manaSpent, nextManaCount);

	player->health = result->getDataInt("health");
	player->healthMax = result->getDataInt("healthmax");

	player->defaultOutfit.lookType = result->getDataInt("looktype");
	player->defaultOutfit.lookHead = result->getDataInt("lookhead");
	player->defaultOutfit.lookBody = result->getDataInt("lookbody");
	player->defaultOutfit.lookLegs = result->getDataInt("looklegs");
	player->defaultOutfit.lookFeet = result->getDataInt("lookfeet");
	player->defaultOutfit.lookAddons = result->getDataInt("lookaddons");
	player->currentOutfit = player->defaultOutfit;

	if (g_game.getWorldType() != WORLD_TYPE_PVP_ENFORCED) {
		const int32_t skullSeconds = result->getDataInt("skulltime") - time(nullptr);
		if (skullSeconds > 0) {
			//ensure that we round up the number of ticks
			player->skullTicks = (skullSeconds + 2) * 1000;
			int32_t skull = result->getDataInt("skull");

			if (skull == SKULL_RED) {
				player->skull = SKULL_RED;
			} else if (skull == SKULL_BLACK) {
				player->skull = SKULL_BLACK;
			}
		}
	}

	player->loginPosition.x = result->getDataInt("posx");
	player->loginPosition.y = result->getDataInt("posy");
	player->loginPosition.z = result->getDataInt("posz");

	player->lastLoginSaved = result->getNumber<uint64_t>("lastlogin");
	player->lastLogout = result->getNumber<uint64_t>("lastlogout");

	player->offlineTrainingTime = result->getDataInt("offlinetraining_time") * 1000;
	player->offlineTrainingSkill = result->getDataInt("offlinetraining_skill");

	Town* town = Towns::getInstance().getTown(result->getDataInt("town_id"));
	if (!town) {
		std::cout << "[Error - IOLoginData::loadPlayer] " << player->name << " has Town ID " << result->getDataInt("town_id") << " which doesn't exist." << std::endl;
		return false;
	}

	player->town = town;

	const Position& loginPos = player->loginPosition;
	if (loginPos.x == 0 && loginPos.y == 0 && loginPos.z == 0) {
		player->loginPosition = player->getTemplePosition();
	}

	player->staminaMinutes = result->getDataInt("stamina");

	static const std::string skillNames[] = {"skill_fist", "skill_club", "skill_sword", "skill_axe", "skill_dist", "skill_shielding", "skill_fishing"};
	static const std::string skillNameTries[] = {"skill_fist_tries", "skill_club_tries", "skill_sword_tries", "skill_axe_tries", "skill_dist_tries", "skill_shielding_tries", "skill_fishing_tries"};
	static const size_t size = sizeof(skillNames) / sizeof(std::string);
	for (size_t i = 0; i < size; ++i) {
		uint32_t skillLevel = result->getDataInt(skillNames[i]);
		uint64_t skillTries = result->getNumber<uint64_t>(skillNameTries[i]);
		uint64_t nextSkillTries = player->vocation->getReqSkillTries(i, skillLevel + 1);
		if (skillTries > nextSkillTries) {
			skillTries = 0;
		}
		player->skills[i][SKILLVALUE_LEVEL] = skillLevel;
		player->skills[i][SKILLVALUE_TRIES] = skillTries;
		player->skills[i][SKILLVALUE_PERCENT] = Player::getPercentLevel(skillTries, nextSkillTries);
	}

	std::ostringstream query;
	query << "SELECT `guild_id`, `rank_id`, `nick` FROM `guild_membership` WHERE `player_id` = " << player->getGUID();
	if ((result = db->storeQuery(query.str()))) {
		uint32_t guildId = result->getDataInt("guild_id");
		uint32_t playerRankId = result->getDataInt("rank_id");
		player->guildNick = result->getDataString("nick");

		Guild* guild = g_game.getGuild(guildId);
		if (!guild) {
			query.str("");
			query << "SELECT `name` FROM `guilds` WHERE `id` = " << guildId;
			if ((result = db->storeQuery(query.str()))) {
				guild = new Guild(guildId, result->getDataString("name"));
				g_game.addGuild(guild);

				query.str("");
				query << "SELECT `id`, `name`, `level` FROM `guild_ranks` WHERE `guild_id` = " << guildId << " LIMIT 3";

				if ((result = db->storeQuery(query.str()))) {
					do {
						guild->addRank(result->getDataInt("id"), result->getDataString("name"), result->getDataInt("level"));
					} while (result->next());
				}
			}
		}

		if (guild) {
			player->guild = guild;
			GuildRank* rank = guild->getRankById(playerRankId);
			if (rank) {
				player->guildLevel = rank->level;
			} else {
				player->guildLevel = 1;
			}

			IOGuild::getWarList(guildId, player->guildWarList);

			query.str("");
			query << "SELECT COUNT(*) AS `members` FROM `guild_membership` WHERE `guild_id` = " << guildId;
			if ((result = db->storeQuery(query.str()))) {
				guild->setMemberCount(result->getDataInt("members"));
			}
		}
	}

	query.str("");
	query << "SELECT `player_id`, `name` FROM `player_spells` WHERE `player_id` = " << player->getGUID();
	if ((result = db->storeQuery(query.str()))) {
		do {
			player->learnedInstantSpellList.emplace_front(result->getDataString("name"));
		} while (result->next());
	}

	//load inventory items
	ItemMap itemMap;

	query.str("");
	query << "SELECT `pid`, `sid`, `itemtype`, `count`, `attributes` FROM `player_items` WHERE `player_id` = " << player->getGUID() << " ORDER BY `sid` DESC";
	if ((result = db->storeQuery(query.str()))) {
		loadItems(itemMap, result);

		for (ItemMap::reverse_iterator it = itemMap.rbegin(); it != itemMap.rend(); ++it) {
			const std::pair<Item*, int32_t>& pair = it->second;
			Item* item = pair.first;
			int32_t pid = pair.second;
			if (pid >= 1 && pid <= 10) {
				player->__internalAddThing(pid, item);
			} else {
				ItemMap::const_iterator it2 = itemMap.find(pid);
				if (it2 == itemMap.end()) {
					continue;
				}

				Container* container = it2->second.first->getContainer();
				if (container) {
					container->__internalAddThing(item);
				}
			}
		}
	}

	//load depot items
	itemMap.clear();

	query.str("");
	query << "SELECT `pid`, `sid`, `itemtype`, `count`, `attributes` FROM `player_depotitems` WHERE `player_id` = " << player->getGUID() << " ORDER BY `sid` DESC";
	if ((result = db->storeQuery(query.str()))) {
		loadItems(itemMap, result);

		for (ItemMap::reverse_iterator it = itemMap.rbegin(); it != itemMap.rend(); ++it) {
			const std::pair<Item*, int32_t>& pair = it->second;
			Item* item = pair.first;

			int32_t pid = pair.second;
			if (pid >= 0 && pid < 100) {
				DepotChest* depotChest = player->getDepotChest(pid, true);
				if (depotChest) {
					depotChest->__internalAddThing(item);
				}
			} else {
				ItemMap::const_iterator it2 = itemMap.find(pid);
				if (it2 == itemMap.end()) {
					continue;
				}

				Container* container = it2->second.first->getContainer();
				if (container) {
					container->__internalAddThing(item);
				}
			}
		}
	}

	//load inbox items
	itemMap.clear();

	query.str("");
	query << "SELECT `pid`, `sid`, `itemtype`, `count`, `attributes` FROM `player_inboxitems` WHERE `player_id` = " << player->getGUID() << " ORDER BY `sid` DESC";
	if ((result = db->storeQuery(query.str()))) {
		loadItems(itemMap, result);

		for (ItemMap::reverse_iterator it = itemMap.rbegin(); it != itemMap.rend(); ++it) {
			const std::pair<Item*, int32_t>& pair = it->second;
			Item* item = pair.first;
			int32_t pid = pair.second;

			if (pid >= 0 && pid < 100) {
				player->getInbox()->__internalAddThing(item);
			} else {
				ItemMap::const_iterator it2 = itemMap.find(pid);

				if (it2 == itemMap.end()) {
					continue;
				}

				Container* container = it2->second.first->getContainer();
				if (container) {
					container->__internalAddThing(item);
				}
			}
		}
	}

	//load storage map
	query.str("");
	query << "SELECT `key`, `value` FROM `player_storage` WHERE `player_id` = " << player->getGUID();
	if ((result = db->storeQuery(query.str()))) {
		do {
			player->addStorageValue(result->getDataInt("key"), result->getDataInt("value"), true);
		} while (result->next());
	}

	//load vip
	query.str("");
	query << "SELECT `player_id` FROM `account_viplist` WHERE `account_id` = " << player->getAccount();
	if ((result = db->storeQuery(query.str()))) {
		do {
			player->addVIPInternal(result->getDataInt("player_id"));
		} while (result->next());
	}

	player->updateBaseSpeed();
	player->updateInventoryWeight();
	player->updateItemsLight(true);
	return true;
}

bool IOLoginData::saveItems(const Player* player, const ItemBlockList& itemList, DBInsert& query_insert, PropWriteStream& propWriteStream)
{
	std::ostringstream ss;

	typedef std::pair<Container*, int32_t> containerBlock;
	std::queue<containerBlock> queue;

	int32_t parentId = 0;
	int32_t runningId = 100;

	Database* db = Database::getInstance();
	for (const auto& it : itemList) {
		int32_t pid = it.first;
		Item* item = it.second;
		++runningId;

		propWriteStream.clear();
		item->serializeAttr(propWriteStream);

		size_t attributesSize;
		const char* attributes = propWriteStream.getStream(attributesSize);

		ss << player->getGUID() << ',' << pid << ',' << runningId << ',' << item->getID() << ',' << (int32_t)item->getSubType() << ',' << db->escapeBlob(attributes, attributesSize);
		if (!query_insert.addRow(ss)) {
			return false;
		}

		if (Container* container = item->getContainer()) {
			queue.emplace(container, runningId);
		}
	}

	while (!queue.empty()) {
		const containerBlock& cb = queue.front();
		Container* container = cb.first;
		parentId = cb.second;
		queue.pop();

		for (Item* item : container->getItemList()) {
			++runningId;

			Container* subContainer = item->getContainer();
			if (subContainer) {
				queue.emplace(subContainer, runningId);
			}

			propWriteStream.clear();
			item->serializeAttr(propWriteStream);

			size_t attributesSize;
			const char* attributes = propWriteStream.getStream(attributesSize);

			ss << player->getGUID() << ',' << parentId << ',' << runningId << ',' << item->getID() << ',' << (int32_t)item->getSubType() << ',' << db->escapeBlob(attributes, attributesSize);
			if (!query_insert.addRow(ss)) {
				return false;
			}
		}
	}
	return query_insert.execute();
}

bool IOLoginData::savePlayer(Player* player)
{
	if (player->getHealth() <= 0) {
		player->changeHealth(1);
	}

	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `save` FROM `players` WHERE `id` = " << player->getGUID();
	DBResult_ptr result = db->storeQuery(query.str());
	if (!result) {
		return false;
	}

	if (result->getDataInt("save") == 0) {
		query.str("");
		query << "UPDATE `players` SET `lastlogin` = " << player->lastLoginSaved << ", `lastip` = " << player->lastIP << " WHERE `id` = " << player->getGUID();
		return db->executeQuery(query.str());
	}

	//serialize conditions
	PropWriteStream propWriteStream;
	for (Condition* condition : player->conditions) {
		if (condition->isPersistent()) {
			if (!condition->serialize(propWriteStream)) {
				return false;
			}

			propWriteStream.ADD_UCHAR(CONDITIONATTR_END);
		}
	}

	size_t conditionsSize;
	const char* conditions = propWriteStream.getStream(conditionsSize);

	//First, an UPDATE query to write the player itself
	query.str("");
	query << "UPDATE `players` SET ";
	query << "`level` = " << player->level << ',';
	query << "`group_id` = " << player->group->id << ',';
	query << "`vocation` = " << player->getVocationId() << ',';
	query << "`health` = " << player->health << ',';
	query << "`healthmax` = " << player->healthMax << ',';
	query << "`experience` = " << player->experience << ',';
	query << "`lookbody` = " << (int32_t)player->defaultOutfit.lookBody << ',';
	query << "`lookfeet` = " << (int32_t)player->defaultOutfit.lookFeet << ',';
	query << "`lookhead` = " << (int32_t)player->defaultOutfit.lookHead << ',';
	query << "`looklegs` = " << (int32_t)player->defaultOutfit.lookLegs << ',';
	query << "`looktype` = " << player->defaultOutfit.lookType << ',';
	query << "`lookaddons` = " << (int32_t)player->defaultOutfit.lookAddons << ',';
	query << "`maglevel` = " << player->magLevel << ',';
	query << "`mana` = " << player->mana << ',';
	query << "`manamax` = " << player->manaMax << ',';
	query << "`manaspent` = " << player->manaSpent << ',';
	query << "`soul` = " << player->soul << ',';
	query << "`town_id` = " << player->town->getID() << ',';

	const Position& loginPosition = player->getLoginPosition();
	query << "`posx` = " << loginPosition.getX() << ',';
	query << "`posy` = " << loginPosition.getY() << ',';
	query << "`posz` = " << loginPosition.getZ() << ',';

	query << "`cap` = " << player->getCapacity() << ',';
	query << "`sex` = " << player->sex << ',';

	if (player->lastLoginSaved != 0) {
		query << "`lastlogin` = " << player->lastLoginSaved << ',';
	}

	if (player->lastIP != 0) {
		query << "`lastip` = " << player->lastIP << ',';
	}

	query << "`conditions` = " << db->escapeBlob(conditions, conditionsSize) << ',';

	if (g_game.getWorldType() != WORLD_TYPE_PVP_ENFORCED) {
		int32_t skullTime = 0;

		if (player->skullTicks > 0) {
			skullTime = time(nullptr) + player->skullTicks / 1000;
		}

		query << "`skulltime` = " << skullTime << ',';
		int32_t skull = 0;

		if (player->skull == SKULL_RED) {
			skull = SKULL_RED;
		} else if (player->skull == SKULL_BLACK) {
			skull = SKULL_BLACK;
		}

		query << "`skull` = " << skull << ',';
	}

	query << "`lastlogout` = " << player->getLastLogout() << ',';
	query << "`balance` = " << player->bankBalance << ',';
	query << "`offlinetraining_time` = " << player->getOfflineTrainingTime() / 1000 << ',';
	query << "`offlinetraining_skill` = " << player->getOfflineTrainingSkill() << ',';
	query << "`stamina` = " << player->getStaminaMinutes() << ',';

	query << "`skill_fist` = " << player->skills[SKILL_FIST][SKILLVALUE_LEVEL] << ',';
	query << "`skill_fist_tries` = " << player->skills[SKILL_FIST][SKILLVALUE_TRIES] << ',';
	query << "`skill_club` = " << player->skills[SKILL_CLUB][SKILLVALUE_LEVEL] << ',';
	query << "`skill_club_tries` = " << player->skills[SKILL_CLUB][SKILLVALUE_TRIES] << ',';
	query << "`skill_sword` = " << player->skills[SKILL_SWORD][SKILLVALUE_LEVEL] << ',';
	query << "`skill_sword_tries` = " << player->skills[SKILL_SWORD][SKILLVALUE_TRIES] << ',';
	query << "`skill_axe` = " << player->skills[SKILL_AXE][SKILLVALUE_LEVEL] << ',';
	query << "`skill_axe_tries` = " << player->skills[SKILL_AXE][SKILLVALUE_TRIES] << ',';
	query << "`skill_dist` = " << player->skills[SKILL_DISTANCE][SKILLVALUE_LEVEL] << ',';
	query << "`skill_dist_tries` = " << player->skills[SKILL_DISTANCE][SKILLVALUE_TRIES] << ',';
	query << "`skill_shielding` = " << player->skills[SKILL_SHIELD][SKILLVALUE_LEVEL] << ',';
	query << "`skill_shielding_tries` = " << player->skills[SKILL_SHIELD][SKILLVALUE_TRIES] << ',';
	query << "`skill_fishing` = " << player->skills[SKILL_FISHING][SKILLVALUE_LEVEL] << ',';
	query << "`skill_fishing_tries` = " << player->skills[SKILL_FISHING][SKILLVALUE_TRIES] << ',';

	if (!player->isOffline()) {
		query << "`onlinetime` = `onlinetime` + " << (time(nullptr) - player->lastLoginSaved) << ',';
	}
	query << "`blessings` = " << static_cast<uint16_t>(player->blessings);
	query << " WHERE `id` = " << player->getGUID();

	DBTransaction transaction;
	if (!transaction.begin()) {
		return false;
	}

	if (!db->executeQuery(query.str())) {
		return false;
	}

	// learned spells
	query.str("");
	query << "DELETE FROM `player_spells` WHERE `player_id` = " << player->getGUID();
	if (!db->executeQuery(query.str())) {
		return false;
	}

	query.str("");

	DBInsert stmt;
	stmt.setQuery("INSERT INTO `player_spells` (`player_id`, `name` ) VALUES ");

	for (const std::string& spellName : player->learnedInstantSpellList) {
		query << player->getGUID() << ',' << db->escapeString(spellName);
		if (!stmt.addRow(query)) {
			return false;
		}
	}

	if (!stmt.execute()) {
		return false;
	}

	//item saving
	query << "DELETE FROM `player_items` WHERE `player_id` = " << player->getGUID();
	if (!db->executeQuery(query.str())) {
		return false;
	}

	stmt.setQuery("INSERT INTO `player_items` (`player_id`, `pid`, `sid`, `itemtype`, `count`, `attributes`) VALUES ");

	ItemBlockList itemList;
	for (int32_t slotId = 1; slotId <= 10; ++slotId) {
		Item* item = player->inventory[slotId];
		if (item) {
			itemList.emplace_back(slotId, item);
		}
	}

	if (!saveItems(player, itemList, stmt, propWriteStream)) {
		return false;
	}

	if (player->lastDepotId != -1) {
		//save depot items
		query.str("");
		query << "DELETE FROM `player_depotitems` WHERE `player_id` = " << player->getGUID();

		if (!db->executeQuery(query.str())) {
			return false;
		}

		stmt.setQuery("INSERT INTO `player_depotitems` (`player_id`, `pid`, `sid`, `itemtype`, `count`, `attributes`) VALUES ");
		itemList.clear();

		for (const auto& it : player->depotChests) {
			DepotChest* depotChest = it.second;
			for (Item* item : depotChest->getItemList()) {
				itemList.emplace_back(it.first, item);
			}
		}

		if (!saveItems(player, itemList, stmt, propWriteStream)) {
			return false;
		}
	}

	//save inbox items
	query.str("");
	query << "DELETE FROM `player_inboxitems` WHERE `player_id` = " << player->getGUID();
	if (!db->executeQuery(query.str())) {
		return false;
	}

	stmt.setQuery("INSERT INTO `player_inboxitems` (`player_id`, `pid`, `sid`, `itemtype`, `count`, `attributes`) VALUES ");
	itemList.clear();

	for (Item* item : player->getInbox()->getItemList()) {
		itemList.emplace_back(0, item);
	}

	if (!saveItems(player, itemList, stmt, propWriteStream)) {
		return false;
	}

	query.str("");
	query << "DELETE FROM `player_storage` WHERE `player_id` = " << player->getGUID();
	if (!db->executeQuery(query.str())) {
		return false;
	}

	query.str("");

	stmt.setQuery("INSERT INTO `player_storage` (`player_id`, `key`, `value`) VALUES ");
	player->genReservedStorageRange();

	for (const auto& it : player->storageMap) {
		query << player->getGUID() << ',' << it.first << ',' << it.second;
		if (!stmt.addRow(query)) {
			return false;
		}
	}

	if (!stmt.execute()) {
		return false;
	}

	//End the transaction
	return transaction.commit();
}

bool IOLoginData::getNameByGuid(uint32_t guid, std::string& name)
{
	std::ostringstream query;
	query << "SELECT `name` FROM `players` WHERE `id` = " << guid;
	DBResult_ptr result = Database::getInstance()->storeQuery(query.str());
	if (!result) {
		return false;
	}

	name = result->getDataString("name");
	return true;
}

bool IOLoginData::getGuidByName(uint32_t& guid, std::string& name)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `id`, `name` FROM `players` WHERE `name` = " << db->escapeString(name);
	DBResult_ptr result = db->storeQuery(query.str());
	if (!result) {
		return false;
	}

	name = result->getDataString("name");
	guid = result->getDataInt("id");
	return true;
}

bool IOLoginData::getGuidByNameEx(uint32_t& guid, bool& specialVip, std::string& name)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `name`, `id`, `group_id`, `account_id` FROM `players` WHERE `name` = " << db->escapeString(name);
	DBResult_ptr result = db->storeQuery(query.str());
	if (!result) {
		return false;
	}

	name = result->getDataString("name");
	guid = result->getDataInt("id");
	Group* group = g_game.getGroup(result->getDataInt("group_id"));

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
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `name` FROM `players` WHERE `name` = " << db->escapeString(name);

	DBResult_ptr result = db->storeQuery(query.str());
	if (!result) {
		return false;
	}

	name = result->getDataString("name");
	return true;
}

void IOLoginData::loadItems(ItemMap& itemMap, DBResult_ptr result)
{
	do {
		int32_t sid = result->getDataInt("sid");
		int32_t pid = result->getDataInt("pid");
		int32_t type = result->getDataInt("itemtype");
		int32_t count = result->getDataInt("count");

		unsigned long attrSize;
		const char* attr = result->getDataStream("attributes", attrSize);

		PropStream propStream;
		propStream.init(attr, attrSize);

		Item* item = Item::CreateItem(type, count);
		if (item) {
			if (!item->unserializeAttr(propStream)) {
				std::cout << "WARNING: Serialize error in IOLoginData::loadItems" << std::endl;
			}

			std::pair<Item*, int32_t> pair(item, pid);
			itemMap[sid] = pair;
		}
	} while (result->next());
}

void IOLoginData::increaseBankBalance(uint32_t guid, uint64_t bankBalance)
{
	std::ostringstream query;
	query << "UPDATE `players` SET `balance` = `balance` + " << bankBalance << " WHERE `id` = " << guid;
	Database::getInstance()->executeQuery(query.str());
}

bool IOLoginData::hasBiddedOnHouse(uint32_t guid)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `id` FROM `houses` WHERE `highest_bidder` = " << guid << " LIMIT 1";
	return db->storeQuery(query.str()).get() != nullptr;
}

std::forward_list<VIPEntry> IOLoginData::getVIPEntries(uint32_t accountId)
{
	std::forward_list<VIPEntry> entries;

	std::ostringstream query;
	query << "SELECT `player_id`, (SELECT `name` FROM `players` WHERE `id` = `player_id`) AS `name`, `description`, `icon`, `notify` FROM `account_viplist` WHERE `account_id` = " << accountId;

	DBResult_ptr result = Database::getInstance()->storeQuery(query.str());
	if (result) {
		do {
			entries.emplace_front(
				result->getDataInt("player_id"),
				result->getDataString("name"),
				result->getDataString("description"),
				result->getDataInt("icon"),
				result->getDataInt("notify") != 0
			);
		} while (result->next());
	}
	return entries;
}

void IOLoginData::addVIPEntry(uint32_t accountId, uint32_t guid, const std::string& description, uint32_t icon, bool notify)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "INSERT INTO `account_viplist` (`account_id`, `player_id`, `description`, `icon`, `notify`) VALUES (" << accountId << ',' << guid << ',' << db->escapeString(description) << ',' << icon << ',' << notify << ')';
	db->executeQuery(query.str());
}

void IOLoginData::editVIPEntry(uint32_t accountId, uint32_t guid, const std::string& description, uint32_t icon, bool notify)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "UPDATE `account_viplist` SET `description` = " << db->escapeString(description) << ", `icon` = " << icon << ", `notify` = " << notify << " WHERE `account_id` = " << accountId << " AND `player_id` = " << guid;
	db->executeQuery(query.str());
}

void IOLoginData::removeVIPEntry(uint32_t accountId, uint32_t guid)
{
	std::ostringstream query;
	query << "DELETE FROM `account_viplist` WHERE `account_id` = " << accountId << " AND `player_id` = " << guid;
	Database::getInstance()->executeQuery(query.str());
}

void IOLoginData::addPremiumDays(uint32_t accountId, int32_t addDays)
{
	std::ostringstream query;
	query << "UPDATE `accounts` SET `premdays` = `premdays` + " << addDays << " WHERE `id` = " << accountId;
	Database::getInstance()->executeQuery(query.str());
}

void IOLoginData::removePremiumDays(uint32_t accountId, int32_t removeDays)
{
	std::ostringstream query;
	query << "UPDATE `accounts` SET `premdays` = `premdays` - " << removeDays << " WHERE `id` = " << accountId;
	Database::getInstance()->executeQuery(query.str());
}
