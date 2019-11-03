/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019  Mark Samman <mark.samman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef FS_OUTFIT_H_C56E7A707E3F422C8C93D9BE09916AA3
#define FS_OUTFIT_H_C56E7A707E3F422C8C93D9BE09916AA3

#include "enums.h"

struct Outfit {
	Outfit(std::string name, uint16_t lookType, bool premium, bool unlocked) :
		name(std::move(name)), lookType(lookType), premium(premium), unlocked(unlocked) {}

	std::string name;
	uint16_t lookType;
	bool premium;
	bool unlocked;
};

struct ProtocolOutfit {
	ProtocolOutfit(const std::string& name, uint16_t lookType, uint8_t addons) :
		name(name), lookType(lookType), addons(addons) {}

	const std::string& name;
	uint16_t lookType;
	uint8_t addons;
};

class Outfits
{
	public:
		static Outfits& getInstance() {
			static Outfits instance;
			return instance;
		}

		bool loadFromXml();

		const Outfit* getOutfitByLookType(PlayerSex_t sex, uint16_t lookType) const;
		const std::vector<Outfit>& getOutfits(PlayerSex_t sex) const {
			return outfits[sex];
		}

	private:
		std::vector<Outfit> outfits[PLAYERSEX_LAST + 1];
};

#endif
