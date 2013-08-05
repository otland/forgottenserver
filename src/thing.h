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

#ifndef __THING_H__
#define __THING_H__

#include "position.h"

enum ReturnValue {
	RET_NOERROR = 1,
	RET_NOTPOSSIBLE = 2,
	RET_NOTENOUGHROOM = 3,
	RET_PLAYERISPZLOCKED = 4,
	RET_PLAYERISNOTINVITED = 5,
	RET_CANNOTTHROW = 6,
	RET_THEREISNOWAY = 7,
	RET_DESTINATIONOUTOFREACH = 8,
	RET_CREATUREBLOCK = 9,
	RET_NOTMOVEABLE = 10,
	RET_DROPTWOHANDEDITEM = 11,
	RET_BOTHHANDSNEEDTOBEFREE = 12,
	RET_CANONLYUSEONEWEAPON = 13,
	RET_NEEDEXCHANGE = 14,
	RET_CANNOTBEDRESSED = 15,
	RET_PUTTHISOBJECTINYOURHAND = 16,
	RET_PUTTHISOBJECTINBOTHHANDS = 17,
	RET_TOOFARAWAY = 18,
	RET_FIRSTGODOWNSTAIRS = 19,
	RET_FIRSTGOUPSTAIRS = 20,
	RET_CONTAINERNOTENOUGHROOM = 21,
	RET_NOTENOUGHCAPACITY = 22,
	RET_CANNOTPICKUP = 23,
	RET_THISISIMPOSSIBLE = 24,
	RET_DEPOTISFULL = 25,
	RET_CREATUREDOESNOTEXIST = 26,
	RET_CANNOTUSETHISOBJECT = 27,
	RET_PLAYERWITHTHISNAMEISNOTONLINE = 28,
	RET_NOTREQUIREDLEVELTOUSERUNE = 29,
	RET_YOUAREALREADYTRADING = 30,
	RET_THISPLAYERISALREADYTRADING = 31,
	RET_YOUMAYNOTLOGOUTDURINGAFIGHT = 32,
	RET_DIRECTPLAYERSHOOT = 33,
	RET_NOTENOUGHLEVEL = 34,
	RET_NOTENOUGHMAGICLEVEL = 35,
	RET_NOTENOUGHMANA = 36,
	RET_NOTENOUGHSOUL = 37,
	RET_YOUAREEXHAUSTED = 38,
	RET_PLAYERISNOTREACHABLE = 39,
	RET_CANONLYUSETHISRUNEONCREATURES = 40,
	RET_ACTIONNOTPERMITTEDINPROTECTIONZONE = 41,
	RET_YOUMAYNOTATTACKTHISPLAYER = 42,
	RET_YOUMAYNOTATTACKAPERSONINPROTECTIONZONE = 43,
	RET_YOUMAYNOTATTACKAPERSONWHILEINPROTECTIONZONE = 44,
	RET_YOUMAYNOTATTACKTHISCREATURE = 45,
	RET_YOUCANONLYUSEITONCREATURES = 46,
	RET_CREATUREISNOTREACHABLE = 47,
	RET_TURNSECUREMODETOATTACKUNMARKEDPLAYERS = 48,
	RET_YOUNEEDPREMIUMACCOUNT = 49,
	RET_YOUNEEDTOLEARNTHISSPELL = 50,
	RET_YOURVOCATIONCANNOTUSETHISSPELL = 51,
	RET_YOUNEEDAWEAPONTOUSETHISSPELL = 52,
	RET_PLAYERISPZLOCKEDLEAVEPVPZONE = 53,
	RET_PLAYERISPZLOCKEDENTERPVPZONE = 54,
	RET_ACTIONNOTPERMITTEDINANOPVPZONE = 55,
	RET_YOUCANNOTLOGOUTHERE = 56,
	RET_YOUNEEDAMAGICITEMTOCASTSPELL = 57,
	RET_CANNOTCONJUREITEMHERE = 58,
	RET_YOUNEEDTOSPLITYOURSPEARS = 59,
	RET_NAMEISTOOAMBIGIOUS = 60,
	RET_CANONLYUSEONESHIELD = 61,
	RET_NOPARTYMEMBERSINRANGE = 62,
	RET_YOUARENOTTHEOWNER = 63
};

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
			return NULL;
		}
		virtual const Cylinder* getParent() const {
			return NULL;
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
			return NULL;
		}
		virtual const Container* getContainer() const {
			return NULL;
		}
		virtual Item* getItem() {
			return NULL;
		}
		virtual const Item* getItem() const {
			return NULL;
		}
		virtual Creature* getCreature() {
			return NULL;
		}
		virtual const Creature* getCreature() const {
			return NULL;
		}

		virtual bool isRemoved() const {
			return true;
		}
};

#endif
