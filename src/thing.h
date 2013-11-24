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

#ifndef THING_H
#define THING_H

#include "position.h"

class Tile;
class Cylinder;
class Item;
class Creature;
class Container;

class Thing
{
	protected:
		Thing();

	public:
		virtual ~Thing();

		virtual std::string getDescription(int32_t lookDistance) const = 0;

		virtual Cylinder* getParent() {
			return nullptr;
		}
		virtual const Cylinder* getParent() const {
			return nullptr;
		}

		virtual void setParent(Cylinder* cylinder) {
			//
		}

		virtual Tile* getTile();
		virtual const Tile* getTile() const;

		virtual const Position& getPosition() const;
		virtual int32_t getThrowRange() const = 0;
		virtual bool isPushable() const = 0;

		virtual Container* getContainer() {
			return nullptr;
		}
		virtual const Container* getContainer() const {
			return nullptr;
		}
		virtual Item* getItem() {
			return nullptr;
		}
		virtual const Item* getItem() const {
			return nullptr;
		}
		virtual Creature* getCreature() {
			return nullptr;
		}
		virtual const Creature* getCreature() const {
			return nullptr;
		}

		virtual bool isRemoved() const {
			return true;
		}
};

#endif
