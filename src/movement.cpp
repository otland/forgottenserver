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

#include "game.h"

#include "pugicast.h"

#include "movement.h"

extern Game g_game;
extern Vocations g_vocations;

MoveEvents::MoveEvents() :
	scriptInterface("MoveEvents Interface")
{
	scriptInterface.initState();
}

MoveEvents::~MoveEvents()
{
	clear(false);
}

void MoveEvents::clearMap(MoveListMap& map, bool fromLua)
{
	for (auto it = map.begin(); it != map.end(); ++it) {
		for (int eventType = MOVE_EVENT_STEP_IN; eventType < MOVE_EVENT_LAST; ++eventType) {
			auto& moveEvents = it->second.moveEvent[eventType];
			for (auto find = moveEvents.begin(); find != moveEvents.end(); ) {
				if (fromLua == find->fromLua) {
					find = moveEvents.erase(find);
				} else {
					++find;
				}
			}
		}
	}
}

void MoveEvents::clearPosMap(MovePosListMap& map, bool fromLua)
{
	for (auto it = map.begin(); it != map.end(); ++it) {
		for (int eventType = MOVE_EVENT_STEP_IN; eventType < MOVE_EVENT_LAST; ++eventType) {
			auto& moveEvents = it->second.moveEvent[eventType];
			for (auto find = moveEvents.begin(); find != moveEvents.end(); ) {
				if (fromLua == find->fromLua) {
					find = moveEvents.erase(find);
				} else {
					++find;
				}
			}
		}
	}
}

void MoveEvents::clear(bool fromLua)
{
	clearMap(itemIdMap, fromLua);
	clearMap(actionIdMap, fromLua);
	clearMap(uniqueIdMap, fromLua);
	clearPosMap(positionMap, fromLua);

	reInitState(fromLua);
}

LuaScriptInterface& MoveEvents::getScriptInterface()
{
	return scriptInterface;
}

std::string MoveEvents::getScriptBaseName() const
{
	return "movements";
}

Event_ptr MoveEvents::getEvent(const std::string& nodeName)
{
	if (strcasecmp(nodeName.c_str(), "movevent") != 0) {
		return nullptr;
	}
	return Event_ptr(new MoveEvent(&scriptInterface));
}

