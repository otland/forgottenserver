// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "podium.h"

#include "game.h"

extern Game g_game;

Attr_ReadValue Podium::readAttr(AttrTypes_t attr, PropStream& propStream)
{
	switch (attr) {
		case ATTR_PODIUMOUTFIT: {
			if (propStream.size() < 15) {
				return ATTR_READ_ERROR;
			}

			uint8_t flags;
			propStream.read<uint8_t>(flags);
			setFlags(flags);

			uint8_t newDirection;
			propStream.read<uint8_t>(newDirection);
			setDirection(static_cast<Direction>(newDirection));

			Outfit_t newOutfit;
			propStream.read<uint16_t>(newOutfit.lookType);
			propStream.read<uint8_t>(newOutfit.lookHead);
			propStream.read<uint8_t>(newOutfit.lookBody);
			propStream.read<uint8_t>(newOutfit.lookLegs);
			propStream.read<uint8_t>(newOutfit.lookFeet);
			propStream.read<uint8_t>(newOutfit.lookAddons);
			propStream.read<uint16_t>(newOutfit.lookMount);
			propStream.read<uint8_t>(newOutfit.lookMountHead);
			propStream.read<uint8_t>(newOutfit.lookMountBody);
			propStream.read<uint8_t>(newOutfit.lookMountLegs);
			propStream.read<uint8_t>(newOutfit.lookMountFeet);
			setOutfit(newOutfit);

			g_game.updatePodium(this);
			return ATTR_READ_CONTINUE;
		}

		default:
			break;
	}
	return Item::readAttr(attr, propStream);
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
