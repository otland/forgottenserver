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

#include "definitions.h"
#include "outfit.h"

#include "pugixml.hpp"
#include "pugicast.h"

#include "creature.h"
#include "player.h"
#include "tools.h"

OutfitList::OutfitList()
{
	//
}

OutfitList::~OutfitList()
{
	for (Outfit* outfit : m_list) {
		delete outfit;
	}
	m_list.clear();
}

void OutfitList::addOutfit(const Outfit& outfit)
{
	for (OutfitListType::iterator it = m_list.begin(), end = m_list.end(); it != end; ++it) {
		if ((*it)->looktype == outfit.looktype) {
			(*it)->addons |= outfit.addons;
			return;
		}
	}

	//adding a new outfit
	Outfit* new_outfit = new Outfit;
	new_outfit->looktype = outfit.looktype;
	new_outfit->addons = outfit.addons;
	new_outfit->premium = outfit.premium;
	m_list.push_back(new_outfit);
}

bool OutfitList::remOutfit(const Outfit& outfit)
{
	OutfitListType::iterator it;

	for (it = m_list.begin(); it != m_list.end(); ++it) {
		if ((*it)->looktype == outfit.looktype) {
			if (outfit.addons == 0xFF) {
				delete *it;
				m_list.erase(it);
			} else {
				(*it)->addons = (*it)->addons & (~outfit.addons);
			}

			return true;
		}
	}

	return false;
}

bool OutfitList::isInList(uint32_t looktype, uint32_t addons, bool playerPremium, int32_t playerSex) const
{
	OutfitListType::const_iterator it, it_;
	const OutfitListType& global_outfits = Outfits::getInstance()->getOutfits(playerSex);

	for (it = global_outfits.begin(); it != global_outfits.end(); ++it) {
		if ((*it)->looktype == looktype) {
			for (Outfit* outfit : m_list) {
				if (outfit->looktype == looktype) {
					if ((outfit->addons & addons) == addons) {
						if (((*it)->premium && playerPremium) || !(*it)->premium) {
							return true;
						}
					}
					return false;
				}
			}

			return false;
		}
	}

	return false;
}

Outfits::Outfits()
{
	m_list.resize(10, NULL);
}

Outfits::~Outfits()
{
	for (OutfitsListVector::iterator it = m_list.begin(), end = m_list.end(); it != end; ++it) {
		delete *it;
	}
	m_list.clear();
}

bool Outfits::loadFromXml()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/XML/outfits.xml");
	if (!result) {
		std::cout << "[Error - Outfits::loadFromXml] Failed to load data/XML/outfits.xml: " << result.description() << std::endl;
		return false;
	}

	for (pugi::xml_node outfitNode = doc.child("outfits").first_child(); outfitNode; outfitNode = outfitNode.next_sibling()) {
		pugi::xml_attribute typeAttribute = outfitNode.attribute("type");
		if (!typeAttribute) {
			std::cout << "[Warning - Outfits::loadFromXml] Missing outfit type." << std::endl;
			continue;
		}

		uint32_t type = pugi::cast<uint32_t>(typeAttribute.value());
		if (type > 9) {
			std::cout << "[Warning - Outfits::loadFromXml] No valid outfit type " << type << std::endl;
			continue;
		}

		if (!m_list[type]) {
			m_list[type] = new OutfitList;
		}
		OutfitList* list = m_list[type];

		Outfit outfit;
		std::string outfitName = outfitNode.attribute("name").as_string();

		pugi::xml_attribute lookTypeAttribute = outfitNode.attribute("looktype");
		if (!lookTypeAttribute) {
			std::cout << "[Warning - Outfits::loadFromXml] Missing looktype on outfit: " << outfitName << std::endl;
			continue;
		}

		outfit.looktype = pugi::cast<uint32_t>(lookTypeAttribute.value());
		outfit.addons = pugi::cast<uint32_t>(outfitNode.attribute("addons").value());
		outfit.premium = outfitNode.attribute("premium").as_bool();

		outfitNamesMap[outfit.looktype] = outfitName;

		if (outfitNode.attribute("enabled").as_bool()) {
			//This way you can add names for outfits without adding them to default list
			list->addOutfit(outfit);
		}
	}
	return true;
}
