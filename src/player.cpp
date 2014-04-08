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

#include "beds.h"
#include "chat.h"
#include "combat.h"
#include "configmanager.h"
#include "creatureevent.h"
#include "game.h"
#include "house.h"
#include "iologindata.h"
#include "monster.h"
#include "movement.h"
#include "outputmessage.h"
#include "player.h"
#include "quests.h"
#include "scheduler.h"
#include "town.h"
#include "weapons.h"

extern ConfigManager g_config;
extern Game g_game;
extern Chat g_chat;
extern Vocations g_vocations;
extern MoveEvents* g_moveEvents;
extern Weapons* g_weapons;
extern CreatureEvents* g_creatureEvents;

MuteCountMap Player::muteCountMap;

uint32_t Player::playerAutoID = 0x10000000;

#ifdef ENABLE_SERVER_DIAGNOSTIC
uint32_t Player::playerCount = 0;
#endif

Player::Player(ProtocolGame* p) :
	Creature()
{
	client = p;
	isConnecting = false;

	if (client) {
		client->setPlayer(this);
	}

	accountNumber = 0;
	setVocation(0);
	capacity = 400.00;
	mana = 0;
	manaMax = 0;
	manaSpent = 0;
	soul = 0;
	soulMax = 100;
	guildLevel = 0;
	guild = nullptr;

	level = 1;
	levelPercent = 0;
	magLevelPercent = 0;
	magLevel = 0;
	experience = 0;

	damageImmunities = 0;
	conditionImmunities = 0;
	conditionSuppressions = 0;
	group = nullptr;
	lastLoginSaved = 0;
	lastLogout = 0;
	lastIP = 0;
	lastPing = OTSYS_TIME();
	lastPong = lastPing;
	MessageBufferTicks = 0;
	MessageBufferCount = 0;
	nextAction = 0;

	windowTextId = 0;
	writeItem = nullptr;
	maxWriteLen = 0;

	editHouse = nullptr;
	editListId = 0;

	shopOwner = nullptr;
	purchaseCallback = -1;
	saleCallback = -1;

	pzLocked = false;
	bloodHitCount = 0;
	shieldBlockCount = 0;
	lastAttackBlockType = BLOCK_NONE;
	addAttackSkillPoint = false;
	lastAttack = 0;
	shootRange = 1;

	blessings = 0;

	mayNotMove = false;

	inMarket = false;
	lastDepotId = -1;

	chaseMode = CHASEMODE_STANDSTILL;
	fightMode = FIGHTMODE_ATTACK;

	bedItem = nullptr;

	tradePartner = nullptr;
	tradeState = TRADE_NONE;
	tradeItem = nullptr;

	walkTask = nullptr;
	walkTaskEvent = 0;
	actionTaskEvent = 0;
	nextStepEvent = 0;

	lastFailedFollow = 0;
	lastWalkthroughAttempt = 0;
	lastToggleMount = 0;

	for (int32_t i = 0; i < 11; i++) {
		inventory[i] = nullptr;
		inventoryAbilities[i] = false;
	}

	for (int32_t i = SKILL_FIRST; i <= SKILL_LAST; ++i) {
		skills[i][SKILLVALUE_LEVEL] = 10;
		skills[i][SKILLVALUE_TRIES] = 0;
		skills[i][SKILLVALUE_PERCENT] = 0;
	}

	for (int32_t i = SKILL_FIRST; i <= SKILL_LAST; ++i) {
		varSkills[i] = 0;
	}

	for (int32_t i = STAT_FIRST; i <= STAT_LAST; ++i) {
		varStats[i] = 0;
	}

	maxDepotItems = 1000;
	maxVipEntries = 20;

	sex = PLAYERSEX_FEMALE;

	town = nullptr;

	accountType = ACCOUNT_TYPE_NORMAL;
	premiumDays = 0;

	idleTime = 0;

	skullTicks = 0;
	skull = SKULL_NONE;
	setParty(nullptr);

	bankBalance = 0;

	inbox = new Inbox(ITEM_INBOX);
	inbox->useThing2();

	offlineTrainingSkill = -1;
	offlineTrainingTime = 0;
	lastStatsTrainingTime = 0;

	ghostMode = false;
	requestedOutfit = false;

	staminaMinutes = 2520;
	nextUseStaminaTime = 0;

	lastQuestlogUpdate = 0;

#ifdef ENABLE_SERVER_DIAGNOSTIC
	playerCount++;
#endif
}

Player::~Player()
{
	for (int i = 0; i < 11; ++i) {
		if (inventory[i]) {
			inventory[i]->setParent(nullptr);
			inventory[i]->releaseThing2();
			inventory[i] = nullptr;
			inventoryAbilities[i] = false;
		}
	}

	for (const auto& it : depotLockerMap) {
		it.second->removeInbox(inbox);
		it.second->releaseThing2();
	}

	inbox->releaseThing2();

	//std::cout << "Player destructor " << this << std::endl;

	setWriteItem(nullptr);
	setEditHouse(nullptr);

#ifdef ENABLE_SERVER_DIAGNOSTIC
	playerCount--;
#endif
}

bool Player::setVocation(uint32_t vocId)
{
	Vocation* voc = g_vocations.getVocation(vocId);
	if (!voc) {
		return false;
	}
	vocation = voc;

	Condition* condition = getCondition(CONDITION_REGENERATION, CONDITIONID_DEFAULT);
	if (condition) {
		condition->setParam(CONDITION_PARAM_HEALTHGAIN, vocation->getHealthGainAmount());
		condition->setParam(CONDITION_PARAM_HEALTHTICKS, vocation->getHealthGainTicks() * 1000);
		condition->setParam(CONDITION_PARAM_MANAGAIN, vocation->getManaGainAmount());
		condition->setParam(CONDITION_PARAM_MANATICKS, vocation->getManaGainTicks() * 1000);
	}

	soulMax = vocation->getSoulMax();
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

	if (guild) {
		const GuildRank* rank = guild->getRankByLevel(guildLevel);
		if (rank) {
			if (lookDistance == -1) {
				s << " You are ";
			} else if (sex == PLAYERSEX_FEMALE) {
				s << " She is ";
			} else {
				s << " He is ";
			}

			s << rank->name << " of the " << guild->getName();
			if (!guildNick.empty()) {
				s << " (" << guildNick << ')';
			}

			size_t memberCount = guild->getMemberCount();
			if (memberCount == 1) {
				s << ", which has 1 member, " << guild->getMembersOnline().size() << " of them online.";
			} else {
				s << ", which has " << memberCount << " members, " << guild->getMembersOnline().size() << " of them online.";
			}
		}
	}
	return s.str();
}

Item* Player::getInventoryItem(slots_t slot) const
{
	if (slot < CONST_SLOT_FIRST || slot > CONST_SLOT_LAST) {
		return nullptr;
	}
	return inventory[slot];
}

void Player::setConditionSuppressions(uint32_t conditions, bool remove)
{
	if (!remove) {
		conditionSuppressions |= conditions;
	} else {
		conditionSuppressions &= ~conditions;
	}
}

Item* Player::getWeapon(bool ignoreAmmo /*= false*/)
{
	for (uint32_t slot = CONST_SLOT_RIGHT; slot <= CONST_SLOT_LEFT; slot++) {
		Item* item = inventory[slot];
		if (!item) {
			continue;
		}

		switch (item->getWeaponType()) {
			case WEAPON_SWORD:
			case WEAPON_AXE:
			case WEAPON_CLUB:
			case WEAPON_WAND: {
				const Weapon* weapon = g_weapons->getWeapon(item);
				if (weapon) {
					return item;
				}
				break;
			}

			case WEAPON_DISTANCE: {
				const ItemType& it = Item::items[item->getID()];
				if (!ignoreAmmo && it.ammoType != AMMO_NONE) {
					Item* ammoItem = inventory[CONST_SLOT_AMMO];
					if (ammoItem && ammoItem->getAmmoType() == it.ammoType) {
						const Weapon* weapon = g_weapons->getWeapon(ammoItem);
						if (weapon) {
							shootRange = it.shootRange;
							return ammoItem;
						}
					}
				} else {
					const Weapon* weapon = g_weapons->getWeapon(item);
					if (weapon) {
						shootRange = it.shootRange;
						return item;
					}
				}
				break;
			}

			default:
				break;
		}
	}
	return nullptr;
}

WeaponType_t Player::getWeaponType()
{
	Item* item = getWeapon();
	if (!item) {
		return WEAPON_NONE;
	}
	return item->getWeaponType();
}

