// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "iologindata.h"

#include "condition.h"
#include "configmanager.h"
#include "depotchest.h"
#include "game.h"
#include "inbox.h"
#include "storeinbox.h"

extern Game g_game;

uint32_t IOLoginData::getAccountIdByPlayerName(const std::string& playerName)
{
	Database& db = Database::getInstance();

	DBResult_ptr result = db.storeQuery(
	    fmt::format("SELECT `account_id` FROM `players` WHERE `name` = {:s}", db.escapeString(playerName)));
	if (!result) {
		return 0;
	}
	return result->getNumber<uint32_t>("account_id");
}

uint32_t IOLoginData::getAccountIdByPlayerId(uint32_t playerId)
{
	Database& db = Database::getInstance();

	DBResult_ptr result = db.storeQuery(fmt::format("SELECT `account_id` FROM `players` WHERE `id` = {:d}", playerId));
	if (!result) {
		return 0;
	}
	return result->getNumber<uint32_t>("account_id");
}

AccountType_t IOLoginData::getAccountType(uint32_t accountId)
{
	DBResult_ptr result =
	    Database::getInstance().storeQuery(fmt::format("SELECT `type` FROM `accounts` WHERE `id` = {:d}", accountId));
	if (!result) {
		return ACCOUNT_TYPE_NORMAL;
	}
	return static_cast<AccountType_t>(result->getNumber<uint16_t>("type"));
}

void IOLoginData::setAccountType(uint32_t accountId, AccountType_t accountType)
{
	Database::getInstance().executeQuery(fmt::format("UPDATE `accounts` SET `type` = {:d} WHERE `id` = {:d}",
	                                                 static_cast<uint16_t>(accountType), accountId));
}

void IOLoginData::updateOnlineStatus(uint32_t guid, bool login)
{
	if (getBoolean(ConfigManager::ALLOW_CLONES)) {
		return;
	}

	if (login) {
		Database::getInstance().executeQuery(fmt::format("INSERT INTO `players_online` VALUES ({:d})", guid));
	} else {
		Database::getInstance().executeQuery(
		    fmt::format("DELETE FROM `players_online` WHERE `player_id` = {:d}", guid));
	}
}

bool IOLoginData::preloadPlayer(const std::shared_ptr<Player>& player)
{
	Database& db = Database::getInstance();

	DBResult_ptr result = db.storeQuery(fmt::format(
	    "SELECT `p`.`name`, `p`.`account_id`, `p`.`group_id`, `a`.`type`, `a`.`premium_ends_at` FROM `players` AS `p` JOIN `accounts` AS `a` ON `a`.`id` = `p`.`account_id` WHERE `p`.`id` = {:d} AND `p`.`deletion` = 0",
	    player->getGUID()));
	if (!result) {
		return false;
	}

	player->setName(result->getString("name"));
	Group* group = g_game.groups.getGroup(result->getNumber<uint16_t>("group_id"));
	if (!group) {
		std::cout << "[Error - IOLoginData::preloadPlayer] " << player->name << " has Group ID "
		          << result->getNumber<uint16_t>("group_id") << " which doesn't exist." << std::endl;
		return false;
	}
	player->setGroup(group);
	player->accountNumber = result->getNumber<uint32_t>("account_id");
	player->accountType = static_cast<AccountType_t>(result->getNumber<uint16_t>("type"));
	player->premiumEndsAt = result->getNumber<time_t>("premium_ends_at");
	return true;
}

bool IOLoginData::loadPlayerById(const std::shared_ptr<Player>& player, uint32_t id)
{
	Database& db = Database::getInstance();
	return loadPlayer(
	    player,
	    db.storeQuery(fmt::format(
	        "SELECT `id`, `name`, `account_id`, `group_id`, `sex`, `vocation`, `experience`, `level`, `maglevel`, `health`, `healthmax`, `blessings`, `mana`, `manamax`, `manaspent`, `soul`, `lookbody`, `lookfeet`, `lookhead`, `looklegs`, `looktype`, `lookaddons`, `lookmount`, `lookmounthead`, `lookmountbody`, `lookmountlegs`, `lookmountfeet`, `currentmount`, `randomizemount`, `posx`, `posy`, `posz`, `cap`, `lastlogin`, `lastlogout`, `lastip`, `conditions`, `skulltime`, `skull`, `town_id`, `balance`, `offlinetraining_time`, `offlinetraining_skill`, `stamina`, `skill_fist`, `skill_fist_tries`, `skill_club`, `skill_club_tries`, `skill_sword`, `skill_sword_tries`, `skill_axe`, `skill_axe_tries`, `skill_dist`, `skill_dist_tries`, `skill_shielding`, `skill_shielding_tries`, `skill_fishing`, `skill_fishing_tries`, `direction` FROM `players` WHERE `id` = {:d}",
	        id)));
}

