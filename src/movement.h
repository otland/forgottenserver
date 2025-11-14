// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_MOVEMENT_H
#define FS_MOVEMENT_H

#include "baseevents.h"
#include "creature.h"
#include "luascript.h"
#include "vocation.h"

class MoveEvent;

extern Vocations g_vocations;

enum MoveEvent_t
{
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

using MoveEvent_ptr = std::unique_ptr<MoveEvent>;

struct MoveEventList
{
	std::list<MoveEvent> moveEvent[MOVE_EVENT_LAST];
};

class MoveEvents final : public BaseEvents
{
public:
	MoveEvents();
	~MoveEvents();

	// non-copyable
	MoveEvents(const MoveEvents&) = delete;
	MoveEvents& operator=(const MoveEvents&) = delete;

	uint32_t onCreatureMove(const std::shared_ptr<Creature>& creature, const std::shared_ptr<const Tile>& tile,
	                        MoveEvent_t eventType);
	ReturnValue onPlayerEquip(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item, slots_t slot,
	                          bool isCheck);
	ReturnValue onPlayerDeEquip(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item, slots_t slot);
	uint32_t onItemMove(const std::shared_ptr<Item>& item, const std::shared_ptr<Tile>& tile, bool isAdd);

	MoveEvent* getEvent(const std::shared_ptr<Item>& item, MoveEvent_t eventType);

	bool registerLuaEvent(MoveEvent* event);
	bool registerLuaFunction(MoveEvent* event);
	void clear(bool fromLua) override final;

	bool isValid(std::map<MoveEvent*, std::vector<uint32_t>> map, MoveEvent* event)
	{
		return map.find(event) != map.end();
	}
	bool isValidPos(std::map<MoveEvent*, std::vector<Position>> map, MoveEvent* event)
	{
		return map.find(event) != map.end();
	}
	void clearItemIdRange(MoveEvent* event) { itemIdRange.erase(event); }
	const std::vector<uint32_t>& getItemIdRange(MoveEvent* event) const { return itemIdRange.at(event); }
	void addItemId(MoveEvent* event, uint32_t id) { itemIdRange[event].emplace_back(id); }

	void clearActionIdRange(MoveEvent* event) { actionIdRange.erase(event); }
	const std::vector<uint32_t>& getActionIdRange(MoveEvent* event) const { return actionIdRange.at(event); }
	void addActionId(MoveEvent* event, uint32_t id) { actionIdRange[event].emplace_back(id); }

	void clearUniqueIdRange(MoveEvent* event) { uniqueIdRange.erase(event); }
	const std::vector<uint32_t>& getUniqueIdRange(MoveEvent* event) const { return uniqueIdRange.at(event); }
	void addUniqueId(MoveEvent* event, uint32_t id) { uniqueIdRange[event].emplace_back(id); }

	void clearPosList(MoveEvent* event) { posList.erase(event); }
	const std::vector<Position>& getPosList(MoveEvent* event) const { return posList.at(event); }
	void addPosList(MoveEvent* event, Position pos) { posList[event].emplace_back(pos); }

private:
	using MoveListMap = std::map<int32_t, MoveEventList>;
	using MovePosListMap = std::map<Position, MoveEventList>;
	void clearMap(MoveListMap& map, bool fromLua);
	void clearPosMap(MovePosListMap& map, bool fromLua);

	LuaScriptInterface& getScriptInterface() override;
	std::string_view getScriptBaseName() const override { return "movements"; }
	Event_ptr getEvent(const std::string& nodeName) override;
	bool registerEvent(Event_ptr event, const pugi::xml_node& node) override;

	void addEvent(MoveEvent moveEvent, int32_t id, MoveListMap& map);

	void addEvent(MoveEvent moveEvent, const Position& pos, MovePosListMap& map);
	MoveEvent* getEvent(const std::shared_ptr<const Tile>& tile, MoveEvent_t eventType);

	MoveEvent* getEvent(const std::shared_ptr<Item>& item, MoveEvent_t eventType, slots_t slot);

	MoveListMap uniqueIdMap;
	MoveListMap actionIdMap;
	MoveListMap itemIdMap;
	MovePosListMap positionMap;
	std::map<MoveEvent*, std::vector<uint32_t>> itemIdRange;
	std::map<MoveEvent*, std::vector<uint32_t>> actionIdRange;
	std::map<MoveEvent*, std::vector<uint32_t>> uniqueIdRange;
	std::map<MoveEvent*, std::vector<Position>> posList;

