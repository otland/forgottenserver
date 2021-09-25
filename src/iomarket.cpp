/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019  Mark Samman <mark.samman@gmail.com>
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

#include "otpch.h"

#include "iomarket.h"

#include "configmanager.h"
#include "databasetasks.h"
#include "iologindata.h"
#include "game.h"
#include "scheduler.h"

#include <fmt/format.h>

extern ConfigManager g_config;
extern Game g_game;

MarketOfferList IOMarket::getActiveOffers(MarketAction_t action, uint16_t itemId)
{
	MarketOfferList offerList;

	DBResult_ptr result = Database::getInstance().storeQuery(fmt::format("SELECT `id`, `amount`, `price`, `created`, `anonymous`, (SELECT `name` FROM `players` WHERE `id` = `player_id`) AS `player_name` FROM `market_offers` WHERE `sale` = {:d} AND `itemtype` = {:d}", action, itemId));
	if (!result) {
		return offerList;
	}

	const int32_t marketOfferDuration = g_config.getNumber(ConfigManager::MARKET_OFFER_DURATION);

	do {
		MarketOffer offer;
		offer.amount = result->getNumber<uint16_t>("amount");
		offer.price = result->getNumber<uint32_t>("price");
		offer.timestamp = result->getNumber<uint32_t>("created") + marketOfferDuration;
		offer.counter = result->getNumber<uint32_t>("id") & 0xFFFF;
		if (result->getNumber<uint16_t>("anonymous") == 0) {
			offer.playerName = result->getString("player_name");
		} else {
			offer.playerName = "Anonymous";
		}
		offerList.push_back(offer);
	} while (result->next());
	return offerList;
}

MarketOfferList IOMarket::getOwnOffers(MarketAction_t action, uint32_t playerId)
{
	MarketOfferList offerList;

	const int32_t marketOfferDuration = g_config.getNumber(ConfigManager::MARKET_OFFER_DURATION);

	DBResult_ptr result = Database::getInstance().storeQuery(fmt::format("SELECT `id`, `amount`, `price`, `created`, `itemtype` FROM `market_offers` WHERE `player_id` = {:d} AND `sale` = {:d}", playerId, action));
	if (!result) {
		return offerList;
	}

	do {
		MarketOffer offer;
		offer.amount = result->getNumber<uint16_t>("amount");
		offer.price = result->getNumber<uint32_t>("price");
		offer.timestamp = result->getNumber<uint32_t>("created") + marketOfferDuration;
		offer.counter = result->getNumber<uint32_t>("id") & 0xFFFF;
		offer.itemId = result->getNumber<uint16_t>("itemtype");
		offerList.push_back(offer);
	} while (result->next());
	return offerList;
}

HistoryMarketOfferList IOMarket::getOwnHistory(MarketAction_t action, uint32_t playerId)
{
	HistoryMarketOfferList offerList;

	DBResult_ptr result = Database::getInstance().storeQuery(fmt::format("SELECT `itemtype`, `amount`, `price`, `expires_at`, `state` FROM `market_history` WHERE `player_id` = {:d} AND `sale` = {:d}", playerId, action));
	if (!result) {
		return offerList;
	}

	do {
		HistoryMarketOffer offer;
		offer.itemId = result->getNumber<uint16_t>("itemtype");
		offer.amount = result->getNumber<uint16_t>("amount");
		offer.price = result->getNumber<uint32_t>("price");
		offer.timestamp = result->getNumber<uint32_t>("expires_at");

		MarketOfferState_t offerState = static_cast<MarketOfferState_t>(result->getNumber<uint16_t>("state"));
		if (offerState == OFFERSTATE_ACCEPTEDEX) {
			offerState = OFFERSTATE_ACCEPTED;
		}

		offer.state = offerState;

		offerList.push_back(offer);
	} while (result->next());
	return offerList;
}

