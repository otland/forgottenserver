// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "player.h"

#include "bed.h"
#include "chat.h"
#include "combat.h"
#include "configmanager.h"
#include "creatureevent.h"
#include "depotchest.h"
#include "events.h"
#include "game.h"
#include "iologindata.h"
#include "monster.h"
#include "movement.h"
#include "npc.h"
#include "outfit.h"
#include "party.h"
#include "scheduler.h"
#include "spectators.h"
#include "storeinbox.h"
#include "tools.h"
#include "weapons.h"

extern Game g_game;
extern Chat* g_chat;
extern Vocations g_vocations;
extern MoveEvents* g_moveEvents;
extern Weapons* g_weapons;
extern CreatureEvents* g_creatureEvents;

MuteCountMap Player::muteCountMap;

uint32_t Player::playerAutoID = 0x10000000;
uint32_t Player::playerIDLimit = 0x20000000;

Player::Player(ProtocolGame_ptr p) :
    Creature(),
    lastPing(OTSYS_TIME()),
    lastPong(lastPing),
    client(std::move(p)),
    storeInbox(std::make_shared<StoreInbox>(ITEM_STORE_INBOX))
{
	// storeInbox->setParent(shared_from_this());
}

Player::~Player()
{
	for (const auto& item : inventory) {
		if (item) {
			item->setParent(nullptr);
		}
	}

	for (const auto& [_, depot] : depotChests) {
		if (auto parent = depot->getRealParent()) {
			parent->internalRemoveThing(depot->getItem());
		}
	}

	if (depotLocker) {
		depotLocker->removeInbox(inbox);
	}

	storeInbox->setParent(nullptr);

	setWriteItem(nullptr);
	setEditHouse(nullptr);
}

void Player::setID()
{
	if (id == 0) {
		// allowClones id assignment
		if (getBoolean(ConfigManager::ALLOW_CLONES)) {
			id = playerAutoID++;
			return;
		}

		// normal id assignment
		if (guid != 0) {
			id = playerAutoID + guid;
		}
	}
}

bool Player::setVocation(uint16_t vocId)
{
	Vocation* voc = g_vocations.getVocation(vocId);
	if (!voc) {
		return false;
	}
	vocation = voc;

	updateRegeneration();
	setBaseSpeed(voc->getBaseSpeed());
	updateBaseSpeed();
	g_game.changeSpeed(getPlayer(), 0);
	return true;
}

bool Player::isPushable() const
{
	if (hasFlag(PlayerFlag_CannotBePushed)) {
		return false;
	}
	return Creature::isPushable();
}

std::string Player::getDescription(int32_t lookDistance) const
{
	std::ostringstream s;

	if (lookDistance == -1) {
		s << "yourself.";

		if (group->access) {
			s << " You are " << group->name << '.';
		} else if (vocation->getId() != VOCATION_NONE) {
			s << " You are " << vocation->getVocDescription() << '.';
		} else {
			s << " You have no vocation.";
		}
	} else {
		s << name;
		if (!group->access) {
			s << " (Level " << level << ')';
		}
		s << '.';

		if (sex == PLAYERSEX_FEMALE) {
			s << " She";
		} else {
			s << " He";
		}

		if (group->access) {
			s << " is " << group->name << '.';
		} else if (vocation->getId() != VOCATION_NONE) {
			s << " is " << vocation->getVocDescription() << '.';
		} else {
			s << " has no vocation.";
		}
	}

	if (party) {
		if (lookDistance == -1) {
			s << " Your party has ";
		} else if (sex == PLAYERSEX_FEMALE) {
			s << " She is in a party with ";
		} else {
			s << " He is in a party with ";
		}

		size_t memberCount = party->getMemberCount() + 1;
		if (memberCount == 1) {
			s << "1 member and ";
		} else {
			s << memberCount << " members and ";
		}

		size_t invitationCount = party->getInvitationCount();
		if (invitationCount == 1) {
			s << "1 pending invitation.";
		} else {
			s << invitationCount << " pending invitations.";
		}
	}

	if (!guild || !guildRank) {
		return s.str();
	}

	if (lookDistance == -1) {
		s << " You are ";
	} else if (sex == PLAYERSEX_FEMALE) {
		s << " She is ";
	} else {
		s << " He is ";
	}

	s << guildRank->name << " of the " << guild->getName();
	if (!guildNick.empty()) {
		s << " (" << guildNick << ')';
	}

	size_t memberCount = guild->getMemberCount();
	if (memberCount == 1) {
		s << ", which has 1 member, " << guild->getMembersOnline().size() << " of them online.";
	} else {
		s << ", which has " << memberCount << " members, " << guild->getMembersOnline().size() << " of them online.";
	}
	return s.str();
}

std::shared_ptr<Item> Player::getInventoryItem(slots_t slot) const
{
	if (slot < CONST_SLOT_FIRST || slot > CONST_SLOT_LAST) {
		return nullptr;
	}
	return inventory[slot];
}

std::shared_ptr<Item> Player::getWeapon(slots_t slot, bool ignoreAmmo) const
{
	auto item = inventory[slot];
	if (!item) {
		return nullptr;
	}

	WeaponType_t weaponType = item->getWeaponType();
	if (weaponType == WEAPON_NONE || weaponType == WEAPON_SHIELD || weaponType == WEAPON_AMMO ||
	    weaponType == WEAPON_QUIVER) {
		return nullptr;
	}

	if (!ignoreAmmo && weaponType == WEAPON_DISTANCE) {
		const ItemType& itemType = Item::items[item->getID()];
		if (itemType.ammoType != AMMO_NONE) {
			auto ammoItem = inventory[CONST_SLOT_AMMO];
			if (!ammoItem || ammoItem->getAmmoType() != itemType.ammoType) {
				// no ammo item was found, search for quiver instead
				auto quiver = inventory[CONST_SLOT_RIGHT] ? inventory[CONST_SLOT_RIGHT]->getContainer() : nullptr;
				if (!quiver || quiver->getWeaponType() != WEAPON_QUIVER) {
					// no quiver equipped
					return nullptr;
				}

				for (ContainerIterator containerItem = quiver->iterator(); containerItem.hasNext();
				     containerItem.advance()) {
					if (itemType.ammoType == (*containerItem)->getAmmoType()) {
						auto weapon = g_weapons->getWeapon(*containerItem);
						if (weapon && weapon->ammoCheck(getPlayer())) {
							return *containerItem;
						}
					}
				}

				// no valid ammo was found in quiver
				return nullptr;
			}
			item = ammoItem;
		}
	}
	return item;
}

std::shared_ptr<Item> Player::getWeapon(bool ignoreAmmo /* = false*/) const
{
	auto item = getWeapon(CONST_SLOT_LEFT, ignoreAmmo);
	if (item) {
		return item;
	}

	item = getWeapon(CONST_SLOT_RIGHT, ignoreAmmo);
	if (item) {
		return item;
	}
	return nullptr;
}

WeaponType_t Player::getWeaponType() const
{
	auto item = getWeapon();
	if (!item) {
		return WEAPON_NONE;
	}
	return item->getWeaponType();
}

int32_t Player::getWeaponSkill(const std::shared_ptr<const Item>& item) const
{
	if (!item) {
		return getSkillLevel(SKILL_FIST);
	}

	int32_t attackSkill;

	WeaponType_t weaponType = item->getWeaponType();
	switch (weaponType) {
		case WEAPON_SWORD: {
			attackSkill = getSkillLevel(SKILL_SWORD);
			break;
		}

		case WEAPON_CLUB: {
			attackSkill = getSkillLevel(SKILL_CLUB);
			break;
		}

		case WEAPON_AXE: {
			attackSkill = getSkillLevel(SKILL_AXE);
			break;
		}

		case WEAPON_DISTANCE: {
			attackSkill = getSkillLevel(SKILL_DISTANCE);
			break;
		}

		default: {
			attackSkill = 0;
			break;
		}
	}
	return attackSkill;
}

int32_t Player::getArmor() const
{
	int32_t armor = 0;

	static const slots_t armorSlots[] = {CONST_SLOT_HEAD, CONST_SLOT_NECKLACE, CONST_SLOT_ARMOR,
	                                     CONST_SLOT_LEGS, CONST_SLOT_FEET,     CONST_SLOT_RING};
	for (slots_t slot : armorSlots) {
		auto inventoryItem = inventory[slot];
		if (inventoryItem) {
			armor += inventoryItem->getArmor();
		}
	}
	return static_cast<int32_t>(armor * vocation->armorMultiplier);
}

std::pair<std::shared_ptr<const Item>, std::shared_ptr<const Item>> Player::getShieldAndWeapon() const
{
	std::shared_ptr<const Item> shield = nullptr;
	std::shared_ptr<const Item> weapon = nullptr;

	for (uint32_t slot = CONST_SLOT_RIGHT; slot <= CONST_SLOT_LEFT; slot++) {
		auto item = inventory[slot];
		if (!item) {
			continue;
		}

		switch (item->getWeaponType()) {
			case WEAPON_NONE:
				break;

			case WEAPON_SHIELD:
			case WEAPON_QUIVER: {
				if (!shield || item->getDefense() > shield->getDefense()) {
					shield = item;
				}
				break;
			}

			default: { // weapons that are not shields
				weapon = item;
				break;
			}
		}
	}

	return std::make_pair(shield, weapon);
}

int32_t Player::getDefense() const
{
	int32_t defenseSkill = getSkillLevel(SKILL_FIST);
	int32_t defenseValue = 7;
	const auto& [shield, weapon] = getShieldAndWeapon();

	if (weapon) {
		defenseValue = weapon->getDefense() + weapon->getExtraDefense();
		defenseSkill = getWeaponSkill(weapon);
	}

	if (shield) {
		defenseValue = weapon ? shield->getDefense() + weapon->getExtraDefense() : shield->getDefense();
		defenseSkill = getSkillLevel(SKILL_SHIELD);
	}

	if (defenseSkill == 0) {
		switch (fightMode) {
			case FIGHTMODE_ATTACK:
			case FIGHTMODE_BALANCED:
				return 1;

			case FIGHTMODE_DEFENSE:
				return 2;
		}
	}

	return (defenseSkill / 4. + 2.23) * defenseValue * 0.15 * getDefenseFactor() * vocation->defenseMultiplier;
}

uint32_t Player::getAttackSpeed() const
{
	auto weapon = getWeapon(true);
	if (!weapon || weapon->getAttackSpeed() == 0) {
		return vocation->getAttackSpeed();
	}

	return weapon->getAttackSpeed();
}

float Player::getAttackFactor() const
{
	switch (fightMode) {
		case FIGHTMODE_ATTACK:
			return 1.0f;
		case FIGHTMODE_BALANCED:
			return 1.2f;
		case FIGHTMODE_DEFENSE:
			return 2.0f;
		default:
			return 1.0f;
	}
}

float Player::getDefenseFactor() const
{
	switch (fightMode) {
		case FIGHTMODE_ATTACK:
			return (OTSYS_TIME() - lastAttack) < getAttackSpeed() ? 0.5f : 1.0f;
		case FIGHTMODE_BALANCED:
			return (OTSYS_TIME() - lastAttack) < getAttackSpeed() ? 0.75f : 1.0f;
		case FIGHTMODE_DEFENSE:
			return 1.0f;
		default:
			return 1.0f;
	}
}

uint32_t Player::getClientIcons() const
{
	uint32_t icons = 0;
	for (Condition* condition : conditions) {
		if (!isSuppress(condition->getType())) {
			icons |= condition->getIcons();
		}
	}

	if (pzLocked) {
		icons |= ICON_REDSWORDS;
	}

	if (tile && tile->hasFlag(TILESTATE_PROTECTIONZONE)) {
		icons |= ICON_PIGEON;

		// Don't show ICON_SWORDS if player is in protection zone.
		icons &= ~ICON_SWORDS;
	}

	return icons;
}

void Player::updateInventoryWeight()
{
	if (hasFlag(PlayerFlag_HasInfiniteCapacity)) {
		return;
	}

	inventoryWeight = 0;
	for (int i = CONST_SLOT_FIRST; i <= CONST_SLOT_LAST; ++i) {
		auto item = inventory[i];
		if (item) {
			inventoryWeight += item->getWeight();
		}
	}

	if (auto storeInbox = getStoreInbox()) {
		inventoryWeight += storeInbox->getWeight();
	}
}

void Player::addSkillAdvance(skills_t skill, uint64_t count)
{
	uint64_t currReqTries = vocation->getReqSkillTries(skill, skills[skill].level);
	uint64_t nextReqTries = vocation->getReqSkillTries(skill, skills[skill].level + 1);
	if (currReqTries >= nextReqTries) {
		// player has reached max skill
		return;
	}

	tfs::events::player::onGainSkillTries(getPlayer(), skill, count);
	if (count == 0) {
		return;
	}

	bool sendUpdateSkills = false;
	while ((skills[skill].tries + count) >= nextReqTries) {
		count -= nextReqTries - skills[skill].tries;
		skills[skill].level++;
		skills[skill].tries = 0;
		skills[skill].percent = 0;

		sendTextMessage(MESSAGE_EVENT_ADVANCE,
		                fmt::format("You advanced to {:s} level {:d}.", getSkillName(skill), skills[skill].level));

		g_creatureEvents->playerAdvance(getPlayer(), skill, (skills[skill].level - 1), skills[skill].level);

		sendUpdateSkills = true;
		currReqTries = nextReqTries;
		nextReqTries = vocation->getReqSkillTries(skill, skills[skill].level + 1);
		if (currReqTries >= nextReqTries) {
			count = 0;
			break;
		}
	}

	skills[skill].tries += count;

	uint32_t newPercent;
	if (nextReqTries > currReqTries) {
		newPercent = Player::getBasisPointLevel(skills[skill].tries, nextReqTries);
	} else {
		newPercent = 0;
	}

	if (skills[skill].percent != newPercent) {
		skills[skill].percent = newPercent;
		sendUpdateSkills = true;
	}

	if (sendUpdateSkills) {
		sendSkills();
	}
}

void Player::removeSkillTries(skills_t skill, uint64_t count, bool notify /* = false*/)
{
	uint16_t oldLevel = skills[skill].level;
	uint8_t oldPercent = skills[skill].percent;

	while (count > skills[skill].tries) {
		count -= skills[skill].tries;

		if (skills[skill].level <= MINIMUM_SKILL_LEVEL) {
			skills[skill].level = MINIMUM_SKILL_LEVEL;
			skills[skill].tries = 0;
			count = 0;
			break;
		}

		skills[skill].tries = vocation->getReqSkillTries(skill, skills[skill].level);
		skills[skill].level--;
	}

	skills[skill].tries = std::max<int32_t>(0, skills[skill].tries - count);
	skills[skill].percent =
	    Player::getBasisPointLevel(skills[skill].tries, vocation->getReqSkillTries(skill, skills[skill].level));

	if (notify) {
		bool sendUpdateSkills = false;
		if (oldLevel != skills[skill].level) {
			sendTextMessage(MESSAGE_EVENT_ADVANCE, fmt::format("You were downgraded to {:s} level {:d}.",
			                                                   getSkillName(skill), skills[skill].level));
			sendUpdateSkills = true;
		}

		if (sendUpdateSkills || oldPercent != skills[skill].percent) {
			sendSkills();
		}
	}
}

void Player::setVarStats(stats_t stat, int32_t modifier)
{
	varStats[stat] += modifier;

	switch (stat) {
		case STAT_MAXHITPOINTS: {
			if (getHealth() > getMaxHealth()) {
				Creature::changeHealth(getMaxHealth() - getHealth());
			} else {
				g_game.addCreatureHealth(getPlayer());
			}
			break;
		}

		case STAT_MAXMANAPOINTS: {
			if (getMana() > getMaxMana()) {
				changeMana(getMaxMana() - getMana());
			}
			break;
		}

		default: {
			break;
		}
	}
}

int32_t Player::getDefaultStats(stats_t stat) const
{
	switch (stat) {
		case STAT_MAXHITPOINTS:
			return healthMax;
		case STAT_MAXMANAPOINTS:
			return manaMax;
		case STAT_MAGICPOINTS:
			return getBaseMagicLevel();
		default:
			return 0;
	}
}

void Player::addContainer(uint8_t cid, const std::shared_ptr<Container>& container)
{
	if (cid > 0xF) {
		return;
	}

	auto it = openContainers.find(cid);
	if (it != openContainers.end()) {
		OpenContainer& openContainer = it->second;
		auto oldContainer = openContainer.container;
		if (oldContainer->getID() == ITEM_BROWSEFIELD) {
			// previous browse field ownership is stored in Game::browseFieldOwners; nothing to do here
		}

		openContainer.container = container;
		openContainer.index = 0;
	} else {
		OpenContainer openContainer;
		openContainer.container = container;
		openContainer.index = 0;
		openContainers[cid] = openContainer;
	}
}

void Player::closeContainer(uint8_t cid)
{
	auto it = openContainers.find(cid);
	if (it == openContainers.end()) {
		return;
	}

	OpenContainer openContainer = it->second;
	auto container = openContainer.container;
	openContainers.erase(it);

	if (container && container->getID() == ITEM_BROWSEFIELD) {
		// when a player closes a browse field container, the global owner will be cleared by
		// Game::decreaseBrowseFieldRef
	}
}

void Player::setContainerIndex(uint8_t cid, uint16_t index)
{
	auto it = openContainers.find(cid);
	if (it == openContainers.end()) {
		return;
	}
	it->second.index = index;
}

