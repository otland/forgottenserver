/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2014  Mark Samman <mark.samman@gmail.com>
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

#include "actions.h"
#include "beds.h"
#include "configmanager.h"
#include "const.h"
#include "container.h"
#include "game.h"
#include "house.h"
#include "item.h"
#include "player.h"
#include "pugicast.h"
#include "spells.h"
#include "tasks.h"

extern Game g_game;
extern Spells* g_spells;
extern Actions* g_actions;
extern ConfigManager g_config;

Actions::Actions() :
	m_scriptInterface("Action Interface")
{
	m_scriptInterface.initState();
}

Actions::~Actions()
{
	clear();
}

inline void Actions::clearMap(ActionUseMap& map)
{
	// Filter out duplicates to avoid double-free
	std::unordered_set<Action*> set;
	for (const auto& it : map) {
		set.insert(it.second);
	}
	map.clear();

	for (Action* action : set) {
		delete action;
	}
}

void Actions::clear()
{
	clearMap(useItemMap);
	clearMap(uniqueItemMap);
	clearMap(actionItemMap);

	m_scriptInterface.reInitState();
}

LuaScriptInterface& Actions::getScriptInterface()
{
	return m_scriptInterface;
}

std::string Actions::getScriptBaseName()
{
	return "actions";
}

Event* Actions::getEvent(const std::string& nodeName)
{
	if (asLowerCaseString(nodeName) == "action") {
		return new Action(&m_scriptInterface);
	}
	return nullptr;
}

bool Actions::registerEvent(Event* event, const pugi::xml_node& node)
{
	Action* action = dynamic_cast<Action*>(event);
	if (!action) {
		return false;
	}

	pugi::xml_attribute attr;
	if ((attr = node.attribute("itemid"))) {
		uint16_t id = pugi::cast<uint16_t>(attr.value());
		if (useItemMap.find(id) != useItemMap.end()) {
			std::cout << "[Warning - Actions::registerEvent] Duplicate registered item with id: " << id << std::endl;
			return false;
		}
		useItemMap[id] = action;
	} else if ((attr = node.attribute("fromid"))) {
		pugi::xml_attribute toIdAttribute = node.attribute("toid");
		if (toIdAttribute) {
			uint16_t fromId = pugi::cast<uint16_t>(attr.value());
			uint16_t iterId = fromId;
			uint16_t toId = pugi::cast<uint16_t>(toIdAttribute.value());

			bool success = false;
			if (useItemMap.find(iterId) != useItemMap.end()) {
				std::cout << "[Warning - Actions::registerEvent] Duplicate registered item with id: " << iterId << " in fromid: " << fromId << ", toid: " << toId << std::endl;
			} else {
				useItemMap[iterId] = action;
				success = true;
			}

			while (++iterId <= toId) {
				if (useItemMap.find(iterId) != useItemMap.end()) {
					std::cout << "[Warning - Actions::registerEvent] Duplicate registered item with id: " << iterId << " in fromid: " << fromId << ", toid: " << toId << std::endl;
					continue;
				}
				useItemMap[iterId] = action;
				success = true;
			}
			return success;
		} else {
			std::cout << "[Warning - Actions::registerEvent] Missing toid in fromid: " << attr.as_string() << std::endl;
			return false;
		}
	} else if ((attr = node.attribute("uniqueid"))) {
		uint16_t uid = pugi::cast<uint16_t>(attr.value());
		if (uniqueItemMap.find(uid) != uniqueItemMap.end()) {
			std::cout << "[Warning - Actions::registerEvent] Duplicate registered item with uniqueid: " << uid << std::endl;
			return false;
		}
		uniqueItemMap[uid] = action;
	} else if ((attr = node.attribute("fromuid"))) {
		pugi::xml_attribute toUidAttribute = node.attribute("touid");
		if (toUidAttribute) {
			uint16_t fromUid = pugi::cast<uint16_t>(attr.value());
			uint16_t iterUid = fromUid;
			uint16_t toUid = pugi::cast<uint16_t>(toUidAttribute.value());

			bool success = false;
			if (uniqueItemMap.find(iterUid) != uniqueItemMap.end()) {
				std::cout << "[Warning - Actions::registerEvent] Duplicate registered item with unique id: " << iterUid << " in fromuid: " << fromUid << ", touid: " << toUid << std::endl;
			} else {
				uniqueItemMap[iterUid] = action;
				success = true;
			}

			while (++iterUid <= toUid) {
				if (uniqueItemMap.find(iterUid) != uniqueItemMap.end()) {
					std::cout << "[Warning - Actions::registerEvent] Duplicate registered item with unique id: " << iterUid << " in fromuid: " << fromUid << ", touid: " << toUid << std::endl;
					continue;
				}
				uniqueItemMap[iterUid] = action;
				success = true;
			}
			return success;
		} else {
			std::cout << "[Warning - Actions::registerEvent] Missing touid in fromuid: " << attr.as_string() << std::endl;
			return false;
		}
	} else if ((attr = node.attribute("actionid"))) {
		uint16_t aid = pugi::cast<uint16_t>(attr.value());
		if (actionItemMap.find(aid) != actionItemMap.end()) {
			std::cout << "[Warning - Actions::registerEvent] Duplicate registered item with actionid: " << aid << std::endl;
			return false;
		}
		actionItemMap[aid] = action;
	} else if ((attr = node.attribute("fromaid"))) {
		pugi::xml_attribute toAidAttribute = node.attribute("toaid");
		if (toAidAttribute) {
			uint16_t fromAid = pugi::cast<uint16_t>(attr.value());
			uint16_t iterAid = fromAid;
			uint16_t toAid = pugi::cast<uint16_t>(toAidAttribute.value());

			bool success = false;
			if (actionItemMap.find(iterAid) != actionItemMap.end()) {
				std::cout << "[Warning - Actions::registerEvent] Duplicate registered item with action id: " << iterAid << " in fromaid: " << fromAid << ", toaid: " << toAid << std::endl;
			} else {
				actionItemMap[iterAid] = action;
				success = true;
			}

			while (++iterAid <= toAid) {
				if (actionItemMap.find(iterAid) != actionItemMap.end()) {
					std::cout << "[Warning - Actions::registerEvent] Duplicate registered item with action id: " << iterAid << " in fromaid: " << fromAid << ", toaid: " << toAid << std::endl;
					continue;
				}
				actionItemMap[iterAid] = action;
				success = true;
			}
			return success;
		} else {
			std::cout << "[Warning - Actions::registerEvent] Missing toaid in fromaid: " << attr.as_string() << std::endl;
			return false;
		}
	} else {
		return false;
	}
	return true;
}

