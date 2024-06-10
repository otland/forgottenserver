// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_ACTIONS_H
#define FS_ACTIONS_H

#include "baseevents.h"
#include "enums.h"
#include "luascript.h"

class Action;
using Action_ptr = std::unique_ptr<Action>;
using ActionFunction = std::function<bool(Player* player, Item* item, const Position& fromPosition, Thing* target,
                                          const Position& toPosition, bool isHotkey)>;

class Action : public Event
{
public:
	explicit Action(LuaScriptInterface* interface);

	bool configureEvent(const pugi::xml_node& node) override;
	bool loadFunction(const pugi::xml_attribute& attr, bool isScripted) override;

	// scripting
	virtual bool executeUse(Player* player, Item* item, const Position& fromPosition, Thing* target,
	                        const Position& toPosition, bool isHotkey);

	bool getAllowFarUse() const { return allowFarUse; }
	void setAllowFarUse(bool v) { allowFarUse = v; }

	bool getCheckLineOfSight() const { return checkLineOfSight; }
	void setCheckLineOfSight(bool v) { checkLineOfSight = v; }

	bool getCheckFloor() const { return checkFloor; }
	void setCheckFloor(bool v) { checkFloor = v; }

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
	bool useItemEx(Player* player, const Position& fromPos, const Position& toPos, uint8_t toStackPos, Item* item,
	               bool isHotkey, Creature* creature = nullptr);

	ReturnValue canUse(const Player* player, const Position& pos);
	ReturnValue canUse(const Player* player, const Position& pos, const Item* item);
	ReturnValue canUseFar(const Creature* creature, const Position& toPos, bool checkLineOfSight, bool checkFloor);

	bool registerLuaEvent(Action* event);
	void clear(bool fromLua) override final;

	bool isValid(std::map<Action*, std::vector<uint16_t>> map, Action* action) { return map.find(action) != map.end(); }
	void clearItemIdRange(Action* action) { ids.erase(action); }
	const std::vector<uint16_t>& getItemIdRange(Action* action) const { return ids.at(action); }
	void addItemId(Action* action, uint16_t id) { ids[action].emplace_back(id); }

	void clearUniqueIdRange(Action* action) { uids.erase(action); }
	const std::vector<uint16_t>& getUniqueIdRange(Action* action) const { return uids.at(action); }
	void addUniqueId(Action* action, uint16_t id) { uids[action].emplace_back(id); }

	void clearActionIdRange(Action* action) { aids.erase(action); }
	const std::vector<uint16_t>& getActionIdRange(Action* action) const { return aids.at(action); }
	void addActionId(Action* action, uint16_t id) { aids[action].emplace_back(id); }

private:
	ReturnValue internalUseItem(Player* player, const Position& pos, uint8_t index, Item* item, bool isHotkey);

	LuaScriptInterface& getScriptInterface() override;
	std::string_view getScriptBaseName() const override { return "actions"; }
	Event_ptr getEvent(const std::string& nodeName) override;
	bool registerEvent(Event_ptr event, const pugi::xml_node& node) override;

	using ActionUseMap = std::map<uint16_t, Action>;
	ActionUseMap useItemMap;
	ActionUseMap uniqueItemMap;
	ActionUseMap actionItemMap;
	std::map<Action*, std::vector<uint16_t>> ids;
	std::map<Action*, std::vector<uint16_t>> uids;
	std::map<Action*, std::vector<uint16_t>> aids;

	Action* getAction(const Item* item);
	void clearMap(ActionUseMap& map, bool fromLua);

	LuaScriptInterface scriptInterface;
};

#endif // FS_ACTIONS_H
