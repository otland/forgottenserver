/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
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

#include "otpch.h"

#include "cylinder.h"

VirtualCylinder* VirtualCylinder::virtualCylinder = new VirtualCylinder;

int32_t Cylinder::__getIndexOfThing(const Thing* thing) const
{
	return -1;
}

int32_t Cylinder::__getFirstIndex() const
{
	return -1;
}

int32_t Cylinder::__getLastIndex() const
{
	return -1;
}

uint32_t Cylinder::__getItemTypeCount(uint16_t itemId, int32_t subType /*= -1*/) const
{
	return 0;
}

std::map<uint32_t, uint32_t>& Cylinder::__getAllItemTypeCount(std::map<uint32_t, uint32_t>& countMap) const
{
	return countMap;
}

Thing* Cylinder::__getThing(uint32_t index) const
{
	return nullptr;
}

void Cylinder::__internalAddThing(Thing* thing)
{
	//
}

void Cylinder::__internalAddThing(uint32_t index, Thing* thing)
{
	//
}

void Cylinder::__startDecaying()
{
	//
}
