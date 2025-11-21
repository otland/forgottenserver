// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_HOUSETILE_H
#define FS_HOUSETILE_H

#include "tile.h"

class House;

class HouseTile final : public DynamicTile
{
public:
	HouseTile(uint16_t x, uint16_t y, uint8_t z, House* house) : DynamicTile{x, y, z}, house{house} {}

	std::shared_ptr<HouseTile> getHouseTile() override
	{
		return std::static_pointer_cast<HouseTile>(shared_from_this());
	}
	std::shared_ptr<const HouseTile> getHouseTile() const override
	{
		return std::static_pointer_cast<const HouseTile>(shared_from_this());
	}

	ReturnValue queryAdd(int32_t index, const std::shared_ptr<const Thing>& thing, uint32_t count, uint32_t flags,
	                     const std::shared_ptr<Creature>& actor = nullptr) const override;

	std::shared_ptr<Thing> queryDestination(int32_t& index, const std::shared_ptr<const Thing>& thing,
	                                        std::shared_ptr<Item>& destItem, uint32_t& flags) override;

	ReturnValue queryRemove(const std::shared_ptr<const Thing>& thing, uint32_t count, uint32_t flags,
	                        const std::shared_ptr<Creature>& actor = nullptr) const override;

	void addThing(int32_t index, const std::shared_ptr<Thing>& thing) override;
	void internalAddThing(uint32_t index, const std::shared_ptr<Thing>& thing) override;

	House* getHouse() const { return house; }

private:
	void updateHouse(const std::shared_ptr<Item>& item);

	House* house;
};

#endif // FS_HOUSETILE_H
