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

#include "otpch.h"

#include "iomapserialize.h"
#include "house.h"
#include "configmanager.h"
#include "game.h"

extern ConfigManager g_config;
extern Game g_game;

bool IOMapSerialize::loadMap(Map* map)
{
	int64_t start = OTSYS_TIME();
	bool s = false;

	if (g_config.getString(ConfigManager::MAP_STORAGE_TYPE) == "binary-tilebased") {
		s = true;
		loadMapBinaryTileBased(map);
	} else if (g_config.getString(ConfigManager::MAP_STORAGE_TYPE) == "binary") {
		s = true;
		loadMapBinary(map);
	} else {
		s = loadMapRelational(map);
	}

	std::cout << "Notice: Map load (" << g_config.getString(ConfigManager::MAP_STORAGE_TYPE) << ") took: " <<
	          (OTSYS_TIME() - start) / (1000.) << " s" << std::endl;

	return s;
}

bool IOMapSerialize::saveMap(Map* map)
{
	int64_t start = OTSYS_TIME();
	bool s = false;

	if (g_config.getString(ConfigManager::MAP_STORAGE_TYPE) == "binary-tilebased") {
		s = saveMapBinaryTileBased(map);
	} else if (g_config.getString(ConfigManager::MAP_STORAGE_TYPE) == "binary") {
		s = saveMapBinary(map);
	} else {
		s = saveMapRelational(map);
	}

	std::cout << "Notice: Map save (" << g_config.getString(ConfigManager::MAP_STORAGE_TYPE) << ") took : " <<
	          (OTSYS_TIME() - start) / (1000.) << " s" << std::endl;

	return s;
}

bool IOMapSerialize::loadMapRelational(Map* map)
{
	Database* db = Database::getInstance();

	for (HouseMap::iterator it = Houses::getInstance().getHouseBegin(); it != Houses::getInstance().getHouseEnd(); ++it) {
		//load tile
		House* house = it->second;

		for (HouseTileList::iterator it = house->getHouseTileBegin(); it != house->getHouseTileEnd(); ++it) {
			loadTile(*db, *it);
		}
	}

	return true;
}

bool IOMapSerialize::loadTile(Database& db, Tile* tile)
{
	typedef std::map<int, std::pair<Item*, int> > ItemMap;
	ItemMap itemMap;

	const Position& tilePos = tile->getPosition();

	std::ostringstream query;
	query << "SELECT `id` FROM `tiles` WHERE `x` = " << tilePos.x
	      << " AND `y` = " << tilePos.y
	      << " AND `z` = " << tilePos.z;

	DBResult* result;

	if (!(result = db.storeQuery(query.str()))) {
		return false;
	}

	int tileId = result->getDataInt("id");
	db.freeResult(result);

	query.str("");
	query << "SELECT * FROM `tile_items` WHERE `tile_id` = " << tileId << " ORDER BY `sid` DESC";

	if ((result = db.storeQuery(query.str()))) {
		Item* item = NULL;

		do {
			int sid = result->getDataInt("sid");
			int pid = result->getDataInt("pid");
			int type = result->getDataInt("itemtype");
			int count = result->getDataInt("count");
			item = NULL;

			unsigned long attrSize = 0;
			const char* attr = result->getDataStream("attributes", attrSize);
			PropStream propStream;
			propStream.init(attr, attrSize);

			const ItemType& iType = Item::items[type];

			if (iType.moveable || /* or object in a container*/ pid != 0) {
				//create a new item
				item = Item::CreateItem(type, count);

				if (item) {
					if (item->unserializeAttr(propStream)) {
						if (pid == 0) {
							tile->__internalAddThing(item);
							item->__startDecaying();
						}
					} else {
						std::cout << "WARNING: Serialize error in IOMapSerialize::loadTile()" << std::endl;
					}
				} else {
					continue;
				}
			} else {
				//find this type in the tile
				if (const TileItemVector* items = tile->getItemList()) {
					for (ItemVector::const_iterator it = items->begin(), end = items->end(); it != end; ++it) {
						Item* findItem = (*it);

						if (findItem->getID() == type) {
							item = findItem;
							break;
						} else if (iType.isDoor() && findItem->getDoor()) {
							item = findItem;
							break;
						} else if (iType.isBed() && findItem->getBed()) {
							item = findItem;
							break;
						}
					}
				}
			}

			if (item) {
				if (item->unserializeAttr(propStream)) {
					item = g_game.transformItem(item, type);

					if (item) {
						std::pair<Item*, int> myPair(item, pid);
						itemMap[sid] = myPair;
					}
				} else {
					std::cout << "WARNING: Serialize error in IOMapSerialize::loadTile()" << std::endl;
				}
			} else {
				std::cout << "WARNING: IOMapSerialize::loadTile(). NULL item at " << tile->getPosition() << ". type = " << type << ", sid = " << sid << ", pid = " << pid << std::endl;
			}
		} while (result->next());

		db.freeResult(result);
	}

	ItemMap::reverse_iterator it;
	ItemMap::iterator it2;

	for (it = itemMap.rbegin(); it != itemMap.rend(); ++it) {
		Item* item = it->second.first;
		int pid = it->second.second;

		it2 = itemMap.find(pid);

		if (it2 != itemMap.end()) {
			if (Container* container = it2->second.first->getContainer()) {
				container->__internalAddThing(item);
				g_game.startDecay(item);
			}
		}
	}

	return true;
}

