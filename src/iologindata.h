/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2014  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_IOLOGINDATA_H_28B0440BEC594654AC0F4E1A5E42B2EF
#define FS_IOLOGINDATA_H_28B0440BEC594654AC0F4E1A5E42B2EF

#include "account.h"
#include "player.h"
#include "database.h"
#include "databasedispatcher.h"

typedef std::list<std::pair<int32_t, Item*>> ItemBlockList;

class IOLoginData
{
	public:
		static void asyncSaveAccount(const Account acc);

		static void asyncLoginserverAuthentication(const std::string& name, const std::string& password, std::function<void(Account, bool)> callback);

		static void asyncGameworldAuthentication(const std::string& accountName, const std::string& password, std::string characterName,
											std::function<void(uint32_t, std::string)> callback);

		static AccountType_t getAccountType(uint32_t accountId);
		static void setAccountType(uint32_t accountId, AccountType_t accountType);
		static bool updateOnlineStatus(uint32_t guid, bool login);
		static void asyncPreloadPlayer(const std::string& name, DBCallback callback);

		static bool loadPlayerById(Player* player, uint32_t id);

		static void asyncLoadPlayerById(const std::string& id, DBCallback callback);
		static void asyncLoadPlayerByName(const std::string& name, DBCallback callback);

		static void asyncLoadPlayer(Player* player, DBResult_ptr result, std::function<void(bool)> callback);

		static void asyncLoadAccount(Player *player, uint32_t accno, std::function<void(bool)> callback);
		static void asyncLoadGuild(Player* player, std::function<void()> callback);
		static void asyncLoadSpells(Player* player, std::function<void()> callback);
		static void asyncLoadItems(Player* player, std::function<void()> callback);
		static void asyncLoadDepot(Player* player, std::function<void()> callback);
		static void asyncLoadInbox(Player* player, std::function<void()> callback);
		static void asyncLoadStorage(Player* player, std::function<void()> callback);
		static void asyncLoadVip(Player* player, std::function<void()> callback);

		static bool savePlayer(Player* player);
		static bool getGuidByName(uint32_t& guid, std::string& name);
		static bool getGuidByNameEx(uint32_t& guid, bool& specialVip, std::string& name);
		static bool getNameByGuid(uint32_t guid, std::string& name);
		static bool formatPlayerName(std::string& name);
		static bool addStorageValue(uint32_t guid, uint32_t storageKey, uint32_t storageValue);
		static void increaseBankBalance(uint32_t guid, uint64_t bankBalance);
		static bool hasBiddedOnHouse(uint32_t guid);

		static std::forward_list<VIPEntry> getVIPEntries(uint32_t accountId);
		static void addVIPEntry(uint32_t accountId, uint32_t guid, const std::string& description, uint32_t icon, bool notify);
		static void editVIPEntry(uint32_t accountId, uint32_t guid, const std::string& description, uint32_t icon, bool notify);
		static void removeVIPEntry(uint32_t accountId, uint32_t guid);

		static void addPremiumDays(uint32_t accountId, int32_t addDays);
		static void removePremiumDays(uint32_t accountId, int32_t removeDays);

	protected:
		typedef std::map<int32_t , std::pair<Item*, int32_t> > ItemMap;

		static void asyncLoadItems(ItemMap& itemMap, DBResult_ptr result);
		static bool saveItems(const Player* player, const ItemBlockList& itemList, DBInsert& query_insert, PropWriteStream& stream);
};

#endif
