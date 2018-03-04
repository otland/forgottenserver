/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2018  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_POSITION_H_5B684192F7034FB8857C8280D2CC6C75
#define FS_POSITION_H_5B684192F7034FB8857C8280D2CC6C75

enum Direction : uint8_t {
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

struct Position
{
	constexpr Position() = default;
	constexpr Position(uint16_t x, uint16_t y, uint8_t z) : x(x), y(y), z(z) {}

	template<int_fast32_t deltax, int_fast32_t deltay>
	static bool areInRange(const Position& p1, const Position& p2) {
		return Position::getDistanceX(p1, p2) <= deltax && Position::getDistanceY(p1, p2) <= deltay;
	}

	template<int_fast32_t deltax, int_fast32_t deltay, int_fast16_t deltaz>
	static bool areInRange(const Position& p1, const Position& p2) {
		return Position::getDistanceX(p1, p2) <= deltax && Position::getDistanceY(p1, p2) <= deltay && Position::getDistanceZ(p1, p2) <= deltaz;
	}

	static int_fast32_t getOffsetX(const Position& p1, const Position& p2) {
		return p1.getX() - p2.getX();
	}
	static int_fast32_t getOffsetY(const Position& p1, const Position& p2) {
		return p1.getY() - p2.getY();
	}
	static int_fast16_t getOffsetZ(const Position& p1, const Position& p2) {
		return p1.getZ() - p2.getZ();
	}

	static int32_t getDistanceX(const Position& p1, const Position& p2) {
		return std::abs(Position::getOffsetX(p1, p2));
	}
	static int32_t getDistanceY(const Position& p1, const Position& p2) {
		return std::abs(Position::getOffsetY(p1, p2));
	}
	static int16_t getDistanceZ(const Position& p1, const Position& p2) {
		return std::abs(Position::getOffsetZ(p1, p2));
	}

	uint16_t x = 0;
	uint16_t y = 0;
	uint8_t z = 0;

	bool operator<(const Position& p) const {
		if (z < p.z) {
			return true;
		}

		if (z > p.z) {
			return false;
		}

		if (y < p.y) {
			return true;
		}

		if (y > p.y) {
			return false;
		}

		if (x < p.x) {
			return true;
		}

		if (x > p.x) {
			return false;
		}

		return false;
	}

	bool operator>(const Position& p) const {
		return ! (*this < p);
	}

	bool operator==(const Position& p) const {
		return p.x == x && p.y == y && p.z == z;
	}

	bool operator!=(const Position& p) const {
		return p.x != x || p.y != y || p.z != z;
	}

	Position operator+(const Position& p1) const {
		return Position(x + p1.x, y + p1.y, z + p1.z);
	}

	Position operator-(const Position& p1) const {
		return Position(x - p1.x, y - p1.y, z - p1.z);
	}

	int_fast32_t getX() const { return x; }
	int_fast32_t getY() const { return y; }
	int_fast16_t getZ() const { return z; }
};

std::ostream& operator<<(std::ostream&, const Position&);
std::ostream& operator<<(std::ostream&, const Direction&);

#endif
