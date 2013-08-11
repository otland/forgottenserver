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

#include "raids.h"

#include "game.h"
#include "player.h"
#include "configmanager.h"

#include <algorithm>

extern Game g_game;
extern ConfigManager g_config;

Raids::Raids()
	: m_scriptInterface("Raid Interface")
{
	loaded = false;
	started = false;
	running = NULL;
	lastRaidEnd = 0;
	checkRaidsEvent = 0;

	m_scriptInterface.initState();
}

Raids::~Raids()
{
	clear();
}

bool Raids::loadFromXml()
{
	if (isLoaded()) {
		return true;
	}

	xmlDocPtr doc = xmlParseFile("data/raids/raids.xml");

	if (doc) {
		xmlNodePtr root, raidNode;
		root = xmlDocGetRootElement(doc);

		if (xmlStrcmp(root->name, (const xmlChar*)"raids") != 0) {
			std::cout << "[Error] Raids: Wrong root node." << std::endl;
			xmlFreeDoc(doc);
			return false;
		}

		int intValue;
		std::string strValue;
		raidNode = root->children;

		while (raidNode) {
			if (xmlStrcmp(raidNode->name, (const xmlChar*)"raid") == 0) {
				std::string name, file;
				uint32_t interval, margin;

				if (readXMLString(raidNode, "name", strValue)) {
					name = strValue;
				} else {
					std::cout << "[Error] Raids: name tag missing for raid." << std::endl;
					raidNode = raidNode->next;
					continue;
				}

				if (readXMLString(raidNode, "file", strValue)) {
					file = strValue;
				} else {
					std::ostringstream ss;
					ss << "raids/" << name << ".xml";
					file = ss.str();
					std::cout << "[Warning] Raids: file tag missing for raid " << name << ". Using default: " << file << std::endl;
				}

				//interval2 is the average interval between
				// 2 executions of the raid in minutes
				if (readXMLInteger(raidNode, "interval2", intValue) && intValue > 0) {
					interval = intValue * 60;
				} else {
					std::cout << "[Error] Raids: interval2 tag missing or divided by 0 for raid " << name << std::endl;
					raidNode = raidNode->next;
					continue;
				}

				if (readXMLInteger(raidNode, "margin", intValue)) {
					margin = intValue * 60 * 1000;
				} else {
					std::cout << "[Warning] Raids: margin tag missing for raid " << name << std::endl;
					margin = 0;
				}

				Raid* newRaid = new Raid(name, interval, margin);

				bool ret = newRaid->loadFromXml("data/raids/" + file);

				if (!ret) {
					std::cout << "[Error] Raids: failed to load raid " << name << std::endl;
					delete newRaid;
				} else {
					raidList.push_back(newRaid);
				}
			}

			raidNode = raidNode->next;
		}

		xmlFreeDoc(doc);

	} else {
		std::cout << "[Error] Raids: Could not load data/raids/raids.xml" << std::endl;
		return false;
	}

	loaded = true;
	return true;
}

#define MAX_RAND_RANGE 10000000

bool Raids::startup()
{
	if (!isLoaded() || isStarted()) {
		return false;
	}

	setLastRaidEnd(OTSYS_TIME());

	checkRaidsEvent = g_scheduler.addEvent(createSchedulerTask(CHECK_RAIDS_INTERVAL * 1000, boost::bind(&Raids::checkRaids, this)));

	started = true;
	return started;
}

void Raids::checkRaids()
{
	if (!getRunning()) {
		uint64_t now = OTSYS_TIME();

		for (RaidList::iterator it = raidList.begin(); it != raidList.end(); ++it) {
			if (now >= (getLastRaidEnd() + (*it)->getMargin())) {
				if (MAX_RAND_RANGE * CHECK_RAIDS_INTERVAL / (*it)->getInterval() >= (uint32_t)random_range(0, MAX_RAND_RANGE)) {
					setRunning(*it);
					(*it)->startRaid();

					if (g_config.getBoolean(ConfigManager::SHUTDOWN_AT_SERVERSAVE)) {
						raidList.erase(it);
					}
					break;
				}
			}
		}
	}

	checkRaidsEvent = g_scheduler.addEvent(createSchedulerTask(CHECK_RAIDS_INTERVAL * 1000, boost::bind(&Raids::checkRaids, this)));
}

