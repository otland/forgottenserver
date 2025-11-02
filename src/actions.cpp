// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "actions.h"

#include "bed.h"
#include "configmanager.h"
#include "container.h"
#include "game.h"
#include "housetile.h"
#include "spells.h"

extern Game g_game;
extern Spells* g_spells;
extern Actions* g_actions;

Actions::Actions() : scriptInterface("Action Interface") { scriptInterface.initState(); }

Actions::~Actions() { clear(false); }

void Actions::clearMap(ActionUseMap& map, bool fromLua)
{
	for (auto it = map.begin(); it != map.end();) {
		if (fromLua == it->second.fromLua) {
			it = map.erase(it);
		} else {
			++it;
		}
	}
}

void Actions::clear(bool fromLua)
{
	clearMap(useItemMap, fromLua);
	clearMap(uniqueItemMap, fromLua);
	clearMap(actionItemMap, fromLua);

	reInitState(fromLua);
}

LuaScriptInterface& Actions::getScriptInterface() { return scriptInterface; }

Event_ptr Actions::getEvent(const std::string& nodeName)
{
	if (!caseInsensitiveEqual(nodeName, "action")) {
		return nullptr;
	}
	return Event_ptr(new Action(&scriptInterface));
}

bool Actions::registerLuaEvent(Action* event)
{
	Action_ptr action{event};
	if (isValid(ids, event)) {
		const auto& range = getItemIdRange(event);
		for (auto& id : range) {
			auto result = useItemMap.emplace(id, *action);
			if (!result.second) {
				std::cout << "[Warning - Actions::registerLuaEvent] Duplicate registered item with id: " << id
				          << " in range from id: " << range.front() << ", to id: " << range.back() << std::endl;
			}
		}
		return true;
	} else if (isValid(uids, event)) {
		const auto& range = getUniqueIdRange(event);
		for (auto& id : range) {
			auto result = uniqueItemMap.emplace(id, *action);
			if (!result.second) {
				std::cout << "[Warning - Actions::registerLuaEvent] Duplicate registered item with uid: " << id
				          << " in range from uid: " << range.front() << ", to uid: " << range.back() << std::endl;
			}
		}
		return true;
	} else if (isValid(aids, event)) {
		const auto& range = getActionIdRange(event);
		for (auto& id : range) {
			auto result = actionItemMap.emplace(id, *action);
			if (!result.second) {
				std::cout << "[Warning - Actions::registerLuaEvent] Duplicate registered item with aid: " << id
				          << " in range from aid: " << range.front() << ", to aid: " << range.back() << std::endl;
			}
		}
		return true;
	}

	std::cout << "[Warning - Actions::registerLuaEvent] There is no id / aid / uid set for this event" << std::endl;
	return false;
}

ReturnValue Actions::canUse(const std::shared_ptr<const Player>& player, const Position& pos)
{
	if (pos.x != 0xFFFF) {
		const Position& playerPos = player->getPosition();
		if (playerPos.z != pos.z) {
			return playerPos.z > pos.z ? RETURNVALUE_FIRSTGOUPSTAIRS : RETURNVALUE_FIRSTGODOWNSTAIRS;
		}

		if (!playerPos.isInRange(pos, 1, 1)) {
			return RETURNVALUE_TOOFARAWAY;
		}
	}
	return RETURNVALUE_NOERROR;
}

ReturnValue Actions::canUse(const std::shared_ptr<const Player>& player, const Position& pos,
                            const std::shared_ptr<const Item>& item)
{
	Action* action = getAction(item);
	if (action) {
		return action->canExecuteAction(player, pos);
	}
	return RETURNVALUE_NOERROR;
}

ReturnValue Actions::canUseFar(const std::shared_ptr<const Creature>& creature, const Position& toPos,
                               bool checkLineOfSight, bool checkFloor)
{
	if (toPos.x == 0xFFFF) {
		return RETURNVALUE_NOERROR;
	}

	const Position& creaturePos = creature->getPosition();
	if (checkFloor && creaturePos.z != toPos.z) {
		return creaturePos.z > toPos.z ? RETURNVALUE_FIRSTGOUPSTAIRS : RETURNVALUE_FIRSTGODOWNSTAIRS;
	}

	if (!toPos.isInRange(creaturePos, Map::maxClientViewportX - 1, Map::maxClientViewportY - 1)) {
		return RETURNVALUE_TOOFARAWAY;
	}

	if (checkLineOfSight && !g_game.canThrowObjectTo(creaturePos, toPos, checkLineOfSight, checkFloor)) {
		return RETURNVALUE_CANNOTTHROW;
	}

	return RETURNVALUE_NOERROR;
}