ReturnValue Actions::canUse(const Player* player, const Position& pos)
{
	if (pos.x != 0xFFFF) {
		const Position& playerPos = player->getPosition();
		if (playerPos.z != pos.z) {
			return playerPos.z > pos.z ? RET_FIRSTGOUPSTAIRS : RET_FIRSTGODOWNSTAIRS;
		}

		if (!Position::areInRange<1, 1>(playerPos, pos)) {
			return RET_TOOFARAWAY;
		}
	}
	return RET_NOERROR;
}

ReturnValue Actions::canUse(const Player* player, const Position& pos, const Item* item)
{
	Action* action = getAction(item);
	if (action) {
		return action->canExecuteAction(player, pos);
	}
	return RET_NOERROR;
}

ReturnValue Actions::canUseFar(const Creature* creature, const Position& toPos, bool checkLineOfSight, bool checkFloor)
{
	if (toPos.x == 0xFFFF) {
		return RET_NOERROR;
	}

	const Position& creaturePos = creature->getPosition();
	if (checkFloor && creaturePos.z != toPos.z) {
		return creaturePos.z > toPos.z ? RET_FIRSTGOUPSTAIRS : RET_FIRSTGODOWNSTAIRS;
	}

	if (!Position::areInRange<7, 5>(toPos, creaturePos)) {
		return RET_TOOFARAWAY;
	}

	if (checkLineOfSight && !g_game.canThrowObjectTo(creaturePos, toPos)) {
		return RET_CANNOTTHROW;
	}

	return RET_NOERROR;
}

Action* Actions::getAction(const Item* item)
{
	uint16_t uniqueId = item->getUniqueId();
	if (uniqueId != 0) {
		auto it = uniqueItemMap.find(uniqueId);
		if (it != uniqueItemMap.end()) {
			return it->second;
		}
	}

	uint16_t actionId = item->getActionId();
	if (actionId != 0) {
		auto it = actionItemMap.find(actionId);
		if (it != actionItemMap.end()) {
			return it->second;
		}
	}

	auto it = useItemMap.find(item->getID());
	if (it != useItemMap.end()) {
		return it->second;
	}

	//rune items
	Action* runeSpell = g_spells->getRuneSpell(item->getID());
	if (runeSpell) {
		return runeSpell;
	}
	return nullptr;
}

