// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_IOLOGINDATA_H
#define FS_IOLOGINDATA_H

#include "database.h"
#include "enums.h"

class Item;
class Player;
class PropWriteStream;
struct VIPEntry;

using ItemBlockList = std::list<std::pair<int32_t, std::shared_ptr<Item>>>;

class IOLoginData
{
public:
	static uint32_t getAccountIdByPlayerName(const std::string& playerName);
	static uint32_t getAccountIdByPlayerId(uint32_t playerId);

	static AccountType_t getAccountType(uint32_t accountId);
	static void setAccountType(uint32_t accountId, AccountType_t accountType);
	static void updateOnlineStatus(uint32_t guid, bool login);
	static bool preloadPlayer(std::shared_ptr<Player> player);

	static bool loadPlayerById(std::shared_ptr<Player> player, uint32_t id);
	static bool loadPlayerByName(std::shared_ptr<Player> player, const std::string& name);
	static bool loadPlayer(std::shared_ptr<Player> player, DBResult_ptr result);
	static bool savePlayer(std::shared_ptr<Player> player);
	static uint32_t getGuidByName(const std::string& name);
	static bool getGuidByNameEx(uint32_t& guid, bool& specialVip, std::string& name);
	static std::string getNameByGuid(uint32_t guid);
	static bool formatPlayerName(std::string& name);
	static void increaseBankBalance(uint32_t guid, uint64_t bankBalance);
	static bool hasBiddedOnHouse(uint32_t guid);

	static std::forward_list<VIPEntry> getVIPEntries(uint32_t accountId);
	static void addVIPEntry(uint32_t accountId, uint32_t guid, const std::string& description, uint32_t icon,
	                        bool notify);
	static void editVIPEntry(uint32_t accountId, uint32_t guid, const std::string& description, uint32_t icon,
	                         bool notify);
	static void removeVIPEntry(uint32_t accountId, uint32_t guid);

	static void updatePremiumTime(uint32_t accountId, time_t endTime);

private:
	using ItemMap = std::map<uint32_t, std::pair<std::shared_ptr<Item>, uint32_t>>;

	static void loadItems(ItemMap& itemMap, DBResult_ptr result);
	static bool saveItems(std::shared_ptr<const Player> player, const ItemBlockList& itemList, DBInsert& query_insert,
	                      PropWriteStream& propWriteStream);
};

#endif // FS_IOLOGINDATA_H