void Raids::clear()
{
	g_scheduler.stopEvent(checkRaidsEvent);
	checkRaidsEvent = 0;

	for (RaidList::const_iterator it = raidList.begin(), end = raidList.end(); it != end; ++it) {
		delete *it;
	}

	raidList.clear();

	loaded = false;
	started = false;
	running = NULL;
	lastRaidEnd = 0;

	m_scriptInterface.reInitState();
}

bool Raids::reload()
{
	clear();
	return loadFromXml();
}

Raid* Raids::getRaidByName(const std::string& name)
{
	for (Raid* raid : raidList) {
		if (strcasecmp(raid->getName().c_str(), name.c_str()) == 0) {
			return raid;
		}
	}
	return NULL;
}

Raid::Raid(const std::string& _name, uint32_t _interval, uint32_t _marginTime)
{
	loaded = false;
	name = _name;
	interval = _interval;
	nextEvent = 0;
	state = RAIDSTATE_IDLE;
	margin = _marginTime;
	nextEventEvent = 0;
}

Raid::~Raid()
{
	stopEvents();

	for (RaidEventVector::iterator it = raidEvents.begin(), end = raidEvents.end(); it != end; ++it) {
		delete (*it);
	}

	raidEvents.clear();
}

bool Raid::loadFromXml(const std::string& _filename)
{
	if (isLoaded()) {
		return true;
	}

	xmlDocPtr doc = xmlParseFile(_filename.c_str());

	if (doc) {
		xmlNodePtr root, eventNode;
		root = xmlDocGetRootElement(doc);

		if (xmlStrcmp(root->name, (const xmlChar*)"raid") != 0) {
			std::cout << "[Error] Raids: Wrong root node." << std::endl;
			xmlFreeDoc(doc);
			return false;
		}

		eventNode = root->children;

		while (eventNode) {
			RaidEvent* event;

			if (xmlStrcmp(eventNode->name, (const xmlChar*)"announce") == 0) {
				event = new AnnounceEvent();
			} else if (xmlStrcmp(eventNode->name, (const xmlChar*)"singlespawn") == 0) {
				event = new SingleSpawnEvent();
			} else if (xmlStrcmp(eventNode->name, (const xmlChar*)"areaspawn") == 0) {
				event = new AreaSpawnEvent();
			} else if (xmlStrcmp(eventNode->name, (const xmlChar*)"script") == 0) {
				event = new ScriptEvent(&Raids::getInstance()->getScriptInterface());
			} else {
				eventNode = eventNode->next;
				continue;
			}

			if (event->configureRaidEvent(eventNode)) {
				raidEvents.push_back(event);
			} else {
				std::cout << "Raids: Error in file(" << _filename << ") eventNode: " << eventNode->name << std::endl;
				delete event;
			}

			eventNode = eventNode->next;
		}

		//sort by delay time
		std::sort(raidEvents.begin(), raidEvents.end(), RaidEvent::compareEvents);

		xmlFreeDoc(doc);
	} else {
		std::cout << "[Error] Raid: Could not load " << _filename << "!" << std::endl;
		return false;
	}

	loaded = true;
	return true;
}

void Raid::startRaid()
{
	RaidEvent* raidEvent = getNextRaidEvent();

	if (raidEvent) {
		state = RAIDSTATE_EXECUTING;
		nextEventEvent = g_scheduler.addEvent(createSchedulerTask(raidEvent->getDelay(), boost::bind(&Raid::executeRaidEvent, this, raidEvent)));
	}
}

