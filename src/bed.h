// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_BED_H
#define FS_BED_H

#include "item.h"

class House;
class Player;

class BedItem final : public Item, public std::enable_shared_from_this<BedItem>
{
public:
	explicit BedItem(uint16_t id);

	using std::enable_shared_from_this<BedItem>::shared_from_this;
	std::shared_ptr<BedItem> getBed() override { return shared_from_this(); }
	std::shared_ptr<const BedItem> getBed() const override { return shared_from_this(); }

	Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream) override;
	void serializeAttr(PropWriteStream& propWriteStream) const override;

	bool canRemove() const override { return !house; }

	uint32_t getSleeper() const { return sleeperGUID; }

	House* getHouse() const { return house; }
	void setHouse(House* h) { house = h; }

	bool canUse(std::shared_ptr<Player> player);

	bool trySleep(std::shared_ptr<Player> player);
	bool sleep(std::shared_ptr<Player> player);
	void wakeUp(std::shared_ptr<Player> player);

	std::shared_ptr<BedItem> getNextBedItem() const;

private:
	void updateAppearance(std::shared_ptr<const Player> player);
	void regeneratePlayer(std::shared_ptr<Player> player) const;
	void internalSetSleeper(std::shared_ptr<const Player> player);
	void internalRemoveSleeper();

	House* house = nullptr;
	uint64_t sleepStart;
	uint32_t sleeperGUID;
};

#endif // FS_BED_H
