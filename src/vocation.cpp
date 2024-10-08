// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "vocation.h"

#include "player.h"
#include "pugicast.h"
#include "tools.h"

namespace {

uint32_t skillBase[SKILL_LAST + 1] = {50, 50, 50, 50, 30, 100, 20};

} // namespace

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

bool tfs::game::vocations::load_from_xml(bool reload)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/XML/vocations.xml");
	if (!result) {
		printXMLError("Error - tfs::game::vocations::load_from_xml", "data/XML/vocations.xml", result);
		return false;
	}

	for (auto& node : doc.child("vocations").children()) {
		pugi::xml_attribute attr;
		if (!(attr = node.attribute("id"))) {
			std::cout << "[Warning - tfs::game::vocations::load_from_xml] Missing vocation id" << std::endl;
			continue;
		}

		auto vocation_id = pugi::cast<uint16_t>(attr.value());

		Vocation_ptr vocation = nullptr;
		if (reload) {
			vocation = tfs::game::vocations::find_by_id(vocation_id);
		}

		if (!vocation) {
			vocation = std::make_shared<Vocation>(vocation_id);
			loaded_vocations.emplace(vocation);
		}

		node.remove_attribute("id");

		for (auto& attr_node : node.attributes()) {
			auto attrName = attr_node.name();
			if (caseInsensitiveEqual(attrName, "name")) {
				vocation->name = attr_node.as_string();
			} else if (caseInsensitiveEqual(attrName, "allowpvp")) {
				vocation->allowPvp = attr_node.as_bool();
			} else if (caseInsensitiveEqual(attrName, "clientid")) {
				vocation->clientId = pugi::cast<uint16_t>(attr_node.value());
			} else if (caseInsensitiveEqual(attrName, "description")) {
				vocation->description = attr_node.as_string();
			} else if (caseInsensitiveEqual(attrName, "magicshield")) {
				vocation->magicShield = attr_node.as_bool();
			} else if (caseInsensitiveEqual(attrName, "gaincap")) {
				vocation->gainCap = pugi::cast<uint32_t>(attr_node.value()) * 100;
			} else if (caseInsensitiveEqual(attrName, "gainhp")) {
				vocation->gainHP = pugi::cast<uint32_t>(attr_node.value());
			} else if (caseInsensitiveEqual(attrName, "gainmana")) {
				vocation->gainMana = pugi::cast<uint32_t>(attr_node.value());
			} else if (caseInsensitiveEqual(attrName, "gainhpticks")) {
				vocation->gainHealthTicks = pugi::cast<uint32_t>(attr_node.value());
			} else if (caseInsensitiveEqual(attrName, "gainhpamount")) {
				vocation->gainHealthAmount = pugi::cast<uint32_t>(attr_node.value());
			} else if (caseInsensitiveEqual(attrName, "gainmanaticks")) {
				vocation->gainManaTicks = pugi::cast<uint32_t>(attr_node.value());
			} else if (caseInsensitiveEqual(attrName, "gainmanaamount")) {
				vocation->gainManaAmount = pugi::cast<uint32_t>(attr_node.value());
			} else if (caseInsensitiveEqual(attrName, "manamultiplier")) {
				vocation->manaMultiplier = pugi::cast<float>(attr_node.value());
			} else if (caseInsensitiveEqual(attrName, "attackspeed")) {
				vocation->attackSpeed = pugi::cast<uint32_t>(attr_node.value());
			} else if (caseInsensitiveEqual(attrName, "basespeed")) {
				vocation->baseSpeed = pugi::cast<uint32_t>(attr_node.value());
			} else if (caseInsensitiveEqual(attrName, "soulmax")) {
				vocation->soulMax = pugi::cast<uint16_t>(attr_node.value());
			} else if (caseInsensitiveEqual(attrName, "gainsoulticks")) {
				vocation->gainSoulTicks = pugi::cast<uint16_t>(attr_node.value());
			} else if (caseInsensitiveEqual(attrName, "fromvoc")) {
				vocation->fromVocation = pugi::cast<uint32_t>(attr_node.value());
			} else if (caseInsensitiveEqual(attrName, "nopongkicktime")) {
				vocation->noPongKickTime = pugi::cast<uint32_t>(attr_node.value()) * 1000;
			} else {
				std::cout << "[Notice - tfs::game::vocations::load_from_xml] Unknown attribute: \"" << attrName
				          << "\" for vocation: " << vocation->id << std::endl;
			}
		}

		for (auto& child_node : node.children()) {
			if (caseInsensitiveEqual(child_node.name(), "skill")) {
				if (!(attr = child_node.attribute("id"))) {
					std::cout << "[Notice - tfs::game::vocations::load_from_xml] Missing skill id for vocation: "
					          << vocation->id << std::endl;
					continue;
				}

				auto skillId = pugi::cast<uint16_t>(attr.value());
				if (skillId > SKILL_LAST) {
					std::cout << "[Notice - tfs::game::vocations::load_from_xml] No valid skill id: " << skillId
					          << " for vocation: " << vocation->id << std::endl;
					continue;
				}
				vocation->skillMultipliers[skillId] = pugi::cast<double>(child_node.attribute("multiplier").value());
			} else if (caseInsensitiveEqual(child_node.name(), "formula")) {
				if ((attr = child_node.attribute("meleeDamage"))) {
					vocation->meleeDamageMultiplier = pugi::cast<float>(attr.value());
				}

				if ((attr = child_node.attribute("distDamage"))) {
					vocation->distDamageMultiplier = pugi::cast<float>(attr.value());
				}

				if ((attr = child_node.attribute("defense"))) {
					vocation->defenseMultiplier = pugi::cast<float>(attr.value());
				}

				if ((attr = child_node.attribute("armor"))) {
					vocation->armorMultiplier = pugi::cast<float>(attr.value());
				}
			}
		}
	}
	return true;
}

Vocation_ptr tfs::game::vocations::find_by_id(uint16_t id)
{
	auto it = std::find_if(loaded_vocations.begin(), loaded_vocations.end(), [id](auto it) { return it->id == id; });
	if (it == loaded_vocations.end()) {
		return nullptr;
	}
	return *it;
}

Vocation_ptr tfs::game::vocations::find_by_name(std::string_view name)
{
	auto it = std::find_if(loaded_vocations.begin(), loaded_vocations.end(),
	                       [=](auto it) { return caseInsensitiveEqual(name, it->name); });

	if (it == loaded_vocations.end()) {
		return nullptr;
	}
	return *it;
}

Vocation_ptr tfs::game::vocations::find_by_promoted_id(uint16_t id)
{
	auto it = std::find_if(loaded_vocations.begin(), loaded_vocations.end(),
	                       [id](auto it) { return it->fromVocation == id && it->id != id; });

	if (it == loaded_vocations.end()) {
		return nullptr;
	}
	return *it;
}

const std::set<Vocation_ptr>& tfs::game::vocations::find_vocations() { return loaded_vocations; }
