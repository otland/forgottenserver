/**
 * The Forgotten Server - a server application for the MMORPG Tibia
 * Copyright (C) 2013  Mark Samman <mark.samman@gmail.com>
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

#include "definitions.h"
#include "vocation.h"
#include <iostream>

#include "pugixml.hpp"
#include "pugicast.h"

#include "tools.h"

Vocations::Vocations()
{
	//
}

Vocations::~Vocations()
{
	for (VocationsMap::iterator it = vocationsMap.begin(); it != vocationsMap.end(); ++it) {
		delete it->second;
	}

	vocationsMap.clear();
}

bool Vocations::loadFromXml()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/XML/vocations.xml");
	if (!result) {
		std::cout << "[Error - Vocations::loadFromXml] Failed to load data/XML/vocations.xml: " << result.description() << std::endl;
		return false;
	}

	for (pugi::xml_node vocationNode = doc.child("vocations").first_child(); vocationNode; vocationNode = vocationNode.next_sibling()) {
		Vocation* voc = new Vocation();

		pugi::xml_attribute idAttribute = vocationNode.attribute("id");
		if (!idAttribute) {
			std::cout << "[Warning - Vocations::loadFromXml] Missing vocation id" << std::endl;
			continue;
		}

		voc->id = pugi::cast<uint32_t>(idAttribute.value());

		pugi::xml_attribute nameAttribute = vocationNode.attribute("name");
		if (nameAttribute) {
			voc->name = nameAttribute.as_string();
		}

		pugi::xml_attribute clientIdAttribute = vocationNode.attribute("clientid");
		if (clientIdAttribute) {
			voc->clientId = pugi::cast<uint16_t>(clientIdAttribute.value());
		}

		pugi::xml_attribute descriptionAttribute = vocationNode.attribute("description");
		if (descriptionAttribute) {
			voc->description = descriptionAttribute.as_string();
		}

		pugi::xml_attribute gainCapAttribute = vocationNode.attribute("gaincap");
		if (gainCapAttribute) {
			voc->gainCap = pugi::cast<uint32_t>(gainCapAttribute.value());
		}

		pugi::xml_attribute gainHpAttribute = vocationNode.attribute("gainhp");
		if (gainHpAttribute) {
			voc->gainHP = pugi::cast<uint32_t>(gainHpAttribute.value());
		}

		pugi::xml_attribute gainManaAttribute = vocationNode.attribute("gainmana");
		if (gainManaAttribute) {
			voc->gainMana = pugi::cast<uint32_t>(gainManaAttribute.value());
		}

		pugi::xml_attribute gainHpTicksAttribute = vocationNode.attribute("gainhpticks");
		if (gainHpTicksAttribute) {
			voc->gainHealthTicks = pugi::cast<uint32_t>(gainHpTicksAttribute.value());
		}

		pugi::xml_attribute gainHpAmountAttribute = vocationNode.attribute("gainhpamount");
		if (gainHpAmountAttribute) {
			voc->gainHealthAmount = pugi::cast<uint32_t>(gainHpAmountAttribute.value());
		}

		pugi::xml_attribute gainManaTicksAttribute = vocationNode.attribute("gainmanaticks");
		if (gainManaTicksAttribute) {
			voc->gainManaTicks = pugi::cast<uint32_t>(gainManaTicksAttribute.value());
		}

		pugi::xml_attribute gainManaAmountAttribute = vocationNode.attribute("gainmanaamount");
		if (gainManaAmountAttribute) {
			voc->gainManaAmount = pugi::cast<uint32_t>(gainManaAmountAttribute.value());
		}

		pugi::xml_attribute manaMultiplierAttribute = vocationNode.attribute("manamultiplier");
		if (manaMultiplierAttribute) {
			voc->manaMultiplier = pugi::cast<float>(manaMultiplierAttribute.value());
		}

		pugi::xml_attribute attackSpeedAttribute = vocationNode.attribute("attackspeed");
		if (attackSpeedAttribute) {
			voc->attackSpeed = pugi::cast<uint32_t>(attackSpeedAttribute.value());
		}

		pugi::xml_attribute baseSpeedAttribute = vocationNode.attribute("basespeed");
		if (baseSpeedAttribute) {
			voc->baseSpeed = pugi::cast<uint32_t>(baseSpeedAttribute.value());
		}

		pugi::xml_attribute soulMaxAttribute = vocationNode.attribute("soulmax");
		if (soulMaxAttribute) {
			voc->soulMax = pugi::cast<uint16_t>(soulMaxAttribute.value());
		}

		pugi::xml_attribute gainSoulTicksAttribute = vocationNode.attribute("gainsoulticks");
		if (gainSoulTicksAttribute) {
			voc->gainSoulTicks = pugi::cast<uint16_t>(gainSoulTicksAttribute.value());
		}

		pugi::xml_attribute fromVocAttribute = vocationNode.attribute("fromvoc");
		if (fromVocAttribute) {
			voc->fromVocation = pugi::cast<uint32_t>(fromVocAttribute.value());
		}

		for (pugi::xml_node childNode = vocationNode.first_child(); childNode; childNode = childNode.next_sibling()) {
			if (strcasecmp(childNode.name(), "skill") == 0) {
				pugi::xml_attribute skillIdAttribute = childNode.attribute("id");
				if (skillIdAttribute) {
					uint16_t skill_id = pugi::cast<uint16_t>(skillIdAttribute.value());
					if (skill_id <= SKILL_LAST) {
						voc->skillMultipliers[skill_id] = pugi::cast<float>(childNode.attribute("multiplier").value());
					} else {
						std::cout << "[Notice - Vocations::loadFromXml] No valid skill id: " << skill_id << " for vocation: " << voc->id << std::endl;
					}
				} else {
					std::cout << "[Notice - Vocations::loadFromXml] Missing skill id for vocation: " << voc->id << std::endl;
				}
			} else if (strcasecmp(childNode.name(), "formula") == 0) {
				pugi::xml_attribute meleeDamageAttribute = childNode.attribute("meleeDamage");
				if (meleeDamageAttribute) {
					voc->meleeDamageMultiplier = pugi::cast<float>(meleeDamageAttribute.value());
				}

				pugi::xml_attribute distDamageAttribute = childNode.attribute("distDamage");
				if (distDamageAttribute) {
					voc->distDamageMultiplier = pugi::cast<float>(distDamageAttribute.value());
				}

				pugi::xml_attribute defenseAttribute = childNode.attribute("defense");
				if (defenseAttribute) {
					voc->defenseMultiplier = pugi::cast<float>(defenseAttribute.value());
				}

				pugi::xml_attribute armorAttribute = childNode.attribute("armor");
				if (armorAttribute) {
					voc->armorMultiplier = pugi::cast<float>(armorAttribute.value());
				}
			}
		}
		vocationsMap[voc->id] = voc;
	}
	return true;
}

Vocation* Vocations::getVocation(uint32_t id)
{
	VocationsMap::iterator it = vocationsMap.find(id);
	if (it == vocationsMap.end()) {
		std::cout << "Warning: [Vocations::getVocation] Vocation " << id << " not found." << std::endl;
		return &def_voc;
	}
	return it->second;
}

int32_t Vocations::getVocationId(const std::string& name)
{
	for (VocationsMap::iterator it = vocationsMap.begin(); it != vocationsMap.end(); ++it) {
		if (strcasecmp(it->second->name.c_str(), name.c_str()) == 0) {
			return it->first;
		}
	}
	return -1;
}

int32_t Vocations::getPromotedVocation(uint32_t vocationId)
{
	for (VocationsMap::iterator it = vocationsMap.begin(); it != vocationsMap.end(); ++it) {
		if (it->second->fromVocation == vocationId && it->first != vocationId) {
			return it->first;
		}
	}

	return 0;
}

uint32_t Vocation::skillBase[SKILL_LAST + 1] = {50, 50, 50, 50, 30, 100, 20};

Vocation::Vocation()
{
	name = "none";
	description = "";
	gainHealthTicks = 6;
	gainHealthAmount = 1;
	gainManaTicks = 6;
	gainManaAmount = 1;
	gainSoulTicks = 120;
	soulMax = 100;

	clientId = 0;
	fromVocation = 0;

	gainCap = 5;
	gainMana = 5;
	gainHP = 5;
	attackSpeed = 1500;
	baseSpeed = 220;
	manaMultiplier = 4.0;
	meleeDamageMultiplier = 1.0;
	distDamageMultiplier = 1.0;
	defenseMultiplier = 1.0;
	armorMultiplier = 1.0;
	skillMultipliers[0] = 1.5f;
	skillMultipliers[1] = 2.0f;
	skillMultipliers[2] = 2.0f;
	skillMultipliers[3] = 2.0f;
	skillMultipliers[4] = 2.0f;
	skillMultipliers[5] = 1.5f;
	skillMultipliers[6] = 1.1f;
}

Vocation::~Vocation()
{
	cacheMana.clear();

	for (int32_t i = SKILL_FIRST; i < SKILL_LAST; ++i) {
		cacheSkill[i].clear();
	}
}

uint32_t Vocation::getReqSkillTries(int32_t skill, int32_t level)
{
	if (skill < SKILL_FIRST || skill > SKILL_LAST) {
		return 0;
	}

	skillCacheMap& skillMap = cacheSkill[skill];
	skillCacheMap::iterator it = skillMap.find(level);

	if (it != cacheSkill[skill].end()) {
		return it->second;
	}

	uint32_t tries = (uint32_t)(skillBase[skill] * pow((float)skillMultipliers[skill], (float)(level - 11)));
	skillMap[level] = tries;
	return tries;
}

uint64_t Vocation::getReqMana(uint32_t magLevel)
{
	manaCacheMap::iterator it = cacheMana.find(magLevel);

	if (it != cacheMana.end()) {
		return it->second;
	}

	uint64_t reqMana = (uint64_t)(400 * pow(manaMultiplier, (int) magLevel - 1));

	if (reqMana % 20 < 10) {
		reqMana = reqMana - (reqMana % 20);
	} else {
		reqMana = reqMana - (reqMana % 20) + 20;
	}

	cacheMana[magLevel] = reqMana;
	return reqMana;
}