bool IOMapSerialize::saveMapRelational(Map* map)
{
	Database* db = Database::getInstance();

	//Start the transaction
	DBTransaction transaction(db);

	if (!transaction.begin()) {
		return false;
	}

	//clear old tile data
	if (!db->executeQuery("DELETE FROM `tile_items`")) {
		return false;
	}

	if (!db->executeQuery("DELETE FROM `tiles`")) {
		return false;
	}

	uint32_t tileId = 0;

	for (HouseMap::iterator it = Houses::getInstance().getHouseBegin(); it != Houses::getInstance().getHouseEnd(); ++it) {
		//save house items
		House* house = it->second;

		for (HouseTileList::iterator it = house->getHouseTileBegin(); it != house->getHouseTileEnd(); ++it) {
			++tileId;
			saveTile(db, tileId, *it);
		}
	}

	//End the transaction
	return transaction.commit();
}

bool IOMapSerialize::saveTile(Database* db, uint32_t tileId, const Tile* tile)
{
	typedef std::list<std::pair<Container*, int> > ContainerStackList;
	typedef ContainerStackList::value_type ContainerStackList_Pair;
	ContainerStackList containerStackList;

	bool storedTile = false;
	int32_t runningID = 0;
	Item* item = NULL;
	Container* container = NULL;

	int32_t parentid = 0;
	std::ostringstream streamitems;

	std::ostringstream tileListQuery;

	DBInsert stmt(db);
	stmt.setQuery("INSERT INTO `tile_items` (`tile_id`, `sid` , `pid` , `itemtype` , `count`, `attributes` ) VALUES ");

	if (const TileItemVector* items = tile->getItemList()) {
		for (ItemVector::const_iterator it = items->begin(), end = items->end(); it != end; ++it) {
			item = (*it);

			if (!(!item->isNotMoveable() ||
			        item->getDoor() ||
			        (item->getContainer() && item->getContainer()->size() != 0) ||
			        item->canWriteText() ||
			        item->getBed())) {
				continue;
			}

			//only save beds in houses
			if (item->getBed() && !tile->hasFlag(TILESTATE_HOUSE)) {
				continue;
			}

			if (!storedTile) {
				const Position& tilePos = tile->getPosition();
				tileListQuery << "INSERT INTO `tiles` (`id`, `x` , `y` , `z` ) VALUES (" << tileId << "," << tilePos.x << "," << tilePos.y << "," << tilePos.z << ")";

				if (!db->executeQuery(tileListQuery.str())) {
					return false;
				}

				tileListQuery.str("");
				storedTile = true;
			}

			++runningID;

			uint32_t attributesSize;

			PropWriteStream propWriteStream;
			item->serializeAttr(propWriteStream);
			const char* attributes = propWriteStream.getStream(attributesSize);

			streamitems << tileId << "," << runningID << "," << parentid << "," << item->getID() << ","
			            << (int32_t)item->getSubType() << "," << db->escapeBlob(attributes, attributesSize);

			if (!stmt.addRow(streamitems)) {
				return false;
			}

			if (item->getContainer()) {
				containerStackList.push_back(ContainerStackList_Pair(item->getContainer(), runningID));
			}
		}

		while (!containerStackList.empty()) {
			ContainerStackList_Pair csPair = containerStackList.front();
			container = csPair.first;
			parentid = csPair.second;
			containerStackList.pop_front();

			for (ItemDeque::const_iterator it = container->getItems(); it != container->getEnd(); ++it) {
				item = (*it);
				++runningID;

				if (item->getContainer()) {
					containerStackList.push_back(ContainerStackList_Pair(item->getContainer(), runningID));
				}

				uint32_t attributesSize;

				PropWriteStream propWriteStream;
				item->serializeAttr(propWriteStream);
				const char* attributes = propWriteStream.getStream(attributesSize);

				streamitems << tileId << "," << runningID << "," << parentid << "," << item->getID() << ","
				            << (int32_t)item->getSubType() << "," << db->escapeBlob(attributes, attributesSize);

				if (!stmt.addRow(streamitems)) {
					return false;
				}
			}
		}
	}

	return stmt.execute();
}