void Raid::executeRaidEvent(RaidEvent* raidEvent)
{
	if (raidEvent->executeEvent()) {
		nextEvent++;
		RaidEvent* newRaidEvent = getNextRaidEvent();

		if (newRaidEvent) {
			uint32_t ticks = (uint32_t)std::max<int32_t>(RAID_MINTICKS, newRaidEvent->getDelay() - raidEvent->getDelay());
			nextEventEvent = g_scheduler.addEvent(createSchedulerTask(ticks, boost::bind(&Raid::executeRaidEvent, this, newRaidEvent)));
		} else {
			resetRaid();
		}
	} else {
		resetRaid();
	}
}

void Raid::resetRaid()
{
	nextEvent = 0;
	state = RAIDSTATE_IDLE;
	Raids::getInstance()->setRunning(NULL);
	Raids::getInstance()->setLastRaidEnd(OTSYS_TIME());
}

void Raid::stopEvents()
{
	if (nextEventEvent != 0) {
		g_scheduler.stopEvent(nextEventEvent);
		nextEventEvent = 0;
	}
}

RaidEvent* Raid::getNextRaidEvent()
{
	if (nextEvent < raidEvents.size()) {
		return raidEvents[nextEvent];
	} else {
		return NULL;
	}
}

void Raid::addEvent(RaidEvent* event)
{
	raidEvents.push_back(event);
}

bool RaidEvent::configureRaidEvent(xmlNodePtr eventNode)
{
	int32_t intValue;

	if (readXMLInteger(eventNode, "delay", intValue)) {
		m_delay = intValue;

		if (m_delay < RAID_MINTICKS) {
			m_delay = RAID_MINTICKS;
		}

		return true;
	} else {
		std::cout << "[Error] Raid: delay tag missing." << std::endl;
		return false;
	}
}

bool AnnounceEvent::configureRaidEvent(xmlNodePtr eventNode)
{
	if (!RaidEvent::configureRaidEvent(eventNode)) {
		return false;
	}

	std::string strValue;

	if (readXMLString(eventNode, "message", strValue)) {
		m_message = strValue;
	} else {
		std::cout << "[Error] Raid: message tag missing for announce event." << std::endl;
		return false;
	}

	if (readXMLString(eventNode, "type", strValue)) {
		std::string tmpStrValue = asLowerCaseString(strValue);

		if (tmpStrValue == "warning") {
			m_messageType = MSG_STATUS_WARNING;
		} else if (tmpStrValue == "event") {
			m_messageType = MSG_EVENT_ADVANCE;
		} else if (tmpStrValue == "default") {
			m_messageType = MSG_EVENT_DEFAULT;
		} else if (tmpStrValue == "description") {
			m_messageType = MSG_INFO_DESCR;
		} else if (tmpStrValue == "smallstatus") {
			m_messageType = MSG_STATUS_SMALL;
		} else if (tmpStrValue == "blueconsole") {
			m_messageType = MSG_STATUS_CONSOLE_BLUE;
		} else if (tmpStrValue == "redconsole") {
			m_messageType = MSG_STATUS_CONSOLE_RED;
		} else {
			std::cout << "[Notice] Raid: Unknown type tag missing for announce event. Using default: " << (int32_t)m_messageType << std::endl;
		}
	} else {
		m_messageType = MSG_EVENT_ADVANCE;
		std::cout << "[Notice] Raid: type tag missing for announce event. Using default: " << (int32_t)m_messageType << std::endl;
	}

	return true;
}

bool AnnounceEvent::executeEvent()
{
	g_game.broadcastMessage(m_message, m_messageType);
	return true;
}

bool SingleSpawnEvent::configureRaidEvent(xmlNodePtr eventNode)
{
	if (!RaidEvent::configureRaidEvent(eventNode)) {
		return false;
	}

	std::string strValue;
	int32_t intValue;

	if (readXMLString(eventNode, "name", strValue)) {
		m_monsterName = strValue;
	} else {
		std::cout << "[Error] Raid: name tag missing for singlespawn event." << std::endl;
		return false;
	}

	if (readXMLInteger(eventNode, "x", intValue)) {
		m_position.x = intValue;
	} else {
		std::cout << "[Error] Raid: x tag missing for singlespawn event." << std::endl;
		return false;
	}

	if (readXMLInteger(eventNode, "y", intValue)) {
		m_position.y = intValue;
	} else {
		std::cout << "[Error] Raid: y tag missing for singlespawn event." << std::endl;
		return false;
	}

	if (readXMLInteger(eventNode, "z", intValue)) {
		m_position.z = intValue;
	} else {
		std::cout << "[Error] Raid: z tag missing for singlespawn event." << std::endl;
		return false;
	}

	return true;
}

