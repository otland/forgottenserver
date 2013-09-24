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

#include "pugicast.h"

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
	running = nullptr;
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

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/raids/raids.xml");
	if (!result) {
		std::cout << "[Error - Raids::loadFromXml] Failed to load data/raids/raids.xml: " << result.description() << std::endl;
		return false;
	}

	for (pugi::xml_node raidNode = doc.child("raids").first_child(); raidNode; raidNode = raidNode.next_sibling()) {
		std::string name, file;
		uint32_t interval, margin;

		pugi::xml_attribute attr;
		if ((attr = raidNode.attribute("name"))) {
			name = attr.as_string();
		} else {
			std::cout << "[Error - Raids::loadFromXml] Name tag missing for raid" << std::endl;
			continue;
		}

		if ((attr = raidNode.attribute("file"))) {
			file = attr.as_string();
		} else {
			std::ostringstream ss;
			ss << "raids/" << name << ".xml";
			file = ss.str();
			std::cout << "[Warning - Raids::loadFromXml] File tag missing for raid " << name << ". Using default: " << file << std::endl;
		}

		interval = pugi::cast<uint32_t>(raidNode.attribute("interval2").value()) * 60;
		if (interval == 0) {
			std::cout << "[Error - Raids::loadFromXml] interval2 tag missing or zero (would divide by 0) for raid: " << name << std::endl;
			continue;
		}

		if ((attr = raidNode.attribute("margin"))) {
			margin = pugi::cast<uint32_t>(attr.value()) * 60 * 1000;
		} else {
			std::cout << "[Warning - Raids::loadFromXml] margin tag missing for raid: " << name << std::endl;
			margin = 0;
		}

		Raid* newRaid = new Raid(name, interval, margin);
		if (newRaid->loadFromXml("data/raids/" + file)) {
			raidList.push_back(newRaid);
		} else {
			std::cout << "[Error - Raids::loadFromXml] Failed to load raid: " << name << std::endl;
			delete newRaid;
		}
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

		for (auto it = raidList.begin(); it != raidList.end(); ++it) {
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

	for (Raid* raid : raidList) {
		delete raid;
	}
	raidList.clear();

	loaded = false;
	started = false;
	running = nullptr;
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
	return nullptr;
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

	for (RaidEvent* raidEvent : raidEvents) {
		delete raidEvent;
	}
}

bool Raid::loadFromXml(const std::string& _filename)
{
	if (isLoaded()) {
		return true;
	}

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(_filename.c_str());
	if (!result) {
		std::cout << "[Error - Raid::loadFromXml] Failed to load " << _filename << ": " << result.description() << std::endl;
		return false;
	}

	for (pugi::xml_node eventNode = doc.child("raid").first_child(); eventNode; eventNode = eventNode.next_sibling()) {
		RaidEvent* event;
		if (strcasecmp(eventNode.name(), "announce") == 0) {
			event = new AnnounceEvent();
		} else if (strcasecmp(eventNode.name(), "singlespawn") == 0) {
			event = new SingleSpawnEvent();
		} else if (strcasecmp(eventNode.name(), "areaspawn") == 0) {
			event = new AreaSpawnEvent();
		} else if (strcasecmp(eventNode.name(), "script") == 0) {
			event = new ScriptEvent(&Raids::getInstance()->getScriptInterface());
		} else {
			continue;
		}

		if (event->configureRaidEvent(eventNode)) {
			raidEvents.push_back(event);
		} else {
			std::cout << "[Error - Raid::loadFromXml] In file (" << _filename << "), eventNode: " << eventNode.name() << std::endl;
			delete event;
		}
	}

	//sort by delay time
	std::sort(raidEvents.begin(), raidEvents.end(), RaidEvent::compareEvents);

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
	Raids::getInstance()->setRunning(nullptr);
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
		return nullptr;
	}
}

void Raid::addEvent(RaidEvent* event)
{
	raidEvents.push_back(event);
}

