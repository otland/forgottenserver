/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2015  Mark Samman <mark.samman@gmail.com>
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

#include "monsters.h"
#include "monster.h"
#include "container.h"
#include "tools.h"
#include "spells.h"
#include "combat.h"
#include "luascript.h"
#include "weapons.h"
#include "configmanager.h"
#include "game.h"

#include "pugicast.h"

extern Game g_game;
extern Spells* g_spells;
extern Monsters g_monsters;
extern ConfigManager g_config;

MonsterType::MonsterType()
{
	reset();
}

void MonsterType::reset()
{
	experience = 0;

	defense = 0;
	armor = 0;

	hiddenHealth = false;

	canPushItems = false;
	canPushCreatures = false;
	staticAttackChance = 95;
	maxSummons = 0;
	targetDistance = 1;
	runAwayHealth = 0;
	pushable = true;
	baseSpeed = 200;
	health = 100;
	healthMax = 100;

	outfit.lookHead = 0;
	outfit.lookBody = 0;
	outfit.lookLegs = 0;
	outfit.lookFeet = 0;
	outfit.lookType = 0;
	outfit.lookTypeEx = 0;
	outfit.lookAddons = 0;
	outfit.lookMount = 0;
	lookcorpse = 0;

	skull = SKULL_NONE;

	conditionImmunities = 0;
	damageImmunities = 0;
	race = RACE_BLOOD;
	isSummonable = false;
	isIllusionable = false;
	isConvinceable = false;
	isAttackable = true;
	isHostile = true;

	lightLevel = 0;
	lightColor = 0;

	manaCost = 0;
	summonList.clear();
	lootItems.clear();
	elementMap.clear();

	for (const spellBlock_t& spellBlock : spellAttackList) {
		if (spellBlock.combatSpell) {
			delete spellBlock.spell;
		}
	}
	spellAttackList.clear();

	for (const spellBlock_t& spellBlock : spellDefenseList) {
		if (spellBlock.combatSpell) {
			delete spellBlock.spell;
		}
	}
	spellDefenseList.clear();

	yellSpeedTicks = 0;
	yellChance = 0;
	voiceVector.clear();

	changeTargetSpeed = 0;
	changeTargetChance = 0;

	scriptInterface = nullptr;
	creatureAppearEvent = -1;
	creatureDisappearEvent = -1;
	creatureMoveEvent = -1;
	creatureSayEvent = -1;
	thinkEvent = -1;

	scriptList.clear();
}

Loot::Loot()
{
	reset();
}

void Loot::reset()
{
	id = 0;
	chance = 0;
	countMax = 1;
	subType = -1;
	actionId = -1;
	text = "";
}

MonsterSpell::MonsterSpell()
{
	reset();
}

void MonsterSpell::reset()
{
	name = "";
	scriptName = "";
	chance = 100;
	interval = 2000;
	range = 0;
	minCombatValue = 0;
	maxCombatValue = 0;
	attack = 0;
	skill = 0;
	combatSpell = false;
	isMelee = false;
	needTarget = false;
	needDirection = false;

	length = 0;
	spread = 0;
	radius = 0;
	conditionType = CONDITION_NONE;
	conditionMinDamage = 0;
	conditionMaxDamage = 0;
	conditionStartDamage = 0;
	tickInterval = 0;

	speedChange = 0;
	duration = 0;

	shoot = CONST_ANI_NONE;
	effect = CONST_ME_NONE;
}


MonsterType::~MonsterType()
{
	reset();
}

uint32_t Monsters::getLootRandom()
{
	return uniform_random(0, MAX_LOOTCHANCE) / g_config.getNumber(ConfigManager::RATE_LOOT);
}

void MonsterType::createLoot(Container* corpse)
{
	if (g_config.getNumber(ConfigManager::RATE_LOOT) == 0) {
		corpse->startDecaying();
		return;
	}

	Player* owner = g_game.getPlayerByID(corpse->getCorpseOwner());
	if (!owner || owner->getStaminaMinutes() > 840) {
		for (auto it = lootItems.rbegin(), end = lootItems.rend(); it != end; ++it) {
			std::list<Item*> itemList = createLootItem(*it);
			if (itemList.empty()) {
				continue;
			}

			for (Item* item : itemList) {
				//check containers
				if (Container* container = item->getContainer()) {
					if (!createLootContainer(container, *it)) {
						delete container;
						continue;
					}
				}
				
				if (g_game.internalAddItem(corpse, item) != RETURNVALUE_NOERROR) {
					corpse->internalAddThing(item);
				}
			}
		}

		if (owner) {
			std::ostringstream ss;
			ss << "Loot of " << nameDescription << ": " << corpse->getContentDescription();

			if (owner->getParty()) {
				owner->getParty()->broadcastPartyLoot(ss.str());
			} else {
				owner->sendTextMessage(MESSAGE_INFO_DESCR, ss.str());
			}
		}
	} else {
		std::ostringstream ss;
		ss << "Loot of " << nameDescription << ": nothing (due to low stamina)";

		if (owner->getParty()) {
			owner->getParty()->broadcastPartyLoot(ss.str());
		} else {
			owner->sendTextMessage(MESSAGE_INFO_DESCR, ss.str());
		}
	}

	corpse->startDecaying();
}

void MonsterType::clone(MonsterType* root, MonsterType* child, const std::string& type) {
	if (type == "attacks") {
		if (!child->spellAttackList.empty()) {
			for (spellBlock_t attacks : child->spellAttackList) {
				root->spellAttackList.push_back(attacks);
			}
		}
	} else if (type == "defenses") {
		if (!child->spellDefenseList.empty()) {
			for (spellBlock_t defenses : child->spellDefenseList) {
				root->spellDefenseList.push_back(defenses);
			}
		}
	} else if (type == "loot") {
		if (!child->lootItems.empty()) {
			for (LootBlock loot : child->lootItems) {
				root->lootItems.push_back(loot);
			}
		}
	} else if (type == "voices") {
		if (!child->voiceVector.empty()) {
			for (voiceBlock_t voices : child->voiceVector) {
				root->voiceVector.push_back(voices);
			}
		}
	}
	else if (type == "summons") {
		if (!child->summonList.empty()) {
			for (summonBlock_t summons : child->summonList) {
				root->summonList.push_back(summons);
			}
		}
	} else if (type == "all") {
		root->experience = child->experience;
		root->hiddenHealth = child->hiddenHealth;
		root->canPushCreatures = child->canPushCreatures;
		root->canPushItems = child->canPushItems;
		root->staticAttackChance = child->staticAttackChance;
		root->maxSummons = child->maxSummons;
		root->targetDistance = child->targetDistance;
		root->runAwayHealth = child->runAwayHealth;
		root->pushable = child->pushable;
		root->baseSpeed = child->baseSpeed;
		root->health = child->health;
		root->healthMax = child->healthMax;

		root->outfit.lookHead = child->outfit.lookHead;
		root->outfit.lookBody = child->outfit.lookBody;
		root->outfit.lookLegs = child->outfit.lookLegs;
		root->outfit.lookFeet = child->outfit.lookFeet;
		root->outfit.lookType = child->outfit.lookType;
		root->outfit.lookTypeEx = child->outfit.lookTypeEx;
		root->outfit.lookAddons = child->outfit.lookAddons;
		root->outfit.lookMount = child->outfit.lookMount;
		root->lookcorpse = child->lookcorpse;

		root->skull = child->skull;
		root->conditionImmunities = child->conditionImmunities;
		root->damageImmunities = child->damageImmunities;
		root->race = child->race;
		root->isSummonable = child->isSummonable;
		root->isIllusionable = child->isIllusionable;
		root->isConvinceable = child->isConvinceable;
		root->isAttackable = child->isAttackable;
		root->isHostile = child->isHostile;
		root->lightLevel = child->lightLevel;
		root->lightColor = child->lightColor;
		root->manaCost = child->manaCost;

		root->elementMap[COMBAT_PHYSICALDAMAGE] = child->elementMap[COMBAT_PHYSICALDAMAGE];
		root->elementMap[COMBAT_ICEDAMAGE] = child->elementMap[COMBAT_ICEDAMAGE];
		root->elementMap[COMBAT_EARTHDAMAGE] = child->elementMap[COMBAT_EARTHDAMAGE];
		root->elementMap[COMBAT_FIREDAMAGE] = child->elementMap[COMBAT_FIREDAMAGE];
		root->elementMap[COMBAT_ENERGYDAMAGE] = child->elementMap[COMBAT_ENERGYDAMAGE];
		root->elementMap[COMBAT_HOLYDAMAGE] = child->elementMap[COMBAT_HOLYDAMAGE];
		root->elementMap[COMBAT_DEATHDAMAGE] = child->elementMap[COMBAT_DEATHDAMAGE];
		root->elementMap[COMBAT_DROWNDAMAGE] = child->elementMap[COMBAT_DROWNDAMAGE];
		root->elementMap[COMBAT_LIFEDRAIN] = child->elementMap[COMBAT_LIFEDRAIN];
		root->elementMap[COMBAT_MANADRAIN] = child->elementMap[COMBAT_MANADRAIN];

		if (!child->voiceVector.empty()) {
			for (voiceBlock_t voices : child->voiceVector) {
				root->voiceVector.push_back(voices);
			}
		}

		if (!child->summonList.empty()) {
			for (summonBlock_t summons : child->summonList) {
				root->summonList.push_back(summons);
			}
		}
	
		if (!child->spellAttackList.empty()) {
			for (spellBlock_t attacks : child->spellAttackList) {
				root->spellAttackList.push_back(attacks);
			}
		}

		if (!child->spellDefenseList.empty()) {
			for (spellBlock_t defenses : child->spellDefenseList) {
				root->spellDefenseList.push_back(defenses);
			}
		}

		if (!child->lootItems.empty()) {
			for (LootBlock loot : child->lootItems) {
				root->lootItems.push_back(loot);
			}
		}

	} else {
		std::cout << "[Error - MonsterType::clone] unknown type" << std::endl;
	}
}

