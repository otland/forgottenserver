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

#include "game.h"
#include "creature.h"
#include "player.h"
#include "tile.h"
#include "tools.h"
#include "combat.h"
#include "vocation.h"

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "movement.h"

extern Game g_game;
extern Vocations g_vocations;
extern MoveEvents* g_moveEvents;

MoveEvents::MoveEvents() :
	m_scriptInterface("MoveEvents Interface")
{
	m_scriptInterface.initState();
}

MoveEvents::~MoveEvents()
{
	clear();
}

void MoveEvents::clear()
{
	MoveListMap::iterator it1 = m_itemIdMap.begin();

	while (it1 != m_itemIdMap.end()) {
		MoveEventList& tmpMoveEventList = it1->second;

		for (int32_t i = 0; i < MOVE_EVENT_LAST; ++i) {
			std::list<MoveEvent*>& moveEventList = tmpMoveEventList.moveEvent[i];

			for (std::list<MoveEvent*>::iterator it = moveEventList.begin(), end = moveEventList.end(); it != end; ++it) {
				delete (*it);
			}
		}

		m_itemIdMap.erase(it1);
		it1 = m_itemIdMap.begin();
	}

	MoveListMap::iterator it2 = m_actionIdMap.begin();

	while (it2 != m_actionIdMap.end()) {
		MoveEventList& tmpMoveEventList = it2->second;

		for (int32_t i = 0; i < MOVE_EVENT_LAST; ++i) {
			std::list<MoveEvent*>& moveEventList = tmpMoveEventList.moveEvent[i];

			for (std::list<MoveEvent*>::iterator it = moveEventList.begin(), end = moveEventList.end(); it != end; ++it) {
				delete (*it);
			}
		}

		m_actionIdMap.erase(it2);
		it2 = m_actionIdMap.begin();
	}

	MoveListMap::iterator it3 = m_uniqueIdMap.begin();

	while (it3 != m_uniqueIdMap.end()) {
		MoveEventList& tmpMoveEventList = it3->second;

		for (int32_t i = 0; i < MOVE_EVENT_LAST; ++i) {
			std::list<MoveEvent*>& moveEventList = tmpMoveEventList.moveEvent[i];

			for (std::list<MoveEvent*>::iterator it = moveEventList.begin(), end = moveEventList.end(); it != end; ++it) {
				delete (*it);
			}
		}

		m_uniqueIdMap.erase(it3);
		it3 = m_uniqueIdMap.begin();
	}

	MovePosListMap::iterator posIter = m_positionMap.begin();

	while (posIter != m_positionMap.end()) {
		MoveEventList& tmpMoveEventList = posIter->second;

		for (int i = 0; i < MOVE_EVENT_LAST; ++i) {
			std::list<MoveEvent*>& moveEventList = tmpMoveEventList.moveEvent[i];

			for (std::list<MoveEvent*>::iterator it = moveEventList.begin(), end = moveEventList.end(); it != end; ++it) {
				delete (*it);
			}
		}

		m_positionMap.erase(posIter);
		posIter = m_positionMap.begin();
	}

	m_scriptInterface.reInitState();
}

LuaScriptInterface& MoveEvents::getScriptInterface()
{
	return m_scriptInterface;
}

std::string MoveEvents::getScriptBaseName()
{
	return "movements";
}

Event* MoveEvents::getEvent(const std::string& nodeName)
{
	if (asLowerCaseString(nodeName) == "movevent") {
		return new MoveEvent(&m_scriptInterface);
	} else {
		return NULL;
	}
}

