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

#ifndef HOUSETILE_H
#define HOUSETILE_H

#include "tile.h"

class House;

class HouseTile : public DynamicTile
{
	public:
		HouseTile(int32_t x, int32_t y, int32_t z, House* _house);
		~HouseTile();

		//cylinder implementations
		virtual ReturnValue __queryAdd(int32_t index, const Thing* thing, uint32_t count,
		                               uint32_t flags, Creature* actor = nullptr) const;

		virtual Cylinder* __queryDestination(int32_t& index, const Thing* thing, Item** destItem,
		                                     uint32_t& flags);

		virtual void __addThing(int32_t index, Thing* thing);
		virtual void __internalAddThing(uint32_t index, Thing* thing);

		House* getHouse() {
			return house;
		}

	private:
		void updateHouse(Item* item);

		House* house;
};

#endif