std::shared_ptr<Container> Player::getContainerByID(uint8_t cid)
{
	auto it = openContainers.find(cid);
	if (it == openContainers.end()) {
		return nullptr;
	}
	return it->second.container;
}

int8_t Player::getContainerID(const std::shared_ptr<const Container>& container) const
{
	for (const auto& it : openContainers) {
		if (it.second.container == container) {
			return it.first;
		}
	}
	return -1;
}

uint16_t Player::getContainerIndex(uint8_t cid) const
{
	auto it = openContainers.find(cid);
	if (it == openContainers.end()) {
		return 0;
	}
	return it->second.index;
}

bool Player::canOpenCorpse(uint32_t ownerId) const
{
	return getID() == ownerId || (party && party->canOpenCorpse(ownerId));
}

uint16_t Player::getLookCorpse() const
{
	if (sex == PLAYERSEX_FEMALE) {
		return ITEM_FEMALE_CORPSE;
	}
	return ITEM_MALE_CORPSE;
}

void Player::setStorageValue(uint32_t key, std::optional<int32_t> value, bool isSpawn /* = false*/)
{
	if (IS_IN_KEYRANGE(key, RESERVED_RANGE)) {
		std::cout << "Warning: unknown reserved key: " << key << " player: " << getName() << std::endl;
		return;
	}

	Creature::setStorageValue(key, value, isSpawn);
}

bool Player::canSee(const Position& pos) const
{
	if (!client) {
		return false;
	}
	return client->canSee(pos);
}

bool Player::canSeeCreature(const std::shared_ptr<const Creature>& creature) const
{
	if (creature.get() == this) {
		return true;
	}

	if (creature->isInGhostMode() && !canSeeGhostMode(creature)) {
		return false;
	}

	if (!creature->getPlayer() && !canSeeInvisibility() && creature->isInvisible()) {
		return false;
	}
	return true;
}

bool Player::canSeeGhostMode(const std::shared_ptr<const Creature>&) const { return group->access; }

bool Player::canWalkthrough(const std::shared_ptr<const Creature>& creature) const
{
	if (group->access || creature->isInGhostMode()) {
		return true;
	}

	auto player = creature->getPlayer();
	if (!player || !getBoolean(ConfigManager::ALLOW_WALKTHROUGH)) {
		return false;
	}

	auto playerTile = player->getTile();
	if (!playerTile || (!playerTile->hasFlag(TILESTATE_PROTECTIONZONE) &&
	                    player->getLevel() > static_cast<uint32_t>(getNumber(ConfigManager::PROTECTION_LEVEL)))) {
		return false;
	}

	auto playerTileGround = playerTile->getGround();
	if (!playerTileGround || !playerTileGround->hasWalkStack()) {
		return false;
	}

	auto thisPlayer = const_cast<Player*>(this);
	if ((OTSYS_TIME() - lastWalkthroughAttempt) > 2000) {
		thisPlayer->setLastWalkthroughAttempt(OTSYS_TIME());
		return false;
	}

	if (creature->getPosition() != lastWalkthroughPosition) {
		thisPlayer->setLastWalkthroughPosition(creature->getPosition());
		return false;
	}

	thisPlayer->setLastWalkthroughPosition(creature->getPosition());
	return true;
}

bool Player::canWalkthroughEx(const std::shared_ptr<const Creature>& creature) const
{
	if (group->access) {
		return true;
	}

	auto player = creature->getPlayer();
	if (!player || !getBoolean(ConfigManager::ALLOW_WALKTHROUGH)) {
		return false;
	}

	auto playerTile = player->getTile();
	return playerTile && (playerTile->hasFlag(TILESTATE_PROTECTIONZONE) ||
	                      player->getLevel() <= static_cast<uint32_t>(getNumber(ConfigManager::PROTECTION_LEVEL)));
}

void Player::onReceiveMail() const
{
	if (isNearDepotBox()) {
		sendTextMessage(MESSAGE_EVENT_ADVANCE, "New mail has arrived.");
	}
}

bool Player::isNearDepotBox() const
{
	const Position& pos = getPosition();
	for (int32_t cx = -NOTIFY_DEPOT_BOX_RANGE; cx <= NOTIFY_DEPOT_BOX_RANGE; ++cx) {
		for (int32_t cy = -NOTIFY_DEPOT_BOX_RANGE; cy <= NOTIFY_DEPOT_BOX_RANGE; ++cy) {
			auto tile = g_game.map.getTile(pos.x + cx, pos.y + cy, pos.z);
			if (!tile) {
				continue;
			}

			if (tile->hasFlag(TILESTATE_DEPOT)) {
				return true;
			}
		}
	}
	return false;
}

std::shared_ptr<DepotChest> Player::getDepotChest(uint32_t depotId, bool autoCreate)
{
	auto it = depotChests.find(depotId);
	if (it != depotChests.end()) {
		return it->second;
	}

	if (!autoCreate) {
		return nullptr;
	}

	uint16_t depotItemId = getDepotBoxId(depotId);
	if (depotItemId == 0) {
		return nullptr;
	}

	const std::shared_ptr<DepotChest>& depotChest =
	    depotChests.emplace(depotId, std::make_shared<DepotChest>(depotItemId)).first->second;
	depotChest->setMaxDepotItems(getMaxDepotItems());
	return depotChest;
}

std::shared_ptr<DepotLocker> Player::getDepotLocker()
{
	if (!depotLocker) {
		depotLocker = std::make_shared<DepotLocker>(ITEM_LOCKER);
		depotLocker->internalAddThing(Item::CreateItem(ITEM_MARKET));
		depotLocker->internalAddThing(getInbox()->getItem());

		auto depotChest = std::make_shared<DepotChest>(ITEM_DEPOT, false);
		// adding in reverse to align them from first to last
		for (int16_t depotId = depotChest->capacity(); depotId >= 0; --depotId) {
			if (const auto& box = getDepotChest(depotId, true)) {
				depotChest->internalAddThing(box->getItem());
			}
		}

		depotLocker->internalAddThing(depotChest->getItem());
	}
	return depotLocker;
}

void Player::sendCancelMessage(ReturnValue message) const { sendCancelMessage(getReturnMessage(message)); }

void Player::sendStats()
{
	if (client) {
		client->sendStats();
		lastStatsTrainingTime = getOfflineTrainingTime() / 60 / 1000;
	}
}

void Player::sendPing()
{
	int64_t timeNow = OTSYS_TIME();

	bool hasLostConnection = false;
	if ((timeNow - lastPing) >= 5000) {
		lastPing = timeNow;
		if (client) {
			client->sendPing();
		} else {
			hasLostConnection = true;
		}
	}

	int64_t noPongTime = timeNow - lastPong;
	if ((hasLostConnection || noPongTime >= 7000) && attackedCreature && attackedCreature->getPlayer()) {
		removeAttackedCreature();
	}

	int32_t noPongKickTime = vocation->getNoPongKickTime();
	if (pzLocked && noPongKickTime < 60000) {
		noPongKickTime = 60000;
	}

	if (noPongTime >= noPongKickTime) {
		if (isConnecting || getTile()->hasFlag(TILESTATE_NOLOGOUT)) {
			return;
		}

		if (!g_creatureEvents->playerLogout(getPlayer())) {
			return;
		}

		if (client) {
			client->logout(true, true);
		} else {
			g_game.removeCreature(getPlayer(), true);
		}
	}
}

std::shared_ptr<Item> Player::getWriteItem(uint32_t& windowTextId, uint16_t& maxWriteLen)
{
	windowTextId = this->windowTextId;
	maxWriteLen = this->maxWriteLen;
	return writeItem;
}

void Player::setWriteItem(const std::shared_ptr<Item>& item, uint16_t maxWriteLen /*= 0*/)
{
	windowTextId++;

	if (item) {
		writeItem = item;
		this->maxWriteLen = maxWriteLen;
	} else {
		writeItem = nullptr;
		this->maxWriteLen = 0;
	}
}

House* Player::getEditHouse(uint32_t& windowTextId, uint32_t& listId)
{
	windowTextId = this->windowTextId;
	listId = this->editListId;
	return editHouse;
}

void Player::setEditHouse(House* house, uint32_t listId /*= 0*/)
{
	windowTextId++;
	editHouse = house;
	editListId = listId;
}

void Player::sendHouseWindow(House* house, uint32_t listId) const
{
	if (!client) {
		return;
	}

	std::string text;
	if (house->getAccessList(listId, text)) {
		client->sendHouseWindow(windowTextId, text);
	}
}

// container
void Player::sendAddContainerItem(const std::shared_ptr<const Container>& container,
                                  const std::shared_ptr<const Item>& item)
{
	if (!client) {
		return;
	}

	auto slotItem = item;
	for (const auto& it : openContainers) {
		const OpenContainer& openContainer = it.second;
		if (openContainer.container != container) {
			continue;
		}

		uint16_t slot = openContainer.index;
		if (container->getID() == ITEM_BROWSEFIELD) {
			uint16_t containerSize = container->size() - 1;
			uint16_t pageEnd = openContainer.index + container->capacity() - 1;
			if (containerSize > pageEnd) {
				slot = pageEnd;
				slotItem = container->getItemByIndex(pageEnd);
			} else {
				slot = containerSize;
			}
		} else if (openContainer.index >= container->capacity()) {
			slotItem = container->getItemByIndex(openContainer.index);
		}

		if (slotItem) {
			client->sendAddContainerItem(it.first, slot, slotItem);
		}
	}
}

void Player::sendUpdateContainerItem(const std::shared_ptr<const Container>& container, uint16_t slot,
                                     const std::shared_ptr<const Item>& newItem)
{
	if (!client) {
		return;
	}

	for (const auto& it : openContainers) {
		const OpenContainer& openContainer = it.second;
		if (openContainer.container != container) {
			continue;
		}

		if (slot < openContainer.index) {
			continue;
		}

		uint16_t pageEnd = openContainer.index + container->capacity();
		if (slot >= pageEnd) {
			continue;
		}

		client->sendUpdateContainerItem(it.first, slot, newItem);
	}
}

void Player::sendRemoveContainerItem(const std::shared_ptr<const Container>& container, uint16_t slot)
{
	if (!client) {
		return;
	}

	for (auto& it : openContainers) {
		OpenContainer& openContainer = it.second;
		if (openContainer.container != container) {
			continue;
		}

		uint16_t& firstIndex = openContainer.index;
		if (firstIndex > 0 && firstIndex >= container->size() - 1) {
			firstIndex -= container->capacity();
			sendContainer(it.first, container, firstIndex);
		}

		client->sendRemoveContainerItem(it.first, std::max<uint16_t>(slot, firstIndex),
		                                container->getItemByIndex(container->capacity() + firstIndex));
	}
}

void Player::openSavedContainers()
{
	std::map<uint8_t, std::shared_ptr<Container>> openContainersList;

	for (int32_t i = CONST_SLOT_FIRST; i <= CONST_SLOT_LAST; i++) {
		auto item = inventory[i];
		if (!item) {
			continue;
		}

		auto itemContainer = item->getContainer();
		if (itemContainer) {
			uint8_t cid = item->getIntAttr(ITEM_ATTRIBUTE_OPENCONTAINER);
			if (cid > 0) {
				openContainersList.emplace(cid, itemContainer);
			}
			for (ContainerIterator it = itemContainer->iterator(); it.hasNext(); it.advance()) {
				auto subContainer = (*it)->getContainer();
				if (subContainer) {
					uint8_t subcid = (*it)->getIntAttr(ITEM_ATTRIBUTE_OPENCONTAINER);
					if (subcid > 0) {
						openContainersList.emplace(subcid, subContainer);
					}
				}
			}
		}
	}

	// fix broken containers when logged in from another location
	for (uint8_t i = 0; i < 16; i++) {
		client->sendEmptyContainer(i);
		client->sendCloseContainer(i);
	}

	// send actual containers
	for (auto& [id, container] : openContainersList) {
		addContainer(id - 1, container);
		onSendContainer(container);
	}
}

void Player::onUpdateTileItem(const std::shared_ptr<const Tile>& tile, const Position& pos,
                              const std::shared_ptr<const Item>& oldItem, const ItemType& oldType,
                              const std::shared_ptr<const Item>& newItem, const ItemType& newType)
{
	Creature::onUpdateTileItem(tile, pos, oldItem, oldType, newItem, newType);

	if (oldItem != newItem) {
		onRemoveTileItem(tile, pos, oldType, oldItem);
	}

	if (tradeState != TRADE_TRANSFER) {
		if (tradeItem && oldItem == tradeItem) {
			g_game.internalCloseTrade(getPlayer());
		}
	}
}

void Player::onRemoveTileItem(const std::shared_ptr<const Tile>& tile, const Position& pos, const ItemType& iType,
                              const std::shared_ptr<const Item>& item)
{
	Creature::onRemoveTileItem(tile, pos, iType, item);

	if (tradeState != TRADE_TRANSFER) {
		checkTradeState(item);

		if (tradeItem) {
			const std::shared_ptr<const Container>& container = item->getContainer();
			if (container && container->isHoldingItem(tradeItem)) {
				g_game.internalCloseTrade(getPlayer());
			}
		}
	}
}

void Player::onCreatureAppear(const std::shared_ptr<Creature>& creature, bool isLogin)
{
	Creature::onCreatureAppear(creature, isLogin);

	if (isLogin && creature.get() == this) {
		sendItems();
		onEquipInventory();

		for (Condition* condition : storedConditionList) {
			addCondition(condition);
		}
		storedConditionList.clear();

		updateRegeneration();

		auto bed = g_game.getBedBySleeper(guid);
		if (bed) {
			bed->wakeUp(getPlayer());
		}

		// load mount speed bonus
		uint16_t currentMountId = currentOutfit.lookMount;
		if (currentMountId != 0) {
			Mount* currentMount = g_game.mounts.getMountByClientID(currentMountId);
			if (currentMount && hasMount(currentMount)) {
				g_game.changeSpeed(getPlayer(), currentMount->speed);
			} else {
				defaultOutfit.lookMount = 0;
				g_game.internalCreatureChangeOutfit(getPlayer(), defaultOutfit);
			}
		}

		// mounted player moved to pz on login, update mount status
		onChangeZone(getZone());

		if (guild) {
			guild->addMember(getPlayer());
		}

		int32_t offlineTime;
		if (getLastLogout() != 0) {
			// Not counting more than 21 days to prevent overflow when multiplying with 1000 (for milliseconds).
			offlineTime = std::min<int32_t>(time(nullptr) - getLastLogout(), 86400 * 21);
		} else {
			offlineTime = 0;
		}

		for (Condition* condition : getMuteConditions()) {
			condition->setTicks(condition->getTicks() - (offlineTime * 1000));
			if (condition->getTicks() <= 0) {
				removeCondition(condition);
			}
		}

		g_game.checkPlayersRecord();
		IOLoginData::updateOnlineStatus(guid, true);
	}
}

void Player::onAttackedCreatureDisappear(bool isLogout)
{
	sendCancelTarget();

	if (!isLogout) {
		sendTextMessage(MESSAGE_STATUS_SMALL, "Target lost.");
	}
}

void Player::onFollowCreatureDisappear(bool isLogout)
{
	sendCancelTarget();

	if (!isLogout) {
		sendTextMessage(MESSAGE_STATUS_SMALL, "Target lost.");
	}
}

void Player::onChangeZone(ZoneType_t zone)
{
	if (zone == ZONE_PROTECTION) {
		if (attackedCreature && !hasFlag(PlayerFlag_IgnoreProtectionZone)) {
			removeAttackedCreature();
			onAttackedCreatureDisappear(false);
		}

		if (!group->access && isMounted()) {
			dismount();
			g_game.internalCreatureChangeOutfit(getPlayer(), defaultOutfit);
			wasMounted = true;
		}
	} else {
		if (wasMounted) {
			toggleMount(true);
			wasMounted = false;
		}
	}

	g_game.updateCreatureWalkthrough(getPlayer());
	sendIcons();
}

void Player::onAttackedCreatureChangeZone(ZoneType_t zone)
{
	if (zone == ZONE_PROTECTION) {
		if (!hasFlag(PlayerFlag_IgnoreProtectionZone)) {
			removeAttackedCreature();
			onAttackedCreatureDisappear(false);
		}
	} else if (zone == ZONE_NOPVP) {
		if (attackedCreature->getPlayer()) {
			if (!hasFlag(PlayerFlag_IgnoreProtectionZone)) {
				removeAttackedCreature();
				onAttackedCreatureDisappear(false);
			}
		}
	} else if (zone == ZONE_NORMAL) {
		// attackedCreature can leave a pvp zone if not pzlocked
		if (g_game.getWorldType() == WORLD_TYPE_NO_PVP) {
			if (attackedCreature->getPlayer()) {
				removeAttackedCreature();
				onAttackedCreatureDisappear(false);
			}
		}
	}
}

