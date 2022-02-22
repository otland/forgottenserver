// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "vocation.h"

#include "player.h"
#include "pugicast.h"
#include "tools.h"

bool Vocations::loadFromXml()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/XML/vocations.xml");
	if (!result) {
		printXMLError("Error - Vocations::loadFromXml", "data/XML/vocations.xml", result);
		return false;
	}

	for (auto vocationNode : doc.child("vocations").children()) {
		pugi::xml_attribute attr = vocationNode.attribute("id");
		if (!attr) {
			std::cout << "[Warning - Vocations::loadFromXml] Missing vocation id" << std::endl;
			continue;
		}

		uint16_t id = pugi::cast<uint16_t>(attr.value());
		auto res = vocationsMap.emplace(std::piecewise_construct,
				std::forward_as_tuple(id), std::forward_as_tuple(id));
		Vocation& voc = res.first->second;

		vocationNode.remove_attribute("id");
		for (auto attrNode : vocationNode.attributes()) {
			const char* attrName = attrNode.name();
			if (strcasecmp(attrName, "name") == 0) {
				voc.name = attrNode.as_string();
			} else if (strcasecmp(attrName, "allowpvp") == 0) {
				voc.allowPvp = attrNode.as_bool();
			} else if (strcasecmp(attrName, "clientid") == 0) {
				voc.clientId = pugi::cast<uint16_t>(attrNode.value());
			} else if (strcasecmp(attrName, "description") == 0) {
				voc.description = attrNode.as_string();
			} else if (strcasecmp(attrName, "gaincap") == 0) {
				voc.gainCap = pugi::cast<uint32_t>(attrNode.value()) * 100;
			} else if (strcasecmp(attrName, "gainhp") == 0) {
				voc.gainHP = pugi::cast<uint32_t>(attrNode.value());
			} else if (strcasecmp(attrName, "gainmana") == 0) {
				voc.gainMana = pugi::cast<uint32_t>(attrNode.value());
			} else if (strcasecmp(attrName, "gainhpticks") == 0) {
				voc.gainHealthTicks = pugi::cast<uint32_t>(attrNode.value());
			} else if (strcasecmp(attrName, "gainhpamount") == 0) {
				voc.gainHealthAmount = pugi::cast<uint32_t>(attrNode.value());
			} else if (strcasecmp(attrName, "gainmanaticks") == 0) {
				voc.gainManaTicks = pugi::cast<uint32_t>(attrNode.value());
			} else if (strcasecmp(attrName, "gainmanaamount") == 0) {
				voc.gainManaAmount = pugi::cast<uint32_t>(attrNode.value());
			} else if (strcasecmp(attrName, "manamultiplier") == 0) {
				voc.manaMultiplier = pugi::cast<float>(attrNode.value());
			} else if (strcasecmp(attrName, "attackspeed") == 0) {
				voc.attackSpeed = pugi::cast<uint32_t>(attrNode.value());
			} else if (strcasecmp(attrName, "basespeed") == 0) {
				voc.baseSpeed = pugi::cast<uint32_t>(attrNode.value());
			} else if (strcasecmp(attrName, "soulmax") == 0) {
				voc.soulMax = pugi::cast<uint16_t>(attrNode.value());
			} else if (strcasecmp(attrName, "gainsoulticks") == 0) {
				voc.gainSoulTicks = pugi::cast<uint16_t>(attrNode.value());
			} else if (strcasecmp(attrName, "fromvoc") == 0) {
				voc.fromVocation = pugi::cast<uint32_t>(attrNode.value());
			} else if (strcasecmp(attrName, "nopongkicktime") == 0) {
				voc.noPongKickTime = pugi::cast<uint32_t>(attrNode.value()) * 1000;
			} else {
				std::cout << "[Notice - Vocations::loadFromXml] Unknown attribute: \"" << attrName << "\" for vocation: " << voc.id << std::endl;
			}
		}

		for (auto childNode : vocationNode.children()) {
			if (strcasecmp(childNode.name(), "skill") == 0) {
				if ((attr = childNode.attribute("id"))) {
					uint16_t skillId = pugi::cast<uint16_t>(attr.value());
					if (skillId <= SKILL_LAST) {
						voc.skillMultipliers[skillId] = pugi::cast<double>(childNode.attribute("multiplier").value());
					} else {
						std::cout << "[Notice - Vocations::loadFromXml] No valid skill id: " << skillId << " for vocation: " << voc.id << std::endl;
					}
				} else {
					std::cout << "[Notice - Vocations::loadFromXml] Missing skill id for vocation: " << voc.id << std::endl;
				}
			} else if (strcasecmp(childNode.name(), "formula") == 0) {
				if ((attr = childNode.attribute("meleeDamage"))) {
					voc.meleeDamageMultiplier = pugi::cast<float>(attr.value());
				}

				if ((attr = childNode.attribute("distDamage"))) {
					voc.distDamageMultiplier = pugi::cast<float>(attr.value());
				}

				if ((attr = childNode.attribute("defense"))) {
					voc.defenseMultiplier = pugi::cast<float>(attr.value());
				}

				if ((attr = childNode.attribute("armor"))) {
					voc.armorMultiplier = pugi::cast<float>(attr.value());
				}
			}
		}
	}
	return true;
}

Vocation* Vocations::getVocation(uint16_t id)
{
	auto it = vocationsMap.find(id);
	if (it == vocationsMap.end()) {
		std::cout << "[Warning - Vocations::getVocation] Vocation " << id << " not found." << std::endl;
		return nullptr;
	}
	return &it->second;
}

int32_t Vocations::getVocationId(const std::string& name) const
{
	auto it = std::find_if(vocationsMap.begin(), vocationsMap.end(), [&name](auto it) {
		return name.size() == it.second.name.size() && std::equal(name.begin(), name.end(), it.second.name.begin(), [](char a, char b) {
			return std::tolower(a) == std::tolower(b);
		});
	});
	return it != vocationsMap.end() ? it->first : -1;
}

uint16_t Vocations::getPromotedVocation(uint16_t id) const
{
	auto it = std::find_if(vocationsMap.begin(), vocationsMap.end(), [id](auto it) {
		return it.second.fromVocation == id && it.first != id;
	});
	return it != vocationsMap.end() ? it->first : VOCATION_NONE;
}

static const uint32_t skillBase[SKILL_LAST + 1] = {50, 50, 50, 50, 30, 100, 20};

uint64_t Vocation::getReqSkillTries(uint8_t skill, uint16_t level)
{
	if (skill > SKILL_LAST) {
		return 0;
	}
	return skillBase[skill] * std::pow(skillMultipliers[skill], static_cast<int32_t>(level - (MINIMUM_SKILL_LEVEL + 1)));
}

uint64_t Vocation::getReqMana(uint32_t magLevel)
{
	if (magLevel == 0) {
		return 0;
	}
	return 1600 * std::pow(manaMultiplier, static_cast<int32_t>(magLevel - 1));
}
