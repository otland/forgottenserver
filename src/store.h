/**
* The Forgotten Server - a free and open-source MMORPG server emulator
* Copyright (C) 2017  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_STORE_H
#define FS_STORE_H

#include <boost/optional/optional.hpp>

#include "player.h"
#include "enums.h"
#include "networkmessage.h"
#include "luascript.h"
#include "tools.h"

class StoreCategory;
class StoreOffer;

struct StoreTransaction {
	uint32_t id;
	int32_t coins;
	std::string description;
	time_t timestamp;

	StoreTransaction(uint32_t _id, int32_t _coins, const std::string& _description, time_t _timestamp) {
		id = _id;
		coins = _coins;
		description = _description;
		timestamp = _timestamp;
	}
};

class Store {
	public:
		bool loadFromXml(bool reloading = false);
		bool reload();

		std::vector<StoreCategory>& getCategories() {
			return categories;
		}

		void getTransactionHistory(uint32_t accountId, uint16_t page, uint32_t entriesPerPage, std::vector<StoreTransaction>& out) const;
		void onTransactionCompleted(uint32_t accountId, int32_t coins, const std::string& description) const;

		boost::optional<StoreOffer&> getOfferById(uint32_t id);

		//scripting
		bool executeOnBuy(Player* player, StoreOffer* offer, const std::string& param);
		bool executeOnRender(Player* player, StoreOffer* offer, std::string& reason);

	protected:
		std::unique_ptr<LuaScriptInterface> scriptInterface;
		std::vector<StoreCategory> categories;

		bool loaded = false;
		uint32_t runningId = 0;

};

class StoreEntry {
	public:
		StoreEntry() = default;

		StoreEntry(const std::string& _name, const StoreOfferState_t& _state, const std::string& _description, const StringVector& _icons) {
			name = _name;
			state = _state;
			description = _description;
			icons = _icons;
		}

		const std::string& getName() const {
			return name;
		}

		const StoreOfferState_t& getState() const {
			return state;
		}

		const std::string& getDescription() const {
			return description;
		}

		const StringVector& getIcons() const {
			return icons;
		}

		const std::string& getParent() const {
			return parent;
		}

	protected:
		std::string name;
		StoreOfferState_t state;
		std::string description;
		StringVector icons;

	private:
		std::string parent; //same as serviceType for SubOffer			

		friend class Store;
		friend class StoreOffer;
};

class StoreCategory : public StoreEntry {
	public:
		StoreCategory(const std::string& _name, const StoreOfferState_t& _state, const std::string& _description, const StringVector& _icons) :
			StoreEntry(_name, _state, _description, _icons) {

		}

		std::vector<StoreOffer>& getOffers() {
			return offers;
		}

	protected:
		std::string parent;

		std::vector<StoreOffer> offers;
		StoreOffer& createOffer(uint32_t offerId);

		friend class Store;
};

typedef StoreEntry SubOffer;

class StoreOffer : public StoreEntry {
	public:
		StoreOffer(uint32_t _id) {
			id = _id;
			price = 0;
			scriptInterface = nullptr;
			renderEvent = -1;
			buyEvent = -1;
		}

		uint32_t getId() const {
			return id;
		}

		uint32_t getPrice() const {
			return price;
		}

		std::vector<SubOffer>& getSubOffers() {
			return subOffers;
		}

		const std::string& getMessage() const {
			return message;
		}

	protected:
		uint32_t id;
		uint32_t price;
		std::string message; //on purchase message

		std::vector<SubOffer> subOffers; //bundled offers

		LuaScriptInterface* scriptInterface;

		int32_t renderEvent;
		int32_t buyEvent;

		friend class Store;
		friend class LuaScriptInterface;
};

#endif
