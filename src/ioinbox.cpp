// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "ioinbox.h"

#include "game.h"
#include "tasks.h"

extern Game g_game;

IOInbox::IOInbox() { db.connect(); }

void IOInbox::savePlayer(Player* player)
{
	if (Inbox* inbox = player->getInbox()) {
		saveInbox(player->getGUID(), inbox, player);
		g_dispatcherInbox.addTask([this, guid = player->getGUID()]() { asyncSave(guid); });
	}
}

void IOInbox::saveInbox(uint32_t guid, Inbox* inbox, Player* player /* = nullptr */)
{
	// dispatcher thread
	ItemBlockList itemList;
	for (Item* item : inbox->getItemList()) {
		itemList.emplace_back(0, item);
	}
	DBEntryListPtr inboxPtr = saveItems(player, itemList);
	std::unique_lock<std::recursive_mutex> lock(taskLock);
	inboxCache[guid] = std::make_shared<PlayerDBEntry>(false, inboxPtr);
}

void IOInbox::loadInboxLogin(uint32_t guid)
{
	if (loading.insert(guid).second) {
		g_dispatcherInbox.addTask([this, guid]() { asyncLoad(guid); });
	}
}

Inbox* IOInbox::loadInbox(uint32_t guid)
{
	// dispatcherInbox thread
	std::unique_lock<std::recursive_mutex> lock(taskLock);

	DBEntryListPtr inboxPtr;
	auto it = inboxCache.find(guid);
	if (it != inboxCache.end()) {
		inboxPtr = it->second->items;
		lock.unlock();
	} else {
		lock.unlock();
		inboxPtr = std::make_shared<DBEntryList>();
		DBResult_ptr result;
		if ((result = db.storeQuery(fmt::format(
		         "SELECT `pid`, `sid`, `itemtype`, `count`, `attributes` FROM `player_inboxitems` WHERE `player_id` = {:d} ORDER BY `sid` DESC",
		         guid)))) {
			do {
				inboxPtr->push_back(DBEntry{result->getNumber<int32_t>("pid"), result->getNumber<int32_t>("sid"),
				                            result->getNumber<uint16_t>("itemtype"),
				                            result->getNumber<uint16_t>("count"),
				                            std::string(result->getString("attributes"))});
			} while (result->next());
		}
	}

	ItemMap itemMap;
	for (auto& itemDBEntry : *inboxPtr) {
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

	Inbox* inbox = new Inbox(ITEM_INBOX);
	inbox->incrementReferenceCounter();
	for (ItemMap::const_reverse_iterator it = itemMap.rbegin(), end = itemMap.rend(); it != end; ++it) {
		const std::pair<Item*, int32_t>& pair = it->second;
		Item* item = pair.first;
		int32_t pid = pair.second;

		if (pid >= 0 && pid < 100) {
			inbox->internalAddThing(item);
		} else {
			ItemMap::const_iterator it2 = itemMap.find(pid);

			if (it2 == itemMap.end()) {
				continue;
			}

			Container* container = it2->second.first->getContainer();
			if (container) {
				container->internalAddThing(item);
			}
		}
	}

	return inbox;
}

DBEntryListPtr IOInbox::saveItems(Player* player, const ItemBlockList& itemList)
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

void IOInbox::asyncSave(uint32_t guid)
{
	// dispatcherInbox thread
	std::unique_lock<std::recursive_mutex> lock(taskLock);
	auto it = inboxCache.find(guid);
	if (it == inboxCache.end() || it->second->saved) {
		return;
	}
	PlayerDBEntryPtr playerDBEntry = it->second;
	lock.unlock();

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

void IOInbox::asyncLoad(uint32_t guid)
{
	// dispatcherInbox thread
	Inbox* inbox = loadInbox(guid);
	if (deliverItems(guid, inbox)) {
		saveInbox(guid, inbox);
		asyncSave(guid);
	}
	g_dispatcher.addTask([this, guid, inbox]() { assignInbox(guid, inbox); });
}

void IOInbox::assignInbox(uint32_t guid, Inbox* inbox)
{
	// dispatcher thread
	std::unique_lock<std::recursive_mutex> lock(taskLock);
	loading.erase(guid);
	lock.unlock();

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
	std::unique_lock<std::recursive_mutex> lock(taskLock);
	auto it = deliverItemsMap.find(guid);
	if (it != deliverItemsMap.end()) {
		for (auto& itemList : it->second) {
			for (auto& itemBlock : itemList) {
				inbox->internalAddThing(itemBlock.second);
			}
		}
		deliverItemsMap.erase(it);
		return true;
	}
	return false;
}

void IOInbox::asyncDeliverItems(uint32_t guid)
{
	// dispatcherInbox thread
	std::unique_lock<std::recursive_mutex> lock(taskLock);
	if (loading.find(guid) == loading.end() && deliverItemsMap.find(guid) != deliverItemsMap.end()) {
		lock.unlock();
		Inbox* inbox = loadInbox(guid);
		deliverItems(guid, inbox);
		saveInbox(guid, inbox);
		asyncSave(guid);
		delete inbox;
	}
}

void IOInbox::pushDeliveryItems(uint32_t guid, ItemBlockList& itemList)
{
	// dispatcher thread
	std::unique_lock<std::recursive_mutex> lock(taskLock);
	deliverItemsMap[guid].push_back(std::move(itemList));
	if (loading.find(guid) == loading.end()) {
		g_dispatcherInbox.addTask([this, guid]() { asyncDeliverItems(guid); });
	}
}

void IOInbox::flushDeliverItems()
{
	// dispatcherInbox thread - should be called as last task
	while (!deliverItemsMap.empty()) {
		auto it = deliverItemsMap.begin();
		asyncDeliverItems(it->first);
	}
}