bool MoveEvents::registerEvent(Event_ptr event, const pugi::xml_node& node)
{
	MoveEvent_ptr moveEvent{static_cast<MoveEvent*>(event.release())}; //event is guaranteed to be a MoveEvent

	const MoveEvent_t eventType = moveEvent->getEventType();
	if (eventType == MOVE_EVENT_ADD_ITEM || eventType == MOVE_EVENT_REMOVE_ITEM) {
		pugi::xml_attribute tileItemAttribute = node.attribute("tileitem");
		if (tileItemAttribute && pugi::cast<uint16_t>(tileItemAttribute.value()) == 1) {
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

	pugi::xml_attribute attr;
	if ((attr = node.attribute("itemid"))) {
		std::vector<int32_t> idList = vectorAtoi(explodeString(attr.as_string(), ";"));

		for (const auto& id : idList) {
			if (moveEvent->getEventType() == MOVE_EVENT_EQUIP) {
				ItemType& it = Item::items.getItemType(id);
				it.wieldInfo = moveEvent->getWieldInfo();
				it.minReqLevel = moveEvent->getReqLevel();
				it.minReqMagicLevel = moveEvent->getReqMagLv();
				it.vocationString = moveEvent->getVocationString();
			}
			addEvent(std::move(*moveEvent), id, itemIdMap);
		}
	} else if ((attr = node.attribute("fromid"))) {
		uint32_t id = pugi::cast<uint32_t>(attr.value());
		uint32_t endId = pugi::cast<uint32_t>(node.attribute("toid").value());

		addEvent(*moveEvent, id, itemIdMap);

		if (moveEvent->getEventType() == MOVE_EVENT_EQUIP) {
			ItemType& it = Item::items.getItemType(id);
			it.wieldInfo = moveEvent->getWieldInfo();
			it.minReqLevel = moveEvent->getReqLevel();
			it.minReqMagicLevel = moveEvent->getReqMagLv();
			it.vocationString = moveEvent->getVocationString();

			while (++id <= endId) {
				addEvent(*moveEvent, id, itemIdMap);

				ItemType& tit = Item::items.getItemType(id);
				tit.wieldInfo = moveEvent->getWieldInfo();
				tit.minReqLevel = moveEvent->getReqLevel();
				tit.minReqMagicLevel = moveEvent->getReqMagLv();
				tit.vocationString = moveEvent->getVocationString();
			}
		} else {
			while (++id <= endId) {
				addEvent(*moveEvent, id, itemIdMap);
			}
		}
	} else if ((attr = node.attribute("uniqueid"))) {
		std::vector<int32_t> uidList = vectorAtoi(explodeString(attr.as_string(), ";"));

		for (const auto& uid : uidList) {
			addEvent(std::move(*moveEvent), uid, uniqueIdMap);
		}
	} else if ((attr = node.attribute("fromuid"))) {
		uint32_t id = pugi::cast<uint32_t>(attr.value());
		uint32_t endId = pugi::cast<uint32_t>(node.attribute("touid").value());
		addEvent(*moveEvent, id, uniqueIdMap);
		while (++id <= endId) {
			addEvent(*moveEvent, id, uniqueIdMap);
		}
	} else if ((attr = node.attribute("actionid"))) {
		std::vector<int32_t> aidList = vectorAtoi(explodeString(attr.as_string(), ";"));

		for (const auto& aid : aidList) {
			addEvent(std::move(*moveEvent), aid, actionIdMap);
		}
	} else if ((attr = node.attribute("fromaid"))) {
		uint32_t id = pugi::cast<uint32_t>(attr.value());
		uint32_t endId = pugi::cast<uint32_t>(node.attribute("toaid").value());
		addEvent(*moveEvent, id, actionIdMap);
		while (++id <= endId) {
			addEvent(*moveEvent, id, actionIdMap);
		}
	} else if ((attr = node.attribute("pos"))) {
		std::vector<int32_t> posList = vectorAtoi(explodeString(attr.as_string(), ";"));
		if (posList.size() < 3) {
			return false;
		}

		Position pos(posList[0], posList[1], posList[2]);
		addEvent(std::move(*moveEvent), pos, positionMap);
	} else {
		return false;
	}
	return true;
}

bool MoveEvents::registerLuaFunction(MoveEvent* event)
{
	MoveEvent_ptr moveEvent{ event };
	if (moveEvent->getItemIdRange().size() > 0) {
		if (moveEvent->getItemIdRange().size() == 1) {
			uint32_t id = moveEvent->getItemIdRange().at(0);
			addEvent(*moveEvent, id, itemIdMap);
			if (moveEvent->getEventType() == MOVE_EVENT_EQUIP) {
				ItemType& it = Item::items.getItemType(id);
				it.wieldInfo = moveEvent->getWieldInfo();
				it.minReqLevel = moveEvent->getReqLevel();
				it.minReqMagicLevel = moveEvent->getReqMagLv();
				it.vocationString = moveEvent->getVocationString();
			}
		} else {
			uint32_t iterId = 0;
			while (++iterId < moveEvent->getItemIdRange().size()) {
				if (moveEvent->getEventType() == MOVE_EVENT_EQUIP) {
					ItemType& it = Item::items.getItemType(moveEvent->getItemIdRange().at(iterId));
					it.wieldInfo = moveEvent->getWieldInfo();
					it.minReqLevel = moveEvent->getReqLevel();
					it.minReqMagicLevel = moveEvent->getReqMagLv();
					it.vocationString = moveEvent->getVocationString();
				}
				addEvent(*moveEvent, moveEvent->getItemIdRange().at(iterId), itemIdMap);
			}
		}
	} else {
		return false;
	}
	return true;
}

bool MoveEvents::registerLuaEvent(MoveEvent* event)
{
	MoveEvent_ptr moveEvent{ event };
	if (moveEvent->getItemIdRange().size() > 0) {
		if (moveEvent->getItemIdRange().size() == 1) {
			uint32_t id = moveEvent->getItemIdRange().at(0);
			addEvent(*moveEvent, id, itemIdMap);
			if (moveEvent->getEventType() == MOVE_EVENT_EQUIP) {
				ItemType& it = Item::items.getItemType(id);
				it.wieldInfo = moveEvent->getWieldInfo();
				it.minReqLevel = moveEvent->getReqLevel();
				it.minReqMagicLevel = moveEvent->getReqMagLv();
				it.vocationString = moveEvent->getVocationString();
			}
		} else {
			auto v = moveEvent->getItemIdRange();
			for (auto i = v.begin(); i != v.end(); i++) {
				if (moveEvent->getEventType() == MOVE_EVENT_EQUIP) {
					ItemType& it = Item::items.getItemType(*i);
					it.wieldInfo = moveEvent->getWieldInfo();
					it.minReqLevel = moveEvent->getReqLevel();
					it.minReqMagicLevel = moveEvent->getReqMagLv();
					it.vocationString = moveEvent->getVocationString();
				}
				addEvent(*moveEvent, *i, itemIdMap);
			}
		}
	} else if (moveEvent->getActionIdRange().size() > 0) {
		if (moveEvent->getActionIdRange().size() == 1) {
			int32_t id = moveEvent->getActionIdRange().at(0);
			addEvent(*moveEvent, id, actionIdMap);
		} else {
			auto v = moveEvent->getActionIdRange();
			for (auto i = v.begin(); i != v.end(); i++) {
				addEvent(*moveEvent, *i, actionIdMap);
			}
		}
	} else if (moveEvent->getUniqueIdRange().size() > 0) {
		if (moveEvent->getUniqueIdRange().size() == 1) {
			int32_t id = moveEvent->getUniqueIdRange().at(0);
			addEvent(*moveEvent, id, uniqueIdMap);
		} else {
			auto v = moveEvent->getUniqueIdRange();
			for (auto i = v.begin(); i != v.end(); i++) {
				addEvent(*moveEvent, *i, uniqueIdMap);
			}
		}
	} else if (moveEvent->getPosList().size() > 0) {
		if (moveEvent->getPosList().size() == 1) {
			Position pos = moveEvent->getPosList().at(0);
			addEvent(*moveEvent, pos, positionMap);
		} else {
			auto v = moveEvent->getPosList();
			for (auto i = v.begin(); i != v.end(); i++) {
				addEvent(*moveEvent, *i, positionMap);
			}
		}
	} else {
		return false;
	}

	return true;
}

void MoveEvents::addEvent(MoveEvent moveEvent, int32_t id, MoveListMap& map)
{
	auto it = map.find(id);
	if (it == map.end()) {
		MoveEventList moveEventList;
		moveEventList.moveEvent[moveEvent.getEventType()].push_back(std::move(moveEvent));
		map[id] = moveEventList;
	} else {
		std::list<MoveEvent>& moveEventList = it->second.moveEvent[moveEvent.getEventType()];
		for (MoveEvent& existingMoveEvent : moveEventList) {
			if (existingMoveEvent.getSlot() == moveEvent.getSlot()) {
				std::cout << "[Warning - MoveEvents::addEvent] Duplicate move event found: " << id << std::endl;
			}
		}
		moveEventList.push_back(std::move(moveEvent));
	}
}

MoveEvent* MoveEvents::getEvent(Item* item, MoveEvent_t eventType, slots_t slot)
{
	uint32_t slotp;
	switch (slot) {
		case CONST_SLOT_HEAD: slotp = SLOTP_HEAD; break;
		case CONST_SLOT_NECKLACE: slotp = SLOTP_NECKLACE; break;
		case CONST_SLOT_BACKPACK: slotp = SLOTP_BACKPACK; break;
		case CONST_SLOT_ARMOR: slotp = SLOTP_ARMOR; break;
		case CONST_SLOT_RIGHT: slotp = SLOTP_RIGHT; break;
		case CONST_SLOT_LEFT: slotp = SLOTP_LEFT; break;
		case CONST_SLOT_LEGS: slotp = SLOTP_LEGS; break;
		case CONST_SLOT_FEET: slotp = SLOTP_FEET; break;
		case CONST_SLOT_AMMO: slotp = SLOTP_AMMO; break;
		case CONST_SLOT_RING: slotp = SLOTP_RING; break;
		default: slotp = 0; break;
	}

	auto it = itemIdMap.find(item->getID());
	if (it != itemIdMap.end()) {
		std::list<MoveEvent>& moveEventList = it->second.moveEvent[eventType];
		for (MoveEvent& moveEvent : moveEventList) {
			if ((moveEvent.getSlot() & slotp) != 0) {
				return &moveEvent;
			}
		}
	}
	return nullptr;
}

MoveEvent* MoveEvents::getEvent(Item* item, MoveEvent_t eventType)
{
	MoveListMap::iterator it;

	if (item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
		it = uniqueIdMap.find(item->getUniqueId());
		if (it != uniqueIdMap.end()) {
			std::list<MoveEvent>& moveEventList = it->second.moveEvent[eventType];
			if (!moveEventList.empty()) {
				return &(*moveEventList.begin());
			}
		}
	}

	if (item->hasAttribute(ITEM_ATTRIBUTE_ACTIONID)) {
		it = actionIdMap.find(item->getActionId());
		if (it != actionIdMap.end()) {
			std::list<MoveEvent>& moveEventList = it->second.moveEvent[eventType];
			if (!moveEventList.empty()) {
				return &(*moveEventList.begin());
			}
		}
	}

	it = itemIdMap.find(item->getID());
	if (it != itemIdMap.end()) {
		std::list<MoveEvent>& moveEventList = it->second.moveEvent[eventType];
		if (!moveEventList.empty()) {
			return &(*moveEventList.begin());
		}
	}
	return nullptr;
}

void MoveEvents::addEvent(MoveEvent moveEvent, const Position& pos, MovePosListMap& map)
{
	auto it = map.find(pos);
	if (it == map.end()) {
		MoveEventList moveEventList;
		moveEventList.moveEvent[moveEvent.getEventType()].push_back(std::move(moveEvent));
		map[pos] = moveEventList;
	} else {
		std::list<MoveEvent>& moveEventList = it->second.moveEvent[moveEvent.getEventType()];
		if (!moveEventList.empty()) {
			std::cout << "[Warning - MoveEvents::addEvent] Duplicate move event found: " << pos << std::endl;
		}

		moveEventList.push_back(std::move(moveEvent));
	}
}

MoveEvent* MoveEvents::getEvent(const Tile* tile, MoveEvent_t eventType)
{
	auto it = positionMap.find(tile->getPosition());
	if (it != positionMap.end()) {
		std::list<MoveEvent>& moveEventList = it->second.moveEvent[eventType];
		if (!moveEventList.empty()) {
			return &(*moveEventList.begin());
		}
	}
	return nullptr;
}

uint32_t MoveEvents::onCreatureMove(Creature* creature, const Tile* tile, MoveEvent_t eventType)
{
	const Position& pos = tile->getPosition();

	uint32_t ret = 1;

	MoveEvent* moveEvent = getEvent(tile, eventType);
	if (moveEvent) {
		ret &= moveEvent->fireStepEvent(creature, nullptr, pos);
	}

	for (size_t i = tile->getFirstIndex(), j = tile->getLastIndex(); i < j; ++i) {
		Thing* thing = tile->getThing(i);
		if (!thing) {
			continue;
		}

		Item* tileItem = thing->getItem();
		if (!tileItem) {
			continue;
		}

		moveEvent = getEvent(tileItem, eventType);
		if (moveEvent) {
			ret &= moveEvent->fireStepEvent(creature, tileItem, pos);
		}
	}
	return ret;
}

ReturnValue MoveEvents::onPlayerEquip(Player* player, Item* item, slots_t slot, bool isCheck)
{
	MoveEvent* moveEvent = getEvent(item, MOVE_EVENT_EQUIP, slot);
	if (!moveEvent) {
		return RETURNVALUE_NOERROR;
	}
	return moveEvent->fireEquip(player, item, slot, isCheck);
}

ReturnValue MoveEvents::onPlayerDeEquip(Player* player, Item* item, slots_t slot)
{
	MoveEvent* moveEvent = getEvent(item, MOVE_EVENT_DEEQUIP, slot);
	if (!moveEvent) {
		return RETURNVALUE_NOERROR;
	}
	return moveEvent->fireEquip(player, item, slot, false);
}

uint32_t MoveEvents::onItemMove(Item* item, Tile* tile, bool isAdd)
{
	MoveEvent_t eventType1, eventType2;
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
		ret &= moveEvent->fireAddRemItem(item, nullptr, tile->getPosition());
	}

	moveEvent = getEvent(item, eventType1);
	if (moveEvent) {
		ret &= moveEvent->fireAddRemItem(item, nullptr, tile->getPosition());
	}

	for (size_t i = tile->getFirstIndex(), j = tile->getLastIndex(); i < j; ++i) {
		Thing* thing = tile->getThing(i);
		if (!thing) {
			continue;
		}

		Item* tileItem = thing->getItem();
		if (!tileItem || tileItem == item) {
			continue;
		}

		moveEvent = getEvent(tileItem, eventType2);
		if (moveEvent) {
			ret &= moveEvent->fireAddRemItem(item, tileItem, tile->getPosition());
		}
	}
	return ret;
}

MoveEvent::MoveEvent(LuaScriptInterface* interface) : Event(interface) {}

std::string MoveEvent::getScriptEventName() const
{
	switch (eventType) {
		case MOVE_EVENT_STEP_IN: return "onStepIn";
		case MOVE_EVENT_STEP_OUT: return "onStepOut";
		case MOVE_EVENT_EQUIP: return "onEquip";
		case MOVE_EVENT_DEEQUIP: return "onDeEquip";
		case MOVE_EVENT_ADD_ITEM: return "onAddItem";
		case MOVE_EVENT_REMOVE_ITEM: return "onRemoveItem";
		default:
			std::cout << "[Error - MoveEvent::getScriptEventName] Invalid event type" << std::endl;
			return std::string();
	}
}

bool MoveEvent::configureEvent(const pugi::xml_node& node)
{
	pugi::xml_attribute eventAttr = node.attribute("event");
	if (!eventAttr) {
		std::cout << "[Error - MoveEvent::configureMoveEvent] Missing event" << std::endl;
		return false;
	}

	std::string tmpStr = asLowerCaseString(eventAttr.as_string());
	if (tmpStr == "stepin") {
		eventType = MOVE_EVENT_STEP_IN;
	} else if (tmpStr == "stepout") {
		eventType = MOVE_EVENT_STEP_OUT;
	} else if (tmpStr == "equip") {
		eventType = MOVE_EVENT_EQUIP;
	} else if (tmpStr == "deequip") {
		eventType = MOVE_EVENT_DEEQUIP;
	} else if (tmpStr == "additem") {
		eventType = MOVE_EVENT_ADD_ITEM;
	} else if (tmpStr == "removeitem") {
		eventType = MOVE_EVENT_REMOVE_ITEM;
	} else {
		std::cout << "Error: [MoveEvent::configureMoveEvent] No valid event name " << eventAttr.as_string() << std::endl;
		return false;
	}

	if (eventType == MOVE_EVENT_EQUIP || eventType == MOVE_EVENT_DEEQUIP) {
		pugi::xml_attribute slotAttribute = node.attribute("slot");
		if (slotAttribute) {
			tmpStr = asLowerCaseString(slotAttribute.as_string());
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
				std::cout << "[Warning - MoveEvent::configureMoveEvent] Unknown slot type: " << slotAttribute.as_string() << std::endl;
			}
		}

		wieldInfo = 0;

		pugi::xml_attribute levelAttribute = node.attribute("level");
		if (levelAttribute) {
			reqLevel = pugi::cast<uint32_t>(levelAttribute.value());
			if (reqLevel > 0) {
				wieldInfo |= WIELDINFO_LEVEL;
			}
		}

		pugi::xml_attribute magLevelAttribute = node.attribute("maglevel");
		if (magLevelAttribute) {
			reqMagLevel = pugi::cast<uint32_t>(magLevelAttribute.value());
			if (reqMagLevel > 0) {
				wieldInfo |= WIELDINFO_MAGLV;
			}
		}

		pugi::xml_attribute premiumAttribute = node.attribute("premium");
		if (premiumAttribute) {
			premium = premiumAttribute.as_bool();
			if (premium) {
				wieldInfo |= WIELDINFO_PREMIUM;
			}
		}

		//Gather vocation information
		std::list<std::string> vocStringList;
		for (auto vocationNode : node.children()) {
			pugi::xml_attribute vocationNameAttribute = vocationNode.attribute("name");
			if (!vocationNameAttribute) {
				continue;
			}

			int32_t vocationId = g_vocations.getVocationId(vocationNameAttribute.as_string());
			if (vocationId != -1) {
				vocEquipMap[vocationId] = true;
				if (vocationNode.attribute("showInDescription").as_bool(true)) {
					vocStringList.push_back(asLowerCaseString(vocationNameAttribute.as_string()));
				}
			}
		}

		if (!vocEquipMap.empty()) {
			wieldInfo |= WIELDINFO_VOCREQ;
		}

		for (const std::string& str : vocStringList) {
			if (!vocationString.empty()) {
				if (str != vocStringList.back()) {
					vocationString.push_back(',');
					vocationString.push_back(' ');
				} else {
					vocationString += " and ";
				}
			}

			vocationString += str;
			vocationString.push_back('s');
		}
	}
	return true;
}

