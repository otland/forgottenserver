// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "iologindata.h"

#include "condition.h"
#include "configmanager.h"
#include "depotchest.h"
#include "game.h"
#include "inbox.h"
#include "storeinbox.h"

extern ConfigManager g_config;
extern Game g_game;

Account IOLoginData::loadAccount(uint32_t accno)
{
	Account account;

	DBResult_ptr result = Database::getInstance().storeQuery(fmt::format(
	    "SELECT `id`, `name`, `password`, `type`, `premium_ends_at` FROM `accounts` WHERE `id` = {:d}", accno));
	if (!result) {
		return account;
	}

	account.id = result->getNumber<uint32_t>("id");
	account.name = result->getString("name");
	account.accountType = static_cast<AccountType_t>(result->getNumber<int32_t>("type"));
	account.premiumEndsAt = result->getNumber<time_t>("premium_ends_at");
	return account;
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
			// if a key is broken, return empty and the comparison will always be false since the token must not be
			// empty
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

	DBResult_ptr result = db.storeQuery(fmt::format(
	    "SELECT `id`, `name`, `password`, `secret`, `type`, `premium_ends_at` FROM `accounts` WHERE `name` = {:s} OR `email` = {:s}",
	    db.escapeString(name), db.escapeString(name)));
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
	account.premiumEndsAt = result->getNumber<time_t>("premium_ends_at");

	result = db.storeQuery(fmt::format(
	    "SELECT `name` FROM `players` WHERE `account_id` = {:d} AND `deletion` = 0 ORDER BY `name` ASC", account.id));
	if (result) {
		do {
			account.characters.push_back(result->getString("name"));
		} while (result->next());
	}
	return true;
}

uint32_t IOLoginData::gameworldAuthentication(const std::string& accountName, const std::string& password,
                                              std::string& characterName, std::string& token, uint32_t tokenTime)
{
	Database& db = Database::getInstance();
	DBResult_ptr result = db.storeQuery(
	    fmt::format("SELECT `id`, `password`, `secret` FROM `accounts` WHERE `name` = {:s} OR `email` = {:s}",
	                db.escapeString(accountName), db.escapeString(accountName)));
	if (!result) {
		return 0;
	}

	// two-factor auth
	if (g_config.getBoolean(ConfigManager::TWO_FACTOR_AUTH)) {
		std::string secret = decodeSecret(result->getString("secret"));
		if (!secret.empty()) {
			if (token.empty()) {
				return 0;
			}

			bool tokenValid = token == generateToken(secret, tokenTime) ||
			                  token == generateToken(secret, tokenTime - 1) ||
			                  token == generateToken(secret, tokenTime + 1);
			if (!tokenValid) {
				return 0;
			}
		}
	}

	if (transformToSHA1(password) != result->getString("password")) {
		return 0;
	}

	uint32_t accountId = result->getNumber<uint32_t>("id");

	result = db.storeQuery(
	    fmt::format("SELECT `name` FROM `players` WHERE `name` = {:s} AND `account_id` = {:d} AND `deletion` = 0",
	                db.escapeString(characterName), accountId));
	if (!result) {
		return 0;
	}

	characterName = result->getString("name");
	return accountId;
}

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
	if (g_config.getBoolean(ConfigManager::ALLOW_CLONES)) {
		return;
	}

	if (login) {
		Database::getInstance().executeQuery(fmt::format("INSERT INTO `players_online` VALUES ({:d})", guid));
	} else {
		Database::getInstance().executeQuery(
		    fmt::format("DELETE FROM `players_online` WHERE `player_id` = {:d}", guid));
	}
}

