// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "iomapserialize.h"

#include "bed.h"
#include "database.h"
#include "game.h"
#include "house.h"
#include "housetile.h"

extern Game g_game;

namespace {

void loadContainer(OTB::iterator& first, const OTB::iterator& last, const std::shared_ptr<Container>& container);

void loadItem(OTB::iterator& first, const OTB::iterator& last, const std::shared_ptr<Thing>& parent)
{
	auto id = OTB::read<uint16_t>(first, last);

	std::shared_ptr<Tile> tile = nullptr;
	if (!parent->hasParent()) {
		tile = parent->getTile();
	}

	const ItemType& iType = Item::items[id];
	if (iType.moveable || iType.forceSerialize || !tile) {
		// create a new item
		if (const auto& item = Item::CreateItem(id)) {
			item->unserializeAttr(first, last);
			if (const auto& container = item->getContainer()) {
				loadContainer(first, last, container);
			}

			parent->internalAddThing(item);
			item->startDecaying();
		}
	} else {
		// Stationary items like doors/beds/blackboards/bookcases
		std::shared_ptr<Item> item = nullptr;
		if (const TileItemVector* items = tile->getItemList()) {
			for (const auto& findItem : *items) {
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

			if (const auto& container = item->getContainer()) {
				loadContainer(first, last, container);
			}

			g_game.transformItem(item, id);
		} else {
			// The map changed since the last save, just read the attributes
			if (const auto& dummy = Item::CreateItem(id)) {
				dummy->unserializeAttr(first, last);
				if (const auto& container = dummy->getContainer()) {
					loadContainer(first, last, container);
				} else if (const auto& bedItem = dummy->getBed()) {
					if (uint32_t sleeperGUID = bedItem->getSleeper(); sleeperGUID != 0) {
						g_game.removeBedSleeper(sleeperGUID);
					}
				}
			}
		}
	}
}

void loadContainer(OTB::iterator& first, const OTB::iterator& last, const std::shared_ptr<Container>& container)
{
	while (container->serializationCount > 0) {
		loadItem(first, last, container);
		container->serializationCount--;
	}

	const auto endAttr = OTB::read<uint8_t>(first, last);
	if (endAttr != 0) {
		throw std::invalid_argument("Unserialization error for container item");
	}
}

} // namespace

void IOMapSerialize::loadHouseItems(Map* map)
{
	int64_t start = OTSYS_TIME();

	const auto& result = Database::getInstance().storeQuery("SELECT `data` FROM `tile_store`");
	if (!result) {
		return;
	}

	do {
		auto attr = result->getString("data");
		auto first = attr.data();
		auto last = first + attr.size();

		try {
			auto x = OTB::read<uint16_t>(first, last);
			auto y = OTB::read<uint16_t>(first, last);
			auto z = OTB::read<uint8_t>(first, last);

			const auto& tile = map->getTile(x, y, z);
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
	for (auto&& house : g_game.getHouses() | std::views::values | std::views::as_const) {
		// save house items
		for (auto&& tile : house->getTiles() | tfs::views::lock_weak_ptrs | std::views::as_const) {
			saveTile(stream, tile);

			if (auto attributes = stream.getStream(); !attributes.empty()) {
				if (!stmt.addRow(std::format("{:d}, {:s}", house->getId(), db.escapeString(attributes)))) {
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

void IOMapSerialize::saveItem(PropWriteStream& stream, const std::shared_ptr<const Item>& item)
{
	const auto& container = item->getContainer();

	// Write ID & props
	stream.write<uint16_t>(item->getID());
	item->serializeAttr(stream);

	if (container) {
		// Hack our way into the attributes
		stream.write<uint8_t>(ATTR_CONTAINER_ITEMS);
		stream.write<uint32_t>(container->size());
		for (auto&& containerItem : container->getItemList() | std::views::reverse) {
			saveItem(stream, containerItem);
		}
	}

	stream.write<uint8_t>(ATTR_END); // attr end
}

void IOMapSerialize::saveTile(PropWriteStream& stream, const std::shared_ptr<const Tile>& tile)
{
	const TileItemVector* tileItems = tile->getItemList();
	if (!tileItems) {
		return;
	}

	std::forward_list<std::shared_ptr<const Item>> items;
	uint16_t count = 0;
	for (const auto& item : *tileItems) {
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
		for (const auto& item : items) {
			saveItem(stream, item);
		}
	}
}

bool IOMapSerialize::loadHouseInfo()
{
	Database& db = Database::getInstance();

	const auto& result = db.storeQuery("SELECT `id`, `owner`, `paid`, `warnings` FROM `houses`");
	if (!result) {
		return false;
	}

	do {
		if (const auto& house = g_game.getHouseById(result->getNumber<uint32_t>("id"))) {
			house->setOwner(result->getNumber<uint32_t>("owner"), false);
			house->setPaidUntil(result->getNumber<time_t>("paid"));
			house->setPayRentWarnings(result->getNumber<uint32_t>("warnings"));
		}
	} while (result->next());

	if (const auto& result = db.storeQuery("SELECT `house_id`, `listid`, `list` FROM `house_lists`")) {
		do {
			if (const auto& house = g_game.getHouseById(result->getNumber<uint32_t>("house_id"))) {
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

	for (auto&& house : g_game.getHouses() | std::views::values | std::views::as_const) {
		if (const auto& result =
		        db.storeQuery(std::format("SELECT `id` FROM `houses` WHERE `id` = {:d}", house->getId()))) {
			db.executeQuery(std::format(
			    "UPDATE `houses` SET `owner` = {:d}, `paid` = {:d}, `warnings` = {:d}, `name` = {:s}, `town_id` = {:d}, `rent` = {:d}, `size` = {:d}, `beds` = {:d} WHERE `id` = {:d}",
			    house->getOwner(), house->getPaidUntil(), house->getPayRentWarnings(),
			    db.escapeString(house->getName()), house->getTownId(), house->getRent(), house->getTiles().size(),
			    house->getBedCount(), house->getId()));
		} else {
			db.executeQuery(std::format(
			    "INSERT INTO `houses` (`id`, `owner`, `paid`, `warnings`, `name`, `town_id`, `rent`, `size`, `beds`) VALUES ({:d}, {:d}, {:d}, {:d}, {:s}, {:d}, {:d}, {:d}, {:d})",
			    house->getId(), house->getOwner(), house->getPaidUntil(), house->getPayRentWarnings(),
			    db.escapeString(house->getName()), house->getTownId(), house->getRent(), house->getTiles().size(),
			    house->getBedCount()));
		}
	}

	DBInsert stmt("INSERT INTO `house_lists` (`house_id` , `listid` , `list`) VALUES ");

	for (auto&& house : g_game.getHouses() | std::views::values | std::views::as_const) {
		std::string listText;
		if (house->getAccessList(GUEST_LIST, listText) && !listText.empty()) {
			if (!stmt.addRow(std::format("{:d}, {:d}, {:s}", house->getId(), std::to_underlying(GUEST_LIST),
			                             db.escapeString(listText)))) {
				return false;
			}

			listText.clear();
		}

		if (house->getAccessList(SUBOWNER_LIST, listText) && !listText.empty()) {
			if (!stmt.addRow(std::format("{:d}, {:d}, {:s}", house->getId(), std::to_underlying(SUBOWNER_LIST),
			                             db.escapeString(listText)))) {
				return false;
			}

			listText.clear();
		}

		for (const auto& door : house->getDoors() | tfs::views::lock_weak_ptrs) {
			if (door->getAccessList(listText) && !listText.empty()) {
				if (!stmt.addRow(std::format("{:d}, {:d}, {:s}", house->getId(), door->getDoorId(),
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

bool IOMapSerialize::saveHouse(const std::shared_ptr<House>& house)
{
	Database& db = Database::getInstance();

	// Start the transaction
	DBTransaction transaction;
	if (!transaction.begin()) {
		return false;
	}

	uint32_t houseId = house->getId();

	// clear old tile data
	if (!db.executeQuery(std::format("DELETE FROM `tile_store` WHERE `house_id` = {:d}", houseId))) {
		return false;
	}

	DBInsert stmt("INSERT INTO `tile_store` (`house_id`, `data`) VALUES ");

	PropWriteStream stream;
	for (const auto& tile : house->getTiles() | tfs::views::lock_weak_ptrs) {
		saveTile(stream, tile);

		if (auto attributes = stream.getStream(); attributes.size() > 0) {
			if (!stmt.addRow(std::format("{:d}, {:s}", houseId, db.escapeString(attributes)))) {
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
