// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "thing.h"
#include "tile.h"

const Position& Thing::getPosition() const
{
	const Tile* tile = getTile();
	if (!tile) {
		return Tile::nullptr_tile.getPosition();
	}
	return tile->getPosition();
}

Tile* Thing::getTile()
{
	return dynamic_cast<Tile*>(this);
}

const Tile* Thing::getTile() const
{
	return dynamic_cast<const Tile*>(this);
}
