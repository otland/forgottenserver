// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_BED_H
#define FS_BED_H

#include "item.h"

class House;
class Player;

class BedItem final : public Item
{
public:
	explicit BedItem(uint16_t id);

	BedItem* getBed() override { return this; }
	const BedItem* getBed() const override { return this; }

	void readAttr(AttrTypes_t attr, OTB::iterator& first, const OTB::iterator& last) override;
	void serializeAttr(PropWriteStream& propWriteStream) const override;

	bool canRemove() const override { return !house; }

	uint32_t getSleeper() const { return sleeperGUID; }

	House* getHouse() const { return house; }
	void setHouse(House* h) { house = h; }

	bool canUse(Player* player);

	bool trySleep(Player* player);
	bool sleep(Player* player);
	void wakeUp(Player* player);

	BedItem* getNextBedItem() const;

private:
	void updateAppearance(const Player* player);
	void regeneratePlayer(Player* player) const;
	void internalSetSleeper(const Player* player);
	void internalRemoveSleeper();

	House* house = nullptr;
	uint64_t sleepStart;
	uint32_t sleeperGUID;
};

#endif // FS_BED_H