bool RaidEvent::configureRaidEvent(const pugi::xml_node& eventNode)
{
	pugi::xml_attribute delayAttribute = eventNode.attribute("delay");
	if (!delayAttribute) {
		std::cout << "[Error] Raid: delay tag missing." << std::endl;
		return false;
	}

	m_delay = pugi::cast<uint32_t>(delayAttribute.value());
	if (m_delay < RAID_MINTICKS) {
		m_delay = RAID_MINTICKS;
	}
	return true;
}

bool AnnounceEvent::configureRaidEvent(const pugi::xml_node& eventNode)
{
	if (!RaidEvent::configureRaidEvent(eventNode)) {
		return false;
	}

	pugi::xml_attribute messageAttribute = eventNode.attribute("message");
	if (messageAttribute) {
		m_message = messageAttribute.as_string();
	} else {
		std::cout << "[Error] Raid: message tag missing for announce event." << std::endl;
		return false;
	}

	pugi::xml_attribute typeAttribute = eventNode.attribute("type");
	if (typeAttribute) {
		std::string tmpStrValue = asLowerCaseString(typeAttribute.as_string());
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

bool SingleSpawnEvent::configureRaidEvent(const pugi::xml_node& eventNode)
{
	if (!RaidEvent::configureRaidEvent(eventNode)) {
		return false;
	}

	pugi::xml_attribute attr;
	if ((attr = eventNode.attribute("name"))) {
		m_monsterName = attr.as_string();
	} else {
		std::cout << "[Error] Raid: name tag missing for singlespawn event." << std::endl;
		return false;
	}

	if ((attr = eventNode.attribute("x"))) {
		m_position.x = pugi::cast<uint16_t>(attr.value());
	} else {
		std::cout << "[Error] Raid: x tag missing for singlespawn event." << std::endl;
		return false;
	}

	if ((attr = eventNode.attribute("y"))) {
		m_position.y = pugi::cast<uint16_t>(attr.value());
	} else {
		std::cout << "[Error] Raid: y tag missing for singlespawn event." << std::endl;
		return false;
	}

	if ((attr = eventNode.attribute("z"))) {
		m_position.z = pugi::cast<uint16_t>(attr.value());
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

bool AreaSpawnEvent::configureRaidEvent(const pugi::xml_node& eventNode)
{
	if (!RaidEvent::configureRaidEvent(eventNode)) {
		return false;
	}

	pugi::xml_attribute attr;
	if ((attr = eventNode.attribute("radius"))) {
		int32_t radius = pugi::cast<int32_t>(attr.value());
		Position centerPos;

		if ((attr = eventNode.attribute("centerx"))) {
			centerPos.x = pugi::cast<uint16_t>(attr.value());
		} else {
			std::cout << "[Error] Raid: centerx tag missing for areaspawn event." << std::endl;
			return false;
		}

		if ((attr = eventNode.attribute("centery"))) {
			centerPos.y = pugi::cast<uint16_t>(attr.value());
		} else {
			std::cout << "[Error] Raid: centery tag missing for areaspawn event." << std::endl;
			return false;
		}

		if ((attr = eventNode.attribute("centerz"))) {
			centerPos.z = pugi::cast<uint16_t>(attr.value());
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
		if ((attr = eventNode.attribute("fromx"))) {
			m_fromPos.x = pugi::cast<uint16_t>(attr.value());
		} else {
			std::cout << "[Error] Raid: fromx tag missing for areaspawn event." << std::endl;
			return false;
		}

		if ((attr = eventNode.attribute("fromy"))) {
			m_fromPos.y = pugi::cast<uint16_t>(attr.value());
		} else {
			std::cout << "[Error] Raid: fromy tag missing for areaspawn event." << std::endl;
			return false;
		}

		if ((attr = eventNode.attribute("fromz"))) {
			m_fromPos.z = pugi::cast<uint16_t>(attr.value());
		} else {
			std::cout << "[Error] Raid: fromz tag missing for areaspawn event." << std::endl;
			return false;
		}

		if ((attr = eventNode.attribute("tox"))) {
			m_toPos.x = pugi::cast<uint16_t>(attr.value());
		} else {
			std::cout << "[Error] Raid: tox tag missing for areaspawn event." << std::endl;
			return false;
		}

		if ((attr = eventNode.attribute("toy"))) {
			m_toPos.y = pugi::cast<uint16_t>(attr.value());
		} else {
			std::cout << "[Error] Raid: toy tag missing for areaspawn event." << std::endl;
			return false;
		}

		if ((attr = eventNode.attribute("toz"))) {
			m_toPos.z = pugi::cast<uint16_t>(attr.value());
		} else {
			std::cout << "[Error] Raid: toz tag missing for areaspawn event." << std::endl;
			return false;
		}
	}

	for (pugi::xml_node monsterNode = eventNode.child("monster").first_child(); monsterNode; monsterNode = monsterNode.next_sibling()) {
		std::string name;

		if ((attr = monsterNode.attribute("name"))) {
			name = attr.as_string();
		} else {
			std::cout << "[Error] Raid: name tag missing for monster node." << std::endl;
			return false;
		}

		int32_t minAmount;
		if ((attr = monsterNode.attribute("minamount"))) {
			minAmount = pugi::cast<uint32_t>(attr.value());
		} else {
			minAmount = 0;
		}

		int32_t maxAmount;
		if ((attr = monsterNode.attribute("maxamount"))) {
			maxAmount = pugi::cast<uint32_t>(attr.value());
		} else {
			maxAmount = 0;
		}

		if (maxAmount == 0 && minAmount == 0) {
			if ((attr = monsterNode.attribute("amount"))) {
				minAmount = pugi::cast<uint32_t>(attr.value());
				maxAmount = minAmount;
			} else {
				std::cout << "[Error] Raid: amount tag missing for monster node." << std::endl;
				return false;
			}
		}

		addMonster(name, minAmount, maxAmount);
	}
	return true;
}

AreaSpawnEvent::~AreaSpawnEvent()
{
	for (MonsterSpawn* spawn : m_spawnList) {
		delete spawn;
	}
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
	for (MonsterSpawn* spawn : m_spawnList) {
		uint32_t amount = random_range(spawn->minAmount, spawn->maxAmount);
		for (uint32_t i = 0; i < amount; ++i) {
			Monster* monster = Monster::createMonster(spawn->name);
			if (!monster) {
				std::cout << "[Error - AreaSpawnEvent::executeEvent] Can't create monster " << spawn->name << std::endl;
				return false;
			}

			bool success = false;
			for (int32_t tries = 0; tries < MAXIMUM_TRIES_PER_MONSTER; tries++) {
				Position pos(random_range(m_fromPos.x, m_toPos.x), random_range(m_fromPos.y, m_toPos.y), random_range(m_fromPos.z, m_toPos.z));
				Tile* tile = g_game.getTile(pos);
				if (tile && !tile->isMoveableBlocking() && !tile->hasFlag(TILESTATE_PROTECTIONZONE) && tile->getTopCreature() == nullptr && g_game.placeCreature(monster, pos, false, true)) {
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

bool ScriptEvent::configureRaidEvent(const pugi::xml_node& eventNode)
{
	if (!RaidEvent::configureRaidEvent(eventNode)) {
		return false;
	}

	pugi::xml_attribute scriptAttribute = eventNode.attribute("script");
	if (!scriptAttribute) {
		std::cout << "Error: [ScriptEvent::configureRaidEvent] No script file found for raid" << std::endl;
		return false;
	}

	if (!loadScript("data/raids/scripts/" + std::string(scriptAttribute.as_string()))) {
		std::cout << "Error: [ScriptEvent::configureRaidEvent] Can not load raid script." << std::endl;
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
