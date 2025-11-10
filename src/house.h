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

	bool isInList(const std::shared_ptr<const Player>& player) const;

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

	std::shared_ptr<Door> getDoor() override { return std::static_pointer_cast<Door>(shared_from_this()); }
	std::shared_ptr<const Door> getDoor() const override
	{
		return std::static_pointer_cast<const Door>(shared_from_this());
	}

	House* getHouse() { return house; }

	// serialization
	Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream) override;
	void serializeAttr(PropWriteStream&) const override {}

	void setDoorId(uint32_t doorId) { setIntAttr(ITEM_ATTRIBUTE_DOORID, doorId); }
	uint32_t getDoorId() const { return getIntAttr(ITEM_ATTRIBUTE_DOORID); }

	bool canUse(const std::shared_ptr<const Player>& player);

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
	static std::shared_ptr<HouseTransferItem> createHouseTransferItem(House* house);

	explicit HouseTransferItem(House* house) : Item{0}, house{house} {}

	void onTradeEvent(TradeEvents_t event, const std::shared_ptr<Player>& owner) override;
	bool canTransform() const override { return false; }

private:
	House* house;
};

class House
{
public:
	explicit House(uint32_t houseId);

	void addTile(const std::shared_ptr<HouseTile>& tile);

	bool canEditAccessList(uint32_t listId, const std::shared_ptr<const Player>& player);
	// listId special values:
	// GUEST_LIST	 guest list
	// SUBOWNER_LIST subowner list
	void setAccessList(uint32_t listId, std::string_view textlist);
	bool getAccessList(uint32_t listId, std::string& list) const;

	bool isInvited(const std::shared_ptr<const Player>& player) const;

	AccessHouseLevel_t getHouseAccessLevel(const std::shared_ptr<const Player>& player) const;
	bool kickPlayer(const std::shared_ptr<Player>& player, const std::shared_ptr<Player>& target);

	void setEntryPos(Position pos) { posEntry = pos; }
	const Position& getEntryPosition() const { return posEntry; }

	void setName(std::string houseName) { this->houseName = houseName; }
	const std::string& getName() const { return houseName; }

	const std::string& getOwnerName() const { return ownerName; }

	void setOwner(uint32_t guid, bool updateDatabase = true, const std::shared_ptr<Player>& player = nullptr);
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

	void addDoor(const std::shared_ptr<Door>& door);
	void removeDoor(const std::shared_ptr<Door>& door);
	std::shared_ptr<Door> getDoorByNumber(uint32_t doorId) const;
	std::shared_ptr<Door> getDoorByPosition(const Position& pos);

	std::shared_ptr<HouseTransferItem> getTransferItem();
	void resetTransferItem();
	bool executeTransfer(const std::shared_ptr<HouseTransferItem>& item, const std::shared_ptr<Player>& newOwner);

	const auto& getTiles() const { return tiles; }
	const auto& getDoors() const { return doors; }

	void addBed(const std::shared_ptr<BedItem>& bed);
	const auto& getBeds() const { return beds; }

	// each bed takes 2 sqms of space, ceil is just for bad maps
	auto getBedCount() const { return (beds.size() + 1) / 2; }

private:
	bool transferToDepot() const;
	bool transferToDepot(const std::shared_ptr<Player>& player) const;

	AccessList guestList;
	AccessList subOwnerList;

	Container transferContainer{ITEM_LOCKER};

	boost::container::flat_set<std::weak_ptr<HouseTile>, std::owner_less<std::weak_ptr<HouseTile>>> tiles;
	boost::container::flat_set<std::weak_ptr<Door>, std::owner_less<std::weak_ptr<Door>>> doors;
	boost::container::flat_set<std::weak_ptr<BedItem>, std::owner_less<std::weak_ptr<BedItem>>> beds;

	std::string houseName;
	std::string ownerName;

	std::shared_ptr<HouseTransferItem> transferItem = nullptr;

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

using HouseMap = std::map<uint32_t, House*>;

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
	Houses() = default;
	~Houses()
	{
		for (const auto& it : houseMap) {
			delete it.second;
		}
	}

	// non-copyable
	Houses(const Houses&) = delete;
	Houses& operator=(const Houses&) = delete;

	House* addHouse(uint32_t id)
	{
		auto it = houseMap.find(id);
		if (it != houseMap.end()) {
			return it->second;
		}

		House* house = new House(id);
		houseMap[id] = house;
		return house;
	}

	House* getHouse(uint32_t houseId)
	{
		auto it = houseMap.find(houseId);
		if (it == houseMap.end()) {
			return nullptr;
		}
		return it->second;
	}

	House* getHouseByPlayerId(uint32_t playerId);

	bool loadHousesXML(const std::string& filename);

	void payHouses(RentPeriod_t rentPeriod) const;

	const HouseMap& getHouses() const { return houseMap; }

private:
	HouseMap houseMap;
};

#endif // FS_HOUSE_H
