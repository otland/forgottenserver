//////////////////////////////////////////////////////////////////////
// The Forgotten Server - a server application for the MMORPG Tibia
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#ifndef __MOVEMENT_H__
#define __MOVEMENT_H__

#include "luascript.h"
#include "baseevents.h"
#include <map>

enum MoveEvent_t {
	MOVE_EVENT_STEP_IN = 0,
	MOVE_EVENT_STEP_OUT,
	MOVE_EVENT_EQUIP,
	MOVE_EVENT_DEEQUIP,
	MOVE_EVENT_ADD_ITEM,
	MOVE_EVENT_REMOVE_ITEM,
	MOVE_EVENT_ADD_ITEM_ITEMTILE,
	MOVE_EVENT_REMOVE_ITEM_ITEMTILE,
	MOVE_EVENT_LAST,
	MOVE_EVENT_NONE,
};

class MoveEvent;

struct MoveEventList {
	std::list<MoveEvent*> moveEvent[MOVE_EVENT_LAST];
};

typedef std::map<int32_t, bool> VocEquipMap;

class MoveEvents : public BaseEvents
{
	public:
		MoveEvents();
		virtual ~MoveEvents();

		uint32_t onCreatureMove(Creature* creature, const Tile* tile, bool isIn);
		uint32_t onPlayerEquip(Player* player, Item* item, slots_t slot, bool isCheck);
		uint32_t onPlayerDeEquip(Player* player, Item* item, slots_t slot, bool isRemoval);
		uint32_t onItemMove(Item* item, Tile* tile, bool isAdd);

		MoveEvent* getEvent(Item* item, MoveEvent_t eventType);

	protected:
		typedef std::map<int32_t, MoveEventList> MoveListMap;
		typedef std::map<Position, MoveEventList> MovePosListMap;
		virtual void clear();
		virtual LuaScriptInterface& getScriptInterface();
		virtual std::string getScriptBaseName();
		virtual Event* getEvent(const std::string& nodeName);
		virtual bool registerEvent(Event* event, xmlNodePtr p);

		void registerItemID(int32_t itemId, MoveEvent_t eventType);
		void registerActionID(int32_t actionId, MoveEvent_t eventType);
		void registerUniqueID(int32_t uniqueId, MoveEvent_t eventType);

		void addEvent(MoveEvent* moveEvent, int32_t id, MoveListMap& map);

		void addEvent(MoveEvent* moveEvent, Position pos, MovePosListMap& map);
		MoveEvent* getEvent(const Tile* tile, MoveEvent_t eventType);

		MoveEvent* getEvent(Item* item, MoveEvent_t eventType, slots_t slot);

		MoveListMap m_uniqueIdMap;
		MoveListMap m_actionIdMap;
		MoveListMap m_itemIdMap;
		MovePosListMap m_positionMap;

		LuaScriptInterface m_scriptInterface;
};

typedef uint32_t (StepFunction)(Creature* creature, Item* item, const Position& pos);
typedef uint32_t (MoveFunction)(Item* item, Item* tileItem, const Position& pos);
typedef uint32_t (EquipFunction)(MoveEvent* moveEvent, Player* player, Item* item, slots_t slot, bool boolean);

class MoveEvent : public Event
{
	public:
		MoveEvent(LuaScriptInterface* _interface);
		MoveEvent(const MoveEvent* copy);
		virtual ~MoveEvent();

		MoveEvent_t getEventType() const;
		void setEventType(MoveEvent_t type);

		virtual bool configureEvent(xmlNodePtr p);
		virtual bool loadFunction(const std::string& functionName);

		uint32_t fireStepEvent(Creature* creature, Item* item, const Position& pos);
		uint32_t fireAddRemItem(Item* item, Item* tileItem, const Position& pos);
		uint32_t fireEquip(Player* player, Item* item, slots_t slot, bool boolean);

		uint32_t getSlot() const {
			return slot;
		}

		//scripting
		uint32_t executeStep(Creature* creature, Item* item, const Position& pos);
		uint32_t executeEquip(Player* player, Item* item, slots_t slot);
		uint32_t executeAddRemItem(Item* item, Item* tileItem, const Position& pos);
		//

		//onEquip information
		int32_t getReqLevel() const {
			return reqLevel;
		}
		int32_t getReqMagLv() const {
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
		virtual std::string getScriptEventName();

		static StepFunction StepInField;
		static StepFunction StepOutField;

		static MoveFunction AddItemField;
		static MoveFunction RemoveItemField;
		static EquipFunction EquipItem;
		static EquipFunction DeEquipItem;

		MoveEvent_t m_eventType;
		StepFunction* stepFunction;
		MoveFunction* moveFunction;
		EquipFunction* equipFunction;
		uint32_t slot;

		//onEquip information
		int32_t reqLevel;
		int32_t reqMagLevel;
		bool premium;
		std::string vocationString;
		uint32_t wieldInfo;
		VocEquipMap vocEquipMap;
};

#endif
