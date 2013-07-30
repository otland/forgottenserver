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

#include "definitions.h"
#include "vocation.h"
#include <iostream>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

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
	std::string filename = "data/XML/vocations.xml";

	xmlDocPtr doc = xmlParseFile(filename.c_str());

	if (doc) {
		xmlNodePtr root, p;
		root = xmlDocGetRootElement(doc);

		if (xmlStrcmp(root->name, (const xmlChar*)"vocations") != 0) {
			xmlFreeDoc(doc);
			return false;
		}

		p = root->children;

		while (p) {
			std::string str;
			int32_t intVal;

			if (xmlStrcmp(p->name, (const xmlChar*)"vocation") == 0) {
				Vocation* voc = new Vocation();
				xmlNodePtr configNode;

				if (readXMLInteger(p, "id", intVal)) {
					float floatVal;

					voc->id = intVal;

					if (readXMLString(p, "name", str)) {
						voc->name = str;
					}

					if (readXMLInteger(p, "clientid", intVal)) {
						voc->clientId = intVal;
					}

					if (readXMLString(p, "description", str)) {
						voc->description = str;
					}

					if (readXMLInteger(p, "gaincap", intVal)) {
						voc->gainCap = intVal;
					}

					if (readXMLInteger(p, "gainhp", intVal)) {
						voc->gainHP = intVal;
					}

					if (readXMLInteger(p, "gainmana", intVal)) {
						voc->gainMana = intVal;
					}

					if (readXMLInteger(p, "gainhpticks", intVal)) {
						voc->gainHealthTicks = intVal;
					}

					if (readXMLInteger(p, "gainhpamount", intVal)) {
						voc->gainHealthAmount = intVal;
					}

					if (readXMLInteger(p, "gainmanaticks", intVal)) {
						voc->gainManaTicks = intVal;
					}

					if (readXMLInteger(p, "gainmanaamount", intVal)) {
						voc->gainManaAmount = intVal;
					}

					if (readXMLFloat(p, "manamultiplier", floatVal)) {
						voc->manaMultiplier = floatVal;
					}

					if (readXMLInteger(p, "attackspeed", intVal)) {
						voc->attackSpeed = intVal;
					}

					if (readXMLInteger(p, "basespeed", intVal)) {
						voc->baseSpeed = intVal;
					}

					if (readXMLInteger(p, "soulmax", intVal)) {
						voc->soulMax = intVal;
					}

					if (readXMLInteger(p, "gainsoulticks", intVal)) {
						voc->gainSoulTicks = intVal;
					}

					if (readXMLInteger(p, "fromvoc", intVal)) {
						voc->fromVocation = intVal;
					}

					configNode = p->children;

					while (configNode) {
						if (xmlStrcmp(configNode->name, (const xmlChar*)"skill") == 0) {
							uint32_t skill_id;

							if (readXMLInteger(configNode, "id", intVal)) {
								skill_id = intVal;

								if (skill_id > SKILL_LAST) {
									std::cout << "No valid skill id. " << skill_id << std::endl;
								} else {
									if (readXMLFloat(configNode, "multiplier", floatVal)) {
										voc->skillMultipliers[skill_id] = floatVal;
									}
								}
							} else {
								std::cout << "Missing skill id." << std::endl;
							}
						} else if (xmlStrcmp(configNode->name, (const xmlChar*)"formula") == 0) {
							if (readXMLFloat(configNode, "meleeDamage", floatVal)) {
								voc->meleeDamageMultipler = floatVal;
							}

							if (readXMLFloat(configNode, "distDamage", floatVal)) {
								voc->distDamageMultipler = floatVal;
							}

							if (readXMLFloat(configNode, "defense", floatVal)) {
								voc->defenseMultipler = floatVal;
							}

							if (readXMLFloat(configNode, "armor", floatVal)) {
								voc->armorMultipler = floatVal;
							}
						}

						configNode = configNode->next;
					}

					vocationsMap[voc->id] = voc;
				} else {
					std::cout << "Missing vocation id." << std::endl;
				}
			}

			p = p->next;
		}

		xmlFreeDoc(doc);
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
	meleeDamageMultipler = 1.0;
	distDamageMultipler = 1.0;
	defenseMultipler = 1.0;
	armorMultipler = 1.0;
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