bool SingleSpawnEvent::executeEvent()
{
	Monster* monster = Monster::createMonster(m_monsterName);

	if (!monster) {
		std::cout << "[Error] Raids: Cant create monster " << m_monsterName << std::endl;
		return false;
	}

	if (!g_game.placeCreature(monster, m_position, false, true)) {
		delete monster;
		std::cout << "[Error] Raids: Cant place monster " << m_monsterName << std::endl;
		return false;
	}

	return true;
}

bool AreaSpawnEvent::configureRaidEvent(xmlNodePtr eventNode)
{
	if (!RaidEvent::configureRaidEvent(eventNode)) {
		return false;
	}

	std::string strValue;
	int32_t intValue;

	if (readXMLInteger(eventNode, "radius", intValue)) {
		int32_t radius = intValue;
		Position centerPos;

		if (readXMLInteger(eventNode, "centerx", intValue)) {
			centerPos.x = intValue;
		} else {
			std::cout << "[Error] Raid: centerx tag missing for areaspawn event." << std::endl;
			return false;
		}

		if (readXMLInteger(eventNode, "centery", intValue)) {
			centerPos.y = intValue;
		} else {
			std::cout << "[Error] Raid: centery tag missing for areaspawn event." << std::endl;
			return false;
		}

		if (readXMLInteger(eventNode, "centerz", intValue)) {
			centerPos.z = intValue;
		} else {
			std::cout << "[Error] Raid: centerz tag missing for areaspawn event." << std::endl;
			return false;
		}

		m_fromPos.x = std::max<int32_t>(0, centerPos.getX() - radius);
		m_fromPos.y = std::max<int32_t>(0, centerPos.getY() - radius);
		m_fromPos.z = centerPos.z;

		m_toPos.x = std::min<int32_t>(0xFFFF, centerPos.getX() + radius);
		m_toPos.y = std::min<int32_t>(0xFFFF, centerPos.getY() + radius);
		m_toPos.z = centerPos.z;
	} else {
		if (readXMLInteger(eventNode, "fromx", intValue)) {
			m_fromPos.x = intValue;
		} else {
			std::cout << "[Error] Raid: fromx tag missing for areaspawn event." << std::endl;
			return false;
		}

		if (readXMLInteger(eventNode, "fromy", intValue)) {
			m_fromPos.y = intValue;
		} else {
			std::cout << "[Error] Raid: fromy tag missing for areaspawn event." << std::endl;
			return false;
		}

		if (readXMLInteger(eventNode, "fromz", intValue)) {
			m_fromPos.z = intValue;
		} else {
			std::cout << "[Error] Raid: fromz tag missing for areaspawn event." << std::endl;
			return false;
		}

		if (readXMLInteger(eventNode, "tox", intValue)) {
			m_toPos.x = intValue;
		} else {
			std::cout << "[Error] Raid: tox tag missing for areaspawn event." << std::endl;
			return false;
		}

		if (readXMLInteger(eventNode, "toy", intValue)) {
			m_toPos.y = intValue;
		} else {
			std::cout << "[Error] Raid: toy tag missing for areaspawn event." << std::endl;
			return false;
		}

		if (readXMLInteger(eventNode, "toz", intValue)) {
			m_toPos.z = intValue;
		} else {
			std::cout << "[Error] Raid: toz tag missing for areaspawn event." << std::endl;
			return false;
		}
	}

	xmlNodePtr monsterNode = eventNode->children;

	while (monsterNode) {
		if (xmlStrcmp(monsterNode->name, (const xmlChar*)"monster") == 0) {
			std::string name;
			int32_t minAmount = 0;
			int32_t maxAmount = 0;

			if (readXMLString(monsterNode, "name", strValue)) {
				name = strValue;
			} else {
				std::cout << "[Error] Raid: name tag missing for monster node." << std::endl;
				return false;
			}

			if (readXMLInteger(monsterNode, "minamount", intValue)) {
				minAmount = intValue;
			}

			if (readXMLInteger(monsterNode, "maxamount", intValue)) {
				maxAmount = intValue;
			}

			if (maxAmount == 0 && minAmount == 0) {
				if (readXMLInteger(monsterNode, "amount", intValue)) {
					maxAmount = intValue;
					minAmount = intValue;
				} else {
					std::cout << "[Error] Raid: amount tag missing for monster node." << std::endl;
					return false;
				}
			}

			addMonster(name, minAmount, maxAmount);
		}

		monsterNode = monsterNode->next;
	}

	return true;
}

