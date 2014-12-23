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

#ifndef FS_ACTIONS_H_87F60C5F587E4B84948F304A6451E6E6
#define FS_ACTIONS_H_87F60C5F587E4B84948F304A6451E6E6

#include "baseevents.h"
#include "enums.h"
#include "luascript.h"

typedef bool (ActionFunction)(Player* player, Item* item, const PositionEx& posFrom, const PositionEx& posTo, bool extendedUse, bool isHotkey);

class Action : public Event
{
	public:
		Action(const Action* copy);
		Action(LuaScriptInterface* _interface);

		bool configureEvent(const pugi::xml_node& node) override;
		bool loadFunction(const std::string& functionName) override;

		//scripting
		virtual bool executeUse(Player* player, Item* item, const PositionEx& posFrom,
			const PositionEx& posTo, bool extendedUse, uint32_t creatureId, bool isHotkey);
		//

		bool getAllowFarUse() const {
			return allowFarUse;
		}
		void setAllowFarUse(bool v) {
			allowFarUse = v;
		}

		bool getCheckLineOfSight() const {
			return checkLineOfSight;
		}
		void setCheckLineOfSight(bool v) {
			checkLineOfSight = v;
		}

		bool getCheckFloor() const {
			return checkFloor;
		}
		void setCheckFloor(bool v) {
			checkFloor = v;
		}

		virtual ReturnValue canExecuteAction(const Player* player, const Position& toPos);
		virtual bool hasOwnErrorHandler() {
			return false;
		}

		ActionFunction* function;

	protected:
		std::string getScriptEventName() const override;

		static ActionFunction increaseItemId;
		static ActionFunction decreaseItemId;
		static ActionFunction enterMarket;

		bool allowFarUse;
		bool checkFloor;
		bool checkLineOfSight;
};

class Actions final : public BaseEvents
{
	public:
		Actions();
		~Actions();

		// non-copyable
		Actions(const Actions&) = delete;
		Actions& operator=(const Actions&) = delete;

		bool useItem(Player* player, const Position& pos, uint8_t index, Item* item, bool isHotkey);
		bool useItemEx(Player* player, const Position& fromPos, const Position& toPos, uint8_t toStackPos, Item* item, bool isHotkey, uint32_t creatureId = 0);

		ReturnValue canUse(const Player* player, const Position& pos);
		ReturnValue canUse(const Player* player, const Position& pos, const Item* item);
		ReturnValue canUseFar(const Creature* creature, const Position& toPos, bool checkLineOfSight, bool checkFloor);

		bool hasAction(const Item* item);

	protected:
		ReturnValue internalUseItem(Player* player, const Position& pos, uint8_t index, Item* item, bool isHotkey);
		static void showUseHotkeyMessage(Player* player, const Item* item, uint32_t count);

		void clear() final;
		LuaScriptInterface& getScriptInterface() final;
		std::string getScriptBaseName() const final;
		Event* getEvent(const std::string& nodeName) final;
		bool registerEvent(Event* event, const pugi::xml_node& node) final;

		void registerItemID(int32_t itemId, Event* event);
		void registerActionID(int32_t actionId, Event* event);
		void registerUniqueID(int32_t uniqueId, Event* event);

		typedef std::map<uint16_t, Action*> ActionUseMap;
		ActionUseMap useItemMap;
		ActionUseMap uniqueItemMap;
		ActionUseMap actionItemMap;

		Action* getAction(const Item* item);
		void clearMap(ActionUseMap& map);

		LuaScriptInterface m_scriptInterface;
};

#endif
