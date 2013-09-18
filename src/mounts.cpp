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

#include "otpch.h"

#include "mounts.h"

#include "pugixml.hpp"
#include "pugicast.h"

Mount::Mount(uint8_t _id, uint16_t _clientId, const std::string& _name, int32_t _speed, bool _premium)
{
	id = _id;
	clientId = _clientId;
	name = _name;
	speed = _speed;
	premium = _premium;
}

bool Mount::isTamed(Player* player) const
{
	if (!player) {
		return false;
	}

	if (player->isAccessPlayer()) {
		return true;
	}

	if (premium && !player->isPremium()) {
		return false;
	}

	uint8_t tmpId = id - 1;

	int32_t value;
	if (!player->getStorageValue(PSTRG_MOUNTS_RANGE_START + (tmpId / 31), value)) {
		return false;
	}

	return ((1 << (tmpId % 31)) & value) != 0;
}

bool Mounts::reload()
{
	mounts.clear();
	return loadFromXml();
}

bool Mounts::loadFromXml()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/XML/mounts.xml");
	if (!result) {
		std::cout << "[Error - Mounts::loadFromXml] Failed to load data/XML/mounts.xml: " << result.description() << std::endl;
		return false;
	}

	for (pugi::xml_node mountNode = doc.child("mounts").first_child(); mountNode; mountNode = mountNode.next_sibling()) {
		mounts.emplace_back(
			static_cast<uint8_t>(pugi::cast<uint16_t>(mountNode.attribute("id").value())),
			pugi::cast<uint16_t>(mountNode.attribute("clientid").value()),
			mountNode.attribute("name").as_string(),
			pugi::cast<int32_t>(mountNode.attribute("speed").value()),
			mountNode.attribute("premium").as_bool()
		);
	}
	return true;
}

Mount* Mounts::getMountByID(uint8_t id)
{
	for (Mount& mount : mounts) {
		if (mount.id == id) {
			return &mount;
		}
	}
	return NULL;
}

Mount* Mounts::getMountByClientID(uint16_t clientId)
{
	for (Mount& mount : mounts) {
		if (mount.clientId == clientId) {
			return &mount;
		}
	}
	return NULL;
}
