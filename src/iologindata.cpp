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

#include "otpch.h"

#include "iologindata.h"
#include <algorithm>
#include <functional>
#include "item.h"
#include "configmanager.h"
#include "tools.h"
#include "town.h"
#include "definitions.h"
#include "game.h"
#include "vocation.h"
#include "house.h"
#include "ban.h"
#include <iostream>
#include <iomanip>

extern ConfigManager g_config;
extern Vocations g_vocations;
extern Game g_game;

Account IOLoginData::loadAccount(uint32_t accno)
{
	Account account;

	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `id`, `name`, `password`, `type`, `premdays`, `lastday` FROM `accounts` WHERE `id` = " << accno;

	DBResult* result = db->storeQuery(query.str());
	if (!result) {
		return account;
	}

	account.id = result->getDataInt("id");
	account.name = result->getDataString("name");
	account.accountType = (AccountType_t)result->getDataInt("type");
	account.premiumDays = result->getDataInt("premdays");
	account.lastDay = result->getDataInt("lastday");
	db->freeResult(result);
	return account;
}

bool IOLoginData::saveAccount(const Account& acc)
{
	std::ostringstream query;
	query << "UPDATE `accounts` SET `premdays` = " << acc.premiumDays << ", `lastday` = " << acc.lastDay << " WHERE `id` = " << acc.id;
	return Database::getInstance()->executeQuery(query.str());
}

bool IOLoginData::loginserverAuthenticate(const std::string& name, const std::string& password, Account& account)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `id`, `name`, `password`, `type`, `premdays`, `lastday` FROM `accounts` WHERE `name` = " << db->escapeString(name);

	DBResult* result = db->storeQuery(query.str());
	if (!result) {
		return false;
	}

	switch (g_config.getNumber(ConfigManager::PASSWORD_TYPE)) {
		case PASSWORD_TYPE_MD5: {
			if (transformToMD5(password) != result->getDataString("password")) {
				db->freeResult(result);
				return false;
			}
			break;
		}

		case PASSWORD_TYPE_SHA1: {
			if (transformToSHA1(password) != result->getDataString("password")) {
				db->freeResult(result);
				return false;
			}
			break;
		}

		default: {
			if (password != result->getDataString("password")) {
				db->freeResult(result);
				return false;
			}
			break;
		}
	}

	account.id = result->getDataInt("id");
	account.name = result->getDataString("name");
	account.accountType = (AccountType_t)result->getDataInt("type");
	account.premiumDays = result->getDataInt("premdays");
	account.lastDay = result->getDataInt("lastday");
	db->freeResult(result);

	query.str("");
	query << "SELECT `name`, `deletion` FROM `players` WHERE `account_id` = " << account.id;
	result = db->storeQuery(query.str());
	if (!result) {
		return false;
	}

	do {
		if (result->getDataInt("deletion") == 0) {
			account.charList.push_back(result->getDataString("name"));
		}
	} while (result->next());
	db->freeResult(result);
	account.charList.sort();
	return true;
}

uint32_t IOLoginData::gameworldAuthentication(const std::string& accountName, const std::string& password, std::string& characterName)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `id`, `password` FROM `accounts` WHERE `name` = " << db->escapeString(accountName);

	DBResult* result = db->storeQuery(query.str());
	if (!result) {
		return 0;
	}

	switch (g_config.getNumber(ConfigManager::PASSWORD_TYPE)) {
		case PASSWORD_TYPE_MD5: {
			if (transformToMD5(password) != result->getDataString("password")) {
				db->freeResult(result);
				return 0;
			}
			break;
		}

		case PASSWORD_TYPE_SHA1: {
			if (transformToSHA1(password) != result->getDataString("password")) {
				db->freeResult(result);
				return 0;
			}
			break;
		}

		default: {
			if (password != result->getDataString("password")) {
				db->freeResult(result);
				return 0;
			}
			break;
		}
	}

	uint32_t accountId = result->getDataInt("id");
	db->freeResult(result);

	query.str("");
	query << "SELECT `account_id`, `name`, `deletion` FROM `players` WHERE `name` = " << db->escapeString(characterName);
	result = db->storeQuery(query.str());
	if (!result) {
		return 0;
	}

	if ((uint32_t)result->getDataInt("account_id") != accountId || result->getDataInt("deletion") != 0) {
		db->freeResult(result);
		return 0;
	}
	characterName = result->getDataString("name");

	db->freeResult(result);
	return accountId;
}

