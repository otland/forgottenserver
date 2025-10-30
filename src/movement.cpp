// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "movement.h"

#include "combat.h"
#include "game.h"
#include "pugicast.h"

extern Game g_game;
extern Vocations g_vocations;

MoveEvents::MoveEvents() : scriptInterface("MoveEvents Interface") { scriptInterface.initState(); }

MoveEvents::~MoveEvents() { clear(false); }

void MoveEvents::clearMap(MoveListMap& map, bool fromLua)
{
	for (auto it = map.begin(); it != map.end(); ++it) {
		for (int eventType = MOVE_EVENT_STEP_IN; eventType < MOVE_EVENT_LAST; ++eventType) {
			auto& moveEvents = it->second.moveEvent[eventType];
			for (auto find = moveEvents.begin(); find != moveEvents.end();) {
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
			for (auto find = moveEvents.begin(); find != moveEvents.end();) {
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

LuaScriptInterface& MoveEvents::getScriptInterface() { return scriptInterface; }

Event_ptr MoveEvents::getEvent(const std::string& nodeName)
{
	if (!caseInsensitiveEqual(nodeName, "movevent")) {
		return nullptr;
	}
	return Event_ptr(new MoveEvent(&scriptInterface));
}

bool MoveEvents::registerEvent(Event_ptr event, const pugi::xml_node& node)
{
	MoveEvent_ptr moveEvent{static_cast<MoveEvent*>(event.release())}; // event is guaranteed to be a MoveEvent

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
	MoveEvent_ptr moveEvent{event};

	const MoveEvent_t eventType = moveEvent->getEventType();
	if (eventType == MOVE_EVENT_ADD_ITEM || eventType == MOVE_EVENT_REMOVE_ITEM) {
		if (moveEvent->getTileItem()) {
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

	if (isValid(itemIdRange, event)) {
		const auto& range = getItemIdRange(event);
		for (auto& id : range) {
			if (moveEvent->getEventType() == MOVE_EVENT_EQUIP) {
				ItemType& it = Item::items.getItemType(id);
				it.wieldInfo = moveEvent->getWieldInfo();
				it.minReqLevel = moveEvent->getReqLevel();
				it.minReqMagicLevel = moveEvent->getReqMagLv();
				it.vocationString = moveEvent->getVocationString();
			}
			addEvent(*moveEvent, id, itemIdMap);
		}
	} else {
		return false;
	}
	return true;
}

bool MoveEvents::registerLuaEvent(MoveEvent* event)
{
	MoveEvent_ptr moveEvent{event};

	const MoveEvent_t eventType = moveEvent->getEventType();
	if (eventType == MOVE_EVENT_ADD_ITEM || eventType == MOVE_EVENT_REMOVE_ITEM) {
		if (moveEvent->getTileItem()) {
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

	if (isValid(itemIdRange, event)) {
		const auto& range = getItemIdRange(event);
		for (auto& id : range) {
			if (moveEvent->getEventType() == MOVE_EVENT_EQUIP) {
				ItemType& it = Item::items.getItemType(id);
				it.wieldInfo = moveEvent->getWieldInfo();
				it.minReqLevel = moveEvent->getReqLevel();
				it.minReqMagicLevel = moveEvent->getReqMagLv();
				it.vocationString = moveEvent->getVocationString();
			}
			addEvent(*moveEvent, id, itemIdMap);
		}
	} else if (isValid(actionIdRange, event)) {
		const auto& range = getActionIdRange(event);
		for (auto& id : range) {
			addEvent(*moveEvent, id, actionIdMap);
		}
	} else if (isValid(uniqueIdRange, event)) {
		const auto& range = getUniqueIdRange(event);
		for (auto& id : range) {
			addEvent(*moveEvent, id, uniqueIdMap);
		}
	} else if (isValidPos(posList, event)) {
		const auto& range = getPosList(event);
		for (auto& pos : range) {
			addEvent(*moveEvent, pos, positionMap);
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

MoveEvent* MoveEvents::getEvent(const std::shared_ptr<Item>& item, MoveEvent_t eventType, slots_t slot)
{
	uint32_t slotp;
	switch (slot) {
		case CONST_SLOT_HEAD:
			slotp = SLOTP_HEAD;
			break;
		case CONST_SLOT_NECKLACE:
			slotp = SLOTP_NECKLACE;
			break;
		case CONST_SLOT_BACKPACK:
			slotp = SLOTP_BACKPACK;
			break;
		case CONST_SLOT_ARMOR:
			slotp = SLOTP_ARMOR;
			break;
		case CONST_SLOT_RIGHT:
			slotp = SLOTP_RIGHT;
			break;
		case CONST_SLOT_LEFT:
			slotp = SLOTP_LEFT;
			break;
		case CONST_SLOT_LEGS:
			slotp = SLOTP_LEGS;
			break;
		case CONST_SLOT_FEET:
			slotp = SLOTP_FEET;
			break;
		case CONST_SLOT_AMMO:
			slotp = SLOTP_AMMO;
			break;
		case CONST_SLOT_RING:
			slotp = SLOTP_RING;
			break;
		default:
			slotp = 0;
			break;
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

MoveEvent* MoveEvents::getEvent(const std::shared_ptr<Item>& item, MoveEvent_t eventType)
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

MoveEvent* MoveEvents::getEvent(const std::shared_ptr<const Tile>& tile, MoveEvent_t eventType)
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

uint32_t MoveEvents::onCreatureMove(const std::shared_ptr<Creature>& creature, const std::shared_ptr<const Tile>& tile,
                                    MoveEvent_t eventType)
{
	const Position& pos = tile->getPosition();

	uint32_t ret = 1;

	MoveEvent* moveEvent = getEvent(tile, eventType);
	if (moveEvent) {
		ret &= moveEvent->fireStepEvent(creature, nullptr, pos);
	}

	for (size_t i = tile->getFirstIndex(), j = tile->getLastIndex(); i < j; ++i) {
		const auto& thing = tile->getThing(i);
		if (!thing) {
			continue;
		}

		const auto& tileItem = thing->getItem();
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

ReturnValue MoveEvents::onPlayerEquip(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item,
                                      slots_t slot, bool isCheck)
{
	MoveEvent* moveEvent = getEvent(item, MOVE_EVENT_EQUIP, slot);
	if (!moveEvent) {
		return RETURNVALUE_NOERROR;
	}
	return moveEvent->fireEquip(player, item, slot, isCheck);
}

ReturnValue MoveEvents::onPlayerDeEquip(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item,
                                        slots_t slot)
{
	MoveEvent* moveEvent = getEvent(item, MOVE_EVENT_DEEQUIP, slot);
	if (!moveEvent) {
		// If the item does not have an event, we make sure to reset the slot, since some items transform into items
		// without events.
		player->setItemAbility(slot, false);
		return RETURNVALUE_NOERROR;
	}
	return moveEvent->fireEquip(player, item, slot, false);
}

uint32_t MoveEvents::onItemMove(const std::shared_ptr<Item>& item, const std::shared_ptr<Tile>& tile, bool isAdd)
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
		const auto& thing = tile->getThing(i);
		if (!thing) {
			continue;
		}

		const auto& tileItem = thing->getItem();
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

std::string_view MoveEvent::getScriptEventName() const
{
	switch (eventType) {
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
			std::cout << "[Error - MoveEvent::getScriptEventName] Invalid event type" << std::endl;
			return "";
	}
}

bool MoveEvent::configureEvent(const pugi::xml_node& node)
{
	pugi::xml_attribute eventAttr = node.attribute("event");
	if (!eventAttr) {
		std::cout << "[Error - MoveEvent::configureMoveEvent] Missing event" << std::endl;
		return false;
	}

	std::string tmpStr = boost::algorithm::to_lower_copy<std::string>(eventAttr.as_string());
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
		std::cout << "Error: [MoveEvent::configureMoveEvent] No valid event name " << eventAttr.as_string()
		          << std::endl;
		return false;
	}

	if (eventType == MOVE_EVENT_EQUIP || eventType == MOVE_EVENT_DEEQUIP) {
		pugi::xml_attribute slotAttribute = node.attribute("slot");
		if (slotAttribute) {
			tmpStr = boost::algorithm::to_lower_copy<std::string>(slotAttribute.as_string());
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
				std::cout << "[Warning - MoveEvent::configureMoveEvent] Unknown slot type: "
				          << slotAttribute.as_string() << std::endl;
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

		// Gather vocation information
		std::list<std::string> vocStringList;
		for (auto vocationNode : node.children()) {
			pugi::xml_attribute vocationNameAttribute = vocationNode.attribute("name");
			if (!vocationNameAttribute) {
				continue;
			}

			int32_t vocationId = g_vocations.getVocationId(vocationNameAttribute.as_string());
			if (vocationId != -1) {
				vocationEquipSet.insert(vocationId);
				if (vocationNode.attribute("showInDescription").as_bool(true)) {
					vocStringList.push_back(
					    boost::algorithm::to_lower_copy<std::string>(vocationNameAttribute.as_string()));
				}
			}
		}

		if (!vocationEquipSet.empty()) {
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

uint32_t MoveEvent::StepInField(const std::shared_ptr<Creature>& creature, const std::shared_ptr<Item>& item,
                                const Position&)
{
	const auto& field = item->getMagicField();
	if (field) {
		field->onStepInField(creature);
		return 1;
	}

	return LUA_ERROR_ITEM_NOT_FOUND;
}

uint32_t MoveEvent::StepOutField(const std::shared_ptr<Creature>&, const std::shared_ptr<Item>&, const Position&)
{
	return 1;
}

uint32_t MoveEvent::AddItemField(const std::shared_ptr<Item>& item, const std::shared_ptr<Item>&, const Position&)
{
	if (const auto& field = item->getMagicField()) {
		const auto& tile = item->getTile();
		if (CreatureVector* creatures = tile->getCreatures()) {
			for (const auto& creature : *creatures) {
				field->onStepInField(creature);
			}
		}
		return 1;
	}
	return LUA_ERROR_ITEM_NOT_FOUND;
}

uint32_t MoveEvent::RemoveItemField(const std::shared_ptr<Item>&, const std::shared_ptr<Item>&, const Position&)
{
	return 1;
}

ReturnValue MoveEvent::EquipItem(MoveEvent* moveEvent, const std::shared_ptr<Player>& player,
                                 const std::shared_ptr<Item>& item, slots_t slot, bool isCheck)
{
	if (!player->hasFlag(PlayerFlag_IgnoreWeaponCheck) && moveEvent->getWieldInfo() != 0) {
		if (!moveEvent->hasVocationEquipSet(player->getVocationId())) {
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
		auto newItem = g_game.transformItem(item, it.transformEquipTo);
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
		Condition* condition =
		    Condition::createCondition(static_cast<ConditionId_t>(slot), CONDITION_MANASHIELD, -1, 0);
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
		Condition* condition =
		    Condition::createCondition(static_cast<ConditionId_t>(slot), CONDITION_REGENERATION, -1, 0);

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

	// skill modifiers
	bool needUpdateSkills = false;

	for (int32_t i = SKILL_FIRST; i <= SKILL_LAST; ++i) {
		if (it.abilities->skills[i]) {
			needUpdateSkills = true;
			player->setVarSkill(static_cast<skills_t>(i), it.abilities->skills[i]);
		}
	}

	for (int32_t i = 0; i < COMBAT_COUNT; ++i) {
		if (it.abilities->specialMagicLevelSkill[i]) {
			player->setSpecialMagicLevelSkill(indexToCombatType(i), it.abilities->specialMagicLevelSkill[i]);
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

	// stat modifiers
	bool needUpdateStats = false;

	for (int32_t s = STAT_FIRST; s <= STAT_LAST; ++s) {
		if (it.abilities->stats[s]) {
			needUpdateStats = true;
			player->setVarStats(static_cast<stats_t>(s), it.abilities->stats[s]);
		}

		if (it.abilities->statsPercent[s]) {
			needUpdateStats = true;
			player->setVarStats(static_cast<stats_t>(s),
			                    static_cast<int32_t>(player->getDefaultStats(static_cast<stats_t>(s)) *
			                                         ((it.abilities->statsPercent[s] - 100) / 100.f)));
		}
	}

	if (needUpdateStats) {
		player->sendStats();
		player->sendSkills();
	}

	return RETURNVALUE_NOERROR;
}

ReturnValue MoveEvent::DeEquipItem(MoveEvent*, const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item,
                                   slots_t slot, bool)
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

	// skill modifiers
	bool needUpdateSkills = false;

	for (int32_t i = SKILL_FIRST; i <= SKILL_LAST; ++i) {
		if (it.abilities->skills[i] != 0) {
			needUpdateSkills = true;
			player->setVarSkill(static_cast<skills_t>(i), -it.abilities->skills[i]);
		}
	}

	for (int32_t i = 0; i < COMBAT_COUNT; ++i) {
		if (it.abilities->specialMagicLevelSkill[i] != 0) {
			player->setSpecialMagicLevelSkill(indexToCombatType(i), -it.abilities->specialMagicLevelSkill[i]);
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

	// stat modifiers
	bool needUpdateStats = false;

	for (int32_t s = STAT_FIRST; s <= STAT_LAST; ++s) {
		if (it.abilities->stats[s]) {
			needUpdateStats = true;
			player->setVarStats(static_cast<stats_t>(s), -it.abilities->stats[s]);
		}

		if (it.abilities->statsPercent[s]) {
			needUpdateStats = true;
			player->setVarStats(static_cast<stats_t>(s),
			                    -static_cast<int32_t>(player->getDefaultStats(static_cast<stats_t>(s)) *
			                                          ((it.abilities->statsPercent[s] - 100) / 100.f)));
		}
	}

	if (needUpdateStats) {
		player->sendStats();
		player->sendSkills();
	}

	return RETURNVALUE_NOERROR;
}

bool MoveEvent::loadFunction(const pugi::xml_attribute& attr, bool isScripted)
{
	const char* functionName = attr.as_string();
	if (caseInsensitiveEqual(functionName, "onstepinfield")) {
		stepFunction = StepInField;
	} else if (caseInsensitiveEqual(functionName, "onstepoutfield")) {
		stepFunction = StepOutField;
	} else if (caseInsensitiveEqual(functionName, "onaddfield")) {
		moveFunction = AddItemField;
	} else if (caseInsensitiveEqual(functionName, "onremovefield")) {
		moveFunction = RemoveItemField;
	} else if (caseInsensitiveEqual(functionName, "onequipitem")) {
		equipFunction = EquipItem;
	} else if (caseInsensitiveEqual(functionName, "ondeequipitem")) {
		equipFunction = DeEquipItem;
	} else {
		if (!isScripted) {
			std::cout << "[Warning - MoveEvent::loadFunction] Function \"" << functionName << "\" does not exist."
			          << std::endl;
			return false;
		}
	}

	if (!isScripted) {
		scripted = false;
	}
	return true;
}

MoveEvent_t MoveEvent::getEventType() const { return eventType; }

void MoveEvent::setEventType(MoveEvent_t type) { eventType = type; }

uint32_t MoveEvent::fireStepEvent(const std::shared_ptr<Creature>& creature, const std::shared_ptr<Item>& item,
                                  const Position& pos)
{
	if (scripted) {
		return executeStep(creature, item, pos);
	}
	return stepFunction(creature, item, pos);
}

bool MoveEvent::executeStep(const std::shared_ptr<Creature>& creature, const std::shared_ptr<Item>& item,
                            const Position& pos)
{
	// onStepIn(creature, item, pos, fromPosition)
	// onStepOut(creature, item, pos, fromPosition)
	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - MoveEvent::executeStep] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(scriptId, scriptInterface);

	lua_State* L = scriptInterface->getLuaState();

	scriptInterface->pushFunction(scriptId);
	tfs::lua::pushSharedPtr(L, creature);
	tfs::lua::setCreatureMetatable(L, -1, creature);
	tfs::lua::pushThing(L, item);
	tfs::lua::pushPosition(L, pos);
	tfs::lua::pushPosition(L, creature->getLastPosition());

	return scriptInterface->callFunction(4);
}

ReturnValue MoveEvent::fireEquip(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item, slots_t slot,
                                 bool isCheck)
{
	ReturnValue ret = RETURNVALUE_NOERROR;
	if (equipFunction) {
		ret = equipFunction(this, player, item, slot, isCheck);
	}
	if (scripted && (ret == RETURNVALUE_NOERROR) && !executeEquip(player, item, slot, isCheck)) {
		ret = RETURNVALUE_CANNOTBEDRESSED;
	}
	return ret;
}

bool MoveEvent::executeEquip(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item, slots_t slot,
                             bool isCheck)
{
	// onEquip(player, item, slot, isCheck)
	// onDeEquip(player, item, slot, isCheck)
	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - MoveEvent::executeEquip] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(scriptId, scriptInterface);

	lua_State* L = scriptInterface->getLuaState();

	scriptInterface->pushFunction(scriptId);
	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");
	tfs::lua::pushThing(L, item);
	lua_pushnumber(L, slot);
	tfs::lua::pushBoolean(L, isCheck);

	return scriptInterface->callFunction(4);
}

uint32_t MoveEvent::fireAddRemItem(const std::shared_ptr<Item>& item, const std::shared_ptr<Item>& tileItem,
                                   const Position& pos)
{
	if (scripted) {
		return executeAddRemItem(item, tileItem, pos);
	}
	return moveFunction(item, tileItem, pos);
}

bool MoveEvent::executeAddRemItem(const std::shared_ptr<Item>& item, const std::shared_ptr<Item>& tileItem,
                                  const Position& pos)
{
	// onaddItem(moveitem, tileitem, pos)
	// onRemoveItem(moveitem, tileitem, pos)
	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - MoveEvent::executeAddRemItem] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(scriptId, scriptInterface);

	lua_State* L = scriptInterface->getLuaState();

	scriptInterface->pushFunction(scriptId);
	tfs::lua::pushThing(L, item);
	tfs::lua::pushThing(L, tileItem);
	tfs::lua::pushPosition(L, pos);

	return scriptInterface->callFunction(3);
}