void IOMapSerialize::loadMapBinary(Map* map)
{
	Database* db = Database::getInstance();

	DBResult* result;

	if (!(result = db->storeQuery("SELECT `house_id`, `data` FROM `map_store`"))) {
		return;
	}

	do {
		unsigned long attrSize = 0;
		const char* attr = result->getDataStream("data", attrSize);
		PropStream propStream;
		propStream.init(attr, attrSize);

		while (propStream.size()) {
			uint32_t item_count = 0;
			uint16_t x = 0, y = 0;
			uint8_t z = 0;

			propStream.GET_USHORT(x);
			propStream.GET_USHORT(y);
			propStream.GET_UCHAR(z);

			Tile* tile = map->getTile(x, y, z);

			if (!tile) {
				std::cout << "ERROR: Unserialization of invalid tile in IOMapSerialize::loadTile(), at position: [X: " << x << ", Y: " << y << ", Z: " << z << "]. House ID: " << result->getDataInt("house_id") << std::endl;
				break;
			}

			propStream.GET_ULONG(item_count);

			while (item_count--) {
				loadItem(propStream, tile);
			}
		}
	} while (result->next());

	db->freeResult(result);
}

void IOMapSerialize::loadMapBinaryTileBased(Map* map)
{
	Database* db = Database::getInstance();
	std::ostringstream query;

	DBResult* result = db->storeQuery("SELECT `id` FROM `houses`");

	if (!result) {
		return;
	}

	do {
		query.str("");
		query << "SELECT `data` FROM `tile_store` WHERE `house_id` = " << result->getDataInt("id");
		DBResult* tileResult = db->storeQuery(query.str());

		if (!tileResult) {
			continue;
		}

		do {
			unsigned long attrSize = 0;
			const char* attr = tileResult->getDataStream("data", attrSize);

			PropStream propStream;
			propStream.init(attr, attrSize);

			uint16_t x = 0, y = 0;
			uint8_t z = 0;
			propStream.GET_USHORT(x);
			propStream.GET_USHORT(y);
			propStream.GET_UCHAR(z);

			if (x == 0 || y == 0) {
				continue;
			}

			Tile* tile = map->getTile(x, y, z);

			if (!tile) {
				continue;
			}

			uint32_t item_count = 0;
			propStream.GET_ULONG(item_count);

			while (item_count--) {
				loadItem(propStream, tile);
			}
		} while (tileResult->next());

		db->freeResult(tileResult);
	} while (result->next());

	db->freeResult(result);
}

