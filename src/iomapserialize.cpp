// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "iomapserialize.h"

#include "bed.h"
#include "game.h"
#include "housetile.h"

extern Game g_game;

namespace {

bool loadContainer(const char*& first, const char* last, Container* container);

bool loadItem(const char*& first, const char* last, Cylinder* parent)
{
	auto id = OTB::read<uint16_t>(first, last);

	Tile* tile = nullptr;
	if (!parent->hasParent()) {
		tile = parent->getTile();
	}

	const ItemType& iType = Item::items[id];
	if (iType.moveable || iType.forceSerialize || !tile) {
		// create a new item
		if (Item* item = Item::CreateItem(id)) {
			try {
				item->unserializeAttr(first, last);
				if (Container* container = item->getContainer()) {
					loadContainer(first, last, container);
				}
			} catch (const std::invalid_argument& e) {
				delete item;
				throw e;
			}

			parent->internalAddThing(item);
			item->startDecaying();
		}
	} else {
		// Stationary items like doors/beds/blackboards/bookcases
		Item* item = nullptr;
		if (const TileItemVector* items = tile->getItemList()) {
			for (Item* findItem : *items) {
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
			item->unserializeAttr(first, last);

			if (Container* container = item->getContainer()) {
				loadContainer(first, last, container);
			}

			g_game.transformItem(item, id);
		} else {
			// The map changed since the last save, just read the attributes
			if (Item* dummy = Item::CreateItem(id)) {
				dummy->unserializeAttr(first, last);
				if (Container* container = dummy->getContainer()) {
					loadContainer(first, last, container);
				} else if (BedItem* bedItem = dynamic_cast<BedItem*>(dummy)) {
					uint32_t sleeperGUID = bedItem->getSleeper();
					if (sleeperGUID != 0) {
						g_game.removeBedSleeper(sleeperGUID);
					}
				}
			}
		}
	}
	return true;
}

bool loadContainer(const char*& first, const char* last, Container* container)
{
	while (container->serializationCount > 0) {
		loadItem(first, last, container);
		container->serializationCount--;
	}

	auto endAttr = OTB::read<uint8_t>(first, last);
	if (endAttr != 0) {
		std::cout << "[Warning - IOMapSerialize::loadContainer] Unserialization error for container item: "
		          << container->getID() << std::endl;
		return false;
	}
	return true;
}

} // namespace

void IOMapSerialize::loadHouseItems(Map* map)
{
	int64_t start = OTSYS_TIME();

	DBResult_ptr result = Database::getInstance().storeQuery("SELECT `data` FROM `tile_store`");
	if (!result) {
		return;
	}

	do {
		auto attr = result->getString("data");
		auto first = attr.begin(), last = attr.end();

		try {
			auto x = OTB::read<uint16_t>(first, last);
			auto y = OTB::read<uint16_t>(first, last);
			auto z = OTB::read<uint8_t>(first, last);

			Tile* tile = map->getTile(x, y, z);
			if (!tile) {
				continue;
			}

			auto item_count = OTB::read<uint32_t>(first, last);

			while (item_count--) {
				loadItem(first, last, tile);
			}
		} catch (const std::exception&) {
			continue;
		}
	} while (result->next());
	std::cout << "> Loaded house items in: " << (OTSYS_TIME() - start) / (1000.) << " s" << std::endl;
}

bool IOMapSerialize::saveHouseItems()
{
	int64_t start = OTSYS_TIME();
	Database& db = Database::getInstance();

	// Start the transaction
	DBTransaction transaction;
	if (!transaction.begin()) {
		return false;
	}

	// clear old tile data
	if (!db.executeQuery("DELETE FROM `tile_store`")) {
		return false;
	}

	DBInsert stmt("INSERT INTO `tile_store` (`house_id`, `data`) VALUES ");

	PropWriteStream stream;
	for (const auto& it : g_game.map.houses.getHouses()) {
		// save house items
		House* house = it.second;
		for (HouseTile* tile : house->getTiles()) {
			saveTile(stream, tile);

			if (auto attributes = stream.getStream(); !attributes.empty()) {
				if (!stmt.addRow(fmt::format("{:d}, {:s}", house->getId(), db.escapeString(attributes)))) {
					return false;
				}
				stream.clear();
			}
		}
	}

	if (!stmt.execute()) {
		return false;
	}

	// End the transaction
	bool success = transaction.commit();
	std::cout << "> Saved house items in: " << (OTSYS_TIME() - start) / (1000.) << " s" << std::endl;
	return success;
}

void IOMapSerialize::saveItem(PropWriteStream& stream, const Item* item)
{
	const Container* container = item->getContainer();

	// Write ID & props
	stream.write<uint16_t>(item->getID());
	item->serializeAttr(stream);

	if (container) {
		// Hack our way into the attributes
		stream.write<uint8_t>(ATTR_CONTAINER_ITEMS);
		stream.write<uint32_t>(container->size());
		for (auto it = container->getReversedItems(), end = container->getReversedEnd(); it != end; ++it) {
			saveItem(stream, *it);
		}
	}

	stream.write<uint8_t>(0x00); // attr end
}

void IOMapSerialize::saveTile(PropWriteStream& stream, const Tile* tile)
{
	const TileItemVector* tileItems = tile->getItemList();
	if (!tileItems) {
		return;
	}

	std::forward_list<Item*> items;
	uint16_t count = 0;
	for (Item* item : *tileItems) {
		const ItemType& it = Item::items[item->getID()];

		// Note that these are NEGATED, ie. these are the items that will be saved.
		if (!(it.moveable || it.forceSerialize || item->getDoor() ||
		      (item->getContainer() && !item->getContainer()->empty()) || it.canWriteText || item->getBed())) {
			continue;
		}

		items.push_front(item);
		++count;
	}

	if (!items.empty()) {
		const Position& tilePosition = tile->getPosition();
		stream.write<uint16_t>(tilePosition.x);
		stream.write<uint16_t>(tilePosition.y);
		stream.write<uint8_t>(tilePosition.z);

		stream.write<uint32_t>(count);
		for (const Item* item : items) {
			saveItem(stream, item);
		}
	}
}

bool IOMapSerialize::loadHouseInfo()
{
	Database& db = Database::getInstance();

	DBResult_ptr result = db.storeQuery("SELECT `id`, `owner`, `paid`, `warnings` FROM `houses`");
	if (!result) {
		return false;
	}

	do {
		House* house = g_game.map.houses.getHouse(result->getNumber<uint32_t>("id"));
		if (house) {
			house->setOwner(result->getNumber<uint32_t>("owner"), false);
			house->setPaidUntil(result->getNumber<time_t>("paid"));
			house->setPayRentWarnings(result->getNumber<uint32_t>("warnings"));
		}
	} while (result->next());

	result = db.storeQuery("SELECT `house_id`, `listid`, `list` FROM `house_lists`");
	if (result) {
		do {
			House* house = g_game.map.houses.getHouse(result->getNumber<uint32_t>("house_id"));
			if (house) {
				house->setAccessList(result->getNumber<uint32_t>("listid"), result->getString("list"));
			}
		} while (result->next());
	}
	return true;
}

bool IOMapSerialize::saveHouseInfo()
{
	Database& db = Database::getInstance();

	DBTransaction transaction;
	if (!transaction.begin()) {
		return false;
	}

	if (!db.executeQuery("DELETE FROM `house_lists`")) {
		return false;
	}

	for (const auto& it : g_game.map.houses.getHouses()) {
		House* house = it.second;
		DBResult_ptr result = db.storeQuery(fmt::format("SELECT `id` FROM `houses` WHERE `id` = {:d}", house->getId()));
		if (result) {
			db.executeQuery(fmt::format(
			    "UPDATE `houses` SET `owner` = {:d}, `paid` = {:d}, `warnings` = {:d}, `name` = {:s}, `town_id` = {:d}, `rent` = {:d}, `size` = {:d}, `beds` = {:d} WHERE `id` = {:d}",
			    house->getOwner(), house->getPaidUntil(), house->getPayRentWarnings(),
			    db.escapeString(house->getName()), house->getTownId(), house->getRent(), house->getTiles().size(),
			    house->getBedCount(), house->getId()));
		} else {
			db.executeQuery(fmt::format(
			    "INSERT INTO `houses` (`id`, `owner`, `paid`, `warnings`, `name`, `town_id`, `rent`, `size`, `beds`) VALUES ({:d}, {:d}, {:d}, {:d}, {:s}, {:d}, {:d}, {:d}, {:d})",
			    house->getId(), house->getOwner(), house->getPaidUntil(), house->getPayRentWarnings(),
			    db.escapeString(house->getName()), house->getTownId(), house->getRent(), house->getTiles().size(),
			    house->getBedCount()));
		}
	}

	DBInsert stmt("INSERT INTO `house_lists` (`house_id` , `listid` , `list`) VALUES ");

	for (const auto& it : g_game.map.houses.getHouses()) {
		House* house = it.second;

		std::string listText;
		if (house->getAccessList(GUEST_LIST, listText) && !listText.empty()) {
			if (!stmt.addRow(fmt::format("{:d}, {:d}, {:s}", house->getId(), tfs::to_underlying(GUEST_LIST),
			                             db.escapeString(listText)))) {
				return false;
			}

			listText.clear();
		}

		if (house->getAccessList(SUBOWNER_LIST, listText) && !listText.empty()) {
			if (!stmt.addRow(fmt::format("{:d}, {:d}, {:s}", house->getId(), tfs::to_underlying(SUBOWNER_LIST),
			                             db.escapeString(listText)))) {
				return false;
			}

			listText.clear();
		}

		for (Door* door : house->getDoors()) {
			if (door->getAccessList(listText) && !listText.empty()) {
				if (!stmt.addRow(fmt::format("{:d}, {:d}, {:s}", house->getId(), door->getDoorId(),
				                             db.escapeString(listText)))) {
					return false;
				}

				listText.clear();
			}
		}
	}

	if (!stmt.execute()) {
		return false;
	}

	return transaction.commit();
}

bool IOMapSerialize::saveHouse(House* house)
{
	Database& db = Database::getInstance();

	// Start the transaction
	DBTransaction transaction;
	if (!transaction.begin()) {
		return false;
	}

	uint32_t houseId = house->getId();

	// clear old tile data
	if (!db.executeQuery(fmt::format("DELETE FROM `tile_store` WHERE `house_id` = {:d}", houseId))) {
		return false;
	}

	DBInsert stmt("INSERT INTO `tile_store` (`house_id`, `data`) VALUES ");

	PropWriteStream stream;
	for (HouseTile* tile : house->getTiles()) {
		saveTile(stream, tile);

		if (auto attributes = stream.getStream(); attributes.size() > 0) {
			if (!stmt.addRow(fmt::format("{:d}, {:s}", houseId, db.escapeString(attributes)))) {
				return false;
			}
			stream.clear();
		}
	}

	if (!stmt.execute()) {
		return false;
	}

	// End the transaction
	return transaction.commit();
}
