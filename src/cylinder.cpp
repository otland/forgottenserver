/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2016  Mark Samman <mark.samman@gmail.com>
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

int32_t Cylinder::getThingIndex(const Thing*) const
{
	return -1;
}

size_t Cylinder::getFirstIndex() const
{
	return 0;
}

size_t Cylinder::getLastIndex() const
{
	return 0;
}

uint32_t Cylinder::getItemTypeCount(uint16_t, int32_t) const
{
	return 0;
}

std::map<uint32_t, uint32_t>& Cylinder::getAllItemTypeCount(std::map<uint32_t, uint32_t>& countMap) const
{
	return countMap;
}

Thing* Cylinder::getThing(size_t) const
{
	return nullptr;
}

void Cylinder::internalAddThing(Thing*)
{
	//
}

void Cylinder::internalAddThing(uint32_t, Thing*)
{
	//
}

void Cylinder::startDecaying()
{
	//
}
