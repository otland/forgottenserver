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

#ifndef __OTSERV_IOMARKET_H__
#define __OTSERV_IOMARKET_H__

#include <string>
#include "account.h"
#include "player.h"
#include "database.h"

class IOMarket
{
	public:
		IOMarket() {}
		~IOMarket() {}

		static IOMarket* getInstance() {
			static IOMarket instance;
			return &instance;
		}

		MarketOfferList getActiveOffers(MarketAction_t action, uint16_t itemId);
		MarketOfferList getOwnOffers(MarketAction_t action, uint32_t playerId);
		HistoryMarketOfferList getOwnHistory(MarketAction_t action, uint32_t playerId);

		ExpiredMarketOfferList getExpiredOffers(MarketAction_t action);

		int32_t getPlayerOfferCount(uint32_t playerId);
		uint32_t getOfferIdByCounter(uint32_t timestamp, uint16_t counter);
		MarketOfferEx getOfferById(uint32_t id);

		void createOffer(uint32_t playerId, MarketAction_t action, uint32_t itemId, uint16_t amount, uint32_t price, bool anonymous);
		void acceptOffer(uint32_t offerId, uint16_t amount);
		void deleteOffer(uint32_t offerId);

		void appendHistory(uint32_t playerId, MarketAction_t type, uint16_t itemId, uint16_t amount, uint32_t price, time_t timestamp, MarketOfferState_t state);
		void moveOfferToHistory(uint32_t offerId, MarketOfferState_t state);

		void updateStatistics();

		MarketStatistics* getPurchaseStatistics(uint16_t itemId);
		MarketStatistics* getSaleStatistics(uint16_t itemId);

	private:
		std::map<uint16_t, MarketStatistics> purchaseStatistics;
		std::map<uint16_t, MarketStatistics> saleStatistics;
};

#endif
