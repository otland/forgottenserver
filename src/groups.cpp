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

#include "groups.h"

#include "tools.h"

bool Groups::load()
{
	xmlDocPtr doc = xmlParseFile("data/XML/groups.xml");
	if (!doc) {
		return false;
	}

	xmlNodePtr root = xmlDocGetRootElement(doc);
	if (xmlStrcmp(root->name, (const xmlChar*)"groups") != 0) {
		xmlFreeDoc(doc);
		return false;
	}

	xmlNodePtr p = root->children;
	while (p) {
		if (xmlStrcmp(p->name, (const xmlChar*)"group") == 0) {
			Group group;
			group.id = readXMLValue<uint32_t>(p, "id");
			readXMLString(p, "name", group.name);
			group.flags = readXMLValue<uint64_t>(p, "flags");
			group.maxDepotItems = readXMLValue<uint32_t>(p, "maxdepotitems");
			group.maxVipEntries = readXMLValue<uint32_t>(p, "maxvipentries");
			groups.push_back(group);
		}
		p = p->next;
	}

	xmlFreeDoc(doc);
	return true;
}

Group* Groups::getGroup(uint32_t id)
{
	for (Group& group : groups) {
		if (group.id == id) {
			return &group;
		}
	}
	return NULL;
}
