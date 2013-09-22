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

#include <iostream>

#include "ext/pugixml.hpp"
#include "pugicast.h"

#include "groups.h"

bool Groups::load()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/XML/groups.xml");
	if (!result) {
		std::cout << "[Error - Groups::load] Failed to load data/XML/groups.xml: " << result.description() << std::endl;
		return false;
	}

	for (pugi::xml_node groupNode = doc.child("groups").first_child(); groupNode; groupNode = groupNode.next_sibling()) {
		Group group;
		group.id = pugi::cast<uint32_t>(groupNode.attribute("id").value());
		group.name = groupNode.attribute("name").as_string();
		group.flags = pugi::cast<uint64_t>(groupNode.attribute("flags").value());
		group.access = groupNode.attribute("access").as_bool();
		group.maxDepotItems = pugi::cast<uint32_t>(groupNode.attribute("maxdepotitems").value());
		group.maxVipEntries = pugi::cast<uint32_t>(groupNode.attribute("maxvipentries").value());
		groups.push_back(group);
	}
	return true;
}

Group* Groups::getGroup(uint32_t id)
{
	for (Group& group : groups) {
		if (group.id == id) {
			return &group;
		}
	}
	return nullptr;
}