bool IOLoginData::preloadPlayer(Player* player, const std::string& name)
{
	Database& db = Database::getInstance();

	DBResult_ptr result = db.storeQuery(fmt::format(
	    "SELECT `p`.`id`, `p`.`account_id`, `p`.`group_id`, `a`.`type`, `a`.`premium_ends_at` FROM `players` as `p` JOIN `accounts` as `a` ON `a`.`id` = `p`.`account_id` WHERE `p`.`name` = {:s} AND `p`.`deletion` = 0",
	    db.escapeString(name)));
	if (!result) {
		return false;
	}

	player->setGUID(result->getNumber<uint32_t>("id"));
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

bool IOLoginData::loadPlayerById(Player* player, uint32_t id)
{
	Database& db = Database::getInstance();
	return loadPlayer(
	    player,
	    db.storeQuery(fmt::format(
	        "SELECT `id`, `name`, `account_id`, `group_id`, `sex`, `vocation`, `experience`, `level`, `maglevel`, `health`, `healthmax`, `blessings`, `mana`, `manamax`, `manaspent`, `soul`, `lookbody`, `lookfeet`, `lookhead`, `looklegs`, `looktype`, `lookaddons`, `lookmount`, `lookmounthead`, `lookmountbody`, `lookmountlegs`, `lookmountfeet`, `randomizemount`, `posx`, `posy`, `posz`, `cap`, `lastlogin`, `lastlogout`, `lastip`, `conditions`, `skulltime`, `skull`, `town_id`, `balance`, `offlinetraining_time`, `offlinetraining_skill`, `stamina`, `skill_fist`, `skill_fist_tries`, `skill_club`, `skill_club_tries`, `skill_sword`, `skill_sword_tries`, `skill_axe`, `skill_axe_tries`, `skill_dist`, `skill_dist_tries`, `skill_shielding`, `skill_shielding_tries`, `skill_fishing`, `skill_fishing_tries`, `inventory_items`, `depot_items`, `inbox_items`, `storeinbox_items`, `direction` FROM `players` WHERE `id` = {:d}",
	        id)));
}

bool IOLoginData::loadPlayerByName(Player* player, const std::string& name)
{
	Database& db = Database::getInstance();
	return loadPlayer(
	    player,
	    db.storeQuery(fmt::format(
	        "SELECT `id`, `name`, `account_id`, `group_id`, `sex`, `vocation`, `experience`, `level`, `maglevel`, `health`, `healthmax`, `blessings`, `mana`, `manamax`, `manaspent`, `soul`, `lookbody`, `lookfeet`, `lookhead`, `looklegs`, `looktype`, `lookaddons`, `lookmount`, `lookmounthead`, `lookmountbody`, `lookmountlegs`, `lookmountfeet`, `randomizemount`, `posx`, `posy`, `posz`, `cap`, `lastlogin`, `lastlogout`, `lastip`, `conditions`, `skulltime`, `skull`, `town_id`, `balance`, `offlinetraining_time`, `offlinetraining_skill`, `stamina`, `skill_fist`, `skill_fist_tries`, `skill_club`, `skill_club_tries`, `skill_sword`, `skill_sword_tries`, `skill_axe`, `skill_axe_tries`, `skill_dist`, `skill_dist_tries`, `skill_shielding`, `skill_shielding_tries`, `skill_fishing`, `skill_fishing_tries`, `inventory_items`, `depot_items`, `inbox_items`, `storeinbox_items`, `direction` FROM `players` WHERE `name` = {:s}",
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

	player->premiumEndsAt = acc.premiumEndsAt;

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
		player->levelPercent = Player::getPercentLevel(player->experience - currExpCount, nextExpCount - currExpCount);
	} else {
		player->levelPercent = 0;
	}

	player->soul = result->getNumber<uint16_t>("soul");
	player->capacity = result->getNumber<uint32_t>("cap") * 100;
	player->blessings = result->getNumber<uint16_t>("blessings");

	// load conditions
	PropStream& propStream = createPropStream(result, "conditions");

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
	player->magLevelPercent = Player::getPercentLevel(player->manaSpent, nextManaCount);

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

	Town* town = g_game.map.towns.getTown(result->getNumber<uint32_t>("town_id"));
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
		player->skills[i].percent = Player::getPercentLevel(skillTries, nextSkillTries);
	}

	// load inventory items
	loadInventoryItems(result, player);

	// load depot items
	loadDepotItems(result, player);

	// load inbox items
	loadInboxItems(result, player);

	// load store inbox items
	loadStoreInboxItems(result, player);

	if ((result = db.storeQuery(
	         fmt::format("SELECT `guild_id`, `rank_id`, `nick` FROM `guild_membership` WHERE `player_id` = {:d}",
	                     player->getGUID())))) {
		uint32_t guildId = result->getNumber<uint32_t>("guild_id");
		uint32_t playerRankId = result->getNumber<uint32_t>("rank_id");
		player->guildNick = result->getString("nick");

		Guild* guild = g_game.getGuild(guildId);
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
			GuildRank_ptr rank = guild->getRankById(playerRankId);
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

	// load storage map
	if ((result = db.storeQuery(
	         fmt::format("SELECT `key`, `value` FROM `player_storage` WHERE `player_id` = {:d}", player->getGUID())))) {
		do {
			player->addStorageValue(result->getNumber<uint32_t>("key"), result->getNumber<int32_t>("value"), true);
		} while (result->next());
	}

	// load vip list
	if ((result = db.storeQuery(fmt::format("SELECT `player_id` FROM `account_viplist` WHERE `account_id` = {:d}",
	                                        player->getAccount())))) {
		do {
			player->addVIPInternal(result->getNumber<uint32_t>("player_id"));
		} while (result->next());
	}

	// open all containers
	std::map<uint8_t, Container*> openContainersList;
	for (int32_t slotId = CONST_SLOT_FIRST; slotId <= CONST_SLOT_LAST; ++slotId) {
		Item* item = player->inventory[slotId];
		if (!item) {
			continue;
		}

		getOpenContainers(item->getContainer(), openContainersList);
	}

	for (auto& it : openContainersList) {
		player->addContainer(it.first - 1, it.second);
		player->onSendContainer(it.second);
	}

	player->updateBaseSpeed();
	player->updateInventoryWeight();
	player->updateItemsLight(true);
	return true;
}

void IOLoginData::getOpenContainers(Container* container, std::map<uint8_t, Container*> list)
{
	if (!container) {
		return;
	}

	uint8_t cid = container->getIntAttr(ITEM_ATTRIBUTE_OPENCONTAINER);
	if (cid > 0) {
		list.emplace(cid, container);
	}

	for (Item* item : container->getItemList()) {
		if (!item) {
			continue;
		}

		getOpenContainers(item->getContainer(), list);
	}
}

void IOLoginData::loadInventoryItems(DBResult_ptr& result, Player* player)
{
	PropStream& propStream = createPropStream(result, "inventory_items");

	uint32_t slotId;
	while (propStream.read<uint32_t>(slotId)) {
		player->internalAddThing(slotId, loadBinaryItem(propStream, nullptr));
	}
}

void IOLoginData::loadDepotItems(DBResult_ptr& result, Player* player)
{
	PropStream& propStream = createPropStream(result, "depot_items");

	uint32_t depotId;
	while (propStream.read<uint32_t>(depotId)) {
		DepotChest* depotChest = player->getDepotChest(depotId, true);
		if (!depotChest) {
			continue;
		}

		uint32_t count = 0;
		if (!propStream.read<uint32_t>(count)) {
			std::cout << "WARNING: Failed to read count in IOLoginData::loadDepotItems" << std::endl;
		}

		for (uint32_t i = 0; i < count; ++i) {
			loadBinaryItem(propStream, depotChest);
		}
	}
}

void IOLoginData::loadInboxItems(DBResult_ptr& result, Player* player)
{
	PropStream& propStream = createPropStream(result, "inbox_items");

	uint32_t count = 0;
	if (!propStream.read<uint32_t>(count)) {
		std::cout << "WARNING: Failed to read count in IOLoginData::loadInboxItems" << std::endl;
	}

	for (uint32_t i = 0; i < count; ++i) {
		loadBinaryItem(propStream, player->getInbox());
	}
}

void IOLoginData::loadStoreInboxItems(DBResult_ptr& result, Player* player)
{
	PropStream& propStream = createPropStream(result, "storeinbox_items");

	uint32_t count = 0;
	if (!propStream.read<uint32_t>(count)) {
		std::cout << "WARNING: Failed to read count in IOLoginData::loadStoreInboxItems" << std::endl;
	}

	for (uint32_t i = 0; i < count; ++i) {
		loadBinaryItem(propStream, player->getStoreInbox());
	}
}

Item* IOLoginData::loadBinaryItem(PropStream& propStream, Container* parent)
{
	uint16_t type = 0, count = 0;
	if (!propStream.read<uint16_t>(type) || !propStream.read<uint16_t>(count)) {
		std::cout << "WARNING: Failed to read item in IOLoginData::loadBinaryItem" << std::endl;
		return nullptr;
	}

	Item* item = Item::CreateItem(type, count);
	if (!item->unserializeAttr(propStream)) {
		std::cout << "WARNING: Serialize error in IOLoginData::loadBinaryItem" << std::endl;
	}

	if (Container* container = item->getContainer()) {
		uint32_t itemsCount = 0;
		if (!propStream.read<uint32_t>(itemsCount)) {
			std::cout << "WARNING: Failed to read item count in IOLoginData::loadBinaryItem" << std::endl;
		}

		for (uint32_t i = 0; i < itemsCount; ++i) {
			loadBinaryItem(propStream, container);
		}
	}

	if (parent != nullptr) {
		parent->internalAddThing(item);
	}
	return item;
}

bool IOLoginData::savePlayer(Player* player)
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

	size_t conditionsSize;
	const char* conditions = propWriteStream.getStream(conditionsSize);

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
	query << "`randomizemount` = " << player->randomizeMount << ",";
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

	if (!player->lastIP.is_unspecified()) {
		query << "`lastip` = INET6_ATON('" << player->lastIP.to_string() << "'),";
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
	query << "`direction` = " << static_cast<uint16_t>(player->getDirection()) << ',';

	if (!player->isOffline()) {
		query << "`onlinetime` = `onlinetime` + " << (time(nullptr) - player->lastLoginSaved) << ',';
	}

	query << "`blessings` = " << player->blessings.to_ulong() << ',';

	// verify open containers
	const auto& openContainers = player->getOpenContainers();

	for (int32_t slotId = CONST_SLOT_FIRST; slotId <= CONST_SLOT_LAST; ++slotId) {
		Item* item = player->inventory[slotId];
		if (!item) {
			continue;
		}

		verifyOpenContainers(item->getContainer(), openContainers);
	}

	for (const auto& it : player->depotChests) {
		for (auto depotIt = it.second->getReversedItems(), end = it.second->getReversedEnd(); depotIt != end;
		     ++depotIt) {
			Item* item = (*depotIt);
			if (!item) {
				continue;
			}

			verifyOpenContainers(item->getContainer(), openContainers);
		}
	}

	for (auto it = player->inbox->getReversedItems(), end = player->inbox->getReversedEnd(); it != end; ++it) {
		Item* item = (*it);
		if (!item) {
			continue;
		}

		verifyOpenContainers(item->getContainer(), openContainers);
	}

	for (auto it = player->storeInbox->getReversedItems(), end = player->storeInbox->getReversedEnd(); it != end;
	     ++it) {
		Item* item = (*it);
		if (!item) {
			continue;
		}

		verifyOpenContainers(item->getContainer(), openContainers);
	}

	// save inventory items
	query << "`inventory_items` = " << saveInventoryItems(db, player) << ',';

	// save depot items
	query << "`depot_items` = " << saveDepotItems(db, player) << ',';

	// save inbox items
	query << "`inbox_items` = " << saveInboxItems(db, player) << ',';

	// save storeinbox items
	query << "`storeinbox_items` = " << saveStoreInboxItems(db, player);

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

	// storages
	if (!db.executeQuery(fmt::format("DELETE FROM `player_storage` WHERE `player_id` = {:d}", player->getGUID()))) {
		return false;
	}

	DBInsert storageQuery("INSERT INTO `player_storage` (`player_id`, `key`, `value`) VALUES ");
	player->genReservedStorageRange();

	for (const auto& it : player->storageMap) {
		if (!storageQuery.addRow(fmt::format("{:d}, {:d}, {:d}", player->getGUID(), it.first, it.second))) {
			return false;
		}
	}

	if (!storageQuery.execute()) {
		return false;
	}

	// End the transaction
	return transaction.commit();
}

void IOLoginData::verifyOpenContainers(Container* container, const std::map<uint8_t, OpenContainer>& list)
{
	if (!container) {
		return;
	}

	if (container->getIntAttr(ITEM_ATTRIBUTE_OPENCONTAINER)) {
		container->setIntAttr(ITEM_ATTRIBUTE_OPENCONTAINER, 0);
	}

	for (const auto& it : list) {
		if (it.second.container == container) {
			container->setIntAttr(ITEM_ATTRIBUTE_OPENCONTAINER, it.first + 1);
			break;
		}
	}

	for (Item* item : container->getItemList()) {
		if (!item) {
			continue;
		}

		Container* container = item->getContainer();
		if (!container) {
			continue;
		}

		verifyOpenContainers(container, list);
	}
}

std::string IOLoginData::saveInventoryItems(Database& db, const Player* player)
{
	PropWriteStream propWriteStream;

	for (int32_t slotId = CONST_SLOT_FIRST; slotId <= CONST_SLOT_LAST; ++slotId) {
		Item* item = player->inventory[slotId];
		if (item) {
			propWriteStream.write<uint32_t>(slotId);
			saveBinaryItem(item, propWriteStream);
		}
	}

	size_t size;
	const char* items = propWriteStream.getStream(size);

	return db.escapeBlob(items, size);
}

std::string IOLoginData::saveDepotItems(Database& db, const Player* player)
{
	PropWriteStream propWriteStream;

	for (const auto& it : player->depotChests) {
		DepotChest* depotChest = it.second;
		propWriteStream.write<uint32_t>(it.first);
		propWriteStream.write<uint32_t>(depotChest->size());

		for (auto depotIt = depotChest->getReversedItems(), end = depotChest->getReversedEnd(); depotIt != end;
		     ++depotIt) {
			Item* item = (*depotIt);
			saveBinaryItem(item, propWriteStream);
		}
	}

	size_t size;
	const char* items = propWriteStream.getStream(size);

	return db.escapeBlob(items, size);
}

std::string IOLoginData::saveInboxItems(Database& db, const Player* player)
{
	PropWriteStream propWriteStream;
	propWriteStream.write<uint32_t>(player->inbox->size());

	for (auto inboxIt = player->inbox->getReversedItems(), end = player->inbox->getReversedEnd(); inboxIt != end;
	     ++inboxIt) {
		Item* item = (*inboxIt);
		saveBinaryItem(item, propWriteStream);
	}

	size_t size;
	const char* items = propWriteStream.getStream(size);

	return db.escapeBlob(items, size);
}

std::string IOLoginData::saveStoreInboxItems(Database& db, const Player* player)
{
	PropWriteStream propWriteStream;
	propWriteStream.write<uint32_t>(player->storeInbox->size());

	for (auto inboxIt = player->storeInbox->getReversedItems(), end = player->storeInbox->getReversedEnd();
	     inboxIt != end; ++inboxIt) {
		Item* item = (*inboxIt);
		saveBinaryItem(item, propWriteStream);
	}

	size_t size;
	const char* items = propWriteStream.getStream(size);

	return db.escapeBlob(items, size);
}

void IOLoginData::saveBinaryItem(Item* item, PropWriteStream& propWriteStream)
{
	if (!item) {
		return;
	}

	propWriteStream.write<uint16_t>(item->getID());
	propWriteStream.write<uint16_t>(item->getSubType());
	item->serializeAttr(propWriteStream);
	propWriteStream.write<uint8_t>(0x00); // attr end

	if (Container* container = item->getContainer()) {
		propWriteStream.write<uint32_t>(container->size());

		for (auto it = container->getReversedItems(), end = container->getReversedEnd(); it != end; ++it) {
			item = (*it);
			if (!item) {
				return;
			}

			saveBinaryItem(item, propWriteStream);
		}
	}
}

std::string IOLoginData::getNameByGuid(uint32_t guid)
{
	DBResult_ptr result =
	    Database::getInstance().storeQuery(fmt::format("SELECT `name` FROM `players` WHERE `id` = {:d}", guid));
	if (!result) {
		return std::string();
	}
	return result->getString("name");
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
