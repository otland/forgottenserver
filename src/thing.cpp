// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "thing.h"

#include "tile.h"

const Position& Thing::getPosition() const
{
	const auto& tile = getTile();
	if (!tile) {
		return Tile::nullptrTile->getPosition();
	}
	return tile->getPosition();
}
