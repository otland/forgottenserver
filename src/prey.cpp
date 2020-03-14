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

#include "prey.h"

#include "monsters.h"
#include "pugicast.h"
#include "tools.h"

extern Monsters g_monsters;

BonusType getBonusTypeByString(const std::string& bonusStr) {
	std::string tmpStr = asLowerCaseString(bonusStr);
	if (tmpStr == "damagereduction") {
		return BONUS_DAMAGE_REDUCTION;
	}
	else if (tmpStr == "damageboost") {
		return BONUS_DAMAGE_BOOST;
	}
	else if (tmpStr == "bonusexperience") {
		return BONUS_XP_BONUS;
	}
	else if (tmpStr == "improvedloot") {
		return BONUS_IMPROVED_LOOT;
	}
	return BONUS_NONE;
}

void Prey::reset()
{
	timeToFreeReroll = 1200;
	preyDuration = 120;
	rerollPricePerLevel = 50;
	preyNames.clear();
	availableBonuses.clear();
}

bool Prey::loadFromXml()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/XML/prey.xml");
	if (!result) {
		printXMLError("Error - Prey::loadFromXml", "data/XML/prey.xml", result);
		return false;
	}

	pugi::xml_node preyNode = doc.child("prey");
	if (!preyNode) {
		std::cout << "[Error - Monsters::loadMonster] Missing prey node in data/XML/prey.xml" << std::endl;
		return false;
	}

	pugi::xml_node node;
	pugi::xml_attribute attr;

	if ((node = preyNode.child("configs"))) {
		for (auto configNode : node.children()) {
			if ((attr = configNode.first_attribute())) {
				const char* configName = attr.name();
				if (strcasecmp(configName, "timetofreereroll") == 0) {
					timeToFreeReroll = pugi::cast<uint16_t>(attr.value());
				}
				else if (strcasecmp(configName, "preyduration") == 0) {
					preyDuration = pugi::cast<uint16_t>(attr.value());
				}
				else if (strcasecmp(configName, "rerollpriceperlevel") == 0) {
					rerollPricePerLevel = pugi::cast<uint16_t>(attr.value());
				}
				else {
					std::cout << "[Warning - Prey::loadFromXml] Config name " << configName << " unknown." << std::endl;
				}
			}
		}
	}

	if ((node = preyNode.child("bonuses"))) {
		for (auto bonusNode : node.children()) {
			if ((attr = bonusNode.attribute("type"))) {
				BonusType type = getBonusTypeByString(attr.as_string());
				uint16_t initialValue = 0;
				uint16_t step = 0;
				if (type != BONUS_NONE) {
					if ((attr = bonusNode.attribute("initialValue"))) {
						initialValue = pugi::cast<uint16_t>(attr.value());
					}
					if ((attr = bonusNode.attribute("step"))) {
						step = pugi::cast<uint16_t>(attr.value());
					}
				}
				availableBonuses.emplace_back(type, initialValue, step);
			}
		}
	}

	if ((node = preyNode.child("monsters"))) {
		for (auto monsterNode : node.children()) {
			if ((attr = monsterNode.attribute("name"))) {
				std::string monsterName = attr.as_string();
				if (MonsterType* mType = g_monsters.getMonsterType(monsterName)) {
					preyNames.emplace_back(mType->name);
				}
				else {
					std::cout << "[Warning - Prey::loadFromXml] Monster with name " << monsterName << " not found." << std::endl;
				}
			}
		}
		std::sort(preyNames.begin(), preyNames.end());
		preyNames.erase(std::unique(preyNames.begin(), preyNames.end()), preyNames.end());
	}

	return true;
}
