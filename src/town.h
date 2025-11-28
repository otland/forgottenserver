// Copyright 2025 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_TOWN_H
#define FS_TOWN_H

#include "position.h"

struct Town
{
	const uint32_t id;
	std::string name;
	Position templePosition;
};

using TownMap = std::map<uint32_t, std::unique_ptr<Town>>;

class Towns
{
public:
	void setTown(uint32_t townId, Town* town) { townMap[townId].reset(town); }

	const Town* getTown(uint32_t townId) const
	{
		auto it = townMap.find(townId);
		if (it == townMap.end()) {
			return nullptr;
		}
		return it->second.get();
	}

	const TownMap& getTowns() const { return townMap; }

private:
	TownMap townMap;
};

#endif // FS_TOWN_H
