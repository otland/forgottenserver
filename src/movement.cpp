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

MoveEvents::~MoveEvents() { clear(); }

void MoveEvents::clear()
{
	itemIdMap.clear();
	actionIdMap.clear();
	uniqueIdMap.clear();
	positionMap.clear();

	scriptInterface.reInitState();
}

bool MoveEvents::registerLuaFunction(MoveEvent_shared_ptr moveEvent)
{
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

	if (!moveEvent->getItemIdRange().empty()) {
		for (auto& id : moveEvent->getItemIdRange()) {
			if (moveEvent->getEventType() == MOVE_EVENT_EQUIP) {
				ItemType& it = Item::items.getItemType(id);
				it.wieldInfo = moveEvent->getWieldInfo();
				it.minReqLevel = moveEvent->getReqLevel();
				it.minReqMagicLevel = moveEvent->getReqMagLv();
				it.vocationString = moveEvent->getVocationString();
			}
			addEvent(moveEvent, id, itemIdMap);
			moveEvent->clearItemIdRange();
		}
	} else {
		return false;
	}
	return true;
}

bool MoveEvents::registerLuaEvent(MoveEvent_shared_ptr moveEvent)
{
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

	if (!moveEvent->getItemIdRange().empty()) {
		for (auto& id : moveEvent->getItemIdRange()) {
			if (moveEvent->getEventType() == MOVE_EVENT_EQUIP) {
				ItemType& it = Item::items.getItemType(id);
				it.wieldInfo = moveEvent->getWieldInfo();
				it.minReqLevel = moveEvent->getReqLevel();
				it.minReqMagicLevel = moveEvent->getReqMagLv();
				it.vocationString = moveEvent->getVocationString();
			}
			addEvent(moveEvent, id, itemIdMap);
			moveEvent->clearItemIdRange();
		}
	} else if (!moveEvent->getActionIdRange().empty()) {
		for (auto& id : moveEvent->getActionIdRange()) {
			addEvent(moveEvent, id, actionIdMap);
		}
		moveEvent->clearActionIdRange();
	} else if (!moveEvent->getUniqueIdRange().empty()) {
		for (auto& id : moveEvent->getUniqueIdRange()) {
			addEvent(moveEvent, id, uniqueIdMap);
		}
		moveEvent->clearUniqueIdRange();
	} else if (!moveEvent->getPosList().empty()) {
		for (auto& pos : moveEvent->getPosList()) {
			addEvent(moveEvent, pos, positionMap);
		}
		moveEvent->clearPosList();
	} else {
		return false;
	}

	return true;
}

MoveEvent_shared_ptr MoveEvents::getMoveEvent(uint16_t id, MoveEvent_t eventType, const std::string& stringType)
{
	if (stringType == "id") {
		auto it = itemIdMap.find(id);
		if (it != itemIdMap.end()) {
			std::list<MoveEvent_shared_ptr>& moveEventList = it->second.moveEvent[eventType];
			for (auto& event : moveEventList) {
				if (event->eventType != eventType) {
					return event;
				}
			}
		}
	} else if (stringType == "uid") {
		auto it = uniqueIdMap.find(id);
		if (it != uniqueIdMap.end()) {
			std::list<MoveEvent_shared_ptr>& moveEventList = it->second.moveEvent[eventType];
			for (auto& event : moveEventList) {
				if (event->eventType != eventType) {
					return event;
				}
			}
		}
	} else if (stringType == "aid") {
		auto it = actionIdMap.find(id);
		if (it != actionIdMap.end()) {
			std::list<MoveEvent_shared_ptr>& moveEventList = it->second.moveEvent[eventType];
			for (auto& event : moveEventList) {
				if (event->eventType != eventType) {
					return event;
				}
			}
		}
	}
	return nullptr;
}

MoveEvent_shared_ptr MoveEvents::getMoveEvent(const Position& pos, MoveEvent_t eventType)
{
	auto& it = positionMap.at(pos);
	std::list<MoveEvent_shared_ptr>& moveEventList = it.moveEvent[eventType];
	for (auto& event : moveEventList) {
		if (event->eventType != eventType) {
			return event;
		}
	}
	return nullptr;
}

void MoveEvents::addEvent(MoveEvent_shared_ptr moveEvent, int32_t id, MoveListMap& map)
{
	auto it = map.find(id);
	if (it == map.end()) {
		MoveEventList moveEventList;
		moveEventList.moveEvent[moveEvent->getEventType()].push_back(moveEvent);
		map[id] = moveEventList;
	} else {
		std::list<MoveEvent_shared_ptr>& moveEventList = it->second.moveEvent[moveEvent->getEventType()];
		for (auto& existingMoveEvent : moveEventList) {
			if (existingMoveEvent->getSlot() == moveEvent->getSlot()) {
				std::cout << "[Warning - MoveEvents::addEvent] Duplicate move event found: " << id << std::endl;
			}
		}
		moveEventList.push_back(moveEvent);
	}
}

MoveEvent_shared_ptr MoveEvents::getEvent(Item* item, MoveEvent_t eventType, slots_t slot)
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
		std::list<MoveEvent_shared_ptr>& moveEventList = it->second.moveEvent[eventType];
		for (auto& moveEvent : moveEventList) {
			if ((moveEvent->getSlot() & slotp) != 0) {
				return moveEvent;
			}
		}
	}
	return nullptr;
}