uint32_t MoveEvent::StepInField(Creature* creature, Item* item, const Position&)
{
	MagicField* field = item->getMagicField();
	if (field) {
		field->onStepInField(creature);
		return 1;
	}

	return LUA_ERROR_ITEM_NOT_FOUND;
}

uint32_t MoveEvent::StepOutField(Creature*, Item*, const Position&)
{
	return 1;
}

uint32_t MoveEvent::AddItemField(Item* item, Item*, const Position&)
{
	if (MagicField* field = item->getMagicField()) {
		Tile* tile = item->getTile();
		if (CreatureVector* creatures = tile->getCreatures()) {
			for (Creature* creature : *creatures) {
				field->onStepInField(creature);
			}
		}
		return 1;
	}
	return LUA_ERROR_ITEM_NOT_FOUND;
}

uint32_t MoveEvent::RemoveItemField(Item*, Item*, const Position&)
{
	return 1;
}

ReturnValue MoveEvent::EquipItem(MoveEvent* moveEvent, Player* player, Item* item, slots_t slot, bool isCheck)
{
	if (!player->hasFlag(PlayerFlag_IgnoreWeaponCheck) && moveEvent->getWieldInfo() != 0) {
		const VocEquipMap& vocEquipMap = moveEvent->getVocEquipMap();
		if (!vocEquipMap.empty() && vocEquipMap.find(player->getVocationId()) == vocEquipMap.end()) {
			return RETURNVALUE_YOUDONTHAVEREQUIREDPROFESSION;
		}

		if (player->getLevel() < moveEvent->getReqLevel()) {
			return RETURNVALUE_NOTENOUGHLEVEL;
		}

		if (player->getMagicLevel() < moveEvent->getReqMagLv()) {
			return RETURNVALUE_NOTENOUGHMAGICLEVEL;
		}

		if (moveEvent->isPremium() && !player->isPremium()) {
			return RETURNVALUE_YOUNEEDPREMIUMACCOUNT;
		}
	}

	if (isCheck) {
		return RETURNVALUE_NOERROR;
	}

	if (player->isItemAbilityEnabled(slot)) {
		return RETURNVALUE_NOERROR;
	}

	const ItemType& it = Item::items[item->getID()];
	if (it.transformEquipTo != 0) {
		Item* newItem = g_game.transformItem(item, it.transformEquipTo);
		g_game.startDecay(newItem);
	} else {
		player->setItemAbility(slot, true);
	}

	if (!it.abilities) {
		return RETURNVALUE_NOERROR;
	}

	if (it.abilities->invisible) {
		Condition* condition = Condition::createCondition(static_cast<ConditionId_t>(slot), CONDITION_INVISIBLE, -1, 0);
		player->addCondition(condition);
	}

	if (it.abilities->manaShield) {
		Condition* condition = Condition::createCondition(static_cast<ConditionId_t>(slot), CONDITION_MANASHIELD, -1, 0);
		player->addCondition(condition);
	}

	if (it.abilities->speed != 0) {
		g_game.changeSpeed(player, it.abilities->speed);
	}

	if (it.abilities->conditionSuppressions != 0) {
		player->addConditionSuppressions(it.abilities->conditionSuppressions);
		player->sendIcons();
	}

	if (it.abilities->regeneration) {
		Condition* condition = Condition::createCondition(static_cast<ConditionId_t>(slot), CONDITION_REGENERATION, -1, 0);

		if (it.abilities->healthGain != 0) {
			condition->setParam(CONDITION_PARAM_HEALTHGAIN, it.abilities->healthGain);
		}

		if (it.abilities->healthTicks != 0) {
			condition->setParam(CONDITION_PARAM_HEALTHTICKS, it.abilities->healthTicks);
		}

		if (it.abilities->manaGain != 0) {
			condition->setParam(CONDITION_PARAM_MANAGAIN, it.abilities->manaGain);
		}

		if (it.abilities->manaTicks != 0) {
			condition->setParam(CONDITION_PARAM_MANATICKS, it.abilities->manaTicks);
		}

		player->addCondition(condition);
	}

	//skill modifiers
	bool needUpdateSkills = false;

	for (int32_t i = SKILL_FIRST; i <= SKILL_LAST; ++i) {
		if (it.abilities->skills[i]) {
			needUpdateSkills = true;
			player->setVarSkill(static_cast<skills_t>(i), it.abilities->skills[i]);
		}
	}

	for (int32_t i = SPECIALSKILL_FIRST; i <= SPECIALSKILL_LAST; ++i) {
		if (it.abilities->specialSkills[i]) {
			needUpdateSkills = true;
			player->setVarSpecialSkill(static_cast<SpecialSkills_t>(i), it.abilities->specialSkills[i]);
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
			player->setVarStats(static_cast<stats_t>(s), it.abilities->stats[s]);
		}

		if (it.abilities->statsPercent[s]) {
			needUpdateStats = true;
			player->setVarStats(static_cast<stats_t>(s), static_cast<int32_t>(player->getDefaultStats(static_cast<stats_t>(s)) * ((it.abilities->statsPercent[s] - 100) / 100.f)));
		}
	}

	if (needUpdateStats) {
		player->sendStats();
	}

	return RETURNVALUE_NOERROR;
}