void MonsterType::loadLoot(MonsterType* monsterType, Loot* loot) 
{
	LootBlock lootblock;
	
	lootblock.id = loot->id;
	lootblock.countmax = loot->countMax;
	
	if (loot->chance != 0) {
		lootblock.chance = loot->chance;
	} else {
		lootblock.chance = MAX_LOOTCHANCE;
	}
	
	if (loot->actionId != -1) {
		lootblock.actionId = loot->actionId;
	}

	if (loot->subType != -1) {
		lootblock.subType = loot->subType;
	} else {
		uint32_t charges = Item::items[lootblock.id].charges;
		if (charges != 0) {
			lootblock.subType = charges;
		}
	}

	if (!loot->text.empty()) {
		lootblock.text = loot->text;
	}

	if (!loot->children.empty()) {
		bool isContainer = Item::items[lootblock.id].isContainer();
		if (isContainer) {
			for (Loot* l : loot->children) {
				loadChildLoot(l, lootblock);
			}
		}
		monsterType->lootItems.push_back(lootblock);
	} else {
		monsterType->lootItems.push_back(lootblock);
	}
}

void MonsterType::loadChildLoot(Loot* loot, LootBlock& parent)
{
	LootBlock child;

	child.id = loot->id;
	child.countmax = loot->countMax;

	if (loot->chance != 0) {
		child.chance = loot->chance;
	}
	else {
		child.chance = MAX_LOOTCHANCE;
	}

	if (loot->actionId != -1) {
		child.actionId = loot->actionId;
	}

	if (loot->subType != -1) {
		child.subType = loot->subType;
	}
	else {
		uint32_t charges = Item::items[child.id].charges;
		if (charges != 0) {
			child.subType = charges;
		}
	}

	if (!loot->text.empty()) {
		child.text = loot->text;
	}

	parent.childLoot.push_back(child);
}

std::list<Item*> MonsterType::createLootItem(const LootBlock& lootBlock)
{
	int32_t itemCount = 0;

	uint32_t randvalue = Monsters::getLootRandom();
	if (randvalue < lootBlock.chance) {
		if (Item::items[lootBlock.id].stackable) {
			itemCount = randvalue % lootBlock.countmax + 1;
		} else {
			itemCount = 1;
		}
	}

	std::list<Item*> itemList;
	while (itemCount > 0) {
		uint16_t n = static_cast<uint16_t>(std::min<int32_t>(itemCount, 100));
		Item* tmpItem = Item::CreateItem(lootBlock.id, n);
		if (!tmpItem) {
			break;
		}

		itemCount -= n;

		if (lootBlock.subType != -1) {
			tmpItem->setSubType(lootBlock.subType);
		}

		if (lootBlock.actionId != -1) {
			tmpItem->setActionId(lootBlock.actionId);
		}

		if (!lootBlock.text.empty()) {
			tmpItem->setText(lootBlock.text);
		}

		itemList.push_back(tmpItem);
	}
	return itemList;
}

bool MonsterType::createLootContainer(Container* parent, const LootBlock& lootblock)
{
	auto it = lootblock.childLoot.begin(), end = lootblock.childLoot.end();
	if (it == end) {
		return true;
	}

	for (; it != end && parent->size() < parent->capacity(); ++it) {
		std::list<Item*> itemList = createLootItem(*it);
		for (Item* tmpItem : itemList) {
			if (Container* container = tmpItem->getContainer()) {
				if (!createLootContainer(container, *it)) {
					delete container;
				} else {
					parent->internalAddThing(container);
				}
			} else {
				parent->internalAddThing(tmpItem);
			}
		}
	}
	return !parent->empty();
}

Monsters::Monsters()
{
	loaded = false;
	scriptInterface = nullptr;
}

Monsters::~Monsters()
{
	for (const auto& it : monsters) {
		delete it.second;
	}
	delete scriptInterface;
}

bool Monsters::loadFromXml(bool reloading /*= false*/)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/monster/monsters.xml");
	if (!result) {
		printXMLError("Error - Monsters::loadFromXml", "data/monster/monsters.xml", result);
		return false;
	}

	loaded = true;
	for (pugi::xml_node monsterNode = doc.child("monsters").first_child(); monsterNode; monsterNode = monsterNode.next_sibling()) {
		loadMonster("data/monster/" + std::string(monsterNode.attribute("file").as_string()), monsterNode.attribute("name").as_string(), reloading);
	}

	if (!monsterScriptList.empty()) {
		if (!scriptInterface) {
			scriptInterface = new LuaScriptInterface("Monster Interface");
			scriptInterface->initState();
		}

		for (const auto& scriptEntry : monsterScriptList) {
			MonsterType* mType = scriptEntry.first;
			if (scriptInterface->loadFile("data/monster/scripts/" + scriptEntry.second) == 0) {
				mType->scriptInterface = scriptInterface;
				mType->creatureAppearEvent = scriptInterface->getEvent("onCreatureAppear");
				mType->creatureDisappearEvent = scriptInterface->getEvent("onCreatureDisappear");
				mType->creatureMoveEvent = scriptInterface->getEvent("onCreatureMove");
				mType->creatureSayEvent = scriptInterface->getEvent("onCreatureSay");
				mType->thinkEvent = scriptInterface->getEvent("onThink");
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Can not load script: " << scriptEntry.second << std::endl;
				std::cout << scriptInterface->getLastLuaError() << std::endl;
			}
		}
	}
	return true;
}

bool Monsters::reload()
{
	loaded = false;

	delete scriptInterface;
	scriptInterface = nullptr;
	monsterScriptList.clear();

	return loadFromXml(true);
}

ConditionDamage* Monsters::getDamageCondition(ConditionType_t conditionType,
        int32_t maxDamage, int32_t minDamage, int32_t startDamage, uint32_t tickInterval)
{
	ConditionDamage* condition = reinterpret_cast<ConditionDamage*>(Condition::createCondition(CONDITIONID_COMBAT, conditionType, 0, 0));
	condition->setParam(CONDITION_PARAM_TICKINTERVAL, tickInterval);
	condition->setParam(CONDITION_PARAM_MINVALUE, minDamage);
	condition->setParam(CONDITION_PARAM_MAXVALUE, maxDamage);
	condition->setParam(CONDITION_PARAM_STARTVALUE, startDamage);
	condition->setParam(CONDITION_PARAM_DELAYED, 1);
	return condition;
}