bool IOLoginData::loadPlayerByName(const std::shared_ptr<Player>& player, const std::string& name)
{
	Database& db = Database::getInstance();
	return loadPlayer(
	    player,
	    db.storeQuery(fmt::format(
	        "SELECT `id`, `name`, `account_id`, `group_id`, `sex`, `vocation`, `experience`, `level`, `maglevel`, `health`, `healthmax`, `blessings`, `mana`, `manamax`, `manaspent`, `soul`, `lookbody`, `lookfeet`, `lookhead`, `looklegs`, `looktype`, `lookaddons`, `lookmount`, `lookmounthead`, `lookmountbody`, `lookmountlegs`, `lookmountfeet`, `currentmount`, `randomizemount`, `posx`, `posy`, `posz`, `cap`, `lastlogin`, `lastlogout`, `lastip`, `conditions`, `skulltime`, `skull`, `town_id`, `balance`, `offlinetraining_time`, `offlinetraining_skill`, `stamina`, `skill_fist`, `skill_fist_tries`, `skill_club`, `skill_club_tries`, `skill_sword`, `skill_sword_tries`, `skill_axe`, `skill_axe_tries`, `skill_dist`, `skill_dist_tries`, `skill_shielding`, `skill_shielding_tries`, `skill_fishing`, `skill_fishing_tries`, `direction` FROM `players` WHERE `name` = {:s}",
	        db.escapeString(name))));
}

static GuildWarVector getWarList(uint32_t guildId)
{
	DBResult_ptr result = Database::getInstance().storeQuery(fmt::format(
	    "SELECT `guild1`, `guild2` FROM `guild_wars` WHERE (`guild1` = {:d} OR `guild2` = {:d}) AND `ended` = 0 AND `status` = 1",
	    guildId, guildId));
	if (!result) {
		return {};
	}

	GuildWarVector guildWarVector;
	do {
		uint32_t guild1 = result->getNumber<uint32_t>("guild1");
		if (guildId != guild1) {
			guildWarVector.push_back(guild1);
		} else {
			guildWarVector.push_back(result->getNumber<uint32_t>("guild2"));
		}
	} while (result->next());
	return guildWarVector;
}