bool IOMapSerialize::loadContainer(PropStream& propStream, Container* container)
{
	while (container->serializationCount > 0) {
		if (!loadItem(propStream, container)) {
			std::cout << "WARNING: Unserialization error for containing item in IOMapSerialize::loadContainer() - " << container->getID() << std::endl;
			return false;
		}

		container->serializationCount--;
	}

	uint8_t endAttr = 0;
	propStream.GET_UCHAR(endAttr);

	if (endAttr != 0x00) {
		std::cout << "WARNING: Unserialization error for containing item in IOMapSerialize::loadContainer() - " << container->getID() << std::endl;
		return false;
	}

	return true;
}

bool IOMapSerialize::loadItem(PropStream& propStream, Cylinder* parent)
{
	Item* item = NULL;

	uint16_t id = 0;
	propStream.GET_USHORT(id);

	const ItemType& iType = Item::items[id];
	Tile* tile = NULL;

	if (parent->getParent() == NULL) {
		tile = parent->getTile();
	}

	if (iType.moveable || !tile) {
		//create a new item
		item = Item::CreateItem(id);

		if (item) {
			if (item->unserializeAttr(propStream)) {
				Container* container = item->getContainer();

				if (container && !loadContainer(propStream, container)) {
					delete item;
					return false;
				}

				parent->__internalAddThing(item);
				item->__startDecaying();
			} else {
				std::cout << "WARNING: Unserialization error in IOMapSerialize::loadItem()" << id << std::endl;
				delete item;
				return false;
			}
		}
	} else {
		// Stationary items like doors/beds/blackboards/bookcases
		if (const TileItemVector* items = tile->getItemList()) {
			for (ItemVector::const_iterator it = items->begin(), end = items->end(); it != end; ++it) {
				Item* findItem = (*it);

				if (findItem->getID() == id) {
					item = findItem;
					break;
				} else if (iType.isDoor() && findItem->getDoor()) {
					item = findItem;
					break;
				} else if (iType.isBed() && findItem->getBed()) {
					item = findItem;
					break;
				}
			}
		}

		if (item) {
			if (item->unserializeAttr(propStream)) {
				Container* container = item->getContainer();

				if (container && !loadContainer(propStream, container)) {
					return false;
				}

				item = g_game.transformItem(item, id);
			} else {
				std::cout << "WARNING: Unserialization error in IOMapSerialize::loadItem()" << id << std::endl;
			}
		} else {
			//The map changed since the last save, just read the attributes
			Item* dummy = Item::CreateItem(id);

			if (dummy) {
				dummy->unserializeAttr(propStream);
				Container* container = dummy->getContainer();

				if (container && !loadContainer(propStream, container)) {
					delete dummy;
					return false;
				}

				delete dummy;
			}
		}
	}

	return true;
}

bool IOMapSerialize::saveMapBinary(Map* map)
{
	Database* db = Database::getInstance();
	std::ostringstream query;

	//Start the transaction
	DBTransaction transaction(db);

	if (!transaction.begin()) {
		return false;
	}

	if (!db->executeQuery("DELETE FROM `map_store`")) {
		return false;
	}

	DBInsert stmt(db);
	stmt.setQuery("INSERT INTO `map_store` (`house_id`, `data`) VALUES ");

	//clear old tile data
	for (HouseMap::iterator it = Houses::getInstance().getHouseBegin();
	        it != Houses::getInstance().getHouseEnd();
	        ++it) {
		//save house items
		House* house = it->second;
		PropWriteStream stream;

		for (HouseTileList::iterator tile_iter = house->getHouseTileBegin(); tile_iter != house->getHouseTileEnd(); ++tile_iter) {
			saveTile(stream, *tile_iter);
		}

		uint32_t attributesSize;
		const char* attributes = stream.getStream(attributesSize);

		query << it->second->getHouseId() << "," << db->escapeBlob(attributes, attributesSize);

		if (!stmt.addRow(query)) {
			return false;
		}
	}

	if (!stmt.execute()) {
		return false;
	}

	//End the transaction
	return transaction.commit();
}

