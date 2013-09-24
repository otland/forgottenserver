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

#ifndef __OTSERV_VOCATION_H__
#define __OTSERV_VOCATION_H__

#include "enums.h"
#include <string>
#include <map>
#include "item.h"

class Vocation
{
	public:
		Vocation() {} // TODO: Remove this constructor when we can use emplace
		Vocation(uint16_t id);

		const std::string& getVocName() const {
			return name;
		}
		const std::string& getVocDescription() const {
			return description;
		}
		uint32_t getReqSkillTries(int32_t skill, int32_t level);
		uint64_t getReqMana(uint32_t magLevel);

		int32_t getId() const {
			return id;
		}

		uint8_t getClientId() const {
			return clientId;
		}

		uint32_t getHPGain() const {
			return gainHP;
		}
		uint32_t getManaGain() const {
			return gainMana;
		}
		uint32_t getCapGain() const {
			return gainCap;
		}

		uint32_t getManaGainTicks() const {
			return gainManaTicks;
		}
		uint32_t getManaGainAmount() const {
			return gainManaAmount;
		}
		uint32_t getHealthGainTicks() const {
			return gainHealthTicks;
		}
		uint32_t getHealthGainAmount() const {
			return gainHealthAmount;
		}

		uint16_t getSoulMax() const {
			return std::min<uint16_t>(soulMax, 255);
		}
		uint16_t getSoulGainTicks() const {
			return gainSoulTicks;
		}

		uint32_t getAttackSpeed() const {
			return attackSpeed;
		}
		uint32_t getBaseSpeed() const {
			return baseSpeed;
		}

		uint32_t getFromVocation() const {
			return fromVocation;
		}

		float meleeDamageMultiplier, distDamageMultiplier, defenseMultiplier, armorMultiplier;

	protected:
		friend class Vocations;

		std::map<uint32_t, uint64_t> cacheMana;
		std::map<uint32_t, uint32_t> cacheSkill[SKILL_LAST + 1];

		std::string name;
		std::string description;

		float skillMultipliers[SKILL_LAST + 1];
		float manaMultiplier;

		uint32_t gainHealthTicks;
		uint32_t gainHealthAmount;
		uint32_t gainManaTicks;
		uint32_t gainManaAmount;
		uint32_t gainCap;
		uint32_t gainMana;
		uint32_t gainHP;
		uint32_t fromVocation;
		uint32_t attackSpeed;
		uint32_t baseSpeed;
		uint16_t id;

		uint16_t gainSoulTicks;
		uint16_t soulMax;

		uint8_t clientId;

		static uint32_t skillBase[SKILL_LAST + 1];
};

class Vocations
{
	public:
		bool loadFromXml();

		Vocation* getVocation(uint16_t id);
		int32_t getVocationId(const std::string& name) const;
		uint16_t getPromotedVocation(uint16_t vocationId) const;

	private:
		std::map<uint16_t, Vocation> vocationsMap;
};

#endif