ReturnValue MoveEvent::DeEquipItem(MoveEvent*, Player* player, Item* item, slots_t slot, bool)
{
	if (!player->isItemAbilityEnabled(slot)) {
		return RETURNVALUE_NOERROR;
	}

	player->setItemAbility(slot, false);

	const ItemType& it = Item::items[item->getID()];
	if (it.transformDeEquipTo != 0) {
		g_game.transformItem(item, it.transformDeEquipTo);
		g_game.startDecay(item);
	}

	if (!it.abilities) {
		return RETURNVALUE_NOERROR;
	}

	if (it.abilities->invisible) {
		player->removeCondition(CONDITION_INVISIBLE, static_cast<ConditionId_t>(slot));
	}

	if (it.abilities->manaShield) {
		player->removeCondition(CONDITION_MANASHIELD, static_cast<ConditionId_t>(slot));
	}

	if (it.abilities->speed != 0) {
		g_game.changeSpeed(player, -it.abilities->speed);
	}

	if (it.abilities->conditionSuppressions != 0) {
		player->removeConditionSuppressions(it.abilities->conditionSuppressions);
		player->sendIcons();
	}

	if (it.abilities->regeneration) {
		player->removeCondition(CONDITION_REGENERATION, static_cast<ConditionId_t>(slot));
	}

	//skill modifiers
	bool needUpdateSkills = false;

	for (int32_t i = SKILL_FIRST; i <= SKILL_LAST; ++i) {
		if (it.abilities->skills[i] != 0) {
			needUpdateSkills = true;
			player->setVarSkill(static_cast<skills_t>(i), -it.abilities->skills[i]);
		}
	}

	for (int32_t i = SPECIALSKILL_FIRST; i <= SPECIALSKILL_LAST; ++i) {
		if (it.abilities->specialSkills[i] != 0) {
			needUpdateSkills = true;
			player->setVarSpecialSkill(static_cast<SpecialSkills_t>(i), -it.abilities->specialSkills[i]);
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
			player->setVarStats(static_cast<stats_t>(s), -it.abilities->stats[s]);
		}

		if (it.abilities->statsPercent[s]) {
			needUpdateStats = true;
			player->setVarStats(static_cast<stats_t>(s), -static_cast<int32_t>(player->getDefaultStats(static_cast<stats_t>(s)) * ((it.abilities->statsPercent[s] - 100) / 100.f)));
		}
	}

	if (needUpdateStats) {
		player->sendStats();
	}

	return RETURNVALUE_NOERROR;
}