bool IOLoginData::loadPlayer(const std::shared_ptr<Player>& player, DBResult_ptr result)
{
	if (!result) {
		return false;
	}

	Database& db = Database::getInstance();

	uint32_t accountId = result->getNumber<uint32_t>("account_id");

	auto account =
	    db.storeQuery(fmt::format("SELECT `type`, `premium_ends_at` FROM `accounts` WHERE `id` = {:d}", accountId));
	if (!account) {
		return false;
	}

	player->accountType = static_cast<AccountType_t>(account->getNumber<int32_t>("type"));
	player->premiumEndsAt = account->getNumber<time_t>("premium_ends_at");

	player->setGUID(result->getNumber<uint32_t>("id"));
	player->name = result->getString("name");
	player->accountNumber = accountId;

	Group* group = g_game.groups.getGroup(result->getNumber<uint16_t>("group_id"));
	if (!group) {
		std::cout << "[Error - IOLoginData::loadPlayer] " << player->name << " has Group ID "
		          << result->getNumber<uint16_t>("group_id") << " which doesn't exist" << std::endl;
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
		player->levelPercent = static_cast<uint8_t>(
		    Player::getBasisPointLevel(player->experience - currExpCount, nextExpCount - currExpCount) / 100);
	} else {
		player->levelPercent = 0;
	}

	player->soul = result->getNumber<uint16_t>("soul");
	player->capacity = result->getNumber<uint32_t>("cap") * 100;
	player->blessings = result->getNumber<uint16_t>("blessings");

	auto conditions = result->getString("conditions");
	PropStream propStream;
	propStream.init(conditions.data(), conditions.size());

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
		std::cout << "[Error - IOLoginData::loadPlayer] " << player->name << " has Vocation ID "
		          << result->getNumber<uint16_t>("vocation") << " which doesn't exist" << std::endl;
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
	player->magLevelPercent = Player::getBasisPointLevel(player->manaSpent, nextManaCount);

	player->health = result->getNumber<int32_t>("health");
	player->healthMax = result->getNumber<int32_t>("healthmax");

	player->defaultOutfit.lookType = result->getNumber<uint16_t>("looktype");
	player->defaultOutfit.lookHead = result->getNumber<uint16_t>("lookhead");
	player->defaultOutfit.lookBody = result->getNumber<uint16_t>("lookbody");
	player->defaultOutfit.lookLegs = result->getNumber<uint16_t>("looklegs");
	player->defaultOutfit.lookFeet = result->getNumber<uint16_t>("lookfeet");
	player->defaultOutfit.lookAddons = result->getNumber<uint16_t>("lookaddons");
	player->defaultOutfit.lookMount = result->getNumber<uint16_t>("lookmount");
	player->defaultOutfit.lookMountHead = result->getNumber<uint16_t>("lookmounthead");
	player->defaultOutfit.lookMountBody = result->getNumber<uint16_t>("lookmountbody");
	player->defaultOutfit.lookMountLegs = result->getNumber<uint16_t>("lookmountlegs");
	player->defaultOutfit.lookMountFeet = result->getNumber<uint16_t>("lookmountfeet");
	player->currentOutfit = player->defaultOutfit;
	player->currentMount = result->getNumber<uint16_t>("currentmount");
	player->direction = static_cast<Direction>(result->getNumber<uint16_t>("direction"));
	player->randomizeMount = result->getNumber<uint8_t>("randomizemount") != 0;

	if (g_game.getWorldType() != WORLD_TYPE_PVP_ENFORCED) {
		const time_t skullSeconds = result->getNumber<time_t>("skulltime") - time(nullptr);
		if (skullSeconds > 0) {
			// ensure that we round up the number of ticks
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

	const Town* town = g_game.map.towns.getTown(result->getNumber<uint32_t>("town_id"));
	if (!town) {
		std::cout << "[Error - IOLoginData::loadPlayer] " << player->name << " has Town ID "
		          << result->getNumber<uint32_t>("town_id") << " which doesn't exist" << std::endl;
		return false;
	}

	player->town = town;

	const Position& loginPos = player->loginPosition;
	if (loginPos.x == 0 && loginPos.y == 0 && loginPos.z == 0) {
		player->loginPosition = player->getTemplePosition();
	}

	player->staminaMinutes = result->getNumber<uint16_t>("stamina");

	static const std::string skillNames[] = {"skill_fist", "skill_club",      "skill_sword",  "skill_axe",
	                                         "skill_dist", "skill_shielding", "skill_fishing"};
	static const std::string skillNameTries[] = {"skill_fist_tries",   "skill_club_tries", "skill_sword_tries",
	                                             "skill_axe_tries",    "skill_dist_tries", "skill_shielding_tries",
	                                             "skill_fishing_tries"};
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
		player->skills[i].percent = Player::getBasisPointLevel(skillTries, nextSkillTries);
	}

	if ((result = db.storeQuery(
	         fmt::format("SELECT `guild_id`, `rank_id`, `nick` FROM `guild_membership` WHERE `player_id` = {:d}",
	                     player->getGUID())))) {
		uint32_t guildId = result->getNumber<uint32_t>("guild_id");
		uint32_t playerRankId = result->getNumber<uint32_t>("rank_id");
		player->guildNick = result->getString("nick");

		auto guild = g_game.getGuild(guildId);
		if (!guild) {
			guild = IOGuild::loadGuild(guildId);
			if (guild) {
				g_game.addGuild(guild);
			} else {
				std::cout << "[Warning - IOLoginData::loadPlayer] " << player->name << " has Guild ID " << guildId
				          << " which doesn't exist" << std::endl;
			}
		}

		if (guild) {
			player->guild = guild;
			auto rank = guild->getRankById(playerRankId);
			if (!rank) {
				if ((result = db.storeQuery(fmt::format(
				         "SELECT `id`, `name`, `level` FROM `guild_ranks` WHERE `id` = {:d}", playerRankId)))) {
					guild->addRank(result->getNumber<uint32_t>("id"), result->getString("name"),
					               result->getNumber<uint16_t>("level"));
				}

				rank = guild->getRankById(playerRankId);
				if (!rank) {
					player->guild = nullptr;
				}
			}

			player->guildRank = rank;
			player->guildWarVector = getWarList(guildId);

			if ((result = db.storeQuery(fmt::format(
			         "SELECT COUNT(*) AS `members` FROM `guild_membership` WHERE `guild_id` = {:d}", guildId)))) {
				guild->setMemberCount(result->getNumber<uint32_t>("members"));
			}
		}
	}

	if ((result = db.storeQuery(fmt::format("SELECT `player_id`, `name` FROM `player_spells` WHERE `player_id` = {:d}",
	                                        player->getGUID())))) {
		do {
			player->learnedInstantSpellList.emplace_front(result->getString("name"));
		} while (result->next());
	}

	// load inventory items
	ItemMap itemMap;
	std::map<uint8_t, std::shared_ptr<Container>> openContainersList;

	if ((result = db.storeQuery(fmt::format(
	         "SELECT `pid`, `sid`, `itemtype`, `count`, `attributes` FROM `player_items` WHERE `player_id` = {:d} ORDER BY `sid` DESC",
	         player->getGUID())))) {
		loadItems(itemMap, result);

		for (ItemMap::const_reverse_iterator it = itemMap.rbegin(), end = itemMap.rend(); it != end; ++it) {
			const auto& [item, pid] = it->second;

			auto itemContainer = item->getContainer();
			if (itemContainer) {
				uint8_t cid = item->getIntAttr(ITEM_ATTRIBUTE_OPENCONTAINER);
				if (cid > 0) {
					openContainersList.emplace(cid, itemContainer);
				}
			}

			if (pid >= CONST_SLOT_FIRST && pid <= CONST_SLOT_LAST) {
				player->internalAddThing(pid, item);
			} else {
				ItemMap::const_iterator it2 = itemMap.find(pid);
				if (it2 == itemMap.end()) {
					continue;
				}

				auto container = it2->second.first->getContainer();
				if (container) {
					container->internalAddThing(item);
				}
			}
		}
	}

	for (auto& it : openContainersList) {
		player->addContainer(it.first - 1, it.second);
		player->onSendContainer(it.second);
	}

	// load depot items
	itemMap.clear();

	if ((result = db.storeQuery(fmt::format(
	         "SELECT `pid`, `sid`, `itemtype`, `count`, `attributes` FROM `player_depotitems` WHERE `player_id` = {:d} ORDER BY `sid` DESC",
	         player->getGUID())))) {
		loadItems(itemMap, result);

		for (ItemMap::const_reverse_iterator it = itemMap.rbegin(), end = itemMap.rend(); it != end; ++it) {
			const auto& [item, pid] = it->second;

			if (pid < 100) {
				if (const auto& depotChest = player->getDepotChest(pid, true)) {
					depotChest->internalAddThing(item);
				}
			} else {
				ItemMap::const_iterator it2 = itemMap.find(pid);
				if (it2 == itemMap.end()) {
					continue;
				}

				auto container = it2->second.first->getContainer();
				if (container) {
					container->internalAddThing(item);
				}
			}
		}
	}

	// load inbox items
	itemMap.clear();

	if ((result = db.storeQuery(fmt::format(
	         "SELECT `pid`, `sid`, `itemtype`, `count`, `attributes` FROM `player_inboxitems` WHERE `player_id` = {:d} ORDER BY `sid` DESC",
	         player->getGUID())))) {
		loadItems(itemMap, result);

		for (ItemMap::const_reverse_iterator it = itemMap.rbegin(), end = itemMap.rend(); it != end; ++it) {
			const auto& [item, pid] = it->second;

			if (pid < 100) {
				player->getInbox()->internalAddThing(item);
			} else {
				ItemMap::const_iterator it2 = itemMap.find(pid);

				if (it2 == itemMap.end()) {
					continue;
				}

				auto container = it2->second.first->getContainer();
				if (container) {
					container->internalAddThing(item);
				}
			}
		}
	}

	// load store inbox items
	itemMap.clear();

	if ((result = db.storeQuery(fmt::format(
	         "SELECT `pid`, `sid`, `itemtype`, `count`, `attributes` FROM `player_storeinboxitems` WHERE `player_id` = {:d} ORDER BY `sid` DESC",
	         player->getGUID())))) {
		loadItems(itemMap, result);

		for (ItemMap::const_reverse_iterator it = itemMap.rbegin(), end = itemMap.rend(); it != end; ++it) {
			const auto& [item, pid] = it->second;

			if (pid < 100) {
				player->getStoreInbox()->internalAddThing(item);
			} else {
				ItemMap::const_iterator it2 = itemMap.find(pid);

				if (it2 == itemMap.end()) {
					continue;
				}

				auto container = it2->second.first->getContainer();
				if (container) {
					container->internalAddThing(item);
				}
			}
		}
	}

	// load storage map
	if ((result = db.storeQuery(
	         fmt::format("SELECT `key`, `value` FROM `player_storage` WHERE `player_id` = {:d}", player->getGUID())))) {
		do {
			player->setStorageValue(result->getNumber<uint32_t>("key"), result->getNumber<int32_t>("value"), true);
		} while (result->next());
	}

	// load vip list
	if ((result = db.storeQuery(fmt::format("SELECT `player_id` FROM `account_viplist` WHERE `account_id` = {:d}",
	                                        player->getAccount())))) {
		do {
			player->addVIPInternal(result->getNumber<uint32_t>("player_id"));
		} while (result->next());
	}

	// load outfits & addons
	if ((result = db.storeQuery(fmt::format(
	         "SELECT `outfit_id`, `addons` FROM `player_outfits` WHERE `player_id` = {:d}", player->getGUID())))) {
		do {
			player->addOutfit(result->getNumber<uint16_t>("outfit_id"), result->getNumber<uint8_t>("addons"));
		} while (result->next());
	}

	// load mounts
	if ((result = db.storeQuery(
	         fmt::format("SELECT `mount_id` FROM `player_mounts` WHERE `player_id` = {:d}", player->getGUID())))) {
		do {
			player->tameMount(result->getNumber<uint16_t>("mount_id"));
		} while (result->next());
	}

	player->updateBaseSpeed();
	player->updateInventoryWeight();
	player->updateItemsLight(true);
	return true;
}

bool IOLoginData::saveItems(const std::shared_ptr<const Player>& player, const ItemBlockList& itemList,
                            DBInsert& query_insert, PropWriteStream& propWriteStream)
{
	using ContainerBlock = std::pair<std::shared_ptr<const Container>, int32_t>;
	std::vector<ContainerBlock> containers;
	containers.reserve(32);

	int32_t runningId = 100;
	const auto& openContainers = player->getOpenContainers();

	Database& db = Database::getInstance();
	for (const auto& [pid, item] : itemList) {
		++runningId;

		if (auto container = item->getContainer()) {
			if (container->getIntAttr(ITEM_ATTRIBUTE_OPENCONTAINER)) {
				container->setIntAttr(ITEM_ATTRIBUTE_OPENCONTAINER, 0);
			}

			if (!openContainers.empty()) {
				for (const auto& [id, openContainer] : openContainers) {
					auto opcontainer = openContainer.container;

					if (opcontainer == container) {
						container->setIntAttr(ITEM_ATTRIBUTE_OPENCONTAINER, static_cast<int64_t>(id) + 1);
						break;
					}
				}
			}

			containers.emplace_back(container, runningId);
		}

		propWriteStream.clear();
		item->serializeAttr(propWriteStream);

		if (!query_insert.addRow(fmt::format("{:d}, {:d}, {:d}, {:d}, {:d}, {:s}", player->getGUID(), pid, runningId,
		                                     item->getID(), item->getSubType(),
		                                     db.escapeString(propWriteStream.getStream())))) {
			return false;
		}
	}

	for (size_t i = 0; i < containers.size(); ++i) {
		const auto& [container, parentId] = containers[i];

		for (const auto& item : container->getItemList()) {
			++runningId;

			auto subContainer = item->getContainer();
			if (subContainer) {
				containers.emplace_back(subContainer, runningId);

				if (subContainer->getIntAttr(ITEM_ATTRIBUTE_OPENCONTAINER)) {
					subContainer->setIntAttr(ITEM_ATTRIBUTE_OPENCONTAINER, 0);
				}

				if (!openContainers.empty()) {
					for (const auto& [id, openContainer] : openContainers) {
						auto opcontainer = openContainer.container;

						if (opcontainer == subContainer) {
							subContainer->setIntAttr(ITEM_ATTRIBUTE_OPENCONTAINER, id + 1);
							break;
						}
					}
				}
			}

			propWriteStream.clear();
			item->serializeAttr(propWriteStream);

			if (!query_insert.addRow(fmt::format("{:d}, {:d}, {:d}, {:d}, {:d}, {:s}", player->getGUID(), parentId,
			                                     runningId, item->getID(), item->getSubType(),
			                                     db.escapeString(propWriteStream.getStream())))) {
				return false;
			}
		}
	}
	return query_insert.execute();
}

bool IOLoginData::savePlayer(const std::shared_ptr<Player>& player)
{
	if (player->isDead()) {
		player->changeHealth(1);
	}

	Database& db = Database::getInstance();

	DBResult_ptr result =
	    db.storeQuery(fmt::format("SELECT `save` FROM `players` WHERE `id` = {:d}", player->getGUID()));
	if (!result) {
		return false;
	}

	if (result->getNumber<uint16_t>("save") == 0) {
		return db.executeQuery(
		    fmt::format("UPDATE `players` SET `lastlogin` = {:d}, `lastip` = INET6_ATON('{:s}') WHERE `id` = {:d}",
		                player->lastLoginSaved, player->lastIP.to_string(), player->getGUID()));
	}

	// serialize conditions
	PropWriteStream propWriteStream;
	for (Condition* condition : player->conditions) {
		if (condition->isPersistent()) {
			condition->serialize(propWriteStream);
			propWriteStream.write<uint8_t>(CONDITIONATTR_END);
		}
	}

	// First, an UPDATE query to write the player itself
	std::ostringstream query;
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
	query << "`lookmount` = " << player->defaultOutfit.lookMount << ',';
	query << "`lookmounthead` = " << static_cast<uint32_t>(player->defaultOutfit.lookMountHead) << ',';
	query << "`lookmountbody` = " << static_cast<uint32_t>(player->defaultOutfit.lookMountBody) << ',';
	query << "`lookmountlegs` = " << static_cast<uint32_t>(player->defaultOutfit.lookMountLegs) << ',';
	query << "`lookmountfeet` = " << static_cast<uint32_t>(player->defaultOutfit.lookMountFeet) << ',';
	query << "`currentmount` = " << static_cast<uint16_t>(player->currentMount) << ',';
	query << "`randomizemount` = " << player->randomizeMount << ",";
	query << "`maglevel` = " << player->magLevel << ',';
	query << "`mana` = " << player->mana << ',';
	query << "`manamax` = " << player->manaMax << ',';
	query << "`manaspent` = " << player->manaSpent << ',';
	query << "`soul` = " << static_cast<uint16_t>(player->soul) << ',';
	query << "`town_id` = " << player->town->id << ',';

	const Position& loginPosition = player->getLoginPosition();
	query << "`posx` = " << loginPosition.getX() << ',';
	query << "`posy` = " << loginPosition.getY() << ',';
	query << "`posz` = " << loginPosition.getZ() << ',';

	query << "`cap` = " << (player->capacity / 100) << ',';
	query << "`sex` = " << static_cast<uint16_t>(player->sex) << ',';

	if (player->lastLoginSaved != 0) {
		query << "`lastlogin` = " << player->lastLoginSaved << ',';
	}

	if (!player->lastIP.is_unspecified()) {
		query << "`lastip` = INET6_ATON('" << player->lastIP.to_string() << "'),";
	}

	query << "`conditions` = " << db.escapeString(propWriteStream.getStream()) << ',';

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
	query << "`direction` = " << static_cast<uint16_t>(player->getDirection()) << ',';

	if (!player->isOffline()) {
		query << "`onlinetime` = `onlinetime` + " << (time(nullptr) - player->lastLoginSaved) << ',';
	}
	query << "`blessings` = " << player->blessings.to_ulong();
	query << " WHERE `id` = " << player->getGUID();

	DBTransaction transaction;
	if (!transaction.begin()) {
		return false;
	}

	if (!db.executeQuery(query.str())) {
		return false;
	}

	// learned spells
	if (!db.executeQuery(fmt::format("DELETE FROM `player_spells` WHERE `player_id` = {:d}", player->getGUID()))) {
		return false;
	}

	DBInsert spellsQuery("INSERT INTO `player_spells` (`player_id`, `name`) VALUES ");
	for (const std::string& spellName : player->learnedInstantSpellList) {
		if (!spellsQuery.addRow(fmt::format("{:d}, {:s}", player->getGUID(), db.escapeString(spellName)))) {
			return false;
		}
	}

	if (!spellsQuery.execute()) {
		return false;
	}

	// item saving
	if (!db.executeQuery(fmt::format("DELETE FROM `player_items` WHERE `player_id` = {:d}", player->getGUID()))) {
		return false;
	}

	DBInsert itemsQuery(
	    "INSERT INTO `player_items` (`player_id`, `pid`, `sid`, `itemtype`, `count`, `attributes`) VALUES ");

	ItemBlockList itemList;
	for (int32_t slotId = CONST_SLOT_FIRST; slotId <= CONST_SLOT_LAST; ++slotId) {
		auto item = player->inventory[slotId];
		if (item) {
			itemList.emplace_back(slotId, item);
		}
	}

	if (!saveItems(player, itemList, itemsQuery, propWriteStream)) {
		return false;
	}

	// save depot items
	if (!db.executeQuery(fmt::format("DELETE FROM `player_depotitems` WHERE `player_id` = {:d}", player->getGUID()))) {
		return false;
	}

	DBInsert depotQuery(
	    "INSERT INTO `player_depotitems` (`player_id`, `pid`, `sid`, `itemtype`, `count`, `attributes`) VALUES ");
	itemList.clear();

	for (const auto& [id, depotChest] : player->depotChests) {
		for (const auto& item : depotChest->getItemList()) {
			itemList.emplace_back(id, item);
		}
	}

	if (!saveItems(player, itemList, depotQuery, propWriteStream)) {
		return false;
	}

	// save inbox items
	if (!db.executeQuery(fmt::format("DELETE FROM `player_inboxitems` WHERE `player_id` = {:d}", player->getGUID()))) {
		return false;
	}

	DBInsert inboxQuery(
	    "INSERT INTO `player_inboxitems` (`player_id`, `pid`, `sid`, `itemtype`, `count`, `attributes`) VALUES ");
	itemList.clear();

	for (const auto& item : player->getInbox()->getItemList()) {
		itemList.emplace_back(0, item);
	}

	if (!saveItems(player, itemList, inboxQuery, propWriteStream)) {
		return false;
	}

	// save store inbox items
	if (!db.executeQuery(
	        fmt::format("DELETE FROM `player_storeinboxitems` WHERE `player_id` = {:d}", player->getGUID()))) {
		return false;
	}

	DBInsert storeInboxQuery(
	    "INSERT INTO `player_storeinboxitems` (`player_id`, `pid`, `sid`, `itemtype`, `count`, `attributes`) VALUES ");
	itemList.clear();

	for (const auto& item : player->getStoreInbox()->getItemList()) {
		itemList.emplace_back(0, item);
	}

	if (!saveItems(player, itemList, storeInboxQuery, propWriteStream)) {
		return false;
	}

	if (!db.executeQuery(fmt::format("DELETE FROM `player_storage` WHERE `player_id` = {:d}", player->getGUID()))) {
		return false;
	}

	DBInsert storageQuery("INSERT INTO `player_storage` (`player_id`, `key`, `value`) VALUES ");

	for (const auto& [key, value] : player->getStorageMap()) {
		if (!storageQuery.addRow(fmt::format("{:d}, {:d}, {:d}", player->getGUID(), key, value))) {
			return false;
		}
	}

	if (!storageQuery.execute()) {
		return false;
	}

	// save outfits & addons
	if (!db.executeQuery(fmt::format("DELETE FROM `player_outfits` WHERE `player_id` = {:d}", player->getGUID()))) {
		return false;
	}

	DBInsert outfitQuery("INSERT INTO `player_outfits` (`player_id`, `outfit_id`, `addons`) VALUES ");

	for (const auto& it : player->outfits) {
		if (!outfitQuery.addRow(fmt::format("{:d}, {:d}, {:d}", player->getGUID(), it.first, it.second))) {
			return false;
		}
	}

	if (!outfitQuery.execute()) {
		return false;
	}

	// save mounts
	if (!db.executeQuery(fmt::format("DELETE FROM `player_mounts` WHERE `player_id` = {:d}", player->getGUID()))) {
		return false;
	}

	DBInsert mountQuery("INSERT INTO `player_mounts` (`player_id`, `mount_id`) VALUES ");

	for (const auto& it : player->mounts) {
		if (!mountQuery.addRow(fmt::format("{:d}, {:d}", player->getGUID(), it))) {
			return false;
		}
	}

	if (!mountQuery.execute()) {
		return false;
	}

	// End the transaction
	return transaction.commit();
}

std::string IOLoginData::getNameByGuid(uint32_t guid)
{
	DBResult_ptr result =
	    Database::getInstance().storeQuery(fmt::format("SELECT `name` FROM `players` WHERE `id` = {:d}", guid));
	if (!result) {
		return {};
	}

	auto name = result->getString("name");
	return {name.data(), name.size()};
}

uint32_t IOLoginData::getGuidByName(const std::string& name)
{
	Database& db = Database::getInstance();

	DBResult_ptr result =
	    db.storeQuery(fmt::format("SELECT `id` FROM `players` WHERE `name` = {:s}", db.escapeString(name)));
	if (!result) {
		return 0;
	}
	return result->getNumber<uint32_t>("id");
}

bool IOLoginData::getGuidByNameEx(uint32_t& guid, bool& specialVip, std::string& name)
{
	Database& db = Database::getInstance();

	DBResult_ptr result = db.storeQuery(fmt::format(
	    "SELECT `name`, `id`, `group_id`, `account_id` FROM `players` WHERE `name` = {:s}", db.escapeString(name)));
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

	DBResult_ptr result =
	    db.storeQuery(fmt::format("SELECT `name` FROM `players` WHERE `name` = {:s}", db.escapeString(name)));
	if (!result) {
		return false;
	}

	name = result->getString("name");
	return true;
}

void IOLoginData::loadItems(ItemMap& itemMap, DBResult_ptr result)
{
	do {
		uint32_t sid = result->getNumber<uint32_t>("sid");
		uint32_t pid = result->getNumber<uint32_t>("pid");
		uint16_t type = result->getNumber<uint16_t>("itemtype");
		uint16_t count = result->getNumber<uint16_t>("count");

		auto attr = result->getString("attributes");
		PropStream propStream;
		propStream.init(attr.data(), attr.size());

		auto item = Item::CreateItem(type, count);
		if (item) {
			if (!item->unserializeAttr(propStream)) {
				std::cout << "WARNING: Serialize error in IOLoginData::loadItems" << std::endl;
			}

			itemMap[sid] = std::make_pair(std::move(item), pid);
		}
	} while (result->next());
}

void IOLoginData::increaseBankBalance(uint32_t guid, uint64_t bankBalance)
{
	Database::getInstance().executeQuery(
	    fmt::format("UPDATE `players` SET `balance` = `balance` + {:d} WHERE `id` = {:d}", bankBalance, guid));
}

bool IOLoginData::hasBiddedOnHouse(uint32_t guid)
{
	Database& db = Database::getInstance();
	return db.storeQuery(fmt::format("SELECT `id` FROM `houses` WHERE `highest_bidder` = {:d} LIMIT 1", guid)).get();
}

std::forward_list<VIPEntry> IOLoginData::getVIPEntries(uint32_t accountId)
{
	std::forward_list<VIPEntry> entries;

	DBResult_ptr result = Database::getInstance().storeQuery(fmt::format(
	    "SELECT `player_id`, (SELECT `name` FROM `players` WHERE `id` = `player_id`) AS `name`, `description`, `icon`, `notify` FROM `account_viplist` WHERE `account_id` = {:d}",
	    accountId));
	if (result) {
		do {
			entries.emplace_front(result->getNumber<uint32_t>("player_id"), result->getString("name"),
			                      result->getString("description"), result->getNumber<uint32_t>("icon"),
			                      result->getNumber<uint16_t>("notify") != 0);
		} while (result->next());
	}
	return entries;
}

void IOLoginData::addVIPEntry(uint32_t accountId, uint32_t guid, const std::string& description, uint32_t icon,
                              bool notify)
{
	Database& db = Database::getInstance();
	db.executeQuery(fmt::format(
	    "INSERT INTO `account_viplist` (`account_id`, `player_id`, `description`, `icon`, `notify`) VALUES ({:d}, {:d}, {:s}, {:d}, {:d})",
	    accountId, guid, db.escapeString(description), icon, notify));
}

void IOLoginData::editVIPEntry(uint32_t accountId, uint32_t guid, const std::string& description, uint32_t icon,
                               bool notify)
{
	Database& db = Database::getInstance();
	db.executeQuery(fmt::format(
	    "UPDATE `account_viplist` SET `description` = {:s}, `icon` = {:d}, `notify` = {:d} WHERE `account_id` = {:d} AND `player_id` = {:d}",
	    db.escapeString(description), icon, notify, accountId, guid));
}

void IOLoginData::removeVIPEntry(uint32_t accountId, uint32_t guid)
{
	Database::getInstance().executeQuery(
	    fmt::format("DELETE FROM `account_viplist` WHERE `account_id` = {:d} AND `player_id` = {:d}", accountId, guid));
}

void IOLoginData::updatePremiumTime(uint32_t accountId, time_t endTime)
{
	Database::getInstance().executeQuery(
	    fmt::format("UPDATE `accounts` SET `premium_ends_at` = {:d} WHERE `id` = {:d}", endTime, accountId));
}
