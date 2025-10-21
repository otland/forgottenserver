// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_HOUSETILE_H
#define FS_HOUSETILE_H

#include "tile.h"

class House;

class HouseTile final : public DynamicTile
{
public:
	HouseTile(int32_t x, int32_t y, int32_t z, House* house);

	using DynamicTile::internalAddThing;

	// cylinder implementations
	ReturnValue queryAdd(int32_t index, std::shared_ptr<const Thing> thing, uint32_t count, uint32_t flags,
	                     std::shared_ptr<Creature> actor = nullptr) const override;

	std::shared_ptr<Cylinder> queryDestination(int32_t& index, std::shared_ptr<const Thing> thing,
	                                           std::shared_ptr<Item>& destItem, uint32_t& flags) override;

	ReturnValue queryRemove(std::shared_ptr<const Thing> thing, uint32_t count, uint32_t flags,
	                        std::shared_ptr<Creature> actor = nullptr) const override;

	void addThing(int32_t index, std::shared_ptr<Thing> thing) override;
	void internalAddThing(uint32_t index, std::shared_ptr<Thing> thing) override;

	House* getHouse() const { return house; }

private:
	void updateHouse(std::shared_ptr<Item> item);

	House* house;
};

#endif // FS_HOUSETILE_H