void Player::onRemoveCreature(const std::shared_ptr<Creature>& creature, bool isLogout)
{
	Creature::onRemoveCreature(creature, isLogout);

	if (creature.get() == this) {
		onDeEquipInventory();

		if (isLogout) {
			loginPosition = getPosition();
		}

		lastLogout = time(nullptr);

		if (eventWalk != 0) {
			removeFollowCreature();
		}

		if (tradePartner) {
			g_game.internalCloseTrade(getPlayer());
		}

		closeShopWindow();

		clearPartyInvitations();

		if (party) {
			party->leaveParty(getPlayer(), true);
		}

		g_chat->removeUserFromAllChannels(getPlayer());

		if (guild) {
			guild->removeMember(getPlayer());
		}

		IOLoginData::updateOnlineStatus(guid, false);

		bool saved = false;
		for (uint32_t tries = 0; tries < 3; ++tries) {
			if (IOLoginData::savePlayer(getPlayer())) {
				saved = true;
				break;
			}
		}

		if (!saved) {
			std::cout << "Error while saving player: " << getName() << std::endl;
		}
	}
}

void Player::openShopWindow(const std::shared_ptr<Npc>& npc, const std::list<ShopInfo>& shop)
{
	shopItemList = shop;
	sendShop(npc);
	sendSaleItemList();
}

bool Player::closeShopWindow(bool sendCloseShopWindow /*= true*/)
{
	// unreference callbacks
	int32_t onBuy;
	int32_t onSell;

	auto npc = getShopOwner(onBuy, onSell);
	if (!npc) {
		shopItemList.clear();
		return false;
	}

	setShopOwner(nullptr, -1, -1);
	npc->onPlayerEndTrade(getPlayer(), onBuy, onSell);

	if (sendCloseShopWindow) {
		sendCloseShop();
	}

	shopItemList.clear();
	return true;
}

void Player::onWalk(Direction& dir)
{
	Creature::onWalk(dir);
	setNextActionTask(nullptr);
	setNextAction(OTSYS_TIME() + getStepDuration(dir));
}

void Player::onCreatureMove(const std::shared_ptr<Creature>& creature, const std::shared_ptr<const Tile>& newTile,
                            const Position& newPos, const std::shared_ptr<const Tile>& oldTile, const Position& oldPos,
                            bool teleport)
{
	Creature::onCreatureMove(creature, newTile, newPos, oldTile, oldPos, teleport);

	if (hasFollowPath && (creature == followCreature || (creature.get() == this && followCreature))) {
		g_dispatcher.addTask([id = getID()]() { g_game.updateCreatureWalk(id); });
	}

	if (creature.get() != this) {
		return;
	}

	if (tradeState != TRADE_TRANSFER) {
		// check if we should close trade
		if (tradeItem && !tradeItem->getPosition().isInRange(getPosition(), 1, 1, 0)) {
			g_game.internalCloseTrade(getPlayer());
		}

		if (tradePartner && !tradePartner->getPosition().isInRange(getPosition(), 2, 2, 0)) {
			g_game.internalCloseTrade(getPlayer());
		}
	}

	// close modal windows
	if (!modalWindows.empty()) {
		// TODO: This shouldn't be hard-coded
		for (uint32_t modalWindowId : modalWindows) {
			if (modalWindowId == std::numeric_limits<uint32_t>::max()) {
				sendTextMessage(MESSAGE_EVENT_ADVANCE, "Offline training aborted.");
				break;
			}
		}
		modalWindows.clear();
	}

	// leave market
	if (inMarket) {
		inMarket = false;
	}

	if (party) {
		party->updateSharedExperience();
	}

	if (teleport || oldPos.z != newPos.z) {
		int32_t ticks = getNumber(ConfigManager::STAIRHOP_DELAY);
		if (ticks > 0) {
			if (Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_PACIFIED, ticks, 0)) {
				addCondition(condition);
			}
		}
	}
}

void Player::onEquipInventory()
{
	for (int32_t slot = CONST_SLOT_FIRST; slot <= CONST_SLOT_LAST; ++slot) {
		auto item = inventory[slot];
		if (item) {
			item->startDecaying();
			g_moveEvents->onPlayerEquip(getPlayer(), item, static_cast<slots_t>(slot), false);
			tfs::events::player::onInventoryUpdate(getPlayer(), item, static_cast<slots_t>(slot), true);
		}
	}
}

void Player::onDeEquipInventory()
{
	for (int32_t slot = CONST_SLOT_FIRST; slot <= CONST_SLOT_LAST; ++slot) {
		auto item = inventory[slot];
		if (item) {
			g_moveEvents->onPlayerDeEquip(getPlayer(), item, static_cast<slots_t>(slot));
			tfs::events::player::onInventoryUpdate(getPlayer(), item, static_cast<slots_t>(slot), false);
		}
	}
}

// container
void Player::onAddContainerItem(const std::shared_ptr<const Item>& item) { checkTradeState(item); }

void Player::onUpdateContainerItem(const std::shared_ptr<const Container>& container,
                                   const std::shared_ptr<const Item>& oldItem,
                                   const std::shared_ptr<const Item>& newItem)
{
	if (oldItem != newItem) {
		onRemoveContainerItem(container, oldItem);
	}

	if (tradeState != TRADE_TRANSFER) {
		checkTradeState(oldItem);
	}
}

void Player::onRemoveContainerItem(const std::shared_ptr<const Container>& container,
                                   const std::shared_ptr<const Item>& item)
{
	if (tradeState != TRADE_TRANSFER) {
		checkTradeState(item);

		if (tradeItem) {
			if (tradeItem->getParent() != container && container->isHoldingItem(tradeItem)) {
				g_game.internalCloseTrade(getPlayer());
			}
		}
	}
}

void Player::onCloseContainer(const std::shared_ptr<const Container>& container)
{
	if (!client) {
		return;
	}

	for (const auto& it : openContainers) {
		if (it.second.container == container) {
			client->sendCloseContainer(it.first);
		}
	}
}

void Player::onSendContainer(const std::shared_ptr<const Container>& container)
{
	if (!client) {
		return;
	}

	for (const auto& [id, openContainer] : openContainers) {
		if (openContainer.container == container) {
			client->sendContainer(id, container, openContainer.index);
		}
	}
}

// inventory
void Player::onUpdateInventoryItem(const std::shared_ptr<Item>& oldItem, const std::shared_ptr<Item>& newItem)
{
	if (oldItem != newItem) {
		onRemoveInventoryItem(oldItem);
	}

	if (tradeState != TRADE_TRANSFER) {
		checkTradeState(oldItem);
	}
}

void Player::onRemoveInventoryItem(const std::shared_ptr<Item>& item)
{
	if (tradeState != TRADE_TRANSFER) {
		checkTradeState(item);

		if (tradeItem) {
			const std::shared_ptr<const Container>& container = item->getContainer();
			if (container && container->isHoldingItem(tradeItem)) {
				g_game.internalCloseTrade(getPlayer());
			}
		}
	}
}

void Player::checkTradeState(const std::shared_ptr<const Item>& item)
{
	if (!tradeItem || tradeState == TRADE_TRANSFER) {
		return;
	}

	if (tradeItem == item) {
		g_game.internalCloseTrade(getPlayer());
	} else if (auto parent = item->getParent()) {
		auto container = parent->getContainer();
		while (container) {
			if (container == tradeItem) {
				g_game.internalCloseTrade(getPlayer());
				break;
			}

			parent = container->getParent();
			container = parent ? parent->getContainer() : nullptr;
		}
	}
}

void Player::setNextWalkActionTask(SchedulerTask* task)
{
	if (walkTaskEvent != 0) {
		g_scheduler.stopEvent(walkTaskEvent);
		walkTaskEvent = 0;
	}

	delete walkTask;
	walkTask = task;
}

void Player::setNextActionTask(SchedulerTask* task, bool resetIdleTime /*= true */)
{
	if (actionTaskEvent != 0) {
		g_scheduler.stopEvent(actionTaskEvent);
		actionTaskEvent = 0;
	}

	if (task) {
		actionTaskEvent = g_scheduler.addEvent(task);
		if (resetIdleTime) {
			this->resetIdleTime();
		}
	}
}

uint32_t Player::getNextActionTime() const { return std::max<int64_t>(SCHEDULER_MINTICKS, nextAction - OTSYS_TIME()); }

void Player::onThink(uint32_t interval)
{
	Creature::onThink(interval);

	sendPing();

	MessageBufferTicks += interval;
	if (MessageBufferTicks >= 1500) {
		MessageBufferTicks = 0;
		addMessageBuffer();
	}

	if (!getTile()->hasFlag(TILESTATE_NOLOGOUT) && !isAccessPlayer()) {
		idleTime += interval;
		const int32_t kickAfterMinutes = getNumber(ConfigManager::KICK_AFTER_MINUTES);
		if (idleTime > (kickAfterMinutes * 60000) + 60000) {
			kickPlayer(true);
		} else if (client && idleTime == 60000 * kickAfterMinutes) {
			client->sendTextMessage(TextMessage(
			    MESSAGE_STATUS_WARNING,
			    fmt::format(
			        "There was no variation in your behaviour for {:d} minutes. You will be disconnected in one minute if there is no change in your actions until then.",
			        kickAfterMinutes)));
		}
	}

	if (g_game.getWorldType() != WORLD_TYPE_PVP_ENFORCED) {
		checkSkullTicks(interval / 1000);
	}

	addOfflineTrainingTime(interval);
	if (lastStatsTrainingTime != getOfflineTrainingTime() / 60 / 1000) {
		sendStats();
	}
}

uint32_t Player::isMuted() const
{
	if (hasFlag(PlayerFlag_CannotBeMuted)) {
		return 0;
	}

	int32_t muteTicks = 0;
	for (Condition* condition : conditions) {
		if (condition->getType() == CONDITION_MUTED && condition->getTicks() > muteTicks) {
			muteTicks = condition->getTicks();
		}
	}
	return static_cast<uint32_t>(muteTicks) / 1000;
}

void Player::addMessageBuffer()
{
	if (MessageBufferCount > 0 && getNumber(ConfigManager::MAX_MESSAGEBUFFER) != 0 &&
	    !hasFlag(PlayerFlag_CannotBeMuted)) {
		--MessageBufferCount;
	}
}

void Player::removeMessageBuffer()
{
	if (hasFlag(PlayerFlag_CannotBeMuted)) {
		return;
	}

	const int32_t maxMessageBuffer = getNumber(ConfigManager::MAX_MESSAGEBUFFER);
	if (maxMessageBuffer != 0 && MessageBufferCount <= maxMessageBuffer + 1) {
		if (++MessageBufferCount > maxMessageBuffer) {
			uint32_t muteCount = 1;
			auto it = muteCountMap.find(guid);
			if (it != muteCountMap.end()) {
				muteCount = it->second;
			}

			uint32_t muteTime = 5 * muteCount * muteCount;
			muteCountMap[guid] = muteCount + 1;
			Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_MUTED, muteTime * 1000, 0);
			addCondition(condition);

			sendTextMessage(MESSAGE_STATUS_SMALL, fmt::format("You are muted for {:d} seconds.", muteTime));
		}
	}
}

void Player::drainHealth(const std::shared_ptr<Creature>& attacker, int32_t damage)
{
	Creature::drainHealth(attacker, damage);
	sendStats();
}

void Player::drainMana(const std::shared_ptr<Creature>& attacker, int32_t manaLoss)
{
	onAttacked();
	changeMana(-manaLoss);

	if (attacker) {
		addDamagePoints(attacker, manaLoss);
	}

	sendStats();
}

void Player::addManaSpent(uint64_t amount)
{
	if (hasFlag(PlayerFlag_NotGainMana)) {
		return;
	}

	uint64_t currReqMana = vocation->getReqMana(magLevel);
	uint64_t nextReqMana = vocation->getReqMana(magLevel + 1);
	if (currReqMana >= nextReqMana) {
		// player has reached max magic level
		return;
	}

	tfs::events::player::onGainSkillTries(getPlayer(), SKILL_MAGLEVEL, amount);
	if (amount == 0) {
		return;
	}

	bool sendUpdateStats = false;
	while ((manaSpent + amount) >= nextReqMana) {
		amount -= nextReqMana - manaSpent;

		magLevel++;
		manaSpent = 0;

		sendTextMessage(MESSAGE_EVENT_ADVANCE, fmt::format("You advanced to magic level {:d}.", magLevel));

		g_creatureEvents->playerAdvance(getPlayer(), SKILL_MAGLEVEL, magLevel - 1, magLevel);

		sendUpdateStats = true;
		currReqMana = nextReqMana;
		nextReqMana = vocation->getReqMana(magLevel + 1);
		if (currReqMana >= nextReqMana) {
			return;
		}
	}

	manaSpent += amount;

	uint8_t oldPercent = magLevelPercent;
	if (nextReqMana > currReqMana) {
		magLevelPercent = Player::getBasisPointLevel(manaSpent, nextReqMana);
	} else {
		magLevelPercent = 0;
	}

	if (oldPercent != magLevelPercent) {
		sendUpdateStats = true;
	}

	if (sendUpdateStats) {
		sendStats();
		sendSkills();
	}
}

void Player::removeManaSpent(uint64_t amount, bool notify /* = false*/)
{
	if (amount == 0) {
		return;
	}

	uint32_t oldLevel = magLevel;
	uint8_t oldPercent = magLevelPercent;

	while (amount > manaSpent && magLevel > 0) {
		amount -= manaSpent;
		manaSpent = vocation->getReqMana(magLevel);
		magLevel--;
	}

	manaSpent -= amount;

	uint64_t nextReqMana = vocation->getReqMana(magLevel + 1);
	if (nextReqMana > vocation->getReqMana(magLevel)) {
		magLevelPercent = Player::getBasisPointLevel(manaSpent, nextReqMana);
	} else {
		magLevelPercent = 0;
	}

	if (notify) {
		bool sendUpdateStats = false;
		if (oldLevel != magLevel) {
			sendTextMessage(MESSAGE_EVENT_ADVANCE, fmt::format("You were downgraded to magic level {:d}.", magLevel));
			sendUpdateStats = true;
		}

		if (sendUpdateStats || oldPercent != magLevelPercent) {
			sendStats();
			sendSkills();
		}
	}
}

void Player::addExperience(const std::shared_ptr<Creature>& source, uint64_t exp, bool sendText /* = false*/)
{
	uint64_t currLevelExp = Player::getExpForLevel(level);
	uint64_t nextLevelExp = Player::getExpForLevel(level + 1);
	uint64_t rawExp = exp;
	if (currLevelExp >= nextLevelExp) {
		// player has reached max level
		levelPercent = 0;
		sendStats();
		return;
	}

	tfs::events::player::onGainExperience(getPlayer(), source, exp, rawExp, sendText);
	if (exp == 0) {
		return;
	}

	experience += exp;

	uint32_t prevLevel = level;
	while (experience >= nextLevelExp) {
		++level;
		healthMax += vocation->getHPGain();
		health += vocation->getHPGain();
		manaMax += vocation->getManaGain();
		mana += vocation->getManaGain();
		capacity += vocation->getCapGain();

		currLevelExp = nextLevelExp;
		nextLevelExp = Player::getExpForLevel(level + 1);
		if (currLevelExp >= nextLevelExp) {
			// player has reached max level
			break;
		}
	}

	if (prevLevel != level) {
		health = getMaxHealth();
		mana = getMaxMana();

		updateBaseSpeed();
		setBaseSpeed(getBaseSpeed());

		g_game.changeSpeed(getPlayer(), 0);
		g_game.addCreatureHealth(getPlayer());

		const uint32_t protectionLevel = static_cast<uint32_t>(getNumber(ConfigManager::PROTECTION_LEVEL));
		if (prevLevel < protectionLevel && level >= protectionLevel) {
			g_game.updateCreatureWalkthrough(getPlayer());
		}

		if (party) {
			party->updateSharedExperience();
		}

		g_creatureEvents->playerAdvance(getPlayer(), SKILL_LEVEL, prevLevel, level);

		sendTextMessage(MESSAGE_EVENT_ADVANCE,
		                fmt::format("You advanced from Level {:d} to Level {:d}.", prevLevel, level));
	}

	if (nextLevelExp > currLevelExp) {
		levelPercent = Player::getBasisPointLevel(experience - currLevelExp, nextLevelExp - currLevelExp) / 100;
	} else {
		levelPercent = 0;
	}
	sendStats();

	sendExperienceTracker(rawExp * ConfigManager::getExperienceStage(getLevel()), exp);
}

