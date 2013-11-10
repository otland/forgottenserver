/**
 * The Forgotten Server - a server application for the MMORPG Tibia
 * Copyright (C) 2013  Mark Samman <mark.samman@gmail.com>
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

#ifndef __OTSERV_POS_H
#define __OTSERV_POS_H

#include <cstdlib>

enum Direction {
	NORTH = 0,
	EAST = 1,
	SOUTH = 2,
	WEST = 3,
	SOUTHWEST = 4,
	SOUTHEAST = 5,
	NORTHWEST = 6,
	NORTHEAST = 7,
	SOUTH_ALT = 8,
	EAST_ALT = 9,
	NODIR = 10
};

struct Position
{
	Position() : x(0), y(0), z(0) {}
	Position(uint16_t _x, uint16_t _y, uint8_t _z)
		: x(_x), y(_y), z(_z) {}
	~Position() {}

	template<int_fast32_t deltax, int_fast32_t deltay>
	inline static bool areInRange(const Position& p1, const Position& p2) {
		return Position::getDistanceX(p1, p2) <= deltax && Position::getDistanceY(p1, p2) <= deltay;
	}

	template<int_fast32_t deltax, int_fast32_t deltay, int_fast16_t deltaz>
	inline static bool areInRange(const Position& p1, const Position& p2) {
		return Position::getDistanceX(p1, p2) <= deltax && Position::getDistanceY(p1, p2) <= deltay && Position::getDistanceZ(p1, p2) <= deltaz;
	}

	inline static int_fast32_t getOffsetX(const Position& p1, const Position& p2) {
		return p1.getX() - p2.getX();
	}
	inline static int_fast32_t getOffsetY(const Position& p1, const Position& p2) {
		return p1.getY() - p2.getY();
	}
	inline static int_fast16_t getOffsetZ(const Position& p1, const Position& p2) {
		return p1.getZ() - p2.getZ();
	}

	inline static int32_t getDistanceX(const Position& p1, const Position& p2) {
		return std::abs(Position::getOffsetX(p1, p2));
	}
	inline static int32_t getDistanceY(const Position& p1, const Position& p2) {
		return std::abs(Position::getOffsetY(p1, p2));
	}
	inline static int16_t getDistanceZ(const Position& p1, const Position& p2) {
		return std::abs(Position::getOffsetZ(p1, p2));
	}

	uint16_t x;
	uint16_t y;
	uint8_t z;

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

	inline int_fast32_t getX() const { return x; }
	inline int_fast32_t getY() const { return y; }
	inline int_fast16_t getZ() const { return z; }
};

std::ostream& operator<<(std::ostream&, const Position&);
std::ostream& operator<<(std::ostream&, const Direction&);

struct PositionEx : public Position
{
	PositionEx() {}
	~PositionEx() {}

	PositionEx(uint16_t _x, uint16_t _y, uint8_t _z, int32_t _stackpos)
		: Position(_x, _y, _z), stackpos(_stackpos) {}

	PositionEx(uint16_t _x, uint16_t _y, uint8_t _z)
		: Position(_x, _y, _z), stackpos(0) {}

	PositionEx(const Position& p)
		: Position(p.x, p.y, p.z), stackpos(0) {}

	PositionEx(const PositionEx& p)
		: Position(p.x, p.y, p.z), stackpos(p.stackpos) {}

	PositionEx(const Position& p, int32_t _stackpos)
		: Position(p.x, p.y, p.z), stackpos(_stackpos) {}

	int32_t stackpos;

	bool operator==(const PositionEx& p)  const {
		return p.x == x && p.y == y && p.z == z && p.stackpos == stackpos;
	}

	bool operator!=(const PositionEx& p)  const {
		return p.x != x || p.y != y || p.z != z || p.stackpos != stackpos;
	}
};

#endif