bool MoveEvents::registerEvent(Event* event, xmlNodePtr p)
{
	MoveEvent* moveEvent = dynamic_cast<MoveEvent*>(event);

	if (!moveEvent) {
		return false;
	}

	bool success = true;
	int32_t id, endId;
	std::string str;

	MoveEvent_t eventType = moveEvent->getEventType();

	if (eventType == MOVE_EVENT_ADD_ITEM || eventType == MOVE_EVENT_REMOVE_ITEM) {
		if (readXMLInteger(p, "tileitem", id) && id == 1) {
			switch (eventType) {
				case MOVE_EVENT_ADD_ITEM:
					moveEvent->setEventType(MOVE_EVENT_ADD_ITEM_ITEMTILE);
					break;
				case MOVE_EVENT_REMOVE_ITEM:
					moveEvent->setEventType(MOVE_EVENT_REMOVE_ITEM_ITEMTILE);
					break;
				default:
					break;
			}
		}
	}

	if (readXMLInteger(p, "itemid", id)) {
		addEvent(moveEvent, id, m_itemIdMap);

		if (moveEvent->getEventType() == MOVE_EVENT_EQUIP) {
			ItemType& it = Item::items.getItemType(id);
			it.wieldInfo = moveEvent->getWieldInfo();
			it.minReqLevel = moveEvent->getReqLevel();
			it.minReqMagicLevel = moveEvent->getReqMagLv();
			it.vocationString = moveEvent->getVocationString();
		}
	} else if (readXMLInteger(p, "fromid", id) && readXMLInteger(p, "toid", endId)) {
		addEvent(moveEvent, id, m_itemIdMap);

		if (moveEvent->getEventType() == MOVE_EVENT_EQUIP) {
			ItemType& it = Item::items.getItemType(id);
			it.wieldInfo = moveEvent->getWieldInfo();
			it.minReqLevel = moveEvent->getReqLevel();
			it.minReqMagicLevel = moveEvent->getReqMagLv();
			it.vocationString = moveEvent->getVocationString();

			while (id < endId) {
				id++;
				addEvent(new MoveEvent(moveEvent), id, m_itemIdMap);

				ItemType& tit = Item::items.getItemType(id);
				tit.wieldInfo = moveEvent->getWieldInfo();
				tit.minReqLevel = moveEvent->getReqLevel();
				tit.minReqMagicLevel = moveEvent->getReqMagLv();
				tit.vocationString = moveEvent->getVocationString();
			}
		} else {
			while (id < endId) {
				addEvent(new MoveEvent(moveEvent), ++id, m_itemIdMap);
			}
		}
	} else if (readXMLInteger(p, "uniqueid", id)) {
		addEvent(moveEvent, id, m_uniqueIdMap);
	} else if (readXMLInteger(p, "fromuid", id) && readXMLInteger(p, "touid", endId)) {
		addEvent(moveEvent, id, m_uniqueIdMap);

		while (id < endId) {
			addEvent(new MoveEvent(moveEvent), ++id, m_uniqueIdMap);
		}
	} else if (readXMLInteger(p, "actionid", id) || readXMLInteger(p, "aid", id)) {
		addEvent(moveEvent, id, m_actionIdMap);
	} else if (readXMLInteger(p, "fromaid", id) && readXMLInteger(p, "toaid", endId)) {
		addEvent(moveEvent, id, m_actionIdMap);

		while (id < endId) {
			addEvent(new MoveEvent(moveEvent), ++id, m_actionIdMap);
		}
	} else if (readXMLString(p, "pos", str)) {
		std::vector<int32_t> posList = vectorAtoi(explodeString(str, ";"));
		if (posList.size() >= 3) {
			Position pos(posList[0], posList[1], posList[2]);
			addEvent(moveEvent, pos, m_positionMap);
		} else {
			success = false;
		}
	} else {
		success = false;
	}

	return success;
}

void MoveEvents::addEvent(MoveEvent* moveEvent, int32_t id, MoveListMap& map)
{
	MoveListMap::iterator it = map.find(id);
	if (it == map.end()) {
		MoveEventList moveEventList;
		moveEventList.moveEvent[moveEvent->getEventType()].push_back(moveEvent);
		map[id] = moveEventList;
	} else {
		std::list<MoveEvent*>& moveEventList = it->second.moveEvent[moveEvent->getEventType()];
		for (MoveEvent* existingMoveEvent : moveEventList) {
			if (existingMoveEvent->getSlot() == moveEvent->getSlot()) {
				std::cout << "Warning: [MoveEvents::addEvent] Duplicate move event found: " << id << std::endl;
			}
		}
		moveEventList.push_back(moveEvent);
	}
}