int32_t Player::getWeaponSkill(const Item* item) const
{
	if (!item) {
		return getSkill(SKILL_FIST, SKILLVALUE_LEVEL);
	}

	int32_t attackSkill;

	WeaponType_t weaponType = item->getWeaponType();
	switch (weaponType) {
		case WEAPON_SWORD: {
			attackSkill = getSkill(SKILL_SWORD, SKILLVALUE_LEVEL);
			break;
		}

		case WEAPON_CLUB: {
			attackSkill = getSkill(SKILL_CLUB, SKILLVALUE_LEVEL);
			break;
		}

		case WEAPON_AXE: {
			attackSkill = getSkill(SKILL_AXE, SKILLVALUE_LEVEL);
			break;
		}

		case WEAPON_DISTANCE: {
			attackSkill = getSkill(SKILL_DISTANCE, SKILLVALUE_LEVEL);
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

	static const slots_t armorSlots[] = {CONST_SLOT_HEAD, CONST_SLOT_NECKLACE, CONST_SLOT_ARMOR, CONST_SLOT_LEGS, CONST_SLOT_FEET, CONST_SLOT_RING};
	for (slots_t slot : armorSlots) {
		Item* inventoryItem = inventory[slot];
		if (inventoryItem) {
			armor += inventoryItem->getArmor();
		}
	}
	return int32_t(armor * vocation->armorMultiplier);
}

void Player::getShieldAndWeapon(const Item*& shield, const Item*& weapon) const
{
	shield = nullptr;
	weapon = nullptr;

	for (uint32_t slot = CONST_SLOT_RIGHT; slot <= CONST_SLOT_LEFT; slot++) {
		Item* item = inventory[slot];
		if (!item) {
			continue;
		}

		switch (item->getWeaponType()) {
			case WEAPON_NONE:
				break;

			case WEAPON_SHIELD: {
				if (!shield || (shield && item->getDefense() > shield->getDefense())) {
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
}

int32_t Player::getDefense() const
{
	int32_t baseDefense = 5;
	int32_t defenseValue = 0;
	int32_t defenseSkill = 0;
	int32_t extraDefense = 0;
	float defenseFactor = getDefenseFactor();
	const Item* weapon;
	const Item* shield;
	getShieldAndWeapon(shield, weapon);

	if (weapon) {
		defenseValue = baseDefense + weapon->getDefense();
		extraDefense = weapon->getExtraDefense();
		defenseSkill = getWeaponSkill(weapon);
	}

	if (shield && shield->getDefense() >= defenseValue) {
		defenseValue = baseDefense + shield->getDefense() + extraDefense;
		defenseSkill = getSkill(SKILL_SHIELD, SKILLVALUE_LEVEL);
	}

	if (defenseSkill == 0) {
		return 0;
	}

	defenseValue = int32_t(defenseValue * vocation->defenseMultiplier);
	return ((int32_t)std::ceil(((float)(defenseSkill * (defenseValue * 0.015)) + (defenseValue * 0.1)) * defenseFactor));
}

float Player::getAttackFactor() const
{
	switch (fightMode) {
		case FIGHTMODE_ATTACK: return 1.0f;
		case FIGHTMODE_BALANCED: return 1.2f;
		case FIGHTMODE_DEFENSE: return 2.0f;
		default: return 1.0f;
	}
}

float Player::getDefenseFactor() const
{
	switch (fightMode) {
		case FIGHTMODE_ATTACK: return 1.0f;
		case FIGHTMODE_BALANCED: return 1.2f;
		case FIGHTMODE_DEFENSE: {
			if ((OTSYS_TIME() - lastAttack) < getAttackSpeed()) {
				return 1.0f;
			}

			return 2.0f;
		}
		default: return 1.0f;
	}
}

uint16_t Player::getClientIcons() const
{
	uint16_t icons = 0;
	for (Condition* condition : conditions) {
		if (!isSuppress(condition->getType())) {
			icons |= condition->getIcons();
		}
	}

	if (pzLocked) {
		icons |= ICON_REDSWORDS;
	}

	if (_tile->hasFlag(TILESTATE_PROTECTIONZONE)) {
		icons |= ICON_PIGEON;

		// Don't show ICON_SWORDS if player is in protection zone.
		if (hasBitSet(ICON_SWORDS, icons)) {
			icons &= ~ICON_SWORDS;
		}
	}

	// Game client debugs with 10 or more icons
	// so let's prevent that from happening.
	std::bitset<20> icon_bitset(static_cast<uint64_t>(icons));
	for (size_t pos = 0, bits_set = icon_bitset.count(); bits_set >= 10; ++pos) {
		if (icon_bitset[pos]) {
			icon_bitset.reset(pos);
			--bits_set;
		}
	}
	return icon_bitset.to_ulong();
}

void Player::updateInventoryWeight()
{
	if (!hasFlag(PlayerFlag_HasInfiniteCapacity)) {
		inventoryWeight = 0.00;

		for (int i = CONST_SLOT_FIRST; i <= CONST_SLOT_LAST; ++i) {
			Item* item = inventory[i];
			if (item) {
				inventoryWeight += item->getWeight();
			}
		}
	}
}

int32_t Player::getPlayerInfo(playerinfo_t playerinfo) const
{
	switch (playerinfo) {
		case PLAYERINFO_LEVEL: return level;
		case PLAYERINFO_LEVELPERCENT: return levelPercent;
		case PLAYERINFO_MAGICLEVEL: return std::max<int32_t>(0, magLevel + varStats[STAT_MAGICPOINTS]);
		case PLAYERINFO_MAGICLEVELPERCENT: return magLevelPercent;
		case PLAYERINFO_HEALTH: return health;
		case PLAYERINFO_MAXHEALTH: return std::max<int32_t>(1, healthMax + varStats[STAT_MAXHITPOINTS]);
		case PLAYERINFO_MANA: return mana;
		case PLAYERINFO_MAXMANA: return std::max<int32_t>(0, manaMax + varStats[STAT_MAXMANAPOINTS]);
		case PLAYERINFO_SOUL: return getSoul();
		default: return 0;
	}
}

int32_t Player::getSkill(skills_t skilltype, skillsid_t skillinfo) const
{
	int32_t n = skills[skilltype][skillinfo];
	if (skillinfo == SKILLVALUE_LEVEL) {
		n += varSkills[skilltype];
	}
	return std::max<int32_t>(0, n);
}

void Player::addSkillAdvance(skills_t skill, uint32_t count)
{
	if (count == 0) {
		return;
	}

	uint64_t currReqTries = vocation->getReqSkillTries(skill, skills[skill][SKILLVALUE_LEVEL]);
	uint64_t nextReqTries = vocation->getReqSkillTries(skill, skills[skill][SKILLVALUE_LEVEL] + 1);
	if (currReqTries >= nextReqTries) {
		//player has reached max skill
		return;
	}

	bool sendUpdateSkills = false;
	while ((skills[skill][SKILLVALUE_TRIES] + count) >= nextReqTries) {
		count -= nextReqTries - skills[skill][SKILLVALUE_TRIES];
		skills[skill][SKILLVALUE_LEVEL]++;
		skills[skill][SKILLVALUE_TRIES] = 0;
		skills[skill][SKILLVALUE_PERCENT] = 0;

		std::ostringstream ss;
		ss << "You advanced to " << getSkillName(skill) << " level " << skills[skill][SKILLVALUE_LEVEL] << '.';
		sendTextMessage(MESSAGE_EVENT_ADVANCE, ss.str());

		g_creatureEvents->playerAdvance(this, skill, (skills[skill][SKILLVALUE_LEVEL] - 1), skills[skill][SKILLVALUE_LEVEL]);

		sendUpdateSkills = true;
		currReqTries = nextReqTries;
		nextReqTries = vocation->getReqSkillTries(skill, skills[skill][SKILLVALUE_LEVEL] + 1);
		if (currReqTries >= nextReqTries) {
			count = 0;
			break;
		}
	}

	skills[skill][SKILLVALUE_TRIES] += count;

	uint32_t newPercent;
	if (nextReqTries > currReqTries) {
		newPercent = Player::getPercentLevel(skills[skill][SKILLVALUE_TRIES], nextReqTries);
	} else {
		newPercent = 0;
	}

	if (skills[skill][SKILLVALUE_PERCENT] != newPercent) {
		skills[skill][SKILLVALUE_PERCENT] = newPercent;
		sendUpdateSkills = true;
	}

	if (sendUpdateSkills) {
		sendSkills();
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
				g_game.addCreatureHealth(this);
			}
			break;
		}

		case STAT_MAXMANAPOINTS: {
			if (getMana() > getMaxMana()) {
				Creature::changeMana(getMaxMana() - getMana());
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
		case STAT_MAXHITPOINTS: return getMaxHealth() - varStats[STAT_MAXHITPOINTS];
		case STAT_MAXMANAPOINTS: return getMaxMana() - varStats[STAT_MAXMANAPOINTS];
		case STAT_SOULPOINTS: return getPlayerInfo(PLAYERINFO_SOUL) - varStats[STAT_SOULPOINTS];
		case STAT_MAGICPOINTS: return getBaseMagicLevel();
		default: return 0;
	}
}

void Player::addContainer(uint8_t cid, Container* container)
{
	if (cid > 0xF) {
		return;
	}

	if (container->getID() == ITEM_BROWSEFIELD) {
		container->useThing2();
	}

	auto it = openContainers.find(cid);
	if (it != openContainers.end()) {
		OpenContainer& openContainer = it->second;
		Container* oldContainer = openContainer.container;
		if (oldContainer->getID() == ITEM_BROWSEFIELD) {
			oldContainer->releaseThing2();
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
	Container* container = openContainer.container;
	openContainers.erase(it);

	if (container && container->getID() == ITEM_BROWSEFIELD) {
		container->releaseThing2();
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

Container* Player::getContainerByID(uint8_t cid)
{
	auto it = openContainers.find(cid);
	if (it == openContainers.end()) {
		return nullptr;
	}
	return it->second.container;
}

int8_t Player::getContainerID(const Container* container) const
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
	} else {
		return ITEM_MALE_CORPSE;
	}
}

uint16_t Player::getDropPercent() const
{
	uint16_t dropPercent;

	std::bitset<5> bitset(blessings);
	switch (bitset.count()) {
		case 1:
			dropPercent = 70;
			break;

		case 2:
			dropPercent = 45;
			break;

		case 3:
			dropPercent = 25;
			break;

		case 4:
			dropPercent = 10;
			break;

		case 5:
			dropPercent = 0;
			break;

		default:
			dropPercent = 100;
			break;
	}
	return dropPercent;
}

void Player::dropLoot(Container* corpse, Creature* _lastHitCreature)
{
	if (corpse && lootDrop && vocation->getId() != VOCATION_NONE) {
		Skulls_t playerSkull = getSkull();
		if (inventory[CONST_SLOT_NECKLACE] && inventory[CONST_SLOT_NECKLACE]->getID() == ITEM_AMULETOFLOSS && playerSkull != SKULL_RED && playerSkull != SKULL_BLACK) {
			Player* lastHitPlayer;

			if (_lastHitCreature) {
				lastHitPlayer = _lastHitCreature->getPlayer();
				if (!lastHitPlayer) {
					Creature* lastHitMaster = _lastHitCreature->getMaster();
					if (lastHitMaster) {
						lastHitPlayer = lastHitMaster->getPlayer();
					}
				}
			} else {
				lastHitPlayer = nullptr;
			}

			if (!lastHitPlayer || blessings < 32) {
				g_game.internalRemoveItem(inventory[CONST_SLOT_NECKLACE], 1);
			}
		} else {
			for (int32_t i = CONST_SLOT_FIRST; i <= CONST_SLOT_LAST; ++i) {
				Item* item = inventory[i];
				if (item) {
					if (playerSkull == SKULL_RED || playerSkull == SKULL_BLACK || uniform_random(1, (item->getContainer() ? 100 : 1000)) <= getDropPercent()) {
						g_game.internalMoveItem(this, corpse, INDEX_WHEREEVER, item, item->getItemCount(), 0);
						sendInventoryItem((slots_t)i, nullptr);
					}
				}
			}
		}
	}

	if (!inventory[CONST_SLOT_BACKPACK]) {
		__internalAddThing(CONST_SLOT_BACKPACK, Item::CreateItem(ITEM_BAG));
	}
}

void Player::addStorageValue(const uint32_t key, const int32_t value, const bool isLogin/* = false*/)
{
	if (IS_IN_KEYRANGE(key, RESERVED_RANGE)) {
		if (IS_IN_KEYRANGE(key, OUTFITS_RANGE)) {
			outfits.emplace_back(
				value >> 16,
				value & 0xFF
			);
			return;
		} else if (IS_IN_KEYRANGE(key, MOUNTS_RANGE)) {
			// do nothing
		} else {
			std::cout << "Warning: unknown reserved key: " << key << " player: " << getName() << std::endl;
			return;
		}
	}

	if (value != -1) {
		int32_t oldValue;
		getStorageValue(key, oldValue);

		storageMap[key] = value;

		if (!isLogin) {
			int64_t currentFrameTime = OutputMessagePool::getInstance()->getFrameTime();
			if (lastQuestlogUpdate != currentFrameTime && Quests::getInstance()->isQuestStorage(key, value, oldValue)) {
				lastQuestlogUpdate = currentFrameTime;
				sendTextMessage(MESSAGE_EVENT_ADVANCE, "Your questlog has been updated.");
			}
		}
	} else {
		storageMap.erase(key);
	}
}

bool Player::getStorageValue(const uint32_t key, int32_t& value) const
{
	auto it = storageMap.find(key);
	if (it == storageMap.end()) {
		value = -1;
		return false;
	}

	value = it->second;
	return true;
}

bool Player::canSee(const Position& pos) const
{
	if (!client) {
		return false;
	}
	return client->canSee(pos);
}

bool Player::canSeeCreature(const Creature* creature) const
{
	if (creature == this) {
		return true;
	}

	if (creature->isInGhostMode() && !group->access) {
		return false;
	}

	if (!creature->getPlayer() && !canSeeInvisibility() && creature->isInvisible()) {
		return false;
	}
	return true;
}

bool Player::canWalkthrough(const Creature* creature) const
{
	if (group->access || creature->isInGhostMode()) {
		return true;
	}

	const Player* player = creature->getPlayer();
	if (!player) {
		return false;
	}

	const Tile* playerTile = player->getTile();
	if (playerTile && playerTile->hasFlag(TILESTATE_PROTECTIONZONE)) {
		Item* playerTileGround = playerTile->ground;
		if (playerTileGround && playerTileGround->hasWalkStack()) {
			Player* thisPlayer = const_cast<Player*>(this);
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
	}

	return false;
}

bool Player::canWalkthroughEx(const Creature* creature) const
{
	if (group->access) {
		return true;
	}

	const Player* player = creature->getPlayer();
	if (!player) {
		return false;
	}

	const Tile* playerTile = player->getTile();
	return playerTile && playerTile->hasFlag(TILESTATE_PROTECTIONZONE);
}

void Player::onReceiveMail()
{
	if (isNearDepotBox()) {
		sendTextMessage(MESSAGE_EVENT_ADVANCE, "New mail has arrived.");
	}
}

bool Player::isNearDepotBox() const
{
	const Position& pos = getPosition();
	for (int32_t cx = -1; cx <= 1; ++cx) {
		for (int32_t cy = -1; cy <= 1; ++cy) {
			Tile* tile = g_game.getTile(pos.x + cx, pos.y + cy, pos.z);
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

DepotChest* Player::getDepotChest(uint32_t depotId, bool autoCreate)
{
	auto it = depotChests.find(depotId);
	if (it != depotChests.end()) {
		return it->second;
	}

	if (!autoCreate) {
		return nullptr;
	}

	DepotChest* depotChest = new DepotChest(ITEM_DEPOT);
	depotChest->useThing2();
	depotChest->setMaxDepotItems(maxDepotItems);
	depotChests[depotId] = depotChest;
	return depotChest;
}

DepotLocker* Player::getDepotLocker(uint32_t depotId)
{
	auto it = depotLockerMap.find(depotId);
	if (it != depotLockerMap.end()) {
		inbox->setParent(it->second);
		return it->second;
	}

	DepotLocker* depotLocker = new DepotLocker(ITEM_LOCKER1);
	depotLocker->setDepotId(depotId);
	depotLocker->__internalAddThing(Item::CreateItem(ITEM_MARKET));
	depotLocker->__internalAddThing(inbox);
	depotLocker->__internalAddThing(getDepotChest(depotId, true));
	depotLockerMap[depotId] = depotLocker;
	return depotLocker;
}

void Player::sendCancelMessage(ReturnValue message) const
{
	sendCancel(getReturnMessage(message));
}

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
		setAttackedCreature(nullptr);
	}

	if (noPongTime >= 60000 && canLogout()) {
		if (g_creatureEvents->playerLogout(this)) {
			if (client) {
				client->logout(true, true);
			} else {
				g_game.removeCreature(this, true);
			}
		}
	}
}

Item* Player::getWriteItem(uint32_t& _windowTextId, uint16_t& _maxWriteLen)
{
	_windowTextId = windowTextId;
	_maxWriteLen = maxWriteLen;
	return writeItem;
}

void Player::setWriteItem(Item* item, uint16_t _maxWriteLen /*= 0*/)
{
	windowTextId++;

	if (writeItem) {
		writeItem->releaseThing2();
	}

	if (item) {
		writeItem = item;
		maxWriteLen = _maxWriteLen;
		writeItem->useThing2();
	} else {
		writeItem = nullptr;
		maxWriteLen = 0;
	}
}

House* Player::getEditHouse(uint32_t& _windowTextId, uint32_t& _listId)
{
	_windowTextId = windowTextId;
	_listId = editListId;
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

//container
void Player::sendAddContainerItem(const Container* container, const Item* item)
{
	if (!client) {
		return;
	}

	for (const auto& it : openContainers) {
		const OpenContainer& openContainer = it.second;
		if (openContainer.container != container) {
			continue;
		}

		uint16_t slot = openContainer.index;

		if (container->getID() == ITEM_BROWSEFIELD) {
			uint16_t containerSize = container->size() - 1;
			uint16_t pageEnd = openContainer.index + container->capacity();
			if (containerSize > pageEnd) {
				slot = pageEnd;
				item = container->getItemByIndex(pageEnd);
			} else {
				slot = containerSize;
			}
		} else if (openContainer.index >= container->capacity()) {
			item = container->getItemByIndex(openContainer.index - 1);
		}

		client->sendAddContainerItem(it.first, slot, item);
	}
}

void Player::sendUpdateContainerItem(const Container* container, uint16_t slot, const Item* newItem)
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

void Player::sendRemoveContainerItem(const Container* container, uint16_t slot)
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
			sendContainer(it.first, container, false, firstIndex);
		}

		client->sendRemoveContainerItem(it.first, std::max<uint16_t>(slot, firstIndex), container->getItemByIndex(container->capacity() + firstIndex));
	}
}

void Player::onUpdateTileItem(const Tile* tile, const Position& pos, const Item* oldItem,
                              const ItemType& oldType, const Item* newItem, const ItemType& newType)
{
	Creature::onUpdateTileItem(tile, pos, oldItem, oldType, newItem, newType);

	if (oldItem != newItem) {
		onRemoveTileItem(tile, pos, oldType, oldItem);
	}

	if (tradeState != TRADE_TRANSFER) {
		if (tradeItem && oldItem == tradeItem) {
			g_game.internalCloseTrade(this);
		}
	}
}

void Player::onRemoveTileItem(const Tile* tile, const Position& pos, const ItemType& iType,
                              const Item* item)
{
	Creature::onRemoveTileItem(tile, pos, iType, item);

	if (tradeState != TRADE_TRANSFER) {
		checkTradeState(item);

		if (tradeItem) {
			const Container* container = item->getContainer();
			if (container && container->isHoldingItem(tradeItem)) {
				g_game.internalCloseTrade(this);
			}
		}
	}
}

void Player::onCreatureAppear(const Creature* creature, bool isLogin)
{
	Creature::onCreatureAppear(creature, isLogin);

	if (isLogin && creature == this) {
		for (int32_t slot = CONST_SLOT_FIRST; slot <= CONST_SLOT_LAST; ++slot) {
			Item* item = inventory[slot];
			if (item) {
				item->__startDecaying();
				g_moveEvents->onPlayerEquip(this, item, (slots_t)slot, false);
			}
		}

		for (Condition* condition : storedConditionList) {
			addCondition(condition);
		}
		storedConditionList.clear();

		BedItem* bed = Beds::getInstance().getBedBySleeper(getGUID());
		if (bed) {
			bed->wakeUp(this);
		}

		std::cout << name << " has logged in." << std::endl;

		if (guild) {
			guild->addMember(this);
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
			setAttackedCreature(nullptr);
			onAttackedCreatureDisappear(false);
		}

		if (!group->access && isMounted()) {
			dismount();
			g_game.internalCreatureChangeOutfit(this, defaultOutfit);
		}
	}

	g_game.updateCreatureWalkthrough(this);
	sendIcons();
}

void Player::onAttackedCreatureChangeZone(ZoneType_t zone)
{
	if (zone == ZONE_PROTECTION) {
		if (!hasFlag(PlayerFlag_IgnoreProtectionZone)) {
			setAttackedCreature(nullptr);
			onAttackedCreatureDisappear(false);
		}
	} else if (zone == ZONE_NOPVP) {
		if (attackedCreature->getPlayer()) {
			if (!hasFlag(PlayerFlag_IgnoreProtectionZone)) {
				setAttackedCreature(nullptr);
				onAttackedCreatureDisappear(false);
			}
		}
	} else if (zone == ZONE_NORMAL) {
		//attackedCreature can leave a pvp zone if not pzlocked
		if (g_game.getWorldType() == WORLD_TYPE_NO_PVP) {
			if (attackedCreature->getPlayer()) {
				setAttackedCreature(nullptr);
				onAttackedCreatureDisappear(false);
			}
		}
	}
}

void Player::onCreatureDisappear(const Creature* creature, uint32_t stackpos, bool isLogout)
{
	Creature::onCreatureDisappear(creature, stackpos, isLogout);

	if (creature == this) {
		if (isLogout) {
			loginPosition = getPosition();
		}

		lastLogout = time(nullptr);

		if (eventWalk != 0) {
			setFollowCreature(nullptr);
		}

		if (tradePartner) {
			g_game.internalCloseTrade(this);
		}

		closeShopWindow();

		clearPartyInvitations();

		if (party) {
			party->leaveParty(this);
		}

		g_chat.removeUserFromAllChannels(*this);

		std::cout << getName() << " has logged out." << std::endl;

		if (guild) {
			guild->removeMember(this);
		}

		IOLoginData::updateOnlineStatus(guid, false);

		bool saved = false;
		for (uint32_t tries = 0; tries < 3; ++tries) {
			if (IOLoginData::savePlayer(this)) {
				saved = true;
				break;
			}
		}

		if (!saved) {
			std::cout << "Error while saving player: " << getName() << std::endl;
		}
	}
}

void Player::openShopWindow(Npc* npc, const std::list<ShopInfo>& shop)
{
	shopItemList = shop;
	sendShop(npc);
	sendSaleItemList();
}

bool Player::closeShopWindow(bool sendCloseShopWindow /*= true*/)
{
	//unreference callbacks
	int32_t onBuy;
	int32_t onSell;

	Npc* npc = getShopOwner(onBuy, onSell);
	if (!npc) {
		shopItemList.clear();
		return false;
	}

	setShopOwner(nullptr, -1, -1);
	npc->onPlayerEndTrade(this, onBuy, onSell);

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

void Player::onCreatureMove(const Creature* creature, const Tile* newTile, const Position& newPos,
                            const Tile* oldTile, const Position& oldPos, bool teleport)
{
	Creature::onCreatureMove(creature, newTile, newPos, oldTile, oldPos, teleport);

	if (hasFollowPath && (creature == followCreature || (creature == this && followCreature))) {
		isUpdatingPath = false;
		g_dispatcher.addTask(createTask(std::bind(&Game::updateCreatureWalk, &g_game, getID())));
	}

	if (creature != this) {
		return;
	}

	if (tradeState != TRADE_TRANSFER) {
		//check if we should close trade
		if (tradeItem && !Position::areInRange<1, 1, 0>(tradeItem->getPosition(), getPosition())) {
			g_game.internalCloseTrade(this);
		}

		if (tradePartner && !Position::areInRange<2, 2, 0>(tradePartner->getPosition(), getPosition())) {
			g_game.internalCloseTrade(this);
		}
	}

	// close modal windows
	if (!modalWindows.empty()) {
		// TODO: This shouldn't be hardcoded
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
		int32_t ticks = g_config.getNumber(ConfigManager::STAIRHOP_DELAY);
		if (ticks > 0) {
			if (Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_PACIFIED, ticks, 0)) {
				addCondition(condition);
			}
		}
	}
}

//container
void Player::onAddContainerItem(const Item* item)
{
	checkTradeState(item);
}

void Player::onUpdateContainerItem(const Container* container, const Item* oldItem, const Item* newItem)
{
	if (oldItem != newItem) {
		onRemoveContainerItem(container, oldItem);
	}

	if (tradeState != TRADE_TRANSFER) {
		checkTradeState(oldItem);
	}
}

void Player::onRemoveContainerItem(const Container* container, const Item* item)
{
	if (tradeState != TRADE_TRANSFER) {
		checkTradeState(item);

		if (tradeItem) {
			if (tradeItem->getParent() != container && container->isHoldingItem(tradeItem)) {
				g_game.internalCloseTrade(this);
			}
		}
	}
}

void Player::onCloseContainer(const Container* container)
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

void Player::onSendContainer(const Container* container)
{
	if (!client) {
		return;
	}

	bool hasParent = container->hasParent();
	for (const auto& it : openContainers) {
		const OpenContainer& openContainer = it.second;
		if (openContainer.container == container) {
			client->sendContainer(it.first, container, hasParent, openContainer.index);
		}
	}
}

//inventory
void Player::onUpdateInventoryItem(Item* oldItem, Item* newItem)
{
	if (oldItem != newItem) {
		onRemoveInventoryItem(oldItem);
	}

	if (tradeState != TRADE_TRANSFER) {
		checkTradeState(oldItem);
	}
}

void Player::onRemoveInventoryItem(Item* item)
{
	if (tradeState != TRADE_TRANSFER) {
		checkTradeState(item);

		if (tradeItem) {
			const Container* container = item->getContainer();
			if (container && container->isHoldingItem(tradeItem)) {
				g_game.internalCloseTrade(this);
			}
		}
	}
}

void Player::checkTradeState(const Item* item)
{
	if (tradeItem && tradeState != TRADE_TRANSFER) {
		if (tradeItem == item) {
			g_game.internalCloseTrade(this);
		} else {
			const Container* container = dynamic_cast<const Container*>(item->getParent());
			while (container) {
				if (container == tradeItem) {
					g_game.internalCloseTrade(this);
					break;
				}

				container = dynamic_cast<const Container*>(container->getParent());
			}
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

void Player::setNextWalkTask(SchedulerTask* task)
{
	if (nextStepEvent != 0) {
		g_scheduler.stopEvent(nextStepEvent);
		nextStepEvent = 0;
	}

	if (task) {
		nextStepEvent = g_scheduler.addEvent(task);
		resetIdleTime();
	}
}

void Player::setNextActionTask(SchedulerTask* task)
{
	if (actionTaskEvent != 0) {
		g_scheduler.stopEvent(actionTaskEvent);
		actionTaskEvent = 0;
	}

	if (task) {
		actionTaskEvent = g_scheduler.addEvent(task);
		resetIdleTime();
	}
}

uint32_t Player::getNextActionTime() const
{
	return std::max<int64_t>(SCHEDULER_MINTICKS, nextAction - OTSYS_TIME());
}

void Player::onThink(uint32_t interval)
{
	Creature::onThink(interval);

	sendPing();

	MessageBufferTicks += interval;
	if (MessageBufferTicks >= 1500) {
		MessageBufferTicks = 0;
		addMessageBuffer();
	}

	if (!getTile()->hasFlag(TILESTATE_NOLOGOUT) && !mayNotMove && !isAccessPlayer()) {
		idleTime += interval;
		const int32_t kickAfterMinutes = g_config.getNumber(ConfigManager::KICK_AFTER_MINUTES);
		if (idleTime > (kickAfterMinutes * 60000) + 60000) {
			kickPlayer(true);
		} else if (client && idleTime == 60000 * kickAfterMinutes) {
			std::ostringstream ss;
			ss << "You have been idle for " << kickAfterMinutes << " minutes. You will be disconnected in one minute if you are still idle then.";
			client->sendTextMessage(MESSAGE_STATUS_WARNING, ss.str());
		}
	}

	if (g_game.getWorldType() != WORLD_TYPE_PVP_ENFORCED) {
		checkSkullTicks(interval);
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
	if (MessageBufferCount > 0 && g_config.getNumber(ConfigManager::MAX_MESSAGEBUFFER) != 0 && !hasFlag(PlayerFlag_CannotBeMuted)) {
		--MessageBufferCount;
	}
}

void Player::removeMessageBuffer()
{
	if (hasFlag(PlayerFlag_CannotBeMuted)) {
		return;
	}

	const int32_t maxMessageBuffer = g_config.getNumber(ConfigManager::MAX_MESSAGEBUFFER);
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

			std::ostringstream ss;
			ss << "You are muted for " << muteTime << " seconds.";
			sendTextMessage(MESSAGE_STATUS_SMALL, ss.str());
		}
	}
}

void Player::drainHealth(Creature* attacker, int32_t damage)
{
	Creature::drainHealth(attacker, damage);
	sendStats();
}

void Player::drainMana(Creature* attacker, int32_t manaLoss)
{
	Creature::drainMana(attacker, manaLoss);
	sendStats();
}

void Player::addManaSpent(uint64_t amount)
{
	if (amount == 0 || hasFlag(PlayerFlag_NotGainMana)) {
		return;
	}

	uint64_t currReqMana = vocation->getReqMana(magLevel);
	uint64_t nextReqMana = vocation->getReqMana(magLevel + 1);
	if (currReqMana >= nextReqMana) {
		//player has reached max magic level
		return;
	}

	bool sendUpdateStats = false;
	while ((manaSpent + amount) >= nextReqMana) {
		amount -= nextReqMana - manaSpent;

		magLevel++;
		manaSpent = 0;

		std::ostringstream ss;
		ss << "You advanced to magic level " << magLevel << '.';
		sendTextMessage(MESSAGE_EVENT_ADVANCE, ss.str());

		g_creatureEvents->playerAdvance(this, SKILL_MAGLEVEL, magLevel - 1, magLevel);

		sendUpdateStats = true;
		currReqMana = nextReqMana;
		nextReqMana = vocation->getReqMana(magLevel + 1);
		if (currReqMana >= nextReqMana) {
			return;
		}
	}

	manaSpent += amount;

	uint32_t newPercent;

	if (nextReqMana > currReqMana) {
		newPercent = Player::getPercentLevel(manaSpent, nextReqMana);
	} else {
		newPercent = 0;
	}

	if (newPercent != magLevelPercent) {
		magLevelPercent = newPercent;
		sendUpdateStats = true;
	}

	if (sendUpdateStats) {
		sendStats();
	}
}

void Player::addExperience(uint64_t exp, bool sendText/* = false*/, bool applyStaminaChange/* = false*/)
{
	uint64_t currLevelExp = Player::getExpForLevel(level);
	uint64_t nextLevelExp = Player::getExpForLevel(level + 1);
	if (currLevelExp >= nextLevelExp) {
		//player has reached max level
		levelPercent = 0;
		sendStats();
		return;
	}

	if (applyStaminaChange && g_config.getBoolean(ConfigManager::STAMINA_SYSTEM)) {
		if (staminaMinutes > 2400) {
			if (isPremium()) {
				exp *= 1.5;
			}
		} else if (staminaMinutes <= 840) {
			exp *= 0.5;
		}
	}

	experience += exp;

	if (sendText) {
		const Position& targetPos = getPosition();

		std::ostringstream ss;
		ss << "You gained " << exp << " experience points.";
		sendExperienceMessage(MESSAGE_EXPERIENCE, ss.str(), targetPos, exp, TEXTCOLOR_WHITE_EXP);

		std::ostringstream ssExp;
		ssExp << getNameDescription() << " gained " << exp << " experience points.";
		std::string strExp = ssExp.str();

		SpectatorVec list;
		g_game.getSpectators(list, targetPos, false, true);
		for (Creature* spectator : list) {
			Player* tmpPlayer = spectator->getPlayer();
			if (tmpPlayer != this) {
				tmpPlayer->sendExperienceMessage(MESSAGE_EXPERIENCE_OTHERS, strExp, targetPos, exp, TEXTCOLOR_WHITE_EXP);
			}
		}
	}

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
			//player has reached max level
			break;
		}
	}

	if (prevLevel != level) {
		health = healthMax;
		mana = manaMax;

		updateBaseSpeed();

		int32_t newSpeed = getBaseSpeed();
		setBaseSpeed(newSpeed);

		g_game.changeSpeed(this, 0);
		g_game.addCreatureHealth(this);

		if (party) {
			party->updateSharedExperience();
		}

		g_creatureEvents->playerAdvance(this, SKILL_LEVEL, prevLevel, level);

		std::ostringstream ss;
		ss << "You advanced from Level " << prevLevel << " to Level " << level << '.';
		sendTextMessage(MESSAGE_EVENT_ADVANCE, ss.str());
	}

	if (nextLevelExp > currLevelExp) {
		uint32_t newPercent = Player::getPercentLevel(experience - currLevelExp, nextLevelExp - currLevelExp);
		levelPercent = newPercent;
	} else {
		levelPercent = 0;
	}
	sendStats();
}

uint32_t Player::getPercentLevel(uint64_t count, uint64_t nextLevelCount)
{
	if (nextLevelCount == 0) {
		return 0;
	}

	uint32_t result = (count * 100) / nextLevelCount;
	if (result > 100) {
		return 0;
	}
	return result;
}

void Player::onBlockHit()
{
	if (shieldBlockCount > 0) {
		--shieldBlockCount;

		if (hasShield()) {
			addSkillAdvance(SKILL_SHIELD, g_config.getNumber(ConfigManager::RATE_SKILL));
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
			//need to draw blood every 30 hits
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
	Item* item = inventory[CONST_SLOT_LEFT];
	if (item && item->getWeaponType() == WEAPON_SHIELD) {
		return true;
	}

	item = inventory[CONST_SLOT_RIGHT];
	if (item && item->getWeaponType() == WEAPON_SHIELD) {
		return true;
	}
	return false;
}

BlockType_t Player::blockHit(Creature* attacker, CombatType_t combatType, int32_t& damage,
                             bool checkDefense /* = false*/, bool checkArmor /* = false*/, bool field /* = false*/)
{
	BlockType_t blockType = Creature::blockHit(attacker, combatType, damage, checkDefense, checkArmor, field);

	if (attacker) {
		sendCreatureSquare(attacker, SQ_COLOR_BLACK);
	}

	if (blockType != BLOCK_NONE) {
		return blockType;
	}

	if (damage > 0) {
		for (int32_t slot = CONST_SLOT_FIRST; slot <= CONST_SLOT_LAST; ++slot) {
			if (!isItemAbilityEnabled((slots_t)slot)) {
				continue;
			}

			Item* item = inventory[slot];
			if (!item) {
				continue;
			}

			const ItemType& it = Item::items[item->getID()];
			if (it.abilities) {
				const int16_t& absorbPercent = it.abilities->absorbPercent[combatTypeToIndex(combatType)];
				if (absorbPercent != 0) {
					damage -= std::ceil(damage * (absorbPercent / 100.));

					uint16_t charges = item->getCharges();
					if (charges != 0) {
						g_game.transformItem(item, item->getID(), charges - 1);
					}
				}

				if (field) {
					const int16_t& fieldAbsorbPercent = it.abilities->fieldAbsorbPercent[combatTypeToIndex(combatType)];
					if (fieldAbsorbPercent != 0) {
						damage -= std::ceil(damage * (fieldAbsorbPercent / 100.));

						uint16_t charges = item->getCharges();
						if (charges != 0) {
							g_game.transformItem(item, item->getID(), charges - 1);
						}
					}
				}
			}
		}

		if (damage <= 0) {
			damage = 0;
			blockType = BLOCK_DEFENSE;
		}
	}
	return blockType;
}

uint32_t Player::getIP() const
{
	if (client) {
		return client->getIP();
	}

	return 0;
}

void Player::death(Creature* _lastHitCreature)
{
	loginPosition = town->getTemplePosition();

	if (skillLoss) {
		uint8_t unfairFightReduction = 100;

		if (_lastHitCreature) {
			Player* lastHitPlayer = _lastHitCreature->getPlayer();
			if (!lastHitPlayer) {
				Creature* lastHitMaster = _lastHitCreature->getMaster();
				if (lastHitMaster) {
					lastHitPlayer = lastHitMaster->getPlayer();
				}
			}

			if (lastHitPlayer) {
				uint32_t sumLevels = 0;
				uint32_t inFightTicks = g_config.getNumber(ConfigManager::PZ_LOCKED);
				for (const auto& it : damageMap) {
					CountBlock_t cb = it.second;
					if ((OTSYS_TIME() - cb.ticks) <= inFightTicks) {
						Player* damageDealer = g_game.getPlayerByID(it.first);
						if (damageDealer) {
							sumLevels += damageDealer->getLevel();
						}
					}
				}

				if (sumLevels > level) {
					double reduce = level / (double)sumLevels;
					unfairFightReduction = std::max<uint8_t>(20, std::floor((reduce * 100) + 0.5));
				}
			}
		}

		//Magic level loss
		uint64_t sumMana = 0;
		uint64_t lostMana = 0;

		//sum up all the mana
		for (uint32_t i = 1; i <= magLevel; ++i) {
			sumMana += vocation->getReqMana(i);
		}

		sumMana += manaSpent;

		double deathLossPercent = getLostPercent() * (unfairFightReduction / 100.);

		lostMana = (uint64_t)(sumMana * deathLossPercent);

		while (lostMana > manaSpent && magLevel > 0) {
			lostMana -= manaSpent;
			manaSpent = vocation->getReqMana(magLevel);
			magLevel--;
		}

		manaSpent -= lostMana;
		uint64_t nextReqMana = vocation->getReqMana(magLevel + 1);

		if (nextReqMana > vocation->getReqMana(magLevel)) {
			magLevelPercent = Player::getPercentLevel(manaSpent, nextReqMana);
		} else {
			magLevelPercent = 0;
		}

		//Skill loss
		for (int16_t i = SKILL_FIRST; i <= SKILL_LAST; ++i) { //for each skill
			uint32_t sumSkillTries = 0;
			for (uint32_t c = 11; c <= skills[i][SKILLVALUE_LEVEL]; ++c) { //sum up all required tries for all skill levels
				sumSkillTries += vocation->getReqSkillTries(i, c);
			}

			sumSkillTries += skills[i][SKILLVALUE_TRIES];

			uint32_t lostSkillTries = (uint32_t)(sumSkillTries * deathLossPercent);
			while (lostSkillTries > skills[i][SKILLVALUE_TRIES]) {
				lostSkillTries -= skills[i][SKILLVALUE_TRIES];

				if (skills[i][SKILLVALUE_LEVEL] <= 10) {
					skills[i][SKILLVALUE_LEVEL] = 10;
					skills[i][SKILLVALUE_TRIES] = 0;
					lostSkillTries = 0;
					break;
				}

				skills[i][SKILLVALUE_TRIES] = vocation->getReqSkillTries(i, skills[i][SKILLVALUE_LEVEL]);
				skills[i][SKILLVALUE_LEVEL]--;
			}

			skills[i][SKILLVALUE_TRIES] = std::max<int32_t>(0, skills[i][SKILLVALUE_TRIES] - lostSkillTries);
			skills[i][SKILLVALUE_PERCENT] = Player::getPercentLevel(skills[i][SKILLVALUE_TRIES], vocation->getReqSkillTries(i, skills[i][SKILLVALUE_LEVEL]));
		}

		//

		//Level loss
		uint32_t oldLevel = level;

		if (vocation->getId() == VOCATION_NONE || level > 7) {
			experience -= (uint64_t)(experience * deathLossPercent);
		}

		while (level > 1 && experience < Player::getExpForLevel(level)) {
			--level;
			healthMax = std::max<int32_t>(0, healthMax - vocation->getHPGain());
			manaMax = std::max<int32_t>(0, manaMax - vocation->getManaGain());
			capacity = std::max<double>(0.00, capacity - vocation->getCapGain());
		}

		if (oldLevel != level) {
			std::ostringstream ss;
			ss << "You were downgraded from Level " << oldLevel << " to Level " << level << '.';
			sendTextMessage(MESSAGE_EVENT_ADVANCE, ss.str());
		}

		uint64_t currLevelExp = Player::getExpForLevel(level);
		uint64_t nextLevelExp = Player::getExpForLevel(level + 1);
		if (nextLevelExp > currLevelExp) {
			levelPercent = Player::getPercentLevel(experience - currLevelExp, nextLevelExp - currLevelExp);
		} else {
			levelPercent = 0;
		}

		std::bitset<6> bitset(blessings);
		if (bitset[5]) {
			Player* lastHitPlayer;

			if (_lastHitCreature) {
				lastHitPlayer = _lastHitCreature->getPlayer();
				if (!lastHitPlayer) {
					Creature* lastHitMaster = _lastHitCreature->getMaster();
					if (lastHitMaster) {
						lastHitPlayer = lastHitMaster->getPlayer();
					}
				}
			} else {
				lastHitPlayer = nullptr;
			}

			if (lastHitPlayer) {
				bitset.reset(5);
				blessings = bitset.to_ulong();
			} else {
				blessings = 32;
			}
		} else {
			blessings = 0;
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

		for (ConditionList::iterator it = conditions.begin(); it != conditions.end();) {
			Condition* condition = *it;
			if (condition->isPersistent()) {
				it = conditions.erase(it);

				condition->endCondition(this);
				onEndCondition(condition->getType());
				delete condition;
			} else {
				++it;
			}
		}
	} else {
		setLossSkill(true);

		for (ConditionList::iterator it = conditions.begin(); it != conditions.end();) {
			Condition* condition = *it;
			if (condition->isPersistent()) {
				it = conditions.erase(it);

				condition->endCondition(this);
				onEndCondition(condition->getType());
				delete condition;
			} else {
				++it;
			}
		}

		health = healthMax;
		g_game.internalTeleport(this, getTemplePosition(), true);
		g_game.addCreatureHealth(this);
		onThink(EVENT_CREATURE_THINK_INTERVAL);
		onIdleStatus();
		sendStats();
	}
}

bool Player::dropCorpse(Creature* _lastHitCreature, Creature* mostDamageCreature, bool lastHitUnjustified, bool mostDamageUnjustified)
{
	if (getZone() == ZONE_PVP) {
		setDropLoot(true);
		return false;
	}
	return Creature::dropCorpse(_lastHitCreature, mostDamageCreature, lastHitUnjustified, mostDamageUnjustified);
}

Item* Player::getCorpse(Creature* _lastHitCreature, Creature* mostDamageCreature)
{
	Item* corpse = Creature::getCorpse(_lastHitCreature, mostDamageCreature);
	if (corpse && corpse->getContainer()) {
		std::ostringstream ss;
		if (_lastHitCreature) {
			ss << "You recognize " << getNameDescription() << ". " << (getSex() == PLAYERSEX_FEMALE ? "She" : "He") << " was killed by " << _lastHitCreature->getNameDescription() << '.';
		} else {
			ss << "You recognize " << getNameDescription() << '.';
		}

		corpse->setSpecialDescription(ss.str());
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

	Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_INFIGHT, g_config.getNumber(ConfigManager::PZ_LOCKED), 0);
	addCondition(condition);
}

void Player::removeList()
{
	g_game.removePlayer(this);

	for (const auto& it : g_game.getPlayers()) {
		it.second->notifyStatusChange(this, VIPSTATUS_OFFLINE);
	}
}

void Player::addList()
{
	for (const auto& it : g_game.getPlayers()) {
		it.second->notifyStatusChange(this, VIPSTATUS_ONLINE);
	}

	g_game.addPlayer(this);
}

void Player::kickPlayer(bool displayEffect)
{
	g_creatureEvents->playerLogout(this);
	if (client) {
		client->logout(displayEffect, true);
	} else {
		g_game.removeCreature(this);
	}
}

void Player::notifyStatusChange(Player* loginPlayer, VipStatus_t status)
{
	if (!client) {
		return;
	}

	auto it = VIPList.find(loginPlayer->getGUID());
	if (it == VIPList.end()) {
		return;
	}

	client->sendUpdatedVIPStatus(loginPlayer->getGUID(), status);

	if (status == VIPSTATUS_ONLINE) {
		client->sendTextMessage(MESSAGE_STATUS_SMALL, (loginPlayer->getName() + " has logged in."));
	} else if (status == VIPSTATUS_OFFLINE) {
		client->sendTextMessage(MESSAGE_STATUS_SMALL, (loginPlayer->getName() + " has logged out."));
	}
}

bool Player::removeVIP(uint32_t _guid)
{
	if (VIPList.erase(_guid) == 0) {
		return false;
	}

	IOLoginData::removeVIPEntry(accountNumber, _guid);
	return true;
}

bool Player::addVIP(uint32_t _guid, const std::string& name, VipStatus_t status)
{
	if (guid == _guid) {
		sendTextMessage(MESSAGE_STATUS_SMALL, "You cannot add yourself.");
		return false;
	}

	if (VIPList.size() >= maxVipEntries || VIPList.size() == 200) { // max number of buddies is 200 in 9.53
		sendTextMessage(MESSAGE_STATUS_SMALL, "You cannot add more buddies.");
		return false;
	}

	auto it = VIPList.find(_guid);
	if (it != VIPList.end()) {
		sendTextMessage(MESSAGE_STATUS_SMALL, "This player is already in your list.");
		return false;
	}

	VIPList.insert(_guid);

	IOLoginData::addVIPEntry(accountNumber, _guid, "", 0, false);

	if (client) {
		client->sendVIP(_guid, name, "", 0, false, status);
	}

	return true;
}

bool Player::addVIPInternal(uint32_t _guid)
{
	if (guid == _guid) {
		return false;
	}

	if (VIPList.size() >= maxVipEntries || VIPList.size() == 200) { // max number of buddies is 200 in 9.53
		return false;
	}

	auto it = VIPList.find(_guid);
	if (it != VIPList.end()) {
		return false;
	}

	VIPList.insert(_guid);
	return true;
}

bool Player::editVIP(uint32_t _guid, const std::string& description, uint32_t icon, bool notify)
{
	auto it = VIPList.find(_guid);
	if (it == VIPList.end()) {
		return false;    // player is not in VIP
	}

	IOLoginData::editVIPEntry(accountNumber, _guid, description, icon, notify);
	return true;
}

//close container and its child containers
void Player::autoCloseContainers(const Container* container)
{
	std::vector<uint32_t> closeList;
	for (const auto& it : openContainers) {
		Container* tmpContainer = it.second.container;
		while (tmpContainer) {
			if (tmpContainer->isRemoved() || tmpContainer == container) {
				closeList.push_back(it.first);
				break;
			}

			tmpContainer = dynamic_cast<Container*>(tmpContainer->getParent());
		}
	}

	for (uint32_t containerId : closeList) {
		closeContainer(containerId);
		if (client) {
			client->sendCloseContainer(containerId);
		}
	}
}

bool Player::hasCapacity(const Item* item, uint32_t count) const
{
	if (hasFlag(PlayerFlag_CannotPickupItem)) {
		return false;
	}

	if (hasFlag(PlayerFlag_HasInfiniteCapacity) || item->getTopParent() == this) {
		return true;
	}

	double itemWeight;
	if (item->isStackable()) {
		itemWeight = Item::items[item->getID()].weight * count;
	} else {
		itemWeight = item->getWeight();
	}
	return itemWeight <= getFreeCapacity();
}

ReturnValue Player::__queryAdd(int32_t index, const Thing* thing, uint32_t count, uint32_t flags, Creature*) const
{
	const Item* item = thing->getItem();
	if (item == nullptr) {
		return RET_NOTPOSSIBLE;
	}

	bool childIsOwner = hasBitSet(FLAG_CHILDISOWNER, flags);
	if (childIsOwner) {
		//a child container is querying the player, just check if enough capacity
		bool skipLimit = hasBitSet(FLAG_NOLIMIT, flags);
		if (skipLimit || hasCapacity(item, count)) {
			return RET_NOERROR;
		}
		return RET_NOTENOUGHCAPACITY;
	}

	if (!item->isPickupable()) {
		return RET_CANNOTPICKUP;
	}

	ReturnValue ret = RET_NOERROR;

	const int32_t& slotPosition = item->getSlotPosition();
	if ((slotPosition & SLOTP_HEAD) || (slotPosition & SLOTP_NECKLACE) ||
	        (slotPosition & SLOTP_BACKPACK) || (slotPosition & SLOTP_ARMOR) ||
	        (slotPosition & SLOTP_LEGS) || (slotPosition & SLOTP_FEET) ||
	        (slotPosition & SLOTP_RING)) {
		ret = RET_CANNOTBEDRESSED;
	} else if (slotPosition & SLOTP_TWO_HAND) {
		ret = RET_PUTTHISOBJECTINBOTHHANDS;
	} else if ((slotPosition & SLOTP_RIGHT) || (slotPosition & SLOTP_LEFT)) {
		ret = RET_PUTTHISOBJECTINYOURHAND;
	}

	switch (index) {
		case CONST_SLOT_HEAD: {
			if (slotPosition & SLOTP_HEAD) {
				ret = RET_NOERROR;
			}
			break;
		}

		case CONST_SLOT_NECKLACE: {
			if (slotPosition & SLOTP_NECKLACE) {
				ret = RET_NOERROR;
			}
			break;
		}

		case CONST_SLOT_BACKPACK: {
			if (slotPosition & SLOTP_BACKPACK) {
				ret = RET_NOERROR;
			}
			break;
		}

		case CONST_SLOT_ARMOR: {
			if (slotPosition & SLOTP_ARMOR) {
				ret = RET_NOERROR;
			}
			break;
		}

		case CONST_SLOT_RIGHT: {
			if (slotPosition & SLOTP_RIGHT) {
				//check if we already carry an item in the other hand
				if (slotPosition & SLOTP_TWO_HAND) {
					if (inventory[CONST_SLOT_LEFT] && inventory[CONST_SLOT_LEFT] != item) {
						ret = RET_BOTHHANDSNEEDTOBEFREE;
					} else {
						ret = RET_NOERROR;
					}
				} else if (inventory[CONST_SLOT_LEFT]) {
					const Item* leftItem = inventory[CONST_SLOT_LEFT];
					WeaponType_t type = item->getWeaponType(), leftType = leftItem->getWeaponType();

					if (leftItem->getSlotPosition() & SLOTP_TWO_HAND) {
						ret = RET_DROPTWOHANDEDITEM;
					} else if (item == leftItem && count == item->getItemCount()) {
						ret = RET_NOERROR;
					} else if (leftType == WEAPON_SHIELD && type == WEAPON_SHIELD) {
						ret = RET_CANONLYUSEONESHIELD;
					} else if (!leftItem->isWeapon() || !item->isWeapon() ||
					           leftType == WEAPON_SHIELD || leftType == WEAPON_AMMO
					           || type == WEAPON_SHIELD || type == WEAPON_AMMO) {
						ret = RET_NOERROR;
					} else {
						ret = RET_CANONLYUSEONEWEAPON;
					}
				} else {
					ret = RET_NOERROR;
				}
			}
			break;
		}

		case CONST_SLOT_LEFT: {
			if (slotPosition & SLOTP_LEFT) {
				//check if we already carry an item in the other hand
				if (slotPosition & SLOTP_TWO_HAND) {
					if (inventory[CONST_SLOT_RIGHT] && inventory[CONST_SLOT_RIGHT] != item) {
						ret = RET_BOTHHANDSNEEDTOBEFREE;
					} else {
						ret = RET_NOERROR;
					}
				} else if (inventory[CONST_SLOT_RIGHT]) {
					const Item* rightItem = inventory[CONST_SLOT_RIGHT];
					WeaponType_t type = item->getWeaponType(), rightType = rightItem->getWeaponType();

					if (rightItem->getSlotPosition() & SLOTP_TWO_HAND) {
						ret = RET_DROPTWOHANDEDITEM;
					} else if (item == rightItem && count == item->getItemCount()) {
						ret = RET_NOERROR;
					} else if (rightType == WEAPON_SHIELD && type == WEAPON_SHIELD) {
						ret = RET_CANONLYUSEONESHIELD;
					} else if (!rightItem->isWeapon() || !item->isWeapon() ||
					           rightType == WEAPON_SHIELD || rightType == WEAPON_AMMO
					           || type == WEAPON_SHIELD || type == WEAPON_AMMO) {
						ret = RET_NOERROR;
					} else {
						ret = RET_CANONLYUSEONEWEAPON;
					}
				} else {
					ret = RET_NOERROR;
				}
			}
			break;
		}

		case CONST_SLOT_LEGS: {
			if (slotPosition & SLOTP_LEGS) {
				ret = RET_NOERROR;
			}
			break;
		}

		case CONST_SLOT_FEET: {
			if (slotPosition & SLOTP_FEET) {
				ret = RET_NOERROR;
			}
			break;
		}

		case CONST_SLOT_RING: {
			if (slotPosition & SLOTP_RING) {
				ret = RET_NOERROR;
			}
			break;
		}

		case CONST_SLOT_AMMO: {
			if (slotPosition & SLOTP_AMMO) {
				ret = RET_NOERROR;
			}
			break;
		}

		case CONST_SLOT_WHEREEVER:
		case -1:
			ret = RET_NOTENOUGHROOM;
			break;

		default:
			ret = RET_NOTPOSSIBLE;
			break;
	}

	if (ret == RET_NOERROR || ret == RET_NOTENOUGHROOM) {
		//need an exchange with source?
		const Item* inventoryItem = getInventoryItem((slots_t)index);
		if (inventoryItem && (!inventoryItem->isStackable() || inventoryItem->getID() != item->getID())) {
			return RET_NEEDEXCHANGE;
		}

		//check if enough capacity
		if (!hasCapacity(item, count)) {
			return RET_NOTENOUGHCAPACITY;
		}

		if (!g_moveEvents->onPlayerEquip(const_cast<Player*>(this), const_cast<Item*>(item), (slots_t)index, true)) {
			return RET_CANNOTBEDRESSED;
		}
	}
	return ret;
}

ReturnValue Player::__queryMaxCount(int32_t index, const Thing* thing, uint32_t count, uint32_t& maxQueryCount,
                                    uint32_t flags) const
{
	const Item* item = thing->getItem();
	if (item == nullptr) {
		maxQueryCount = 0;
		return RET_NOTPOSSIBLE;
	}

	if (index == INDEX_WHEREEVER) {
		uint32_t n = 0;
		for (int32_t slotIndex = CONST_SLOT_FIRST; slotIndex <= CONST_SLOT_LAST; ++slotIndex) {
			Item* inventoryItem = inventory[slotIndex];
			if (inventoryItem) {
				if (Container* subContainer = inventoryItem->getContainer()) {
					uint32_t queryCount = 0;
					subContainer->__queryMaxCount(INDEX_WHEREEVER, item, item->getItemCount(), queryCount, flags);
					n += queryCount;

					//iterate through all items, including sub-containers (deep search)
					for (ContainerIterator cit = subContainer->begin(); cit != subContainer->end(); ++cit) {
						if (Container* tmpContainer = (*cit)->getContainer()) {
							queryCount = 0;
							tmpContainer->__queryMaxCount(INDEX_WHEREEVER, item, item->getItemCount(), queryCount, flags);
							n += queryCount;
						}
					}
				} else if (inventoryItem->isStackable() && item->getID() == inventoryItem->getID() && inventoryItem->getItemCount() < 100) {
					uint32_t remainder = (100 - inventoryItem->getItemCount());

					if (__queryAdd(slotIndex, item, remainder, flags) == RET_NOERROR) {
						n += remainder;
					}
				}
			} else if (__queryAdd(slotIndex, item, item->getItemCount(), flags) == RET_NOERROR) { //empty slot
				if (item->isStackable()) {
					n += 100;
				} else {
					++n;
				}
			}
		}

		maxQueryCount = n;
	} else {
		const Item* destItem = nullptr;

		const Thing* destThing = __getThing(index);
		if (destThing) {
			destItem = destThing->getItem();
		}

		if (destItem) {
			if (destItem->isStackable() && item->getID() == destItem->getID() && destItem->getItemCount() < 100) {
				maxQueryCount = 100 - destItem->getItemCount();
			} else {
				maxQueryCount = 0;
			}
		} else if (__queryAdd(index, item, count, flags) == RET_NOERROR) { //empty slot
			if (item->isStackable()) {
				maxQueryCount = 100;
			} else {
				maxQueryCount = 1;
			}

			return RET_NOERROR;
		}
	}

	if (maxQueryCount < count) {
		return RET_NOTENOUGHROOM;
	} else {
		return RET_NOERROR;
	}
}

ReturnValue Player::__queryRemove(const Thing* thing, uint32_t count, uint32_t flags) const
{
	int32_t index = __getIndexOfThing(thing);
	if (index == -1) {
		return RET_NOTPOSSIBLE;
	}

	const Item* item = thing->getItem();
	if (item == nullptr) {
		return RET_NOTPOSSIBLE;
	}

	if (count == 0 || (item->isStackable() && count > item->getItemCount())) {
		return RET_NOTPOSSIBLE;
	}

	if (!item->isMoveable() && !hasBitSet(FLAG_IGNORENOTMOVEABLE, flags)) {
		return RET_NOTMOVEABLE;
	}

	return RET_NOERROR;
}

Cylinder* Player::__queryDestination(int32_t& index, const Thing* thing, Item** destItem,
                                     uint32_t& flags)
{
	if (index == 0 /*drop to capacity window*/ || index == INDEX_WHEREEVER) {
		*destItem = nullptr;

		const Item* item = thing->getItem();
		if (item == nullptr) {
			return this;
		}

		bool autoStack = !((flags & FLAG_IGNOREAUTOSTACK) == FLAG_IGNOREAUTOSTACK);
		bool isStackable = item->isStackable();

		std::vector<Container*> containers;

		for (uint32_t slotIndex = CONST_SLOT_FIRST; slotIndex <= CONST_SLOT_LAST; ++slotIndex) {
			Item* inventoryItem = inventory[slotIndex];
			if (inventoryItem) {
				if (inventoryItem == tradeItem) {
					continue;
				}

				if (inventoryItem == item) {
					continue;
				}

				if (autoStack && isStackable) {
					//try find an already existing item to stack with
					if (__queryAdd(slotIndex, item, item->getItemCount(), 0) == RET_NOERROR) {
						if (inventoryItem->getID() == item->getID() && inventoryItem->getItemCount() < 100) {
							index = slotIndex;
							*destItem = inventoryItem;
							return this;
						}
					}

					if (Container* subContainer = inventoryItem->getContainer()) {
						containers.push_back(subContainer);
					}
				} else if (Container* subContainer = inventoryItem->getContainer()) {
					if (subContainer->__queryAdd(INDEX_WHEREEVER, item, item->getItemCount(), flags) == RET_NOERROR) {
						index = INDEX_WHEREEVER;
						*destItem = nullptr;
						return subContainer;
					}

					containers.push_back(subContainer);
				}
			} else if (__queryAdd(slotIndex, item, item->getItemCount(), flags) == RET_NOERROR) { //empty slot
				index = slotIndex;
				*destItem = nullptr;
				return this;
			}
		}

		size_t i = 0;
		while (i < containers.size()) {
			Container* tmpContainer = containers[i++];
			if (!autoStack || !isStackable) {
				//we need to find first empty container as fast as we can for non-stackable items
				uint32_t n = tmpContainer->capacity() - tmpContainer->size();
				while (n) {
					if (tmpContainer->__queryAdd(tmpContainer->capacity() - n, item, item->getItemCount(), flags) == RET_NOERROR) {
						index = tmpContainer->capacity() - n;
						*destItem = nullptr;
						return tmpContainer;
					}

					n--;
				}

				for (Item* tmpContainerItem : tmpContainer->getItemList()) {
					if (Container* subContainer = tmpContainerItem->getContainer()) {
						containers.push_back(subContainer);
					}
				}

				continue;
			}

			uint32_t n = 0;

			for (Item* tmpItem : tmpContainer->getItemList()) {
				if (tmpItem == tradeItem) {
					continue;
				}

				if (tmpItem == item) {
					continue;
				}

				//try find an already existing item to stack with
				if (tmpItem->getID() == item->getID() && tmpItem->getItemCount() < 100) {
					index = n;
					*destItem = tmpItem;
					return tmpContainer;
				}

				if (Container* subContainer = tmpItem->getContainer()) {
					containers.push_back(subContainer);
				}

				n++;
			}

			if (n < tmpContainer->capacity() && tmpContainer->__queryAdd(n, item, item->getItemCount(), flags) == RET_NOERROR) {
				index = n;
				*destItem = nullptr;
				return tmpContainer;
			}
		}

		return this;
	}

	Thing* destThing = __getThing(index);
	if (destThing) {
		*destItem = destThing->getItem();
	}

	Cylinder* subCylinder = dynamic_cast<Cylinder*>(destThing);
	if (subCylinder) {
		index = INDEX_WHEREEVER;
		*destItem = nullptr;
		return subCylinder;
	} else {
		return this;
	}
}

void Player::__addThing(int32_t index, Thing* thing)
{
	if (index < CONST_SLOT_FIRST || index > CONST_SLOT_LAST) {
		return /*RET_NOTPOSSIBLE*/;
	}

	Item* item = thing->getItem();
	if (!item) {
		return /*RET_NOTPOSSIBLE*/;
	}

	item->setParent(this);
	inventory[index] = item;

	//send to client
	sendInventoryItem((slots_t)index, item);
}

void Player::__updateThing(Thing* thing, uint16_t itemId, uint32_t count)
{
	int32_t index = __getIndexOfThing(thing);
	if (index == -1) {
		return /*RET_NOTPOSSIBLE*/;
	}

	Item* item = thing->getItem();
	if (!item) {
		return /*RET_NOTPOSSIBLE*/;
	}

	item->setID(itemId);
	item->setSubType(count);

	//send to client
	sendInventoryItem((slots_t)index, item);

	//event methods
	onUpdateInventoryItem(item, item);
}

void Player::__replaceThing(uint32_t index, Thing* thing)
{
	if (index > CONST_SLOT_LAST) {
		return /*RET_NOTPOSSIBLE*/;
	}

	Item* oldItem = getInventoryItem((slots_t)index);
	if (!oldItem) {
		return /*RET_NOTPOSSIBLE*/;
	}

	Item* item = thing->getItem();
	if (!item) {
		return /*RET_NOTPOSSIBLE*/;
	}

	//send to client
	sendInventoryItem((slots_t)index, item);

	//event methods
	onUpdateInventoryItem(oldItem, item);

	item->setParent(this);

	inventory[index] = item;
}

void Player::__removeThing(Thing* thing, uint32_t count)
{
	Item* item = thing->getItem();
	if (!item) {
		return /*RET_NOTPOSSIBLE*/;
	}

	int32_t index = __getIndexOfThing(thing);
	if (index == -1) {
		return /*RET_NOTPOSSIBLE*/;
	}

	if (item->isStackable()) {
		if (count == item->getItemCount()) {
			//send change to client
			sendInventoryItem((slots_t)index, nullptr);

			//event methods
			onRemoveInventoryItem(item);

			item->setParent(nullptr);
			inventory[index] = nullptr;
		} else {
			uint8_t newCount = (uint8_t)std::max<int32_t>(0, item->getItemCount() - count);
			item->setItemCount(newCount);

			//send change to client
			sendInventoryItem((slots_t)index, item);

			//event methods
			onUpdateInventoryItem(item, item);
		}
	} else {
		//send change to client
		sendInventoryItem((slots_t)index, nullptr);

		//event methods
		onRemoveInventoryItem(item);

		item->setParent(nullptr);
		inventory[index] = nullptr;
	}
}

int32_t Player::__getIndexOfThing(const Thing* thing) const
{
	for (int i = CONST_SLOT_FIRST; i <= CONST_SLOT_LAST; ++i) {
		if (inventory[i] == thing) {
			return i;
		}
	}
	return -1;
}

int32_t Player::__getFirstIndex() const
{
	return CONST_SLOT_FIRST;
}

int32_t Player::__getLastIndex() const
{
	return CONST_SLOT_LAST + 1;
}

uint32_t Player::__getItemTypeCount(uint16_t itemId, int32_t subType /*= -1*/) const
{
	uint32_t count = 0;
	for (int32_t i = CONST_SLOT_FIRST; i <= CONST_SLOT_LAST; i++) {
		Item* item = inventory[i];
		if (!item) {
			continue;
		}

		if (item->getID() == itemId) {
			count += Item::countByType(item, subType);
		} else if (Container* container = item->getContainer()) {
			for (ContainerIterator it = container->begin(), end = container->end(); it != end; ++it) {
				if ((*it)->getID() == itemId) {
					count += Item::countByType(*it, subType);
				}
			}
		}
	}
	return count;
}

bool Player::removeItemOfType(uint16_t itemId, uint32_t amount, int32_t subType, bool ignoreEquipped/* = false*/) const
{
	if (amount == 0) {
		return true;
	}

	std::vector<Item*> itemList;

	uint32_t count = 0;
	for (int32_t i = CONST_SLOT_FIRST; i <= CONST_SLOT_LAST && count < amount; i++) {
		Item* item = inventory[i];
		if (!item) {
			continue;
		}

		if (!ignoreEquipped && item->getID() == itemId) {
			count += Item::countByType(item, subType);
			itemList.push_back(item);
		} else if (Container* container = item->getContainer()) {
			for (ContainerIterator it = container->begin(), end = container->end(); it != end; ++it) {
				Item* containerItem = *it;
				if (containerItem->getID() == itemId) {
					count += Item::countByType(containerItem, subType);
					itemList.push_back(containerItem);
					if (count >= amount) {
						break;
					}
				}
			}
		}
	}

	if (count < amount) {
		return false;
	}

	const ItemType& it = Item::items[itemId];
	if (it.stackable) {
		for (Item* item : itemList) {
			if (item->getItemCount() > amount) {
				g_game.internalRemoveItem(item, amount);
				break;
			} else {
				amount -= item->getItemCount();
				g_game.internalRemoveItem(item);
			}
		}
	} else {
		for (Item* item : itemList) {
			g_game.internalRemoveItem(item);
		}
	}
	return true;
}

std::map<uint32_t, uint32_t>& Player::__getAllItemTypeCount(std::map<uint32_t, uint32_t>& countMap) const
{
	for (int32_t i = CONST_SLOT_FIRST; i <= CONST_SLOT_LAST; i++) {
		Item* item = inventory[i];
		if (!item) {
			continue;
		}

		countMap[item->getID()] += Item::countByType(item, -1);

		if (Container* container = item->getContainer()) {
			for (ContainerIterator it = container->begin(), end = container->end(); it != end; ++it) {
				countMap[(*it)->getID()] += Item::countByType(*it, -1);
			}
		}
	}
	return countMap;
}

Thing* Player::__getThing(size_t index) const
{
	if (index >= CONST_SLOT_FIRST && index <= CONST_SLOT_LAST) {
		return inventory[index];
	}
	return nullptr;
}

void Player::postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t link /*= LINK_OWNER*/)
{
	if (link == LINK_OWNER) {
		//calling movement scripts
		g_moveEvents->onPlayerEquip(this, thing->getItem(), (slots_t)index, false);
	}

	bool requireListUpdate = true;

	if (link == LINK_OWNER || link == LINK_TOPPARENT) {
		const Item* i = (oldParent ? oldParent->getItem() : nullptr);

		// Check if we owned the old container too, so we don't need to do anything,
		// as the list was updated in postRemoveNotification
		assert(i ? i->getContainer() != nullptr : true);

		if (i) {
			requireListUpdate = i->getContainer()->getHoldingPlayer() != this;
		} else {
			requireListUpdate = oldParent != this;
		}

		updateInventoryWeight();
		updateItemsLight();
		sendStats();
	}

	if (const Item* item = thing->getItem()) {
		if (const Container* container = item->getContainer()) {
			onSendContainer(container);
		}

		if (shopOwner && requireListUpdate) {
			updateSaleShopList(item->getID());
		}
	} else if (const Creature* creature = thing->getCreature()) {
		if (creature == this) {
			//check containers
			std::vector<Container*> containers;

			for (const auto& it : openContainers) {
				Container* container = it.second.container;
				if (!Position::areInRange<1, 1, 0>(container->getPosition(), getPosition())) {
					containers.push_back(container);
				}
			}

			for (const Container* container : containers) {
				autoCloseContainers(container);
			}
		}
	}
}

void Player::postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, bool isCompleteRemoval, cylinderlink_t link /*= LINK_OWNER*/)
{
	if (link == LINK_OWNER) {
		//calling movement scripts
		g_moveEvents->onPlayerDeEquip(this, thing->getItem(), (slots_t)index, isCompleteRemoval);
	}

	bool requireListUpdate = true;

	if (link == LINK_OWNER || link == LINK_TOPPARENT) {
		const Item* i = (newParent ? newParent->getItem() : nullptr);

		// Check if we owned the old container too, so we don't need to do anything,
		// as the list was updated in postRemoveNotification
		assert(i ? i->getContainer() != nullptr : true);

		if (i) {
			requireListUpdate = i->getContainer()->getHoldingPlayer() != this;
		} else {
			requireListUpdate = newParent != this;
		}

		updateInventoryWeight();
		updateItemsLight();
		sendStats();
	}

	if (const Item* item = thing->getItem()) {
		if (const Container* container = item->getContainer()) {
			if (container->isRemoved() || !Position::areInRange<1, 1, 0>(getPosition(), container->getPosition())) {
				autoCloseContainers(container);
			} else if (container->getTopParent() == this) {
				onSendContainer(container);
			} else if (const Container* topContainer = dynamic_cast<const Container*>(container->getTopParent())) {
				if (const DepotChest* depotChest = dynamic_cast<const DepotChest*>(topContainer)) {
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
				} else {
					onSendContainer(container);
				}
			} else {
				autoCloseContainers(container);
			}
		}

		if (shopOwner && requireListUpdate) {
			updateSaleShopList(item->getID());
		}
	}
}

void Player::updateSaleShopList(uint32_t itemId)
{
	auto it = std::find_if(shopItemList.begin(), shopItemList.end(), [itemId] (const ShopInfo& shopInfo) { return shopInfo.itemId == itemId; });
	if (it == shopItemList.end()) {
		return;
	}

	if (client) {
		client->sendSaleItemList(shopItemList);
	}
}

bool Player::hasShopItemForSale(uint32_t itemId, uint8_t subType) const
{
	const ItemType& itemType = Item::items[itemId];
	for (const ShopInfo& shopInfo : shopItemList) {
		if (shopInfo.itemId != itemId) {
			continue;
		}

		if (shopInfo.buyPrice == 0) {
			continue;
		}

		if (itemType.isFluidContainer() && shopInfo.subType != subType) {
			continue;
		}

		return true;
	}
	return false;
}

void Player::__internalAddThing(Thing* thing)
{
	__internalAddThing(0, thing);
}

void Player::__internalAddThing(uint32_t index, Thing* thing)
{
	Item* item = thing->getItem();
	if (!item) {
		return;
	}

	//index == 0 means we should equip this item at the most appropiate slot
	if (index == 0) {
		return;
	}

	if (index > 0 && index < 11) {
		if (inventory[index]) {
			return;
		}

		inventory[index] = item;
		item->setParent(this);
	}
}

bool Player::setFollowCreature(Creature* creature)
{
	if (!Creature::setFollowCreature(creature)) {
		setFollowCreature(nullptr);
		setAttackedCreature(nullptr);

		sendCancelMessage(RET_THEREISNOWAY);
		sendCancelTarget();
		stopWalk();
		return false;
	}
	return true;
}

bool Player::setAttackedCreature(Creature* creature)
{
	if (!Creature::setAttackedCreature(creature)) {
		sendCancelTarget();
		return false;
	}

	if (chaseMode == CHASEMODE_FOLLOW && creature) {
		if (followCreature != creature) {
			//chase opponent
			setFollowCreature(creature);
		}
	} else if (followCreature) {
		setFollowCreature(nullptr);
	}

	if (creature) {
		g_dispatcher.addTask(createTask(std::bind(&Game::checkCreatureAttack, &g_game, getID())));
	}
	return true;
}

void Player::goToFollowCreature()
{
	if (!walkTask) {
		if ((OTSYS_TIME() - lastFailedFollow) < 2000) {
			return;
		}

		Creature::goToFollowCreature();

		if (followCreature && !hasFollowPath) {
			lastFailedFollow = OTSYS_TIME();
		}
	}
}

void Player::getPathSearchParams(const Creature* creature, FindPathParams& fpp) const
{
	Creature::getPathSearchParams(creature, fpp);
	fpp.fullPathSearch = true;
}

void Player::onAttacking(uint32_t interval)
{
	Creature::onAttacking(interval);
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

		Item* tool = getWeapon();
		const Weapon* weapon = g_weapons->getWeapon(tool);
		if (weapon) {
			if (!weapon->interruptSwing()) {
				result = weapon->useWeapon(this, tool, attackedCreature);
			} else if (!canDoAction()) {
				uint32_t delay = getNextActionTime();
				SchedulerTask* task = createSchedulerTask(delay, std::bind(&Game::checkCreatureAttack,
				                      &g_game, getID()));
				setNextActionTask(task);
			} else {
				result = weapon->useWeapon(this, tool, attackedCreature);
			}
		} else {
			result = Weapon::useFist(this, attackedCreature);
		}

		if (result) {
			lastAttack = OTSYS_TIME();
		}
	}
}

uint64_t Player::getGainedExperience(Creature* attacker) const
{
	if (g_config.getBoolean(ConfigManager::EXPERIENCE_FROM_PLAYERS)) {
		Player* attackerPlayer = attacker->getPlayer();
		if (attackerPlayer && attackerPlayer != this && skillLoss && std::abs((int32_t)(attackerPlayer->getLevel() - level)) <= g_config.getNumber(ConfigManager::EXP_FROM_PLAYERS_LEVEL_RANGE)) {
			return std::max<uint64_t>(0, std::floor(getLostExperience() * getDamageRatio(attacker) * 0.75));
		}
	}
	return 0;
}

void Player::onFollowCreature(const Creature* creature)
{
	if (!creature) {
		stopWalk();
	}
}

void Player::setChaseMode(chaseMode_t mode)
{
	chaseMode_t prevChaseMode = chaseMode;
	chaseMode = mode;

	if (prevChaseMode != chaseMode) {
		if (chaseMode == CHASEMODE_FOLLOW) {
			if (!followCreature && attackedCreature) {
				//chase opponent
				setFollowCreature(attackedCreature);
			}
		} else if (attackedCreature) {
			setFollowCreature(nullptr);
			cancelNextWalk = true;
		}
	}
}

void Player::setFightMode(fightMode_t mode)
{
	fightMode = mode;
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

void Player::stopWalk()
{
	cancelNextWalk = true;
}

void Player::getCreatureLight(LightInfo& light) const
{
	if (internalLight.level > itemsLight.level) {
		light = internalLight;
	} else {
		light = itemsLight;
	}
}

void Player::updateItemsLight(bool internal /*=false*/)
{
	LightInfo maxLight;
	LightInfo curLight;

	for (int32_t i = CONST_SLOT_FIRST; i <= CONST_SLOT_LAST; ++i) {
		Item* item = inventory[i];
		if (item) {
			item->getLight(curLight);

			if (curLight.level > maxLight.level) {
				maxLight = curLight;
			}
		}
	}

	if (itemsLight.level != maxLight.level || itemsLight.color != maxLight.color) {
		itemsLight = maxLight;

		if (!internal) {
			g_game.changeLight(this);
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
			g_game.updatePlayerSkull(this);
		}
	}

	sendIcons();
}

void Player::onCombatRemoveCondition(Condition* condition)
{
	//Creature::onCombatRemoveCondition(condition);
	if (condition->getId() > 0) {
		//Means the condition is from an item, id == slot
		if (g_game.getWorldType() == WORLD_TYPE_PVP_ENFORCED) {
			Item* item = getInventoryItem((slots_t)condition->getId());
			if (item) {
				//25% chance to destroy the item
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

void Player::onAttackedCreature(Creature* target)
{
	Creature::onAttackedCreature(target);

	if (target == this) {
		addInFightTicks();
		return;
	}

	if (hasFlag(PlayerFlag_NotGainInFight)) {
		return;
	}

	Player* targetPlayer = target->getPlayer();
	if (targetPlayer && !isPartner(targetPlayer) && !isGuildMate(targetPlayer)) {
		if (!pzLocked && g_game.getWorldType() == WORLD_TYPE_PVP_ENFORCED) {
			pzLocked = true;
			sendIcons();
		}

		if (getSkull() == SKULL_NONE && getSkullClient(targetPlayer) == SKULL_YELLOW) {
			addAttacked(targetPlayer);
			targetPlayer->sendCreatureSkull(this);
		} else if (!targetPlayer->hasAttacked(this)) {
			if (!pzLocked && g_game.getWorldType() != WORLD_TYPE_PVP_ENFORCED) {
				pzLocked = true;
				sendIcons();
			}

			if (!Combat::isInPvpZone(this, targetPlayer) && !isInWar(targetPlayer)) {
				addAttacked(targetPlayer);

				if (targetPlayer->getSkull() == SKULL_NONE && getSkull() == SKULL_NONE) {
					setSkull(SKULL_WHITE);
					g_game.updatePlayerSkull(this);
				}

				if (getSkull() == SKULL_NONE) {
					targetPlayer->sendCreatureSkull(this);
				}
			}
		}
	}

	addInFightTicks();
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
		party->clearPlayerPoints(this);
	}
}

void Player::onPlacedCreature()
{
	//scripting event - onLogin
	if (!g_creatureEvents->playerLogin(this)) {
		kickPlayer(true);
	}
}

void Player::onRemovedCreature()
{
	//
}

void Player::onAttackedCreatureDrainHealth(Creature* target, int32_t points)
{
	Creature::onAttackedCreatureDrainHealth(target, points);

	if (target) {
		if (party && !Combat::isPlayerCombat(target)) {
			Monster* tmpMonster = target->getMonster();
			if (tmpMonster && tmpMonster->isHostile()) {
				//We have fulfilled a requirement for shared experience
				party->addPlayerDamageMonster(this, points);
			}
		}
	}
}

void Player::onTargetCreatureGainHealth(Creature* target, int32_t points)
{
	if (target && party) {
		Player* tmpPlayer = nullptr;

		if (target->getPlayer()) {
			tmpPlayer = target->getPlayer();
		} else if (Creature* targetMaster = target->getMaster()) {
			if (Player* targetMasterPlayer = targetMaster->getPlayer()) {
				tmpPlayer = targetMasterPlayer;
			}
		}

		if (isPartner(tmpPlayer)) {
			party->addPlayerHealedMember(this, points);
		}
	}
}

bool Player::onKilledCreature(Creature* target, bool lastHit/* = true*/)
{
	bool unjustified = false;

	if (hasFlag(PlayerFlag_NotGenerateLoot)) {
		target->setDropLoot(false);
	}

	Creature::onKilledCreature(target, lastHit);

	if (Player* targetPlayer = target->getPlayer()) {
		if (targetPlayer && targetPlayer->getZone() == ZONE_PVP) {
			targetPlayer->setDropLoot(false);
			targetPlayer->setLossSkill(false);
		} else if (!hasFlag(PlayerFlag_NotGainInFight) && !isPartner(targetPlayer)) {
			if (!Combat::isInPvpZone(this, targetPlayer) && hasAttacked(targetPlayer) && !targetPlayer->hasAttacked(this) && !isGuildMate(targetPlayer) && targetPlayer != this) {
				if (targetPlayer->getSkull() == SKULL_NONE && !isInWar(targetPlayer)) {
					unjustified = true;
					addUnjustifiedDead(targetPlayer);
				}

				if (lastHit && hasCondition(CONDITION_INFIGHT)) {
					pzLocked = true;
					Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_INFIGHT, g_config.getNumber(ConfigManager::WHITE_SKULL_TIME), 0);
					addCondition(condition);
				}
			}
		}
	}

	return unjustified;
}

void Player::gainExperience(uint64_t gainExp)
{
	if (!hasFlag(PlayerFlag_NotGainExperience) && gainExp > 0) {
		if (staminaMinutes == 0) {
			return;
		}

		uint64_t oldExperience = experience;

		addExperience(gainExp * g_game.getExperienceStage(level), true, true);

		//soul regeneration
		int64_t gainedExperience = experience - oldExperience;
		if (gainedExperience >= level) {
			Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_SOUL, 4 * 60 * 1000, 0);
			condition->setParam(CONDITION_PARAM_SOULGAIN, 1);
			condition->setParam(CONDITION_PARAM_SOULTICKS, vocation->getSoulGainTicks() * 1000);
			addCondition(condition);
		}
	}
}

void Player::onGainExperience(uint64_t gainExp, Creature* target)
{
	if (hasFlag(PlayerFlag_NotGainExperience)) {
		return;
	}

	if (target) {
		if (gainExp > 0 && target->getMonster()) {
			useStamina();
		}

		if (!target->getPlayer() && party && party->isSharedExperienceActive() && party->isSharedExperienceEnabled()) {
			party->shareExperience(gainExp);
			//We will get a share of the experience through the sharing mechanism
			return;
		}
	}

	Creature::onGainExperience(gainExp, target);
	gainExperience(gainExp);
}

void Player::onGainSharedExperience(uint64_t gainExp)
{
	gainExperience(gainExp);
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

bool Player::isAttackable() const
{
	return !hasFlag(PlayerFlag_CannotBeAttacked);
}

void Player::changeHealth(int32_t healthChange, bool sendHealthChange/* = true*/)
{
	Creature::changeHealth(healthChange, sendHealthChange);
	sendStats();
}

void Player::changeMana(int32_t manaChange)
{
	if (!hasFlag(PlayerFlag_HasInfiniteMana)) {
		Creature::changeMana(manaChange);
	}

	sendStats();
}

void Player::changeSoul(int32_t soulChange)
{
	if (soulChange > 0) {
		soul += std::min<int32_t>(soulChange, soulMax - soul);
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

	const Outfit* outfit = Outfits::getInstance()->getOutfitByLookType(sex, lookType);
	if (!outfit) {
		return false;
	}

	if (outfit->premium && !isPremium()) {
		return false;
	}

	if (outfit->unlocked && addons == 0) {
		return true;
	}

	for (const OutfitEntry& outfitEntry : outfits) {
		if (outfitEntry.lookType != lookType) {
			continue;
		}
		return (outfitEntry.addons & addons) == addons;
	}
	return false;
}

bool Player::canLogout()
{
	if (isConnecting) {
		return false;
	}

	if (getTile()->hasFlag(TILESTATE_NOLOGOUT)) {
		return false;
	}

	if (getTile()->hasFlag(TILESTATE_PROTECTIONZONE)) {
		return true;
	}

	return !isPzLocked() && !hasCondition(CONDITION_INFIGHT);
}

void Player::genReservedStorageRange()
{
	//generate outfits range
	uint32_t base_key = PSTRG_OUTFITS_RANGE_START;
	for (const OutfitEntry& entry : outfits) {
		storageMap[++base_key] = (entry.lookType << 16) | entry.addons;
	}
}

void Player::addOutfit(uint16_t lookType, uint8_t addons)
{
	for (OutfitEntry& outfitEntry : outfits) {
		if (outfitEntry.lookType == lookType) {
			outfitEntry.addons |= addons;
			return;
		}
	}
	outfits.emplace_back(lookType, addons);
}

bool Player::removeOutfit(uint16_t lookType)
{
	for (auto it = outfits.begin(); it != outfits.end(); ++it) {
		OutfitEntry& entry = *it;
		if (entry.lookType == lookType) {
			outfits.erase(it);
			return true;
		}
	}
	return false;
}

bool Player::removeOutfitAddon(uint16_t lookType, uint8_t addons)
{
	for (OutfitEntry& outfitEntry : outfits) {
		if (outfitEntry.lookType == lookType) {
			outfitEntry.addons &= ~addons;
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

	for (const OutfitEntry& outfitEntry : outfits) {
		if (outfitEntry.lookType != outfit.lookType) {
			continue;
		}

		addons = outfitEntry.addons;
		return true;
	}

	if (!outfit.unlocked) {
		return false;
	}

	addons = 0;
	return true;
}

void Player::setSex(PlayerSex_t newSex)
{
	sex = newSex;
}

Skulls_t Player::getSkull() const
{
	if (hasFlag(PlayerFlag_NotGainInFight)) {
		return SKULL_NONE;
	}
	return skull;
}

Skulls_t Player::getSkullClient(const Player* player) const
{
	if (!player || g_game.getWorldType() != WORLD_TYPE_PVP) {
		return SKULL_NONE;
	}

	if (player->getSkull() == SKULL_NONE) {
		if (isInWar(player)) {
			return SKULL_GREEN;
		}

		if (!player->getGuildWarList().empty() && guild == player->getGuild()) {
			return SKULL_GREEN;
		}

		if (player->hasAttacked(this)) {
			return SKULL_YELLOW;
		}

		if (isPartner(player)) {
			return SKULL_GREEN;
		}
	}
	return player->getSkull();
}

bool Player::hasAttacked(const Player* attacked) const
{
	if (hasFlag(PlayerFlag_NotGainInFight) || !attacked) {
		return false;
	}

	return attackedSet.find(attacked->getGUID()) != attackedSet.end();
}

void Player::addAttacked(const Player* attacked)
{
	if (hasFlag(PlayerFlag_NotGainInFight) || !attacked || attacked == this) {
		return;
	}

	attackedSet.insert(attacked->getGUID());
}

void Player::clearAttacked()
{
	attackedSet.clear();
}

void Player::addUnjustifiedDead(const Player* attacked)
{
	if (hasFlag(PlayerFlag_NotGainInFight) || attacked == this || g_game.getWorldType() == WORLD_TYPE_PVP_ENFORCED) {
		return;
	}

	if (client) {
		std::ostringstream ss;
		ss << "Warning! The murder of " << attacked->getName() << " was not justified.";
		client->sendTextMessage(MESSAGE_EVENT_ADVANCE, ss.str());
	}

	skullTicks += g_config.getNumber(ConfigManager::FRAG_TIME);

	if (getSkull() != SKULL_BLACK) {
		if (g_config.getNumber(ConfigManager::KILLS_TO_BLACK) != 0 && skullTicks > (g_config.getNumber(ConfigManager::KILLS_TO_BLACK) - 1) * g_config.getNumber(ConfigManager::FRAG_TIME)) {
			setSkull(SKULL_BLACK);
			g_game.updatePlayerSkull(this);
		} else if (getSkull() != SKULL_RED && g_config.getNumber(ConfigManager::KILLS_TO_RED) != 0 && skullTicks > (g_config.getNumber(ConfigManager::KILLS_TO_RED) - 1) * g_config.getNumber(ConfigManager::FRAG_TIME)) {
			setSkull(SKULL_RED);
			g_game.updatePlayerSkull(this);
		}
	}
}

void Player::checkSkullTicks(int32_t ticks)
{
	int32_t newTicks = skullTicks - ticks;
	if (newTicks < 0) {
		skullTicks = 0;
	} else {
		skullTicks = newTicks;
	}

	if ((skull == SKULL_RED || skull == SKULL_BLACK) && skullTicks < 1000 && !hasCondition(CONDITION_INFIGHT)) {
		setSkull(SKULL_NONE);
		g_game.updatePlayerSkull(this);
	}
}

bool Player::isPromoted() const
{
	int32_t promotedVocation = g_vocations.getPromotedVocation(vocation->getId());
	return promotedVocation == 0 && vocation->getId() != promotedVocation;
}

double Player::getLostPercent() const
{
	std::bitset<5> bitset(blessings);

	const int32_t deathLosePercent = g_config.getNumber(ConfigManager::DEATH_LOSE_PERCENT);
	if (deathLosePercent != -1) {
		int32_t lossPercent = deathLosePercent;

		if (isPromoted()) {
			lossPercent -= 3;
		}

		lossPercent -= (int32_t)bitset.count();
		return std::max<int32_t>(0, lossPercent) / (double)100;
	} else {
		double lossPercent;

		if (level >= 25) {
			double tmpLevel = level + (levelPercent / 100.);
			lossPercent = (double)((tmpLevel + 50) * 50 * ((tmpLevel * tmpLevel) - (5 * tmpLevel) + 8)) / experience;
		} else {
			lossPercent = 10;
		}

		if (isPromoted()) {
			lossPercent *= 0.7;
		}

		return lossPercent * pow(0.92, (int32_t)bitset.count()) / 100;
	}
}

void Player::learnInstantSpell(const std::string& name)
{
	if (!hasLearnedInstantSpell(name)) {
		learnedInstantSpellList.push_front(name);
	}
}

void Player::forgetInstantSpell(const std::string& name)
{
	learnedInstantSpellList.remove(name);
}

bool Player::hasLearnedInstantSpell(const std::string& name) const
{
	if (hasFlag(PlayerFlag_CannotUseSpells)) {
		return false;
	}

	if (hasFlag(PlayerFlag_IgnoreSpellCheck)) {
		return true;
	}

	for (const auto& learnedSpellName : learnedInstantSpellList) {
		if (strcasecmp(learnedSpellName.c_str(), name.c_str()) == 0) {
			return true;
		}
	}
	return false;
}

bool Player::isInWar(const Player* player) const
{
	if (!player || !guild) {
		return false;
	}

	const Guild* playerGuild = player->getGuild();
	if (!playerGuild) {
		return false;
	}

	return isInWarList(playerGuild->getId()) && player->isInWarList(guild->getId());
}

bool Player::isInWarList(uint32_t guildId) const
{
	return std::find(guildWarList.begin(), guildWarList.end(), guildId) != guildWarList.end();
}

bool Player::isPremium() const
{
	if (g_config.getBoolean(ConfigManager::FREE_PREMIUM) || hasFlag(PlayerFlag_IsAlwaysPremium)) {
		return true;
	}

	return premiumDays > 0;
}

void Player::setPremiumDays(int32_t v)
{
	premiumDays = v;
	sendBasicData();
}

void Player::setGuildLevel(uint8_t newGuildLevel)
{
	guildLevel = newGuildLevel;
}

void Player::setGroup(Group* newGroup)
{
	group = newGroup;

	if (group->maxDepotItems > 0) {
		maxDepotItems = group->maxDepotItems;
	} else if (isPremium()) {
		maxDepotItems = 2000;
	}

	if (group->maxVipEntries > 0) {
		maxVipEntries = group->maxVipEntries;
	} else if (isPremium()) {
		maxVipEntries = 100;
	}
}

PartyShields_t Player::getPartyShield(const Player* player) const
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

		if (isInviting(player)) {
			return SHIELD_WHITEBLUE;
		}
	}

	if (player->isInviting(this)) {
		return SHIELD_WHITEYELLOW;
	}

	if (player->party) {
		return SHIELD_GRAY;
	}

	return SHIELD_NONE;
}

bool Player::isInviting(const Player* player) const
{
	if (!player || !party || party->getLeader() != this) {
		return false;
	}
	return party->isPlayerInvited(player);
}

bool Player::isPartner(const Player* player) const
{
	if (!player || !party) {
		return false;
	}
	return party == player->party;
}

bool Player::isGuildMate(const Player* player) const
{
	if (!player || !guild) {
		return false;
	}
	return guild == player->guild;
}

void Player::sendPlayerPartyIcons(Player* player)
{
	sendCreatureShield(player);
	sendCreatureSkull(player);
}

bool Player::addPartyInvitation(Party* party)
{
	if (!party) {
		return false;
	}

	auto it = std::find(invitePartyList.begin(), invitePartyList.end(), party);
	if (it != invitePartyList.end()) {
		return false;
	}

	invitePartyList.push_front(party);
	return true;
}

void Player::removePartyInvitation(Party* party)
{
	invitePartyList.remove(party);
}

void Player::clearPartyInvitations()
{
	for (Party* invitingParty : invitePartyList) {
		invitingParty->removeInvite(*this, false);
	}
	invitePartyList.clear();
}

GuildEmblems_t Player::getGuildEmblem(const Player* player) const
{
	if (!player) {
		return GUILDEMBLEM_NONE;
	}

	const Guild* playerGuild = player->getGuild();
	if (!playerGuild) {
		return GUILDEMBLEM_NONE;
	}

	if (player->getGuildWarList().empty()) {
		if (guild == playerGuild) {
			return GUILDEMBLEM_MEMBER;
		} else {
			return GUILDEMBLEM_OTHER;
		}
	} else if (guild == playerGuild) {
		return GUILDEMBLEM_ALLY;
	} else if (isInWar(player)) {
		return GUILDEMBLEM_ENEMY;
	}

	return GUILDEMBLEM_NEUTRAL;
}

uint8_t Player::getCurrentMount() const
{
	int32_t value;
	if (getStorageValue(PSTRG_MOUNTS_CURRENTMOUNT, value)) {
		return value;
	}
	return 0;
}

void Player::setCurrentMount(uint8_t mount)
{
	addStorageValue(PSTRG_MOUNTS_CURRENTMOUNT, mount);
}

bool Player::toggleMount(bool mount)
{
	if ((OTSYS_TIME() - lastToggleMount) < 3000) {
		sendCancelMessage(RET_YOUAREEXHAUSTED);
		return false;
	}

	if (mount) {
		if (isMounted()) {
			return false;
		}

		if (!group->access && _tile->hasFlag(TILESTATE_PROTECTIONZONE)) {
			sendCancelMessage(RET_ACTIONNOTPERMITTEDINPROTECTIONZONE);
			return false;
		}

		uint8_t currentMountId = getCurrentMount();
		if (currentMountId == 0) {
			sendOutfitWindow();
			return false;
		}

		Mount* currentMount = Mounts::getInstance()->getMountByID(currentMountId);
		if (!currentMount) {
			return false;
		}

		if (!hasMount(currentMount)) {
			setCurrentMount(0);
			sendOutfitWindow();
			return false;
		}

		if (currentMount->premium && !isPremium()) {
			sendCancelMessage(RET_YOUNEEDPREMIUMACCOUNT);
			return false;
		}

		if (hasCondition(CONDITION_OUTFIT)) {
			sendCancelMessage(RET_NOTPOSSIBLE);
			return false;
		}

		defaultOutfit.lookMount = currentMount->clientId;

		if (currentMount->speed != 0) {
			g_game.changeSpeed(this, currentMount->speed);
		}
	} else {
		if (!isMounted()) {
			return false;
		}

		dismount();
	}

	g_game.internalCreatureChangeOutfit(this, defaultOutfit);
	lastToggleMount = OTSYS_TIME();
	return true;
}

bool Player::tameMount(uint8_t mountId)
{
	if (!Mounts::getInstance()->getMountByID(mountId)) {
		return false;
	}

	const uint8_t tmpMountId = mountId - 1;
	const uint32_t key = PSTRG_MOUNTS_RANGE_START + (tmpMountId / 31);

	int32_t value;
	if (getStorageValue(key, value)) {
		value |= (1 << (tmpMountId % 31));
	} else {
		value = (1 << (tmpMountId % 31));
	}

	addStorageValue(key, value);
	return true;
}

bool Player::untameMount(uint8_t mountId)
{
	if (!Mounts::getInstance()->getMountByID(mountId)) {
		return false;
	}

	const uint8_t tmpMountId = mountId - 1;
	const uint32_t key = PSTRG_MOUNTS_RANGE_START + (tmpMountId / 31);

	int32_t value;
	if (!getStorageValue(key, value)) {
		return true;
	}

	value &= ~(1 << (tmpMountId % 31));
	addStorageValue(key, value);

	if (getCurrentMount() == mountId) {
		if (isMounted()) {
			dismount();
			g_game.internalCreatureChangeOutfit(this, defaultOutfit);
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

	const uint8_t tmpMountId = mount->id - 1;

	int32_t value;
	if (!getStorageValue(PSTRG_MOUNTS_RANGE_START + (tmpMountId / 31), value)) {
		return false;
	}

	return ((1 << (tmpMountId % 31)) & value) != 0;
}

void Player::dismount()
{
	Mount* mount = Mounts::getInstance()->getMountByID(getCurrentMount());
	if (mount && mount->speed > 0) {
		g_game.changeSpeed(this, -mount->speed);
	}

	defaultOutfit.lookMount = 0;
}

bool Player::addOfflineTrainingTries(skills_t skill, int32_t tries)
{
	if (tries <= 0 || skill == SKILL_LEVEL) {
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
		oldPercentToNextLevel = (long double)(manaSpent * 100) / nextReqMana;

		tries *= g_config.getNumber(ConfigManager::RATE_MAGIC);
		uint32_t currMagLevel = magLevel;

		while ((manaSpent + tries) >= nextReqMana) {
			tries -= nextReqMana - manaSpent;

			magLevel++;
			manaSpent = 0;

			g_creatureEvents->playerAdvance(this, SKILL_MAGLEVEL, magLevel - 1, magLevel);

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
			std::ostringstream ss;
			ss << "You advanced to magic level " << magLevel << '.';
			sendTextMessage(MESSAGE_EVENT_ADVANCE, ss.str());
		}

		uint32_t newPercent;

		if (nextReqMana > currReqMana) {
			newPercent = Player::getPercentLevel(manaSpent, nextReqMana);
			newPercentToNextLevel = (long double)(manaSpent * 100) / nextReqMana;
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
		uint64_t currReqTries = vocation->getReqSkillTries(skill, skills[skill][SKILLVALUE_LEVEL]);
		uint64_t nextReqTries = vocation->getReqSkillTries(skill, skills[skill][SKILLVALUE_LEVEL] + 1);

		if (currReqTries >= nextReqTries) {
			return false;
		}

		oldSkillValue = skills[skill][SKILLVALUE_LEVEL];
		oldPercentToNextLevel = (long double)(skills[skill][SKILLVALUE_TRIES] * 100) / nextReqTries;

		tries *= g_config.getNumber(ConfigManager::RATE_SKILL);
		uint32_t currSkillLevel = skills[skill][SKILLVALUE_LEVEL];

		while ((skills[skill][SKILLVALUE_TRIES] + tries) >= nextReqTries) {
			tries -= nextReqTries - skills[skill][SKILLVALUE_TRIES];

			skills[skill][SKILLVALUE_LEVEL]++;
			skills[skill][SKILLVALUE_TRIES] = 0;
			skills[skill][SKILLVALUE_PERCENT] = 0;

			g_creatureEvents->playerAdvance(this, skill, (skills[skill][SKILLVALUE_LEVEL] - 1), skills[skill][SKILLVALUE_LEVEL]);

			sendUpdate = true;
			currReqTries = nextReqTries;
			nextReqTries = vocation->getReqSkillTries(skill, skills[skill][SKILLVALUE_LEVEL] + 1);

			if (currReqTries >= nextReqTries) {
				tries = 0;
				break;
			}
		}

		skills[skill][SKILLVALUE_TRIES] += tries;

		if (currSkillLevel != skills[skill][SKILLVALUE_LEVEL]) {
			std::ostringstream ss;
			ss << "You advanced to " << getSkillName(skill) << " level " << skills[skill][SKILLVALUE_LEVEL] << '.';
			sendTextMessage(MESSAGE_EVENT_ADVANCE, ss.str());
		}

		uint32_t newPercent;

		if (nextReqTries > currReqTries) {
			newPercent = Player::getPercentLevel(skills[skill][SKILLVALUE_TRIES], nextReqTries);
			newPercentToNextLevel = (long double)(skills[skill][SKILLVALUE_TRIES] * 100) / nextReqTries;
		} else {
			newPercent = 0;
			newPercentToNextLevel = 0;
		}

		if (skills[skill][SKILLVALUE_PERCENT] != newPercent) {
			skills[skill][SKILLVALUE_PERCENT] = newPercent;
			sendUpdate = true;
		}

		newSkillValue = skills[skill][SKILLVALUE_LEVEL];
	}

	std::ostringstream ss;
	ss << std::fixed << std::setprecision(2) << "Your " << ucwords(getSkillName(skill)) << " skill changed from level " << oldSkillValue << " (with " << oldPercentToNextLevel << "% progress towards level " << (oldSkillValue + 1) << ") to level " << newSkillValue << " (with " << newPercentToNextLevel << "% progress towards level " << (newSkillValue + 1) << ')';
	sendTextMessage(MESSAGE_EVENT_ADVANCE, ss.str());
	return sendUpdate;
}

bool Player::hasModalWindowOpen(uint32_t modalWindowId) const
{
	return find(modalWindows.begin(), modalWindows.end(), modalWindowId) != modalWindows.end();
}

void Player::onModalWindowHandled(uint32_t modalWindowId)
{
	modalWindows.remove(modalWindowId);
}

void Player::sendModalWindow(const ModalWindow& modalWindow)
{
	if (!client) {
		return;
	}

	modalWindows.push_front(modalWindow.id);
	client->sendModalWindow(modalWindow);
}

void Player::clearModalWindows()
{
	modalWindows.clear();
}

void Player::regenerateStamina(int32_t offlineTime)
{
	if (!g_config.getBoolean(ConfigManager::STAMINA_SYSTEM)) {
		return;
	}

	offlineTime -= 600;

	if (offlineTime < 180) {
		return;
	}

	int16_t regainStaminaMinutes = offlineTime / 180;
	int16_t maxNormalStaminaRegen = 2400 - std::min<int16_t>(2400, staminaMinutes);

	if (regainStaminaMinutes > maxNormalStaminaRegen) {
		int16_t happyHourStaminaRegen = (offlineTime - (maxNormalStaminaRegen * 180)) / 600;
		staminaMinutes = std::min<int16_t>(2520, std::max<int16_t>(2400, staminaMinutes) + happyHourStaminaRegen);
	} else {
		staminaMinutes += regainStaminaMinutes;
	}
}

void Player::useStamina()
{
	if (!g_config.getBoolean(ConfigManager::STAMINA_SYSTEM) || staminaMinutes == 0) {
		return;
	}

	time_t currentTime = time(nullptr);

	if (currentTime > nextUseStaminaTime) {
		time_t timePassed = currentTime - nextUseStaminaTime;

		if (timePassed > 60) {
			if (staminaMinutes > 2) {
				staminaMinutes -= 2;
			} else {
				staminaMinutes = 0;
			}

			nextUseStaminaTime = currentTime + 120;
		} else {
			--staminaMinutes;
			nextUseStaminaTime = currentTime + 60;
		}

		sendStats();
	}
}

uint16_t Player::getHelpers() const
{
	uint16_t helpers;

	if (guild && party) {
		std::unordered_set<Player*> helperSet;

		const auto& guildMembers = guild->getMembersOnline();
		helperSet.insert(guildMembers.begin(), guildMembers.end());

		const auto& partyMembers = party->getMembers();
		helperSet.insert(partyMembers.begin(), partyMembers.end());

		const auto& partyInvitees = party->getInvitees();
		helperSet.insert(partyInvitees.begin(), partyInvitees.end());

		helperSet.insert(party->getLeader());

		helpers = helperSet.size();
	} else if (guild) {
		helpers = guild->getMembersOnline().size();
	} else if (party) {
		helpers = party->getMemberCount() + party->getInvitationCount() + 1;
	} else {
		helpers = 0;
	}

	return helpers;
}

void Player::sendClosePrivate(uint16_t channelId)
{
	if (channelId == CHANNEL_GUILD || channelId == CHANNEL_PARTY) {
		g_chat.removeUserFromChannel(*this, channelId);
	}

	if (client) {
		client->sendClosePrivate(channelId);
	}
}

uint64_t Player::getMoney() const
{
	std::vector<const Container*> containers;
	uint64_t moneyCount = 0;

	for (int32_t i = CONST_SLOT_FIRST; i <= CONST_SLOT_LAST; ++i) {
		Item* item = inventory[i];
		if (!item) {
			continue;
		}

		const Container* container = item->getContainer();
		if (container) {
			containers.push_back(container);
		} else {
			moneyCount += item->getWorth();
		}
	}

	size_t i = 0;
	while (i < containers.size()) {
		const Container* container = containers[i++];
		for (const Item* item : container->getItemList()) {
			const Container* tmpContainer = item->getContainer();
			if (tmpContainer) {
				containers.push_back(tmpContainer);
			} else {
				moneyCount += item->getWorth();
			}
		}
	}
	return moneyCount;
}
