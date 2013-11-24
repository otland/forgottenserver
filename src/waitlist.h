/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
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

#ifndef WAITLIST_H
#define WAITLIST_H

#include "player.h"

struct Wait {
	uint32_t acc;
	uint32_t ip;
	std::string name;
	bool premium;
	int64_t timeout;
};

typedef std::list<Wait*> WaitList;
typedef WaitList::iterator WaitListIterator;

class WaitingList
{
	public:
		static WaitingList* getInstance() {
			static WaitingList waitingList;
			return &waitingList;
		}

		bool clientLogin(const Player* player);
		int32_t getClientSlot(const Player* player);
		static int32_t getTime(int32_t slot);

	protected:
		WaitList priorityWaitList;
		WaitList waitList;

		static int32_t getTimeOut(int32_t slot);
		WaitListIterator findClient(const Player* player, uint32_t& slot);
		void cleanUpList();
};

#endif
