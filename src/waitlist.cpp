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

#include "configmanager.h"
#include "game.h"
#include "waitlist.h"

#include <tuple>

extern ConfigManager g_config;
extern Game g_game;

namespace {

struct Wait
{
	constexpr Wait(int64_t timeout, uint32_t playerGUID) :
			timeout(timeout), playerGUID(playerGUID) {}

	int64_t timeout;
	uint32_t playerGUID;
};

using WaitList = std::list<Wait>;

void cleanupList(WaitList& list)
{
	int64_t time = OTSYS_TIME();

	auto it = list.begin(), end = list.end();
	while (it != end) {
		if ((it->timeout - time) <= 0) {
			it = list.erase(it);
		} else {
			++it;
		}
	}
}

int64_t getTimeout(std::size_t slot)
{
	//timeout is set to 15 seconds longer than expected retry attempt
	return WaitingList::getTime(slot) + 15;
}

} // namespace

struct WaitListInfo
{
	WaitList priorityWaitList;
	WaitList waitList;

	std::tuple<WaitList&, WaitList::iterator, WaitList::size_type> findClient(const Player *player) {
		std::size_t slot = 1;
		for (auto it = priorityWaitList.begin(), end = priorityWaitList.end(); it != end; ++it, ++slot) {
			if (it->playerGUID == player->getGUID()) {
				return std::make_tuple(std::ref(priorityWaitList), it, slot);
			}
		}

		for (auto it = waitList.begin(), end = waitList.end(); it != end; ++it, ++slot) {
			if (it->playerGUID == player->getGUID()) {
				return std::make_tuple(std::ref(waitList), it, slot);
			}
		}
		return std::make_tuple(std::ref(waitList), waitList.end(), slot);
	}
};

WaitingList& WaitingList::getInstance()
{
	static WaitingList waitingList;
	return waitingList;
}

uint8_t WaitingList::getTime(std::size_t slot)
{
	if (slot < 5) {
		return 5;
	} else if (slot < 10) {
		return 10;
	} else if (slot < 20) {
		return 20;
	} else if (slot < 50) {
		return 60;
	} else {
		return 120;
	}
}

std::size_t WaitingList::clientLogin(const Player* player)
{
	// Currentslot = position in wait list, 0 for direct access
	std::size_t currentSlot = 0;
	if (player->hasFlag(PlayerFlag_CanAlwaysLogin) || player->getAccountType() >= ACCOUNT_TYPE_GAMEMASTER) {
		return currentSlot;
	}

	cleanupList(info->priorityWaitList);
	cleanupList(info->waitList);

	uint32_t maxPlayers = static_cast<uint32_t>(g_config.getNumber(ConfigManager::MAX_PLAYERS));
	if (maxPlayers == 0 || (info->priorityWaitList.empty() && info->waitList.empty() && g_game.getPlayersOnline() < maxPlayers)) {
		return currentSlot;
	}

	auto result = info->findClient(player);
	if (std::get<1>(result) != std::get<0>(result).end()) {
		currentSlot = std::get<2>(result);
		// If server has capacity for this client, let him in even though his current slot might be higher than 0.
		if ((g_game.getPlayersOnline() + currentSlot) <= maxPlayers) {
			std::get<0>(result).erase(std::get<1>(result));
			return 0;
		}

		//let them wait a bit longer
		std::get<1>(result)->timeout = OTSYS_TIME() + (getTimeout(currentSlot) * 1000);
		return currentSlot;
	}

	currentSlot = info->priorityWaitList.size();
	if (player->isPremium()) {
		info->priorityWaitList.emplace_back(OTSYS_TIME() + (getTimeout(++currentSlot) * 1000), player->getGUID());
	} else {
		currentSlot += info->waitList.size();
		info->waitList.emplace_back(OTSYS_TIME() + (getTimeout(++currentSlot) * 1000), player->getGUID());
	}
	return currentSlot;
}

WaitingList::WaitingList() : info(new WaitListInfo) {}