	LuaScriptInterface scriptInterface;
};

using StepFunction = std::function<uint32_t(const std::shared_ptr<Creature>& creature,
                                            const std::shared_ptr<Item>& item, const Position& pos)>;
using MoveFunction = std::function<uint32_t(const std::shared_ptr<Item>& item, const std::shared_ptr<Item>& tileItem,
                                            const Position& pos)>;
using EquipFunction = std::function<ReturnValue(MoveEvent* moveEvent, const std::shared_ptr<Player>& player,
                                                const std::shared_ptr<Item>& item, slots_t slot, bool boolean)>;

class MoveEvent final : public Event
{
public:
	explicit MoveEvent(LuaScriptInterface* interface);

	MoveEvent_t getEventType() const;
	void setEventType(MoveEvent_t type);

	bool configureEvent(const pugi::xml_node& node) override;
	bool loadFunction(const pugi::xml_attribute& attr, bool isScripted) override;

	uint32_t fireStepEvent(const std::shared_ptr<Creature>& creature, const std::shared_ptr<Item>& item,
	                       const Position& pos);
	uint32_t fireAddRemItem(const std::shared_ptr<Item>& item, const std::shared_ptr<Item>& tileItem,
	                        const Position& pos);
	ReturnValue fireEquip(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item, slots_t slot,
	                      bool isCheck);

	uint32_t getSlot() const { return slot; }

	// scripting
	bool executeStep(const std::shared_ptr<Creature>& creature, const std::shared_ptr<Item>& item, const Position& pos);
	bool executeEquip(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item, slots_t slot,
	                  bool isCheck);
	bool executeAddRemItem(const std::shared_ptr<Item>& item, const std::shared_ptr<Item>& tileItem,
	                       const Position& pos);
	//

	// onEquip information
	uint32_t getReqLevel() const { return reqLevel; }
	uint32_t getReqMagLv() const { return reqMagLevel; }
	bool isPremium() const { return premium; }
	const std::string& getVocationString() const { return vocationString; }
	void setVocationString(const std::string& str) { vocationString = str; }
	uint32_t getWieldInfo() const { return wieldInfo; }
	const auto& getVocationEquipSet() const { return vocationEquipSet; }
	void addVocationEquipSet(const std::string& vocationName)
	{
		int32_t vocationId = g_vocations.getVocationId(vocationName);
		if (vocationId != -1) {
			vocationEquipSet.insert(vocationId);
		}
	}
	// If the set is empty, it is considered to be for all vocations.
	bool hasVocationEquipSet(uint16_t vocationId) const
	{
		return vocationEquipSet.empty() || vocationEquipSet.find(vocationId) != vocationEquipSet.end();
	}
	bool getTileItem() const { return tileItem; }
	void setTileItem(bool b) { tileItem = b; }
	void setSlot(uint32_t s) { slot = s; }
	uint32_t getRequiredLevel() { return reqLevel; }
	void setRequiredLevel(uint32_t level) { reqLevel = level; }
	uint32_t getRequiredMagLevel() { return reqMagLevel; }
	void setRequiredMagLevel(uint32_t level) { reqMagLevel = level; }
	bool needPremium() { return premium; }
	void setNeedPremium(bool b) { premium = b; }
	uint32_t getWieldInfo() { return wieldInfo; }
	void setWieldInfo(WieldInfo_t info) { wieldInfo |= info; }

	static uint32_t StepInField(const std::shared_ptr<Creature>& creature, const std::shared_ptr<Item>& item,
	                            const Position& pos);
	static uint32_t StepOutField(const std::shared_ptr<Creature>& creature, const std::shared_ptr<Item>& item,
	                             const Position& pos);

	static uint32_t AddItemField(const std::shared_ptr<Item>& item, const std::shared_ptr<Item>& tileItem,
	                             const Position& pos);
	static uint32_t RemoveItemField(const std::shared_ptr<Item>& item, const std::shared_ptr<Item>& tileItem,
	                                const Position& pos);

	static ReturnValue EquipItem(MoveEvent* moveEvent, const std::shared_ptr<Player>& player,
	                             const std::shared_ptr<Item>& item, slots_t slot, bool isCheck);
	static ReturnValue DeEquipItem(MoveEvent* moveEvent, const std::shared_ptr<Player>& player,
	                               const std::shared_ptr<Item>& item, slots_t slot, bool);

	MoveEvent_t eventType = MOVE_EVENT_NONE;
	StepFunction stepFunction;
	MoveFunction moveFunction;
	EquipFunction equipFunction;

private:
	std::string_view getScriptEventName() const override;

	uint32_t slot = SLOTP_WHEREEVER;

	// onEquip information
	uint32_t reqLevel = 0;
	uint32_t reqMagLevel = 0;
	bool premium = false;
	std::string vocationString;
	uint32_t wieldInfo = 0;
	std::unordered_set<uint16_t> vocationEquipSet;
	bool tileItem = false;
};

#endif // FS_MOVEMENT_H
