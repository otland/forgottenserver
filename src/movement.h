/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2017  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_MOVEMENT_H_5E0D2626D4634ACA83AC6509518E5F49
#define FS_MOVEMENT_H_5E0D2626D4634ACA83AC6509518E5F49

#include "baseevents.h"
#include "item.h"
#include "luascript.h"

enum MoveEvent_t {
	MOVE_EVENT_STEP_IN,
	MOVE_EVENT_STEP_OUT,
	MOVE_EVENT_EQUIP,
	MOVE_EVENT_DEEQUIP,
	MOVE_EVENT_ADD_ITEM,
	MOVE_EVENT_REMOVE_ITEM,
	MOVE_EVENT_ADD_ITEM_ITEMTILE,
	MOVE_EVENT_REMOVE_ITEM_ITEMTILE,

	MOVE_EVENT_LAST,
	MOVE_EVENT_NONE
};

class MoveEvent;

struct MoveEventList {
	std::list<MoveEvent*> moveEvent[MOVE_EVENT_LAST];
};

using VocEquipMap = std::map<uint16_t, bool>;

class MoveEvents final : public BaseEvents
{
	public:
		MoveEvents();
		~MoveEvents();

		// non-copyable
		MoveEvents(const MoveEvents&) = delete;
		MoveEvents& operator=(const MoveEvents&) = delete;

		uint32_t onCreatureMove(Creature* creature, const Tile* tile, MoveEvent_t eventType);
		uint32_t onPlayerEquip(Player* player, Item* item, slots_t slot, bool isCheck);
		uint32_t onPlayerDeEquip(Player* player, Item* item, slots_t slot);
		uint32_t onItemMove(Item* item, Tile* tile, bool isAdd);

		MoveEvent* getEvent(Item* item, MoveEvent_t eventType);

	private:
		using MoveListMap = std::map<int32_t, MoveEventList>;
		void clearMap(MoveListMap& map);

		using MovePosListMap = std::map<Position, MoveEventList>;
		void clear() override;
		LuaScriptInterface& getScriptInterface() override;
		std::string getScriptBaseName() const override;
		Event* getEvent(const std::string& nodeName) override;
		bool registerEvent(Event* event, const pugi::xml_node& node) override;

		void addEvent(MoveEvent* moveEvent, int32_t id, MoveListMap& map);

		void addEvent(MoveEvent* moveEvent, const Position& pos, MovePosListMap& map);
		MoveEvent* getEvent(const Tile* tile, MoveEvent_t eventType);

		MoveEvent* getEvent(Item* item, MoveEvent_t eventType, slots_t slot);

		MoveListMap uniqueIdMap;
		MoveListMap actionIdMap;
		MoveListMap itemIdMap;
		MovePosListMap positionMap;

		LuaScriptInterface scriptInterface;
};

using StepFunction = std::function<uint32_t(Creature* creature, Item* item, const Position& pos)>;
using MoveFunction = std::function<uint32_t(Item* item, Item* tileItem, const Position& pos)>;
using EquipFunction = std::function<uint32_t(MoveEvent* moveEvent, Player* player, Item* item, slots_t slot, bool boolean)>;

class MoveEvent final : public Event
{
	public:
		explicit MoveEvent(LuaScriptInterface* interface);

		MoveEvent_t getEventType() const;
		void setEventType(MoveEvent_t type);

		bool configureEvent(const pugi::xml_node& node) override;
		bool loadFunction(const pugi::xml_attribute& attr) override;

		uint32_t fireStepEvent(Creature* creature, Item* item, const Position& pos);
		uint32_t fireAddRemItem(Item* item, Item* tileItem, const Position& pos);
		uint32_t fireEquip(Player* player, Item* item, slots_t slot, bool isCheck);

		uint32_t getSlot() const {
			return slot;
		}

		//scripting
		bool executeStep(Creature* creature, Item* item, const Position& pos);
		bool executeEquip(Player* player, Item* item, slots_t slot, bool isCheck);
		bool executeAddRemItem(Item* item, Item* tileItem, const Position& pos);
		//

		//onEquip information
		uint32_t getReqLevel() const {
			return reqLevel;
		}
		uint32_t getReqMagLv() const {
			return reqMagLevel;
		}
		bool isPremium() const {
			return premium;
		}
		const std::string& getVocationString() const {
			return vocationString;
		}
		uint32_t getWieldInfo() const {
			return wieldInfo;
		}
		const VocEquipMap& getVocEquipMap() const {
			return vocEquipMap;
		}

	private:
		std::string getScriptEventName() const override;

		MoveEvent_t eventType = MOVE_EVENT_NONE;
		StepFunction stepFunction;
		MoveFunction moveFunction;
		EquipFunction equipFunction;
		uint32_t slot = SLOTP_WHEREEVER;

		//onEquip information
		uint32_t reqLevel = 0;
		uint32_t reqMagLevel = 0;
		bool premium = false;
		std::string vocationString;
		uint32_t wieldInfo = 0;
		VocEquipMap vocEquipMap;
};

#endif