bool MoveEvent::loadFunction(const pugi::xml_attribute& attr, bool isScripted)
{
	const char* functionName = attr.as_string();
	if (strcasecmp(functionName, "onstepinfield") == 0) {
		stepFunction = StepInField;
	} else if (strcasecmp(functionName, "onstepoutfield") == 0) {
		stepFunction = StepOutField;
	} else if (strcasecmp(functionName, "onaddfield") == 0) {
		moveFunction = AddItemField;
	} else if (strcasecmp(functionName, "onremovefield") == 0) {
		moveFunction = RemoveItemField;
	} else if (strcasecmp(functionName, "onequipitem") == 0) {
		equipFunction = EquipItem;
	} else if (strcasecmp(functionName, "ondeequipitem") == 0) {
		equipFunction = DeEquipItem;
	} else {
		if (!isScripted) {
			std::cout << "[Warning - MoveEvent::loadFunction] Function \"" << functionName << "\" does not exist." << std::endl;
			return false;
		}
	}

	if (!isScripted) {
		scripted = false;
	}
	return true;
}

MoveEvent_t MoveEvent::getEventType() const
{
	return eventType;
}

void MoveEvent::setEventType(MoveEvent_t type)
{
	eventType = type;
}

uint32_t MoveEvent::fireStepEvent(Creature* creature, Item* item, const Position& pos)
{
	if (scripted) {
		return executeStep(creature, item, pos);
	} else {
		return stepFunction(creature, item, pos);
	}
}

