// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.


#ifndef FS_IOINBOX_H
#define FS_IOINBOX_H

#include "database.h"
#include "inbox.h"
#include "iologindata.h"
#include "player.h"

struct DBEntry {
	int32_t pid;
	int32_t sid;
	uint16_t itemtype;
	uint16_t count;
	std::string attributes;
};

using DBEntryList = std::list<DBEntry>;
using DBEntryListPtr = std::shared_ptr<DBEntryList>;

struct PlayerDBEntry {
	bool saved = false;
	DBEntryListPtr items;
};

using PlayerDBEntryPtr = std::shared_ptr<PlayerDBEntry>;

class IOInbox
{
public:
	static IOInbox& getInstance()
	{
		static IOInbox instance;
		return instance;
	}

	void savePlayer(Player* player);
	void loadInboxLogin(uint32_t guid);
	void pushDeliveryItems(uint32_t guid, ItemBlockList& itemList);
	void flushDeliverItems();

private:
	IOInbox();

	static DBEntryListPtr saveItems(Player* player, const ItemBlockList& itemList);

	void saveInbox(uint32_t guid, Inbox* inbox, Player* player = nullptr);
	Inbox* loadInbox(uint32_t guid);
	void asyncSave(uint32_t guid);
	void asyncLoad(uint32_t guid);
	void assignInbox(uint32_t guid, Inbox* inbox);
	bool deliverItems(uint32_t guid, Inbox* inbox);
	void asyncDeliverItems(uint32_t guid);

	Database db;
	std::recursive_mutex taskLock;
	std::map<uint32_t, PlayerDBEntryPtr> inboxCache;
	std::map<uint32_t, std::list<ItemBlockList>> deliverItemsMap;
	std::set<uint32_t> loading;
};

#endif // FS_IOINBOX_H
