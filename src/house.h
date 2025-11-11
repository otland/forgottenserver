// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_HOUSE_H
#define FS_HOUSE_H

#include "const.h"
#include "container.h"
#include "enums.h"
#include "position.h"

class BedItem;
class House;
class HouseTile;
class Player;

class AccessList
{
public:
	void parseList(std::string_view list);
	void addPlayer(const std::string& name);
	void addGuild(const std::string& name);
	void addGuildRank(const std::string& name, const std::string& rankName);

	bool isInList(const Player* player) const;

	void getList(std::string& list) const;

private:
	std::string list;
	std::unordered_set<uint32_t> playerList;
	std::unordered_set<uint32_t> guildRankList;
	bool allowEveryone = false;
};

class Door final : public Item
{
public:
	explicit Door(uint16_t type);

	// non-copyable
	Door(const Door&) = delete;
	Door& operator=(const Door&) = delete;

	Door* getDoor() override { return this; }
	const Door* getDoor() const override { return this; }

	House* getHouse() { return house; }

	// serialization
	Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream) override;
	void serializeAttr(PropWriteStream&) const override {}

	void setDoorId(uint32_t doorId) { setIntAttr(ITEM_ATTRIBUTE_DOORID, doorId); }
	uint32_t getDoorId() const { return getIntAttr(ITEM_ATTRIBUTE_DOORID); }

	bool canUse(const Player* player);

	void setAccessList(std::string_view textlist);
	bool getAccessList(std::string& list) const;

	void onRemoved() override;

private:
	void setHouse(House* house);

	House* house = nullptr;
	std::unique_ptr<AccessList> accessList;
	friend class House;
};

enum AccessList_t
{
	GUEST_LIST = 0x100,
	SUBOWNER_LIST = 0x101,
};

enum AccessHouseLevel_t
{
	HOUSE_NOT_INVITED = 0,
	HOUSE_GUEST = 1,
	HOUSE_SUBOWNER = 2,
	HOUSE_OWNER = 3,
};

class HouseTransferItem final : public Item
{
public:
	static HouseTransferItem* createHouseTransferItem(House* house);

	explicit HouseTransferItem(House* house) : Item(0), house(house) {}

	void onTradeEvent(TradeEvents_t event, Player* owner) override;
	bool canTransform() const override { return false; }

private:
	House* house;
};

class House
{
public:
	explicit House(uint32_t houseId);

	void addTile(HouseTile* tile);

	bool canEditAccessList(uint32_t listId, const Player* player);
	// listId special values:
	// GUEST_LIST	 guest list
	// SUBOWNER_LIST subowner list
	void setAccessList(uint32_t listId, std::string_view textlist);
	bool getAccessList(uint32_t listId, std::string& list) const;

	bool isInvited(const Player* player) const;

	AccessHouseLevel_t getHouseAccessLevel(const Player* player) const;
	bool kickPlayer(Player* player, Player* target);

	void setEntryPos(Position pos) { posEntry = pos; }
	const Position& getEntryPosition() const { return posEntry; }

	void setName(std::string houseName) { this->houseName = houseName; }
	const std::string& getName() const { return houseName; }

	const std::string& getOwnerName() const { return ownerName; }

	void setOwner(uint32_t guid, bool updateDatabase = true, Player* player = nullptr);
	uint32_t getOwner() const { return owner; }

	void setPaidUntil(time_t paid) { paidUntil = paid; }
	time_t getPaidUntil() const { return paidUntil; }

	void setRent(uint32_t rent) { this->rent = rent; }
	uint32_t getRent() const { return rent; }

	void setPayRentWarnings(uint32_t warnings) { rentWarnings = warnings; }
	uint32_t getPayRentWarnings() const { return rentWarnings; }

	void setTownId(uint32_t townId) { this->townId = townId; }
	uint32_t getTownId() const { return townId; }

	uint32_t getId() const { return id; }

	void addDoor(Door* door);
	void removeDoor(Door* door);
	Door* getDoorByNumber(uint32_t doorId) const;
	Door* getDoorByPosition(const Position& pos);

	HouseTransferItem* getTransferItem();
	void resetTransferItem();
	bool executeTransfer(HouseTransferItem* item, Player* newOwner);

	const auto& getTiles() const { return houseTiles; }

	const auto& getDoors() const { return doorSet; }

	void addBed(BedItem* bed);
	const auto& getBeds() const { return bedsList; }
	uint32_t getBedCount()
	{
		return static_cast<uint32_t>(
		    std::ceil(bedsList.size() / 2.)); // each bed takes 2 sqms of space, ceil is just for bad maps
	}

private:
	bool transferToDepot() const;
	bool transferToDepot(Player* player) const;

	AccessList guestList;
	AccessList subOwnerList;

	Container transfer_container{ITEM_LOCKER};

	boost::container::flat_set<HouseTile*> houseTiles;
	boost::container::flat_set<Door*> doorSet;
	boost::container::flat_set<BedItem*> bedsList;

	std::string houseName;
	std::string ownerName;

	HouseTransferItem* transferItem = nullptr;

	time_t paidUntil = 0;

	uint32_t id;
	uint32_t owner = 0;
	uint32_t ownerAccountId = 0;
	uint32_t rentWarnings = 0;
	uint32_t rent = 0;
	uint32_t townId = 0;

	Position posEntry = {};

	bool isLoaded = false;
};

enum RentPeriod_t
{
	RENTPERIOD_DAILY,
	RENTPERIOD_WEEKLY,
	RENTPERIOD_MONTHLY,
	RENTPERIOD_YEARLY,
	RENTPERIOD_NEVER,
};

class Houses
{
public:
	House* addHouse(uint32_t id);
	House* getHouse(uint32_t houseId);
	House* getHouseByPlayerId(uint32_t playerId);

	bool loadHousesXML(const std::string& filename);

	void payHouses(RentPeriod_t rentPeriod) const;

	const auto& getHouses() const { return houseMap; }

private:
	std::map<uint32_t, std::unique_ptr<House>> houseMap;
};

#endif // FS_HOUSE_H