void Player::removeExperience(uint64_t exp, bool sendText /* = false*/)
{
	if (experience == 0 || exp == 0) {
		return;
	}

	tfs::events::player::onLoseExperience(getPlayer(), exp);
	if (exp == 0) {
		return;
	}

	uint64_t lostExp = experience;
	experience = std::max<int64_t>(0, experience - exp);

	if (sendText) {
		lostExp -= experience;

		std::string expString = std::to_string(lostExp) + (lostExp != 1 ? " experience points." : " experience point.");

		TextMessage message(MESSAGE_EXPERIENCE, "You lost " + expString);
		message.position = position;
		message.primary.value = lostExp;
		message.primary.color = TEXTCOLOR_RED;
		sendTextMessage(message);

		SpectatorVec spectators;
		g_game.map.getSpectators(spectators, position, false, true);
		spectators.erase(getPlayer());
		if (!spectators.empty()) {
			message.type = MESSAGE_EXPERIENCE_OTHERS;
			message.text = getName() + " lost " + expString;
			for (const auto& spectator : spectators) {
				assert(spectator->getPlayer() != nullptr);
				std::static_pointer_cast<Player>(spectator)->sendTextMessage(message);
			}
		}
	}

	uint32_t oldLevel = level;
	uint64_t currLevelExp = Player::getExpForLevel(level);

	while (level > 1 && experience < currLevelExp) {
		--level;
		healthMax = std::max<int32_t>(0, healthMax - vocation->getHPGain());
		manaMax = std::max<int32_t>(0, manaMax - vocation->getManaGain());
		capacity = std::max<int32_t>(0, capacity - vocation->getCapGain());
		currLevelExp = Player::getExpForLevel(level);
	}

	if (oldLevel != level) {
		health = getMaxHealth();
		mana = getMaxMana();

		updateBaseSpeed();
		setBaseSpeed(getBaseSpeed());

		g_game.changeSpeed(getPlayer(), 0);
		g_game.addCreatureHealth(getPlayer());

		const uint32_t protectionLevel = static_cast<uint32_t>(getNumber(ConfigManager::PROTECTION_LEVEL));
		if (oldLevel >= protectionLevel && level < protectionLevel) {
			g_game.updateCreatureWalkthrough(getPlayer());
		}

		if (party) {
			party->updateSharedExperience();
		}

		sendTextMessage(MESSAGE_EVENT_ADVANCE,
		                fmt::format("You were downgraded from Level {:d} to Level {:d}.", oldLevel, level));
	}

	uint64_t nextLevelExp = Player::getExpForLevel(level + 1);
	if (nextLevelExp > currLevelExp) {
		levelPercent = Player::getBasisPointLevel(experience - currLevelExp, nextLevelExp - currLevelExp) / 100;
	} else {
		levelPercent = 0;
	}
	sendStats();

	sendExperienceTracker(0, -static_cast<int64_t>(exp));
}

uint16_t Player::getBasisPointLevel(uint64_t count, uint64_t nextLevelCount)
{
	if (nextLevelCount == 0) {
		return 0;
	}

	uint16_t result = ((count * 10000.) / nextLevelCount);
	if (result > 10000) {
		return 0;
	}
	return result;
}

void Player::onBlockHit()
{
	if (shieldBlockCount > 0) {
		--shieldBlockCount;

		if (hasShield()) {
			addSkillAdvance(SKILL_SHIELD, 1);
		}
	}
}

void Player::onAttackedCreatureBlockHit(BlockType_t blockType)
{
	lastAttackBlockType = blockType;

	switch (blockType) {
		case BLOCK_NONE: {
			addAttackSkillPoint = true;
			bloodHitCount = 30;
			shieldBlockCount = 30;
			break;
		}

		case BLOCK_DEFENSE:
		case BLOCK_ARMOR: {
			// need to draw blood every 30 hits
			if (bloodHitCount > 0) {
				addAttackSkillPoint = true;
				--bloodHitCount;
			} else {
				addAttackSkillPoint = false;
			}
			break;
		}

		default: {
			addAttackSkillPoint = false;
			break;
		}
	}
}

bool Player::hasShield() const
{
	auto item = inventory[CONST_SLOT_LEFT];
	if (item && item->getWeaponType() == WEAPON_SHIELD) {
		return true;
	}

	item = inventory[CONST_SLOT_RIGHT];
	if (item && item->getWeaponType() == WEAPON_SHIELD) {
		return true;
	}
	return false;
}

BlockType_t Player::blockHit(const std::shared_ptr<Creature>& attacker, CombatType_t combatType, int32_t& damage,
                             bool checkDefense /* = false*/, bool checkArmor /* = false*/, bool field /* = false*/,
                             bool ignoreResistances /* = false*/)
{
	BlockType_t blockType =
	    Creature::blockHit(attacker, combatType, damage, checkDefense, checkArmor, field, ignoreResistances);

	if (attacker && combatType != COMBAT_HEALING) {
		sendCreatureSquare(attacker, SQ_COLOR_BLACK);
	}

	if (blockType != BLOCK_NONE) {
		return blockType;
	}

	if (damage <= 0) {
		damage = 0;
		return BLOCK_ARMOR;
	}

	if (!ignoreResistances) {
		Reflect reflect;

		size_t combatIndex = combatTypeToIndex(combatType);
		for (int32_t slot = CONST_SLOT_FIRST; slot <= CONST_SLOT_LAST; ++slot) {
			if (!isItemAbilityEnabled(static_cast<slots_t>(slot))) {
				continue;
			}

			auto item = inventory[slot];
			if (!item) {
				continue;
			}

			const ItemType& it = Item::items[item->getID()];
			if (!it.abilities) {
				if (damage <= 0) {
					damage = 0;
					return BLOCK_ARMOR;
				}

				continue;
			}

			const int16_t& absorbPercent = it.abilities->absorbPercent[combatIndex];
			if (absorbPercent != 0) {
				damage -= std::ceil(damage * (absorbPercent / 100.));

				uint16_t charges = item->getCharges();
				if (charges != 0) {
					g_game.transformItem(item, item->getID(), charges - 1);
				}
			}

			reflect += item->getReflect(combatType);

			if (field) {
				const int16_t& fieldAbsorbPercent = it.abilities->fieldAbsorbPercent[combatIndex];
				if (fieldAbsorbPercent != 0) {
					damage -= std::ceil(damage * (fieldAbsorbPercent / 100.));

					uint16_t charges = item->getCharges();
					if (charges != 0) {
						g_game.transformItem(item, item->getID(), charges - 1);
					}
				}
			}
		}

		if (attacker && reflect.chance > 0 && reflect.percent != 0 && uniform_random(1, 100) <= reflect.chance) {
			CombatDamage reflectDamage;
			reflectDamage.primary.type = combatType;
			reflectDamage.primary.value = -std::round(damage * (reflect.percent / 100.));
			reflectDamage.origin = ORIGIN_REFLECT;
			g_game.combatChangeHealth(getPlayer(), attacker, reflectDamage);
		}
	}

	if (damage <= 0) {
		damage = 0;
		blockType = BLOCK_ARMOR;
	}
	return blockType;
}

Connection::Address Player::getIP() const
{
	if (client) {
		return client->getIP();
	}

	return {};
}

void Player::death(const std::shared_ptr<Creature>& lastHitCreature)
{
	loginPosition = town->templePosition;

	if (skillLoss) {
		uint8_t unfairFightReduction = 100;
		bool lastHitPlayer = Player::lastHitIsPlayer(lastHitCreature);

		if (lastHitPlayer) {
			uint32_t sumLevels = 0;
			uint32_t inFightTicks = getNumber(ConfigManager::PZ_LOCKED);
			for (const auto& it : damageMap) {
				CountBlock_t cb = it.second;
				if ((OTSYS_TIME() - cb.ticks) <= inFightTicks) {
					std::shared_ptr<Player> damageDealer = g_game.getPlayerByID(it.first);
					if (damageDealer) {
						sumLevels += damageDealer->getLevel();
					}
				}
			}

			if (sumLevels > level) {
				double reduce = level / static_cast<double>(sumLevels);
				unfairFightReduction = std::max<uint8_t>(20, std::floor((reduce * 100) + 0.5));
			}
		}

		// Magic level loss
		uint64_t sumMana = 0;
		for (uint32_t i = 1; i <= magLevel; ++i) {
			sumMana += vocation->getReqMana(i);
		}

		double deathLossPercent = getLostPercent() * (unfairFightReduction / 100.);
		removeManaSpent(static_cast<uint64_t>((sumMana + manaSpent) * deathLossPercent), false);

		// Skill loss
		for (uint8_t i = SKILL_FIRST; i <= SKILL_LAST; ++i) { // for each skill
			uint64_t sumSkillTries = 0;
			for (uint16_t c = MINIMUM_SKILL_LEVEL + 1; c <= skills[i].level;
			     ++c) { // sum up all required tries for all skill levels
				sumSkillTries += vocation->getReqSkillTries(i, c);
			}

			sumSkillTries += skills[i].tries;

			removeSkillTries(static_cast<skills_t>(i), sumSkillTries * deathLossPercent, false);
		}

		// Level loss
		uint64_t expLoss = static_cast<uint64_t>(experience * deathLossPercent);
		tfs::events::player::onLoseExperience(getPlayer(), expLoss);
		if (expLoss != 0) {
			uint32_t oldLevel = level;

			if (vocation->getId() == VOCATION_NONE || level > 7) {
				experience -= expLoss;
			}

			while (level > 1 && experience < Player::getExpForLevel(level)) {
				--level;
				healthMax = std::max<int32_t>(0, healthMax - vocation->getHPGain());
				manaMax = std::max<int32_t>(0, manaMax - vocation->getManaGain());
				capacity = std::max<int32_t>(0, capacity - vocation->getCapGain());
			}

			if (oldLevel != level) {
				sendTextMessage(MESSAGE_EVENT_ADVANCE,
				                fmt::format("You were downgraded from Level {:d} to Level {:d}.", oldLevel, level));
			}

			uint64_t currLevelExp = Player::getExpForLevel(level);
			uint64_t nextLevelExp = Player::getExpForLevel(level + 1);
			if (nextLevelExp > currLevelExp) {
				levelPercent = Player::getBasisPointLevel(experience - currLevelExp, nextLevelExp - currLevelExp) / 100;
			} else {
				levelPercent = 0;
			}
		}

		if (blessings.test(5)) {
			if (lastHitPlayer) {
				blessings.reset(5);
			} else {
				blessings.reset();
				blessings.set(5);
			}
		} else {
			blessings.reset();
		}

		sendStats();
		sendSkills();
		sendReLoginWindow(unfairFightReduction);

		if (getSkull() == SKULL_BLACK) {
			health = 40;
			mana = 0;
		} else {
			health = healthMax;
			mana = manaMax;
		}

		auto it = conditions.begin(), end = conditions.end();
		while (it != end) {
			Condition* condition = *it;
			if (condition->isPersistent()) {
				it = conditions.erase(it);

				condition->endCondition(getPlayer());
				onEndCondition(condition->getType());
				delete condition;
			} else {
				++it;
			}
		}
	} else {
		setSkillLoss(true);

		auto it = conditions.begin(), end = conditions.end();
		while (it != end) {
			Condition* condition = *it;
			if (condition->isPersistent()) {
				it = conditions.erase(it);

				condition->endCondition(getPlayer());
				onEndCondition(condition->getType());
				delete condition;
			} else {
				++it;
			}
		}

		health = healthMax;
		g_game.internalTeleport(getCreature(), getTemplePosition(), true);
		g_game.addCreatureHealth(getPlayer());
		onThink(EVENT_CREATURE_THINK_INTERVAL);
		onIdleStatus();
		sendStats();
	}
}

bool Player::dropCorpse(const std::shared_ptr<Creature>& lastHitCreature,
                        const std::shared_ptr<Creature>& mostDamageCreature, bool lastHitUnjustified,
                        bool mostDamageUnjustified)
{
	if (getZone() != ZONE_PVP || !Player::lastHitIsPlayer(lastHitCreature)) {
		return Creature::dropCorpse(lastHitCreature, mostDamageCreature, lastHitUnjustified, mostDamageUnjustified);
	}

	setDropLoot(true);
	return false;
}

std::shared_ptr<Item> Player::getCorpse(const std::shared_ptr<Creature>& lastHitCreature,
                                        const std::shared_ptr<Creature>& mostDamageCreature)
{
	auto corpse = Creature::getCorpse(lastHitCreature, mostDamageCreature);
	if (corpse && corpse->getContainer()) {
		size_t killersSize = getKillers().size();

		if (lastHitCreature) {
			if (!mostDamageCreature) {
				corpse->setSpecialDescription(
				    fmt::format("You recognize {:s}. {:s} was killed by {:s}{:s}", getNameDescription(),
				                getSex() == PLAYERSEX_FEMALE ? "She" : "He", lastHitCreature->getNameDescription(),
				                killersSize > 1 ? " and others." : "."));
			} else if (lastHitCreature != mostDamageCreature) {
				corpse->setSpecialDescription(
				    fmt::format("You recognize {:s}. {:s} was killed by {:s}, {:s}{:s}", getNameDescription(),
				                getSex() == PLAYERSEX_FEMALE ? "She" : "He", mostDamageCreature->getNameDescription(),
				                lastHitCreature->getNameDescription(), killersSize > 2 ? " and others." : "."));
			} else {
				corpse->setSpecialDescription(
				    fmt::format("You recognize {:s}. {:s} was killed by {:s} and others.", getNameDescription(),
				                getSex() == PLAYERSEX_FEMALE ? "She" : "He", mostDamageCreature->getNameDescription()));
			}
		} else if (mostDamageCreature) {
			if (killersSize > 1) {
				corpse->setSpecialDescription(fmt::format(
				    "You recognize {:s}. {:s} was killed by something evil, {:s}, and others", getNameDescription(),
				    getSex() == PLAYERSEX_FEMALE ? "She" : "He", mostDamageCreature->getNameDescription()));
			} else {
				corpse->setSpecialDescription(fmt::format(
				    "You recognize {:s}. {:s} was killed by something evil and others", getNameDescription(),
				    getSex() == PLAYERSEX_FEMALE ? "She" : "He", mostDamageCreature->getNameDescription()));
			}
		} else {
			corpse->setSpecialDescription(fmt::format("You recognize {:s}. {:s} was killed by something evil {:s}",
			                                          getNameDescription(), getSex() == PLAYERSEX_FEMALE ? "She" : "He",
			                                          killersSize ? " and others." : "."));
		}
	}
	return corpse;
}

void Player::addInFightTicks(bool pzlock /*= false*/)
{
	if (hasFlag(PlayerFlag_NotGainInFight)) {
		return;
	}

	if (pzlock) {
		pzLocked = true;
	}

	Condition* condition =
	    Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_INFIGHT, getNumber(ConfigManager::PZ_LOCKED), 0);
	addCondition(condition);
}

void Player::removeList()
{
	g_game.removePlayer(getPlayer());

	for (const auto& it : g_game.getPlayers()) {
		it.second->notifyStatusChange(getPlayer(), VIPSTATUS_OFFLINE);
	}
}

void Player::addList()
{
	for (const auto& it : g_game.getPlayers()) {
		it.second->notifyStatusChange(getPlayer(), VIPSTATUS_ONLINE);
	}

	g_game.addPlayer(getPlayer());
}

void Player::kickPlayer(bool displayEffect)
{
	g_creatureEvents->playerLogout(getPlayer());
	if (client) {
		client->logout(displayEffect, true);
	} else {
		g_game.removeCreature(getPlayer());
	}
}

void Player::notifyStatusChange(const std::shared_ptr<Player>& loginPlayer, VipStatus_t status)
{
	if (!client) {
		return;
	}

	auto it = VIPList.find(loginPlayer->guid);
	if (it == VIPList.end()) {
		return;
	}

	client->sendUpdatedVIPStatus(loginPlayer->guid, status);

	if (status == VIPSTATUS_ONLINE) {
		client->sendTextMessage(TextMessage(MESSAGE_STATUS_SMALL, loginPlayer->getName() + " has logged in."));
	} else if (status == VIPSTATUS_OFFLINE) {
		client->sendTextMessage(TextMessage(MESSAGE_STATUS_SMALL, loginPlayer->getName() + " has logged out."));
	}
}

bool Player::removeVIP(uint32_t vipGuid)
{
	if (VIPList.erase(vipGuid) == 0) {
		return false;
	}

	IOLoginData::removeVIPEntry(accountNumber, vipGuid);
	return true;
}

bool Player::addVIP(uint32_t vipGuid, const std::string& vipName, VipStatus_t status)
{
	if (VIPList.size() >= getMaxVIPEntries()) {
		sendTextMessage(MESSAGE_STATUS_SMALL, "You cannot add more buddies.");
		return false;
	}

	auto result = VIPList.insert(vipGuid);
	if (!result.second) {
		sendTextMessage(MESSAGE_STATUS_SMALL, "This player is already in your list.");
		return false;
	}

	IOLoginData::addVIPEntry(accountNumber, vipGuid, "", 0, false);
	if (client) {
		client->sendVIP(vipGuid, vipName, "", 0, false, status);
	}
	return true;
}

bool Player::addVIPInternal(uint32_t vipGuid)
{
	if (VIPList.size() >= getMaxVIPEntries()) {
		return false;
	}

	return VIPList.insert(vipGuid).second;
}

bool Player::editVIP(uint32_t vipGuid, const std::string& description, uint32_t icon, bool notify)
{
	auto it = VIPList.find(vipGuid);
	if (it == VIPList.end()) {
		return false; // player is not in VIP
	}

	IOLoginData::editVIPEntry(accountNumber, vipGuid, description, icon, notify);
	return true;
}

// close container and its child containers
void Player::autoCloseContainers(const std::shared_ptr<const Container>& container)
{
	std::vector<uint32_t> closeList;
	for (const auto& [id, openContainer] : openContainers) {
		auto tmpContainer = openContainer.container;
		while (tmpContainer) {
			if (tmpContainer->isRemoved() || tmpContainer == container) {
				closeList.push_back(id);
				break;
			}

			auto parent = tmpContainer->getParent();
			tmpContainer = parent ? parent->getContainer() : nullptr;
		}
	}

	for (uint32_t containerId : closeList) {
		closeContainer(containerId);
		if (client) {
			client->sendCloseContainer(containerId);
		}
	}
}

bool Player::hasCapacity(const std::shared_ptr<const Item>& item, uint32_t count) const
{
	if (hasFlag(PlayerFlag_CannotPickupItem)) {
		return false;
	}

	if (hasFlag(PlayerFlag_HasInfiniteCapacity) || item->getTopParent().get() == this) {
		return true;
	}

	uint32_t itemWeight = item->getContainer() ? item->getWeight() : item->getBaseWeight();
	if (item->isStackable()) {
		itemWeight *= count;
	}
	return itemWeight <= getFreeCapacity();
}