void IOMarket::processExpiredOffers(DBResult_ptr result, bool)
{
	if (!result) {
		return;
	}

	do {
		if (!IOMarket::moveOfferToHistory(result->getNumber<uint32_t>("id"), OFFERSTATE_EXPIRED)) {
			continue;
		}

		const uint32_t playerId = result->getNumber<uint32_t>("player_id");
		const uint16_t amount = result->getNumber<uint16_t>("amount");
		if (result->getNumber<uint16_t>("sale") == 1) {
			const ItemType& itemType = Item::items[result->getNumber<uint16_t>("itemtype")];
			if (itemType.id == 0) {
				continue;
			}

			Player* player = g_game.getPlayerByGUID(playerId);
			if (!player) {
				player = new Player(nullptr);
				if (!IOLoginData::loadPlayerById(player, playerId)) {
					delete player;
					continue;
				}
			}

			if (itemType.stackable) {
				uint16_t tmpAmount = amount;
				while (tmpAmount > 0) {
					uint16_t stackCount = std::min<uint16_t>(100, tmpAmount);
					Item* item = Item::CreateItem(itemType.id, stackCount);
					if (g_game.internalAddItem(player->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) != RETURNVALUE_NOERROR) {
						delete item;
						break;
					}

					tmpAmount -= stackCount;
				}
			} else {
				int32_t subType;
				if (itemType.charges != 0) {
					subType = itemType.charges;
				} else {
					subType = -1;
				}

				for (uint16_t i = 0; i < amount; ++i) {
					Item* item = Item::CreateItem(itemType.id, subType);
					if (g_game.internalAddItem(player->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT) != RETURNVALUE_NOERROR) {
						delete item;
						break;
					}
				}
			}

			if (player->isOffline()) {
				IOLoginData::savePlayer(player);
				delete player;
			}
		} else {
			uint64_t totalPrice = result->getNumber<uint64_t>("price") * amount;

			Player* player = g_game.getPlayerByGUID(playerId);
			if (player) {
				player->setBankBalance(player->getBankBalance() + totalPrice);
			} else {
				IOLoginData::increaseBankBalance(playerId, totalPrice);
			}
		}
	} while (result->next());
}

void IOMarket::checkExpiredOffers()
{
	const time_t lastExpireDate = time(nullptr) - g_config.getNumber(ConfigManager::MARKET_OFFER_DURATION);

	g_databaseTasks.addTask(fmt::format("SELECT `id`, `amount`, `price`, `itemtype`, `player_id`, `sale` FROM `market_offers` WHERE `created` <= {:d}", lastExpireDate), IOMarket::processExpiredOffers, true);

	int32_t checkExpiredMarketOffersEachMinutes = g_config.getNumber(ConfigManager::CHECK_EXPIRED_MARKET_OFFERS_EACH_MINUTES);
	if (checkExpiredMarketOffersEachMinutes <= 0) {
		return;
	}

	g_scheduler.addEvent(createSchedulerTask(checkExpiredMarketOffersEachMinutes * 60 * 1000, std::bind(IOMarket::checkExpiredOffers)));
}

uint32_t IOMarket::getPlayerOfferCount(uint32_t playerId)
{
	DBResult_ptr result = Database::getInstance().storeQuery(fmt::format("SELECT COUNT(*) AS `count` FROM `market_offers` WHERE `player_id` = {:d}", playerId));
	if (!result) {
		return 0;
	}
	return result->getNumber<int32_t>("count");
}

MarketOfferEx IOMarket::getOfferByCounter(uint32_t timestamp, uint16_t counter)
{
	MarketOfferEx offer;

	const int32_t created = timestamp - g_config.getNumber(ConfigManager::MARKET_OFFER_DURATION);

	DBResult_ptr result = Database::getInstance().storeQuery(fmt::format("SELECT `id`, `sale`, `itemtype`, `amount`, `created`, `price`, `player_id`, `anonymous`, (SELECT `name` FROM `players` WHERE `id` = `player_id`) AS `player_name` FROM `market_offers` WHERE `created` = {:d} AND (`id` & 65535) = {:d} LIMIT 1", created, counter));
	if (!result) {
		offer.id = 0;
		offer.playerId = 0;
		return offer;
	}

	offer.id = result->getNumber<uint32_t>("id");
	offer.type = static_cast<MarketAction_t>(result->getNumber<uint16_t>("sale"));
	offer.amount = result->getNumber<uint16_t>("amount");
	offer.counter = result->getNumber<uint32_t>("id") & 0xFFFF;
	offer.timestamp = result->getNumber<uint32_t>("created");
	offer.price = result->getNumber<uint32_t>("price");
	offer.itemId = result->getNumber<uint16_t>("itemtype");
	offer.playerId = result->getNumber<uint32_t>("player_id");
	if (result->getNumber<uint16_t>("anonymous") == 0) {
		offer.playerName = result->getString("player_name");
	} else {
		offer.playerName = "Anonymous";
	}
	return offer;
}