bool Monsters::deserializeSpell(MonsterSpell* spell, spellBlock_t& sb, const std::string& description)
{
	if (!spell->scriptName.empty()) {
		spell->isScripted = true;
	} else if (!spell->name.empty()) {
		spell->isScripted = false;
	} else {
		return false;
	}

	sb.speed = spell->interval;

	if (spell->chance > 100) {
		sb.chance = 100;
	} else {
		sb.chance = spell->chance;
	}

	if (spell->range > (Map::maxViewportX * 2)) {
		spell->range = Map::maxViewportX * 2;
	}
	sb.range = spell->range;

	sb.minCombatValue = spell->minCombatValue;
	sb.maxCombatValue = spell->maxCombatValue;
	if (std::abs(sb.minCombatValue) > std::abs(sb.maxCombatValue)) {
		int32_t value = sb.maxCombatValue;
		sb.maxCombatValue = sb.minCombatValue;
		sb.minCombatValue = value;
	}

	sb.spell = g_spells->getSpellByName(spell->name);
	if (sb.spell) {
		return true;
	}

	CombatSpell* combatSpell = nullptr;

	if (spell->isScripted) {
		std::unique_ptr<CombatSpell> combatSpellPtr(new CombatSpell(nullptr, spell->needTarget, spell->needDirection));
		if (!combatSpellPtr->loadScript("data/" + g_spells->getScriptBaseName() + "/scripts/" + spell->scriptName)) {
			std::cout << "cannot find file" << std::endl;
			return false;
		}

		if (!combatSpellPtr->loadScriptCombat()) {
			return false;
		}

		combatSpell = combatSpellPtr.release();
		combatSpell->getCombat()->setPlayerCombatValues(COMBAT_FORMULA_DAMAGE, sb.minCombatValue, 0, sb.maxCombatValue, 0);
	} else {
		Combat* combat = new Combat;
		sb.combatSpell = true;

		if (spell->length > 0) {
			//need direction spell
			//if (spell->spread > 0) {
				spell->spread = std::max<int32_t>(0, spell->spread);
			//} else {
				//spell->spread = 3;
			//}

			AreaCombat* area = new AreaCombat();
			area->setupArea(spell->length, spell->spread);
			combat->setArea(area);

			spell->needDirection = true;
		}

		if (spell->radius > 0) {
			AreaCombat* area = new AreaCombat();
			area->setupArea(spell->radius);
			combat->setArea(area);
		}

		std::string tmpName = asLowerCaseString(spell->name);

		if (tmpName == "melee") {
			sb.isMelee = true;

			if (spell->attack > 0 && spell->skill > 0) {
				sb.minCombatValue = 0;
				sb.maxCombatValue = -Weapons::getMaxMeleeDamage(spell->skill, spell->attack);
			}

			ConditionType_t conditionType = CONDITION_NONE;
			int32_t minDamage = 0;
			int32_t maxDamage = 0;
			uint32_t tickInterval = 2000;

			if (spell->conditionType != CONDITION_NONE) {
				conditionType = spell->conditionType;

				minDamage = spell->conditionMinDamage;
				maxDamage = minDamage;
				if (spell->tickInterval != 0) {
					tickInterval = spell->tickInterval;
				}

				Condition* condition = getDamageCondition(conditionType, maxDamage, minDamage, spell->conditionStartDamage, tickInterval);
				combat->setCondition(condition);
			}

			sb.range = 1;
			combat->setParam(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE);
			combat->setParam(COMBAT_PARAM_BLOCKARMOR, 1);
			combat->setParam(COMBAT_PARAM_BLOCKSHIELD, 1);
			combat->setOrigin(ORIGIN_MELEE);
		} else if (tmpName == "combat") {
			if (spell->combatType == COMBAT_PHYSICALDAMAGE) {
				combat->setParam(COMBAT_PARAM_BLOCKARMOR, 1);
				combat->setOrigin(ORIGIN_RANGED);
			} else if (spell->combatType == COMBAT_HEALING) {
				combat->setParam(COMBAT_PARAM_AGGRESSIVE, 0);
			}
			combat->setParam(COMBAT_PARAM_TYPE, spell->combatType);
		} else if (tmpName == "speed") {
			int32_t speedChange = 0;
			int32_t duration = 10000;

			if (spell->duration != 0) {
				duration = spell->duration;
			}

			if (spell->speedChange != 0) {
				speedChange = spell->speedChange;
				if (speedChange < -1000) {
					//cant be slower than 100%
					speedChange = -1000;
				}
			}

			ConditionType_t conditionType;
			if (speedChange > 0) {
				conditionType = CONDITION_HASTE;
				combat->setParam(COMBAT_PARAM_AGGRESSIVE, 0);
			} else {
				conditionType = CONDITION_PARALYZE;
			}

			ConditionSpeed* condition = reinterpret_cast<ConditionSpeed*>(Condition::createCondition(CONDITIONID_COMBAT, conditionType, duration, 0));
			condition->setFormulaVars(speedChange / 1000.0, 0, speedChange / 1000.0, 0);
			combat->setCondition(condition);
		} else if (tmpName == "outfit") {
			int32_t duration = 10000;

			if (spell->duration != 0) {
				duration = spell->duration;
			}

			ConditionOutfit* condition = reinterpret_cast<ConditionOutfit*>(Condition::createCondition(CONDITIONID_COMBAT, CONDITION_OUTFIT, duration, 0));
			condition->setOutfit(spell->outfit);
			combat->setParam(COMBAT_PARAM_AGGRESSIVE, 0);
			combat->setCondition(condition);
		} else if (tmpName == "invisible") {
			int32_t duration = 10000;

			if (spell->duration != 0) {
				duration = spell->duration;
			}

			Condition* condition = Condition::createCondition(CONDITIONID_COMBAT, CONDITION_INVISIBLE, duration, 0);
			combat->setParam(COMBAT_PARAM_AGGRESSIVE, 0);
			combat->setCondition(condition);
		} else if (tmpName == "drunk") {
			int32_t duration = 10000;

			if (spell->duration != 0) {
				duration = spell->duration;
			}

			Condition* condition = Condition::createCondition(CONDITIONID_COMBAT, CONDITION_DRUNK, duration, 0);
			combat->setCondition(condition);
		} else if (tmpName == "firefield") {
			combat->setParam(COMBAT_PARAM_CREATEITEM, ITEM_FIREFIELD_PVP_FULL);
		} else if (tmpName == "poisonfield") {
			combat->setParam(COMBAT_PARAM_CREATEITEM, ITEM_POISONFIELD_PVP);
		} else if (tmpName == "energyfield") {
			combat->setParam(COMBAT_PARAM_CREATEITEM, ITEM_ENERGYFIELD_PVP);
		} else if (tmpName == "condition") {
			uint32_t tickInterval = 2000;

			if (spell->conditionType == CONDITION_NONE) {
				std::cout << "[Error - Monsters::deserializeSpell] - " << description << " - Condition is not set for: " << spell->name << std::endl;
				return false;
			}

			if (spell->tickInterval != 0) {
				int32_t value = spell->tickInterval;
				if (value > 0) {
					tickInterval = value;
				}
			}

			int32_t minDamage = std::abs(spell->conditionMinDamage);
			int32_t maxDamage = std::abs(spell->conditionMaxDamage);
			int32_t startDamage = 0;

			if (spell->conditionStartDamage != 0) {
				int32_t value = std::abs(spell->conditionStartDamage);
				if (value <= minDamage) {
					startDamage = value;
				}
			}

			Condition* condition = getDamageCondition(spell->conditionType, maxDamage, minDamage, startDamage, tickInterval);
			combat->setCondition(condition);
		} else {
			std::cout << "[Error - Monsters::deserializeSpell] - " << description << " - Unknown spell name: " << spell->name << std::endl;
			delete combat;
			return false;
		}

		combat->setPlayerCombatValues(COMBAT_FORMULA_DAMAGE, sb.minCombatValue, 0, sb.maxCombatValue, 0);
		combatSpell = new CombatSpell(combat, spell->needTarget, spell->needDirection);

		if (spell->needTarget) {
			if (spell->shoot != CONST_ANI_NONE) {
				combat->setParam(COMBAT_PARAM_DISTANCEEFFECT, spell->shoot);
			} else {
				std::cout << "[Warning - Monsters::deserializeSpells] Shoot Effect type \"" << spell->shoot << "\" does not exist." << std::endl;
			}
		}

		if (spell->effect != CONST_ME_NONE) {
			combat->setParam(COMBAT_PARAM_EFFECT, spell->effect);
		} else {
			std::cout << "[Warning - Monsters::deserializeSpells] Effect type \"" << spell->effect << "\" does not exist." << std::endl;
		}
	}

	sb.spell = combatSpell;
	return true;
}

