/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2015  Mark Samman <mark.samman@gmail.com>
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

#include "pugicast.h"
#include "tools.h"

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
		printXMLError("Error - Mounts::loadFromXml", "data/XML/mounts.xml", result);
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
	return nullptr;
}

Mount* Mounts::getMountByClientID(uint16_t clientId)
{
	for (Mount& mount : mounts) {
		if (mount.clientId == clientId) {
			return &mount;
		}
	}
	return nullptr;
}
