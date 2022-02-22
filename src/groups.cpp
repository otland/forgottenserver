// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "groups.h"

#include "pugicast.h"
#include "tools.h"

const std::unordered_map<std::string, PlayerFlags> ParsePlayerFlagMap = {
	{"cannotusecombat", PlayerFlag_CannotUseCombat},
	{"cannotattackplayer", PlayerFlag_CannotAttackPlayer},
	{"cannotattackmonster", PlayerFlag_CannotAttackMonster},
	{"cannotbeattacked", PlayerFlag_CannotBeAttacked},
	{"canconvinceall", PlayerFlag_CanConvinceAll},
	{"cansummonall", PlayerFlag_CanSummonAll},
	{"canillusionall", PlayerFlag_CanIllusionAll},
	{"cansenseinvisibility", PlayerFlag_CanSenseInvisibility},
	{"ignoredbymonsters", PlayerFlag_IgnoredByMonsters},
	{"notgaininfight", PlayerFlag_NotGainInFight},
	{"hasinfinitemana", PlayerFlag_HasInfiniteMana},
	{"hasinfinitesoul", PlayerFlag_HasInfiniteSoul},
	{"hasnoexhaustion", PlayerFlag_HasNoExhaustion},
	{"cannotusespells", PlayerFlag_CannotUseSpells},
	{"cannotpickupitem", PlayerFlag_CannotPickupItem},
	{"canalwayslogin", PlayerFlag_CanAlwaysLogin},
	{"canbroadcast", PlayerFlag_CanBroadcast},
	{"canedithouses", PlayerFlag_CanEditHouses},
	{"cannotbebanned", PlayerFlag_CannotBeBanned},
	{"cannotbepushed", PlayerFlag_CannotBePushed},
	{"hasinfinitecapacity", PlayerFlag_HasInfiniteCapacity},
	{"canpushallcreatures", PlayerFlag_CanPushAllCreatures},
	{"cantalkredprivate", PlayerFlag_CanTalkRedPrivate},
	{"cantalkredchannel", PlayerFlag_CanTalkRedChannel},
	{"talkorangehelpchannel", PlayerFlag_TalkOrangeHelpChannel},
	{"notgainexperience", PlayerFlag_NotGainExperience},
	{"notgainmana", PlayerFlag_NotGainMana},
	{"notgainhealth", PlayerFlag_NotGainHealth},
	{"notgainskill", PlayerFlag_NotGainSkill},
	{"setmaxspeed", PlayerFlag_SetMaxSpeed},
	{"specialvip", PlayerFlag_SpecialVIP},
	{"notgenerateloot", PlayerFlag_NotGenerateLoot},
	{"ignoreprotectionzone", PlayerFlag_IgnoreProtectionZone},
	{"ignorespellcheck", PlayerFlag_IgnoreSpellCheck},
	{"ignoreweaponcheck", PlayerFlag_IgnoreWeaponCheck},
	{"cannotbemuted", PlayerFlag_CannotBeMuted},
	{"isalwayspremium", PlayerFlag_IsAlwaysPremium}
};

bool Groups::load()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/XML/groups.xml");
	if (!result) {
		printXMLError("Error - Groups::load", "data/XML/groups.xml", result);
		return false;
	}

	for (auto groupNode : doc.child("groups").children()) {
		Group group;
		group.id = pugi::cast<uint32_t>(groupNode.attribute("id").value());
		group.name = groupNode.attribute("name").as_string();
		group.access = groupNode.attribute("access").as_bool();
		group.maxDepotItems = pugi::cast<uint32_t>(groupNode.attribute("maxdepotitems").value());
		group.maxVipEntries = pugi::cast<uint32_t>(groupNode.attribute("maxvipentries").value());
		group.flags = pugi::cast<uint64_t>(groupNode.attribute("flags").value());
		if (pugi::xml_node node = groupNode.child("flags")) {
			for (auto flagNode : node.children()) {
				pugi::xml_attribute attr = flagNode.first_attribute();
				if (!attr || !attr.as_bool()) {
					continue;
				}

				auto parseFlag = ParsePlayerFlagMap.find(attr.name());
				if (parseFlag != ParsePlayerFlagMap.end()) {
					group.flags |= parseFlag->second;
				}
			}
		}

		groups.push_back(group);
	}
	return true;
}

Group* Groups::getGroup(uint16_t id)
{
	for (Group& group : groups) {
		if (group.id == id) {
			return &group;
		}
	}
	return nullptr;
}
