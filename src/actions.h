// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_ACTIONS_H
#define FS_ACTIONS_H

#include "baseevents.h"
#include "enums.h"
#include "luascript.h"

class Action;
using Action_ptr = std::unique_ptr<Action>;
using ActionFunction =
    std::function<bool(std::shared_ptr<Player> player, std::shared_ptr<Item> item, const Position& fromPosition,
                       std::shared_ptr<Thing> target, const Position& toPosition, bool isHotkey)>;

class Action : public Event
{
public:
	explicit Action(LuaScriptInterface* interface);

	bool configureEvent(const pugi::xml_node&) override { return false; }

	// scripting
	virtual bool executeUse(std::shared_ptr<Player> player, std::shared_ptr<Item> item, const Position& fromPosition,
	                        std::shared_ptr<Thing> target, const Position& toPosition, bool isHotkey);

	bool getAllowFarUse() const { return allowFarUse; }
	void setAllowFarUse(bool v) { allowFarUse = v; }

	bool getCheckLineOfSight() const { return checkLineOfSight; }
	void setCheckLineOfSight(bool v) { checkLineOfSight = v; }

	bool getCheckFloor() const { return checkFloor; }
	void setCheckFloor(bool v) { checkFloor = v; }

	virtual ReturnValue canExecuteAction(std::shared_ptr<const Player> player, const Position& toPos);
	virtual bool hasOwnErrorHandler() { return false; }
	virtual std::shared_ptr<Thing> getTarget(std::shared_ptr<Player> player, std::shared_ptr<Creature> targetCreature,
	                                         const Position& toPosition, uint8_t toStackPos) const;

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

	bool useItem(std::shared_ptr<Player> player, const Position& pos, uint8_t index, std::shared_ptr<Item> item,
	             bool isHotkey);
	bool useItemEx(std::shared_ptr<Player> player, const Position& fromPos, const Position& toPos, uint8_t toStackPos,
	               std::shared_ptr<Item> item, bool isHotkey, std::shared_ptr<Creature> creature = nullptr);

	ReturnValue canUse(std::shared_ptr<const Player> player, const Position& pos);
	ReturnValue canUse(std::shared_ptr<const Player> player, const Position& pos, std::shared_ptr<const Item> item);
	ReturnValue canUseFar(std::shared_ptr<const Creature> creature, const Position& toPos, bool checkLineOfSight,
	                      bool checkFloor);

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
	ReturnValue internalUseItem(std::shared_ptr<Player> player, const Position& pos, uint8_t index,
	                            std::shared_ptr<Item> item, bool isHotkey);

	LuaScriptInterface& getScriptInterface() override;
	std::string_view getScriptBaseName() const override { return "actions"; }
	Event_ptr getEvent(const std::string& nodeName) override;
	bool registerEvent(Event_ptr, const pugi::xml_node&) override { return false; }

	using ActionUseMap = std::map<uint16_t, Action>;
	ActionUseMap useItemMap;
	ActionUseMap uniqueItemMap;
	ActionUseMap actionItemMap;
	std::map<Action*, std::vector<uint16_t>> ids;
	std::map<Action*, std::vector<uint16_t>> uids;
	std::map<Action*, std::vector<uint16_t>> aids;

	Action* getAction(std::shared_ptr<const Item> item);
	void clearMap(ActionUseMap& map, bool fromLua);

	LuaScriptInterface scriptInterface;
};

#endif // FS_ACTIONS_H
