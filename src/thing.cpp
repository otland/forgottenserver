// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "thing.h"

#include "tile.h"

class VirtualThing final : public Thing
{
public:
	ReturnValue queryAdd(int32_t, const std::shared_ptr<const Thing>&, uint32_t, uint32_t,
	                     const std::shared_ptr<Creature>& = nullptr) const override
	{
		return RETURNVALUE_NOTPOSSIBLE;
	}
	ReturnValue queryMaxCount(int32_t, const std::shared_ptr<const Thing>&, uint32_t, uint32_t&,
	                          uint32_t) const override
	{
		return RETURNVALUE_NOTPOSSIBLE;
	}
	ReturnValue queryRemove(const std::shared_ptr<const Thing>&, uint32_t, uint32_t,
	                        const std::shared_ptr<Creature>& = nullptr) const override
	{
		return RETURNVALUE_NOTPOSSIBLE;
	}
	std::shared_ptr<Thing> queryDestination(int32_t&, const std::shared_ptr<const Thing>&, std::shared_ptr<Item>&,
	                                        uint32_t&) override
	{
		return nullptr;
	}

	void addThing(const std::shared_ptr<Thing>&) override {}
	void addThing(int32_t, const std::shared_ptr<Thing>&) override {}
	void updateThing(const std::shared_ptr<Thing>&, uint16_t, uint32_t) override {}
	void replaceThing(uint32_t, const std::shared_ptr<Thing>&) override {}
	void removeThing(const std::shared_ptr<Thing>&, uint32_t) override {}

	void postAddNotification(const std::shared_ptr<Thing>&, const std::shared_ptr<const Thing>&, int32_t,
	                         cylinderlink_t = LINK_OWNER) override
	{}
	void postRemoveNotification(const std::shared_ptr<Thing>&, const std::shared_ptr<const Thing>&, int32_t,
	                            cylinderlink_t = LINK_OWNER) override
	{}
};

const std::shared_ptr<Thing> Thing::virtualThing = std::make_shared<VirtualThing>();

const Position& Thing::getPosition() const
{
	const auto& tile = getTile();
	if (!tile) {
		return Tile::nullptrTile->getPosition();
	}
	return tile->getPosition();
}
