// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "networkmessage.h"

#include "container.h"
#include "podium.h"

#include <simdutf.h>

std::string NetworkMessage::getString(uint16_t stringLen /* = 0*/)
{
	if (stringLen == 0) {
		stringLen = get<uint16_t>();
	}

	if (!canRead(stringLen)) {
		return {};
	}

	auto it = reinterpret_cast<char*>(buffer.data() + info.position);
	info.position += stringLen;
	auto out = std::string(simdutf::utf8_length_from_latin1(it, stringLen), '\0');
	std::ignore = simdutf::convert_latin1_to_utf8(it, stringLen, out.data());
	return out;
}

Position NetworkMessage::getPosition()
{
	Position pos;
	pos.x = get<uint16_t>();
	pos.y = get<uint16_t>();
	pos.z = getByte();
	return pos;
}

void NetworkMessage::addString(std::string_view value)
{
	auto stringLen = simdutf::latin1_length_from_utf8(value.data(), value.size());
	if (!canAdd(stringLen + 2) || stringLen > 8192) {
		return;
	}

	add<uint16_t>(stringLen);
	auto it = reinterpret_cast<char*>(buffer.data() + info.position);
	std::ignore = simdutf::convert_utf8_to_latin1(value.data(), value.size(), it);
	info.position += stringLen;
	info.length += stringLen;
}

void NetworkMessage::addDouble(double value, uint8_t precision /* = 2*/)
{
	addByte(precision);
	add<uint32_t>(static_cast<uint32_t>((value * std::pow(static_cast<float>(10), precision)) +
	                                    std::numeric_limits<int32_t>::max()));
}

void NetworkMessage::addBytes(const char* bytes, size_t size)
{
	if (!canAdd(size) || size > 8192) {
		return;
	}

	std::memcpy(buffer.data() + info.position, bytes, size);
	info.position += size;
	info.length += size;
}

void NetworkMessage::addPaddingBytes(size_t n)
{
	if (!canAdd(n)) {
		return;
	}

	std::fill_n(buffer.data() + info.position, n, 0x33);
	info.length += n;
}

void NetworkMessage::addPosition(const Position& pos)
{
	add<uint16_t>(pos.x);
	add<uint16_t>(pos.y);
	addByte(pos.z);
}

void NetworkMessage::addItem(uint16_t id, uint8_t count)
{
	const ItemType& it = Item::items[id];

	add<uint16_t>(it.clientId);

	if (it.stackable) {
		addByte(count);
	} else if (it.isSplash() || it.isFluidContainer()) {
		addByte(fluidMap[count & 7]);
	} else if (it.isContainer()) {
		addByte(0x00); // assigned loot container icon
		addByte(0x00); // quiver ammo count
	} else if (it.classification > 0) {
		addByte(0x00); // item tier (0-10)
	} else if (it.showClientCharges) {
		add<uint32_t>(it.charges);
		addByte(0x00); // boolean (is brand new)
	} else if (it.showClientDuration) {
		add<uint32_t>(it.decayTimeMin);
		addByte(0x00); // boolean (is brand new)
	}

	if (it.isPodium()) {
		add<uint16_t>(0); // looktype
		add<uint16_t>(0); // lookmount
		addByte(2);       // direction
		addByte(0x01);    // is visible (bool)
	}
}

void NetworkMessage::addItem(const Item* item)
{
	const ItemType& it = Item::items[item->getID()];

	add<uint16_t>(it.clientId);

	if (it.stackable) {
		addByte(std::min<uint16_t>(0xFF, item->getItemCount()));
	} else if (it.isSplash() || it.isFluidContainer()) {
		addByte(fluidMap[item->getFluidType() & 7]);
	} else if (it.classification > 0) {
		addByte(0x00); // item tier (0-10)
	}

	if (it.showClientCharges) {
		add<uint32_t>(item->getCharges());
		addByte(0); // boolean (is brand new)
	} else if (it.showClientDuration) {
		add<uint32_t>(item->getDuration() / 1000);
		addByte(0); // boolean (is brand new)
	}

	if (it.isContainer()) {
		addByte(0x00); // assigned loot container icon
		// quiver ammo count
		const Container* container = item->getContainer();
		if (container && it.weaponType == WEAPON_QUIVER) {
			addByte(0x01);
			add<uint32_t>(container->getAmmoCount());
		} else {
			addByte(0x00);
		}
	}

	// display outfit on the podium
	if (it.isPodium()) {
		const Podium* podium = item->getPodium();
		const Outfit_t& outfit = podium->getOutfit();

		// add outfit
		if (podium->hasFlag(PODIUM_SHOW_OUTFIT)) {
			add<uint16_t>(outfit.lookType);
			if (outfit.lookType != 0) {
				addByte(outfit.lookHead);
				addByte(outfit.lookBody);
				addByte(outfit.lookLegs);
				addByte(outfit.lookFeet);
				addByte(outfit.lookAddons);
			}
		} else {
			add<uint16_t>(0);
		}

		// add mount
		if (podium->hasFlag(PODIUM_SHOW_MOUNT)) {
			add<uint16_t>(outfit.lookMount);
			if (outfit.lookMount != 0) {
				addByte(outfit.lookMountHead);
				addByte(outfit.lookMountBody);
				addByte(outfit.lookMountLegs);
				addByte(outfit.lookMountFeet);
			}
		} else {
			add<uint16_t>(0);
		}

		addByte(podium->getDirection());
		addByte(podium->hasFlag(PODIUM_SHOW_PLATFORM) ? 0x01 : 0x00);
		return;
	}
}

void NetworkMessage::addItemId(uint16_t itemId) { add<uint16_t>(Item::items[itemId].clientId); }