ReturnValue Player::queryAdd(int32_t index, const std::shared_ptr<const Thing>& thing, uint32_t count, uint32_t flags,
                             const std::shared_ptr<Creature>&) const
{
	auto item = thing->getItem();
	if (!item) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	bool childIsOwner = hasBitSet(FLAG_CHILDISOWNER, flags);
	if (childIsOwner) {
		// a child container is querying the player, just check if enough capacity
		bool skipLimit = hasBitSet(FLAG_NOLIMIT, flags);
		if (skipLimit || hasCapacity(item, count)) {
			return RETURNVALUE_NOERROR;
		}
		return RETURNVALUE_NOTENOUGHCAPACITY;
	}

	if (!item->isPickupable()) {
		return RETURNVALUE_CANNOTPICKUP;
	}

	if (item->isStoreItem()) {
		return RETURNVALUE_ITEMCANNOTBEMOVEDTHERE;
	}

	ReturnValue ret = RETURNVALUE_NOTPOSSIBLE;

	const int32_t& slotPosition = item->getSlotPosition();
	if ((slotPosition & SLOTP_HEAD) || (slotPosition & SLOTP_NECKLACE) || (slotPosition & SLOTP_BACKPACK) ||
	    (slotPosition & SLOTP_ARMOR) || (slotPosition & SLOTP_LEGS) || (slotPosition & SLOTP_FEET) ||
	    (slotPosition & SLOTP_RING)) {
		ret = RETURNVALUE_CANNOTBEDRESSED;
	} else if (slotPosition & SLOTP_TWO_HAND) {
		ret = RETURNVALUE_PUTTHISOBJECTINBOTHHANDS;
	} else if ((slotPosition & SLOTP_RIGHT) || (slotPosition & SLOTP_LEFT)) {
		if (!getBoolean(ConfigManager::CLASSIC_EQUIPMENT_SLOTS)) {
			ret = RETURNVALUE_CANNOTBEDRESSED;
		} else {
			ret = RETURNVALUE_PUTTHISOBJECTINYOURHAND;
		}
	}

	switch (index) {
		case CONST_SLOT_HEAD: {
			if (slotPosition & SLOTP_HEAD) {
				ret = RETURNVALUE_NOERROR;
			}
			break;
		}

		case CONST_SLOT_NECKLACE: {
			if (slotPosition & SLOTP_NECKLACE) {
				ret = RETURNVALUE_NOERROR;
			}
			break;
		}

		case CONST_SLOT_BACKPACK: {
			if (slotPosition & SLOTP_BACKPACK) {
				ret = RETURNVALUE_NOERROR;
			}
			break;
		}

		case CONST_SLOT_ARMOR: {
			if (slotPosition & SLOTP_ARMOR) {
				ret = RETURNVALUE_NOERROR;
			}
			break;
		}

		case CONST_SLOT_RIGHT: {
			if (slotPosition & SLOTP_RIGHT) {
				if (!getBoolean(ConfigManager::CLASSIC_EQUIPMENT_SLOTS)) {
					if (item->getWeaponType() != WEAPON_SHIELD && item->getWeaponType() != WEAPON_QUIVER) {
						ret = RETURNVALUE_CANNOTBEDRESSED;
					} else {
						auto leftItem = inventory[CONST_SLOT_LEFT];
						if (leftItem) {
							if ((leftItem->getSlotPosition() | slotPosition) & SLOTP_TWO_HAND) {
								if (leftItem->getWeaponType() != WEAPON_DISTANCE ||
								    item->getWeaponType() != WEAPON_QUIVER) {
									ret = RETURNVALUE_BOTHHANDSNEEDTOBEFREE;
								} else {
									ret = RETURNVALUE_NOERROR;
								}
							} else {
								ret = RETURNVALUE_NOERROR;
							}
						} else {
							ret = RETURNVALUE_NOERROR;
						}
					}
				} else if (slotPosition & SLOTP_TWO_HAND) {
					auto leftItem = inventory[CONST_SLOT_LEFT];
					if (leftItem && leftItem != item) {
						ret = RETURNVALUE_BOTHHANDSNEEDTOBEFREE;
					} else {
						ret = RETURNVALUE_NOERROR;
					}
				} else if (inventory[CONST_SLOT_LEFT]) {
					auto leftItem = inventory[CONST_SLOT_LEFT];
					WeaponType_t type = item->getWeaponType(), leftType = leftItem->getWeaponType();

					if (leftItem->getSlotPosition() & SLOTP_TWO_HAND) {
						if (leftItem->getWeaponType() != WEAPON_DISTANCE || type != WEAPON_QUIVER) {
							ret = RETURNVALUE_DROPTWOHANDEDITEM;
						} else {
							ret = RETURNVALUE_NOERROR;
						}
					} else if (item == leftItem && count == item->getItemCount()) {
						ret = RETURNVALUE_NOERROR;
					} else if (leftType == WEAPON_SHIELD && type == WEAPON_SHIELD) {
						ret = RETURNVALUE_CANONLYUSEONESHIELD;
					} else if (leftType == WEAPON_NONE || type == WEAPON_NONE || leftType == WEAPON_SHIELD ||
					           type == WEAPON_SHIELD || leftType == WEAPON_AMMO || type == WEAPON_AMMO ||
					           leftType == WEAPON_QUIVER || type == WEAPON_QUIVER) {
						ret = RETURNVALUE_NOERROR;
					} else {
						ret = RETURNVALUE_CANONLYUSEONEWEAPON;
					}
				} else {
					ret = RETURNVALUE_NOERROR;
				}
			}
			break;
		}

		case CONST_SLOT_LEFT: {
			if (slotPosition & SLOTP_LEFT) {
				if (!getBoolean(ConfigManager::CLASSIC_EQUIPMENT_SLOTS)) {
					WeaponType_t type = item->getWeaponType();
					auto rightItem = inventory[CONST_SLOT_RIGHT];
					if (type == WEAPON_NONE || type == WEAPON_SHIELD || type == WEAPON_AMMO || type == WEAPON_QUIVER) {
						ret = RETURNVALUE_CANNOTBEDRESSED;
					} else if (rightItem && (slotPosition & SLOTP_TWO_HAND)) {
						if (type != WEAPON_DISTANCE || rightItem->getWeaponType() != WEAPON_QUIVER) {
							ret = RETURNVALUE_BOTHHANDSNEEDTOBEFREE;
						} else {
							ret = RETURNVALUE_NOERROR;
						}
					} else {
						ret = RETURNVALUE_NOERROR;
					}
				} else if (slotPosition & SLOTP_TWO_HAND) {
					auto rightItem = inventory[CONST_SLOT_RIGHT];
					if (rightItem && rightItem != item) {
						if (item->getWeaponType() != WEAPON_DISTANCE || rightItem->getWeaponType() != WEAPON_QUIVER) {
							ret = RETURNVALUE_BOTHHANDSNEEDTOBEFREE;
						} else {
							ret = RETURNVALUE_NOERROR;
						}
					} else {
						ret = RETURNVALUE_NOERROR;
					}
				} else if (inventory[CONST_SLOT_RIGHT]) {
					auto rightItem = inventory[CONST_SLOT_RIGHT];
					WeaponType_t type = item->getWeaponType(), rightType = rightItem->getWeaponType();

					if (rightItem->getSlotPosition() & SLOTP_TWO_HAND) {
						if (type != WEAPON_DISTANCE || rightItem->getWeaponType() != WEAPON_QUIVER) {
							ret = RETURNVALUE_DROPTWOHANDEDITEM;
						} else {
							ret = RETURNVALUE_NOERROR;
						}
					} else if (item == rightItem && count == item->getItemCount()) {
						ret = RETURNVALUE_NOERROR;
					} else if (rightType == WEAPON_SHIELD && type == WEAPON_SHIELD) {
						ret = RETURNVALUE_CANONLYUSEONESHIELD;
					} else if (rightType == WEAPON_NONE || type == WEAPON_NONE || rightType == WEAPON_SHIELD ||
					           type == WEAPON_SHIELD || rightType == WEAPON_AMMO || type == WEAPON_AMMO ||
					           rightType == WEAPON_QUIVER || type == WEAPON_QUIVER) {
						ret = RETURNVALUE_NOERROR;
					} else {
						ret = RETURNVALUE_CANONLYUSEONEWEAPON;
					}
				} else {
					ret = RETURNVALUE_NOERROR;
				}
			}
			break;
		}

		case CONST_SLOT_LEGS: {
			if (slotPosition & SLOTP_LEGS) {
				ret = RETURNVALUE_NOERROR;
			}
			break;
		}

		case CONST_SLOT_FEET: {
			if (slotPosition & SLOTP_FEET) {
				ret = RETURNVALUE_NOERROR;
			}
			break;
		}

		case CONST_SLOT_RING: {
			if (slotPosition & SLOTP_RING) {
				ret = RETURNVALUE_NOERROR;
			}
			break;
		}

		case CONST_SLOT_AMMO: {
			if ((slotPosition & SLOTP_AMMO) || getBoolean(ConfigManager::CLASSIC_EQUIPMENT_SLOTS)) {
				ret = RETURNVALUE_NOERROR;
			}
			break;
		}

		case CONST_SLOT_WHEREEVER:
		case -1:
			ret = RETURNVALUE_NOTENOUGHROOM;
			break;

		default:
			ret = RETURNVALUE_NOTPOSSIBLE;
			break;
	}

	if (ret != RETURNVALUE_NOERROR && ret != RETURNVALUE_NOTENOUGHROOM) {
		return ret;
	}

	// check if enough capacity
	if (!hasCapacity(item, count)) {
		return RETURNVALUE_NOTENOUGHCAPACITY;
	}

	if (index != CONST_SLOT_WHEREEVER && index != -1) { // we don't try to equip whereever call
		ret = g_moveEvents->onPlayerEquip(std::const_pointer_cast<Player>(getPlayer()),
		                                  std::const_pointer_cast<Item>(item), static_cast<slots_t>(index), true);
		if (ret != RETURNVALUE_NOERROR) {
			return ret;
		}
	}

	// need an exchange with source? (destination item is swapped with currently moved item)
	auto inventoryItem = getInventoryItem(static_cast<slots_t>(index));
	if (inventoryItem && (!inventoryItem->isStackable() || inventoryItem->getID() != item->getID())) {
		if (!getBoolean(ConfigManager::CLASSIC_EQUIPMENT_SLOTS)) {
			auto topParent = item->getTopParent();
			if (topParent && (std::dynamic_pointer_cast<const DepotChest>(topParent) ||
			                  std::dynamic_pointer_cast<const Player>(topParent))) {
				return RETURNVALUE_NEEDEXCHANGE;
			}
			return RETURNVALUE_NOTENOUGHROOM;
		}

		return RETURNVALUE_NEEDEXCHANGE;
	}
	return ret;
}

ReturnValue Player::queryMaxCount(int32_t index, const std::shared_ptr<const Thing>& thing, uint32_t count,
                                  uint32_t& maxQueryCount, uint32_t flags) const
{
	auto item = thing->getItem();
	if (!item) {
		maxQueryCount = 0;
		return RETURNVALUE_NOTPOSSIBLE;
	}

	if (index == INDEX_WHEREEVER) {
		uint32_t n = 0;
		for (int32_t slotIndex = CONST_SLOT_FIRST; slotIndex <= CONST_SLOT_LAST; ++slotIndex) {
			auto inventoryItem = inventory[slotIndex];
			if (inventoryItem) {
				if (auto subContainer = inventoryItem->getContainer()) {
					uint32_t queryCount = 0;
					subContainer->queryMaxCount(INDEX_WHEREEVER, item, item->getItemCount(), queryCount, flags);
					n += queryCount;

					// iterate through all items, including sub-containers (deep search)
					for (ContainerIterator it = subContainer->iterator(); it.hasNext(); it.advance()) {
						if (auto tmpContainer = (*it)->getContainer()) {
							queryCount = 0;
							tmpContainer->queryMaxCount(INDEX_WHEREEVER, item, item->getItemCount(), queryCount, flags);
							n += queryCount;
						}
					}
				} else if (inventoryItem->isStackable() && *item == *inventoryItem &&
				           inventoryItem->getItemCount() < ITEM_STACK_SIZE) {
					uint32_t remainder = (100 - inventoryItem->getItemCount());

					if (queryAdd(slotIndex, item, remainder, flags) == RETURNVALUE_NOERROR) {
						n += remainder;
					}
				}
			} else if (queryAdd(slotIndex, item, item->getItemCount(), flags) == RETURNVALUE_NOERROR) { // empty slot
				if (item->isStackable()) {
					n += 100;
				} else {
					++n;
				}
			}
		}

		maxQueryCount = n;
	} else {
		std::shared_ptr<Item> destItem = nullptr;

		auto destThing = getThing(index);
		if (destThing) {
			destItem = destThing->getItem();
		}

		if (destItem) {
			if (destItem->isStackable() && *item == *destItem && destItem->getItemCount() < ITEM_STACK_SIZE) {
				maxQueryCount = 100 - destItem->getItemCount();
			} else {
				maxQueryCount = 0;
			}
		} else if (queryAdd(index, item, count, flags) == RETURNVALUE_NOERROR) { // empty slot
			if (item->isStackable()) {
				maxQueryCount = 100;
			} else {
				maxQueryCount = 1;
			}

			return RETURNVALUE_NOERROR;
		}
	}

	if (maxQueryCount < count) {
		return RETURNVALUE_NOTENOUGHROOM;
	}
	return RETURNVALUE_NOERROR;
}

ReturnValue Player::queryRemove(const std::shared_ptr<const Thing>& thing, uint32_t count, uint32_t flags,
                                const std::shared_ptr<Creature>& /*= nullptr*/) const
{
	int32_t index = getThingIndex(thing);
	if (index == -1) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	auto item = thing->getItem();
	if (!item) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	if (count == 0 || (item->isStackable() && count > item->getItemCount())) {
		return RETURNVALUE_NOTPOSSIBLE;
	}

	if (!item->isMoveable() && !hasBitSet(FLAG_IGNORENOTMOVEABLE, flags)) {
		return RETURNVALUE_NOTMOVEABLE;
	}

	return RETURNVALUE_NOERROR;
}

std::shared_ptr<Thing> Player::queryDestination(int32_t& index, const std::shared_ptr<const Thing>& thing,
                                                std::shared_ptr<Item>& destItem, uint32_t& flags)
{
	if (index == 0 /*drop to capacity window*/ || index == INDEX_WHEREEVER) {
		destItem = nullptr;

		auto item = thing->getItem();
		if (!item) {
			return getPlayer();
		}

		bool autoStack = !((flags & FLAG_IGNOREAUTOSTACK) == FLAG_IGNOREAUTOSTACK);
		bool isStackable = item->isStackable();

		std::vector<std::shared_ptr<Container>> containers;

		for (uint32_t slotIndex = CONST_SLOT_FIRST; slotIndex <= CONST_SLOT_LAST; ++slotIndex) {
			auto inventoryItem = inventory[slotIndex];
			if (inventoryItem) {
				if (inventoryItem == tradeItem) {
					continue;
				}

				if (inventoryItem == item) {
					continue;
				}

				if (autoStack && isStackable) {
					// try find an already existing item to stack with
					if (queryAdd(slotIndex, item, item->getItemCount(), 0) == RETURNVALUE_NOERROR) {
						if (*inventoryItem == *item && inventoryItem->getItemCount() < ITEM_STACK_SIZE) {
							index = slotIndex;
							destItem = inventoryItem;
							return getPlayer();
						}
					}

					if (auto subContainer = inventoryItem->getContainer()) {
						containers.push_back(subContainer);
					}
				} else if (auto subContainer = inventoryItem->getContainer()) {
					containers.push_back(subContainer);
				}
			} else if (queryAdd(slotIndex, item, item->getItemCount(), flags) == RETURNVALUE_NOERROR) { // empty slot
				index = slotIndex;
				destItem = nullptr;
				return getPlayer();
			}
		}

		size_t i = 0;
		while (i < containers.size()) {
			auto tmpContainer = containers[i++];
			if (!autoStack || !isStackable) {
				// we need to find first empty container as fast as we can for non-stackable items
				uint32_t n = tmpContainer->capacity() -
				             std::min(tmpContainer->capacity(), static_cast<uint32_t>(tmpContainer->size()));
				while (n) {
					if (tmpContainer->queryAdd(tmpContainer->capacity() - n, item, item->getItemCount(), flags) ==
					    RETURNVALUE_NOERROR) {
						index = tmpContainer->capacity() - n;
						destItem = nullptr;
						return tmpContainer;
					}

					--n;
				}

				for (const auto& tmpContainerItem : tmpContainer->getItemList()) {
					if (auto subContainer = tmpContainerItem->getContainer()) {
						containers.push_back(subContainer);
					}
				}

				continue;
			}

			uint32_t n = 0;

			for (const auto& tmpItem : tmpContainer->getItemList()) {
				if (tmpItem == tradeItem) {
					continue;
				}

				if (tmpItem == item) {
					continue;
				}

				// try find an already existing item to stack with
				if (*tmpItem == *item && tmpItem->getItemCount() < ITEM_STACK_SIZE) {
					index = n;
					destItem = tmpItem;
					return tmpContainer;
				}

				if (auto subContainer = tmpItem->getContainer()) {
					containers.push_back(subContainer);
				}

				n++;
			}

			if (n < tmpContainer->capacity() &&
			    tmpContainer->queryAdd(n, item, item->getItemCount(), flags) == RETURNVALUE_NOERROR) {
				index = n;
				destItem = nullptr;
				return tmpContainer;
			}
		}

		return getPlayer();
	}

	auto destThing = getThing(index);
	if (destThing) {
		destItem = destThing->getItem();
	}

	auto container = destThing->getContainer();
	if (container) {
		index = INDEX_WHEREEVER;
		destItem = nullptr;
		return container;
	}
	return getPlayer();
}

