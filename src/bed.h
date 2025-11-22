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

	Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream) override;
	void serializeAttr(PropWriteStream& propWriteStream) const override;

	bool canRemove() const override { return !house; }

	uint32_t getSleeper() const { return sleeperGUID; }

	House* getHouse() const { return house; }
	void setHouse(House* h) { house = h; }

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

	House* house = nullptr;
	uint64_t sleepStart;
	uint32_t sleeperGUID;
};

#endif // FS_BED_H
