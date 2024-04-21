// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_IOMAPSERIALIZE_H
#define FS_IOMAPSERIALIZE_H

class Container;
class Cylinder;
class Database;
class House;
class Item;
class Map;
class PropStream;
class PropWriteStream;
class Tile;

struct DBHouseList
{
	uint32_t listId;
	std::string listText;
};

struct DBHouseInfo
{
	uint32_t owner;
	time_t paidUntil;
	uint32_t payrentWarnings;
	std::string name;
	uint32_t townId;
	uint32_t rent;
	size_t size;
	uint32_t bedCount;
	std::list<DBHouseList> lists{};
};

using DBHouseInfoPtr = std::shared_ptr<DBHouseInfo>;
using DBHouseTileList = std::list<std::string>;
using DBHouseTileListPtr = std::shared_ptr<DBHouseTileList>;

class IOMapSerialize
{
public:
	static void loadHousesItems(Map* map);
	static bool saveHousesItems(bool async = false);
	static bool loadHousesInfo();
	static bool saveHousesInfo(bool async = false);
	static bool saveHouse(House* house, bool async = false);

private:
	static void saveItem(PropWriteStream& stream, const Item* item);
	static void saveTile(PropWriteStream& stream, const Tile* tile);

	static bool loadContainer(PropStream& propStream, Container* container);
	static bool loadItem(PropStream& propStream, Cylinder* parent);

	static bool saveHouseInfo(const uint32_t& houseId, DBHouseInfoPtr houseInfo, Database& db);
	static bool saveHouseItems(const uint32_t& houseId, DBHouseTileListPtr tileList, Database& db);
};

#endif // FS_IOMAPSERIALIZE_H