MoveEvent* MoveEvents::getEvent(Item* item, MoveEvent_t eventType, slots_t slot)
{
	uint32_t slotp;
	switch (slot) {
		case SLOT_HEAD:
			slotp = SLOTP_HEAD;
			break;
		case SLOT_NECKLACE:
			slotp = SLOTP_NECKLACE;
			break;
		case SLOT_BACKPACK:
			slotp = SLOTP_BACKPACK;
			break;
		case SLOT_ARMOR:
			slotp = SLOTP_ARMOR;
			break;
		case SLOT_RIGHT:
			slotp = SLOTP_RIGHT;
			break;
		case SLOT_LEFT:
			slotp = SLOTP_LEFT;
			break;
		case SLOT_LEGS:
			slotp = SLOTP_LEGS;
			break;
		case SLOT_FEET:
			slotp = SLOTP_FEET;
			break;
		case SLOT_AMMO:
			slotp = SLOTP_AMMO;
			break;
		case SLOT_RING:
			slotp = SLOTP_RING;
			break;
		default:
			slotp = 0;
			break;
	}

	MoveListMap::iterator it = m_itemIdMap.find(item->getID());
	if (it != m_itemIdMap.end()) {
		std::list<MoveEvent*>& moveEventList = it->second.moveEvent[eventType];
		for (MoveEvent* moveEvent : moveEventList) {
			if ((moveEvent->getSlot() & slotp) != 0) {
				return moveEvent;
			}
		}
	}
	return NULL;
}

MoveEvent* MoveEvents::getEvent(Item* item, MoveEvent_t eventType)
{
	MoveListMap::iterator it;

	if (item->getUniqueId() != 0) {
		it = m_uniqueIdMap.find(item->getUniqueId());

		if (it != m_uniqueIdMap.end()) {
			std::list<MoveEvent*>& moveEventList = it->second.moveEvent[eventType];

			if (!moveEventList.empty()) {
				return *moveEventList.begin();
			}
		}
	}

	if (item->getActionId() != 0) {
		it = m_actionIdMap.find(item->getActionId());

		if (it != m_actionIdMap.end()) {
			std::list<MoveEvent*>& moveEventList = it->second.moveEvent[eventType];

			if (!moveEventList.empty()) {
				return *moveEventList.begin();
			}
		}
	}

	it = m_itemIdMap.find(item->getID());

	if (it != m_itemIdMap.end()) {
		std::list<MoveEvent*>& moveEventList = it->second.moveEvent[eventType];

		if (!moveEventList.empty()) {
			return *moveEventList.begin();
		}
	}

	return NULL;
}

void MoveEvents::addEvent(MoveEvent* moveEvent, const Position& pos, MovePosListMap& map)
{
	MovePosListMap::iterator it = map.find(pos);

	if (it == map.end()) {
		MoveEventList moveEventList;
		moveEventList.moveEvent[moveEvent->getEventType()].push_back(moveEvent);
		map[pos] = moveEventList;
	} else {
		std::list<MoveEvent*>& moveEventList = it->second.moveEvent[moveEvent->getEventType()];

		if (!moveEventList.empty()) {
			std::cout << "Warning: [MoveEvents::addEvent] Duplicate move event found: " << pos << std::endl;
		}

		moveEventList.push_back(moveEvent);
	}
}

MoveEvent* MoveEvents::getEvent(const Tile* tile, MoveEvent_t eventType)
{
	MovePosListMap::iterator it = m_positionMap.find(tile->getPosition());

	if (it != m_positionMap.end()) {
		std::list<MoveEvent*>& moveEventList = it->second.moveEvent[eventType];

		if (!moveEventList.empty()) {
			return *moveEventList.begin();
		}
	}

	return NULL;
}

