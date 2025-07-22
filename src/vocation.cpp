// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "vocation.h"

#include "player.h"
#include "pugicast.h"
#include "tools.h"

bool Vocations::loadFromXml(std::istream& is, std::string_view filename)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load(is);
	if (!result) {
		printXMLError("Error - Vocations::loadFromXml", filename, result);
		return false;
	}

	for (auto vocationNode : doc.child("vocations").children()) {
		pugi::xml_attribute attr = vocationNode.attribute("id");
		if (!attr) {
			std::cout << "[Warning - Vocations::loadFromXml] Missing vocation id" << std::endl;
			continue;
		}

		uint16_t id = pugi::cast<uint16_t>(attr.value());
		auto res = vocationsMap.emplace(std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(id));
		Vocation& voc = res.first->second;

		vocationNode.remove_attribute("id");
		for (auto attrNode : vocationNode.attributes()) {
			const char* attrName = attrNode.name();
			if (boost::iequals(attrName, "name")) {
				voc.name = attrNode.as_string();
			} else if (boost::iequals(attrName, "allowpvp")) {
				voc.allowPvp = attrNode.as_bool();
			} else if (boost::iequals(attrName, "clientid")) {
				voc.clientId = pugi::cast<uint16_t>(attrNode.value());
			} else if (boost::iequals(attrName, "description")) {
				voc.description = attrNode.as_string();
			} else if (boost::iequals(attrName, "magicshield")) {
				voc.magicShield = attrNode.as_bool();
			} else if (boost::iequals(attrName, "gaincap")) {
				voc.gainCap = pugi::cast<uint32_t>(attrNode.value()) * 100;
			} else if (boost::iequals(attrName, "gainhp")) {
				voc.gainHP = pugi::cast<uint32_t>(attrNode.value());
			} else if (boost::iequals(attrName, "gainmana")) {
				voc.gainMana = pugi::cast<uint32_t>(attrNode.value());
			} else if (boost::iequals(attrName, "gainhpticks")) {
				voc.gainHealthTicks = pugi::cast<uint32_t>(attrNode.value());
			} else if (boost::iequals(attrName, "gainhpamount")) {
				voc.gainHealthAmount = pugi::cast<uint32_t>(attrNode.value());
			} else if (boost::iequals(attrName, "gainmanaticks")) {
				voc.gainManaTicks = pugi::cast<uint32_t>(attrNode.value());
			} else if (boost::iequals(attrName, "gainmanaamount")) {
				voc.gainManaAmount = pugi::cast<uint32_t>(attrNode.value());
			} else if (boost::iequals(attrName, "manamultiplier")) {
				voc.manaMultiplier = pugi::cast<float>(attrNode.value());
			} else if (boost::iequals(attrName, "attackspeed")) {
				voc.attackSpeed = pugi::cast<uint32_t>(attrNode.value());
			} else if (boost::iequals(attrName, "basespeed")) {
				voc.baseSpeed = pugi::cast<uint32_t>(attrNode.value());
			} else if (boost::iequals(attrName, "soulmax")) {
				voc.soulMax = pugi::cast<uint16_t>(attrNode.value());
			} else if (boost::iequals(attrName, "gainsoulticks")) {
				voc.gainSoulTicks = pugi::cast<uint16_t>(attrNode.value());
			} else if (boost::iequals(attrName, "fromvoc")) {
				voc.fromVocation = pugi::cast<uint32_t>(attrNode.value());
			} else if (boost::iequals(attrName, "nopongkicktime")) {
				voc.noPongKickTime = pugi::cast<uint32_t>(attrNode.value()) * 1000;
			} else {
				std::cout << "[Notice - Vocations::loadFromXml] Unknown attribute: \"" << attrName
				          << "\" for vocation: " << voc.id << std::endl;
			}
		}

		for (auto childNode : vocationNode.children()) {
			if (boost::iequals(childNode.name(), "skill")) {
				if ((attr = childNode.attribute("id"))) {
					uint16_t skillId = pugi::cast<uint16_t>(attr.value());
					if (skillId <= SKILL_LAST) {
						voc.skillMultipliers[skillId] = pugi::cast<double>(childNode.attribute("multiplier").value());
					} else {
						std::cout << "[Notice - Vocations::loadFromXml] No valid skill id: " << skillId
						          << " for vocation: " << voc.id << std::endl;
					}
				} else {
					std::cout << "[Notice - Vocations::loadFromXml] Missing skill id for vocation: " << voc.id
					          << std::endl;
				}
			} else if (boost::iequals(childNode.name(), "formula")) {
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

int32_t Vocations::getVocationId(std::string_view name) const
{
	auto it = std::find_if(vocationsMap.begin(), vocationsMap.end(),
	                       [=](auto it) { return boost::iequals(name, it.second.name); });
	return it != vocationsMap.end() ? it->first : -1;
}

uint16_t Vocations::getPromotedVocation(uint16_t id) const
{
	auto it = std::find_if(vocationsMap.begin(), vocationsMap.end(),
	                       [id](auto it) { return it.second.fromVocation == id && it.first != id; });
	return it != vocationsMap.end() ? it->first : VOCATION_NONE;
}

static const uint32_t skillBase[SKILL_LAST + 1] = {50, 50, 50, 50, 30, 100, 20};

uint64_t Vocation::getReqSkillTries(uint8_t skill, uint16_t level)
{
	if (skill > SKILL_LAST) {
		return 0;
	}
	return skillBase[skill] *
	       std::pow(skillMultipliers[skill], static_cast<int32_t>(level - (MINIMUM_SKILL_LEVEL + 1)));
}

uint64_t Vocation::getReqMana(uint32_t magLevel)
{
	if (magLevel == 0) {
		return 0;
	}
	return 1600 * std::pow(manaMultiplier, static_cast<int32_t>(magLevel - 1));
}