bool IOMapSerialize::saveMapBinaryTileBased(Map* map)
{
	Database* db = Database::getInstance();
	std::ostringstream query;

	//Start the transaction
	DBTransaction transaction(db);

	if (!transaction.begin()) {
		return false;
	}

	if (!db->executeQuery("DELETE FROM `tile_store`")) {
		return false;
	}

	DBInsert stmt(db);
	stmt.setQuery("INSERT INTO `tile_store` (`house_id`, `data`) VALUES ");

	//clear old tile data
	for (HouseMap::iterator it = Houses::getInstance().getHouseBegin();
	        it != Houses::getInstance().getHouseEnd();
	        ++it) {
		//save house items
		House* house = it->second;

		for (HouseTileList::iterator tile_iter = house->getHouseTileBegin(); tile_iter != house->getHouseTileEnd(); ++tile_iter) {
			PropWriteStream stream;
			saveTile(stream, *tile_iter);
			uint32_t attributesSize;
			const char* attributes = stream.getStream(attributesSize);

			if (attributesSize > 0) {
				query << it->second->getHouseId() << "," << db->escapeBlob(attributes, attributesSize);

				if (!stmt.addRow(query)) {
					return false;
				}
			}
		}
	}

	if (!stmt.execute()) {
		return false;
	}

	//End the transaction
	return transaction.commit();
}

void IOMapSerialize::saveItem(PropWriteStream& stream, const Item* item)
{
	const Container* container = item->getContainer();

	// Write ID & props
	stream.ADD_USHORT(item->getID());
	item->serializeAttr(stream);

	if (container) {
		// Hack our way into the attributes
		stream.ADD_UCHAR(ATTR_CONTAINER_ITEMS);
		stream.ADD_ULONG(container->size());

		for (ItemDeque::const_reverse_iterator i = container->getReversedItems(); i != container->getReversedEnd(); ++i) {
			saveItem(stream, *i);
		}
	}

	stream.ADD_UCHAR(0x00); // attr end
}

void IOMapSerialize::saveTile(PropWriteStream& stream, const Tile* tile)
{
	const Position& tilePosition = tile->getPosition();

	if (tilePosition.x == 0 || tilePosition.y == 0) {
		return;
	}

	std::vector<Item*> items;

	if (const TileItemVector* tileItems = tile->getItemList()) {
		for (ItemVector::const_reverse_iterator it = tileItems->rbegin(), rend = tileItems->rend(); it != rend; ++it) {
			Item* item = (*it);

			// Note that these are NEGATED, ie. these are the items that will be saved.
			if (!(!item->isNotMoveable() || item->getDoor() || (item->getContainer() &&
			        item->getContainer()->size() != 0) || item->canWriteText() || item->getBed())) {
				continue;
			}

			items.push_back(item);
		}
	}

	if (!items.empty()) {
		stream.ADD_USHORT(tilePosition.x);
		stream.ADD_USHORT(tilePosition.y);
		stream.ADD_UCHAR(tilePosition.z);

		stream.ADD_ULONG(items.size());

		for (std::vector<Item*>::iterator iter = items.begin(), end = items.end(); iter != end; ++iter) {
			saveItem(stream, *iter);
		}
	}
}

