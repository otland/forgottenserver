// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_ACTIONS_H
#define FS_ACTIONS_H

#include "baseevents.h"
#include "enums.h"
#include "luascript.h"

class Action;
using Action_shared_ptr = std::shared_ptr<Action>;
using ActionFunction = std::function<bool(Player* player, Item* item, const Position& fromPosition, Thing* target,
                                          const Position& toPosition, bool isHotkey)>;

class Action : public Event
{
public:
	explicit Action(LuaScriptInterface* interface);

	// scripting
	virtual bool executeUse(Player* player, Item* item, const Position& fromPosition, Thing* target,
	                        const Position& toPosition, bool isHotkey);

	bool getAllowFarUse() const { return allowFarUse; }
	void setAllowFarUse(bool v) { allowFarUse = v; }

	bool getCheckLineOfSight() const { return checkLineOfSight; }
	void setCheckLineOfSight(bool v) { checkLineOfSight = v; }

	bool getCheckFloor() const { return checkFloor; }
	void setCheckFloor(bool v) { checkFloor = v; }

	void clearItemIdRange() { return ids.clear(); }
	const std::vector<uint16_t>& getItemIdRange() const { return ids; }
	void addItemId(uint16_t id) { ids.emplace_back(id); }

	void clearUniqueIdRange() { return uids.clear(); }
	const std::vector<uint16_t>& getUniqueIdRange() const { return uids; }
	void addUniqueId(uint16_t id) { uids.emplace_back(id); }

	void clearActionIdRange() { return aids.clear(); }
	const std::vector<uint16_t>& getActionIdRange() const { return aids; }
	void addActionId(uint16_t id) { aids.emplace_back(id); }

	virtual ReturnValue canExecuteAction(const Player* player, const Position& toPos);
	virtual bool hasOwnErrorHandler() { return false; }
	virtual Thing* getTarget(Player* player, Creature* targetCreature, const Position& toPosition,
	                         uint8_t toStackPos) const;

	ActionFunction function;

private:
	std::string_view getScriptEventName() const override { return "onUse"; }

	bool allowFarUse = false;
	bool checkFloor = true;
	bool checkLineOfSight = true;
	std::vector<uint16_t> ids;
	std::vector<uint16_t> uids;
	std::vector<uint16_t> aids;
};

class Actions
{
public:
	Actions();
	~Actions();

	// non-copyable
	Actions(const Actions&) = delete;
	Actions& operator=(const Actions&) = delete;

	bool useItem(Player* player, const Position& pos, uint8_t index, Item* item, bool isHotkey);
	bool useItemEx(Player* player, const Position& fromPos, const Position& toPos, uint8_t toStackPos, Item* item,
	               bool isHotkey, Creature* creature = nullptr);

	ReturnValue canUse(const Player* player, const Position& pos);
	ReturnValue canUse(const Player* player, const Position& pos, const Item* item);
	ReturnValue canUseFar(const Creature* creature, const Position& toPos, bool checkLineOfSight, bool checkFloor);

	bool registerLuaEvent(Action_shared_ptr action);
	Action_shared_ptr getActionEvent(const std::string& type, uint16_t id);
	void clear();

private:
	ReturnValue internalUseItem(Player* player, const Position& pos, uint8_t index, Item* item, bool isHotkey);

	using ActionUseMap = std::map<uint16_t, Action_shared_ptr>;
	ActionUseMap useItemMap;
	ActionUseMap uniqueItemMap;
	ActionUseMap actionItemMap;

	Action_shared_ptr getAction(const Item* item);

	LuaScriptInterface scriptInterface;
};

#endif // FS_ACTIONS_H