bool MoveEvent::executeStep(Creature* creature, Item* item, const Position& pos)
{
	//onStepIn(creature, item, pos, fromPosition)
	//onStepOut(creature, item, pos, fromPosition)
	if (!scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - MoveEvent::executeStep] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = scriptInterface->getScriptEnv();
	env->setScriptId(scriptId, scriptInterface);

	lua_State* L = scriptInterface->getLuaState();

	scriptInterface->pushFunction(scriptId);
	LuaScriptInterface::pushUserdata<Creature>(L, creature);
	LuaScriptInterface::setCreatureMetatable(L, -1, creature);
	LuaScriptInterface::pushThing(L, item);
	LuaScriptInterface::pushPosition(L, pos);
	LuaScriptInterface::pushPosition(L, creature->getLastPosition());

	return scriptInterface->callFunction(4);
}

ReturnValue MoveEvent::fireEquip(Player* player, Item* item, slots_t slot, bool isCheck)
{
	if (scripted) {
		if (!equipFunction || equipFunction(this, player, item, slot, isCheck) == RETURNVALUE_NOERROR) {
			if (executeEquip(player, item, slot, isCheck)) {
				return RETURNVALUE_NOERROR;
			}
			return RETURNVALUE_CANNOTBEDRESSED;
		}
		return equipFunction(this, player, item, slot, isCheck);
	} else {
		return equipFunction(this, player, item, slot, isCheck);
	}
}