bool IOMapSerialize::loadHouseInfo(Map* map)
{
	Database* db = Database::getInstance();
	std::ostringstream query;

	DBResult* result = db->storeQuery("SELECT `id`, `owner`, `paid`, `warnings` FROM `houses`");

	if (!result) {
		return false;
	}

	do {
		House* house = Houses::getInstance().getHouse(result->getDataInt("id"));

		if (house) {
			house->setHouseOwner(result->getDataInt("owner"));
			house->setPaidUntil(result->getDataInt("paid"));
			house->setPayRentWarnings(result->getDataInt("warnings"));
		}
	} while (result->next());

	db->freeResult(result);

	for (HouseMap::iterator it = Houses::getInstance().getHouseBegin(); it != Houses::getInstance().getHouseEnd(); ++it) {
		House* house = it->second;

		if (house->getHouseOwner() != 0 && house->getHouseId() != 0) {
			query.str("");
			query << "SELECT `listid`, `list` FROM `house_lists` WHERE `house_id` = " << house->getHouseId();

			if ((result = db->storeQuery(query.str()))) {
				do {
					house->setAccessList(result->getDataInt("listid"), result->getDataString("list"));
				} while (result->next());

				db->freeResult(result);
			}
		}
	}

	return true;
}

bool IOMapSerialize::saveHouseInfo(Map* map)
{
	Database* db = Database::getInstance();
	std::ostringstream query;

	DBTransaction transaction(db);

	if (!transaction.begin()) {
		return false;
	}

	if (!db->executeQuery("DELETE FROM `house_lists`")) {
		return false;
	}

	for (HouseMap::iterator it = Houses::getInstance().getHouseBegin(); it != Houses::getInstance().getHouseEnd(); ++it) {
		House* house = it->second;
		query << "SELECT `id` FROM `houses` WHERE `id` = " << house->getHouseId();
		DBResult* result = db->storeQuery(query.str());
		query.str("");

		if (result) {
			db->freeResult(result);
			query << "UPDATE `houses` SET `owner` = " << house->getHouseOwner() << ", `paid` = " << house->getPaidUntil() << ", `warnings` = " << house->getPayRentWarnings() << ", `name` = " << db->escapeString(house->getName()) << ", `town_id` = " << house->getTownId() << ", `rent` = " << house->getRent() << ", `size` = " << house->getHouseTileSize() << ", `beds` = " << house->getBedCount() << " WHERE `id` = " << house->getHouseId();
		} else {
			query << "INSERT INTO `houses` (`id`, `owner`, `paid`, `warnings`, `name`, `town_id`, `rent`, `size`, `beds`) VALUES (" << house->getHouseId() << "," << house->getHouseOwner() << "," << house->getPaidUntil() << "," << house->getPayRentWarnings() << "," << db->escapeString(house->getName()) << "," << house->getTownId() << "," << house->getRent() << "," << house->getHouseTileSize() << "," << house->getBedCount() << ")";
		}

		db->executeQuery(query.str());
		query.str("");
	}

	DBInsert stmt(db);
	stmt.setQuery("INSERT INTO `house_lists` (`house_id` , `listid` , `list`) VALUES ");

	for (HouseMap::iterator it = Houses::getInstance().getHouseBegin(); it != Houses::getInstance().getHouseEnd(); ++it) {
		House* house = it->second;

		std::string listText;

		if (house->getAccessList(GUEST_LIST, listText) && listText != "") {
			query << house->getHouseId() << "," << GUEST_LIST << "," << db->escapeString(listText);

			if (!stmt.addRow(query)) {
				return false;
			}
		}

		if (house->getAccessList(SUBOWNER_LIST, listText) && listText != "") {
			query << house->getHouseId() << "," << SUBOWNER_LIST << "," << db->escapeString(listText);

			if (!stmt.addRow(query)) {
				return false;
			}
		}

		for (HouseDoorList::iterator it = house->getHouseDoorBegin(); it != house->getHouseDoorEnd(); ++it) {
			const Door* door = *it;

			if (door->getAccessList(listText) && listText != "") {
				query << house->getHouseId() << "," << door->getDoorId() << "," << db->escapeString(listText);

				if (!stmt.addRow(query)) {
					return false;
				}
			}
		}
	}

	if (!stmt.execute()) {
		return false;
	}

	return transaction.commit();
}