MoveEvent_shared_ptr MoveEvents::getEvent(Item* item, MoveEvent_t eventType)
{
	MoveListMap::iterator it;

	if (item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
		it = uniqueIdMap.find(item->getUniqueId());
		if (it != uniqueIdMap.end()) {
			std::list<MoveEvent_shared_ptr>& moveEventList = it->second.moveEvent[eventType];
			if (!moveEventList.empty()) {
				return *moveEventList.begin();
			}
		}
	}

	if (item->hasAttribute(ITEM_ATTRIBUTE_ACTIONID)) {
		it = actionIdMap.find(item->getActionId());
		if (it != actionIdMap.end()) {
			std::list<MoveEvent_shared_ptr>& moveEventList = it->second.moveEvent[eventType];
			if (!moveEventList.empty()) {
				return *moveEventList.begin();
			}
		}
	}

	it = itemIdMap.find(item->getID());
	if (it != itemIdMap.end()) {
		std::list<MoveEvent_shared_ptr>& moveEventList = it->second.moveEvent[eventType];
		if (!moveEventList.empty()) {
			return *moveEventList.begin();
		}
	}
	return nullptr;
}

void MoveEvents::addEvent(MoveEvent_shared_ptr moveEvent, const Position& pos, MovePosListMap& map)
{
	auto it = map.find(pos);
	if (it == map.end()) {
		MoveEventList moveEventList;
		moveEventList.moveEvent[moveEvent->getEventType()].push_back(moveEvent);
		map[pos] = moveEventList;
	} else {
		std::list<MoveEvent_shared_ptr>& moveEventList = it->second.moveEvent[moveEvent->getEventType()];
		if (!moveEventList.empty()) {
			std::cout << "[Warning - MoveEvents::addEvent] Duplicate move event found: " << pos << std::endl;
		}

		moveEventList.push_back(moveEvent);
	}
}

MoveEvent_shared_ptr MoveEvents::getEvent(const Tile* tile, MoveEvent_t eventType)
{
	auto it = positionMap.find(tile->getPosition());
	if (it != positionMap.end()) {
		std::list<MoveEvent_shared_ptr>& moveEventList = it->second.moveEvent[eventType];
		if (!moveEventList.empty()) {
			return *moveEventList.begin();
		}
	}
	return nullptr;
}

uint32_t MoveEvents::onCreatureMove(Creature* creature, const Tile* tile, MoveEvent_t eventType)
{
	const Position& pos = tile->getPosition();

	uint32_t ret = 1;

	MoveEvent_shared_ptr moveEvent = getEvent(tile, eventType);
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
	MoveEvent_shared_ptr moveEvent = getEvent(item, MOVE_EVENT_EQUIP, slot);
	if (!moveEvent) {
		return RETURNVALUE_NOERROR;
	}
	return moveEvent->fireEquip(player, item, slot, isCheck);
}

ReturnValue MoveEvents::onPlayerDeEquip(Player* player, Item* item, slots_t slot)
{
	MoveEvent_shared_ptr moveEvent = getEvent(item, MOVE_EVENT_DEEQUIP, slot);
	if (!moveEvent) {
		// If the item does not have an event, we make sure to reset the slot, since some items transform into items
		// without events.
		player->setItemAbility(slot, false);
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
	MoveEvent_shared_ptr moveEvent = getEvent(tile, eventType1);
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

uint32_t MoveEvent::StepInField(Creature* creature, Item* item, const Position&)
{
	MagicField* field = item->getMagicField();
	if (field) {
		field->onStepInField(creature);
		return 1;
	}

	return LUA_ERROR_ITEM_NOT_FOUND;
}

uint32_t MoveEvent::StepOutField(Creature*, Item*, const Position&) { return 1; }

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

uint32_t MoveEvent::RemoveItemField(Item*, Item*, const Position&) { return 1; }

ReturnValue MoveEvent::EquipItem(MoveEvent* moveEvent, Player* player, Item* item, slots_t slot, bool isCheck)
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

uint32_t MoveEvent::fireStepEvent(Creature* creature, Item* item, const Position& pos)
{
	if (scripted) {
		return executeStep(creature, item, pos);
	}
	return stepFunction(creature, item, pos);
}

bool MoveEvent::executeStep(Creature* creature, Item* item, const Position& pos)
{
	// onStepIn(creature, item, pos, fromPosition)
	// onStepOut(creature, item, pos, fromPosition)
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
	ReturnValue ret = RETURNVALUE_NOERROR;
	if (equipFunction) {
		ret = equipFunction(this, player, item, slot, isCheck);
	}
	if (scripted && (ret == RETURNVALUE_NOERROR) && !executeEquip(player, item, slot, isCheck)) {
		ret = RETURNVALUE_CANNOTBEDRESSED;
	}
	return ret;
}

bool MoveEvent::executeEquip(Player* player, Item* item, slots_t slot, bool isCheck)
{
	// onEquip(player, item, slot, isCheck)
	// onDeEquip(player, item, slot, isCheck)
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
	}
	return moveFunction(item, tileItem, pos);
}

bool MoveEvent::executeAddRemItem(Item* item, Item* tileItem, const Position& pos)
{
	// onaddItem(moveitem, tileitem, pos)
	// onRemoveItem(moveitem, tileitem, pos)
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
