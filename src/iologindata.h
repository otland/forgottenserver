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
		static IOLoginData* getInstance() {
			static IOLoginData instance;
			return &instance;
		}

		Account loadAccount(uint32_t accno);
		bool saveAccount(const Account& acc);

		bool loginserverAuthentication(const std::string& name, const std::string& password, Account& account);
		uint32_t gameworldAuthentication(const std::string& accountName, const std::string& password, std::string& characterName);

		AccountType_t getAccountType(uint32_t accountId);
		void setAccountType(uint32_t accountId, AccountType_t accountType);
		bool updateOnlineStatus(uint32_t guid, bool login);
		bool preloadPlayer(Player* player, const std::string& name);

		bool loadPlayerById(Player* player, uint32_t id);
		bool loadPlayerByName(Player* player, const std::string& name);
		bool loadPlayer(Player* player, DBResult* result);
		bool savePlayer(Player* player);
		bool getGuidByName(uint32_t& guid, std::string& name);
		bool getGuidByNameEx(uint32_t& guid, bool& specialVip, std::string& name);
		bool getNameByGuid(uint32_t guid, std::string& name);
		bool playerExists(const std::string& name);
		bool formatPlayerName(std::string& name);
		bool changeName(uint32_t guid, const std::string& newName);
		uint32_t getAccountNumberByName(const std::string& name);
		bool addStorageValue(uint32_t guid, uint32_t storageKey, uint32_t storageValue);
		uint32_t getLastIPByName(const std::string& name);
		void increaseBankBalance(uint32_t guid, uint64_t bankBalance);
		time_t getLastLoginSaved(uint32_t guid);
		bool hasBiddedOnHouse(uint32_t guid);
		void updateHouseOwners();

		std::list<VIPEntry> getVIPEntries(uint32_t accountId);
		void addVIPEntry(uint32_t accountId, uint32_t guid, const std::string& description, uint32_t icon, bool notify);
		void editVIPEntry(uint32_t accountId, uint32_t guid, const std::string& description, uint32_t icon, bool notify);
		void removeVIPEntry(uint32_t accountId, uint32_t guid);

		void addPremiumDays(uint32_t accountId, int32_t addDays);
		void removePremiumDays(uint32_t accountId, int32_t removeDays);

	protected:
		typedef std::map<int32_t , std::pair<Item*, int32_t> > ItemMap;

		void loadItems(ItemMap& itemMap, DBResult* result);
		bool saveItems(const Player* player, const ItemBlockList& itemList, DBInsert& query_insert);
};

#endif
