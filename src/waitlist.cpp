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

#include "otpch.h"

#include <iostream>
#include <sstream>

#include "configmanager.h"
#include "waitlist.h"
#include "status.h"

extern ConfigManager g_config;

WaitingList::WaitingList()
{
	//
}

WaitingList::~WaitingList()
{
	waitList.clear();
}

WaitListIterator WaitingList::findClient(const Player* player, uint32_t& slot)
{
	slot = 1;

	for (WaitListIterator it = waitList.begin(); it != waitList.end(); ++it) {
		Wait* wait = *it;

		if (wait->acc == player->getAccount() && wait->ip == player->getIP() && wait->name == player->getName()) {
			return it;
		}

		++slot;
	}

	return waitList.end();
}

int32_t WaitingList::getTime(int32_t slot)
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

int32_t WaitingList::getTimeOut(int32_t slot)
{
	//timeout is set to 15 seconds longer than expected retry attempt
	return getTime(slot) + 15;
}

bool WaitingList::clientLogin(const Player* player)
{
	if (player->hasFlag(PlayerFlag_CanAlwaysLogin) || player->getAccountType() >= ACCOUNT_TYPE_GAMEMASTER) {
		return true;
	}

	if (waitList.empty() && Status::getInstance()->getPlayersOnline() < (uint32_t)g_config.getNumber(ConfigManager::MAX_PLAYERS)) {
		//no waiting list and enough room
		return true;
	}

	cleanUpList();

	uint32_t slot;

	WaitListIterator it = findClient(player, slot);
	if (it != waitList.end()) {
		if ((Status::getInstance()->getPlayersOnline() + slot) <= (uint32_t)g_config.getNumber(ConfigManager::MAX_PLAYERS)) {
			//should be able to login now
			delete *it;
			waitList.erase(it);
			return true;
		} else {
			//let them wait a bit longer
			(*it)->timeout = OTSYS_TIME() + (getTimeOut(slot) * 1000);
			return false;
		}
	}

	Wait* wait = new Wait();

	if (player->isPremium()) {
		slot = 1;

		for (it = waitList.begin(); it != waitList.end(); ++it) {
			if (!(*it)->premium) {
				waitList.insert(it, wait);
				break;
			}

			++slot;
		}
	} else {
		waitList.push_back(wait);
		slot = (uint32_t)waitList.size();
	}

	wait->name = player->getName();
	wait->acc = player->getAccount();
	wait->ip = player->getIP();
	wait->premium = player->isPremium();
	wait->timeout = OTSYS_TIME() + (getTimeOut(slot) * 1000);
	return false;
}

int32_t WaitingList::getClientSlot(const Player* player)
{
	uint32_t slot;
	WaitListIterator it = findClient(player, slot);

	if (it != waitList.end()) {
		return slot;
	}

	return -1;
}

void WaitingList::cleanUpList()
{
	for (WaitListIterator it = waitList.begin(); it != waitList.end();) {
		if ((*it)->timeout - OTSYS_TIME() <= 0) {
			delete *it;
			waitList.erase(it++);
		} else {
			++it;
		}
	}
}