bool Monsters::deserializeSpell(const pugi::xml_node& node, spellBlock_t& sb, const std::string& description)
{
	sb.chance = 100;
	sb.speed = 2000;
	sb.range = 0;
	sb.minCombatValue = 0;
	sb.maxCombatValue = 0;
	sb.combatSpell = false;
	sb.isMelee = false;

	std::string name;
	std::string scriptName;
	bool isScripted;

	pugi::xml_attribute attr;
	if ((attr = node.attribute("script"))) {
		scriptName = attr.as_string();
		isScripted = true;
	} else if ((attr = node.attribute("name"))) {
		name = attr.as_string();
		isScripted = false;
	} else {
		return false;
	}

	if ((attr = node.attribute("speed")) || (attr = node.attribute("interval"))) {
		sb.speed = std::max<int32_t>(1, pugi::cast<int32_t>(attr.value()));
	}

	if ((attr = node.attribute("chance"))) {
		uint32_t chance = pugi::cast<uint32_t>(attr.value());
		if (chance > 100) {
			chance = 100;
		}
		sb.chance = chance;
	}

	if ((attr = node.attribute("range"))) {
		uint32_t range = pugi::cast<uint32_t>(attr.value());
		if (range > (Map::maxViewportX * 2)) {
			range = Map::maxViewportX * 2;
		}
		sb.range = range;
	}

	if ((attr = node.attribute("min"))) {
		sb.minCombatValue = pugi::cast<int32_t>(attr.value());
	}

	if ((attr = node.attribute("max"))) {
		sb.maxCombatValue = pugi::cast<int32_t>(attr.value());

		//normalize values
		if (std::abs(sb.minCombatValue) > std::abs(sb.maxCombatValue)) {
			int32_t value = sb.maxCombatValue;
			sb.maxCombatValue = sb.minCombatValue;
			sb.minCombatValue = value;
		}
	}

	sb.spell = g_spells->getSpellByName(name);
	if (sb.spell) {
		return true;
	}

	CombatSpell* combatSpell = nullptr;
	bool needTarget = false;
	bool needDirection = false;

	if (isScripted) {
		if ((attr = node.attribute("direction"))) {
			needDirection = attr.as_bool();
		}

		if ((attr = node.attribute("target"))) {
			needTarget = attr.as_bool();
		}

		std::unique_ptr<CombatSpell> combatSpellPtr(new CombatSpell(nullptr, needTarget, needDirection));
		if (!combatSpellPtr->loadScript("data/" + g_spells->getScriptBaseName() + "/scripts/" + scriptName)) {
			return false;
		}

		if (!combatSpellPtr->loadScriptCombat()) {
			return false;
		}

		combatSpell = combatSpellPtr.release();
		combatSpell->getCombat()->setPlayerCombatValues(COMBAT_FORMULA_DAMAGE, sb.minCombatValue, 0, sb.maxCombatValue, 0);
	} else {
		Combat* combat = new Combat;
		sb.combatSpell = true;

		if ((attr = node.attribute("length"))) {
			int32_t length = pugi::cast<int32_t>(attr.value());
			if (length > 0) {
				int32_t spread = 3;

				//need direction spell
				if ((attr = node.attribute("spread"))) {
					spread = std::max<int32_t>(0, pugi::cast<int32_t>(attr.value()));
				}

				AreaCombat* area = new AreaCombat();
				area->setupArea(length, spread);
				combat->setArea(area);

				needDirection = true;
			}
		}

		if ((attr = node.attribute("radius"))) {
			int32_t radius = pugi::cast<int32_t>(attr.value());

			//target spell
			if ((attr = node.attribute("target"))) {
				needTarget = attr.as_bool();
			}

			AreaCombat* area = new AreaCombat();
			area->setupArea(radius);
			combat->setArea(area);
		}

		std::string tmpName = asLowerCaseString(name);

		if (tmpName == "melee") {
			sb.isMelee = true;

			pugi::xml_attribute attackAttribute, skillAttribute;
			if ((attackAttribute = node.attribute("attack")) && (skillAttribute = node.attribute("skill"))) {
				sb.minCombatValue = 0;
				sb.maxCombatValue = -Weapons::getMaxMeleeDamage(pugi::cast<int32_t>(skillAttribute.value()), pugi::cast<int32_t>(attackAttribute.value()));
			}

			ConditionType_t conditionType = CONDITION_NONE;
			int32_t minDamage = 0;
			int32_t maxDamage = 0;
			uint32_t tickInterval = 2000;

			if ((attr = node.attribute("fire"))) {
				conditionType = CONDITION_FIRE;

				minDamage = pugi::cast<int32_t>(attr.value());
				maxDamage = minDamage;
				tickInterval = 9000;
			} else if ((attr = node.attribute("poison"))) {
				conditionType = CONDITION_POISON;

				minDamage = pugi::cast<int32_t>(attr.value());
				maxDamage = minDamage;
				tickInterval = 5000;
			} else if ((attr = node.attribute("energy"))) {
				conditionType = CONDITION_ENERGY;

				minDamage = pugi::cast<int32_t>(attr.value());
				maxDamage = minDamage;
				tickInterval = 10000;
			} else if ((attr = node.attribute("drown"))) {
				conditionType = CONDITION_DROWN;

				minDamage = pugi::cast<int32_t>(attr.value());
				maxDamage = minDamage;
				tickInterval = 5000;
			} else if ((attr = node.attribute("freeze"))) {
				conditionType = CONDITION_FREEZING;

				minDamage = pugi::cast<int32_t>(attr.value());
				maxDamage = minDamage;
				tickInterval = 8000;
			} else if ((attr = node.attribute("dazzle"))) {
				conditionType = CONDITION_DAZZLED;

				minDamage = pugi::cast<int32_t>(attr.value());
				maxDamage = minDamage;
				tickInterval = 10000;
			} else if ((attr = node.attribute("curse"))) {
				conditionType = CONDITION_CURSED;

				minDamage = pugi::cast<int32_t>(attr.value());
				maxDamage = minDamage;
				tickInterval = 4000;
			} else if ((attr = node.attribute("bleed")) || (attr = node.attribute("physical"))) {
				conditionType = CONDITION_BLEEDING;
				tickInterval = 5000;
			}

			if ((attr = node.attribute("tick"))) {
				int32_t value = pugi::cast<int32_t>(attr.value());
				if (value > 0) {
					tickInterval = value;
				}
			}

			if (conditionType != CONDITION_NONE) {
				Condition* condition = getDamageCondition(conditionType, maxDamage, minDamage, 0, tickInterval);
				combat->setCondition(condition);
			}

			sb.range = 1;
			combat->setParam(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE);
			combat->setParam(COMBAT_PARAM_BLOCKARMOR, 1);
			combat->setParam(COMBAT_PARAM_BLOCKSHIELD, 1);
			combat->setOrigin(ORIGIN_MELEE);
		} else if (tmpName == "physical") {
			combat->setParam(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE);
			combat->setParam(COMBAT_PARAM_BLOCKARMOR, 1);
			combat->setOrigin(ORIGIN_RANGED);
		} else if (tmpName == "bleed") {
			combat->setParam(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE);
		} else if (tmpName == "poison" || tmpName == "earth") {
			combat->setParam(COMBAT_PARAM_TYPE, COMBAT_EARTHDAMAGE);
		} else if (tmpName == "fire") {
			combat->setParam(COMBAT_PARAM_TYPE, COMBAT_FIREDAMAGE);
		} else if (tmpName == "energy") {
			combat->setParam(COMBAT_PARAM_TYPE, COMBAT_ENERGYDAMAGE);
		} else if (tmpName == "drown") {
			combat->setParam(COMBAT_PARAM_TYPE, COMBAT_DROWNDAMAGE);
		} else if (tmpName == "ice") {
			combat->setParam(COMBAT_PARAM_TYPE, COMBAT_ICEDAMAGE);
		} else if (tmpName == "holy") {
			combat->setParam(COMBAT_PARAM_TYPE, COMBAT_HOLYDAMAGE);
		} else if (tmpName == "death") {
			combat->setParam(COMBAT_PARAM_TYPE, COMBAT_DEATHDAMAGE);
		} else if (tmpName == "lifedrain") {
			combat->setParam(COMBAT_PARAM_TYPE, COMBAT_LIFEDRAIN);
		} else if (tmpName == "manadrain") {
			combat->setParam(COMBAT_PARAM_TYPE, COMBAT_MANADRAIN);
		} else if (tmpName == "healing") {
			combat->setParam(COMBAT_PARAM_TYPE, COMBAT_HEALING);
			combat->setParam(COMBAT_PARAM_AGGRESSIVE, 0);
		} else if (tmpName == "speed") {
			int32_t speedChange = 0;
			int32_t duration = 10000;

			if ((attr = node.attribute("duration"))) {
				duration = pugi::cast<int32_t>(attr.value());
			}

			if ((attr = node.attribute("speedchange"))) {
				speedChange = pugi::cast<int32_t>(attr.value());
				if (speedChange < -1000) {
					//cant be slower than 100%
					speedChange = -1000;
				}
			}

			ConditionType_t conditionType;
			if (speedChange > 0) {
				conditionType = CONDITION_HASTE;
				combat->setParam(COMBAT_PARAM_AGGRESSIVE, 0);
			} else {
				conditionType = CONDITION_PARALYZE;
			}

			ConditionSpeed* condition = reinterpret_cast<ConditionSpeed*>(Condition::createCondition(CONDITIONID_COMBAT, conditionType, duration, 0));
			condition->setFormulaVars(speedChange / 1000.0, 0, speedChange / 1000.0, 0);
			combat->setCondition(condition);
		} else if (tmpName == "outfit") {
			int32_t duration = 10000;

			if ((attr = node.attribute("duration"))) {
				duration = pugi::cast<int32_t>(attr.value());
			}

			if ((attr = node.attribute("monster"))) {
				MonsterType* mType = g_monsters.getMonsterType(attr.as_string());
				if (mType) {
					ConditionOutfit* condition = reinterpret_cast<ConditionOutfit*>(Condition::createCondition(CONDITIONID_COMBAT, CONDITION_OUTFIT, duration, 0));
					condition->setOutfit(mType->outfit);
					combat->setParam(COMBAT_PARAM_AGGRESSIVE, 0);
					combat->setCondition(condition);
				}
			} else if ((attr = node.attribute("item"))) {
				Outfit_t outfit;
				outfit.lookTypeEx = pugi::cast<uint16_t>(attr.value());

				ConditionOutfit* condition = reinterpret_cast<ConditionOutfit*>(Condition::createCondition(CONDITIONID_COMBAT, CONDITION_OUTFIT, duration, 0));
				condition->setOutfit(outfit);
				combat->setParam(COMBAT_PARAM_AGGRESSIVE, 0);
				combat->setCondition(condition);
			}
		} else if (tmpName == "invisible") {
			int32_t duration = 10000;

			if ((attr = node.attribute("duration"))) {
				duration = pugi::cast<int32_t>(attr.value());
			}

			Condition* condition = Condition::createCondition(CONDITIONID_COMBAT, CONDITION_INVISIBLE, duration, 0);
			combat->setParam(COMBAT_PARAM_AGGRESSIVE, 0);
			combat->setCondition(condition);
		} else if (tmpName == "drunk") {
			int32_t duration = 10000;

			if ((attr = node.attribute("duration"))) {
				duration = pugi::cast<int32_t>(attr.value());
			}

			Condition* condition = Condition::createCondition(CONDITIONID_COMBAT, CONDITION_DRUNK, duration, 0);
			combat->setCondition(condition);
		} else if (tmpName == "firefield") {
			combat->setParam(COMBAT_PARAM_CREATEITEM, ITEM_FIREFIELD_PVP_FULL);
		} else if (tmpName == "poisonfield") {
			combat->setParam(COMBAT_PARAM_CREATEITEM, ITEM_POISONFIELD_PVP);
		} else if (tmpName == "energyfield") {
			combat->setParam(COMBAT_PARAM_CREATEITEM, ITEM_ENERGYFIELD_PVP);
		} else if (tmpName == "firecondition" || tmpName == "energycondition" ||
		           tmpName == "earthcondition" || tmpName == "poisoncondition" ||
		           tmpName == "icecondition" || tmpName == "freezecondition" ||
		           tmpName == "deathcondition" || tmpName == "cursecondition" ||
		           tmpName == "holycondition" || tmpName == "dazzlecondition" ||
		           tmpName == "drowncondition" || tmpName == "bleedcondition" ||
		           tmpName == "physicalcondition") {
			ConditionType_t conditionType = CONDITION_NONE;
			uint32_t tickInterval = 2000;

			if (tmpName == "firecondition") {
				conditionType = CONDITION_FIRE;
				tickInterval = 10000;
			} else if (tmpName == "poisoncondition" || tmpName == "earthcondition") {
				conditionType = CONDITION_POISON;
				tickInterval = 5000;
			} else if (tmpName == "energycondition") {
				conditionType = CONDITION_ENERGY;
				tickInterval = 10000;
			} else if (tmpName == "drowncondition") {
				conditionType = CONDITION_DROWN;
				tickInterval = 5000;
			} else if (tmpName == "freezecondition" || tmpName == "icecondition") {
				conditionType = CONDITION_FREEZING;
				tickInterval = 10000;
			} else if (tmpName == "cursecondition" || tmpName == "deathcondition") {
				conditionType = CONDITION_CURSED;
				tickInterval = 4000;
			} else if (tmpName == "dazzlecondition" || tmpName == "holycondition") {
				conditionType = CONDITION_DAZZLED;
				tickInterval = 10000;
			} else if (tmpName == "physicalcondition" || tmpName == "bleedcondition") {
				conditionType = CONDITION_BLEEDING;
				tickInterval = 5000;
			}

			if ((attr = node.attribute("tick"))) {
				int32_t value = pugi::cast<int32_t>(attr.value());
				if (value > 0) {
					tickInterval = value;
				}
			}

			int32_t minDamage = std::abs(sb.minCombatValue);
			int32_t maxDamage = std::abs(sb.maxCombatValue);
			int32_t startDamage = 0;

			if ((attr = node.attribute("start"))) {
				int32_t value = std::abs(pugi::cast<int32_t>(attr.value()));
				if (value <= minDamage) {
					startDamage = value;
				}
			}

			Condition* condition = getDamageCondition(conditionType, maxDamage, minDamage, startDamage, tickInterval);
			combat->setCondition(condition);
		} else if (tmpName == "strength") {
			//
		} else if (tmpName == "effect") {
			//
		} else {
			std::cout << "[Error - Monsters::deserializeSpell] - " << description << " - Unknown spell name: " << name << std::endl;
			delete combat;
			return false;
		}

		combat->setPlayerCombatValues(COMBAT_FORMULA_DAMAGE, sb.minCombatValue, 0, sb.maxCombatValue, 0);
		combatSpell = new CombatSpell(combat, needTarget, needDirection);

		for (pugi::xml_node attributeNode = node.first_child(); attributeNode; attributeNode = attributeNode.next_sibling()) {
			if ((attr = attributeNode.attribute("key"))) {
				const char* value = attr.value();
				if (strcasecmp(value, "shooteffect") == 0) {
					if ((attr = attributeNode.attribute("value"))) {
						ShootType_t shoot = getShootType(attr.as_string());
						if (shoot != CONST_ANI_NONE) {
							combat->setParam(COMBAT_PARAM_DISTANCEEFFECT, shoot);
						} else {
							std::cout << "[Warning - Monsters::deserializeSpell] " << description << " - Unknown shootEffect: " << attr.as_string() << std::endl;
						}
					}
				} else if (strcasecmp(value, "areaeffect") == 0) {
					if ((attr = attributeNode.attribute("value"))) {
						MagicEffectClasses effect = getMagicEffect(attr.as_string());
						if (effect != CONST_ME_NONE) {
							combat->setParam(COMBAT_PARAM_EFFECT, effect);
						} else {
							std::cout << "[Warning - Monsters::deserializeSpell] " << description << " - Unknown areaEffect: " << attr.as_string() << std::endl;
						}
					}
				} else {
					std::cout << "[Warning - Monsters::deserializeSpells] Effect type \"" << attr.as_string() << "\" does not exist." << std::endl;
				}
			}
		}
	}

	sb.spell = combatSpell;
	return true;
}