uint32_t MoveEvents::onCreatureMove(Creature* creature, const Tile* tile, bool isIn)
{
	MoveEvent_t eventType;

	if (isIn) {
		eventType = MOVE_EVENT_STEP_IN;
	} else {
		eventType = MOVE_EVENT_STEP_OUT;
	}

	Position pos = tile->getPosition();

	uint32_t ret = 1;

	MoveEvent* moveEvent = getEvent(tile, eventType);
	if (moveEvent) {
		ret = ret & moveEvent->fireStepEvent(creature, NULL, pos);
	}

	for (int32_t i = tile->__getFirstIndex(), j = tile->__getLastIndex(); i < j; ++i) {
		Thing* thing = tile->__getThing(i);
		if (thing) {
			Item* tileItem = thing->getItem();
			if (tileItem) {
				moveEvent = getEvent(tileItem, eventType);
				if (moveEvent) {
					ret = ret & moveEvent->fireStepEvent(creature, tileItem, pos);
				}
			}
		}
	}

	return ret;
}

uint32_t MoveEvents::onPlayerEquip(Player* player, Item* item, slots_t slot, bool isCheck)
{
	MoveEvent* moveEvent = getEvent(item, MOVE_EVENT_EQUIP, slot);

	if (moveEvent) {
		return moveEvent->fireEquip(player, item, slot, isCheck);
	}

	return 1;
}

uint32_t MoveEvents::onPlayerDeEquip(Player* player, Item* item, slots_t slot, bool isRemoval)
{
	MoveEvent* moveEvent = getEvent(item, MOVE_EVENT_DEEQUIP, slot);

	if (moveEvent) {
		return moveEvent->fireEquip(player, item, slot, isRemoval);
	}

	return 1;
}

uint32_t MoveEvents::onItemMove(Item* item, Tile* tile, bool isAdd)
{
	MoveEvent_t eventType1;
	MoveEvent_t eventType2;

	if (isAdd) {
		eventType1 = MOVE_EVENT_ADD_ITEM;
		eventType2 = MOVE_EVENT_ADD_ITEM_ITEMTILE;
	} else {
		eventType1 = MOVE_EVENT_REMOVE_ITEM;
		eventType2 = MOVE_EVENT_REMOVE_ITEM_ITEMTILE;
	}

	uint32_t ret = 1;
	MoveEvent* moveEvent = getEvent(tile, eventType1);
	if (moveEvent) {
		ret &= moveEvent->fireAddRemItem(item, NULL, tile->getPosition());
	}

	moveEvent = getEvent(item, eventType1);
	if (moveEvent) {
		ret &= moveEvent->fireAddRemItem(item, NULL, tile->getPosition());
	}

	for (int32_t i = tile->__getFirstIndex(), j = tile->__getLastIndex(); i < j; ++i) {
		Thing* thing = tile->__getThing(i);
		if (thing) {
			Item* tileItem = thing->getItem();
			if (tileItem && tileItem != item) {
				moveEvent = getEvent(tileItem, eventType2);
				if (moveEvent) {
					ret &= moveEvent->fireAddRemItem(item, tileItem, tile->getPosition());
				}
			}
		}
	}
	return ret;
}

MoveEvent::MoveEvent(LuaScriptInterface* _interface) :
	Event(_interface)
{
	m_eventType = MOVE_EVENT_NONE;
	stepFunction = NULL;
	moveFunction = NULL;
	equipFunction = NULL;
	slot = SLOTP_WHEREEVER;
	wieldInfo = 0;
	reqLevel = 0;
	reqMagLevel = 0;
	premium = false;
}

MoveEvent::MoveEvent(const MoveEvent* copy) :
	Event(copy)
{
	m_eventType = copy->m_eventType;
	stepFunction = copy->stepFunction;
	moveFunction = copy->moveFunction;
	equipFunction = copy->equipFunction;
	slot = copy->slot;

	if (copy->m_eventType == MOVE_EVENT_EQUIP) {
		wieldInfo = copy->wieldInfo;
		reqLevel = copy->reqLevel;
		reqMagLevel = copy->reqMagLevel;
		vocationString = copy->vocationString;
		premium = copy->premium;
		vocEquipMap = copy->vocEquipMap;
	}
}

MoveEvent::~MoveEvent()
{
	//
}