ReturnValue Actions::internalUseItem(Player* player, const Position& pos, uint8_t index, Item* item)
{
	if (Door* door = item->getDoor()) {
		if (!door->canUse(player)) {
			return RET_CANNOTUSETHISOBJECT;
		}
	}

	Action* action = getAction(item);
	if (action) {
		int32_t stack = item->getParent()->__getIndexOfThing(item);
		PositionEx posEx(pos, stack);

		if (action->isScripted()) {
			if (action->executeUse(player, item, posEx, posEx, false, 0)) {
				return RET_NOERROR;
			}
		} else if (action->function) {
			if (action->function(player, item, posEx, posEx, false)) {
				return RET_NOERROR;
			}
		}
	}

	if (BedItem* bed = item->getBed()) {
		if (!bed->canUse(player)) {
			return RET_CANNOTUSETHISOBJECT;
		}

		if (bed->trySleep(player)) {
			player->setBedItem(bed);
			g_game.sendOfflineTrainingDialog(player);
		}

		return RET_NOERROR;
	}

	if (Container* container = item->getContainer()) {
		Container* openContainer;

		//depot container
		if (DepotLocker* depot = container->getDepotLocker()) {
			DepotLocker* myDepotLocker = player->getDepotLocker(depot->getDepotId());
			myDepotLocker->setParent(depot->getParent()->getTile());
			openContainer = myDepotLocker;
			player->setLastDepotId(depot->getDepotId());
		} else {
			openContainer = container;
		}

		uint32_t corpseOwner = container->getCorpseOwner();
		if (corpseOwner != 0 && !player->canOpenCorpse(corpseOwner)) {
			return RET_YOUARENOTTHEOWNER;
		}

		//open/close container
		int32_t oldContainerId = player->getContainerID(openContainer);
		if (oldContainerId != -1) {
			player->onCloseContainer(openContainer);
			player->closeContainer(oldContainerId);
		} else {
			player->addContainer(index, openContainer);
			player->onSendContainer(openContainer);
		}

		return RET_NOERROR;
	}

	if (item->isReadable()) {
		if (item->canWriteText()) {
			player->setWriteItem(item, item->getMaxWriteLength());
			player->sendTextWindow(item, item->getMaxWriteLength(), true);
		} else {
			player->setWriteItem(nullptr);
			player->sendTextWindow(item, 0, false);
		}

		return RET_NOERROR;
	}

	return RET_CANNOTUSETHISOBJECT;
}

bool Actions::useItem(Player* player, const Position& pos, uint8_t index, Item* item, bool isHotkey)
{
	if (!player->canDoAction()) {
		return false;
	}

	player->setNextActionTask(nullptr);
	player->setNextAction(OTSYS_TIME() + g_config.getNumber(ConfigManager::ACTIONS_DELAY_INTERVAL));
	player->stopWalk();

	if (isHotkey) {
		showUseHotkeyMessage(player, item->getID(), player->__getItemTypeCount(item->getID(), -1));
	}

	ReturnValue ret = internalUseItem(player, pos, index, item);
	if (ret != RET_NOERROR) {
		player->sendCancelMessage(ret);
		return false;
	}

	return true;
}

bool Actions::useItemEx(Player* player, const Position& fromPos, const Position& toPos,
                        uint8_t toStackPos, Item* item, bool isHotkey, uint32_t creatureId/* = 0*/)
{
	if (!player->canDoAction()) {
		return false;
	}

	player->setNextActionTask(nullptr);
	player->setNextAction(OTSYS_TIME() + g_config.getNumber(ConfigManager::EX_ACTIONS_DELAY_INTERVAL));
	player->stopWalk();

	Action* action = getAction(item);
	if (!action) {
		player->sendCancelMessage(RET_CANNOTUSETHISOBJECT);
		return false;
	}

	ReturnValue ret = action->canExecuteAction(player, toPos);
	if (ret != RET_NOERROR) {
		player->sendCancelMessage(ret);
		return false;
	}

	if (isHotkey) {
		showUseHotkeyMessage(player, item->getID(), player->__getItemTypeCount(item->getID(), -1));
	}

	int32_t fromStackPos = item->getParent()->__getIndexOfThing(item);
	PositionEx fromPosEx(fromPos, fromStackPos);
	PositionEx toPosEx(toPos, toStackPos);

	if (!action->executeUse(player, item, fromPosEx, toPosEx, true, creatureId)) {
		if (!action->hasOwnErrorHandler()) {
			player->sendCancelMessage(RET_CANNOTUSETHISOBJECT);
		}

		return false;
	}

	return true;
}