Action* Actions::getAction(const std::shared_ptr<const Item>& item)
{
	if (item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
		auto it = uniqueItemMap.find(item->getUniqueId());
		if (it != uniqueItemMap.end()) {
			return &it->second;
		}
	}

	if (item->hasAttribute(ITEM_ATTRIBUTE_ACTIONID)) {
		auto it = actionItemMap.find(item->getActionId());
		if (it != actionItemMap.end()) {
			return &it->second;
		}
	}

	auto it = useItemMap.find(item->getID());
	if (it != useItemMap.end()) {
		return &it->second;
	}

	// rune items
	return g_spells->getRuneSpell(item->getID());
}

ReturnValue Actions::internalUseItem(const std::shared_ptr<Player>& player, const Position& pos, uint8_t index,
                                     const std::shared_ptr<Item>& item, bool isHotkey)
{
	if (const auto& door = item->getDoor()) {
		if (!door->canUse(player)) {
			return RETURNVALUE_NOTPOSSIBLE;
		}
	}

	Action* action = getAction(item);
	if (action) {
		if (action->isScripted()) {
			if (action->executeUse(player, item, pos, nullptr, pos, isHotkey)) {
				return RETURNVALUE_NOERROR;
			}

			if (item->isRemoved()) {
				return RETURNVALUE_CANNOTUSETHISOBJECT;
			}
		} else if (action->function && action->function(player, item, pos, nullptr, pos, isHotkey)) {
			return RETURNVALUE_NOERROR;
		}
	}

	if (const auto& bed = item->getBed()) {
		if (!bed->canUse(player)) {
			if (!bed->getHouse()) {
				return RETURNVALUE_YOUCANNOTUSETHISBED;
			}

			if (!player->isPremium()) {
				return RETURNVALUE_YOUNEEDPREMIUMACCOUNT;
			}
			return RETURNVALUE_CANNOTUSETHISOBJECT;
		}

		if (bed->trySleep(player)) {
			player->setBedItem(bed);
			g_game.sendOfflineTrainingDialog(player);
		}

		return RETURNVALUE_NOERROR;
	}

	if (const auto& container = item->getContainer()) {
		auto openContainer = container;

		// Handle depot containers
		if (const auto& depot = container->getDepotLocker()) {
			const auto& myDepotLocker = player->getDepotLocker();
			myDepotLocker->setParent(depot->getParent()->getTile());
			openContainer = myDepotLocker;
		}

		// Handle corpse ownership restrictions
		if (uint32_t corpseOwner = container->getCorpseOwner()) {
			if (!player->canOpenCorpse(corpseOwner)) {
				return RETURNVALUE_YOUARENOTTHEOWNER;
			}
		}

		// Toggle container: open if closed, close if already open
		int32_t oldContainerId = player->getContainerID(openContainer);
		if (oldContainerId == -1) {
			player->addContainer(index, openContainer);
			player->onSendContainer(openContainer);
		} else {
			player->onCloseContainer(openContainer);
			player->closeContainer(oldContainerId);
		}

		return RETURNVALUE_NOERROR;
	}

	const ItemType& it = Item::items[item->getID()];
	if (it.canReadText) {
		if (it.canWriteText) {
			player->setWriteItem(item, it.maxTextLen);
			player->sendTextWindow(item, it.maxTextLen, true);
		} else {
			player->setWriteItem(nullptr);
			player->sendTextWindow(item, 0, false);
		}

		return RETURNVALUE_NOERROR;
	}

	return RETURNVALUE_CANNOTUSETHISOBJECT;
}

static void showUseHotkeyMessage(const std::shared_ptr<Player>& player, const std::shared_ptr<const Item>& item,
                                 uint32_t count)
{
	const ItemType& it = Item::items[item->getID()];
	if (!it.showCount) {
		player->sendTextMessage(MESSAGE_HOTKEY_PRESSED, fmt::format("Using one of {:s}...", item->getName()));
	} else if (count == 1) {
		player->sendTextMessage(MESSAGE_HOTKEY_PRESSED, fmt::format("Using the last {:s}...", item->getName()));
	} else {
		player->sendTextMessage(MESSAGE_HOTKEY_PRESSED,
		                        fmt::format("Using one of {:d} {:s}...", count, item->getPluralName()));
	}
}