AreaSpawnEvent::~AreaSpawnEvent()
{
	for (MonsterSpawnList::iterator it = m_spawnList.begin(), end = m_spawnList.end(); it != end; ++it) {
		delete (*it);
	}

	m_spawnList.clear();
}

void AreaSpawnEvent::addMonster(MonsterSpawn* monsterSpawn)
{
	m_spawnList.push_back(monsterSpawn);
}

void AreaSpawnEvent::addMonster(const std::string& monsterName, uint32_t minAmount, uint32_t maxAmount)
{
	MonsterSpawn* monsterSpawn = new MonsterSpawn();
	monsterSpawn->name = monsterName;
	monsterSpawn->minAmount = minAmount;
	monsterSpawn->maxAmount = maxAmount;
	addMonster(monsterSpawn);
}

bool AreaSpawnEvent::executeEvent()
{
	for (MonsterSpawnList::iterator it = m_spawnList.begin(), end = m_spawnList.end(); it != end; ++it) {
		MonsterSpawn* spawn = (*it);

		uint32_t amount = random_range(spawn->minAmount, spawn->maxAmount);

		for (uint32_t i = 0; i < amount; ++i) {
			Monster* monster = Monster::createMonster(spawn->name);

			if (!monster) {
				std::cout << "[Error] Raids: Cant create monster " << spawn->name << std::endl;
				return false;
			}

			bool success = false;

			for (int32_t tries = 0; tries < MAXIMUM_TRIES_PER_MONSTER; tries++) {
				Position pos;
				pos.x = random_range(m_fromPos.x, m_toPos.x);
				pos.y = random_range(m_fromPos.y, m_toPos.y);
				pos.z = random_range(m_fromPos.z, m_toPos.z);

				Tile* tile = g_game.getMap()->getTile(pos);

				if (tile && !tile->isMoveableBlocking() && !tile->hasFlag(TILESTATE_PROTECTIONZONE) && tile->getTopCreature() == NULL && g_game.placeCreature(monster, pos, false, true)) {
					success = true;
					break;
				}
			}

			if (!success) {
				delete monster;
			}
		}
	}

	return true;
}

ScriptEvent::ScriptEvent(LuaScriptInterface* _interface) :
	Event(_interface)
{
}

bool ScriptEvent::configureRaidEvent(xmlNodePtr eventNode)
{
	if (!RaidEvent::configureRaidEvent(eventNode)) {
		return false;
	}

	std::string str;

	if (readXMLString(eventNode, "script", str)) {
		if (!loadScript("data/raids/scripts/" + str)) {
			std::cout << "Error: [ScriptEvent::configureRaidEvent] Can not load raid script." << std::endl;
			return false;
		}
	} else {
		std::cout << "Error: [ScriptEvent::configureRaidEvent] No script file found for raid" << std::endl;
		return false;
	}

	return true;
}

std::string ScriptEvent::getScriptEventName()
{
	return "onRaid";
}

bool ScriptEvent::executeEvent()
{
	//onRaid()
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - ScriptEvent::onRaid] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();

	env->setScriptId(m_scriptId, m_scriptInterface);

	m_scriptInterface->pushFunction(m_scriptId);

	return m_scriptInterface->callFunction(0);
}
