// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_POSITION_H
#define FS_POSITION_H

enum Direction : uint8_t
{
	DIRECTION_NORTH = 0,
	DIRECTION_EAST = 1,
	DIRECTION_SOUTH = 2,
	DIRECTION_WEST = 3,

	DIRECTION_DIAGONAL_MASK = 4,
	DIRECTION_SOUTHWEST = DIRECTION_DIAGONAL_MASK | 0,
	DIRECTION_SOUTHEAST = DIRECTION_DIAGONAL_MASK | 1,
	DIRECTION_NORTHWEST = DIRECTION_DIAGONAL_MASK | 2,
	DIRECTION_NORTHEAST = DIRECTION_DIAGONAL_MASK | 3,

	DIRECTION_LAST = DIRECTION_NORTHEAST,
	DIRECTION_NONE = 8,
};

namespace tfs {

inline constexpr auto abs(std::integral auto num) { return num < 0 ? -num : num; }

} // namespace tfs

struct Position
{
	constexpr Position() = default;
	constexpr Position(uint16_t x, uint16_t y, uint8_t z) : x(x), y(y), z(z) {}

	constexpr bool isInRange(const Position& p, int32_t deltax, int32_t deltay) const
	{
		return getDistanceX(p) <= deltax && getDistanceY(p) <= deltay;
	}

	constexpr bool isInRange(const Position& p, int32_t deltax, int32_t deltay, int16_t deltaz) const
	{
		return getDistanceX(p) <= deltax && getDistanceY(p) <= deltay && getDistanceZ(p) <= deltaz;
	}

	constexpr int32_t getOffsetX(const Position& p) const { return getX() - p.getX(); }
	constexpr int32_t getOffsetY(const Position& p) const { return getY() - p.getY(); }
	constexpr int16_t getOffsetZ(const Position& p) const { return getZ() - p.getZ(); }

	constexpr int32_t getDistanceX(const Position& p) const { return tfs::abs(getOffsetX(p)); }
	constexpr int32_t getDistanceY(const Position& p) const { return tfs::abs(getOffsetY(p)); }
	constexpr int16_t getDistanceZ(const Position& p) const { return tfs::abs(getOffsetZ(p)); }

	uint16_t x = 0;
	uint16_t y = 0;
	uint8_t z = 0;

	constexpr bool operator==(const Position& p) const { return std::tie(x, y, z) == std::tie(p.x, p.y, p.z); }
	constexpr bool operator!=(const Position& p) const { return std::tie(x, y, z) != std::tie(p.x, p.y, p.z); }
	constexpr bool operator<(const Position& p) const { return std::tie(z, y, x) < std::tie(p.z, p.y, p.x); }

	constexpr int32_t getX() const { return x; }
	constexpr int32_t getY() const { return y; }
	constexpr int16_t getZ() const { return z; }
};

std::ostream& operator<<(std::ostream&, const Position&);

#endif // FS_POSITION_H