std::string MoveEvent::getScriptEventName()
{
	switch (m_eventType) {
		case MOVE_EVENT_STEP_IN:
			return "onStepIn";

		case MOVE_EVENT_STEP_OUT:
			return "onStepOut";

		case MOVE_EVENT_EQUIP:
			return "onEquip";

		case MOVE_EVENT_DEEQUIP:
			return "onDeEquip";

		case MOVE_EVENT_ADD_ITEM:
			return "onAddItem";

		case MOVE_EVENT_REMOVE_ITEM:
			return "onRemoveItem";

		default:
			std::cout << "Error: [MoveEvent::getScriptEventName()] No valid event type." << std::endl;
			return "";
	}
}

bool MoveEvent::configureEvent(xmlNodePtr p)
{
	std::string str;
	int32_t intValue;

	if (readXMLString(p, "event", str)) {
		std::string tmpStr = asLowerCaseString(str);

		if (tmpStr == "stepin") {
			m_eventType = MOVE_EVENT_STEP_IN;
		} else if (tmpStr == "stepout") {
			m_eventType = MOVE_EVENT_STEP_OUT;
		} else if (tmpStr == "equip") {
			m_eventType = MOVE_EVENT_EQUIP;
		} else if (tmpStr == "deequip") {
			m_eventType = MOVE_EVENT_DEEQUIP;
		} else if (tmpStr == "additem") {
			m_eventType = MOVE_EVENT_ADD_ITEM;
		} else if (tmpStr == "removeitem") {
			m_eventType = MOVE_EVENT_REMOVE_ITEM;
		} else {
			std::cout << "Error: [MoveEvent::configureMoveEvent] No valid event name " << str << std::endl;
			return false;
		}

		if (m_eventType == MOVE_EVENT_EQUIP || m_eventType == MOVE_EVENT_DEEQUIP) {
			if (readXMLString(p, "slot", str)) {
				tmpStr = asLowerCaseString(str);
				if (tmpStr == "head") {
					slot = SLOTP_HEAD;
				} else if (tmpStr == "necklace") {
					slot = SLOTP_NECKLACE;
				} else if (tmpStr == "backpack") {
					slot = SLOTP_BACKPACK;
				} else if (tmpStr == "armor") {
					slot = SLOTP_ARMOR;
				} else if (tmpStr == "right-hand") {
					slot = SLOTP_RIGHT;
				} else if (tmpStr == "left-hand") {
					slot = SLOTP_LEFT;
				} else if (tmpStr == "hand" || tmpStr == "shield") {
					slot = SLOTP_RIGHT | SLOTP_LEFT;
				} else if (tmpStr == "legs") {
					slot = SLOTP_LEGS;
				} else if (tmpStr == "feet") {
					slot = SLOTP_FEET;
				} else if (tmpStr == "ring") {
					slot = SLOTP_RING;
				} else if (tmpStr == "ammo") {
					slot = SLOTP_AMMO;
				} else {
					std::cout << "Warning: [MoveEvent::configureMoveEvent] " << "Unknown slot type " << str << std::endl;
				}
			}

			wieldInfo = 0;

			if (readXMLInteger(p, "lvl", intValue) || readXMLInteger(p, "level", intValue)) {
				reqLevel = intValue;

				if (reqLevel > 0) {
					wieldInfo |= WIELDINFO_LEVEL;
				}
			}

			if (readXMLInteger(p, "maglv", intValue) || readXMLInteger(p, "maglevel", intValue)) {
				reqMagLevel = intValue;

				if (reqMagLevel > 0) {
					wieldInfo |= WIELDINFO_MAGLV;
				}
			}

			if (readXMLInteger(p, "prem", intValue) || readXMLInteger(p, "premium", intValue)) {
				premium = (intValue != 0);

				if (premium) {
					wieldInfo |= WIELDINFO_PREMIUM;
				}
			}

			//Gather vocation information
			typedef std::list<std::string> STRING_LIST;
			STRING_LIST vocStringList;
			xmlNodePtr vocationNode = p->children;

			while (vocationNode) {
				if (xmlStrcmp(vocationNode->name, (const xmlChar*)"vocation") == 0) {
					if (readXMLString(vocationNode, "name", str)) {
						int32_t vocationId = g_vocations.getVocationId(str);

						if (vocationId != -1) {
							vocEquipMap[vocationId] = true;
							intValue = 1;
							readXMLInteger(vocationNode, "showInDescription", intValue);

							if (intValue != 0) {
								toLowerCaseString(str);
								vocStringList.push_back(str);
							}
						}
					}
				}

				vocationNode = vocationNode->next;
			}

			if (!vocEquipMap.empty()) {
				wieldInfo |= WIELDINFO_VOCREQ;
			}

			if (!vocStringList.empty()) {
				for (STRING_LIST::iterator it = vocStringList.begin(); it != vocStringList.end(); ++it) {
					if (*it != vocStringList.front()) {
						if (*it != vocStringList.back()) {
							vocationString += ", ";
						} else {
							vocationString += " and ";
						}
					}

					vocationString += *it;
					vocationString += "s";
				}
			}
		}
	} else {
		std::cout << "Error: [MoveEvent::configureMoveEvent] No event found." << std::endl;
		return false;
	}

	return true;
}