void Actions::showUseHotkeyMessage(Player* player, int32_t id, uint32_t count)
{
	const ItemType& it = Item::items[id];
	std::ostringstream ss;

	if (!it.showCount) {
		ss << "Using one of " << it.name << "...";
	} else if (count == 1) {
		ss << "Using the last " << it.name << "...";
	} else {
		ss << "Using one of " << count << ' ' << it.getPluralName() << "...";
	}

	player->sendTextMessage(MESSAGE_INFO_DESCR, ss.str());
}

bool Actions::hasAction(const Item* item)
{
	return getAction(item) != nullptr;
}

Action::Action(LuaScriptInterface* _interface) :
	Event(_interface)
{
	allowFarUse = false;
	checkFloor = true;
	checkLineOfSight = true;
	function = nullptr;
}

Action::Action(const Action* copy) :
	Event(copy)
{
	allowFarUse = copy->allowFarUse;
	checkLineOfSight = copy->checkLineOfSight;
	function = copy->function;
}

Action::~Action()
{
	//
}

bool Action::configureEvent(const pugi::xml_node& node)
{
	pugi::xml_attribute allowFarUseAttr = node.attribute("allowfaruse");
	if (allowFarUseAttr) {
		setAllowFarUse(allowFarUseAttr.as_bool());
	}

	pugi::xml_attribute blockWallsAttr = node.attribute("blockwalls");
	if (blockWallsAttr) {
		setCheckLineOfSight(blockWallsAttr.as_bool());
	}

	pugi::xml_attribute checkFloorAttr = node.attribute("checkfloor");
	if (checkFloorAttr) {
		setCheckFloor(checkFloorAttr.as_bool());
	}

	return true;
}

bool Action::loadFunction(const std::string& functionName)
{
	const std::string& tmpFunctionName = asLowerCaseString(functionName);
	if (tmpFunctionName == "increaseitemid") {
		function = increaseItemId;
	} else if (tmpFunctionName == "decreaseitemid") {
		function = decreaseItemId;
	} else if (tmpFunctionName == "market") {
		function = enterMarket;
	} else {
		std::cout << "[Warning - Action::loadFunction] Function \"" << functionName << "\" does not exist." << std::endl;
		return false;
	}

	m_scripted = false;
	return true;
}

bool Action::increaseItemId(Player*, Item* item, const PositionEx&, const PositionEx&, bool)
{
	Item* newItem = g_game.transformItem(item, item->getID() + 1);
	g_game.startDecay(newItem);
	return true;
}

bool Action::decreaseItemId(Player*, Item* item, const PositionEx&, const PositionEx&, bool)
{
	Item* newItem = g_game.transformItem(item, item->getID() - 1);
	g_game.startDecay(newItem);
	return true;
}

bool Action::enterMarket(Player* player, Item*, const PositionEx&, const PositionEx&, bool)
{
	if (player->getLastDepotId() == -1) {
		return false;
	}

	player->sendMarketEnter(player->getLastDepotId());
	return true;
}

std::string Action::getScriptEventName()
{
	return "onUse";
}

ReturnValue Action::canExecuteAction(const Player* player, const Position& toPos)
{
	if (!getAllowFarUse()) {
		return g_actions->canUse(player, toPos);
	} else {
		return g_actions->canUseFar(player, toPos, getCheckLineOfSight(), getCheckFloor());
	}
}

bool Action::executeUse(Player* player, Item* item, const PositionEx& fromPos, const PositionEx& toPos, bool extendedUse, uint32_t)
{
	//onUse(cid, item, fromPosition, itemEx, toPosition)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - Action::executeUse] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_scriptId, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();

	m_scriptInterface->pushFunction(m_scriptId);
	lua_pushnumber(L, player->getID());
	LuaScriptInterface::pushThing(L, item, env->addThing(item));
	LuaScriptInterface::pushPosition(L, fromPos, fromPos.stackpos);
	Thing* thing = g_game.internalGetThing(player, toPos, toPos.stackpos);
	if (thing && (!extendedUse || thing != item)) {
		LuaScriptInterface::pushThing(L, thing, env->addThing(thing));
		LuaScriptInterface::pushPosition(L, toPos, toPos.stackpos);
	} else {
		LuaScriptInterface::pushThing(L, nullptr, 0);
		Position posEx;
		LuaScriptInterface::pushPosition(L, posEx);
	}
	return m_scriptInterface->callFunction(5);
}