void Player::addThing(int32_t index, const std::shared_ptr<Thing>& thing)
{
	if (index < CONST_SLOT_FIRST || index > CONST_SLOT_LAST) {
		return /*RETURNVALUE_NOTPOSSIBLE*/;
	}

	auto item = thing->getItem();
	if (!item) {
		return /*RETURNVALUE_NOTPOSSIBLE*/;
	}

	item->setParent(getPlayer());
	inventory[index] = item;

	// send to client
	sendInventoryItem(static_cast<slots_t>(index), item);
}

void Player::updateThing(const std::shared_ptr<Thing>& thing, uint16_t itemId, uint32_t count)
{
	int32_t index = getThingIndex(thing);
	if (index == -1) {
		return /*RETURNVALUE_NOTPOSSIBLE*/;
	}

	auto item = thing->getItem();
	if (!item) {
		return /*RETURNVALUE_NOTPOSSIBLE*/;
	}

	item->setID(itemId);
	item->setSubType(count);

	// send to client
	sendInventoryItem(static_cast<slots_t>(index), item);

	// event methods
	onUpdateInventoryItem(item, item);
}

void Player::replaceThing(uint32_t index, const std::shared_ptr<Thing>& thing)
{
	if (index > CONST_SLOT_LAST) {
		return /*RETURNVALUE_NOTPOSSIBLE*/;
	}

	auto oldItem = getInventoryItem(static_cast<slots_t>(index));
	if (!oldItem) {
		return /*RETURNVALUE_NOTPOSSIBLE*/;
	}

	auto item = thing->getItem();
	if (!item) {
		return /*RETURNVALUE_NOTPOSSIBLE*/;
	}

	// send to client
	sendInventoryItem(static_cast<slots_t>(index), item);

	// event methods
	onUpdateInventoryItem(oldItem, item);

	item->setParent(getPlayer());

	inventory[index] = item;
}

void Player::removeThing(const std::shared_ptr<Thing>& thing, uint32_t count)
{
	auto item = thing->getItem();
	if (!item) {
		return /*RETURNVALUE_NOTPOSSIBLE*/;
	}

	int32_t index = getThingIndex(thing);
	if (index == -1) {
		return /*RETURNVALUE_NOTPOSSIBLE*/;
	}

	if (item->isStackable()) {
		if (count == item->getItemCount()) {
			// send change to client
			sendInventoryItem(static_cast<slots_t>(index), nullptr);

			// event methods
			onRemoveInventoryItem(item);

			item->setParent(nullptr);
			inventory[index] = nullptr;
		} else {
			uint8_t newCount = static_cast<uint8_t>(std::max<int32_t>(0, item->getItemCount() - count));
			item->setItemCount(newCount);

			// send change to client
			sendInventoryItem(static_cast<slots_t>(index), item);

			// event methods
			onUpdateInventoryItem(item, item);
		}
	} else {
		// send change to client
		sendInventoryItem(static_cast<slots_t>(index), nullptr);

		// event methods
		onRemoveInventoryItem(item);

		item->setParent(nullptr);
		inventory[index] = nullptr;
	}
}

int32_t Player::getThingIndex(const std::shared_ptr<const Thing>& thing) const
{
	for (int i = CONST_SLOT_FIRST; i <= CONST_SLOT_LAST; ++i) {
		if (inventory[i] == thing) {
			return i;
		}
	}
	return -1;
}

uint32_t Player::getItemTypeCount(uint16_t itemId, int32_t subType /*= -1*/) const
{
	uint32_t count = 0;
	for (int32_t i = CONST_SLOT_FIRST; i <= CONST_SLOT_LAST; i++) {
		auto item = inventory[i];
		if (!item) {
			continue;
		}

		if (item->getID() == itemId) {
			count += Item::countByType(item, subType);
		}

		if (auto container = item->getContainer()) {
			for (ContainerIterator it = container->iterator(); it.hasNext(); it.advance()) {
				if ((*it)->getID() == itemId) {
					count += Item::countByType(*it, subType);
				}
			}
		}
	}
	return count;
}

bool Player::removeItemOfType(uint16_t itemId, uint32_t amount, int32_t subType, bool ignoreEquipped /* = false*/) const
{
	if (amount == 0) {
		return true;
	}

	std::vector<std::shared_ptr<Item>> itemList;

	uint32_t count = 0;
	for (int32_t i = CONST_SLOT_FIRST; i <= CONST_SLOT_LAST; i++) {
		auto item = inventory[i];
		if (!item) {
			continue;
		}

		if (!ignoreEquipped && item->getID() == itemId) {
			uint32_t itemCount = Item::countByType(item, subType);
			if (itemCount == 0) {
				continue;
			}

			itemList.push_back(item);

			count += itemCount;
			if (count >= amount) {
				g_game.internalRemoveItems(std::move(itemList), amount, Item::items[itemId].stackable);
				return true;
			}
		} else if (auto container = item->getContainer()) {
			for (ContainerIterator it = container->iterator(); it.hasNext(); it.advance()) {
				auto containerItem = *it;
				if (containerItem->getID() == itemId) {
					uint32_t itemCount = Item::countByType(containerItem, subType);
					if (itemCount == 0) {
						continue;
					}

					itemList.push_back(containerItem);

					count += itemCount;
					if (count >= amount) {
						g_game.internalRemoveItems(std::move(itemList), amount, Item::items[itemId].stackable);
						return true;
					}
				}
			}
		}
	}
	return false;
}

std::map<uint32_t, uint32_t>& Player::getAllItemTypeCount(std::map<uint32_t, uint32_t>& countMap) const
{
	for (int32_t i = CONST_SLOT_FIRST; i <= CONST_SLOT_LAST; i++) {
		auto item = inventory[i];
		if (!item) {
			continue;
		}

		countMap[item->getID()] += Item::countByType(item, -1);

		if (auto container = item->getContainer()) {
			for (ContainerIterator it = container->iterator(); it.hasNext(); it.advance()) {
				countMap[(*it)->getID()] += Item::countByType(*it, -1);
			}
		}
	}
	return countMap;
}

std::shared_ptr<Thing> Player::getThing(size_t index) const
{
	if (index >= CONST_SLOT_FIRST && index <= CONST_SLOT_LAST) {
		return inventory[index];
	}
	return nullptr;
}

void Player::postAddNotification(const std::shared_ptr<Thing>& thing, const std::shared_ptr<const Thing>& oldParent,
                                 int32_t index, cylinderlink_t link /*= LINK_OWNER*/)
{
	if (link == LINK_OWNER) {
		// calling movement scripts
		g_moveEvents->onPlayerEquip(getPlayer(), thing->getItem(), static_cast<slots_t>(index), false);
		tfs::events::player::onInventoryUpdate(getPlayer(), thing->getItem(), static_cast<slots_t>(index), true);
	}

	bool requireListUpdate = false;

	if (link == LINK_OWNER || link == LINK_TOPPARENT) {
		auto i = (oldParent ? oldParent->getItem() : nullptr);

		// Check if we owned the old container too, so we don't need to do anything,
		// as the list was updated in postRemoveNotification
		assert(i ? i->getContainer() != nullptr : true);

		if (i) {
			requireListUpdate = std::static_pointer_cast<const Container>(i)->getHoldingPlayer().get() != this;
		} else {
			requireListUpdate = oldParent.get() != this;
		}

		updateInventoryWeight();
		updateItemsLight();
		sendStats();
		sendItems();
	}

	if (auto item = thing->getItem()) {
		if (auto container = item->getContainer()) {
			onSendContainer(container);
		}

		if (shopOwner && requireListUpdate) {
			updateSaleShopList(item);
		}
	} else if (auto creature = thing->getCreature()) {
		if (creature.get() == this) {
			// check containers
			std::vector<std::shared_ptr<Container>> containers;

			for (const auto& [_, openContainers] : openContainers) {
				auto container = openContainers.container;
				if (!container->getPosition().isInRange(getPosition(), 1, 1, 0)) {
					containers.push_back(container);
				}
			}

			for (const auto& container : containers) {
				autoCloseContainers(container);
			}
		}
	}
}

void Player::postRemoveNotification(const std::shared_ptr<Thing>& thing, const std::shared_ptr<const Thing>& newParent,
                                    int32_t index, cylinderlink_t link /*= LINK_OWNER*/)
{
	if (link == LINK_OWNER) {
		// calling movement scripts
		g_moveEvents->onPlayerDeEquip(getPlayer(), thing->getItem(), static_cast<slots_t>(index));
		tfs::events::player::onInventoryUpdate(getPlayer(), thing->getItem(), static_cast<slots_t>(index), false);
	}

	bool requireListUpdate = false;

	if (link == LINK_OWNER || link == LINK_TOPPARENT) {
		auto i = (newParent ? newParent->getItem() : nullptr);

		// Check if we owned the old container too, so we don't need to do anything,
		// as the list was updated in postRemoveNotification
		assert(i ? i->getContainer() != nullptr : true);

		if (i) {
			requireListUpdate = std::static_pointer_cast<const Container>(i)->getHoldingPlayer().get() != this;
		} else {
			requireListUpdate = newParent.get() != this;
		}

		updateInventoryWeight();
		updateItemsLight();
		sendStats();
		sendItems();
	}

	if (auto item = thing->getItem()) {
		if (item->isSupply()) {
			if (auto player = item->getHoldingPlayer()) {
				player->sendSupplyUsed(item->getClientID());
			}
		}

		if (auto container = item->getContainer()) {
			if (container->isRemoved() || !getPosition().isInRange(container->getPosition(), 1, 1, 0)) {
				autoCloseContainers(container);
			} else if (container->getTopParent().get() == this) {
				onSendContainer(container);
			} else if (auto topContainer = container->getTopParent()) {
				if (auto depotChest = std::dynamic_pointer_cast<const DepotChest>(topContainer)) {
					bool isOwner = false;

					for (const auto& it : depotChests) {
						if (it.second == depotChest) {
							isOwner = true;
							onSendContainer(container);
						}
					}

					if (!isOwner) {
						autoCloseContainers(container);
					}
				} else if (auto inboxContainer = std::dynamic_pointer_cast<const Inbox>(topContainer)) {
					if (inboxContainer == inbox) {
						onSendContainer(container);
					} else {
						autoCloseContainers(container);
					}
				} else {
					onSendContainer(container);
				}
			} else {
				autoCloseContainers(container);
			}
		}

		if (shopOwner && requireListUpdate) {
			updateSaleShopList(item);
		}
	}
}

bool Player::updateSaleShopList(const std::shared_ptr<const Item>& item)
{
	uint16_t itemId = item->getID();
	bool isCurrency = false;
	for (const auto& it : Item::items.currencyItems) {
		if (it.second == itemId) {
			isCurrency = true;
			break;
		}
	}

	if (!isCurrency) {
		auto it = std::find_if(shopItemList.begin(), shopItemList.end(), [itemId](const ShopInfo& shopInfo) {
			return shopInfo.itemId == itemId && shopInfo.sellPrice != 0;
		});
		if (it == shopItemList.end()) {
			auto container = item->getContainer();
			if (!container) {
				return false;
			}

			const auto& items = container->getItemList();
			return std::any_of(items.begin(), items.end(),
			                   [this](const auto& containerItem) { return updateSaleShopList(containerItem); });
		}
	}

	if (client) {
		client->sendSaleItemList(shopItemList);
	}
	return true;
}

bool Player::hasShopItemForSale(uint32_t itemId, uint8_t subType) const
{
	const ItemType& itemType = Item::items[itemId];
	return std::any_of(shopItemList.begin(), shopItemList.end(), [&](const ShopInfo& shopInfo) {
		return shopInfo.itemId == itemId && (shopInfo.buyPrice != 0 || shopInfo.sellPrice != 0) &&
		       (!itemType.isFluidContainer() || shopInfo.subType == subType);
	});
}

void Player::internalAddThing(uint32_t index, const std::shared_ptr<Thing>& thing)
{
	auto item = thing->getItem();
	if (!item) {
		return;
	}

	// index == 0 means we should equip this item at the most appropriate slot (no action required here)
	if (index > CONST_SLOT_WHEREEVER && index <= CONST_SLOT_LAST) {
		if (inventory[index]) {
			return;
		}

		inventory[index] = item;
		item->setParent(getPlayer());
	}
}

void Player::setFollowCreature(const std::shared_ptr<Creature>& creature)
{
	if (isFollowingCreature(creature)) {
		return;
	}

	if (!canFollowCreature(creature)) {
		removeFollowCreature();
		removeAttackedCreature();
		sendCancelTarget();
		sendCancelMessage(RETURNVALUE_THEREISNOWAY);
		stopWalk();
		return;
	}

	Creature::setFollowCreature(creature);
}

void Player::setAttackedCreature(const std::shared_ptr<Creature>& creature)
{
	if (isAttackingCreature(creature)) {
		return;
	}

	if (!canAttackCreature(creature)) {
		removeAttackedCreature();
		sendCancelTarget();
		return;
	}

	Creature::setAttackedCreature(creature);

	if (chaseMode) {
		if (followCreature != creature) {
			// chase opponent
			setFollowCreature(creature);
		}
	} else if (followCreature) {
		removeFollowCreature();
	}

	g_dispatcher.addTask([id = getID()]() { g_game.checkCreatureAttack(id); });
}

void Player::removeAttackedCreature()
{
	Creature::removeAttackedCreature();

	if (followCreature) {
		removeFollowCreature();
	}
}

void Player::goToFollowCreature()
{
	if (walkTask || !followCreature) {
		return;
	}

	if ((OTSYS_TIME() - lastFailedFollow) < 2000) {
		return;
	}

	FindPathParams fpp;
	getPathSearchParams(followCreature, fpp);
	updateFollowCreaturePath(fpp);

	if (!hasFollowPath) {
		lastFailedFollow = OTSYS_TIME();
	}
}

void Player::getPathSearchParams(const std::shared_ptr<const Creature>& creature, FindPathParams& fpp) const
{
	Creature::getPathSearchParams(creature, fpp);
	fpp.fullPathSearch = true;
}

void Player::doAttacking(uint32_t)
{
	if (lastAttack == 0) {
		lastAttack = OTSYS_TIME() - getAttackSpeed() - 1;
	}

	if (hasCondition(CONDITION_PACIFIED)) {
		return;
	}

	if ((OTSYS_TIME() - lastAttack) >= getAttackSpeed()) {
		bool result = false;

		auto tool = getWeapon();
		const Weapon* weapon = g_weapons->getWeapon(tool);
		uint32_t delay = getAttackSpeed();
		bool classicSpeed = getBoolean(ConfigManager::CLASSIC_ATTACK_SPEED);

		if (weapon) {
			if (!weapon->interruptSwing()) {
				result = weapon->useWeapon(getPlayer(), tool, attackedCreature);
			} else if (!classicSpeed && !canDoAction()) {
				delay = getNextActionTime();
			} else {
				result = weapon->useWeapon(getPlayer(), tool, attackedCreature);
			}
		} else {
			result = Weapon::useFist(getPlayer(), attackedCreature);
		}

		SchedulerTask* task = createSchedulerTask(std::max<uint32_t>(SCHEDULER_MINTICKS, delay),
		                                          [id = getID()]() { g_game.checkCreatureAttack(id); });
		if (!classicSpeed) {
			setNextActionTask(task, false);
		} else {
			g_scheduler.stopEvent(classicAttackEvent);
			classicAttackEvent = g_scheduler.addEvent(task);
		}

		if (result) {
			lastAttack = OTSYS_TIME();
		}
	}
}

uint64_t Player::getGainedExperience(const std::shared_ptr<Creature>& attacker) const
{
	if (getBoolean(ConfigManager::EXPERIENCE_FROM_PLAYERS)) {
		auto attackerPlayer = attacker->getPlayer();
		if (attackerPlayer && attackerPlayer.get() != this && skillLoss &&
		    std::abs(static_cast<int32_t>(attackerPlayer->getLevel() - level)) <=
		        getNumber(ConfigManager::EXP_FROM_PLAYERS_LEVEL_RANGE)) {
			return std::max<uint64_t>(0, std::floor(getLostExperience() * getDamageRatio(attacker) * 0.75));
		}
	}
	return 0;
}

void Player::onUnfollowCreature()
{
	Creature::onUnfollowCreature();

	stopWalk();
}

void Player::setChaseMode(bool mode)
{
	bool prevChaseMode = chaseMode;
	chaseMode = mode;

	if (prevChaseMode != chaseMode) {
		if (chaseMode) {
			if (!followCreature && attackedCreature) {
				// chase opponent
				setFollowCreature(attackedCreature);
			}
		} else if (attackedCreature) {
			removeFollowCreature();
			cancelNextWalk = true;
		}
	}
}

void Player::onWalkAborted()
{
	setNextWalkActionTask(nullptr);
	sendCancelWalk();
}

