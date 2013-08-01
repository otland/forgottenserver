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

#ifndef __OTSERV_OUTFIT_H__
#define __OTSERV_OUTFIT_H__

#include <list>
#include <vector>
#include <string>
#include <map>
#include "enums.h"

struct Outfit {
	uint32_t looktype;
	uint32_t addons;
	bool premium;
};

typedef std::list<Outfit*> OutfitListType;

class OutfitList
{
	public:
		OutfitList();
		~OutfitList();

		void addOutfit(const Outfit& outfit);
		bool remOutfit(const Outfit& outfit);
		const OutfitListType& getOutfits() const {
			return m_list;
		}
		bool isInList(uint32_t looktype, uint32_t addons, bool playerPremium, int32_t playerSex) const;

	private:
		OutfitListType m_list;
};

class Outfits
{
	public:
		~Outfits();

		static Outfits* getInstance() {
			static Outfits instance;
			return &instance;
		}

		bool loadFromXml();
		const OutfitListType& getOutfits(uint32_t type) {
			return getOutfitList(type).getOutfits();
		}

		const OutfitList& getOutfitList(uint32_t type) {
			if (type < m_list.size()) {
				return *m_list[type];
			} else {
				if (type == PLAYERSEX_FEMALE) {
					return m_female_list;
				} else {
					return m_male_list;
				}
			}
		}

		const std::string& getOutfitName(uint32_t looktype) const {
			std::map<uint32_t, std::string>::const_iterator it;
			it = outfitNamesMap.find(looktype);

			if (it != outfitNamesMap.end()) {
				return it->second;
			} else {
				static const std::string d = "Outfit";
				return d;
			}
		}

	private:
		Outfits();
		typedef std::vector<OutfitList*> OutfitsListVector;
		OutfitsListVector m_list;

		std::map<uint32_t, std::string> outfitNamesMap;

		OutfitList m_female_list;
		OutfitList m_male_list;
};

#endif
