// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "ioinbox.h"

#include "game.h"
#include "tasks.h"

extern Game g_game;

IOInbox::IOInbox() { db.connect(); }

void IOInbox::loadPlayer(const Player* player)
{
	std::unique_lock<std::recursive_mutex> lockClass(lock);
	auto result = pendingPlayerSet.insert(player->getGUID());
	if (result.second) {
		g_dispatcherInbox.addTask([this, guid = player->getGUID()]() { loadPlayerAsync(guid); });
	}
}

void IOInbox::savePlayer(const Player* player)
{
	if (Inbox* inbox = player->getInbox()) {
		saveInbox(player->getGUID(), inbox, player);
		g_dispatcherInbox.addTask([this, guid = player->getGUID()]() { savePlayerAsync(guid); });
	}
}

void IOInbox::savePlayerItems(const Player* player, ItemBlockList& itemList)
{
	savePlayerItems(player->getGUID(), itemList);
}

void IOInbox::savePlayerItems(const uint32_t& guid, ItemBlockList& itemList)
{
	// dispatcher thread
	std::unique_lock<std::recursive_mutex> lockClass(lock);
	pendingItemsToSave[guid].push_back(std::move(itemList));

	auto it = pendingPlayerSet.find(guid);
	if (it == pendingPlayerSet.end()) {
		g_dispatcherInbox.addTask([this, guid]() { savePlayerItemsAsync(guid); });
	}
}

Inbox* IOInbox::loadInbox(const uint32_t& guid)
{
	// dispatcherInbox thread

	DBEntryListPtr tmpInbox = getPlayerInbox(guid);
	if (!tmpInbox) {
		tmpInbox = loadPlayerInbox(guid);
	}

	ItemMap itemMap;
	for (auto& itemDBEntry : *tmpInbox) {
		PropStream propStream;
		propStream.init(itemDBEntry.attributes.data(), itemDBEntry.attributes.size());

		Item* item = Item::CreateItem(itemDBEntry.itemtype, itemDBEntry.count);
		if (item) {
			if (!item->unserializeAttr(propStream)) {
				std::cout << "WARNING: Serialize error in IOInbox::loadInbox" << std::endl;
			}

			std::pair<Item*, uint32_t> pair(item, itemDBEntry.pid);
			itemMap[itemDBEntry.sid] = pair;
		}
	}

	return createInboxItem(itemMap);
}

void IOInbox::saveInbox(const uint32_t& guid, Inbox* inbox, const Player* player /* = nullptr */)
{
	// dispatcher thread
	ItemBlockList itemList;
	for (Item* item : inbox->getItemList()) {
		itemList.emplace_back(0, item);
	}
	DBEntryListPtr inboxPtr = saveItems(player, itemList);
	addPlayerInbox(guid, inboxPtr);
}

Inbox* IOInbox::createInboxItem(const ItemMap& items)
{
	Inbox* inbox = new Inbox(ITEM_INBOX);
	inbox->incrementReferenceCounter();

	for (ItemMap::const_reverse_iterator it = items.rbegin(), end = items.rend(); it != end; ++it) {
		const std::pair<Item*, int32_t>& pair = it->second;

		Item* item = pair.first;
		int32_t pid = pair.second;
		if (pid >= 0 && pid < 100) {
			inbox->internalAddThing(item);
		} else {
			ItemMap::const_iterator it2 = items.find(pid);
			if (it2 == items.end()) {
				continue;
			}

			if (Container* container = it2->second.first->getContainer()) {
				container->internalAddThing(item);
			}
		}
	}

	return inbox;
}

bool IOInbox::canSavePlayerItems(const uint32_t& guid)
{
	// dispatcherInbox thread
	std::unique_lock<std::recursive_mutex> lockClass(lock);

	auto playerIt = pendingPlayerSet.find(guid);
	if (playerIt != pendingPlayerSet.end()) {
		return false;
	}

	auto itemIt = pendingItemsToSave.find(guid);
	return itemIt != pendingItemsToSave.end();
}