AccountType_t IOLoginData::getAccountType(uint32_t accountId)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `type` FROM `accounts` WHERE `id` = " << accountId;

	DBResult* result = db->storeQuery(query.str());
	if (!result) {
		return ACCOUNT_TYPE_NORMAL;
	}

	AccountType_t accountType = (AccountType_t)result->getDataInt("type");
	db->freeResult(result);
	return accountType;
}

void IOLoginData::setAccountType(uint32_t accountId, AccountType_t accountType)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "UPDATE `accounts` SET `type` = " << accountType << " WHERE `id` = " << accountId;
	db->executeQuery(query.str());
}

uint32_t IOLoginData::getLastIPByName(const std::string& name)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `lastip` FROM `players` WHERE `name` = " << db->escapeString(name);

	DBResult* result = db->storeQuery(query.str());

	if (!result) {
		return 0;
	}

	uint32_t lastIp = result->getDataInt("lastip");
	db->freeResult(result);
	return lastIp;
}

bool IOLoginData::updateOnlineStatus(uint32_t guid, bool login)
{
	Database* db = Database::getInstance();

	std::ostringstream query;

	if (login) {
		query << "INSERT INTO `players_online` VALUES (" << guid << ")";
	} else {
		query << "DELETE FROM `players_online` WHERE `player_id` = " << guid;
	}

	return db->executeQuery(query.str());
}

bool IOLoginData::preloadPlayer(Player* player, const std::string& name)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `id`, `account_id`, `group_id`, `deletion`, (SELECT `type` FROM `accounts` WHERE `accounts`.`id` = `account_id`) AS `account_type`";
	if (!g_config.getBoolean(ConfigManager::FREE_PREMIUM)) {
		query << ", (SELECT `premdays` FROM `accounts` WHERE `accounts`.`id` = `account_id`) AS `premium_days`";
	} else {
		player->premiumDays = 0xFFFF;
	}
	query << " FROM `players` WHERE `name` = " << db->escapeString(name);

	DBResult* result = db->storeQuery(query.str());
	if (!result) {
		return false;
	}

	if (result->getDataInt("deletion") != 0) {
		db->freeResult(result);
		return false;
	}

	player->setGUID(result->getDataInt("id"));
	player->setGroupId(result->getDataInt("group_id"));
	player->accountNumber = (uint32_t)result->getDataInt("account_id");
	player->accountType = (AccountType_t)result->getDataInt("account_type");
	player->premiumDays = result->getDataInt("premium_days");
	db->freeResult(result);
	return true;
}

