// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "podium.h"

#include "game.h"

extern Game g_game;

void Podium::readAttr(AttrTypes_t attr, OTB::iterator& first, const OTB::iterator& last)
{
	switch (attr) {
		case ATTR_PODIUMOUTFIT: {
			if (last - first < 15) [[unlikely]] {
				throw std::invalid_argument("Invalid podium outfit");
			}

			auto flags = OTB::read<uint8_t>(first, last);
			setFlags(flags);

			auto newDirection = OTB::read<uint8_t>(first, last);
			setDirection(static_cast<Direction>(newDirection));

			setOutfit({.lookType = OTB::read<uint16_t>(first, last),
			           .lookHead = OTB::read<uint8_t>(first, last),
			           .lookBody = OTB::read<uint8_t>(first, last),
			           .lookLegs = OTB::read<uint8_t>(first, last),
			           .lookFeet = OTB::read<uint8_t>(first, last),
			           .lookAddons = OTB::read<uint8_t>(first, last),
			           .lookMount = OTB::read<uint16_t>(first, last),
			           .lookMountHead = OTB::read<uint8_t>(first, last),
			           .lookMountBody = OTB::read<uint8_t>(first, last),
			           .lookMountLegs = OTB::read<uint8_t>(first, last),
			           .lookMountFeet = OTB::read<uint8_t>(first, last)});

			g_game.updatePodium(this);
			break;
		}

		default:
			Item::readAttr(attr, first, last);
			break;
	}
}

void Podium::serializeAttr(PropWriteStream& propWriteStream) const
{
	if (ATTR_PODIUMOUTFIT != 0) {
		propWriteStream.write<uint8_t>(ATTR_PODIUMOUTFIT);
		propWriteStream.write<uint8_t>(static_cast<uint8_t>(flags.to_ulong()));
		propWriteStream.write<uint8_t>(direction);
		propWriteStream.write<uint16_t>(outfit.lookType);
		propWriteStream.write<uint8_t>(outfit.lookHead);
		propWriteStream.write<uint8_t>(outfit.lookBody);
		propWriteStream.write<uint8_t>(outfit.lookLegs);
		propWriteStream.write<uint8_t>(outfit.lookFeet);
		propWriteStream.write<uint8_t>(outfit.lookAddons);
		propWriteStream.write<uint16_t>(outfit.lookMount);
		propWriteStream.write<uint8_t>(outfit.lookMountHead);
		propWriteStream.write<uint8_t>(outfit.lookMountBody);
		propWriteStream.write<uint8_t>(outfit.lookMountLegs);
		propWriteStream.write<uint8_t>(outfit.lookMountFeet);
	}
}
