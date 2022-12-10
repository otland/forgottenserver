// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "quests.h"

#include "player.h"
#include "pugicast.h"

uint32_t Quests::questAutoID = 0;
uint32_t Quest::missionAutoID = 0;

std::string Mission::getDescription(Player* player) const
{
	if (callback) {
		return callback->getDescription(player);
	}

	int32_t value;
	player->getStorageValue(storageID, value);

	if (!mainDescription.empty()) {
		std::string desc = mainDescription;
		boost::algorithm::replace_all(desc, "|STATE|", std::to_string(value));
		boost::algorithm::replace_all(desc, "\\n", "\n");
		return desc;
	}

	if (ignoreEndValue) {
		for (int32_t current = endValue; current >= startValue; current--) {
			if (value >= current) {
				auto sit = descriptions.find(current);
				if (sit != descriptions.end()) {
					return sit->second;
				}
			}
		}
	} else {
		for (int32_t current = endValue; current >= startValue; current--) {
			if (value == current) {
				auto sit = descriptions.find(current);
				if (sit != descriptions.end()) {
					return sit->second;
				}
			}
		}
	}
	return "An error has occurred, please contact a gamemaster.";
}

bool Mission::isStarted(Player* player) const
{
	if (!player) {
		return false;
	}

	int32_t value;
	if (!player->getStorageValue(storageID, value)) {
		return false;
	}

	if (value < startValue) {
		return false;
	}

	if (!ignoreEndValue && value > endValue) {
		return false;
	}

	return true;
}

bool Mission::isCompleted(Player* player) const
{
	if (!player) {
		return false;
	}

	int32_t value;
	if (!player->getStorageValue(storageID, value)) {
		return false;
	}

	if (ignoreEndValue) {
		return value >= endValue;
	}

	return value == endValue;
}

std::string Mission::getName(Player* player) const
{
	if (isCompleted(player)) {
		return name + " (completed)";
	}
	return name;
}

std::string MissionCallback::getDescription(Player* player) const
{
	// getDescription(player)
	if (!scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - MissionCallback::getDescription] Call stack overflow" << std::endl;
		return std::string();
	}

	ScriptEnvironment* env = scriptInterface->getScriptEnv();
	if (!env->setCallbackId(scriptId, scriptInterface)) {
		scriptInterface->resetScriptEnv();
		return std::string();
	}

	lua_State* L = scriptInterface->getLuaState();
	scriptInterface->pushFunction(scriptId);

	LuaScriptInterface::pushUserdata<Player>(L, player);
	LuaScriptInterface::setMetatable(L, -1, "Player");

	std::string description;
	if (scriptInterface->protectedCall(L, 1, 1) != 0) {
		LuaScriptInterface::reportError(nullptr, LuaScriptInterface::popString(L));
	} else {
		description = std::move(LuaScriptInterface::getString(L, -1));
		lua_pop(L, 1);
	}

	scriptInterface->resetScriptEnv();
	return description;
}

uint16_t Quest::getMissionsCount(Player* player) const
{
	uint16_t count = 0;
	for (const Mission& mission : missions) {
		if (mission.isStarted(player)) {
			count++;
		}
	}
	return count;
}

bool Quest::isCompleted(Player* player) const
{
	for (const Mission& mission : missions) {
		if (!mission.isCompleted(player)) {
			return false;
		}
	}
	return true;
}

bool Quest::isStarted(Player* player) const
{
	if (!player) {
		return false;
	}

	int32_t value;
	if (!player->getStorageValue(startStorageID, value) || value < startStorageValue) {
		return false;
	}

	return true;
}

const Mission* Quest::getMissionById(uint16_t missionId) const
{
	auto it = std::find_if(missions.cbegin(), missions.cend(),
	                       [missionId](const Mission& mission) { return mission.id == missionId; });

	return it != missions.cend() ? &*it : nullptr;
}

bool Quest::isTracking(const uint32_t key, const int32_t value) const
{
	if (startStorageID == key && startStorageValue == value) {
		return true;
	}

	for (const Mission& mission : missions) {
		if (mission.getStorageId() == key && value >= mission.getStartStorageValue() &&
		    value <= mission.getEndStorageValue()) {
			return true;
		}
	}
	return false;
}

bool Quests::reload()
{
	clear(false);
	return loadFromXml();
}

void Quests::clear(bool fromLua)
{
	quests.remove_if([fromLua](const Quest& quest) { return quest.fromLua == fromLua; });
}

bool Quests::loadFromXml()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/XML/quests.xml");
	if (!result) {
		printXMLError("Error - Quests::loadFromXml", "data/XML/quests.xml", result);
		return false;
	}

	for (auto questNode : doc.child("quests").children()) {
		quests.emplace_back(questNode.attribute("name").as_string(), ++questAutoID,
		                    pugi::cast<int32_t>(questNode.attribute("startstorageid").value()),
		                    pugi::cast<int32_t>(questNode.attribute("startstoragevalue").value()));
		Quest& quest = quests.back();

		for (auto missionNode : questNode.children()) {
			std::string mainDescription = missionNode.attribute("description").as_string();

			quest.missions.emplace_back(missionNode.attribute("name").as_string(), ++Quest::missionAutoID,
			                            pugi::cast<int32_t>(missionNode.attribute("storageid").value()),
			                            pugi::cast<int32_t>(missionNode.attribute("startvalue").value()),
			                            pugi::cast<int32_t>(missionNode.attribute("endvalue").value()),
			                            missionNode.attribute("ignoreendvalue").as_bool());
			Mission& mission = quest.missions.back();

			if (mainDescription.empty()) {
				for (auto missionStateNode : missionNode.children()) {
					int32_t missionId = pugi::cast<int32_t>(missionStateNode.attribute("id").value());
					mission.descriptions.emplace(missionId, missionStateNode.attribute("description").as_string());
				}
			} else {
				mission.mainDescription = mainDescription;
			}
		}
	}
	return true;
}

Quest* Quests::getQuestByID(uint16_t id)
{
	for (Quest& quest : quests) {
		if (quest.id == id) {
			return &quest;
		}
	}
	return nullptr;
}

uint16_t Quests::getQuestsCount(Player* player) const
{
	uint16_t count = 0;
	for (const Quest& quest : quests) {
		if (quest.isStarted(player)) {
			count++;
		}
	}
	return count;
}

const Mission* Quests::getMissionByID(uint16_t id) const
{
	for (const Quest& quest : quests) {
		if (auto mission = quest.getMissionById(id)) {
			return mission;
		}
	}
	return nullptr;
}

bool Quests::isQuestStorage(const uint32_t key, const int32_t value, const int32_t oldValue) const
{
	for (const Quest& quest : quests) {
		if (quest.getStartStorageId() == key && quest.getStartStorageValue() == value) {
			return true;
		}

		for (const Mission& mission : quest.getMissions()) {
			if (mission.getStorageId() == key && value >= mission.getStartStorageValue() &&
			    value <= mission.getEndStorageValue()) {
				return mission.mainDescription.empty() || oldValue < mission.getStartStorageValue() ||
				       oldValue > mission.getEndStorageValue();
			}
		}
	}
	return false;
}
