// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_PODIUM_H
#define FS_PODIUM_H

#include "item.h"

class Podium final : public Item
{
public:
	explicit Podium(uint16_t type) : Item(type) {};

	std::shared_ptr<Podium> getPodium() override { return std::static_pointer_cast<Podium>(getItem()); }
	std::shared_ptr<const Podium> getPodium() const override
	{
		return std::static_pointer_cast<const Podium>(getItem());
	}

	Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream) override;
	void serializeAttr(PropWriteStream& propWriteStream) const override;

	void setOutfit(const Outfit_t& newOutfit) { outfit = newOutfit; }
	const Outfit_t getOutfit() const { return outfit; }

	bool hasFlag(PodiumFlags flag) const { return flags.test(flag); }
	void setFlagValue(PodiumFlags flag, bool value)
	{
		if (value) {
			flags.set(flag);
		} else {
			flags.reset(flag);
		}
	}
	void setFlags(uint8_t newFlags) { flags = newFlags; }

	Direction getDirection() const { return direction; }
	void setDirection(Direction newDirection) { direction = newDirection; }

protected:
	Outfit_t outfit;

private:
	std::bitset<3> flags = {true}; // show platform only
	Direction direction = DIRECTION_SOUTH;
};

#endif // FS_PODIUM_H