void IOMarket::createOffer(uint32_t playerId, MarketAction_t action, uint32_t itemId, uint16_t amount, uint32_t price, bool anonymous)
{
	Database::getInstance().executeQuery(fmt::format("INSERT INTO `market_offers` (`player_id`, `sale`, `itemtype`, `amount`, `price`, `created`, `anonymous`) VALUES ({:d}, {:d}, {:d}, {:d}, {:d}, {:d}, {:d})", playerId, action, itemId, amount, price, time(nullptr), anonymous));
}

void IOMarket::acceptOffer(uint32_t offerId, uint16_t amount)
{
	Database::getInstance().executeQuery(fmt::format("UPDATE `market_offers` SET `amount` = `amount` - {:d} WHERE `id` = {:d}", amount, offerId));
}

void IOMarket::deleteOffer(uint32_t offerId)
{
	Database::getInstance().executeQuery(fmt::format("DELETE FROM `market_offers` WHERE `id` = {:d}", offerId));
}

void IOMarket::appendHistory(uint32_t playerId, MarketAction_t type, uint16_t itemId, uint16_t amount, uint32_t price, time_t timestamp, MarketOfferState_t state)
{
	g_databaseTasks.addTask(fmt::format("INSERT INTO `market_history` (`player_id`, `sale`, `itemtype`, `amount`, `price`, `expires_at`, `inserted`, `state`) VALUES ({:d}, {:d}, {:d}, {:d}, {:d}, {:d}, {:d}, {:d})", playerId, type, itemId, amount, price, timestamp, time(nullptr), state));
}

bool IOMarket::moveOfferToHistory(uint32_t offerId, MarketOfferState_t state)
{
	const int32_t marketOfferDuration = g_config.getNumber(ConfigManager::MARKET_OFFER_DURATION);

	Database& db = Database::getInstance();

	DBResult_ptr result = db.storeQuery(fmt::format("SELECT `player_id`, `sale`, `itemtype`, `amount`, `price`, `created` FROM `market_offers` WHERE `id` = {:d}", offerId));
	if (!result) {
		return false;
	}

	if (!db.executeQuery(fmt::format("DELETE FROM `market_offers` WHERE `id` = {:d}", offerId))) {
		return false;
	}

	appendHistory(result->getNumber<uint32_t>("player_id"), static_cast<MarketAction_t>(result->getNumber<uint16_t>("sale")), result->getNumber<uint16_t>("itemtype"), result->getNumber<uint16_t>("amount"), result->getNumber<uint32_t>("price"), result->getNumber<uint32_t>("created") + marketOfferDuration, state);
	return true;
}

void IOMarket::updateStatistics()
{
	DBResult_ptr result = Database::getInstance().storeQuery(fmt::format("SELECT `sale` AS `sale`, `itemtype` AS `itemtype`, COUNT(`price`) AS `num`, MIN(`price`) AS `min`, MAX(`price`) AS `max`, SUM(`price`) AS `sum` FROM `market_history` WHERE `state` = {:d} GROUP BY `itemtype`, `sale`", OFFERSTATE_ACCEPTED));
	if (!result) {
		return;
	}

	do {
		MarketStatistics* statistics;
		if (result->getNumber<uint16_t>("sale") == MARKETACTION_BUY) {
			statistics = &purchaseStatistics[result->getNumber<uint16_t>("itemtype")];
		} else {
			statistics = &saleStatistics[result->getNumber<uint16_t>("itemtype")];
		}

		statistics->numTransactions = result->getNumber<uint32_t>("num");
		statistics->lowestPrice = result->getNumber<uint32_t>("min");
		statistics->totalPrice = result->getNumber<uint64_t>("sum");
		statistics->highestPrice = result->getNumber<uint32_t>("max");
	} while (result->next());
}

MarketStatistics* IOMarket::getPurchaseStatistics(uint16_t itemId)
{
	auto it = purchaseStatistics.find(itemId);
	if (it == purchaseStatistics.end()) {
		return nullptr;
	}
	return &it->second;
}

MarketStatistics* IOMarket::getSaleStatistics(uint16_t itemId)
{
	auto it = saleStatistics.find(itemId);
	if (it == saleStatistics.end()) {
		return nullptr;
	}
	return &it->second;
}
