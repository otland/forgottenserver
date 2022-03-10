// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

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