bool IOLoginData::loadPlayer(Player* player, const std::string& name)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `id`, `account_id`, `group_id`, `sex`, `vocation`, `experience`, `level`, `maglevel`, `health`, `healthmax`, `blessings`, `mana`, `manamax`, `manaspent`, `soul`, `lookbody`, `lookfeet`, `lookhead`, `looklegs`, `looktype`, `lookaddons`, `posx`, `posy`, `posz`, `cap`, `lastlogin`, `lastlogout`, `lastip`, `conditions`, `skulltime`, `skull`, `town_id`, `balance`, `offlinetraining_time`, `offlinetraining_skill`, `stamina` FROM `players` WHERE `name` = " << db->escapeString(name);

	DBResult* result = db->storeQuery(query.str());
	if (!result) {
		return false;
	}

	uint32_t accno = result->getDataInt("account_id");
	Account acc = loadAccount(accno);

	player->setGUID(result->getDataInt("id"));
	player->accountNumber = accno;

	player->accountType = acc.accountType;

	if (g_config.getBoolean(ConfigManager::FREE_PREMIUM)) {
		player->premiumDays = 0xFFFF;
	} else {
		player->premiumDays = acc.premiumDays;
	}

	player->setGroupId(result->getDataInt("group_id"));

	player->bankBalance = (uint64_t)result->getDataLong("balance");

	player->setSex((PlayerSex_t)result->getDataInt("sex"));
	player->level = std::max<uint32_t>(1, result->getDataInt("level"));

	uint64_t currExpCount = Player::getExpForLevel(player->level);
	uint64_t nextExpCount = Player::getExpForLevel(player->level + 1);
	uint64_t experience = (uint64_t)result->getDataLong("experience");

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

	unsigned long conditionsSize = 0;
	const char* conditions = result->getDataStream("conditions", conditionsSize);
	PropStream propStream;
	propStream.init(conditions, conditionsSize);

	Condition* condition = Condition::createCondition(propStream);
	while (condition) {
		if (condition->unserialize(propStream)) {
			player->storedConditionList.push_back(condition);
		} else {
			delete condition;
		}
		condition = Condition::createCondition(propStream);
	}

	player->setVocation(result->getDataInt("vocation"));
	player->mana = result->getDataInt("mana");
	player->manaMax = result->getDataInt("manamax");
	player->magLevel = result->getDataInt("maglevel");

	uint64_t nextManaCount = player->vocation->getReqMana(player->magLevel + 1);
	uint64_t manaSpent = result->getDataLong("manaspent");

	if (manaSpent > nextManaCount) {
		manaSpent = 0;
	}

	player->manaSpent = manaSpent;
	player->magLevelPercent = Player::getPercentLevel(player->manaSpent,
	                          nextManaCount);

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
		int32_t skullSeconds = result->getDataInt("skulltime") - time(NULL);

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

	player->lastLoginSaved = result->getDataLong("lastlogin");
	player->lastLogout = result->getDataLong("lastlogout");

	player->offlineTrainingTime = result->getDataInt("offlinetraining_time") * 1000;
	player->offlineTrainingSkill = result->getDataInt("offlinetraining_skill");

	player->town = result->getDataInt("town_id");

	Town* town = Towns::getInstance().getTown(player->town);

	if (town) {
		player->masterPos = town->getTemplePosition();
	}

	Position loginPos = player->loginPosition;

	if (loginPos.x == 0 && loginPos.y == 0 && loginPos.z == 0) {
		player->loginPosition = player->masterPos;
	}

	player->staminaMinutes = result->getDataInt("stamina");

	db->freeResult(result);

	query.str("");
	query << "SELECT `guild_id`, `rank_id`, `nick` FROM `guild_membership` WHERE `player_id` = " << player->getGUID();

	if ((result = db->storeQuery(query.str()))) {
		uint32_t guildId = result->getDataInt("guild_id");
		uint32_t playerRankId = result->getDataInt("rank_id");
		player->guildNick = result->getDataString("nick");
		db->freeResult(result);

		Guild* guild = g_game.getGuild(guildId);

		if (!guild) {
			query.str("");
			query << "SELECT `name` FROM `guilds` WHERE `id` = " << guildId;

			if ((result = db->storeQuery(query.str()))) {
				guild = new Guild(guildId, result->getDataString("name"));
				db->freeResult(result);
				g_game.addGuild(guild);

				query.str("");
				query << "SELECT `id`, `name`, `level` FROM `guild_ranks` WHERE `guild_id` = " << guildId << " LIMIT 3";

				if ((result = db->storeQuery(query.str()))) {
					do {
						guild->addRank(result->getDataInt("id"), result->getDataString("name"), result->getDataInt("level"));
					} while (result->next());

					db->freeResult(result);
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

			IOGuild::getInstance()->getWarList(guildId, player->guildWarList);

			query.str("");
			query << "SELECT COUNT(*) AS `members` FROM `guild_membership` WHERE `guild_id` = " << guildId;

			if ((result = db->storeQuery(query.str()))) {
				guild->setMemberCount(result->getDataInt("members"));
				db->freeResult(result);
			}
		}
	}

	//get password
	query.str("");
	query << "SELECT `password` FROM `accounts` WHERE `id` = " << accno;

	result = db->storeQuery(query.str());
	if (!result) {
		return false;
	}

	player->password = result->getDataString("password");
	db->freeResult(result);

	// we need to find out our skills
	// so we query the skill table
	query.str("");
	query << "SELECT `skillid`, `value`, `count` FROM `player_skills` WHERE `player_id` = " << player->getGUID();

	if ((result = db->storeQuery(query.str()))) {
		//now iterate over the skills
		do {
			int32_t skillid = result->getDataInt("skillid");

			if (skillid >= SKILL_FIRST && skillid <= SKILL_LAST) {
				uint32_t skillLevel = result->getDataInt("value");
				uint64_t skillCount = result->getDataLong("count");

				uint64_t nextSkillCount = player->vocation->getReqSkillTries(skillid, skillLevel + 1);

				if (skillCount > nextSkillCount) {
					skillCount = 0;
				}

				player->skills[skillid][SKILL_LEVEL] = skillLevel;
				player->skills[skillid][SKILL_TRIES] = skillCount;
				player->skills[skillid][SKILL_PERCENT] = Player::getPercentLevel(skillCount, nextSkillCount);
			}
		} while (result->next());

		db->freeResult(result);
	}

	query.str("");
	query << "SELECT `player_id`, `name` FROM `player_spells` WHERE `player_id` = " << player->getGUID();

	if ((result = db->storeQuery(query.str()))) {
		do {
			std::string spellName = result->getDataString("name");
			player->learnedInstantSpellList.push_back(spellName);
		} while (result->next());

		db->freeResult(result);
	}

	//load inventory items
	ItemMap itemMap;

	query.str("");
	query << "SELECT `pid`, `sid`, `itemtype`, `count`, `attributes` FROM `player_items` WHERE `player_id` = " << player->getGUID() << " ORDER BY `sid` DESC";

	if ((result = db->storeQuery(query.str()))) {
		loadItems(itemMap, result);
		db->freeResult(result);

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
		db->freeResult(result);

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
		db->freeResult(result);

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
			player->addStorageValue(result->getDataInt("key"), result->getDataLong("value"), true);
		} while (result->next());

		db->freeResult(result);
	}

	//load vip
	query.str("");
	query << "SELECT `player_id` FROM `account_viplist` WHERE `account_id` = " << player->getAccount();

	if ((result = db->storeQuery(query.str()))) {
		do {
			player->addVIPInternal(result->getDataInt("player_id"));
		} while (result->next());

		db->freeResult(result);
	}

	player->updateBaseSpeed();
	player->updateInventoryWeight();
	player->updateItemsLight(true);
	return true;
}