bool MoveEvent::executeEquip(Player* player, Item* item, slots_t slot, bool isCheck)
{
	//onEquip(player, item, slot, isCheck)
	//onDeEquip(player, item, slot, isCheck)
	if (!scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - MoveEvent::executeEquip] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = scriptInterface->getScriptEnv();
	env->setScriptId(scriptId, scriptInterface);

	lua_State* L = scriptInterface->getLuaState();

	scriptInterface->pushFunction(scriptId);
	LuaScriptInterface::pushUserdata<Player>(L, player);
	LuaScriptInterface::setMetatable(L, -1, "Player");
	LuaScriptInterface::pushThing(L, item);
	lua_pushnumber(L, slot);
	LuaScriptInterface::pushBoolean(L, isCheck);

	return scriptInterface->callFunction(4);
}

uint32_t MoveEvent::fireAddRemItem(Item* item, Item* tileItem, const Position& pos)
{
	if (scripted) {
		return executeAddRemItem(item, tileItem, pos);
	} else {
		return moveFunction(item, tileItem, pos);
	}
}

bool MoveEvent::executeAddRemItem(Item* item, Item* tileItem, const Position& pos)
{
	//onaddItem(moveitem, tileitem, pos)
	//onRemoveItem(moveitem, tileitem, pos)
	if (!scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - MoveEvent::executeAddRemItem] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = scriptInterface->getScriptEnv();
	env->setScriptId(scriptId, scriptInterface);

	lua_State* L = scriptInterface->getLuaState();

	scriptInterface->pushFunction(scriptId);
	LuaScriptInterface::pushThing(L, item);
	LuaScriptInterface::pushThing(L, tileItem);
	LuaScriptInterface::pushPosition(L, pos);

	return scriptInterface->callFunction(3);
}