bool MoveEvent::loadFunction(const std::string& functionName)
{
	std::string tmpFunctionName = asLowerCaseString(functionName);

	if (tmpFunctionName == "onstepinfield") {
		stepFunction = StepInField;
	} else if (tmpFunctionName == "onstepoutfield") {
		stepFunction = StepOutField;
	} else if (tmpFunctionName == "onaddfield") {
		moveFunction = AddItemField;
	} else if (tmpFunctionName == "onremovefield") {
		moveFunction = RemoveItemField;
	} else if (tmpFunctionName == "onequipitem") {
		equipFunction = EquipItem;
	} else if (tmpFunctionName == "ondeequipitem") {
		equipFunction = DeEquipItem;
	} else {
		std::cout << "[Warning - MoveEvent::loadFunction] Function \"" << functionName << "\" does not exist." << std::endl;
		return false;
	}

	m_scripted = false;
	return true;
}

MoveEvent_t MoveEvent::getEventType() const
{
	if (m_eventType == MOVE_EVENT_NONE) {
		std::cout << "Error: [MoveEvent::getEventType()] MOVE_EVENT_NONE" << std::endl;
		return (MoveEvent_t)0;
	}

	return m_eventType;
}

void MoveEvent::setEventType(MoveEvent_t type)
{
	m_eventType = type;
}

uint32_t MoveEvent::StepInField(Creature* creature, Item* item, const Position& pos)
{
	MagicField* field = item->getMagicField();

	if (field) {
		field->onStepInField(creature);
		return 1;
	}

	return LUA_ERROR_ITEM_NOT_FOUND;
}

uint32_t MoveEvent::StepOutField(Creature* creature, Item* item, const Position& pos)
{
	return 1;
}

uint32_t MoveEvent::AddItemField(Item* item, Item* tileItem, const Position& pos)
{
	if (MagicField* field = item->getMagicField()) {
		Tile* tile = item->getTile();

		if (CreatureVector* creatures = tile->getCreatures()) {
			for (CreatureVector::iterator cit = creatures->begin(); cit != creatures->end(); ++cit) {
				field->onStepInField(*cit);
			}
		}

		return 1;
	}

	return LUA_ERROR_ITEM_NOT_FOUND;
}

uint32_t MoveEvent::RemoveItemField(Item* item, Item* tileItem, const Position& pos)
{
	return 1;
}

