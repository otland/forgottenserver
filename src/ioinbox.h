// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_IOINBOX_H
#define FS_IOINBOX_H

#include "database.h"
#include "inbox.h"
#include "iologindata.h"
#include "player.h"

struct DBEntry
{
	int32_t pid;
	int32_t sid;
	uint16_t itemtype;
	uint16_t count;
	std::string attributes;
};

using DBEntryList = std::list<DBEntry>;
using DBEntryListPtr = std::shared_ptr<DBEntryList>;

struct PlayerDBEntry
{
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

	void savePlayer(const Player* player);
	void loadPlayer(const Player* player);

	void savePlayerItems(const Player* player, ItemBlockList& itemList);
	void savePlayerItems(const uint32_t& guid, ItemBlockList& itemList);

	void flush();

private:
	IOInbox();

	static DBEntryListPtr saveItems(const Player* player, const ItemBlockList& itemList);
	static Inbox* createInboxItem(const ItemMap& items);

	void savePlayerAsync(const uint32_t& guid);
	void loadPlayerAsync(const uint32_t& guid);

	Inbox* loadInbox(const uint32_t& guid);
	void saveInbox(const uint32_t& guid, Inbox* inbox, const Player* player = nullptr);

	bool canSavePlayerItems(const uint32_t& guid);
	DBEntryListPtr getPlayerInbox(const uint32_t& guid);
	void addPlayerInbox(const uint32_t& guid, DBEntryListPtr inbox);
	DBEntryListPtr loadPlayerInbox(const uint32_t& guid);

	void assignInbox(const uint32_t& guid, Inbox* inbox);
	bool deliverItems(const uint32_t& guid, Inbox* inbox);
	void savePlayerItemsAsync(const uint32_t& guid);

	Database db;
	std::recursive_mutex lock;
	std::map<uint32_t, PlayerDBEntryPtr> inboxCache;
	std::map<uint32_t, std::list<ItemBlockList>> pendingItemsToSave;
	std::set<uint32_t> pendingPlayerSet;
};

#endif // FS_IOINBOX_H
