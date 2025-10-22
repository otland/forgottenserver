// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "monsters.h"

#include "combat.h"
#include "configmanager.h"
#include "game.h"
#include "matrixarea.h"
#include "pugicast.h"
#include "spells.h"
#include "weapons.h"

extern Game g_game;
extern Spells* g_spells;
extern Monsters g_monsters;

spellBlock_t::~spellBlock_t()
{
	if (combatSpell) {
		delete spell;
	}
}

void MonsterType::loadLoot(MonsterType* monsterType, LootBlock lootBlock)
{
	if (lootBlock.childLoot.empty()) {
		bool isContainer = Item::items[lootBlock.id].isContainer();
		if (isContainer) {
			for (LootBlock child : lootBlock.childLoot) {
				lootBlock.childLoot.push_back(child);
			}
		}
		monsterType->info.lootItems.push_back(lootBlock);
	} else {
		monsterType->info.lootItems.push_back(lootBlock);
	}
}

bool Monsters::loadFromXml(bool reloading /*= false*/)
{
	unloadedMonsters = {};
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/monster/monsters.xml");
	if (!result) {
		printXMLError("Error - Monsters::loadFromXml", "data/monster/monsters.xml", result);
		return false;
	}

	loaded = true;

	for (auto monsterNode : doc.child("monsters").children()) {
		std::string name = boost::algorithm::to_lower_copy<std::string>(monsterNode.attribute("name").as_string());
		std::string file = "data/monster/" + std::string(monsterNode.attribute("file").as_string());
		unloadedMonsters.emplace(name, file);
	}

	bool forceLoad = getBoolean(ConfigManager::FORCE_MONSTERTYPE_LOAD);

	for (auto it : unloadedMonsters) {
		if (forceLoad || (reloading && monsters.find(it.first) != monsters.end())) {
			loadMonster(it.second, it.first, reloading);
		}
	}

	return true;
}

bool Monsters::reload()
{
	loaded = false;

	scriptInterface.reset();
	bestiary.clear();
	bestiaryMonsters.clear();

	return loadFromXml(true);
}

std::shared_ptr<ConditionDamage> Monsters::getDamageCondition(ConditionType_t conditionType, int32_t maxDamage,
                                                              int32_t minDamage, int32_t startDamage,
                                                              uint32_t tickInterval)
{
	auto condition =
	    std::static_pointer_cast<ConditionDamage>(Condition::createCondition(CONDITIONID_COMBAT, conditionType, 0, 0));
	condition->setParam(CONDITION_PARAM_TICKINTERVAL, tickInterval);
	condition->setParam(CONDITION_PARAM_MINVALUE, minDamage);
	condition->setParam(CONDITION_PARAM_MAXVALUE, maxDamage);
	condition->setParam(CONDITION_PARAM_STARTVALUE, startDamage);
	condition->setParam(CONDITION_PARAM_DELAYED, 1);
	return condition;
}

