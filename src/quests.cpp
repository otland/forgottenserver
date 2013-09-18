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

#include <sstream>

#include "quests.h"
#include "tools.h"

#include "pugixml.hpp"
#include "pugicast.h"

MissionState::MissionState(const std::string& _description, int32_t _missionID)
{
	description = _description;
	missionID = _missionID;
}

Mission::Mission(const std::string& _missionName, int32_t _storageID, int32_t _startValue, int32_t _endValue, bool _ignoreEndValue)
{
	missionName = _missionName;
	endValue = _endValue;
	ignoreEndValue = _ignoreEndValue;
	startValue = _startValue;
	storageID = _storageID;
	mainState = NULL;
}

Mission::~Mission()
{
	for (uint32_t it = 0; it != state.size(); it++) {
		delete state[it];
	}

	state.clear();
}

std::string Mission::getDescription(Player* player)
{
	int32_t value;
	player->getStorageValue(storageID, value);

	if (mainState) {
		std::string desc = mainState->getMissionDescription();
		replaceString(desc, "|STATE|", std::to_string(value));
		replaceString(desc, "\\n", "\n");
		return desc;
	}

	int32_t current = endValue;

	if (ignoreEndValue) {
		while (current >= startValue) {
			if (value >= current) {
				StateList::const_iterator sit = state.find(current);

				if (sit != state.end()) {
					return sit->second->getMissionDescription();
				}
			}

			current--;
		}
	} else {
		while (current >= startValue) {
			if (value == current) {
				StateList::const_iterator sit = state.find(current);

				if (sit != state.end()) {
					return sit->second->getMissionDescription();
				}
			}

			current--;
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

std::string Mission::getName(Player* player)
{
	if (isCompleted(player)) {
		return missionName + " (completed)";
	}

	return missionName;
}

Quest::Quest(const std::string& _name, uint16_t _id, int32_t _startStorageID, int32_t _startStorageValue)
{
	name = _name;
	id = _id;
	startStorageID = _startStorageID;
	startStorageValue = _startStorageValue;
}

Quest::~Quest()
{
	for (MissionsList::iterator it = missions.begin(), end = missions.end(); it != end; ++it) {
		delete (*it);
	}

	missions.clear();
}

uint16_t Quest::getMissionsCount(Player* player) const
{
	uint16_t count = 0;

	for (MissionsList::const_iterator it = missions.begin(), end = missions.end(); it != end; ++it) {
		if ((*it)->isStarted(player)) {
			count++;
		}
	}

	return count;
}

bool Quest::isCompleted(Player* player)
{
	for (MissionsList::const_iterator it = missions.begin(), end = missions.end(); it != end; ++it) {
		if (!(*it)->isCompleted(player)) {
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

Quests::Quests()
{
	//
}

Quests::~Quests()
{
	for (QuestsList::iterator it = quests.begin(), end = quests.end(); it != end; ++it) {
		delete (*it);
	}

	quests.clear();
}

bool Quests::reload()
{
	for (QuestsList::iterator it = quests.begin(), end = quests.end(); it != end; ++it) {
		delete (*it);
	}

	quests.clear();
	return loadFromXml();
}

bool Quests::loadFromXml()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/XML/quests.xml");
	if (!result) {
		std::cout << "[Error - Quests::loadFromXml] Failed to load data/XML/quests.xml: " << result.description() << std::endl;
		return false;
	}

	for (pugi::xml_node questNode = doc.child("quests").first_child(); questNode; questNode = questNode.next_sibling()) {
		uint16_t id = 0;
		Quest* quest = new Quest(
			questNode.attribute("name").as_string(),
			id++,
			pugi::cast<int32_t>(questNode.attribute("startstorageid").value()),
			pugi::cast<int32_t>(questNode.attribute("startstoragevalue").value())
		);
		for (pugi::xml_node missionNode = questNode.first_child(); missionNode; missionNode = missionNode.next_sibling()) {
			std::string missionState = missionNode.attribute("description").as_string();

			Mission* mission = new Mission(
				missionNode.attribute("name").as_string(),
				pugi::cast<int32_t>(missionNode.attribute("storageid").value()),
				pugi::cast<int32_t>(missionNode.attribute("startvalue").value()),
				pugi::cast<int32_t>(missionNode.attribute("endvalue").value()),
				missionNode.attribute("ignoreendvalue").as_bool()
			);
			if (missionState.empty()) {
				for (pugi::xml_node missionStateNode = missionNode.first_child(); missionStateNode; missionStateNode = missionStateNode.next_sibling()) {
					int32_t missionId = pugi::cast<int32_t>(missionStateNode.attribute("id").value());
					mission->state[missionId] = new MissionState(missionStateNode.attribute("description").as_string(), missionId);
				}
			} else {
				mission->mainState = new MissionState(missionState, 0);
			}
			quest->addMission(mission);
		}
		quests.push_back(quest);
	}
	return true;
}

Quest* Quests::getQuestByID(uint16_t id)
{
	for (QuestsList::iterator it = quests.begin(), end = quests.end(); it != end; ++it) {
		if ((*it)->getID() == id) {
			return (*it);
		}
	}
	return NULL;
}

uint16_t Quests::getQuestsCount(Player* player)
{
	uint16_t count = 0;

	for (QuestsList::const_iterator it = quests.begin(), end = quests.end(); it != end; ++it) {
		if ((*it)->isStarted(player)) {
			count++;
		}
	}

	return count;
}

bool Quests::isQuestStorage(const uint32_t key, const int32_t value, const int32_t oldValue)
{
	for (QuestsList::const_iterator it = quests.begin(), end = quests.end(); it != end; ++it) {
		Quest* quest = *it;

		if (quest->getStartStorageId() == key && quest->getStartStorageValue() == value) {
			return true;
		}

		for (MissionsList::const_iterator m_it = quest->getFirstMission(), m_end = quest->getLastMission(); m_it != m_end; ++m_it) {
			Mission* mission = *m_it;

			if (mission->mainState) {
				if (mission->getStorageId() == key && value >= mission->getStartStorageValue() && value <= mission->getEndStorageValue() && (oldValue < mission->getStartStorageValue() || oldValue > mission->getEndStorageValue())) {
					return true;
				}
			} else {
				if (mission->getStorageId() == key && value >= mission->getStartStorageValue() && value <= mission->getEndStorageValue()) {
					return true;
				}
			}
		}
	}

	return false;
}
