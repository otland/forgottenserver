//////////////////////////////////////////////////////////////////////
// The Forgotten Server - a server application for the MMORPG Tibia
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#ifndef __OTS_BEDS_H__
#define __OTS_BEDS_H__

#include "item.h"
#include "position.h"
#include "definitions.h"

#include <ctime>
#include <list>

class House;
class Player;

class BedItem : public Item
{
	public:
		BedItem(uint16_t id);
		virtual ~BedItem() {};

		virtual BedItem* getBed() {
			return this;
		}
		virtual const BedItem* getBed() const {
			return this;
		}

		virtual Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream);
		virtual bool serializeAttr(PropWriteStream& propWriteStream) const;

		virtual bool canRemove() const {
			return (house == NULL);
		}

		uint32_t getSleeper() const {
			return sleeperGUID;
		}
		void setSleeper(uint32_t guid) {
			sleeperGUID = guid;
		}

		uint64_t getSleepStart() const {
			return sleepStart;
		}
		void setSleepStart(uint64_t now) {
			sleepStart = now;
		}

		House* getHouse() const {
			return house;
		}
		void setHouse(House* h) {
			house = h;
		}

		bool canUse(Player* player);

		bool trySleep(Player* player);
		bool sleep(Player* player);
		void wakeUp(Player* player);

		BedItem* getNextBedItem();

	protected:
		void updateAppearance(const Player* player);
		void regeneratePlayer(Player* player) const;
		void internalSetSleeper(const Player* player);
		void internalRemoveSleeper();

		uint32_t sleeperGUID;
		uint64_t sleepStart;
		House* house;
};

class Beds
{
	public:
		virtual ~Beds() {}

		static Beds& getInstance() {
			static Beds instance;
			return instance;
		}

		BedItem* getBedBySleeper(uint32_t guid);
		void setBedSleeper(BedItem* bed, uint32_t guid);

	protected:
		Beds() {
			BedSleepersMap.clear();
		}

		std::map<uint32_t, BedItem*> BedSleepersMap;
};

#endif
