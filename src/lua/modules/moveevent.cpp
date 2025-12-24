#include "../../otpch.h"

#include "../../movement.h"
#include "../../script.h"
#include "../api.h"
#include "../env.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

extern MoveEvents* g_moveEvents;
extern Scripts* g_scripts;

namespace {

int luaCreateMoveEvent(lua_State* L)
{
	// MoveEvent()
	if (tfs::lua::getScriptEnv()->getScriptInterface() != &g_scripts->getScriptInterface()) {
		tfs::lua::reportError(L, "MoveEvents can only be registered in the Scripts interface.");
		lua_pushnil(L);
		return 1;
	}

	MoveEvent* moveevent = new MoveEvent(tfs::lua::getScriptEnv()->getScriptInterface());
	moveevent->fromLua = true;
	tfs::lua::pushUserdata(L, moveevent);
	tfs::lua::setMetatable(L, -1, "MoveEvent");
	return 1;
}

int luaMoveEventType(lua_State* L)
{
	// moveevent:type(callback)
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		std::string typeName = tfs::lua::getString(L, 2);
		std::string tmpStr = boost::algorithm::to_lower_copy(typeName);
		if (tmpStr == "stepin") {
			moveevent->setEventType(MOVE_EVENT_STEP_IN);
			moveevent->stepFunction = moveevent->StepInField;
		} else if (tmpStr == "stepout") {
			moveevent->setEventType(MOVE_EVENT_STEP_OUT);
			moveevent->stepFunction = moveevent->StepOutField;
		} else if (tmpStr == "equip") {
			moveevent->setEventType(MOVE_EVENT_EQUIP);
			moveevent->equipFunction = moveevent->EquipItem;
		} else if (tmpStr == "deequip") {
			moveevent->setEventType(MOVE_EVENT_DEEQUIP);
			moveevent->equipFunction = moveevent->DeEquipItem;
		} else if (tmpStr == "additem") {
			moveevent->setEventType(MOVE_EVENT_ADD_ITEM);
			moveevent->moveFunction = moveevent->AddItemField;
		} else if (tmpStr == "removeitem") {
			moveevent->setEventType(MOVE_EVENT_REMOVE_ITEM);
			moveevent->moveFunction = moveevent->RemoveItemField;
		} else {
			std::cout << "Error: [MoveEvent::configureMoveEvent] No valid event name " << typeName << '\n';
			tfs::lua::pushBoolean(L, false);
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMoveEventRegister(lua_State* L)
{
	// moveevent:register()
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		if ((moveevent->getEventType() == MOVE_EVENT_EQUIP || moveevent->getEventType() == MOVE_EVENT_DEEQUIP) &&
		    moveevent->getSlot() == SLOTP_WHEREEVER) {
			uint32_t id = g_moveEvents->getItemIdRange(moveevent).at(0);
			ItemType& it = Item::items.getItemType(id);
			moveevent->setSlot(it.slotPosition);
		}
		if (!moveevent->isScripted()) {
			tfs::lua::pushBoolean(L, g_moveEvents->registerLuaFunction(moveevent));
			g_moveEvents->clearItemIdRange(moveevent);
			return 1;
		}
		tfs::lua::pushBoolean(L, g_moveEvents->registerLuaEvent(moveevent));
		g_moveEvents->clearItemIdRange(moveevent);
		g_moveEvents->clearActionIdRange(moveevent);
		g_moveEvents->clearUniqueIdRange(moveevent);
		g_moveEvents->clearPosList(moveevent);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMoveEventOnCallback(lua_State* L)
{
	// moveevent:onEquip / deEquip / etc. (callback)
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		if (!moveevent->loadCallback()) {
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMoveEventSlot(lua_State* L)
{
	// moveevent:slot(slot)
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (!moveevent) {
		lua_pushnil(L);
		return 1;
	}

	if (moveevent->getEventType() == MOVE_EVENT_EQUIP || moveevent->getEventType() == MOVE_EVENT_DEEQUIP) {
		std::string slotName = boost::algorithm::to_lower_copy(tfs::lua::getString(L, 2));
		if (slotName == "head") {
			moveevent->setSlot(SLOTP_HEAD);
		} else if (slotName == "necklace") {
			moveevent->setSlot(SLOTP_NECKLACE);
		} else if (slotName == "backpack") {
			moveevent->setSlot(SLOTP_BACKPACK);
		} else if (slotName == "armor" || slotName == "body") {
			moveevent->setSlot(SLOTP_ARMOR);
		} else if (slotName == "right-hand") {
			moveevent->setSlot(SLOTP_RIGHT);
		} else if (slotName == "left-hand") {
			moveevent->setSlot(SLOTP_LEFT);
		} else if (slotName == "hand" || slotName == "shield") {
			moveevent->setSlot(SLOTP_RIGHT | SLOTP_LEFT);
		} else if (slotName == "legs") {
			moveevent->setSlot(SLOTP_LEGS);
		} else if (slotName == "feet") {
			moveevent->setSlot(SLOTP_FEET);
		} else if (slotName == "ring") {
			moveevent->setSlot(SLOTP_RING);
		} else if (slotName == "ammo") {
			moveevent->setSlot(SLOTP_AMMO);
		} else {
			std::cout << "[Warning - MoveEvent::configureMoveEvent] Unknown slot type: " << slotName << '\n';
			tfs::lua::pushBoolean(L, false);
			return 1;
		}
	}

	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaMoveEventLevel(lua_State* L)
{
	// moveevent:level(lvl)
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		moveevent->setRequiredLevel(tfs::lua::getNumber<uint32_t>(L, 2));
		moveevent->setWieldInfo(WIELDINFO_LEVEL);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMoveEventMagLevel(lua_State* L)
{
	// moveevent:magicLevel(lvl)
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		moveevent->setRequiredMagLevel(tfs::lua::getNumber<uint32_t>(L, 2));
		moveevent->setWieldInfo(WIELDINFO_MAGLV);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMoveEventPremium(lua_State* L)
{
	// moveevent:premium(bool)
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		moveevent->setNeedPremium(tfs::lua::getBoolean(L, 2));
		moveevent->setWieldInfo(WIELDINFO_PREMIUM);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMoveEventVocation(lua_State* L)
{
	// moveevent:vocation(vocName[, showInDescription = false, lastVoc = false])
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		moveevent->addVocationEquipSet(tfs::lua::getString(L, 2));
		moveevent->setWieldInfo(WIELDINFO_VOCREQ);
		std::string tmp;
		bool showInDescription = false;
		bool lastVoc = false;
		if (tfs::lua::getBoolean(L, 3)) {
			showInDescription = tfs::lua::getBoolean(L, 3);
		}
		if (tfs::lua::getBoolean(L, 4)) {
			lastVoc = tfs::lua::getBoolean(L, 4);
		}
		if (showInDescription) {
			if (moveevent->getVocationString().empty()) {
				tmp = boost::algorithm::to_lower_copy(tfs::lua::getString(L, 2));
				tmp += "s";
				moveevent->setVocationString(tmp);
			} else {
				tmp = moveevent->getVocationString();
				if (lastVoc) {
					tmp += " and ";
				} else {
					tmp += ", ";
				}
				tmp += boost::algorithm::to_lower_copy(tfs::lua::getString(L, 2));
				tmp += "s";
				moveevent->setVocationString(tmp);
			}
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMoveEventTileItem(lua_State* L)
{
	// moveevent:tileItem(bool)
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		moveevent->setTileItem(tfs::lua::getBoolean(L, 2));
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMoveEventItemId(lua_State* L)
{
	// moveevent:id(ids)
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		int parameters = lua_gettop(L) - 1; // - 1 because self is a parameter aswell, which we want to skip ofc
		if (parameters > 1) {
			for (int i = 0; i < parameters; ++i) {
				g_moveEvents->addItemId(moveevent, tfs::lua::getNumber<uint32_t>(L, 2 + i));
			}
		} else {
			g_moveEvents->addItemId(moveevent, tfs::lua::getNumber<uint32_t>(L, 2));
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMoveEventActionId(lua_State* L)
{
	// moveevent:aid(ids)
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		int parameters = lua_gettop(L) - 1; // - 1 because self is a parameter aswell, which we want to skip ofc
		if (parameters > 1) {
			for (int i = 0; i < parameters; ++i) {
				g_moveEvents->addActionId(moveevent, tfs::lua::getNumber<uint32_t>(L, 2 + i));
			}
		} else {
			g_moveEvents->addActionId(moveevent, tfs::lua::getNumber<uint32_t>(L, 2));
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMoveEventUniqueId(lua_State* L)
{
	// moveevent:uid(ids)
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		int parameters = lua_gettop(L) - 1; // - 1 because self is a parameter aswell, which we want to skip ofc
		if (parameters > 1) {
			for (int i = 0; i < parameters; ++i) {
				g_moveEvents->addUniqueId(moveevent, tfs::lua::getNumber<uint32_t>(L, 2 + i));
			}
		} else {
			g_moveEvents->addUniqueId(moveevent, tfs::lua::getNumber<uint32_t>(L, 2));
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaMoveEventPosition(lua_State* L)
{
	// moveevent:position(positions)
	MoveEvent* moveevent = tfs::lua::getUserdata<MoveEvent>(L, 1);
	if (moveevent) {
		int parameters = lua_gettop(L) - 1; // - 1 because self is a parameter aswell, which we want to skip ofc
		if (parameters > 1) {
			for (int i = 0; i < parameters; ++i) {
				g_moveEvents->addPosList(moveevent, tfs::lua::getPosition(L, 2 + i));
			}
		} else {
			g_moveEvents->addPosList(moveevent, tfs::lua::getPosition(L, 2));
		}
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

} // namespace

void tfs::lua::registerMoveEvent(LuaScriptInterface& lsi)
{
	lsi.registerClass("MoveEvent", "", luaCreateMoveEvent);
	lsi.registerMethod("MoveEvent", "type", luaMoveEventType);
	lsi.registerMethod("MoveEvent", "register", luaMoveEventRegister);
	lsi.registerMethod("MoveEvent", "level", luaMoveEventLevel);
	lsi.registerMethod("MoveEvent", "magicLevel", luaMoveEventMagLevel);
	lsi.registerMethod("MoveEvent", "slot", luaMoveEventSlot);
	lsi.registerMethod("MoveEvent", "id", luaMoveEventItemId);
	lsi.registerMethod("MoveEvent", "aid", luaMoveEventActionId);
	lsi.registerMethod("MoveEvent", "uid", luaMoveEventUniqueId);
	lsi.registerMethod("MoveEvent", "position", luaMoveEventPosition);
	lsi.registerMethod("MoveEvent", "premium", luaMoveEventPremium);
	lsi.registerMethod("MoveEvent", "vocation", luaMoveEventVocation);
	lsi.registerMethod("MoveEvent", "tileItem", luaMoveEventTileItem);
	lsi.registerMethod("MoveEvent", "onEquip", luaMoveEventOnCallback);
	lsi.registerMethod("MoveEvent", "onDeEquip", luaMoveEventOnCallback);
	lsi.registerMethod("MoveEvent", "onStepIn", luaMoveEventOnCallback);
	lsi.registerMethod("MoveEvent", "onStepOut", luaMoveEventOnCallback);
	lsi.registerMethod("MoveEvent", "onAddItem", luaMoveEventOnCallback);
	lsi.registerMethod("MoveEvent", "onRemoveItem", luaMoveEventOnCallback);
}