bool Actions::useItem(const std::shared_ptr<Player>& player, const Position& pos, uint8_t index,
                      const std::shared_ptr<Item>& item, bool isHotkey)
{
	int32_t cooldown = getNumber(ConfigManager::ACTIONS_DELAY_INTERVAL);
	player->setNextAction(OTSYS_TIME() + cooldown);
	player->sendUseItemCooldown(cooldown);
	if (item->isSupply()) {
		player->sendSupplyUsed(item->getClientID());
	}

	if (isHotkey) {
		uint16_t subType = item->getSubType();
		showUseHotkeyMessage(player, item,
		                     player->getItemTypeCount(item->getID(), subType != item->getItemCount() ? subType : -1));
	}

	if (getBoolean(ConfigManager::ONLY_INVITED_CAN_MOVE_HOUSE_ITEMS)) {
		if (const auto& tile = item->getTile()) {
			if (const auto& houseTile = tile->getHouseTile()) {
				if (!item->getTopParent()->getCreature() && !houseTile->getHouse()->isInvited(player)) {
					player->sendCancelMessage(RETURNVALUE_PLAYERISNOTINVITED);
					return false;
				}
			}
		}
	}

	ReturnValue ret = internalUseItem(player, pos, index, item, isHotkey);
	if (ret == RETURNVALUE_YOUCANNOTUSETHISBED) {
		g_game.internalCreatureSay(player, TALKTYPE_MONSTER_SAY, getReturnMessage(ret), false);
		return false;
	}

	if (ret != RETURNVALUE_NOERROR) {
		player->sendCancelMessage(ret);
		return false;
	}

	return true;
}

bool Actions::useItemEx(const std::shared_ptr<Player>& player, const Position& fromPos, const Position& toPos,
                        uint8_t toStackPos, const std::shared_ptr<Item>& item, bool isHotkey,
                        const std::shared_ptr<Creature>& creature /* = nullptr*/)
{
	int32_t cooldown = getNumber(ConfigManager::EX_ACTIONS_DELAY_INTERVAL);
	player->setNextAction(OTSYS_TIME() + cooldown);
	player->sendUseItemCooldown(cooldown);

	Action* action = getAction(item);
	if (!action) {
		player->sendCancelMessage(RETURNVALUE_CANNOTUSETHISOBJECT);
		return false;
	}

	ReturnValue ret = action->canExecuteAction(player, toPos);
	if (ret != RETURNVALUE_NOERROR) {
		player->sendCancelMessage(ret);
		return false;
	}

	if (isHotkey) {
		uint16_t subType = item->getSubType();
		showUseHotkeyMessage(player, item,
		                     player->getItemTypeCount(item->getID(), subType != item->getItemCount() ? subType : -1));
	}

	if (getBoolean(ConfigManager::ONLY_INVITED_CAN_MOVE_HOUSE_ITEMS)) {
		if (const auto& tile = item->getTile()) {
			if (const auto& houseTile = tile->getHouseTile()) {
				if (!item->getTopParent()->getCreature() && !houseTile->getHouse()->isInvited(player)) {
					player->sendCancelMessage(RETURNVALUE_PLAYERISNOTINVITED);
					return false;
				}
			}
		}
	}

	if (action->executeUse(player, item, fromPos, action->getTarget(player, creature, toPos, toStackPos), toPos,
	                       isHotkey)) {
		return true;
	}

	if (!action->hasOwnErrorHandler()) {
		player->sendCancelMessage(RETURNVALUE_CANNOTUSETHISOBJECT);
	}

	return false;
}

Action::Action(LuaScriptInterface* interface) :
    Event(interface), function(nullptr), allowFarUse(false), checkFloor(true), checkLineOfSight(true)
{}

ReturnValue Action::canExecuteAction(const std::shared_ptr<const Player>& player, const Position& toPos)
{
	if (allowFarUse) {
		return g_actions->canUseFar(player, toPos, checkLineOfSight, checkFloor);
	}
	return g_actions->canUse(player, toPos);
}

std::shared_ptr<Thing> Action::getTarget(const std::shared_ptr<Player>& player,
                                         const std::shared_ptr<Creature>& targetCreature, const Position& toPosition,
                                         uint8_t toStackPos) const
{
	if (targetCreature) {
		return targetCreature;
	}
	return g_game.internalGetThing(player, toPosition, toStackPos, 0, STACKPOS_USETARGET);
}

bool Action::executeUse(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item,
                        const Position& fromPosition, const std::shared_ptr<Thing>& target, const Position& toPosition,
                        bool isHotkey)
{
	// onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - Action::executeUse] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(scriptId, scriptInterface);

	lua_State* L = scriptInterface->getLuaState();

	scriptInterface->pushFunction(scriptId);

	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");

	tfs::lua::pushThing(L, item);
	tfs::lua::pushPosition(L, fromPosition);

	tfs::lua::pushThing(L, target);
	tfs::lua::pushPosition(L, toPosition);

	tfs::lua::pushBoolean(L, isHotkey);
	return scriptInterface->callFunction(6);
}