bool Monsters::loadMonster(const std::string& file, const std::string& monster_name, bool reloading /*= false*/)
{
	MonsterType* mType = nullptr;
	bool new_mType = true;

	if (reloading) {
		uint32_t id = getIdByName(monster_name);
		if (id != 0) {
			mType = getMonsterType(id);
			if (mType != nullptr) {
				new_mType = false;
				mType->reset();
			}
		}
	}

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(file.c_str());
	if (!result) {
		printXMLError("Error - Monsters::loadMonster", file, result);
		return false;
	}

	pugi::xml_node monsterNode = doc.child("monster");
	if (!monsterNode) {
		std::cout << "[Error - Monsters::loadMonster] Missing monster node in: " << file << std::endl;
		return false;
	}

	pugi::xml_attribute attr;
	if (!(attr = monsterNode.attribute("name"))) {
		std::cout << "[Error - Monsters::loadMonster] Missing name in: " << file << std::endl;
		return false;
	}

	if (mType == nullptr) {
		mType = new MonsterType();
	}
	mType->name = attr.as_string();

	if ((attr = monsterNode.attribute("nameDescription"))) {
		mType->nameDescription = attr.as_string();
	} else {
		mType->nameDescription = "a " + mType->name;
		toLowerCaseString(mType->nameDescription);
	}

	if ((attr = monsterNode.attribute("race"))) {
		std::string tmpStrValue = asLowerCaseString(attr.as_string());
		uint16_t tmpInt = pugi::cast<uint16_t>(attr.value());
		if (tmpStrValue == "venom" || tmpInt == 1) {
			mType->race = RACE_VENOM;
		} else if (tmpStrValue == "blood" || tmpInt == 2) {
			mType->race = RACE_BLOOD;
		} else if (tmpStrValue == "undead" || tmpInt == 3) {
			mType->race = RACE_UNDEAD;
		} else if (tmpStrValue == "fire" || tmpInt == 4) {
			mType->race = RACE_FIRE;
		} else if (tmpStrValue == "energy" || tmpInt == 5) {
			mType->race = RACE_ENERGY;
		} else {
			std::cout << "[Warning - Monsters::loadMonster] Unknown race type " << attr.as_string() << ". " << file << std::endl;
		}
	}

	if ((attr = monsterNode.attribute("experience"))) {
		mType->experience = pugi::cast<uint64_t>(attr.value());
	}

	if ((attr = monsterNode.attribute("speed"))) {
		mType->baseSpeed = pugi::cast<int32_t>(attr.value());
	}

	if ((attr = monsterNode.attribute("manacost"))) {
		mType->manaCost = pugi::cast<uint32_t>(attr.value());
	}

	if ((attr = monsterNode.attribute("skull"))) {
		mType->skull = getSkullType(attr.as_string());
	}

	if ((attr = monsterNode.attribute("script"))) {
		monsterScriptList[mType] = attr.as_string();
	}

	pugi::xml_node node;
	if ((node = monsterNode.child("health"))) {
		if ((attr = node.attribute("now"))) {
			mType->health = pugi::cast<int32_t>(attr.value());
		} else {
			std::cout << "[Error - Monsters::loadMonster] Missing health now. " << file << std::endl;
		}

		if ((attr = node.attribute("max"))) {
			mType->healthMax = pugi::cast<int32_t>(attr.value());
		} else {
			std::cout << "[Error - Monsters::loadMonster] Missing health max. " << file << std::endl;
		}
	}

	if ((node = monsterNode.child("flags"))) {
		for (pugi::xml_node flagNode = node.first_child(); flagNode; flagNode = flagNode.next_sibling()) {
			attr = flagNode.first_attribute();
			const char* attrName = attr.name();
			if (strcasecmp(attrName, "summonable") == 0) {
				mType->isSummonable = attr.as_bool();
			} else if (strcasecmp(attrName, "attackable") == 0) {
				mType->isAttackable = attr.as_bool();
			} else if (strcasecmp(attrName, "hostile") == 0) {
				mType->isHostile = attr.as_bool();
			} else if (strcasecmp(attrName, "illusionable") == 0) {
				mType->isIllusionable = attr.as_bool();
			} else if (strcasecmp(attrName, "convinceable") == 0) {
				mType->isConvinceable = attr.as_bool();
			} else if (strcasecmp(attrName, "pushable") == 0) {
				mType->pushable = attr.as_bool();
			} else if (strcasecmp(attrName, "canpushitems") == 0) {
				mType->canPushItems = attr.as_bool();
			} else if (strcasecmp(attrName, "canpushcreatures") == 0) {
				mType->canPushCreatures = attr.as_bool();
			} else if (strcasecmp(attrName, "staticattack") == 0) {
				uint32_t staticAttack = pugi::cast<uint32_t>(attr.value());
				if (staticAttack > 100) {
					std::cout << "[Warning - Monsters::loadMonster] staticattack greater than 100. " << file << std::endl;
					staticAttack = 100;
				}

				mType->staticAttackChance = staticAttack;
			} else if (strcasecmp(attrName, "lightlevel") == 0) {
				mType->lightLevel = pugi::cast<uint16_t>(attr.value());
			} else if (strcasecmp(attrName, "lightcolor") == 0) {
				mType->lightColor = pugi::cast<uint16_t>(attr.value());
			} else if (strcasecmp(attrName, "targetdistance") == 0) {
				mType->targetDistance = std::max<int32_t>(1, pugi::cast<int32_t>(attr.value()));
			} else if (strcasecmp(attrName, "runonhealth") == 0) {
				mType->runAwayHealth = pugi::cast<int32_t>(attr.value());
			} else if (strcasecmp(attrName, "hidehealth") == 0) {
				mType->hiddenHealth = attr.as_bool();
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Unknown flag attribute: " << attrName << ". " << file << std::endl;
			}
		}

		//if a monster can push creatures,
		// it should not be pushable
		if (mType->canPushCreatures && mType->pushable) {
			mType->pushable = false;
		}
	}

	if ((node = monsterNode.child("targetchange"))) {
		if ((attr = node.attribute("speed")) || (attr = node.attribute("interval"))) {
			mType->changeTargetSpeed = pugi::cast<uint32_t>(attr.value());
		} else {
			std::cout << "[Warning - Monsters::loadMonster] Missing targetchange speed. " << file << std::endl;
		}

		if ((attr = node.attribute("chance"))) {
			mType->changeTargetChance = pugi::cast<int32_t>(attr.value());
		} else {
			std::cout << "[Warning - Monsters::loadMonster] Missing targetchange chance. " << file << std::endl;
		}
	}

	if ((node = monsterNode.child("look"))) {
		if ((attr = node.attribute("type"))) {
			mType->outfit.lookType = pugi::cast<uint16_t>(attr.value());

			if ((attr = node.attribute("head"))) {
				mType->outfit.lookHead = pugi::cast<uint16_t>(attr.value());
			}

			if ((attr = node.attribute("body"))) {
				mType->outfit.lookBody = pugi::cast<uint16_t>(attr.value());
			}

			if ((attr = node.attribute("legs"))) {
				mType->outfit.lookLegs = pugi::cast<uint16_t>(attr.value());
			}

			if ((attr = node.attribute("feet"))) {
				mType->outfit.lookFeet = pugi::cast<uint16_t>(attr.value());
			}

			if ((attr = node.attribute("addons"))) {
				mType->outfit.lookAddons = pugi::cast<uint16_t>(attr.value());
			}
		} else if ((attr = node.attribute("typeex"))) {
			mType->outfit.lookTypeEx = pugi::cast<uint16_t>(attr.value());
		} else {
			std::cout << "[Warning - Monsters::loadMonster] Missing look type/typeex. " << file << std::endl;
		}

		if ((attr = node.attribute("mount"))) {
			mType->outfit.lookMount = pugi::cast<uint16_t>(attr.value());
		}

		if ((attr = node.attribute("corpse"))) {
			mType->lookcorpse = pugi::cast<uint16_t>(attr.value());
		}
	}

	if ((node = monsterNode.child("attacks"))) {
		for (pugi::xml_node attackNode = node.first_child(); attackNode; attackNode = attackNode.next_sibling()) {
			spellBlock_t sb;
			if (deserializeSpell(attackNode, sb, monster_name)) {
				mType->spellAttackList.push_back(sb);
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Cant load spell. " << file << std::endl;
			}
		}
	}

	if ((node = monsterNode.child("defenses"))) {
		if ((attr = node.attribute("defense"))) {
			mType->defense = pugi::cast<int32_t>(attr.value());
		}

		if ((attr = node.attribute("armor"))) {
			mType->armor = pugi::cast<int32_t>(attr.value());
		}

		for (pugi::xml_node defenseNode = node.first_child(); defenseNode; defenseNode = defenseNode.next_sibling()) {
			spellBlock_t sb;
			if (deserializeSpell(defenseNode, sb, monster_name)) {
				mType->spellDefenseList.push_back(sb);
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Cant load spell. " << file << std::endl;
			}
		}
	}

	if ((node = monsterNode.child("immunities"))) {
		for (pugi::xml_node immunityNode = node.first_child(); immunityNode; immunityNode = immunityNode.next_sibling()) {
			if ((attr = immunityNode.attribute("name"))) {
				std::string tmpStrValue = asLowerCaseString(attr.as_string());
				if (tmpStrValue == "physical") {
					mType->damageImmunities |= COMBAT_PHYSICALDAMAGE;
					mType->conditionImmunities |= CONDITION_BLEEDING;
				} else if (tmpStrValue == "energy") {
					mType->damageImmunities |= COMBAT_ENERGYDAMAGE;
					mType->conditionImmunities |= CONDITION_ENERGY;
				} else if (tmpStrValue == "fire") {
					mType->damageImmunities |= COMBAT_FIREDAMAGE;
					mType->conditionImmunities |= CONDITION_FIRE;
				} else if (tmpStrValue == "poison" ||
							tmpStrValue == "earth") {
					mType->damageImmunities |= COMBAT_EARTHDAMAGE;
					mType->conditionImmunities |= CONDITION_POISON;
				} else if (tmpStrValue == "drown") {
					mType->damageImmunities |= COMBAT_DROWNDAMAGE;
					mType->conditionImmunities |= CONDITION_DROWN;
				} else if (tmpStrValue == "ice") {
					mType->damageImmunities |= COMBAT_ICEDAMAGE;
					mType->conditionImmunities |= CONDITION_FREEZING;
				} else if (tmpStrValue == "holy") {
					mType->damageImmunities |= COMBAT_HOLYDAMAGE;
					mType->conditionImmunities |= CONDITION_DAZZLED;
				} else if (tmpStrValue == "death") {
					mType->damageImmunities |= COMBAT_DEATHDAMAGE;
					mType->conditionImmunities |= CONDITION_CURSED;
				} else if (tmpStrValue == "lifedrain") {
					mType->damageImmunities |= COMBAT_LIFEDRAIN;
				} else if (tmpStrValue == "manadrain") {
					mType->damageImmunities |= COMBAT_MANADRAIN;
				} else if (tmpStrValue == "paralyze") {
					mType->conditionImmunities |= CONDITION_PARALYZE;
				} else if (tmpStrValue == "outfit") {
					mType->conditionImmunities |= CONDITION_OUTFIT;
				} else if (tmpStrValue == "drunk") {
					mType->conditionImmunities |= CONDITION_DRUNK;
				} else if (tmpStrValue == "invisible" || tmpStrValue == "invisibility") {
					mType->conditionImmunities |= CONDITION_INVISIBLE;
				} else if (tmpStrValue == "bleed") {
					mType->conditionImmunities |= CONDITION_BLEEDING;
				} else {
					std::cout << "[Warning - Monsters::loadMonster] Unknown immunity name " << attr.as_string() << ". " << file << std::endl;
				}
			} else if ((attr = immunityNode.attribute("physical"))) {
				if (attr.as_bool()) {
					mType->damageImmunities |= COMBAT_PHYSICALDAMAGE;
					mType->conditionImmunities |= CONDITION_BLEEDING;
				}
			} else if ((attr = immunityNode.attribute("energy"))) {
				if (attr.as_bool()) {
					mType->damageImmunities |= COMBAT_ENERGYDAMAGE;
					mType->conditionImmunities |= CONDITION_ENERGY;
				}
			} else if ((attr = immunityNode.attribute("fire"))) {
				if (attr.as_bool()) {
					mType->damageImmunities |= COMBAT_FIREDAMAGE;
					mType->conditionImmunities |= CONDITION_FIRE;
				}
			} else if ((attr = immunityNode.attribute("poison")) || (attr = immunityNode.attribute("earth"))) {
				if (attr.as_bool()) {
					mType->damageImmunities |= COMBAT_EARTHDAMAGE;
					mType->conditionImmunities |= CONDITION_POISON;
				}
			} else if ((attr = immunityNode.attribute("drown"))) {
				if (attr.as_bool()) {
					mType->damageImmunities |= COMBAT_DROWNDAMAGE;
					mType->conditionImmunities |= CONDITION_DROWN;
				}
			} else if ((attr = immunityNode.attribute("ice"))) {
				if (attr.as_bool()) {
					mType->damageImmunities |= COMBAT_ICEDAMAGE;
					mType->conditionImmunities |= CONDITION_FREEZING;
				}
			} else if ((attr = immunityNode.attribute("holy"))) {
				if (attr.as_bool()) {
					mType->damageImmunities |= COMBAT_HOLYDAMAGE;
					mType->conditionImmunities |= CONDITION_DAZZLED;
				}
			} else if ((attr = immunityNode.attribute("death"))) {
				if (attr.as_bool()) {
					mType->damageImmunities |= COMBAT_DEATHDAMAGE;
					mType->conditionImmunities |= CONDITION_CURSED;
				}
			} else if ((attr = immunityNode.attribute("lifedrain"))) {
				if (attr.as_bool()) {
					mType->damageImmunities |= COMBAT_LIFEDRAIN;
				}
			} else if ((attr = immunityNode.attribute("manadrain"))) {
				if (attr.as_bool()) {
					mType->damageImmunities |= COMBAT_MANADRAIN;
				}
			} else if ((attr = immunityNode.attribute("paralyze"))) {
				if (attr.as_bool()) {
					mType->conditionImmunities |= CONDITION_PARALYZE;
				}
			} else if ((attr = immunityNode.attribute("outfit"))) {
				if (attr.as_bool()) {
					mType->conditionImmunities |= CONDITION_OUTFIT;
				}
			} else if ((attr = immunityNode.attribute("bleed"))) {
				if (attr.as_bool()) {
					mType->conditionImmunities |= CONDITION_BLEEDING;
				}
			} else if ((attr = immunityNode.attribute("drunk"))) {
				if (attr.as_bool()) {
					mType->conditionImmunities |= CONDITION_DRUNK;
				}
			} else if ((attr = immunityNode.attribute("invisible")) || (attr = immunityNode.attribute("invisibility"))) {
				if (attr.as_bool()) {
					mType->conditionImmunities |= CONDITION_INVISIBLE;
				}
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Unknown immunity. " << file << std::endl;
			}
		}
	}

	if ((node = monsterNode.child("voices"))) {
		if ((attr = node.attribute("speed")) || (attr = node.attribute("interval"))) {
			mType->yellSpeedTicks = pugi::cast<uint32_t>(attr.value());
		} else {
			std::cout << "[Warning - Monsters::loadMonster] Missing voices speed. " << file << std::endl;
		}

		if ((attr = node.attribute("chance"))) {
			mType->yellChance = pugi::cast<uint32_t>(attr.value());
		} else {
			std::cout << "[Warning - Monsters::loadMonster] Missing voices chance. " << file << std::endl;
		}

		for (pugi::xml_node voiceNode = node.first_child(); voiceNode; voiceNode = voiceNode.next_sibling()) {
			voiceBlock_t vb;
			if ((attr = voiceNode.attribute("sentence"))) {
				vb.text = attr.as_string();
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Missing voice sentence. " << file << std::endl;
			}

			if ((attr = voiceNode.attribute("yell"))) {
				vb.yellText = attr.as_bool();
			} else {
				vb.yellText = false;
			}
			mType->voiceVector.push_back(vb);
		}
	}

	if ((node = monsterNode.child("loot"))) {
		for (pugi::xml_node lootNode = node.first_child(); lootNode; lootNode = lootNode.next_sibling()) {
			LootBlock lootBlock;
			if (loadLootItem(lootNode, lootBlock)) {
				mType->lootItems.push_back(lootBlock);
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Cant load loot. " << file << std::endl;
			}
		}
	}

	if ((node = monsterNode.child("elements"))) {
		for (pugi::xml_node elementNode = node.first_child(); elementNode; elementNode = elementNode.next_sibling()) {
			if ((attr = elementNode.attribute("physicalPercent"))) {
				mType->elementMap[COMBAT_PHYSICALDAMAGE] = pugi::cast<int32_t>(attr.value());
			} else if ((attr = elementNode.attribute("icePercent"))) {
				mType->elementMap[COMBAT_ICEDAMAGE] = pugi::cast<int32_t>(attr.value());
			} else if ((attr = elementNode.attribute("poisonPercent")) || (attr = elementNode.attribute("earthPercent"))) {
				mType->elementMap[COMBAT_EARTHDAMAGE] = pugi::cast<int32_t>(attr.value());
			} else if ((attr = elementNode.attribute("firePercent"))) {
				mType->elementMap[COMBAT_FIREDAMAGE] = pugi::cast<int32_t>(attr.value());
			} else if ((attr = elementNode.attribute("energyPercent"))) {
				mType->elementMap[COMBAT_ENERGYDAMAGE] = pugi::cast<int32_t>(attr.value());
			} else if ((attr = elementNode.attribute("holyPercent"))) {
				mType->elementMap[COMBAT_HOLYDAMAGE] = pugi::cast<int32_t>(attr.value());
			} else if ((attr = elementNode.attribute("deathPercent"))) {
				mType->elementMap[COMBAT_DEATHDAMAGE] = pugi::cast<int32_t>(attr.value());
			} else if ((attr = elementNode.attribute("drownPercent"))) {
				mType->elementMap[COMBAT_DROWNDAMAGE] = pugi::cast<int32_t>(attr.value());
			} else if ((attr = elementNode.attribute("lifedrainPercent"))) {
				mType->elementMap[COMBAT_LIFEDRAIN] = pugi::cast<int32_t>(attr.value());
			} else if ((attr = elementNode.attribute("manadrainPercent"))) {
				mType->elementMap[COMBAT_MANADRAIN] = pugi::cast<int32_t>(attr.value());
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Unknown element percent. " << file << std::endl;
			}
		}
	}

	if ((node = monsterNode.child("summons"))) {
		if ((attr = node.attribute("maxSummons"))) {
			mType->maxSummons = std::min<uint32_t>(pugi::cast<uint32_t>(attr.value()), 100);
		} else {
			std::cout << "[Warning - Monsters::loadMonster] Missing summons maxSummons. " << file << std::endl;
		}

		for (pugi::xml_node summonNode = node.first_child(); summonNode; summonNode = summonNode.next_sibling()) {
			int32_t chance = 100;
			int32_t speed = 1000;

			if ((attr = summonNode.attribute("speed")) || (attr = summonNode.attribute("interval"))) {
				speed = pugi::cast<int32_t>(attr.value());
			}

			if ((attr = summonNode.attribute("chance"))) {
				chance = pugi::cast<int32_t>(attr.value());
			}

			if ((attr = summonNode.attribute("name"))) {
				summonBlock_t sb;
				sb.name = attr.as_string();
				sb.speed = speed;
				sb.chance = chance;
				mType->summonList.push_back(sb);
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Missing summon name. " << file << std::endl;
			}
		}
	}

	if ((node = monsterNode.child("script"))) {
		for (pugi::xml_node eventNode = node.first_child(); eventNode; eventNode = eventNode.next_sibling()) {
			if ((attr = eventNode.attribute("name"))) {
				mType->scriptList.push_back(attr.as_string());
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Missing name for script event. " << file << std::endl;
			}
		}
	}

	if (new_mType) {
		addMonsterType(monster_name, mType);
	}
	return true;
}

