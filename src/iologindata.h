//////////////////////////////////////////////////////////////////////
// The Forgotten Server - a server application for the MMORPG Tibia
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#ifndef __IOLOGINDATA_H
#define __IOLOGINDATA_H

#include <string>
#include "account.h"
#include "player.h"
#include "database.h"

struct PlayerGroup {
	std::string m_name;
	uint64_t m_flags;
	uint32_t m_access;
	uint32_t m_maxdepotitems;
	uint32_t m_maxviplist;
};

typedef std::pair<int32_t, Item*> itemBlock;
typedef std::list<itemBlock> ItemBlockList;

class IOLoginData
{
	public:
		IOLoginData() {}
		~IOLoginData() {
			for (PlayerGroupMap::iterator it = playerGroupMap.begin(); it != playerGroupMap.end(); ++it) {
				delete it->second;
			}
		}

		static IOLoginData* getInstance() {
			static IOLoginData instance;
			return &instance;
		}

		Account loadAccount(uint32_t accno);
		Account loadAccount(const std::string& name);
		bool saveAccount(const Account& acc);
		bool getPassword(const std::string& accname, const std::string& name, std::string& password, uint32_t& accNumber);
		AccountType_t getAccountType(uint32_t accountId);
		void setAccountType(uint32_t accountId, AccountType_t accountType);
		bool updateOnlineStatus(uint32_t guid, bool login);
		bool loadPlayer(Player* player, const std::string& name, bool preload = false);
		bool savePlayer(Player* player);
		bool getGuidByName(uint32_t& guid, std::string& name);
		bool getGuidByNameEx(uint32_t& guid, bool& specialVip, std::string& name);
		bool getNameByGuid(uint32_t guid, std::string& name);
		bool playerExists(const std::string& name);
		bool formatPlayerName(std::string& name);
		bool changeName(uint32_t guid, const std::string& newName);
		uint32_t getAccountNumberByName(const std::string& name);
		bool addStorageValue(uint32_t guid, uint32_t storageKey, uint32_t storageValue);
		const PlayerGroup* getPlayerGroup(uint32_t groupid);
		uint32_t getLastIPByName(const std::string& name);
		void increaseBankBalance(uint32_t guid, uint64_t bankBalance);
		time_t getLastLoginSaved(uint32_t guid);
		bool isPendingDeletion(const std::string& characterName);
		bool hasBiddedOnHouse(uint32_t guid);
		void updateHouseOwners();

		std::list<VIPEntry> getVIPEntries(uint32_t accountId);
		void addVIPEntry(uint32_t accountId, uint32_t guid, const std::string& description, uint32_t icon, bool notify);
		void editVIPEntry(uint32_t accountId, uint32_t guid, const std::string& description, uint32_t icon, bool notify);
		void removeVIPEntry(uint32_t accountId, uint32_t guid);

	protected:
		const PlayerGroup* getPlayerGroupByAccount(uint32_t accno);
		struct StringCompareCase {
			bool operator()(const std::string& l, const std::string& r) const {
				return strcasecmp(l.c_str(), r.c_str()) < 0;
			}
		};

		typedef std::map<int32_t , std::pair<Item*, int32_t> > ItemMap;

		void loadItems(ItemMap& itemMap, DBResult* result);
		bool saveItems(const Player* player, const ItemBlockList& itemList, DBInsert& query_insert);

		typedef std::map<uint32_t, std::string> NameCacheMap;
		typedef std::map<std::string, uint32_t, StringCompareCase> GuidCacheMap;
		typedef std::map<uint32_t, PlayerGroup*> PlayerGroupMap;

		PlayerGroupMap playerGroupMap;
		NameCacheMap nameCacheMap;
		GuidCacheMap guidCacheMap;
};

#endif