void Player::onWalkComplete()
{
	if (walkTask) {
		walkTaskEvent = g_scheduler.addEvent(walkTask);
		walkTask = nullptr;
	}
}

void Player::stopWalk() { cancelNextWalk = true; }

LightInfo Player::getCreatureLight() const
{
	if (internalLight.level > itemsLight.level) {
		return internalLight;
	}
	return itemsLight;
}

void Player::updateItemsLight(bool internal /*=false*/)
{
	LightInfo maxLight;

	for (int32_t i = CONST_SLOT_FIRST; i <= CONST_SLOT_LAST; ++i) {
		auto item = inventory[i];
		if (item) {
			LightInfo curLight = item->getLightInfo();

			if (curLight.level > maxLight.level) {
				maxLight = std::move(curLight);
			}
		}
	}

	if (itemsLight.level != maxLight.level || itemsLight.color != maxLight.color) {
		itemsLight = maxLight;

		if (!internal) {
			g_game.changeLight(getPlayer());
		}
	}
}

void Player::onAddCondition(ConditionType_t type)
{
	Creature::onAddCondition(type);

	if (type == CONDITION_OUTFIT && isMounted()) {
		dismount();
	}

	sendIcons();
}

void Player::onAddCombatCondition(ConditionType_t type)
{
	switch (type) {
		case CONDITION_POISON:
			sendTextMessage(MESSAGE_STATUS_DEFAULT, "You are poisoned.");
			break;

		case CONDITION_DROWN:
			sendTextMessage(MESSAGE_STATUS_DEFAULT, "You are drowning.");
			break;

		case CONDITION_PARALYZE:
			sendTextMessage(MESSAGE_STATUS_DEFAULT, "You are paralyzed.");
			break;

		case CONDITION_DRUNK:
			sendTextMessage(MESSAGE_STATUS_DEFAULT, "You are drunk.");
			break;

		case CONDITION_CURSED:
			sendTextMessage(MESSAGE_STATUS_DEFAULT, "You are cursed.");
			break;

		case CONDITION_FREEZING:
			sendTextMessage(MESSAGE_STATUS_DEFAULT, "You are freezing.");
			break;

		case CONDITION_DAZZLED:
			sendTextMessage(MESSAGE_STATUS_DEFAULT, "You are dazzled.");
			break;

		case CONDITION_BLEEDING:
			sendTextMessage(MESSAGE_STATUS_DEFAULT, "You are bleeding.");
			break;

		case CONDITION_ROOT:
			sendTextMessage(MESSAGE_STATUS_DEFAULT, "You are rooted.");
			break;

		default:
			break;
	}
}

void Player::onEndCondition(ConditionType_t type)
{
	Creature::onEndCondition(type);

	if (type == CONDITION_INFIGHT) {
		onIdleStatus();
		pzLocked = false;
		clearAttacked();

		if (getSkull() != SKULL_RED && getSkull() != SKULL_BLACK) {
			setSkull(SKULL_NONE);
		}
	}

	sendIcons();
}

void Player::onCombatRemoveCondition(Condition* condition)
{
	// Creature::onCombatRemoveCondition(condition);
	if (condition->getId() > 0) {
		// Means the condition is from an item, id == slot
		if (g_game.getWorldType() == WORLD_TYPE_PVP_ENFORCED) {
			auto item = getInventoryItem(static_cast<slots_t>(condition->getId()));
			if (item) {
				// 25% chance to destroy the item
				if (25 >= uniform_random(1, 100)) {
					g_game.internalRemoveItem(item);
				}
			}
		}
	} else {
		if (!canDoAction()) {
			const uint32_t delay = getNextActionTime();
			const int32_t ticks = delay - (delay % EVENT_CREATURE_THINK_INTERVAL);
			if (ticks < 0) {
				removeCondition(condition);
			} else {
				condition->setTicks(ticks);
			}
		} else {
			removeCondition(condition);
		}
	}
}

void Player::onAttackedCreature(const std::shared_ptr<Creature>& target, bool addFightTicks /* = true */)
{
	Creature::onAttackedCreature(target);

	if (target->getZone() == ZONE_PVP) {
		return;
	}

	if (target.get() == this) {
		if (addFightTicks) {
			addInFightTicks();
		}
		return;
	}

	if (hasFlag(PlayerFlag_NotGainInFight)) {
		return;
	}

	auto targetPlayer = target->getPlayer();
	if (targetPlayer && !isPartner(targetPlayer) && !isGuildMate(targetPlayer)) {
		if (!pzLocked && g_game.getWorldType() == WORLD_TYPE_PVP_ENFORCED) {
			pzLocked = true;
			sendIcons();
		}

		targetPlayer->addInFightTicks();

		if (getSkull() == SKULL_NONE && getSkullClient(targetPlayer) == SKULL_YELLOW) {
			addAttacked(targetPlayer);
			targetPlayer->sendCreatureSkull(getPlayer());
		} else if (!targetPlayer->hasAttacked(getPlayer())) {
			if (!pzLocked) {
				pzLocked = true;
				sendIcons();
			}

			if (!Combat::isInPvpZone(getPlayer(), targetPlayer) && !isInWar(targetPlayer)) {
				addAttacked(targetPlayer);

				if (targetPlayer->getSkull() == SKULL_NONE && getSkull() == SKULL_NONE) {
					setSkull(SKULL_WHITE);
				}

				if (getSkull() == SKULL_NONE) {
					targetPlayer->sendCreatureSkull(getPlayer());
				}
			}
		}
	}

	if (addFightTicks) {
		addInFightTicks();
	}
}

void Player::onAttacked()
{
	Creature::onAttacked();

	addInFightTicks();
}

void Player::onIdleStatus()
{
	Creature::onIdleStatus();

	if (party) {
		party->clearPlayerPoints(getPlayer());
	}
}

void Player::onPlacedCreature()
{
	// scripting event - onLogin
	if (!g_creatureEvents->playerLogin(getPlayer())) {
		kickPlayer(true);
	}
}

void Player::onAttackedCreatureDrainHealth(const std::shared_ptr<Creature>& target, int32_t points)
{
	Creature::onAttackedCreatureDrainHealth(target, points);

	if (target) {
		if (party && !Combat::isPlayerCombat(target)) {
			auto tmpMonster = target->getMonster();
			if (tmpMonster && tmpMonster->isHostile()) {
				// We have fulfilled a requirement for shared experience
				party->updatePlayerTicks(getPlayer(), points);
			}
		}
	}
}

void Player::onTargetCreatureGainHealth(const std::shared_ptr<Creature>& target, int32_t points)
{
	if (target && party) {
		std::shared_ptr<Player> tmpPlayer = nullptr;

		if (target->getPlayer()) {
			tmpPlayer = target->getPlayer();
		} else if (auto targetMaster = target->getMaster()) {
			if (auto targetMasterPlayer = targetMaster->getPlayer()) {
				tmpPlayer = targetMasterPlayer;
			}
		}

		if (isPartner(tmpPlayer)) {
			party->updatePlayerTicks(getPlayer(), points);
		}
	}
}

bool Player::onKilledCreature(const std::shared_ptr<Creature>& target, bool lastHit /* = true*/)
{
	bool unjustified = false;

	if (hasFlag(PlayerFlag_NotGenerateLoot)) {
		target->setDropLoot(false);
	}

	Creature::onKilledCreature(target, lastHit);

	auto targetPlayer = target->getPlayer();
	if (!targetPlayer) {
		return false;
	}

	if (targetPlayer->getZone() == ZONE_PVP) {
		targetPlayer->setDropLoot(false);
		targetPlayer->setSkillLoss(false);
	} else if (!hasFlag(PlayerFlag_NotGainInFight) && !isPartner(targetPlayer)) {
		if (!Combat::isInPvpZone(getPlayer(), targetPlayer) && hasAttacked(targetPlayer) &&
		    !targetPlayer->hasAttacked(getPlayer()) && !isGuildMate(targetPlayer) && targetPlayer.get() != this) {
			if (targetPlayer->getSkull() == SKULL_NONE && !isInWar(targetPlayer)) {
				unjustified = true;
				addUnjustifiedDead(targetPlayer);
			}

			if (lastHit && hasCondition(CONDITION_INFIGHT)) {
				pzLocked = true;
				Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_INFIGHT,
				                                                  getNumber(ConfigManager::WHITE_SKULL_TIME) * 1000, 0);
				addCondition(condition);
			}
		}
	}

	return unjustified;
}

void Player::gainExperience(uint64_t gainExp, const std::shared_ptr<Creature>& source)
{
	if (hasFlag(PlayerFlag_NotGainExperience) || gainExp == 0 || staminaMinutes == 0) {
		return;
	}

	addExperience(source, gainExp, true);
}

void Player::onGainExperience(uint64_t gainExp, const std::shared_ptr<Creature>& target)
{
	if (hasFlag(PlayerFlag_NotGainExperience)) {
		return;
	}

	if (target && !target->getPlayer() && party && party->isSharedExperienceActive() &&
	    party->isSharedExperienceEnabled()) {
		party->shareExperience(gainExp, target);
		// We will get a share of the experience through the sharing mechanism
		return;
	}

	Creature::onGainExperience(gainExp, target);
	gainExperience(gainExp, target);
}

void Player::onGainSharedExperience(uint64_t gainExp, const std::shared_ptr<Creature>& source)
{
	gainExperience(gainExp, source);
}

bool Player::isImmune(CombatType_t type) const
{
	if (hasFlag(PlayerFlag_CannotBeAttacked)) {
		return true;
	}
	return Creature::isImmune(type);
}

bool Player::isImmune(ConditionType_t type) const
{
	if (hasFlag(PlayerFlag_CannotBeAttacked)) {
		return true;
	}
	return Creature::isImmune(type);
}

bool Player::isAttackable() const { return !hasFlag(PlayerFlag_CannotBeAttacked); }

bool Player::lastHitIsPlayer(const std::shared_ptr<Creature>& lastHitCreature)
{
	if (!lastHitCreature) {
		return false;
	}

	if (lastHitCreature->getPlayer()) {
		return true;
	}

	std::shared_ptr<Creature> lastHitMaster = lastHitCreature->getMaster();
	return lastHitMaster && lastHitMaster->getPlayer();
}

void Player::changeHealth(int32_t healthChange, bool sendHealthChange /* = true*/)
{
	Creature::changeHealth(healthChange, sendHealthChange);
	sendStats();
}

void Player::changeMana(int32_t manaChange)
{
	if (!hasFlag(PlayerFlag_HasInfiniteMana)) {
		if (manaChange > 0) {
			mana += std::min<int32_t>(manaChange, getMaxMana() - mana);
		} else {
			mana = std::max<int32_t>(0, mana + manaChange);
		}
	}

	sendStats();
}

void Player::changeSoul(int32_t soulChange)
{
	if (soulChange > 0) {
		soul += std::min<int32_t>(soulChange, vocation->getSoulMax() - soul);
	} else {
		soul = std::max<int32_t>(0, soul + soulChange);
	}

	sendStats();
}

bool Player::canWear(uint32_t lookType, uint8_t addons) const
{
	if (group->access) {
		return true;
	}

	const Outfit* outfit = Outfits::getInstance().getOutfitByLookType(sex, lookType);
	if (!outfit) {
		return false;
	}

	if (outfit->premium && !isPremium()) {
		return false;
	}

	if (outfit->unlocked && addons == 0) {
		return true;
	}

	for (auto& [outfitType, addon] : outfits) {
		if (outfitType == lookType) {
			if (addon == addons || addon == 3 || addons == 0) {
				return true;
			}
			return false; // have lookType on list and addons don't match
		}
	}
	return false;
}

bool Player::hasOutfit(uint32_t lookType, uint8_t addons)
{
	const Outfit* outfit = Outfits::getInstance().getOutfitByLookType(sex, lookType);
	if (!outfit) {
		return false;
	}

	if (outfit->unlocked && addons == 0) {
		return true;
	}

	for (auto& [outfitType, addon] : outfits) {
		if (outfitType == lookType) {
			if (addon == addons || addon == 3 || addons == 0) {
				return true;
			}
			return false; // have lookType on list and addons don't match
		}
	}
	return false;
}

void Player::addOutfit(uint16_t lookType, uint8_t addons)
{
	for (auto& [outfit, addon] : outfits) {
		if (outfit == lookType) {
			addon |= addons;
			return;
		}
	}
	outfits.insert(std::pair(lookType, addons));
}

bool Player::removeOutfit(uint16_t lookType)
{
	for (auto& [outfit, addon] : outfits) {
		if (outfit == lookType) {
			outfits.erase(outfit);
			return true;
		}
	}
	return false;
}

bool Player::removeOutfitAddon(uint16_t lookType, uint8_t addons)
{
	for (auto& [outfit, addon] : outfits) {
		if (outfit == lookType) {
			addon &= ~addons;
			return true;
		}
	}
	return false;
}

bool Player::getOutfitAddons(const Outfit& outfit, uint8_t& addons) const
{
	if (group->access) {
		addons = 3;
		return true;
	}

	if (outfit.premium && !isPremium()) {
		return false;
	}

	for (auto& [lookType, addon] : outfits) {
		if (lookType != outfit.lookType) {
			continue;
		}

		addons = addon;
		return true;
	}

	if (!outfit.unlocked) {
		return false;
	}

	addons = 0;
	return true;
}

void Player::setSex(PlayerSex_t newSex) { sex = newSex; }

Skulls_t Player::getSkull() const
{
	if (hasFlag(PlayerFlag_NotGainInFight)) {
		return SKULL_NONE;
	}
	return skull;
}

Skulls_t Player::getSkullClient(const std::shared_ptr<const Creature>& creature) const
{
	if (!creature || g_game.getWorldType() != WORLD_TYPE_PVP) {
		return SKULL_NONE;
	}

	const std::shared_ptr<const Player>& player = creature->getPlayer();
	if (!player || player->getSkull() != SKULL_NONE) {
		return Creature::getSkullClient(creature);
	}

	if (player->hasAttacked(getPlayer())) {
		return SKULL_YELLOW;
	}

	if (party && party == player->party) {
		return SKULL_GREEN;
	}
	return Creature::getSkullClient(creature);
}

bool Player::hasAttacked(const std::shared_ptr<const Player>& attacked) const
{
	if (hasFlag(PlayerFlag_NotGainInFight) || !attacked) {
		return false;
	}

	return attackedSet.find(attacked->guid) != attackedSet.end();
}

void Player::addAttacked(const std::shared_ptr<const Player>& attacked)
{
	if (hasFlag(PlayerFlag_NotGainInFight) || !attacked || attacked.get() == this) {
		return;
	}

	attackedSet.insert(attacked->guid);
}

void Player::removeAttacked(const std::shared_ptr<const Player>& attacked)
{
	if (!attacked || attacked.get() == this) {
		return;
	}

	auto it = attackedSet.find(attacked->guid);
	if (it != attackedSet.end()) {
		attackedSet.erase(it);
	}
}

void Player::clearAttacked() { attackedSet.clear(); }

void Player::addUnjustifiedDead(const std::shared_ptr<const Player>& attacked)
{
	if (hasFlag(PlayerFlag_NotGainInFight) || attacked.get() == this ||
	    g_game.getWorldType() == WORLD_TYPE_PVP_ENFORCED) {
		return;
	}

	sendTextMessage(MESSAGE_EVENT_ADVANCE, "Warning! The murder of " + attacked->getName() + " was not justified.");

	skullTicks += getNumber(ConfigManager::FRAG_TIME);

	if (getSkull() != SKULL_BLACK) {
		if (getNumber(ConfigManager::KILLS_TO_BLACK) != 0 &&
		    skullTicks > (getNumber(ConfigManager::KILLS_TO_BLACK) - 1) *
		                     static_cast<int64_t>(getNumber(ConfigManager::FRAG_TIME))) {
			setSkull(SKULL_BLACK);
		} else if (getSkull() != SKULL_RED && getNumber(ConfigManager::KILLS_TO_RED) != 0 &&
		           skullTicks > (getNumber(ConfigManager::KILLS_TO_RED) - 1) *
		                            static_cast<int64_t>(getNumber(ConfigManager::FRAG_TIME))) {
			setSkull(SKULL_RED);
		}
	}
}

void Player::checkSkullTicks(int64_t ticks)
{
	int64_t newTicks = skullTicks - ticks;
	if (newTicks < 0) {
		skullTicks = 0;
	} else {
		skullTicks = newTicks;
	}

	if ((skull == SKULL_RED || skull == SKULL_BLACK) && skullTicks < 1 && !hasCondition(CONDITION_INFIGHT)) {
		setSkull(SKULL_NONE);
	}
}

bool Player::isPromoted() const
{
	uint16_t promotedVocation = g_vocations.getPromotedVocation(vocation->getId());
	return promotedVocation == VOCATION_NONE && vocation->getId() != promotedVocation;
}

double Player::getLostPercent() const
{
	int32_t deathLosePercent = getNumber(ConfigManager::DEATH_LOSE_PERCENT);
	if (deathLosePercent != -1) {
		if (isPromoted()) {
			deathLosePercent -= 3;
		}

		deathLosePercent -= blessings.count();
		return std::max<int32_t>(0, deathLosePercent) / 100.;
	}

	double lossPercent;
	if (level >= 25) {
		double tmpLevel = level + (levelPercent / 100.);
		lossPercent =
		    static_cast<double>((tmpLevel + 50) * 50 * ((tmpLevel * tmpLevel) - (5 * tmpLevel) + 8)) / experience;
	} else {
		lossPercent = 10;
	}

	double percentReduction = 0;
	if (isPromoted()) {
		percentReduction += 30;
	}
	percentReduction += blessings.count() * 8;
	return lossPercent * (1 - (percentReduction / 100.)) / 100.;
}