bool Monsters::loadLootItem(const pugi::xml_node& node, LootBlock& lootBlock)
{
	pugi::xml_attribute attr;
	if ((attr = node.attribute("id"))) {
		lootBlock.id = pugi::cast<int32_t>(attr.value());
	}

	if (lootBlock.id == 0) {
		return false;
	}

	if ((attr = node.attribute("countmax"))) {
		lootBlock.countmax = std::max<int32_t>(1, pugi::cast<int32_t>(attr.value()));
	} else {
		lootBlock.countmax = 1;
	}

	if ((attr = node.attribute("chance")) || (attr = node.attribute("chance1"))) {
		lootBlock.chance = std::min<int32_t>(MAX_LOOTCHANCE, pugi::cast<int32_t>(attr.value()));
	} else {
		lootBlock.chance = MAX_LOOTCHANCE;
	}

	if (Item::items[lootBlock.id].isContainer()) {
		loadLootContainer(node, lootBlock);
	}

	//optional
	if ((attr = node.attribute("subtype"))) {
		lootBlock.subType = pugi::cast<int32_t>(attr.value());
	} else {
		uint32_t charges = Item::items[lootBlock.id].charges;
		if (charges != 0) {
			lootBlock.subType = charges;
		}
	}

	if ((attr = node.attribute("actionId"))) {
		lootBlock.actionId = pugi::cast<int32_t>(attr.value());
	}

	if ((attr = node.attribute("text"))) {
		lootBlock.text = attr.as_string();
	}
	return true;
}

void Monsters::loadLootContainer(const pugi::xml_node& node, LootBlock& lBlock)
{
	for (pugi::xml_node subNode = node.first_child(); subNode; subNode = subNode.next_sibling()) {
		LootBlock lootBlock;
		if (loadLootItem(subNode, lootBlock)) {
			lBlock.childLoot.push_back(lootBlock);
		}
	}
}

MonsterType* Monsters::getMonsterType(const std::string& name)
{
	uint32_t mId = getIdByName(name);
	if (mId == 0) {
		return nullptr;
	}
	return getMonsterType(mId);
}

MonsterType* Monsters::getMonsterType(uint32_t mid)
{
	auto it = monsters.find(mid);
	if (it == monsters.end()) {
		return nullptr;
	}
	return it->second;
}

uint32_t Monsters::getIdByName(const std::string& name)
{
	auto it = monsterNames.find(asLowerCaseString(name));
	if (it == monsterNames.end()) {
		return 0;
	}
	return it->second;
}

void Monsters::addMonsterType(const std::string& name, MonsterType* mType)
{
	static uint32_t id = 0;
	std::string lowername = name;
	toLowerCaseString(lowername);

	monsterNames[lowername] = ++id;
	monsters[id] = mType;
}
