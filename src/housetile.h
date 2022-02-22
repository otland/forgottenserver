// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_HOUSETILE_H_57D59BEC1CE741D9B142BFC54634505B
#define FS_HOUSETILE_H_57D59BEC1CE741D9B142BFC54634505B

#include "tile.h"

class House;

class HouseTile final : public DynamicTile
{
	public:
		HouseTile(int32_t x, int32_t y, int32_t z, House* house);

		//cylinder implementations
		ReturnValue queryAdd(int32_t index, const Thing& thing, uint32_t count,
				uint32_t flags, Creature* actor = nullptr) const override;

		Tile* queryDestination(int32_t& index, const Thing& thing, Item** destItem,
				uint32_t& flags) override;

		ReturnValue queryRemove(const Thing& thing, uint32_t count, uint32_t flags, Creature* actor = nullptr) const override;

		void addThing(int32_t index, Thing* thing) override;
		void internalAddThing(uint32_t index, Thing* thing) override;

		House* getHouse() const {
			return house;
		}

	private:
		void updateHouse(Item* item);

		House* house;
};

#endif