void Player::learnInstantSpell(const std::string& spellName)
{
	if (!hasLearnedInstantSpell(spellName)) {
		learnedInstantSpellList.push_front(spellName);
	}
}

void Player::forgetInstantSpell(const std::string& spellName) { learnedInstantSpellList.remove(spellName); }

bool Player::hasLearnedInstantSpell(const std::string& spellName) const
{
	if (hasFlag(PlayerFlag_CannotUseSpells)) {
		return false;
	}

	if (hasFlag(PlayerFlag_IgnoreSpellCheck)) {
		return true;
	}

	for (const auto& learnedSpellName : learnedInstantSpellList) {
		if (caseInsensitiveEqual(learnedSpellName, spellName)) {
			return true;
		}
	}
	return false;
}

bool Player::isInWar(const std::shared_ptr<const Player>& player) const
{
	if (!player || !guild) {
		return false;
	}

	const auto& playerGuild = player->getGuild();
	if (!playerGuild) {
		return false;
	}

	return isInWarList(playerGuild->getId()) && player->isInWarList(guild->getId());
}

bool Player::isInWarList(uint32_t guildId) const
{
	return std::find(guildWarVector.begin(), guildWarVector.end(), guildId) != guildWarVector.end();
}

bool Player::isPremium() const
{
	if (getBoolean(ConfigManager::FREE_PREMIUM) || hasFlag(PlayerFlag_IsAlwaysPremium)) {
		return true;
	}

	return premiumEndsAt > time(nullptr);
}

void Player::setPremiumTime(time_t premiumEndsAt)
{
	this->premiumEndsAt = premiumEndsAt;
	sendBasicData();
}

PartyShields_t Player::getPartyShield(const std::shared_ptr<const Player>& player) const
{
	if (!player) {
		return SHIELD_NONE;
	}

	if (party) {
		if (party->getLeader() == player) {
			if (party->isSharedExperienceActive()) {
				if (party->isSharedExperienceEnabled()) {
					return SHIELD_YELLOW_SHAREDEXP;
				}

				if (party->canUseSharedExperience(player)) {
					return SHIELD_YELLOW_NOSHAREDEXP;
				}

				return SHIELD_YELLOW_NOSHAREDEXP_BLINK;
			}

			return SHIELD_YELLOW;
		}

		if (player->party == party) {
			if (party->isSharedExperienceActive()) {
				if (party->isSharedExperienceEnabled()) {
					return SHIELD_BLUE_SHAREDEXP;
				}

				if (party->canUseSharedExperience(player)) {
					return SHIELD_BLUE_NOSHAREDEXP;
				}

				return SHIELD_BLUE_NOSHAREDEXP_BLINK;
			}

			return SHIELD_BLUE;
		}

		// isInviting(player) if members aren't supposed to see the invited player emblem
		if (party->isPlayerInvited(player)) {
			return SHIELD_WHITEBLUE;
		}
	}

	if (player->isInviting(getPlayer())) {
		return SHIELD_WHITEYELLOW;
	}

	if (player->party) {
		return SHIELD_GRAY;
	}

	return SHIELD_NONE;
}

bool Player::isInviting(const std::shared_ptr<const Player>& player) const
{
	if (!player || !party || party->getLeader().get() != this) {
		return false;
	}
	return party->isPlayerInvited(player);
}

bool Player::isPartner(const std::shared_ptr<const Player>& player) const
{
	if (!player || !party || player.get() == this) {
		return false;
	}
	return party == player->party;
}

bool Player::isGuildMate(const std::shared_ptr<const Player>& player) const
{
	if (!player || !guild) {
		return false;
	}
	return guild == player->guild;
}

void Player::sendPlayerPartyIcons(const std::shared_ptr<Player>& player)
{
	sendCreatureShield(player);
	sendCreatureSkull(player);
}

bool Player::addPartyInvitation(Party* party)
{
	auto it = std::find(invitePartyList.begin(), invitePartyList.end(), party);
	if (it != invitePartyList.end()) {
		return false;
	}

	invitePartyList.push_front(party);
	return true;
}

void Player::removePartyInvitation(Party* party) { invitePartyList.remove(party); }

void Player::clearPartyInvitations()
{
	for (Party* invitingParty : invitePartyList) {
		invitingParty->removeInvite(getPlayer(), false);
	}
	invitePartyList.clear();
}

GuildEmblems_t Player::getGuildEmblem(const std::shared_ptr<const Player>& player) const
{
	if (!player) {
		return GUILDEMBLEM_NONE;
	}

	const auto& playerGuild = player->getGuild();
	if (!playerGuild) {
		return GUILDEMBLEM_NONE;
	}

	if (player->getGuildWarVector().empty()) {
		if (guild == playerGuild) {
			return GUILDEMBLEM_MEMBER;
		}
		return GUILDEMBLEM_OTHER;
	} else if (guild == playerGuild) {
		return GUILDEMBLEM_ALLY;
	} else if (isInWar(player)) {
		return GUILDEMBLEM_ENEMY;
	}

	return GUILDEMBLEM_NEUTRAL;
}

uint16_t Player::getRandomMount() const
{
	std::vector<uint16_t> mountsId;
	for (const Mount& mount : g_game.mounts.getMounts()) {
		if (hasMount(&mount)) {
			mountsId.push_back(mount.id);
		}
	}

	return mountsId[uniform_random(0, mountsId.size() - 1)];
}

uint16_t Player::getCurrentMount() const { return currentMount; }

void Player::setCurrentMount(uint16_t mountId) { currentMount = mountId; }

bool Player::toggleMount(bool mount)
{
	if ((OTSYS_TIME() - lastToggleMount) < 3000 && !wasMounted) {
		sendCancelMessage(RETURNVALUE_YOUAREEXHAUSTED);
		return false;
	}

	if (mount) {
		if (isMounted()) {
			return false;
		}

		if (!group->access && tile->hasFlag(TILESTATE_PROTECTIONZONE)) {
			sendCancelMessage(RETURNVALUE_ACTIONNOTPERMITTEDINPROTECTIONZONE);
			return false;
		}

		const Outfit* playerOutfit = Outfits::getInstance().getOutfitByLookType(getSex(), defaultOutfit.lookType);
		if (!playerOutfit) {
			return false;
		}

		uint16_t currentMountId = getCurrentMount();
		if (currentMountId == 0) {
			sendOutfitWindow();
			return false;
		}

		if (randomizeMount) {
			currentMountId = getRandomMount();
		}

		Mount* currentMount = g_game.mounts.getMountByID(currentMountId);
		if (!currentMount) {
			return false;
		}

		if (!hasMount(currentMount)) {
			setCurrentMount(0);
			sendOutfitWindow();
			return false;
		}

		if (currentMount->premium && !isPremium()) {
			sendCancelMessage(RETURNVALUE_YOUNEEDPREMIUMACCOUNT);
			return false;
		}

		if (hasCondition(CONDITION_OUTFIT)) {
			sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
			return false;
		}

		defaultOutfit.lookMount = currentMount->clientId;

		if (currentMount->speed != 0) {
			g_game.changeSpeed(getPlayer(), currentMount->speed);
		}
	} else {
		if (!isMounted()) {
			return false;
		}

		dismount();
	}

	g_game.internalCreatureChangeOutfit(getPlayer(), defaultOutfit);
	lastToggleMount = OTSYS_TIME();
	return true;
}

bool Player::tameMount(uint16_t mountId)
{
	Mount* mount = g_game.mounts.getMountByID(mountId);
	if (!mount || hasMount(mount)) {
		return false;
	}

	mounts.insert(mountId);
	return true;
}

bool Player::untameMount(uint16_t mountId)
{
	Mount* mount = g_game.mounts.getMountByID(mountId);
	if (!mount || !hasMount(mount)) {
		return false;
	}

	mounts.erase(mountId);

	if (getCurrentMount() == mountId) {
		if (isMounted()) {
			dismount();
			g_game.internalCreatureChangeOutfit(getPlayer(), defaultOutfit);
		}

		setCurrentMount(0);
	}

	return true;
}

bool Player::hasMount(const Mount* mount) const
{
	if (isAccessPlayer()) {
		return true;
	}

	if (mount->premium && !isPremium()) {
		return false;
	}

	return mounts.find(mount->id) != mounts.end();
}

bool Player::hasMounts() const
{
	for (const Mount& mount : g_game.mounts.getMounts()) {
		if (hasMount(&mount)) {
			return true;
		}
	}
	return false;
}

void Player::dismount()
{
	Mount* mount = g_game.mounts.getMountByID(getCurrentMount());
	if (mount && mount->speed > 0) {
		g_game.changeSpeed(getPlayer(), -mount->speed);
	}

	defaultOutfit.lookMount = 0;
}

bool Player::addOfflineTrainingTries(skills_t skill, uint64_t tries)
{
	if (tries == 0 || skill == SKILL_LEVEL) {
		return false;
	}

	bool sendUpdate = false;
	uint32_t oldSkillValue, newSkillValue;
	long double oldPercentToNextLevel, newPercentToNextLevel;

	if (skill == SKILL_MAGLEVEL) {
		uint64_t currReqMana = vocation->getReqMana(magLevel);
		uint64_t nextReqMana = vocation->getReqMana(magLevel + 1);

		if (currReqMana >= nextReqMana) {
			return false;
		}

		oldSkillValue = magLevel;
		oldPercentToNextLevel = static_cast<long double>(manaSpent * 100) / nextReqMana;

		tfs::events::player::onGainSkillTries(getPlayer(), SKILL_MAGLEVEL, tries);
		uint32_t currMagLevel = magLevel;

		while ((manaSpent + tries) >= nextReqMana) {
			tries -= nextReqMana - manaSpent;

			magLevel++;
			manaSpent = 0;

			g_creatureEvents->playerAdvance(getPlayer(), SKILL_MAGLEVEL, magLevel - 1, magLevel);

			sendUpdate = true;
			currReqMana = nextReqMana;
			nextReqMana = vocation->getReqMana(magLevel + 1);

			if (currReqMana >= nextReqMana) {
				tries = 0;
				break;
			}
		}

		manaSpent += tries;

		if (magLevel != currMagLevel) {
			sendTextMessage(MESSAGE_EVENT_ADVANCE, fmt::format("You advanced to magic level {:d}.", magLevel));
		}

		uint8_t newPercent;
		if (nextReqMana > currReqMana) {
			newPercent = Player::getBasisPointLevel(manaSpent, nextReqMana);
			newPercentToNextLevel = static_cast<long double>(manaSpent * 100) / nextReqMana;
		} else {
			newPercent = 0;
			newPercentToNextLevel = 0;
		}

		if (newPercent != magLevelPercent) {
			magLevelPercent = newPercent;
			sendUpdate = true;
		}

		newSkillValue = magLevel;
	} else {
		uint64_t currReqTries = vocation->getReqSkillTries(skill, skills[skill].level);
		uint64_t nextReqTries = vocation->getReqSkillTries(skill, skills[skill].level + 1);
		if (currReqTries >= nextReqTries) {
			return false;
		}

		oldSkillValue = skills[skill].level;
		oldPercentToNextLevel = static_cast<long double>(skills[skill].tries * 100) / nextReqTries;

		tfs::events::player::onGainSkillTries(getPlayer(), skill, tries);
		uint32_t currSkillLevel = skills[skill].level;

		while ((skills[skill].tries + tries) >= nextReqTries) {
			tries -= nextReqTries - skills[skill].tries;

			skills[skill].level++;
			skills[skill].tries = 0;
			skills[skill].percent = 0;

			g_creatureEvents->playerAdvance(getPlayer(), skill, (skills[skill].level - 1), skills[skill].level);

			sendUpdate = true;
			currReqTries = nextReqTries;
			nextReqTries = vocation->getReqSkillTries(skill, skills[skill].level + 1);

			if (currReqTries >= nextReqTries) {
				tries = 0;
				break;
			}
		}

		skills[skill].tries += tries;

		if (currSkillLevel != skills[skill].level) {
			sendTextMessage(MESSAGE_EVENT_ADVANCE,
			                fmt::format("You advanced to {:s} level {:d}.", getSkillName(skill), skills[skill].level));
		}

		uint8_t newPercent;
		if (nextReqTries > currReqTries) {
			newPercent = Player::getBasisPointLevel(skills[skill].tries, nextReqTries);
			newPercentToNextLevel = static_cast<long double>(skills[skill].tries * 100) / nextReqTries;
		} else {
			newPercent = 0;
			newPercentToNextLevel = 0;
		}

		if (skills[skill].percent != newPercent) {
			skills[skill].percent = newPercent;
			sendUpdate = true;
		}

		newSkillValue = skills[skill].level;
	}

	if (sendUpdate) {
		sendSkills();
	}

	sendTextMessage(
	    MESSAGE_EVENT_ADVANCE,
	    fmt::format(
	        "Your {:s} skill changed from level {:d} (with {:.2f}% progress towards level {:d}) to level {:d} (with {:.2f}% progress towards level {:d})",
	        ucwords(getSkillName(skill)), oldSkillValue, oldPercentToNextLevel, (oldSkillValue + 1), newSkillValue,
	        newPercentToNextLevel, (newSkillValue + 1)));
	return sendUpdate;
}

bool Player::hasModalWindowOpen(uint32_t modalWindowId) const
{
	return find(modalWindows.begin(), modalWindows.end(), modalWindowId) != modalWindows.end();
}

void Player::onModalWindowHandled(uint32_t modalWindowId) { modalWindows.remove(modalWindowId); }

void Player::sendModalWindow(const ModalWindow& modalWindow)
{
	if (!client) {
		return;
	}

	modalWindows.push_front(modalWindow.id);
	client->sendModalWindow(modalWindow);
}

void Player::clearModalWindows() { modalWindows.clear(); }

void Player::sendClosePrivate(uint16_t channelId)
{
	if (channelId == CHANNEL_GUILD || channelId == CHANNEL_PARTY) {
		g_chat->removeUserFromChannel(getPlayer(), channelId);
	}

	if (client) {
		client->sendClosePrivate(channelId);
	}
}

uint64_t Player::getMoney() const
{
	std::vector<std::shared_ptr<const Container>> containers;
	uint64_t moneyCount = 0;

	for (int32_t i = CONST_SLOT_FIRST; i <= CONST_SLOT_LAST; ++i) {
		auto item = inventory[i];
		if (!item) {
			continue;
		}

		auto container = item->getContainer();
		if (container) {
			containers.push_back(container);
		} else {
			moneyCount += item->getWorth();
		}
	}

	size_t i = 0;
	while (i < containers.size()) {
		auto container = containers[i++];
		for (const auto& item : container->getItemList()) {
			auto tmpContainer = item->getContainer();
			if (tmpContainer) {
				containers.push_back(tmpContainer);
			} else {
				moneyCount += item->getWorth();
			}
		}
	}
	return moneyCount;
}

size_t Player::getMaxVIPEntries() const
{
	if (group->maxVipEntries != 0) {
		return group->maxVipEntries;
	}

	return getNumber(isPremium() ? ConfigManager::VIP_PREMIUM_LIMIT : ConfigManager::VIP_FREE_LIMIT);
}

size_t Player::getMaxDepotItems() const
{
	if (group->maxDepotItems != 0) {
		return group->maxDepotItems;
	}

	return getNumber(isPremium() ? ConfigManager::DEPOT_PREMIUM_LIMIT : ConfigManager::DEPOT_FREE_LIMIT);
}

std::forward_list<Condition*> Player::getMuteConditions() const
{
	std::forward_list<Condition*> muteConditions;
	for (Condition* condition : conditions) {
		if (condition->getTicks() <= 0) {
			continue;
		}

		ConditionType_t type = condition->getType();
		if (type != CONDITION_MUTED && type != CONDITION_CHANNELMUTEDTICKS && type != CONDITION_YELLTICKS) {
			continue;
		}

		muteConditions.push_front(condition);
	}
	return muteConditions;
}

void Player::setGuild(Guild_ptr guild)
{
	if (guild == this->guild) {
		return;
	}

	auto oldGuild = this->guild;

	this->guildNick.clear();
	this->guild = nullptr;
	this->guildRank = nullptr;

	if (guild) {
		const auto& rank = guild->getRankByLevel(Guild::MEMBER_RANK_LEVEL_DEFAULT);
		if (!rank) {
			return;
		}

		this->guild = guild;
		this->guildRank = rank;
		guild->addMember(getPlayer());
	}

	if (oldGuild) {
		oldGuild->removeMember(getPlayer());
	}
}

void Player::updateRegeneration()
{
	if (!vocation) {
		return;
	}

	Condition* condition = getCondition(CONDITION_REGENERATION, CONDITIONID_DEFAULT);
	if (condition) {
		condition->setParam(CONDITION_PARAM_HEALTHGAIN, vocation->getHealthGainAmount());
		condition->setParam(CONDITION_PARAM_HEALTHTICKS, vocation->getHealthGainTicks() * 1000);
		condition->setParam(CONDITION_PARAM_MANAGAIN, vocation->getManaGainAmount());
		condition->setParam(CONDITION_PARAM_MANATICKS, vocation->getManaGainTicks() * 1000);
	}
}
