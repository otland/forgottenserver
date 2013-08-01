/**
 * The Forgotten Server - a server application for the MMORPG Tibia
 * Copyright (C) 2013  Mark Samman <mark.samman@gmail.com>
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

#ifndef __OTSERV_HOUSE_H__
#define __OTSERV_HOUSE_H__

#include <string>
#include <list>
#include <map>

#include <regex>

#include "definitions.h"
#include "position.h"
#include "housetile.h"
#include "player.h"

class House;
class BedItem;

class AccessList
{
	public:
		AccessList();
		~AccessList();

		bool parseList(const std::string& _list);
		bool addPlayer(std::string& name);
		bool addGuild(const std::string& guildName, const std::string& rank);
		bool addExpression(const std::string& expression);

		bool isInList(const Player* player);

		void getList(std::string& _list) const;

	private:
		typedef std::unordered_set<uint32_t> PlayerList;
		typedef std::unordered_set<uint32_t> GuildList; //TODO: include ranks

		typedef std::list<std::string> ExpressionList;
		typedef std::list<std::pair<std::regex, bool> > RegExList;
		std::string list;
		PlayerList playerList;
		GuildList guildList;
		ExpressionList expressionList;
		RegExList regExList;
};

class Door : public Item
{
	public:
		Door(uint16_t _type);
		virtual ~Door();

		virtual Door* getDoor() {
			return this;
		}
		virtual const Door* getDoor() const {
			return this;
		}

		House* getHouse() {
			return house;
		}

		//serialization
		virtual Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream);
		virtual bool serializeAttr(PropWriteStream& propWriteStream) const;

		void setDoorId(uint32_t _doorId) {
			setIntAttr(ATTR_ITEM_DOORID, (uint32_t)_doorId);
		}
		uint32_t getDoorId() const {
			return getIntAttr(ATTR_ITEM_DOORID);
		}

		bool canUse(const Player* player);

		void setAccessList(const std::string& textlist);
		bool getAccessList(std::string& list) const;

		//overrides
		virtual void onRemoved();
		void copyAttributes(Item* item);

	protected:
		void setHouse(House* _house);

	private:
		House* house;
		AccessList* accessList;
		friend class House;
};

enum AccessList_t {
	GUEST_LIST = 0x100,
	SUBOWNER_LIST = 0x101
};

enum AccessHouseLevel_t {
	HOUSE_NO_INVITED = 0,
	HOUSE_GUEST = 1,
	HOUSE_SUBOWNER = 2,
	HOUSE_OWNER = 3
};

typedef std::list<HouseTile*> HouseTileList;
typedef std::list<Door*> HouseDoorList;
typedef std::list<BedItem*> HouseBedItemList;

class HouseTransferItem : public Item
{
	public:
		static HouseTransferItem* createHouseTransferItem(House* house);

		HouseTransferItem(House* _house) : Item(0) {
			house = _house;
		}
		virtual ~HouseTransferItem() {}

		virtual bool onTradeEvent(TradeEvents_t event, Player* owner);

		House* getHouse() {
			return house;
		}
		virtual bool canTransform() const {
			return false;
		}

	protected:
		House* house;
};

class House
{
	public:
		House(uint32_t _houseid);
		~House() {}

		void addTile(HouseTile* tile);
		void updateDoorDescription();

		bool canEditAccessList(uint32_t listId, const Player* player);
		// listId special values:
		//	GUEST_LIST	 guest list
		//  SUBOWNER_LIST subowner list
		void setAccessList(uint32_t listId, const std::string& textlist);
		bool getAccessList(uint32_t listId, std::string& list) const;

		bool isInvited(const Player* player);

		AccessHouseLevel_t getHouseAccessLevel(const Player* player);
		bool kickPlayer(Player* player, Player* target);

		void setEntryPos(const Position& pos) {
			posEntry = pos;
		}
		const Position& getEntryPosition() const {
			return posEntry;
		}

		void setName(const std::string& _houseName) {
			houseName = _houseName;
		}
		const std::string& getName() const {
			return houseName;
		}

		void setHouseOwner(uint32_t guid, bool updateDatabase = true, Player* player = NULL);
		uint32_t getHouseOwner() const {
			return houseOwner;
		}

		void setPaidUntil(uint32_t paid) {
			paidUntil = paid;
		}
		uint32_t getPaidUntil() const {
			return paidUntil;
		}

		void setRent(uint32_t _rent) {
			rent = _rent;
		}
		uint32_t getRent() const {
			return rent;
		}

		void setPayRentWarnings(uint32_t warnings) {
			rentWarnings = warnings;
		}
		uint32_t getPayRentWarnings() const {
			return rentWarnings;
		}

		void setTownId(uint32_t _town) {
			townid = _town;
		}
		uint32_t getTownId() const {
			return townid;
		}

		uint32_t getHouseId() const {
			return houseid;
		}

		void addDoor(Door* door);
		void removeDoor(Door* door);
		Door* getDoorByNumber(uint32_t doorId);
		Door* getDoorByNumber(uint32_t doorId) const;
		Door* getDoorByPosition(const Position& pos);

		HouseTransferItem* getTransferItem();
		void resetTransferItem();
		bool executeTransfer(HouseTransferItem* item, Player* player);

		const HouseTileList& getHouseTiles() const {
			return houseTiles;
		}

		const HouseDoorList& getHouseDoors() const {
			return doorList;
		}

		void addBed(BedItem* bed);
		HouseBedItemList::iterator getHouseBedsBegin() {
			return bedsList.begin();
		}
		HouseBedItemList::iterator getHouseBedsEnd() {
			return bedsList.end();
		}
		size_t getBedTiles() {
			return bedsList.size();
		}
		uint32_t getBedCount() {
			return (uint32_t)std::ceil((double)getBedTiles() / 2);   //each bed takes 2 sqms of space, ceil is just for bad maps
		}

	private:
		bool transferToDepot();
		bool transferToDepot(Player* player);

		bool isLoaded;
		uint32_t houseid;
		uint32_t houseOwner;
		std::string houseOwnerName;
		HouseTileList houseTiles;
		HouseDoorList doorList;
		HouseBedItemList bedsList;
		AccessList guestList;
		AccessList subOwnerList;
		std::string houseName;
		Position posEntry;
		uint32_t paidUntil;
		uint32_t rentWarnings;
		uint32_t rent;
		uint32_t townid;

		HouseTransferItem* transferItem;
		Container transfer_container;
};

typedef std::map<uint32_t, House*> HouseMap;

enum RentPeriod_t {
	RENTPERIOD_DAILY,
	RENTPERIOD_WEEKLY,
	RENTPERIOD_MONTHLY,
	RENTPERIOD_YEARLY,
	RENTPERIOD_NEVER
};

class Houses
{
		Houses();
		~Houses() {
			for (HouseMap::iterator it = houseMap.begin(); it != houseMap.end(); ++it) {
				delete it->second;
			}
		}

	public:
		static Houses& getInstance() {
			static Houses instance;
			return instance;
		}

		House* getHouse(uint32_t houseid, bool add = false) {
			HouseMap::iterator it = houseMap.find(houseid);

			if (it != houseMap.end()) {
				return it->second;
			}

			if (add) {
				House* house = new House(houseid);
				houseMap[houseid] = house;
				return house;
			} else {
				return NULL;
			}
		}

		House* getHouseByPlayerId(uint32_t playerId);

		bool loadHousesXML(const std::string& filename);

		bool payHouses();

		const HouseMap& getHouses() const {
			return houseMap;
		}

	private:
		RentPeriod_t rentPeriod;
		HouseMap houseMap;
};

#endif
