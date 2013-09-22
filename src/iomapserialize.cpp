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

	Database* db = Database::getInstance();
	std::ostringstream query;

	DBResult* result = db->storeQuery("SELECT `id` FROM `houses`");
	if (!result) {
		return true;
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
	std::cout << "> Loaded house items in: " << (OTSYS_TIME() - start) / (1000.) << " s" << std::endl;
	return true;
}

bool IOMapSerialize::saveMap(Map* map)
{
	int64_t start = OTSYS_TIME();
	Database* db = Database::getInstance();
	std::ostringstream query;

	//Start the transaction
	DBTransaction transaction;
	if (!transaction.begin()) {
		return false;
	}

	if (!db->executeQuery("DELETE FROM `tile_store`")) {
		return false;
	}

	DBInsert stmt;
	stmt.setQuery("INSERT INTO `tile_store` (`house_id`, `data`) VALUES ");

	//clear old tile data
	for (const auto& it : Houses::getInstance().getHouses()) {
		//save house items
		House* house = it.second;
		for (HouseTile* tile : house->getHouseTiles()) {
			PropWriteStream stream;
			saveTile(stream, tile);

			uint32_t attributesSize;
			const char* attributes = stream.getStream(attributesSize);
			if (attributesSize > 0) {
				query << house->getHouseId() << ',' << db->escapeBlob(attributes, attributesSize);
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
	bool success = transaction.commit();
	std::cout << "> Saved house items in: " <<
	          (OTSYS_TIME() - start) / (1000.) << " s" << std::endl;
	return success;
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
	Item* item = nullptr;

	uint16_t id = 0;
	propStream.GET_USHORT(id);

	const ItemType& iType = Item::items[id];
	Tile* tile = nullptr;

	if (parent->getParent() == nullptr) {
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
			if (!(!item->isNotMoveable() || item->getDoor() || (item->getContainer() && !item->getContainer()->empty()) || item->canWriteText() || item->getBed())) {
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
			house->setHouseOwner(result->getDataInt("owner"), false);
			house->setPaidUntil(result->getDataInt("paid"));
			house->setPayRentWarnings(result->getDataInt("warnings"));
		}
	} while (result->next());

	db->freeResult(result);

	for (const auto& it : Houses::getInstance().getHouses()) {
		House* house = it.second;
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

	DBTransaction transaction;
	if (!transaction.begin()) {
		return false;
	}

	if (!db->executeQuery("DELETE FROM `house_lists`")) {
		return false;
	}

	std::ostringstream query;
	for (const auto& it : Houses::getInstance().getHouses()) {
		House* house = it.second;
		query << "SELECT `id` FROM `houses` WHERE `id` = " << house->getHouseId();
		DBResult* result = db->storeQuery(query.str());
		if (result) {
			db->freeResult(result);
			query.str("");
			query << "UPDATE `houses` SET `owner` = " << house->getHouseOwner() << ", `paid` = " << house->getPaidUntil() << ", `warnings` = " << house->getPayRentWarnings() << ", `name` = " << db->escapeString(house->getName()) << ", `town_id` = " << house->getTownId() << ", `rent` = " << house->getRent() << ", `size` = " << house->getHouseTiles().size() << ", `beds` = " << house->getBedCount() << " WHERE `id` = " << house->getHouseId();
		} else {
			query.str("");
			query << "INSERT INTO `houses` (`id`, `owner`, `paid`, `warnings`, `name`, `town_id`, `rent`, `size`, `beds`) VALUES (" << house->getHouseId() << ',' << house->getHouseOwner() << ',' << house->getPaidUntil() << ',' << house->getPayRentWarnings() << ',' << db->escapeString(house->getName()) << ',' << house->getTownId() << ',' << house->getRent() << ',' << house->getHouseTiles().size() << ',' << house->getBedCount() << ')';
		}

		db->executeQuery(query.str());
		query.str("");
	}

	DBInsert stmt;
	stmt.setQuery("INSERT INTO `house_lists` (`house_id` , `listid` , `list`) VALUES ");

	for (const auto& it : Houses::getInstance().getHouses()) {
		House* house = it.second;

		std::string listText;

		if (house->getAccessList(GUEST_LIST, listText) && !listText.empty()) {
			query << house->getHouseId() << ',' << GUEST_LIST << ',' << db->escapeString(listText);

			if (!stmt.addRow(query)) {
				return false;
			}
		}

		if (house->getAccessList(SUBOWNER_LIST, listText) && !listText.empty()) {
			query << house->getHouseId() << ',' << SUBOWNER_LIST << ',' << db->escapeString(listText);
			if (!stmt.addRow(query)) {
				return false;
			}
		}

		for (Door* door : house->getHouseDoors()) {
			if (door->getAccessList(listText) && !listText.empty()) {
				query << house->getHouseId() << ',' << door->getDoorId() << ',' << db->escapeString(listText);
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