DBEntryListPtr IOInbox::getPlayerInbox(const uint32_t& guid)
{
	// dispatcherInbox thread
	std::unique_lock<std::recursive_mutex> lockClass(lock);

	auto it = inboxCache.find(guid);
	if (it == inboxCache.end()) {
		return nullptr;
	}
	return it->second->items;
}

void IOInbox::addPlayerInbox(const uint32_t& guid, DBEntryListPtr inbox)
{
	// dispatcher thread
	std::unique_lock<std::recursive_mutex> lockClass(lock);

	inboxCache[guid] = std::make_shared<PlayerDBEntry>(PlayerDBEntry{false, inbox});
}

DBEntryListPtr IOInbox::loadPlayerInbox(const uint32_t& guid)
{
	auto inbox = std::make_shared<DBEntryList>();

	DBResult_ptr result = db.storeQuery(fmt::format(
	    "SELECT `pid`, `sid`, `itemtype`, `count`, `attributes` FROM `player_inboxitems` WHERE `player_id` = {:d} ORDER BY `sid` DESC",
	    guid));
	if (result) {
		do {
			DBEntry entry;
			entry.pid = result->getNumber<int32_t>("pid");
			entry.sid = result->getNumber<int32_t>("sid");
			entry.itemtype = result->getNumber<uint16_t>("itemtype");
			entry.count = result->getNumber<uint16_t>("count");
			entry.attributes = result->getString("attributes");
			inbox->push_back(entry);
		} while (result->next());
	}

	return inbox;
}

DBEntryListPtr IOInbox::saveItems(const Player* player, const ItemBlockList& itemList)
{
	DBEntryListPtr list = std::make_shared<DBEntryList>();
	using ContainerBlock = std::pair<Container*, int32_t>;
	std::vector<ContainerBlock> containers;
	containers.reserve(32);

	int32_t runningId = 100;
	std::map<uint8_t, OpenContainer> openContainers;
	if (player) {
		openContainers = player->getOpenContainers();
	}

	PropWriteStream propWriteStream;
	for (const auto& it : itemList) {
		int32_t pid = it.first;
		Item* item = it.second;
		++runningId;

		if (Container* container = item->getContainer()) {
			if (player) {
				if (container->getIntAttr(ITEM_ATTRIBUTE_OPENCONTAINER)) {
					container->setIntAttr(ITEM_ATTRIBUTE_OPENCONTAINER, 0);
				}

				if (!openContainers.empty()) {
					for (const auto& its : openContainers) {
						auto openContainer = its.second;
						auto opcontainer = openContainer.container;

						if (opcontainer == container) {
							container->setIntAttr(ITEM_ATTRIBUTE_OPENCONTAINER, static_cast<int64_t>(its.first) + 1);
							break;
						}
					}
				}
			}

			containers.emplace_back(container, runningId);
		}

		propWriteStream.clear();
		item->serializeAttr(propWriteStream);

		list->emplace_back(
		    DBEntry{pid, runningId, item->getID(), item->getSubType(), std::string(propWriteStream.getStream())});
	}

	for (size_t i = 0; i < containers.size(); i++) {
		const ContainerBlock& cb = containers[i];
		Container* container = cb.first;
		int32_t parentId = cb.second;

		for (Item* item : container->getItemList()) {
			++runningId;

			Container* subContainer = item->getContainer();
			if (subContainer) {
				containers.emplace_back(subContainer, runningId);

				if (player) {
					if (subContainer->getIntAttr(ITEM_ATTRIBUTE_OPENCONTAINER)) {
						subContainer->setIntAttr(ITEM_ATTRIBUTE_OPENCONTAINER, 0);
					}

					if (!openContainers.empty()) {
						for (const auto& it : openContainers) {
							auto openContainer = it.second;
							auto opcontainer = openContainer.container;

							if (opcontainer == subContainer) {
								subContainer->setIntAttr(ITEM_ATTRIBUTE_OPENCONTAINER, it.first + 1);
								break;
							}
						}
					}
				}
			}

			propWriteStream.clear();
			item->serializeAttr(propWriteStream);

			list->emplace_back(DBEntry{parentId, runningId, item->getID(), item->getSubType(),
			                           std::string(propWriteStream.getStream())});
		}
	}
	return list;
}

