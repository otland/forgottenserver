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

#include "quests.h"

#include "pugicast.h"

std::string Mission::getDescription(Player* player) const
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

bool Quests::reload()
{
	quests.clear();
	return loadFromXml();
}

bool Quests::loadFromXml()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/XML/quests.xml");
	if (!result) {
		printXMLError("Error - Quests::loadFromXml", "data/XML/quests.xml", result);
		return false;
	}

	uint16_t id = 0;
	for (auto questNode : doc.child("quests").children()) {
		quests.emplace_back(
			questNode.attribute("name").as_string(),
			++id,
			pugi::cast<int32_t>(questNode.attribute("startstorageid").value()),
			pugi::cast<int32_t>(questNode.attribute("startstoragevalue").value())
		);
		Quest& quest = quests.back();

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