bool IOLoginData::saveItems(const Player* player, const ItemBlockList& itemList, DBInsert& query_insert)
{
	std::ostringstream stream;

	typedef std::pair<Container*, int32_t> containerBlock;
	std::list<containerBlock> stack;

	int32_t parentId = 0;
	int32_t runningId = 100;

	Database* db = Database::getInstance();
	Item* item;
	int32_t pid;

	for (ItemBlockList::const_iterator it = itemList.begin(); it != itemList.end(); ++it) {
		pid = it->first;
		item = it->second;
		++runningId;

		uint32_t attributesSize = 0;

		PropWriteStream propWriteStream;
		item->serializeAttr(propWriteStream);
		const char* attributes = propWriteStream.getStream(attributesSize);

		stream << player->getGUID() << "," << pid << "," << runningId << "," << item->getID() << "," << (int32_t)item->getSubType() << "," << db->escapeBlob(attributes, attributesSize);

		if (!query_insert.addRow(stream)) {
			return false;
		}

		if (Container* container = item->getContainer()) {
			stack.push_back(containerBlock(container, runningId));
		}
	}

	while (!stack.empty()) {
		const containerBlock& cb = stack.front();
		Container* container = cb.first;
		parentId = cb.second;
		stack.pop_front();

		for (ItemDeque::const_iterator it = container->getItems(), end = container->getEnd(); it != end; ++it) {
			++runningId;
			item = *it;
			Container* container = item->getContainer();

			if (container) {
				stack.push_back(containerBlock(container, runningId));
			}

			uint32_t attributesSize = 0;
			PropWriteStream propWriteStream;
			item->serializeAttr(propWriteStream);
			const char* attributes = propWriteStream.getStream(attributesSize);

			stream << player->getGUID() << "," << parentId << "," << runningId << "," << item->getID() << "," << (int32_t)item->getSubType() << "," << db->escapeBlob(attributes, attributesSize);

			if (!query_insert.addRow(stream)) {
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

	DBResult* result = db->storeQuery(query.str());

	if (!result) {
		return false;
	}

	if (result->getDataInt("save") == 0) {
		db->freeResult(result);
		query.str("");
		query << "UPDATE `players` SET `lastlogin` = " << player->lastLoginSaved << ", `lastip` = " << player->lastIP << " WHERE `id` = " << player->getGUID();
		return db->executeQuery(query.str());
	}

	db->freeResult(result);

	//serialize conditions
	PropWriteStream propWriteStream;

	for (ConditionList::const_iterator it = player->conditions.begin(); it != player->conditions.end(); ++it) {
		Condition* condition = *it;

		if (condition->isPersistent()) {
			if (!condition->serialize(propWriteStream)) {
				return false;
			}

			propWriteStream.ADD_UCHAR(CONDITIONATTR_END);
		}
	}

	uint32_t conditionsSize = 0;
	const char* conditions = propWriteStream.getStream(conditionsSize);

	//First, an UPDATE query to write the player itself
	query.str("");
	query << "UPDATE `players` SET ";
	query << "`level` = " << player->level << ", ";
	query << "`group_id` = " << player->groupId << ", ";
	query << "`vocation` = " << (int32_t)player->getVocationId() << ", ";
	query << "`health` = " << player->health << ", ";
	query << "`healthmax` = " << player->healthMax << ", ";
	query << "`experience` = " << player->experience << ", ";
	query << "`lookbody` = " << (int32_t)player->defaultOutfit.lookBody << ", ";
	query << "`lookfeet` = " << (int32_t)player->defaultOutfit.lookFeet << ", ";
	query << "`lookhead` = " << (int32_t)player->defaultOutfit.lookHead << ", ";
	query << "`looklegs` = " << (int32_t)player->defaultOutfit.lookLegs << ", ";
	query << "`looktype` = " << (int32_t)player->defaultOutfit.lookType << ", ";
	query << "`lookaddons` = " << (int32_t)player->defaultOutfit.lookAddons << ", ";
	query << "`maglevel` = " << player->magLevel << ", ";
	query << "`mana` = " << player->mana << ", ";
	query << "`manamax` = " << player->manaMax << ", ";
	query << "`manaspent` = " << player->manaSpent << ", ";
	query << "`soul` = " << player->soul << ", ";
	query << "`town_id` = " << player->town << ", ";

	const Position& loginPosition = player->getLoginPosition();
	query << "`posx` = " << loginPosition.x << ", ";
	query << "`posy` = " << loginPosition.y << ", ";
	query << "`posz` = " << loginPosition.z << ", ";

	query << "`cap` = " << player->getCapacity() << ", ";
	query << "`sex` = " << player->sex << ", ";

	if (player->lastLoginSaved != 0) {
		query << "`lastlogin` = " << player->lastLoginSaved << ", ";
	}

	if (player->lastIP != 0) {
		query << "`lastip` = " << player->lastIP << ", ";
	}

	query << "`conditions` = " << db->escapeBlob(conditions, conditionsSize) << ", ";

	if (g_game.getWorldType() != WORLD_TYPE_PVP_ENFORCED) {
		int32_t skullTime = 0;

		if (player->skullTicks > 0) {
			skullTime = time(NULL) + player->skullTicks / 1000;
		}

		query << "`skulltime` = " << skullTime << ", ";
		int32_t skull = 0;

		if (player->skull == SKULL_RED) {
			skull = SKULL_RED;
		} else if (player->skull == SKULL_BLACK) {
			skull = SKULL_BLACK;
		}

		query << "`skull` = " << skull << ", ";
	}

	query << "`lastlogout` = " << player->getLastLogout() << ", ";
	query << "`balance` = " << player->bankBalance << ", ";
	query << "`offlinetraining_time` = " << player->getOfflineTrainingTime() / 1000 << ", ";
	query << "`offlinetraining_skill` = " << player->getOfflineTrainingSkill() << ", ";
	query << "`stamina` = " << player->getStaminaMinutes() << ", ";
	if (!player->isOffline()) {
		query << "`onlinetime` = `onlinetime` + " << (time(NULL) - player->lastLoginSaved) << ", ";
	}
	query << "`blessings` = " << player->blessings;
	query << " WHERE `id` = " << player->getGUID();

	DBTransaction transaction;
	if (!transaction.begin()) {
		return false;
	}

	if (!db->executeQuery(query.str())) {
		return false;
	}

	// skills
	for (int32_t i = SKILL_FIRST; i <= SKILL_LAST; i++) {
		query.str("");
		query << "UPDATE `player_skills` SET `value` = " << player->skills[i][SKILL_LEVEL] << ", `count` = " << player->skills[i][SKILL_TRIES] << " WHERE `player_id` = " << player->getGUID() << " AND `skillid` = " << i;

		if (!db->executeQuery(query.str())) {
			return false;
		}
	}

	// learned spells
	query.str("");
	query << "DELETE FROM `player_spells` WHERE `player_id` = " << player->getGUID();

	if (!db->executeQuery(query.str())) {
		return false;
	}

	query.str("");

	DBInsert stmt(db);
	stmt.setQuery("INSERT INTO `player_spells` (`player_id`, `name` ) VALUES ");

	for (LearnedInstantSpellList::const_iterator it = player->learnedInstantSpellList.begin();
	        it != player->learnedInstantSpellList.end(); ++it) {
		query << player->getGUID() << "," << db->escapeString(*it);

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
			itemList.push_back(itemBlock(slotId, item));
		}
	}

	if (!saveItems(player, itemList, stmt)) {
		return false;
	}

	if (player->depotChange) {
		//save depot items
		query.str("");
		query << "DELETE FROM `player_depotitems` WHERE `player_id` = " << player->getGUID();

		if (!db->executeQuery(query.str())) {
			return false;
		}

		stmt.setQuery("INSERT INTO `player_depotitems` (`player_id`, `pid`, `sid`, `itemtype`, `count`, `attributes`) VALUES ");
		itemList.clear();

		for (DepotMap::iterator it = player->depotChests.begin(); it != player->depotChests.end() ; ++it) {
			DepotChest* depotChest = it->second;

			for (ItemDeque::const_iterator iit = depotChest->getItems(), end = depotChest->getEnd(); iit != end; ++iit) {
				itemList.push_back(itemBlock(it->first, *iit));
			}
		}

		if (!saveItems(player, itemList, stmt)) {
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

	for (ItemDeque::const_iterator it = player->getInbox()->getItems(), end = player->getInbox()->getEnd(); it != end; ++it) {
		itemList.push_back(itemBlock(0, *it));
	}

	if (!saveItems(player, itemList, stmt)) {
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

	for (StorageMap::const_iterator cit = player->getStorageIteratorBegin(), end = player->getStorageIteratorEnd(); cit != end; ++cit) {
		query << player->getGUID() << "," << cit->first << "," << cit->second;

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
	NameCacheMap::const_iterator it = nameCacheMap.find(guid);

	if (it != nameCacheMap.end()) {
		name = it->second;
		return true;
	}

	std::ostringstream query;
	query << "SELECT `name` FROM `players` WHERE `id` = " << guid;

	Database* db = Database::getInstance();

	DBResult* result = db->storeQuery(query.str());
	if (!result) {
		return false;
	}

	name = result->getDataString("name");
	db->freeResult(result);

	nameCacheMap[guid] = name;
	return true;
}

bool IOLoginData::getGuidByName(uint32_t& guid, std::string& name)
{
	GuidCacheMap::const_iterator it = guidCacheMap.find(name);

	if (it != guidCacheMap.end()) {
		name = it->first;
		guid = it->second;
		return true;
	}

	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `id`, `name` FROM `players` WHERE `name` = " << db->escapeString(name);

	DBResult* result = db->storeQuery(query.str());
	if (!result) {
		return false;
	}

	name = result->getDataString("name");
	guid = result->getDataInt("id");
	db->freeResult(result);

	guidCacheMap[name] = guid;
	return true;
}

bool IOLoginData::getGuidByNameEx(uint32_t& guid, bool& specialVip, std::string& name)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `name`, `id`, `group_id`, `account_id` FROM `players` WHERE `name` = " << db->escapeString(name);

	DBResult* result = db->storeQuery(query.str());
	if (!result) {
		return false;
	}

	name = result->getDataString("name");
	guid = result->getDataInt("id");
	const PlayerGroup* accountGroup = getPlayerGroupByAccount(result->getDataInt("account_id"));
	const PlayerGroup* playerGroup = getPlayerGroup(result->getDataInt("group_id"));
	db->freeResult(result);

	uint64_t flags = 0;

	if (playerGroup) {
		flags |= playerGroup->m_flags;
	}

	if (accountGroup) {
		flags |= accountGroup->m_flags;
	}

	specialVip = (0 != (flags & ((uint64_t)1 << PlayerFlag_SpecialVIP)));
	return true;
}

bool IOLoginData::playerExists(const std::string& name)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	DBResult* result;

	query << "SELECT `id` FROM `players` WHERE `name` = " << db->escapeString(name);

	if ((result = db->storeQuery(query.str()))) {
		db->freeResult(result);
		return true;
	}

	return false;
}

bool IOLoginData::formatPlayerName(std::string& name)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `name` FROM `players` WHERE `name` = " << db->escapeString(name);

	DBResult* result = db->storeQuery(query.str());
	if (!result) {
		return false;
	}

	name = result->getDataString("name");
	db->freeResult(result);
	return true;
}

const PlayerGroup* IOLoginData::getPlayerGroup(uint32_t groupid)
{
	PlayerGroupMap::const_iterator it = playerGroupMap.find(groupid);
	if (it != playerGroupMap.end()) {
		return it->second;
	}

	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `name`, `flags`, `access`, `maxdepotitems`, `maxviplist` FROM `groups` WHERE `id` = " << groupid;

	DBResult* result = db->storeQuery(query.str());
	if (!result) {
		return NULL;
	}

	PlayerGroup* group = new PlayerGroup;
	group->m_name = result->getDataString("name");
	group->m_flags = result->getDataLong("flags");
	group->m_access = result->getDataInt("access");
	group->m_maxdepotitems = result->getDataInt("maxdepotitems");
	group->m_maxviplist = result->getDataInt("maxviplist");
	db->freeResult(result);

	playerGroupMap[groupid] = group;
	return group;
}

const PlayerGroup* IOLoginData::getPlayerGroupByAccount(uint32_t accno)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `group_id` FROM `accounts` WHERE `id` = " << accno;

	DBResult* result = db->storeQuery(query.str());
	if (!result) {
		return NULL;
	}

	uint32_t groupId = result->getDataInt("group_id");
	db->freeResult(result);
	return getPlayerGroup(groupId);
}

void IOLoginData::loadItems(ItemMap& itemMap, DBResult* result)
{
	do {
		int32_t sid = result->getDataInt("sid");
		int32_t pid = result->getDataInt("pid");
		int32_t type = result->getDataInt("itemtype");
		int32_t count = result->getDataInt("count");

		unsigned long attrSize = 0;
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

bool IOLoginData::changeName(uint32_t guid, const std::string& newName)
{
	Database* db = Database::getInstance();
	std::ostringstream query;
	query << "UPDATE `players` SET `name` = " << db->escapeString(newName) << " WHERE `id` = " << guid;

	if (!db->executeQuery(query.str())) {
		return false;
	}

	nameCacheMap[guid] = newName;
	return true;
}

uint32_t IOLoginData::getAccountNumberByName(const std::string& name)
{
	Database* db = Database::getInstance();
	std::ostringstream query;
	query << "SELECT `account_id` FROM `players` WHERE `name` = " << db->escapeString(name);

	DBResult* result = db->storeQuery(query.str());
	if (!result) {
		return 0;
	}

	uint32_t accountId = result->getDataInt("account_id");
	db->freeResult(result);
	return accountId;
}

void IOLoginData::increaseBankBalance(uint32_t guid, uint64_t bankBalance)
{
	std::ostringstream query;
	query << "UPDATE `players` SET `balance` = `balance` + " << bankBalance << " WHERE `id` = " << guid;
	Database::getInstance()->executeQuery(query.str());
}

time_t IOLoginData::getLastLoginSaved(uint32_t guid)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `lastlogin` FROM `players` WHERE `id` = " << guid;
	DBResult* result;
	time_t lastLoginSaved;

	if ((result = db->storeQuery(query.str()))) {
		lastLoginSaved = result->getDataLong("lastlogin");
		db->freeResult(result);
	} else {
		lastLoginSaved = 0;
	}

	return lastLoginSaved;
}

void IOLoginData::updateHouseOwners()
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	DBResult* result;
	query << "SELECT `id`, `highest_bidder`, `last_bid` FROM `houses` WHERE `owner` = 0 AND `bid_end` != 0 AND `bid_end` < " << time(NULL);

	if ((result = db->storeQuery(query.str()))) {
		do {
			House* house = Houses::getInstance().getHouse(result->getDataInt("id"));

			if (house) {
				DBResult* result2;
				query.str("");
				query << "SELECT `balance` FROM `players` WHERE `id` = " << result->getDataInt("highest_bidder");
				bool canPay = false;

				if ((result2 = db->storeQuery(query.str()))) {
					if (result2->getDataInt("balance") >= result->getDataInt("last_bid")) {
						canPay = true;
					}

					db->freeResult(result2);
				}

				if (canPay) {
					query.str("");
					query << "UPDATE `players` SET `balance` = `balance` - " << result->getDataInt("last_bid") << " WHERE `id` = " << result->getDataInt("highest_bidder");
					db->executeQuery(query.str());

					house->setHouseOwner(result->getDataInt("highest_bidder"));
				}

				query.str("");
				query << "UPDATE `houses` SET `last_bid` = 0, `bid_end` = 0, `highest_bidder` = 0, `bid` = 0 WHERE `id` = " << result->getDataInt("id");
				db->executeQuery(query.str());
			}
		} while (result->next());

		db->freeResult(result);
	}
}

bool IOLoginData::hasBiddedOnHouse(uint32_t guid)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	DBResult* result;
	query << "SELECT `id` FROM `houses` WHERE `highest_bidder` = " << guid << " LIMIT 1";

	if ((result = db->storeQuery(query.str()))) {
		db->freeResult(result);
		return true;
	}

	return false;
}

std::list<VIPEntry> IOLoginData::getVIPEntries(uint32_t accountId)
{
	std::list<VIPEntry> entries;

	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "SELECT `player_id`, (SELECT `name` FROM `players` WHERE `id` = `player_id`) AS `name`, `description`, `icon`, `notify` FROM `account_viplist` WHERE `account_id` = " << accountId;

	DBResult* result;

	if ((result = db->storeQuery(query.str()))) {
		do {
			VIPEntry entry;
			entry.guid = result->getDataInt("player_id");
			entry.name = result->getDataString("name");
			entry.description = result->getDataString("description");
			entry.icon = result->getDataInt("icon");
			entry.notify = result->getDataInt("notify") != 0;
			entries.push_back(entry);
		} while (result->next());

		db->freeResult(result);
	}

	return entries;
}

void IOLoginData::addVIPEntry(uint32_t accountId, uint32_t guid, const std::string& description, uint32_t icon, bool notify)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "INSERT INTO `account_viplist` (`account_id`, `player_id`, `description`, `icon`, `notify`) VALUES (" << accountId << ", " << guid << ", " << db->escapeString(description) << ", " << icon << ", " << notify << ")";
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
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "DELETE FROM `account_viplist` WHERE `account_id` = " << accountId << " AND `player_id` = " << guid;
	db->executeQuery(query.str());
}

void IOLoginData::addPremiumDays(uint32_t accountId, int32_t addDays)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "UPDATE `accounts` SET `premdays` = `premdays` + " << addDays << " WHERE `id` = " << accountId;
	db->executeQuery(query.str());
}

void IOLoginData::removePremiumDays(uint32_t accountId, int32_t removeDays)
{
	Database* db = Database::getInstance();

	std::ostringstream query;
	query << "UPDATE `accounts` SET `premdays` = `premdays` - " << removeDays << " WHERE `id` = " << accountId;
	db->executeQuery(query.str());
}