uint32_t MoveEvent::EquipItem(MoveEvent* moveEvent, Player* player, Item* item, slots_t slot, bool isCheck)
{
	if (player->isItemAbilityEnabled(slot)) {
		return 1;
	}

	if (!player->hasFlag(PlayerFlag_IgnoreWeaponCheck) && moveEvent->getWieldInfo() != 0) {
		if (player->getLevel() < (uint32_t)moveEvent->getReqLevel() || player->getMagicLevel() < moveEvent->getReqMagLv()) {
			return 0;
		}

		if (moveEvent->isPremium() && !player->isPremium()) {
			return 0;
		}

		const VocEquipMap& vocEquipMap = moveEvent->getVocEquipMap();

		if (!vocEquipMap.empty() && vocEquipMap.find(player->getVocationId()) == vocEquipMap.end()) {
			return 0;
		}
	}

	if (isCheck) {
		return 1;
	}

	const ItemType& it = Item::items[item->getID()];

	if (it.transformEquipTo != 0) {
		Item* newItem = g_game.transformItem(item, it.transformEquipTo);
		g_game.startDecay(newItem);
	} else {
		player->setItemAbility(slot, true);
	}

	if (!it.abilities) {
		return 1;
	}

	if (it.abilities->invisible) {
		Condition* condition = Condition::createCondition((ConditionId_t)slot, CONDITION_INVISIBLE, -1, 0);
		player->addCondition(condition);
	}

	if (it.abilities->manaShield) {
		Condition* condition = Condition::createCondition((ConditionId_t)slot, CONDITION_MANASHIELD, -1, 0);
		player->addCondition(condition);
	}

	if (it.abilities->speed != 0) {
		g_game.changeSpeed(player, it.abilities->speed);
	}

	if (it.abilities->conditionSuppressions != 0) {
		player->setConditionSuppressions(it.abilities->conditionSuppressions, false);
		player->sendIcons();
	}

	if (it.abilities->regeneration) {
		Condition* condition = Condition::createCondition((ConditionId_t)slot, CONDITION_REGENERATION, -1, 0);

		if (it.abilities->healthGain != 0) {
			condition->setParam(CONDITIONPARAM_HEALTHGAIN, it.abilities->healthGain);
		}

		if (it.abilities->healthTicks != 0) {
			condition->setParam(CONDITIONPARAM_HEALTHTICKS, it.abilities->healthTicks);
		}

		if (it.abilities->manaGain != 0) {
			condition->setParam(CONDITIONPARAM_MANAGAIN, it.abilities->manaGain);
		}

		if (it.abilities->manaTicks != 0) {
			condition->setParam(CONDITIONPARAM_MANATICKS, it.abilities->manaTicks);
		}

		player->addCondition(condition);
	}

	//skill modifiers
	bool needUpdateSkills = false;

	for (int32_t i = SKILL_FIRST; i <= SKILL_LAST; ++i) {
		if (it.abilities->skills[i]) {
			needUpdateSkills = true;
			player->setVarSkill((skills_t)i, it.abilities->skills[i]);
		}
	}

	if (needUpdateSkills) {
		player->sendSkills();
	}

	//stat modifiers
	bool needUpdateStats = false;

	for (int32_t s = STAT_FIRST; s <= STAT_LAST; ++s) {
		if (it.abilities->stats[s]) {
			needUpdateStats = true;
			player->setVarStats((stats_t)s, it.abilities->stats[s]);
		}

		if (it.abilities->statsPercent[s]) {
			needUpdateStats = true;
			player->setVarStats((stats_t)s, (int32_t)(player->getDefaultStats((stats_t)s) * ((it.abilities->statsPercent[s] - 100) / 100.f)));
		}
	}

	if (needUpdateStats) {
		player->sendStats();
	}

	return 1;
}

uint32_t MoveEvent::DeEquipItem(MoveEvent* moveEvent, Player* player, Item* item, slots_t slot, bool isRemoval)
{
	if (!player->isItemAbilityEnabled(slot)) {
		return 1;
	}

	player->setItemAbility(slot, false);

	const ItemType& it = Item::items[item->getID()];

	if (isRemoval && it.transformDeEquipTo != 0) {
		g_game.transformItem(item, it.transformDeEquipTo);
		g_game.startDecay(item);
	}

	if (!it.abilities) {
		return 1;
	}

	if (it.abilities->invisible) {
		player->removeCondition(CONDITION_INVISIBLE, (ConditionId_t)slot);
	}

	if (it.abilities->manaShield) {
		player->removeCondition(CONDITION_MANASHIELD, (ConditionId_t)slot);
	}

	if (it.abilities->speed != 0) {
		g_game.changeSpeed(player, -it.abilities->speed);
	}

	if (it.abilities->conditionSuppressions != 0) {
		player->setConditionSuppressions(it.abilities->conditionSuppressions, true);
		player->sendIcons();
	}

	if (it.abilities->regeneration) {
		player->removeCondition(CONDITION_REGENERATION, (ConditionId_t)slot);
	}

	//skill modifiers
	bool needUpdateSkills = false;

	for (int32_t i = SKILL_FIRST; i <= SKILL_LAST; ++i) {
		if (it.abilities->skills[i] != 0) {
			needUpdateSkills = true;
			player->setVarSkill((skills_t)i, -it.abilities->skills[i]);
		}
	}

	if (needUpdateSkills) {
		player->sendSkills();
	}

	//stat modifiers
	bool needUpdateStats = false;

	for (int32_t s = STAT_FIRST; s <= STAT_LAST; ++s) {
		if (it.abilities->stats[s]) {
			needUpdateStats = true;
			player->setVarStats((stats_t)s, -it.abilities->stats[s]);
		}

		if (it.abilities->statsPercent[s]) {
			needUpdateStats = true;
			player->setVarStats((stats_t)s, -(int32_t)(player->getDefaultStats((stats_t)s) * ((it.abilities->statsPercent[s] - 100) / 100.f)));
		}
	}

	if (needUpdateStats) {
		player->sendStats();
	}

	return 1;
}