bool Monsters::deserializeSpell(const pugi::xml_node& node, spellBlock_t& sb, const std::string& description)
{
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
			std::cout << "[Warning - Monsters::deserializeSpell] " << description
			          << " - Chance value out of bounds for spell: " << name << std::endl;
		}
		sb.chance = chance;
	} else if (boost::algorithm::to_lower_copy(name) != "melee") {
		std::cout << "[Warning - Monsters::deserializeSpell] " << description
		          << " - Missing chance value on non-melee spell: " << name << std::endl;
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

		// normalize values
		if (std::abs(sb.minCombatValue) > std::abs(sb.maxCombatValue)) {
			int32_t value = sb.maxCombatValue;
			sb.maxCombatValue = sb.minCombatValue;
			sb.minCombatValue = value;
		}
	}

	if (auto spell = g_spells->getSpellByName(name)) {
		sb.spell = spell;
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
		if (!combatSpellPtr->loadScript("data/" + std::string{g_spells->getScriptBaseName()} + "/scripts/" +
		                                scriptName)) {
			return false;
		}

		if (!combatSpellPtr->loadScriptCombat()) {
			return false;
		}

		combatSpell = combatSpellPtr.release();
		combatSpell->getCombat()->setPlayerCombatValues(COMBAT_FORMULA_DAMAGE, sb.minCombatValue, 0, sb.maxCombatValue,
		                                                0);
	} else {
		Combat_ptr combat = std::make_shared<Combat>();
		if ((attr = node.attribute("length"))) {
			int32_t length = pugi::cast<int32_t>(attr.value());
			if (length > 0) {
				int32_t spread = 3;

				// need direction spell
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

			// target spell
			if ((attr = node.attribute("target"))) {
				needTarget = attr.as_bool();
			}

			AreaCombat* area = new AreaCombat();
			area->setupArea(radius);
			combat->setArea(area);
		}

		if ((attr = node.attribute("ring"))) {
			int32_t ring = pugi::cast<int32_t>(attr.value());

			// target spell
			if ((attr = node.attribute("target"))) {
				needTarget = attr.as_bool();
			}

			AreaCombat* area = new AreaCombat();
			area->setupAreaRing(ring);
			combat->setArea(area);
		}

		std::string tmpName = boost::algorithm::to_lower_copy(name);

		if (tmpName == "melee") {
			sb.isMelee = true;

			pugi::xml_attribute attackAttribute, skillAttribute;
			if ((attackAttribute = node.attribute("attack")) && (skillAttribute = node.attribute("skill"))) {
				sb.minCombatValue = 0;
				sb.maxCombatValue = -Weapons::getMaxMeleeDamage(pugi::cast<int32_t>(skillAttribute.value()),
				                                                pugi::cast<int32_t>(attackAttribute.value()));
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
				tickInterval = 4000;
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
				tickInterval = 4000;
			}

			if ((attr = node.attribute("tick"))) {
				int32_t value = pugi::cast<int32_t>(attr.value());
				if (value > 0) {
					tickInterval = value;
				}
			}

			if (conditionType != CONDITION_NONE) {
				auto condition = getDamageCondition(conditionType, maxDamage, minDamage, 0, tickInterval);
				combat->addCondition(condition);
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
			int32_t minSpeedChange = 0;
			int32_t maxSpeedChange = 0;
			int32_t duration = 10000;

			if ((attr = node.attribute("duration"))) {
				duration = pugi::cast<int32_t>(attr.value());
			}

			if ((attr = node.attribute("speedchange"))) {
				minSpeedChange = pugi::cast<int32_t>(attr.value());
			} else if ((attr = node.attribute("minspeedchange"))) {
				minSpeedChange = pugi::cast<int32_t>(attr.value());

				if ((attr = node.attribute("maxspeedchange"))) {
					maxSpeedChange = pugi::cast<int32_t>(attr.value());
				}

				if (minSpeedChange == 0) {
					std::cout << "[Error - Monsters::deserializeSpell] - " << description
					          << " - missing speedchange/minspeedchange value" << std::endl;
					return false;
				}

				if (maxSpeedChange == 0) {
					maxSpeedChange = minSpeedChange; // static speedchange value
				}
			}

			if (minSpeedChange < -1000) {
				std::cout << "[Warning - Monsters::deserializeSpell] - " << description
				          << " - you cannot reduce a creatures speed below -1000 (100%)" << std::endl;
				minSpeedChange = -1000;
			}

			ConditionType_t conditionType;
			if (minSpeedChange >= 0) {
				conditionType = CONDITION_HASTE;
				combat->setParam(COMBAT_PARAM_AGGRESSIVE, 0);
			} else {
				conditionType = CONDITION_PARALYZE;
			}

			auto condition = std::static_pointer_cast<ConditionSpeed>(
			    Condition::createCondition(CONDITIONID_COMBAT, conditionType, duration, 0));
			condition->setFormulaVars(minSpeedChange / 1000.0, 0, maxSpeedChange / 1000.0, 0);
			combat->addCondition(condition);
		} else if (tmpName == "outfit") {
			int32_t duration = 10000;

			if ((attr = node.attribute("duration"))) {
				duration = pugi::cast<int32_t>(attr.value());
			}

			if ((attr = node.attribute("monster"))) {
				MonsterType* mType = g_monsters.getMonsterType(attr.as_string());
				if (mType) {
					auto condition = std::static_pointer_cast<ConditionOutfit>(
					    Condition::createCondition(CONDITIONID_COMBAT, CONDITION_OUTFIT, duration, 0));
					condition->setOutfit(mType->info.outfit);
					combat->setParam(COMBAT_PARAM_AGGRESSIVE, 0);
					combat->addCondition(condition);
				}
			} else if ((attr = node.attribute("item"))) {
				Outfit_t outfit;
				outfit.lookTypeEx = pugi::cast<uint16_t>(attr.value());

				auto condition = std::static_pointer_cast<ConditionOutfit>(
				    Condition::createCondition(CONDITIONID_COMBAT, CONDITION_OUTFIT, duration, 0));
				condition->setOutfit(outfit);
				combat->setParam(COMBAT_PARAM_AGGRESSIVE, 0);
				combat->addCondition(condition);
			}
		} else if (tmpName == "invisible") {
			int32_t duration = 10000;

			if ((attr = node.attribute("duration"))) {
				duration = pugi::cast<int32_t>(attr.value());
			}

			auto condition = Condition::createCondition(CONDITIONID_COMBAT, CONDITION_INVISIBLE, duration, 0);
			combat->setParam(COMBAT_PARAM_AGGRESSIVE, 0);
			combat->addCondition(condition);
		} else if (tmpName == "drunk") {
			int32_t duration = 10000;
			uint8_t drunkenness = 25;

			if ((attr = node.attribute("duration"))) {
				duration = pugi::cast<int32_t>(attr.value());
			}

			if ((attr = node.attribute("drunkenness"))) {
				drunkenness = pugi::cast<uint8_t>(attr.value());
			}

			auto condition = Condition::createCondition(CONDITIONID_COMBAT, CONDITION_DRUNK, duration, drunkenness);
			combat->addCondition(condition);
		} else if (tmpName == "firefield") {
			combat->setParam(COMBAT_PARAM_CREATEITEM, ITEM_FIREFIELD_PVP_FULL);
		} else if (tmpName == "poisonfield") {
			combat->setParam(COMBAT_PARAM_CREATEITEM, ITEM_POISONFIELD_PVP);
		} else if (tmpName == "energyfield") {
			combat->setParam(COMBAT_PARAM_CREATEITEM, ITEM_ENERGYFIELD_PVP);
		} else if (tmpName == "firecondition" || tmpName == "energycondition" || tmpName == "earthcondition" ||
		           tmpName == "poisoncondition" || tmpName == "icecondition" || tmpName == "freezecondition" ||
		           tmpName == "deathcondition" || tmpName == "cursecondition" || tmpName == "holycondition" ||
		           tmpName == "dazzlecondition" || tmpName == "drowncondition" || tmpName == "bleedcondition" ||
		           tmpName == "physicalcondition") {
			ConditionType_t conditionType = CONDITION_NONE;
			uint32_t tickInterval = 2000;

			if (tmpName == "firecondition") {
				conditionType = CONDITION_FIRE;
				tickInterval = 10000;
			} else if (tmpName == "poisoncondition" || tmpName == "earthcondition") {
				conditionType = CONDITION_POISON;
				tickInterval = 4000;
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
				tickInterval = 4000;
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

			auto condition = getDamageCondition(conditionType, maxDamage, minDamage, startDamage, tickInterval);
			combat->addCondition(condition);
		} else if (tmpName == "strength") {
			//
		} else if (tmpName == "effect") {
			//
		} else {
			std::cout << "[Error - Monsters::deserializeSpell] - " << description << " - Unknown spell name: " << name
			          << std::endl;
			return false;
		}

		combat->setPlayerCombatValues(COMBAT_FORMULA_DAMAGE, sb.minCombatValue, 0, sb.maxCombatValue, 0);
		combatSpell = new CombatSpell(combat, needTarget, needDirection);

		for (auto attributeNode : node.children()) {
			if ((attr = attributeNode.attribute("key"))) {
				const char* value = attr.value();
				if (caseInsensitiveEqual(value, "shooteffect")) {
					if ((attr = attributeNode.attribute("value"))) {
						ShootType_t shoot =
						    getShootType(boost::algorithm::to_lower_copy<std::string>(attr.as_string()));
						if (shoot != CONST_ANI_NONE) {
							combat->setParam(COMBAT_PARAM_DISTANCEEFFECT, shoot);
						} else {
							std::cout << "[Warning - Monsters::deserializeSpell] " << description
							          << " - Unknown shootEffect: " << attr.as_string() << std::endl;
						}
					}
				} else if (caseInsensitiveEqual(value, "areaeffect")) {
					if ((attr = attributeNode.attribute("value"))) {
						MagicEffectClasses effect =
						    getMagicEffect(boost::algorithm::to_lower_copy<std::string>(attr.as_string()));
						if (effect != CONST_ME_NONE) {
							combat->setParam(COMBAT_PARAM_EFFECT, effect);
						} else {
							std::cout << "[Warning - Monsters::deserializeSpell] " << description
							          << " - Unknown areaEffect: " << attr.as_string() << std::endl;
						}
					}
				} else {
					std::cout << "[Warning - Monsters::deserializeSpells] Effect type \"" << attr.as_string()
					          << "\" does not exist." << std::endl;
				}
			}
		}
	}

	sb.spell = combatSpell;
	if (combatSpell) {
		sb.combatSpell = true;
	}
	return true;
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
		if (!combatSpellPtr->loadScript("data/" + std::string{g_spells->getScriptBaseName()} + "/scripts/" +
		                                spell->scriptName)) {
			std::cout << "cannot find file" << std::endl;
			return false;
		}

		if (!combatSpellPtr->loadScriptCombat()) {
			return false;
		}

		combatSpell = combatSpellPtr.release();
		combatSpell->getCombat()->setPlayerCombatValues(COMBAT_FORMULA_DAMAGE, sb.minCombatValue, 0, sb.maxCombatValue,
		                                                0);
	} else {
		Combat_ptr combat = std::make_shared<Combat>();
		sb.combatSpell = true;

		if (spell->length > 0) {
			spell->spread = std::max<int32_t>(0, spell->spread);

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

		if (spell->ring > 0) {
			AreaCombat* area = new AreaCombat();
			area->setupAreaRing(spell->ring);
			combat->setArea(area);
		}

		if (spell->conditionType != CONDITION_NONE) {
			ConditionType_t conditionType = spell->conditionType;

			uint32_t tickInterval = 2000;
			if (spell->tickInterval != 0) {
				tickInterval = spell->tickInterval;
			}

			int32_t conMinDamage = std::abs(spell->conditionMinDamage);
			int32_t conMaxDamage = std::abs(spell->conditionMaxDamage);
			int32_t startDamage = std::abs(spell->conditionStartDamage);

			auto condition = getDamageCondition(conditionType, conMaxDamage, conMinDamage, startDamage, tickInterval);
			combat->addCondition(condition);
		}

		std::string tmpName = boost::algorithm::to_lower_copy(spell->name);

		if (tmpName == "melee") {
			sb.isMelee = true;

			if (spell->attack > 0 && spell->skill > 0) {
				sb.minCombatValue = 0;
				sb.maxCombatValue = -Weapons::getMaxMeleeDamage(spell->skill, spell->attack);
			}

			sb.range = 1;
			combat->setParam(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE);
			combat->setParam(COMBAT_PARAM_BLOCKARMOR, 1);
			combat->setParam(COMBAT_PARAM_BLOCKSHIELD, 1);
			combat->setOrigin(ORIGIN_MELEE);
		} else if (tmpName == "combat") {
			if (spell->combatType == COMBAT_UNDEFINEDDAMAGE) {
				std::cout << "[Warning - Monsters::deserializeSpell] - " << description
				          << " - spell has undefined damage" << std::endl;
				combat->setParam(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE);
			}

			if (spell->combatType == COMBAT_PHYSICALDAMAGE) {
				combat->setParam(COMBAT_PARAM_BLOCKARMOR, 1);
				combat->setOrigin(ORIGIN_RANGED);
			} else if (spell->combatType == COMBAT_HEALING) {
				combat->setParam(COMBAT_PARAM_AGGRESSIVE, 0);
			}
			combat->setParam(COMBAT_PARAM_TYPE, spell->combatType);
		} else if (tmpName == "speed") {
			int32_t minSpeedChange = 0;
			int32_t maxSpeedChange = 0;
			int32_t duration = 10000;

			if (spell->duration != 0) {
				duration = spell->duration;
			}

			if (spell->minSpeedChange != 0) {
				minSpeedChange = spell->minSpeedChange;
			} else {
				std::cout << "[Error - Monsters::deserializeSpell] - " << description
				          << " - missing speedchange/minspeedchange value" << std::endl;
				delete spell;
				return false;
			}

			if (minSpeedChange < -1000) {
				std::cout << "[Warning - Monsters::deserializeSpell] - " << description
				          << " - you cannot reduce a creatures speed below -1000 (100%)" << std::endl;
				minSpeedChange = -1000;
			}

			if (spell->maxSpeedChange != 0) {
				maxSpeedChange = spell->maxSpeedChange;
			} else {
				maxSpeedChange = minSpeedChange; // static speedchange value
			}

			ConditionType_t conditionType;
			if (minSpeedChange >= 0) {
				conditionType = CONDITION_HASTE;
				combat->setParam(COMBAT_PARAM_AGGRESSIVE, 0);
			} else {
				conditionType = CONDITION_PARALYZE;
			}

			auto condition = std::static_pointer_cast<ConditionSpeed>(
			    Condition::createCondition(CONDITIONID_COMBAT, conditionType, duration, 0));
			condition->setFormulaVars(minSpeedChange / 1000.0, 0, maxSpeedChange / 1000.0, 0);
			combat->addCondition(condition);
		} else if (tmpName == "outfit") {
			int32_t duration = 10000;

			if (spell->duration != 0) {
				duration = spell->duration;
			}

			auto condition = std::static_pointer_cast<ConditionOutfit>(
			    Condition::createCondition(CONDITIONID_COMBAT, CONDITION_OUTFIT, duration, 0));
			condition->setOutfit(spell->outfit);
			combat->setParam(COMBAT_PARAM_AGGRESSIVE, 0);
			combat->addCondition(condition);
		} else if (tmpName == "invisible") {
			int32_t duration = 10000;

			if (spell->duration != 0) {
				duration = spell->duration;
			}

			auto condition = Condition::createCondition(CONDITIONID_COMBAT, CONDITION_INVISIBLE, duration, 0);
			combat->setParam(COMBAT_PARAM_AGGRESSIVE, 0);
			combat->addCondition(condition);
		} else if (tmpName == "drunk") {
			int32_t duration = 10000;
			uint8_t drunkenness = 25;

			if (spell->duration != 0) {
				duration = spell->duration;
			}

			if (spell->drunkenness != 0) {
				drunkenness = spell->drunkenness;
			}

			auto condition = Condition::createCondition(CONDITIONID_COMBAT, CONDITION_DRUNK, duration, drunkenness);
			combat->addCondition(condition);
		} else if (tmpName == "firefield") {
			combat->setParam(COMBAT_PARAM_CREATEITEM, ITEM_FIREFIELD_PVP_FULL);
		} else if (tmpName == "poisonfield") {
			combat->setParam(COMBAT_PARAM_CREATEITEM, ITEM_POISONFIELD_PVP);
		} else if (tmpName == "energyfield") {
			combat->setParam(COMBAT_PARAM_CREATEITEM, ITEM_ENERGYFIELD_PVP);
		} else if (tmpName == "condition") {
			if (spell->conditionType == CONDITION_NONE) {
				std::cout << "[Error - Monsters::deserializeSpell] - " << description
				          << " - Condition is not set for: " << spell->name << std::endl;
			}
		} else if (tmpName == "strength") {
			//
		} else if (tmpName == "effect") {
			//
		} else {
			std::cout << "[Error - Monsters::deserializeSpell] - " << description
			          << " - Unknown spell name: " << spell->name << std::endl;
		}

		if (spell->needTarget) {
			if (spell->shoot != CONST_ANI_NONE) {
				combat->setParam(COMBAT_PARAM_DISTANCEEFFECT, spell->shoot);
			}
		}

		if (spell->effect != CONST_ME_NONE) {
			combat->setParam(COMBAT_PARAM_EFFECT, spell->effect);
		}

		combat->setPlayerCombatValues(COMBAT_FORMULA_DAMAGE, sb.minCombatValue, 0, sb.maxCombatValue, 0);
		combatSpell = new CombatSpell(combat, spell->needTarget, spell->needDirection);
	}

	sb.spell = combatSpell;
	if (combatSpell) {
		sb.combatSpell = true;
	}
	return true;
}

MonsterType* Monsters::loadMonster(const std::string& file, const std::string& monsterName, bool reloading /*= false*/)
{
	MonsterType* mType = nullptr;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(file.c_str());
	if (!result) {
		printXMLError("Error - Monsters::loadMonster", file, result);
		return nullptr;
	}

	pugi::xml_node monsterNode = doc.child("monster");
	if (!monsterNode) {
		std::cout << "[Error - Monsters::loadMonster] Missing monster node in: " << file << std::endl;
		return nullptr;
	}

	pugi::xml_attribute attr;
	if (!(attr = monsterNode.attribute("name"))) {
		std::cout << "[Error - Monsters::loadMonster] Missing name in: " << file << std::endl;
		return nullptr;
	}

	if (reloading) {
		auto it = monsters.find(boost::algorithm::to_lower_copy(monsterName));
		if (it != monsters.end()) {
			mType = &it->second;
			mType->info = {};
		}
	}

	if (!mType) {
		mType = &monsters[boost::algorithm::to_lower_copy(monsterName)];
		mType->info = {};
	}

	mType->name = attr.as_string();

	if ((attr = monsterNode.attribute("nameDescription"))) {
		mType->nameDescription = attr.as_string();
	} else {
		mType->nameDescription = "a " + boost::algorithm::to_lower_copy(mType->name);
	}

	if ((attr = monsterNode.attribute("race"))) {
		std::string tmpStrValue = boost::algorithm::to_lower_copy<std::string>(attr.as_string());
		uint16_t tmpInt = pugi::cast<uint16_t>(attr.value());
		if (tmpStrValue == "venom" || tmpInt == 1) {
			mType->info.race = RACE_VENOM;
		} else if (tmpStrValue == "blood" || tmpInt == 2) {
			mType->info.race = RACE_BLOOD;
		} else if (tmpStrValue == "undead" || tmpInt == 3) {
			mType->info.race = RACE_UNDEAD;
		} else if (tmpStrValue == "fire" || tmpInt == 4) {
			mType->info.race = RACE_FIRE;
		} else if (tmpStrValue == "energy" || tmpInt == 5) {
			mType->info.race = RACE_ENERGY;
		} else if (tmpStrValue == "ink" || tmpInt == 6) {
			mType->info.race = RACE_INK;
		} else {
			std::cout << "[Warning - Monsters::loadMonster] Unknown race type " << attr.as_string() << ". " << file
			          << std::endl;
		}
	}

	if ((attr = monsterNode.attribute("experience"))) {
		mType->info.experience = pugi::cast<uint64_t>(attr.value());
	}

	if ((attr = monsterNode.attribute("speed"))) {
		mType->info.baseSpeed = pugi::cast<int32_t>(attr.value());
	}

	if ((attr = monsterNode.attribute("manacost"))) {
		mType->info.manaCost = pugi::cast<uint32_t>(attr.value());
	}

	if ((attr = monsterNode.attribute("skull"))) {
		mType->info.skull = getSkullType(boost::algorithm::to_lower_copy<std::string>(attr.as_string()));
	}

	if ((attr = monsterNode.attribute("raceId"))) {
		mType->bestiaryInfo.raceId = attr.as_uint();
	}

	if ((attr = monsterNode.attribute("script"))) {
		if (!scriptInterface) {
			scriptInterface.reset(new LuaScriptInterface("Monster Interface"));
			scriptInterface->initState();
		}

		std::string script = attr.as_string();
		if (scriptInterface->loadFile("data/monster/scripts/" + script) == 0) {
			mType->info.scriptInterface = scriptInterface.get();
			mType->info.creatureAppearEvent = scriptInterface->getEvent("onCreatureAppear");
			mType->info.creatureDisappearEvent = scriptInterface->getEvent("onCreatureDisappear");
			mType->info.creatureMoveEvent = scriptInterface->getEvent("onCreatureMove");
			mType->info.creatureSayEvent = scriptInterface->getEvent("onCreatureSay");
			mType->info.thinkEvent = scriptInterface->getEvent("onThink");
		} else {
			std::cout << "[Warning - Monsters::loadMonster] Can not load script: " << script << std::endl;
			std::cout << scriptInterface->getLastLuaError() << std::endl;
		}
	}

	pugi::xml_node node;
	if ((node = monsterNode.child("health"))) {
		if ((attr = node.attribute("now"))) {
			mType->info.health = pugi::cast<int32_t>(attr.value());
		} else {
			std::cout << "[Error - Monsters::loadMonster] Missing health now. " << file << std::endl;
		}

		if ((attr = node.attribute("max"))) {
			mType->info.healthMax = pugi::cast<int32_t>(attr.value());
		} else {
			std::cout << "[Error - Monsters::loadMonster] Missing health max. " << file << std::endl;
		}

		if (mType->info.health > mType->info.healthMax) {
			mType->info.health = mType->info.healthMax;
			std::cout << "[Warning - Monsters::loadMonster] Health now is greater than health max." << file
			          << std::endl;
		}
	}

	if ((node = monsterNode.child("flags"))) {
		for (auto flagNode : node.children()) {
			attr = flagNode.first_attribute();
			const char* attrName = attr.name();
			if (caseInsensitiveEqual(attrName, "summonable")) {
				mType->info.isSummonable = attr.as_bool();
			} else if (caseInsensitiveEqual(attrName, "attackable")) {
				mType->info.isAttackable = attr.as_bool();
			} else if (caseInsensitiveEqual(attrName, "hostile")) {
				mType->info.isHostile = attr.as_bool();
			} else if (caseInsensitiveEqual(attrName, "ignorespawnblock")) {
				mType->info.isIgnoringSpawnBlock = attr.as_bool();
			} else if (caseInsensitiveEqual(attrName, "illusionable")) {
				mType->info.isIllusionable = attr.as_bool();
			} else if (caseInsensitiveEqual(attrName, "challengeable")) {
				mType->info.isChallengeable = attr.as_bool();
			} else if (caseInsensitiveEqual(attrName, "convinceable")) {
				mType->info.isConvinceable = attr.as_bool();
			} else if (caseInsensitiveEqual(attrName, "pushable")) {
				mType->info.pushable = attr.as_bool();
			} else if (caseInsensitiveEqual(attrName, "isboss")) {
				mType->info.isBoss = attr.as_bool();
			} else if (caseInsensitiveEqual(attrName, "canpushitems")) {
				mType->info.canPushItems = attr.as_bool();
			} else if (caseInsensitiveEqual(attrName, "canpushcreatures")) {
				mType->info.canPushCreatures = attr.as_bool();
			} else if (caseInsensitiveEqual(attrName, "staticattack")) {
				uint32_t staticAttack = pugi::cast<uint32_t>(attr.value());
				if (staticAttack > 100) {
					std::cout << "[Warning - Monsters::loadMonster] staticattack greater than 100. " << file
					          << std::endl;
					staticAttack = 100;
				}

				mType->info.staticAttackChance = staticAttack;
			} else if (caseInsensitiveEqual(attrName, "lightlevel")) {
				mType->info.light.level = pugi::cast<uint16_t>(attr.value());
			} else if (caseInsensitiveEqual(attrName, "lightcolor")) {
				mType->info.light.color = pugi::cast<uint16_t>(attr.value());
			} else if (caseInsensitiveEqual(attrName, "targetdistance")) {
				int32_t targetDistance = pugi::cast<int32_t>(attr.value());
				if (targetDistance < 1) {
					targetDistance = 1;
					std::cout << "[Warning - Monsters::loadMonster] targetdistance less than 1. " << file << std::endl;
				}
				mType->info.targetDistance = targetDistance;
			} else if (caseInsensitiveEqual(attrName, "runonhealth")) {
				mType->info.runAwayHealth = pugi::cast<int32_t>(attr.value());
			} else if (caseInsensitiveEqual(attrName, "hidehealth")) {
				mType->info.hiddenHealth = attr.as_bool();
			} else if (caseInsensitiveEqual(attrName, "canwalkonenergy")) {
				mType->info.canWalkOnEnergy = attr.as_bool();
			} else if (caseInsensitiveEqual(attrName, "canwalkonfire")) {
				mType->info.canWalkOnFire = attr.as_bool();
			} else if (caseInsensitiveEqual(attrName, "canwalkonpoison")) {
				mType->info.canWalkOnPoison = attr.as_bool();
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Unknown flag attribute: " << attrName << ". " << file
				          << std::endl;
			}
		}

		// if a monster can push creatures, it should not be pushable.
		if (mType->info.canPushCreatures) {
			mType->info.pushable = false;
		}
	}

	if ((node = monsterNode.child("bestiary"))) {
		if ((attr = node.attribute("class"))) {
			mType->bestiaryInfo.className = attr.as_string();
		}
		if ((attr = node.attribute("prowess"))) {
			mType->bestiaryInfo.prowess = pugi::cast<uint32_t>(attr.value());
		}
		if ((attr = node.attribute("expertise"))) {
			mType->bestiaryInfo.expertise = pugi::cast<uint32_t>(attr.value());
		}
		if ((attr = node.attribute("mastery"))) {
			mType->bestiaryInfo.mastery = pugi::cast<uint32_t>(attr.value());
		}
		if ((attr = node.attribute("charmPoints"))) {
			mType->bestiaryInfo.charmPoints = pugi::cast<uint32_t>(attr.value());
		}
		if ((attr = node.attribute("difficulty"))) {
			std::string tmpStrValue = boost::algorithm::to_lower_copy<std::string>(attr.as_string());
			if (tmpStrValue == "harmless") {
				mType->bestiaryInfo.difficulty = 0;
			} else if (tmpStrValue == "trivial") {
				mType->bestiaryInfo.difficulty = 1;
			} else if (tmpStrValue == "easy") {
				mType->bestiaryInfo.difficulty = 2;
			} else if (tmpStrValue == "medium") {
				mType->bestiaryInfo.difficulty = 3;
			} else if (tmpStrValue == "hard") {
				mType->bestiaryInfo.difficulty = 4;
			} else if (tmpStrValue == "challenging") {
				mType->bestiaryInfo.difficulty = 5;
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Unknown difficulty: " << attr.as_string() << ". "
				          << file << std::endl;
			}
		}
		if ((attr = node.attribute("occurrence"))) {
			mType->bestiaryInfo.occurrence = pugi::cast<uint32_t>(attr.value());
		}
		if ((attr = node.attribute("locations"))) {
			mType->bestiaryInfo.locations = attr.as_string();
		}

		if (!isValidBestiaryInfo(mType->bestiaryInfo)) {
			mType->bestiaryInfo = {};
			std::cout << "[Warning - Monsters::loadMonster] invalid bestiary info for " << mType->name << "."
			          << std::endl;
		} else {
			addBestiaryMonsterType(mType);
		}
	}

	if (mType->info.manaCost == 0 && (mType->info.isSummonable || mType->info.isConvinceable)) {
		std::cout
		    << "[Warning - Monsters::loadMonster] manaCost missing or zero on monster with summonable and/or convinceable flags: "
		    << file << std::endl;
	}

	if ((node = monsterNode.child("targetchange"))) {
		if ((attr = node.attribute("speed")) || (attr = node.attribute("interval"))) {
			mType->info.changeTargetSpeed = pugi::cast<uint32_t>(attr.value());
		} else {
			std::cout << "[Warning - Monsters::loadMonster] Missing targetchange speed. " << file << std::endl;
		}

		if ((attr = node.attribute("chance"))) {
			int32_t chance = pugi::cast<int32_t>(attr.value());
			if (chance > 100) {
				chance = 100;
				std::cout << "[Warning - Monsters::loadMonster] targetchange chance value out of bounds. " << file
				          << std::endl;
			}
			mType->info.changeTargetChance = chance;
		} else {
			std::cout << "[Warning - Monsters::loadMonster] Missing targetchange chance. " << file << std::endl;
		}
	}

	if ((node = monsterNode.child("look"))) {
		if ((attr = node.attribute("type"))) {
			mType->info.outfit.lookType = pugi::cast<uint16_t>(attr.value());

			if ((attr = node.attribute("head"))) {
				mType->info.outfit.lookHead = pugi::cast<uint16_t>(attr.value());
			}

			if ((attr = node.attribute("body"))) {
				mType->info.outfit.lookBody = pugi::cast<uint16_t>(attr.value());
			}

			if ((attr = node.attribute("legs"))) {
				mType->info.outfit.lookLegs = pugi::cast<uint16_t>(attr.value());
			}

			if ((attr = node.attribute("feet"))) {
				mType->info.outfit.lookFeet = pugi::cast<uint16_t>(attr.value());
			}

			if ((attr = node.attribute("addons"))) {
				mType->info.outfit.lookAddons = pugi::cast<uint16_t>(attr.value());
			}
		} else if ((attr = node.attribute("typeex"))) {
			mType->info.outfit.lookTypeEx = pugi::cast<uint16_t>(attr.value());
		} else {
			std::cout << "[Warning - Monsters::loadMonster] Missing look type/typeex. " << file << std::endl;
		}

		if ((attr = node.attribute("mount"))) {
			mType->info.outfit.lookMount = pugi::cast<uint16_t>(attr.value());
		}

		if ((attr = node.attribute("corpse"))) {
			mType->info.lookcorpse = pugi::cast<uint16_t>(attr.value());
		}
	}

	if ((node = monsterNode.child("attacks"))) {
		for (auto attackNode : node.children()) {
			spellBlock_t sb;
			if (deserializeSpell(attackNode, sb, monsterName)) {
				mType->info.attackSpells.emplace_back(std::move(sb));
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Cant load spell. " << file << std::endl;
			}
		}
	}

	if ((node = monsterNode.child("defenses"))) {
		if ((attr = node.attribute("defense"))) {
			mType->info.defense = pugi::cast<int32_t>(attr.value());
		}

		if ((attr = node.attribute("armor"))) {
			mType->info.armor = pugi::cast<int32_t>(attr.value());
		}

		for (auto defenseNode : node.children()) {
			spellBlock_t sb;
			if (deserializeSpell(defenseNode, sb, monsterName)) {
				mType->info.defenseSpells.emplace_back(std::move(sb));
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Cant load spell. " << file << std::endl;
			}
		}
	}

	if ((node = monsterNode.child("immunities"))) {
		for (auto immunityNode : node.children()) {
			if ((attr = immunityNode.attribute("name"))) {
				std::string tmpStrValue = boost::algorithm::to_lower_copy<std::string>(attr.as_string());
				if (tmpStrValue == "physical") {
					mType->info.damageImmunities |= COMBAT_PHYSICALDAMAGE;
					mType->info.conditionImmunities |= CONDITION_BLEEDING;
				} else if (tmpStrValue == "energy") {
					mType->info.damageImmunities |= COMBAT_ENERGYDAMAGE;
					mType->info.conditionImmunities |= CONDITION_ENERGY;
				} else if (tmpStrValue == "fire") {
					mType->info.damageImmunities |= COMBAT_FIREDAMAGE;
					mType->info.conditionImmunities |= CONDITION_FIRE;
				} else if (tmpStrValue == "poison" || tmpStrValue == "earth") {
					mType->info.damageImmunities |= COMBAT_EARTHDAMAGE;
					mType->info.conditionImmunities |= CONDITION_POISON;
				} else if (tmpStrValue == "drown") {
					mType->info.damageImmunities |= COMBAT_DROWNDAMAGE;
					mType->info.conditionImmunities |= CONDITION_DROWN;
				} else if (tmpStrValue == "ice") {
					mType->info.damageImmunities |= COMBAT_ICEDAMAGE;
					mType->info.conditionImmunities |= CONDITION_FREEZING;
				} else if (tmpStrValue == "holy") {
					mType->info.damageImmunities |= COMBAT_HOLYDAMAGE;
					mType->info.conditionImmunities |= CONDITION_DAZZLED;
				} else if (tmpStrValue == "death") {
					mType->info.damageImmunities |= COMBAT_DEATHDAMAGE;
					mType->info.conditionImmunities |= CONDITION_CURSED;
				} else if (tmpStrValue == "lifedrain") {
					mType->info.damageImmunities |= COMBAT_LIFEDRAIN;
				} else if (tmpStrValue == "manadrain") {
					mType->info.damageImmunities |= COMBAT_MANADRAIN;
				} else if (tmpStrValue == "paralyze") {
					mType->info.conditionImmunities |= CONDITION_PARALYZE;
				} else if (tmpStrValue == "outfit") {
					mType->info.conditionImmunities |= CONDITION_OUTFIT;
				} else if (tmpStrValue == "drunk") {
					mType->info.conditionImmunities |= CONDITION_DRUNK;
				} else if (tmpStrValue == "invisible" || tmpStrValue == "invisibility") {
					mType->info.conditionImmunities |= CONDITION_INVISIBLE;
				} else if (tmpStrValue == "bleed") {
					mType->info.conditionImmunities |= CONDITION_BLEEDING;
				} else {
					std::cout << "[Warning - Monsters::loadMonster] Unknown immunity name " << attr.as_string() << ". "
					          << file << std::endl;
				}
			} else if ((attr = immunityNode.attribute("physical"))) {
				if (attr.as_bool()) {
					mType->info.damageImmunities |= COMBAT_PHYSICALDAMAGE;
					mType->info.conditionImmunities |= CONDITION_BLEEDING;
				}
			} else if ((attr = immunityNode.attribute("energy"))) {
				if (attr.as_bool()) {
					mType->info.damageImmunities |= COMBAT_ENERGYDAMAGE;
					mType->info.conditionImmunities |= CONDITION_ENERGY;
				}
			} else if ((attr = immunityNode.attribute("fire"))) {
				if (attr.as_bool()) {
					mType->info.damageImmunities |= COMBAT_FIREDAMAGE;
					mType->info.conditionImmunities |= CONDITION_FIRE;
				}
			} else if ((attr = immunityNode.attribute("poison")) || (attr = immunityNode.attribute("earth"))) {
				if (attr.as_bool()) {
					mType->info.damageImmunities |= COMBAT_EARTHDAMAGE;
					mType->info.conditionImmunities |= CONDITION_POISON;
				}
			} else if ((attr = immunityNode.attribute("drown"))) {
				if (attr.as_bool()) {
					mType->info.damageImmunities |= COMBAT_DROWNDAMAGE;
					mType->info.conditionImmunities |= CONDITION_DROWN;
				}
			} else if ((attr = immunityNode.attribute("ice"))) {
				if (attr.as_bool()) {
					mType->info.damageImmunities |= COMBAT_ICEDAMAGE;
					mType->info.conditionImmunities |= CONDITION_FREEZING;
				}
			} else if ((attr = immunityNode.attribute("holy"))) {
				if (attr.as_bool()) {
					mType->info.damageImmunities |= COMBAT_HOLYDAMAGE;
					mType->info.conditionImmunities |= CONDITION_DAZZLED;
				}
			} else if ((attr = immunityNode.attribute("death"))) {
				if (attr.as_bool()) {
					mType->info.damageImmunities |= COMBAT_DEATHDAMAGE;
					mType->info.conditionImmunities |= CONDITION_CURSED;
				}
			} else if ((attr = immunityNode.attribute("lifedrain"))) {
				if (attr.as_bool()) {
					mType->info.damageImmunities |= COMBAT_LIFEDRAIN;
				}
			} else if ((attr = immunityNode.attribute("manadrain"))) {
				if (attr.as_bool()) {
					mType->info.damageImmunities |= COMBAT_MANADRAIN;
				}
			} else if ((attr = immunityNode.attribute("paralyze"))) {
				if (attr.as_bool()) {
					mType->info.conditionImmunities |= CONDITION_PARALYZE;
				}
			} else if ((attr = immunityNode.attribute("outfit"))) {
				if (attr.as_bool()) {
					mType->info.conditionImmunities |= CONDITION_OUTFIT;
				}
			} else if ((attr = immunityNode.attribute("bleed"))) {
				if (attr.as_bool()) {
					mType->info.conditionImmunities |= CONDITION_BLEEDING;
				}
			} else if ((attr = immunityNode.attribute("drunk"))) {
				if (attr.as_bool()) {
					mType->info.conditionImmunities |= CONDITION_DRUNK;
				}
			} else if ((attr = immunityNode.attribute("invisible")) ||
			           (attr = immunityNode.attribute("invisibility"))) {
				if (attr.as_bool()) {
					mType->info.conditionImmunities |= CONDITION_INVISIBLE;
				}
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Unknown immunity. " << file << std::endl;
			}
		}
	}

	if ((node = monsterNode.child("voices"))) {
		if ((attr = node.attribute("speed")) || (attr = node.attribute("interval"))) {
			mType->info.yellSpeedTicks = pugi::cast<uint32_t>(attr.value());
		} else {
			std::cout << "[Warning - Monsters::loadMonster] Missing voices speed. " << file << std::endl;
		}

		if ((attr = node.attribute("chance"))) {
			uint32_t chance = pugi::cast<uint32_t>(attr.value());
			if (chance > 100) {
				chance = 100;
				std::cout << "[Warning - Monsters::loadMonster] yell chance value out of bounds. " << file << std::endl;
			}
			mType->info.yellChance = chance;
		} else {
			std::cout << "[Warning - Monsters::loadMonster] Missing voices chance. " << file << std::endl;
		}

		for (auto voiceNode : node.children()) {
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
			mType->info.voiceVector.emplace_back(vb);
		}
	}

	if ((node = monsterNode.child("loot"))) {
		for (auto lootNode : node.children()) {
			LootBlock lootBlock;
			if (loadLootItem(lootNode, lootBlock)) {
				mType->info.lootItems.emplace_back(std::move(lootBlock));
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Cant load loot. " << file << std::endl;
			}
		}
	}

	if ((node = monsterNode.child("elements"))) {
		for (auto elementNode : node.children()) {
			if ((attr = elementNode.attribute("physicalPercent"))) {
				mType->info.elementMap[COMBAT_PHYSICALDAMAGE] = pugi::cast<int32_t>(attr.value());
				if (mType->info.damageImmunities & COMBAT_PHYSICALDAMAGE) {
					std::cout
					    << "[Warning - Monsters::loadMonster] Same element \"physical\" on immunity and element tags. "
					    << file << std::endl;
				}
			} else if ((attr = elementNode.attribute("icePercent"))) {
				mType->info.elementMap[COMBAT_ICEDAMAGE] = pugi::cast<int32_t>(attr.value());
				if (mType->info.damageImmunities & COMBAT_ICEDAMAGE) {
					std::cout << "[Warning - Monsters::loadMonster] Same element \"ice\" on immunity and element tags. "
					          << file << std::endl;
				}
			} else if ((attr = elementNode.attribute("poisonPercent")) ||
			           (attr = elementNode.attribute("earthPercent"))) {
				mType->info.elementMap[COMBAT_EARTHDAMAGE] = pugi::cast<int32_t>(attr.value());
				if (mType->info.damageImmunities & COMBAT_EARTHDAMAGE) {
					std::cout
					    << "[Warning - Monsters::loadMonster] Same element \"earth\" on immunity and element tags. "
					    << file << std::endl;
				}
			} else if ((attr = elementNode.attribute("firePercent"))) {
				mType->info.elementMap[COMBAT_FIREDAMAGE] = pugi::cast<int32_t>(attr.value());
				if (mType->info.damageImmunities & COMBAT_FIREDAMAGE) {
					std::cout
					    << "[Warning - Monsters::loadMonster] Same element \"fire\" on immunity and element tags. "
					    << file << std::endl;
				}
			} else if ((attr = elementNode.attribute("energyPercent"))) {
				mType->info.elementMap[COMBAT_ENERGYDAMAGE] = pugi::cast<int32_t>(attr.value());
				if (mType->info.damageImmunities & COMBAT_ENERGYDAMAGE) {
					std::cout
					    << "[Warning - Monsters::loadMonster] Same element \"energy\" on immunity and element tags. "
					    << file << std::endl;
				}
			} else if ((attr = elementNode.attribute("holyPercent"))) {
				mType->info.elementMap[COMBAT_HOLYDAMAGE] = pugi::cast<int32_t>(attr.value());
				if (mType->info.damageImmunities & COMBAT_HOLYDAMAGE) {
					std::cout
					    << "[Warning - Monsters::loadMonster] Same element \"holy\" on immunity and element tags. "
					    << file << std::endl;
				}
			} else if ((attr = elementNode.attribute("deathPercent"))) {
				mType->info.elementMap[COMBAT_DEATHDAMAGE] = pugi::cast<int32_t>(attr.value());
				if (mType->info.damageImmunities & COMBAT_DEATHDAMAGE) {
					std::cout
					    << "[Warning - Monsters::loadMonster] Same element \"death\" on immunity and element tags. "
					    << file << std::endl;
				}
			} else if ((attr = elementNode.attribute("drownPercent"))) {
				mType->info.elementMap[COMBAT_DROWNDAMAGE] = pugi::cast<int32_t>(attr.value());
				if (mType->info.damageImmunities & COMBAT_DROWNDAMAGE) {
					std::cout
					    << "[Warning - Monsters::loadMonster] Same element \"drown\" on immunity and element tags. "
					    << file << std::endl;
				}
			} else if ((attr = elementNode.attribute("lifedrainPercent"))) {
				mType->info.elementMap[COMBAT_LIFEDRAIN] = pugi::cast<int32_t>(attr.value());
				if (mType->info.damageImmunities & COMBAT_LIFEDRAIN) {
					std::cout
					    << "[Warning - Monsters::loadMonster] Same element \"lifedrain\" on immunity and element tags. "
					    << file << std::endl;
				}
			} else if ((attr = elementNode.attribute("manadrainPercent"))) {
				mType->info.elementMap[COMBAT_MANADRAIN] = pugi::cast<int32_t>(attr.value());
				if (mType->info.damageImmunities & COMBAT_MANADRAIN) {
					std::cout
					    << "[Warning - Monsters::loadMonster] Same element \"manadrain\" on immunity and element tags. "
					    << file << std::endl;
				}
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Unknown element percent. " << file << std::endl;
			}
		}
	}

	if ((node = monsterNode.child("summons"))) {
		if ((attr = node.attribute("maxSummons"))) {
			mType->info.maxSummons = std::min<uint32_t>(pugi::cast<uint32_t>(attr.value()), 100);
		} else {
			std::cout << "[Warning - Monsters::loadMonster] Missing summons maxSummons. " << file << std::endl;
		}

		for (auto summonNode : node.children()) {
			int32_t chance = 100;
			int32_t speed = 1000;
			int32_t max = mType->info.maxSummons;
			MagicEffectClasses effect = CONST_ME_TELEPORT;
			MagicEffectClasses masterEffect = CONST_ME_NONE;
			bool force = false;

			if ((attr = summonNode.attribute("speed")) || (attr = summonNode.attribute("interval"))) {
				speed = std::max<int32_t>(1, pugi::cast<int32_t>(attr.value()));
			}

			if ((attr = summonNode.attribute("chance"))) {
				chance = pugi::cast<int32_t>(attr.value());
				if (chance > 100) {
					chance = 100;
					std::cout << "[Warning - Monsters::loadMonster] Summon chance value out of bounds. " << file
					          << std::endl;
				}
			}

			if ((attr = summonNode.attribute("max"))) {
				max = pugi::cast<uint32_t>(attr.value());
			}

			for (const auto& attributeNode : summonNode.children()) {
				if ((attr = attributeNode.attribute("key"))) {
					const char* value = attr.value();
					if (caseInsensitiveEqual(value, "mastereffect")) {
						if ((attr = attributeNode.attribute("value"))) {
							masterEffect =
							    getMagicEffect(boost::algorithm::to_lower_copy<std::string>(attr.as_string()));
							if (masterEffect == CONST_ME_NONE) {
								std::cout
								    << "[Warning - Monsters::loadMonster] Summon master effect - Unknown masterEffect: "
								    << attr.as_string() << std::endl;
							}
						}
					} else if (caseInsensitiveEqual(value, "effect")) {
						if ((attr = attributeNode.attribute("value"))) {
							effect = getMagicEffect(boost::algorithm::to_lower_copy<std::string>(attr.as_string()));
							if (effect == CONST_ME_NONE) {
								effect = CONST_ME_TELEPORT;
								std::cout << "[Warning - Monsters::loadMonster] Summon effect - Unknown effect: "
								          << attr.as_string() << std::endl;
							}
						}
					} else {
						std::cout << "[Warning - Monsters::loadMonster] Summon effect type \"" << attr.as_string()
						          << "\" does not exist." << std::endl;
					}
				}
			}

			if ((attr = summonNode.attribute("force"))) {
				force = attr.as_bool();
			}

			if ((attr = summonNode.attribute("name"))) {
				summonBlock_t sb;
				sb.name = attr.as_string();
				sb.speed = speed;
				sb.chance = chance;
				sb.max = max;
				sb.effect = effect;
				sb.masterEffect = masterEffect;
				sb.force = force;
				mType->info.summons.emplace_back(sb);
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Missing summon name. " << file << std::endl;
			}
		}
	}

	if ((node = monsterNode.child("script"))) {
		for (auto eventNode : node.children()) {
			if ((attr = eventNode.attribute("name"))) {
				mType->info.scripts.emplace_back(attr.as_string());
			} else {
				std::cout << "[Warning - Monsters::loadMonster] Missing name for script event. " << file << std::endl;
			}
		}
	}

	mType->info.summons.shrink_to_fit();
	mType->info.lootItems.shrink_to_fit();
	mType->info.attackSpells.shrink_to_fit();
	mType->info.defenseSpells.shrink_to_fit();
	mType->info.voiceVector.shrink_to_fit();
	mType->info.scripts.shrink_to_fit();
	return mType;
}

bool MonsterType::loadCallback(LuaScriptInterface* scriptInterface)
{
	int32_t id = scriptInterface->getEvent();
	if (id == -1) {
		std::cout << "[Warning - MonsterType::loadCallback] Event not found. " << std::endl;
		return false;
	}

	info.scriptInterface = scriptInterface;
	if (info.eventType == MONSTERS_EVENT_THINK) {
		info.thinkEvent = id;
	} else if (info.eventType == MONSTERS_EVENT_APPEAR) {
		info.creatureAppearEvent = id;
	} else if (info.eventType == MONSTERS_EVENT_DISAPPEAR) {
		info.creatureDisappearEvent = id;
	} else if (info.eventType == MONSTERS_EVENT_MOVE) {
		info.creatureMoveEvent = id;
	} else if (info.eventType == MONSTERS_EVENT_SAY) {
		info.creatureSayEvent = id;
	}
	return true;
}

bool Monsters::loadLootItem(const pugi::xml_node& node, LootBlock& lootBlock)
{
	pugi::xml_attribute attr;
	if ((attr = node.attribute("id"))) {
		int32_t id = pugi::cast<int32_t>(attr.value());
		const ItemType& it = Item::items.getItemType(id);

		if (it.name.empty()) {
			std::cout << "[Warning - Monsters::loadMonster] Unknown loot item id \"" << id << "\". " << std::endl;
			return false;
		}

		lootBlock.id = id;

	} else if ((attr = node.attribute("name"))) {
		auto name = attr.as_string();
		auto ids = Item::items.nameToItems.equal_range(boost::algorithm::to_lower_copy<std::string>(name));

		if (ids.first == Item::items.nameToItems.cend()) {
			std::cout << "[Warning - Monsters::loadMonster] Unknown loot item \"" << name << "\". " << std::endl;
			return false;
		}

		uint32_t id = ids.first->second;

		if (std::next(ids.first) != ids.second) {
			std::cout << "[Warning - Monsters::loadMonster] Non-unique loot item \"" << name << "\". " << std::endl;
			return false;
		}

		lootBlock.id = id;
	}

	if (lootBlock.id == 0) {
		return false;
	}

	if ((attr = node.attribute("countmax"))) {
		int32_t lootCountMax = pugi::cast<int32_t>(attr.value());
		lootBlock.countmax = std::max<int32_t>(1, lootCountMax);
	} else {
		lootBlock.countmax = 1;
	}

	if ((attr = node.attribute("chance")) || (attr = node.attribute("chance1"))) {
		int32_t lootChance = pugi::cast<int32_t>(attr.value());
		if (lootChance > static_cast<int32_t>(MAX_LOOTCHANCE)) {
			std::cout << "[Warning - Monsters::loadMonster] Invalid \"chance\" " << lootChance
			          << " used for loot, the max is " << MAX_LOOTCHANCE << ". " << std::endl;
		}
		lootBlock.chance = std::min<int32_t>(MAX_LOOTCHANCE, lootChance);
	} else {
		lootBlock.chance = MAX_LOOTCHANCE;
	}

	if (Item::items[lootBlock.id].isContainer()) {
		loadLootContainer(node, lootBlock);
	}

	// optional
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
	// NOTE: <inside> attribute was left for backwards compatibility with pre 1.x TFS versions. Please don't use it, if
	// you don't have to.
	for (auto subNode : node.child("inside") ? node.child("inside").children() : node.children()) {
		LootBlock lootBlock;
		if (loadLootItem(subNode, lootBlock)) {
			lBlock.childLoot.emplace_back(std::move(lootBlock));
		}
	}
}

MonsterType* Monsters::getMonsterType(const std::string& name, bool loadFromFile /*= true */)
{
	std::string lowerCaseName = boost::algorithm::to_lower_copy(name);

	auto it = monsters.find(lowerCaseName);
	if (it == monsters.end()) {
		if (!loadFromFile) {
			return nullptr;
		}

		auto it2 = unloadedMonsters.find(lowerCaseName);
		if (it2 == unloadedMonsters.end()) {
			return nullptr;
		}

		return loadMonster(it2->second, name);
	}
	return &it->second;
}

MonsterType* Monsters::getMonsterType(uint32_t raceId)
{
	if (bestiaryMonsters.contains(raceId)) {
		return getMonsterType(bestiaryMonsters[raceId]);
	}

	return nullptr;
}

bool Monsters::addBestiaryMonsterType(const MonsterType* monsterType)
{
	if (!isValidBestiaryInfo(monsterType->bestiaryInfo)) {
		return false;
	}

	bestiary[monsterType->bestiaryInfo.className].emplace(monsterType->name);
	bestiaryMonsters[monsterType->bestiaryInfo.raceId] = monsterType->name;
	return true;
}

bool Monsters::isValidBestiaryInfo(const BestiaryInfo& info) const
{
	if (info.raceId == 0) {
		std::cout << "[Warning - Monsters::isValidBestiaryInfo] race id can't be 0." << std::endl;
		return false;
	}

	if (info.className.empty()) {
		std::cout << "[Warning - Monsters::isValidBestiaryInfo] class name can't be empty." << std::endl;
		return false;
	}

	if (info.prowess == 0 || info.expertise == 0 || info.mastery == 0) {
		std::cout << "[Warning - Monsters::isValidBestiaryInfo] prowess, expertise and mastery can't be 0."
		          << std::endl;
		return false;
	}

	if (info.prowess >= info.expertise || info.expertise >= info.mastery) {
		std::cout
		    << "[Warning - Monsters::isValidBestiaryInfo] prowess must be lower than expertise and expertise must be lower than mastery."
		    << std::endl;
		return false;
	}

	if (info.difficulty > BESTIARY_MAX_DIFFICULTY) {
		std::cout << "[Warning - Monsters::isValidBestiaryInfo] difficulty can't be higher than "
		          << BESTIARY_MAX_DIFFICULTY << '.' << std::endl;
		return false;
	}

	if (info.occurrence > BESTIARY_MAX_OCCURRENCE) {
		std::cout << "[Warning - Monsters::isValidBestiaryInfo] occurrence can't be higher than "
		          << BESTIARY_MAX_OCCURRENCE << '.' << std::endl;
		return false;
	}

	return true;
}
