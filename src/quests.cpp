/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2017  Mark Samman <mark.samman@gmail.com>
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

#include "quests.h"

#include "pugicast.h"

std::string Mission::getXmlDescription(Player* player) const
{
	int32_t value;
	player->getStorageValue(storageID, value);

	if (!mainDescription.empty()) {
		std::string desc = mainDescription;
		replaceString(desc, "|STATE|", std::to_string(value));
		replaceString(desc, "\\n", "\n");
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
	}
	else {
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

std::string Mission::getDescription(Player* player) const
{
	if (player && info.missionSetDescription != -1 && info.scriptInterface) {
		LuaScriptInterface*	scriptInterface = info.scriptInterface;
		if (!scriptInterface->reserveScriptEnv()) {
			std::cout << "[Error - Mission::missionSetDescription] Call stack overflow" << std::endl;
			return false;
		}

		ScriptEnvironment* env = scriptInterface->getScriptEnv();
		env->setScriptId(info.missionSetDescription, scriptInterface);

		lua_State* L = scriptInterface->getLuaState();
		scriptInterface->pushFunction(info.missionSetDescription);

		LuaScriptInterface::pushUserdata<const Mission>(L, this);
		LuaScriptInterface::setMetatable(L, -1, "Mission");

		LuaScriptInterface::pushUserdata<Player>(L, player);
		LuaScriptInterface::setMetatable(L, -1, "Player");

		if (scriptInterface->protectedCall(L, 2, 1) != 0) {
			LuaScriptInterface::reportError(nullptr, LuaScriptInterface::popString(L));
		} else {
			return LuaScriptInterface::popString(L);
		}
	}
	return getXmlDescription(player);
}

bool Mission::isStarted(Player* player) const
{
	if (!player) {
		return false;
	}

	if (info.missionIsStarted != -1 && info.scriptInterface) {
		LuaScriptInterface*	scriptInterface = info.scriptInterface;
		if (!scriptInterface->reserveScriptEnv()) {
			std::cout << "[Error - Mission::missionIsStarted] Call stack overflow" << std::endl;
			return false;
		}

		ScriptEnvironment* env = scriptInterface->getScriptEnv();
		env->setScriptId(info.missionIsStarted, scriptInterface);

		lua_State* L = scriptInterface->getLuaState();
		scriptInterface->pushFunction(info.missionIsStarted);

		LuaScriptInterface::pushUserdata<const Mission>(L, this);
		LuaScriptInterface::setMetatable(L, -1, "Mission");

		LuaScriptInterface::pushUserdata<Player>(L, player);
		LuaScriptInterface::setMetatable(L, -1, "Player");

		return scriptInterface->callFunction(2);
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

	if (info.missionIsCompleted != -1 && info.scriptInterface) {
		LuaScriptInterface*	scriptInterface = info.scriptInterface;
		if (!scriptInterface->reserveScriptEnv()) {
			std::cout << "[Error - Mission::isCompleted] Call stack overflow" << std::endl;
			return false;
		}

		ScriptEnvironment* env = scriptInterface->getScriptEnv();
		env->setScriptId(info.missionIsCompleted, scriptInterface);

		lua_State* L = scriptInterface->getLuaState();
		scriptInterface->pushFunction(info.missionIsCompleted);

		LuaScriptInterface::pushUserdata<const Mission>(L, this);
		LuaScriptInterface::setMetatable(L, -1, "Mission");

		LuaScriptInterface::pushUserdata<Player>(L, player);
		LuaScriptInterface::setMetatable(L, -1, "Player");

		return scriptInterface->callFunction(2);
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
	std::string missionName = name;

	if (player && info.missionSetName != -1 && info.scriptInterface) {
		LuaScriptInterface*	scriptInterface = info.scriptInterface;
		if (!scriptInterface->reserveScriptEnv()) {
			std::cout << "[Error - Mission::setName] Call stack overflow" << std::endl;
			return false;
		}

		ScriptEnvironment* env = scriptInterface->getScriptEnv();
		env->setScriptId(info.missionSetName, scriptInterface);

		lua_State* L = scriptInterface->getLuaState();
		scriptInterface->pushFunction(info.missionSetName);

		LuaScriptInterface::pushUserdata<const Mission>(L, this);
		LuaScriptInterface::setMetatable(L, -1, "Mission");

		LuaScriptInterface::pushUserdata<Player>(L, player);
		LuaScriptInterface::setMetatable(L, -1, "Player");

		if (scriptInterface->protectedCall(L, 2, 1) != 0) {
			LuaScriptInterface::reportError(nullptr, LuaScriptInterface::popString(L));
		} else {
			missionName = LuaScriptInterface::popString(L);
		}
	}

	if (isCompleted(player)) {
		missionName += " (completed)";
	}
	return missionName;
}

std::string Quest::getName(Player* player) const
{
	if (player && info.questSetName != -1 && info.scriptInterface) {
		LuaScriptInterface*	scriptInterface = info.scriptInterface;
		if (!scriptInterface->reserveScriptEnv()) {
			std::cout << "[Error - Quest::setName] Call stack overflow" << std::endl;
			return false;
		}

		ScriptEnvironment* env = scriptInterface->getScriptEnv();
		env->setScriptId(info.questSetName, scriptInterface);

		lua_State* L = scriptInterface->getLuaState();
		scriptInterface->pushFunction(info.questSetName);

		LuaScriptInterface::pushUserdata<const Quest>(L, this);
		LuaScriptInterface::setMetatable(L, -1, "Quest");

		LuaScriptInterface::pushUserdata<Player>(L, player);
		LuaScriptInterface::setMetatable(L, -1, "Player");

		if (scriptInterface->protectedCall(L, 2, 1) != 0) {
			LuaScriptInterface::reportError(nullptr, LuaScriptInterface::popString(L));
		} else {
			return LuaScriptInterface::popString(L);
		}
	}

	return name;
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
	if (!player) {
		return false;
	}

	if (info.questIsCompleted != -1 && info.scriptInterface) {
		LuaScriptInterface*	scriptInterface = info.scriptInterface;
		if (!scriptInterface->reserveScriptEnv()) {
			std::cout << "[Error - Quest::isCompleted] Call stack overflow" << std::endl;
			return false;
		}

		ScriptEnvironment* env = scriptInterface->getScriptEnv();
		env->setScriptId(info.questIsCompleted, scriptInterface);

		lua_State* L = scriptInterface->getLuaState();
		scriptInterface->pushFunction(info.questIsCompleted);
			
		LuaScriptInterface::pushUserdata<const Quest>(L, this);
		LuaScriptInterface::setMetatable(L, -1, "Quest");
		
		LuaScriptInterface::pushUserdata<Player>(L, player);
		LuaScriptInterface::setMetatable(L, -1, "Player");

		return scriptInterface->callFunction(2);
	}

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

	if (info.questIsStarted != -1 && info.scriptInterface) {
		LuaScriptInterface*	scriptInterface = info.scriptInterface;
		if (!scriptInterface->reserveScriptEnv()) {
			std::cout << "[Error - Quest::isStarted] Call stack overflow" << std::endl;
			return false;
		}

		ScriptEnvironment* env = scriptInterface->getScriptEnv();
		env->setScriptId(info.questIsStarted, scriptInterface);

		lua_State* L = scriptInterface->getLuaState();
		scriptInterface->pushFunction(info.questIsStarted);

		LuaScriptInterface::pushUserdata<const Quest>(L, this);
		LuaScriptInterface::setMetatable(L, -1, "Quest");

		LuaScriptInterface::pushUserdata<Player>(L, player);
		LuaScriptInterface::setMetatable(L, -1, "Player");

		return scriptInterface->callFunction(2);
	}

	int32_t value;
	if (!player->getStorageValue(startStorageID, value) || value < startStorageValue) {
		return false;
	}

	return true;
}

bool Quests::reload()
{
	quests.clear();
	return loadFromXml();
}

bool Quests::loadFromXml()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/quests/quests.xml");
	if (!result) {
		printXMLError("Error - Quests::loadFromXml", "data/quests/quests.xml", result);
		return false;
	}

	pugi::xml_attribute attr;
	uint16_t id = 0;
	for (auto questNode : doc.child("quests").children()) {
		quests.emplace_back(
			questNode.attribute("name").as_string(),
			++id,
			pugi::cast<int32_t>(questNode.attribute("startstorageid").value()),
			pugi::cast<int32_t>(questNode.attribute("startstoragevalue").value())
		);
		Quest& quest = quests.back();
		
		if ((attr = questNode.attribute("script"))) {
			if (!scriptInterface) {
				scriptInterface.reset(new LuaScriptInterface("Quest Interface"));
				scriptInterface->initState();
			}

			std::string scriptFile = attr.as_string();
			if (scriptInterface->loadFile("data/quests/scripts/" + scriptFile) != 0) {
				std::cout << "[Warning - Quests::loadFromXml] Can not load script: " << scriptFile << std::endl;
				std::cout << scriptInterface->getLastLuaError() << std::endl;
			} else {
				quest.info.scriptInterface = scriptInterface.get();
				quest.info.questIsStarted = scriptInterface->getMetaEvent("Quest", "isStarted");
				quest.info.questIsCompleted = scriptInterface->getMetaEvent("Quest", "isCompleted");
				quest.info.questSetName = scriptInterface->getMetaEvent("Quest", "setName");
			}
		}

		for (auto missionNode : questNode.children()) {
			std::string mainDescription = missionNode.attribute("description").as_string();

			quest.missions.emplace_back(
				missionNode.attribute("name").as_string(),
				pugi::cast<int32_t>(missionNode.attribute("storageid").value()),
				pugi::cast<int32_t>(missionNode.attribute("startvalue").value()),
				pugi::cast<int32_t>(missionNode.attribute("endvalue").value()),
				missionNode.attribute("ignoreendvalue").as_bool()
			);
			Mission& mission = quest.missions.back();

			if ((attr = missionNode.attribute("script"))) {
				if (!scriptInterface) {
					scriptInterface.reset(new LuaScriptInterface("Quest Interface"));
					scriptInterface->initState();
				}

				std::string scriptFile = attr.as_string();
				if (scriptInterface->loadFile("data/quests/scripts/" + scriptFile) != 0) {
					std::cout << "[Warning - Quests::loadFromXml] Can not load script: " << scriptFile << std::endl;
					std::cout << scriptInterface->getLastLuaError() << std::endl;
				} else {
					mission.info.scriptInterface = scriptInterface.get();
					mission.info.missionIsStarted = scriptInterface->getMetaEvent("Mission", "isStarted");
					mission.info.missionIsCompleted = scriptInterface->getMetaEvent("Mission", "isCompleted");
					mission.info.missionSetName = scriptInterface->getMetaEvent("Mission", "setName");
					mission.info.missionSetDescription = scriptInterface->getMetaEvent("Mission", "setDescription");
				}
			}

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

bool Quests::isQuestStorage(const uint32_t key, const int32_t value, const int32_t oldValue) const
{
	for (const Quest& quest : quests) {
		if (quest.getStartStorageId() == key && quest.getStartStorageValue() == value) {
			return true;
		}

		for (const Mission& mission : quest.getMissions()) {
			if (mission.getStorageId() == key && value >= mission.getStartStorageValue() && value <= mission.getEndStorageValue()) {
				return mission.mainDescription.empty() || oldValue < mission.getStartStorageValue() || oldValue > mission.getEndStorageValue();
			}
		}
	}
	return false;
}