uint32_t MoveEvent::fireStepEvent(Creature* creature, Item* item, const Position& pos)
{
	if (m_scripted) {
		return executeStep(creature, item, pos);
	} else {
		return stepFunction(creature, item, pos);
	}
}

bool MoveEvent::executeStep(Creature* creature, Item* item, const Position& pos)
{
	//onStepIn(cid, item, pos, fromPosition)
	//onStepOut(cid, item, pos, fromPosition)
	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();

	env->setScriptId(m_scriptId, m_scriptInterface);
	env->setRealPos(creature->getPosition());

	uint32_t itemid = env->addThing(item);

	lua_State* L = m_scriptInterface->getLuaState();

	m_scriptInterface->pushFunction(m_scriptId);
	lua_pushnumber(L, creature->getID());
	LuaScriptInterface::pushThing(L, item, itemid);
	LuaScriptInterface::pushPosition(L, pos, 0);
	LuaScriptInterface::pushPosition(L, creature->getLastPosition(), 0);

	return m_scriptInterface->callFunction(4);
}

uint32_t MoveEvent::fireEquip(Player* player, Item* item, slots_t slot, bool boolean)
{
	if (m_scripted) {
		return executeEquip(player, item, slot);
	} else {
		return equipFunction(this, player, item, slot, boolean);
	}
}

bool MoveEvent::executeEquip(Player* player, Item* item, slots_t slot)
{
	//onEquip(cid, item, slot)
	//onDeEquip(cid, item, slot)
	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();

	env->setScriptId(m_scriptId, m_scriptInterface);
	env->setRealPos(player->getPosition());

	uint32_t itemid = env->addThing(item);

	lua_State* L = m_scriptInterface->getLuaState();

	m_scriptInterface->pushFunction(m_scriptId);
	lua_pushnumber(L, player->getID());
	LuaScriptInterface::pushThing(L, item, itemid);
	lua_pushnumber(L, slot);

	return m_scriptInterface->callFunction(3);
}

uint32_t MoveEvent::fireAddRemItem(Item* item, Item* tileItem, const Position& pos)
{
	if (m_scripted) {
		return executeAddRemItem(item, tileItem, pos);
	} else {
		return moveFunction(item, tileItem, pos);
	}
}

bool MoveEvent::executeAddRemItem(Item* item, Item* tileItem, const Position& pos)
{
	//onAddItem(moveitem, tileitem, pos)
	//onRemoveItem(moveitem, tileitem, pos)
	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();

	env->setScriptId(m_scriptId, m_scriptInterface);
	env->setRealPos(pos);

	uint32_t itemidMoved = env->addThing(item);
	uint32_t itemidTile = env->addThing(tileItem);

	lua_State* L = m_scriptInterface->getLuaState();

	m_scriptInterface->pushFunction(m_scriptId);
	LuaScriptInterface::pushThing(L, item, itemidMoved);
	LuaScriptInterface::pushThing(L, tileItem, itemidTile);
	LuaScriptInterface::pushPosition(L, pos, 0);

	return m_scriptInterface->callFunction(3);
}
