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

#include "otpch.h"

#include "configmanager.h"
#include "game.h"
#include "waitlist.h"

extern ConfigManager g_config;
extern Game g_game;

WaitListIterator WaitingList::findClient(const Player* player, uint32_t& slot)
{
	slot = 1;
	for (WaitListIterator it = priorityWaitList.begin(); it != priorityWaitList.end(); ++it) {
		if (it->playerGUID == player->getGUID()) {
			return it;
		}
		++slot;
	}

	for (WaitListIterator it = waitList.begin(); it != waitList.end(); ++it) {
		if (it->playerGUID == player->getGUID()) {
			return it;
		}
		++slot;
	}
	return waitList.end();
}

uint32_t WaitingList::getTime(uint32_t slot)
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

uint32_t WaitingList::getTimeout(uint32_t slot)
{
	//timeout is set to 15 seconds longer than expected retry attempt
	return getTime(slot) + 15;
}

bool WaitingList::clientLogin(const Player* player)
{
	if (player->hasFlag(PlayerFlag_CanAlwaysLogin) || player->getAccountType() >= ACCOUNT_TYPE_GAMEMASTER) {
		return true;
	}

	uint32_t maxPlayers = static_cast<uint32_t>(g_config.getNumber(ConfigManager::MAX_PLAYERS));
	if (priorityWaitList.empty() && waitList.empty() && g_game.getPlayersOnline() < maxPlayers) {
		return true;
	}

	WaitingList::cleanupList(priorityWaitList);
	WaitingList::cleanupList(waitList);

	uint32_t slot;

	WaitListIterator it = findClient(player, slot);
	if (it != waitList.end()) {
		if ((g_game.getPlayersOnline() + slot) <= maxPlayers) {
			//should be able to login now
			waitList.erase(it);
			return true;
		}

		//let them wait a bit longer
		it->timeout = OTSYS_TIME() + (getTimeout(slot) * 1000);
		return false;
	}

	slot = priorityWaitList.size();
	if (player->isPremium()) {
		priorityWaitList.emplace_back(OTSYS_TIME() + (getTimeout(slot + 1) * 1000), player->getGUID());
	} else {
		slot += waitList.size();
		waitList.emplace_back(OTSYS_TIME() + (getTimeout(slot + 1) * 1000), player->getGUID());
	}
	return false;
}

uint32_t WaitingList::getClientSlot(const Player* player)
{
	uint32_t slot;
	WaitListIterator it = findClient(player, slot);
	if (it == waitList.end()) {
		return 0;
	}
	return slot;
}

void WaitingList::cleanupList(WaitList& list)
{
	for (WaitListIterator it = list.begin(); it != list.end();) {
		if (it->timeout - OTSYS_TIME() <= 0) {
			it = list.erase(it);
		} else {
			++it;
		}
	}
}
