// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_IOMARKET_H
#define FS_IOMARKET_H

#include "database.h"
#include "enums.h"

namespace tfs::io::market {

MarketOfferList getActiveOffers(MarketAction_t action, uint16_t itemId);
MarketOfferList getOwnOffers(MarketAction_t action, uint32_t playerId);
HistoryMarketOfferList getOwnHistory(MarketAction_t action, uint32_t playerId);

void processExpiredOffers(DBResult_ptr result, bool);
void checkExpiredOffers();

uint32_t getPlayerOfferCount(uint32_t playerId);
MarketOfferEx getOfferByCounter(uint32_t timestamp, uint16_t counter);

void createOffer(uint32_t playerId, MarketAction_t action, uint32_t itemId, uint16_t amount, uint64_t price,
                 bool anonymous);
void acceptOffer(uint32_t offerId, uint16_t amount);
void deleteOffer(uint32_t offerId);

void appendHistory(uint32_t playerId, MarketAction_t type, uint16_t itemId, uint16_t amount, uint64_t price,
                   time_t timestamp, MarketOfferState_t state);
bool moveOfferToHistory(uint32_t offerId, MarketOfferState_t state);

void updateStatistics();

MarketStatistics* getPurchaseStatistics(uint16_t itemId);
MarketStatistics* getSaleStatistics(uint16_t itemId);

} // namespace tfs::io::market

#endif // FS_IOMARKET_H
