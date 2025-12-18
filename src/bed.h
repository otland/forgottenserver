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

	std::shared_ptr<BedItem> getBed() override { return std::static_pointer_cast<BedItem>(shared_from_this()); }
	std::shared_ptr<const BedItem> getBed() const override
	{
		return std::static_pointer_cast<const BedItem>(shared_from_this());
	}

	void readAttr(AttrTypes_t attr, OTB::iterator& first, const OTB::iterator& last) override;
	void serializeAttr(PropWriteStream& propWriteStream) const override;

	bool canRemove() const override { return house.expired(); }

	uint32_t getSleeper() const { return sleeperGUID; }

	std::shared_ptr<House> getHouse() const { return house.lock(); }
	void setHouse(const std::shared_ptr<House>& house) { this->house = house; }

	bool canUse(const std::shared_ptr<Player>& player);

	bool trySleep(const std::shared_ptr<Player>& player);
	bool sleep(const std::shared_ptr<Player>& player);
	void wakeUp(const std::shared_ptr<Player>& player);

	std::shared_ptr<BedItem> getNextBedItem() const;

private:
	void updateAppearance(const std::shared_ptr<const Player>& player);
	void regeneratePlayer(const std::shared_ptr<Player>& player) const;
	void internalSetSleeper(const std::shared_ptr<const Player>& player);
	void internalRemoveSleeper();

	std::weak_ptr<House> house;
	uint64_t sleepStart;
	uint32_t sleeperGUID;
};

#endif // FS_BED_H