void IOInbox::loadPlayerAsync(const uint32_t& guid)
{
	// dispatcherInbox thread
	Inbox* inbox = loadInbox(guid);
	if (deliverItems(guid, inbox)) {
		saveInbox(guid, inbox);
		savePlayerAsync(guid);
	}
	g_dispatcher.addTask([this, guid, inbox]() { assignInbox(guid, inbox); });
}

void IOInbox::savePlayerAsync(const uint32_t& guid)
{
	// dispatcherInbox thread
	std::unique_lock<std::recursive_mutex> lockClass(lock);
	auto it = inboxCache.find(guid);
	if (it == inboxCache.end() || it->second->saved) {
		return;
	}
	PlayerDBEntryPtr playerDBEntry = it->second;
	lockClass.unlock();

	// disable foreign key checks to prevent locking of the players table during insert
	// we do not except that the player is deleted while saving the inbox
	db.executeQuery("SET foreign_key_checks = 0;");

	std::ostringstream query;
	DBTransaction transaction(db);
	if (transaction.begin()) {
		if (db.executeQuery(fmt::format("DELETE FROM `player_inboxitems` WHERE `player_id` = {:d}", guid))) {
			DBInsert query_insert(
			    "INSERT INTO `player_inboxitems` (`player_id`, `pid`, `sid`, `itemtype`, `count`, `attributes`) VALUES ",
			    db);
			for (auto& itemEntry : *playerDBEntry->items) {
				if (!query_insert.addRow(fmt::format("{:d}, {:d}, {:d}, {:d}, {:d}, {:s}", guid, itemEntry.pid,
				                                     itemEntry.sid, itemEntry.itemtype, itemEntry.count,
				                                     db.escapeString(itemEntry.attributes)))) {
					return;
				}
			}
			if (!query_insert.execute()) {
				return;
			}
		}
		if (transaction.commit()) {
			playerDBEntry->saved = true;
		}
	}
}

void IOInbox::assignInbox(uint32_t guid, Inbox* inbox)
{
	// dispatcher thread
	std::unique_lock<std::recursive_mutex> lockClass(lock);
	pendingPlayerSet.erase(guid);
	lockClass.unlock();

	Player* player = g_game.getPlayerByGUID(guid);
	if (!player) {
		g_dispatcherInbox.addTask([inbox]() { delete inbox; });
		return;
	}
	deliverItems(guid, inbox);
	player->setInbox(inbox);
}

bool IOInbox::deliverItems(uint32_t guid, Inbox* inbox)
{
	// any thread
	std::unique_lock<std::recursive_mutex> lockClass(lock);
	auto it = pendingItemsToSave.find(guid);
	if (it != pendingItemsToSave.end()) {
		for (auto& itemList : it->second) {
			for (auto& itemBlock : itemList) {
				inbox->internalAddThing(itemBlock.second);
			}
		}
		pendingItemsToSave.erase(it);
		return true;
	}
	return false;
}

void IOInbox::savePlayerItemsAsync(uint32_t guid)
{
	// dispatcherInbox thread
	if (canSavePlayerItems(guid)) {
		Inbox* inbox = loadInbox(guid);
		deliverItems(guid, inbox);
		saveInbox(guid, inbox);
		savePlayerAsync(guid);
		delete inbox;
	}
}

void IOInbox::flush()
{
	// dispatcherInbox thread - only called as ultimate task on shutdown
	std::lock_guard<std::recursive_mutex> lockClass(lock);

	pendingPlayerSet.clear();

	while (!pendingItemsToSave.empty()) {
		auto it = pendingItemsToSave.begin();
		savePlayerItemsAsync(it->first);
	}
}
