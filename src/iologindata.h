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

#ifndef __IOLOGINDATA_H
#define __IOLOGINDATA_H

#include <string>
#include "account.h"
#include "player.h"
#include "database.h"

typedef std::pair<int32_t, Item*> itemBlock;
typedef std::list<itemBlock> ItemBlockList;

class IOLoginData
{
	public:
		static Account loadAccount(uint32_t accno);
		static bool saveAccount(const Account& acc);

		static bool loginserverAuthentication(const std::string& name, const std::string& password, Account& account);
		static uint32_t gameworldAuthentication(const std::string& accountName, const std::string& password, std::string& characterName);

		static AccountType_t getAccountType(uint32_t accountId);
		static void setAccountType(uint32_t accountId, AccountType_t accountType);
		static bool updateOnlineStatus(uint32_t guid, bool login);
		static bool preloadPlayer(Player* player, const std::string& name);

		static bool loadPlayerById(Player* player, uint32_t id);
		static bool loadPlayerByName(Player* player, const std::string& name);
		static bool loadPlayer(Player* player, DBResult* result);
		static bool savePlayer(Player* player);
		static bool getGuidByName(uint32_t& guid, std::string& name);
		static bool getGuidByNameEx(uint32_t& guid, bool& specialVip, std::string& name);
		static bool getNameByGuid(uint32_t guid, std::string& name);
		static bool formatPlayerName(std::string& name);
		static bool changeName(uint32_t guid, const std::string& newName);
		static uint32_t getAccountNumberByName(const std::string& name);
		static bool addStorageValue(uint32_t guid, uint32_t storageKey, uint32_t storageValue);
		static void increaseBankBalance(uint32_t guid, uint64_t bankBalance);
		static time_t getLastLoginSaved(uint32_t guid);
		static bool hasBiddedOnHouse(uint32_t guid);
		static void updateHouseOwners();

		static std::list<VIPEntry> getVIPEntries(uint32_t accountId);
		static void addVIPEntry(uint32_t accountId, uint32_t guid, const std::string& description, uint32_t icon, bool notify);
		static void editVIPEntry(uint32_t accountId, uint32_t guid, const std::string& description, uint32_t icon, bool notify);
		static void removeVIPEntry(uint32_t accountId, uint32_t guid);

		static void addPremiumDays(uint32_t accountId, int32_t addDays);
		static void removePremiumDays(uint32_t accountId, int32_t removeDays);

	protected:
		typedef std::map<int32_t , std::pair<Item*, int32_t> > ItemMap;

		static void loadItems(ItemMap& itemMap, DBResult* result);
		static bool saveItems(const Player* player, const ItemBlockList& itemList, DBInsert& query_insert);
};

#endif
