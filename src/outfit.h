// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_OUTFIT_H
#define FS_OUTFIT_H

#include "enums.h"

struct Outfit
{
	Outfit(std::string name, uint16_t lookType, bool premium, bool unlocked) :
	    name(std::move(name)), lookType(lookType), premium(premium), unlocked(unlocked)
	{}

	bool operator==(const Outfit& otherOutfit) const
	{
		return name == otherOutfit.name && lookType == otherOutfit.lookType && premium == otherOutfit.premium &&
		       unlocked == otherOutfit.unlocked;
	}

	std::string name;
	uint16_t lookType;
	bool premium;
	bool unlocked;
};

struct ProtocolOutfit
{
	ProtocolOutfit(const std::string& name, uint16_t lookType, uint8_t addons) :
	    name(name), lookType(lookType), addons(addons)
	{}

	const std::string& name;
	uint16_t lookType;
	uint8_t addons;
};

class Outfits
{
public:
	static Outfits& getInstance()
	{
		static Outfits instance;
		return instance;
	}

	bool loadFromXml();

	const Outfit* getOutfitByLookType(PlayerSex_t sex, uint16_t lookType) const;
	const Outfit* getOutfitByLookType(uint16_t lookType) const;
	const std::vector<Outfit>& getOutfits(PlayerSex_t sex) const { return outfits[sex]; }

private:
	std::vector<Outfit> outfits[PLAYERSEX_LAST + 1];
};

#endif // FS_OUTFIT_H
