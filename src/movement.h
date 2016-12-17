/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2016  Mark Samman <mark.samman@gmail.com>
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

typedef std::map<uint16_t, bool> VocEquipMap;

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

	protected:
		typedef std::map<int32_t, MoveEventList> MoveListMap;
		void clearMap(MoveListMap& map);

		typedef std::map<Position, MoveEventList> MovePosListMap;
		void clear() final;
		LuaScriptInterface& getScriptInterface() final;
		std::string getScriptBaseName() const final;
		Event* getEvent(const std::string& nodeName) final;
		bool registerEvent(Event* event, const pugi::xml_node& node) final;

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

typedef uint32_t (StepFunction)(Creature* creature, Item* item, const Position& pos);
typedef uint32_t (MoveFunction)(Item* item, Item* tileItem, const Position& pos);
typedef uint32_t (EquipFunction)(MoveEvent* moveEvent, Player* player, Item* item, slots_t slot, bool boolean);

class MoveEvent final : public Event
{
	public:
		explicit MoveEvent(LuaScriptInterface* interface);
		explicit MoveEvent(const MoveEvent* copy);

		MoveEvent_t getEventType() const;
		void setEventType(MoveEvent_t type);

		bool configureEvent(const pugi::xml_node& node) final;
		bool loadFunction(const pugi::xml_attribute& attr) final;

		uint32_t fireStepEvent(Creature* creature, Item* item, const Position& pos);
		uint32_t fireAddRemItem(Item* item, Item* tileItem, const Position& pos);
		uint32_t fireEquip(Player* player, Item* item, slots_t slot, bool boolean);

		uint32_t getSlot() const {
			return slot;
		}

		//scripting
		bool executeStep(Creature* creature, Item* item, const Position& pos);
		bool executeEquip(Player* player, Item* item, slots_t slot);
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

	protected:
		std::string getScriptEventName() const final;

		static StepFunction StepInField;
		static StepFunction StepOutField;

		static MoveFunction AddItemField;
		static MoveFunction RemoveItemField;
		static EquipFunction EquipItem;
		static EquipFunction DeEquipItem;

		MoveEvent_t eventType = MOVE_EVENT_NONE;
		StepFunction* stepFunction = nullptr;
		MoveFunction* moveFunction = nullptr;
		EquipFunction* equipFunction = nullptr;
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
