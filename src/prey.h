/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_PREY_H
#define FS_PREY_H

enum BonusType : uint8_t {
	BONUS_DAMAGE_BOOST = 0,
	BONUS_DAMAGE_REDUCTION = 1,
	BONUS_XP_BONUS = 2,
	BONUS_IMPROVED_LOOT = 3,
	BONUS_NONE = 4,
};

enum PreyAction_t : uint8_t {
	PREY_ACTION_LISTREROLL = 0,
	PREY_ACTION_BONUSREROLL = 1,
	PREY_ACTION_MONSTERSELECTION = 2,
};

enum PreyState : uint8_t {
	STATE_LOCKED = 0,
	STATE_INACTIVE = 1,
	STATE_ACTIVE = 2,
	STATE_SELECTION = 3,
	STATE_SELECTION_CHANGE_MONSTER = 4,
};

enum UnlockState : uint8_t {
	UNLOCK_PREMIUM_OR_STORE = 0,
	UNLOCK_STORE = 1,
	UNLOCK_NONE = 2,
};

struct BonusEntry {
	BonusType type = BONUS_NONE;
	uint16_t initialValue = 0;
	uint16_t step = 0;

	BonusEntry() = default;
	BonusEntry(BonusType type, uint16_t initialValue, uint16_t step) : type(type), initialValue(initialValue), step(step) {}
};

struct PreyData {
	PreyState state = STATE_LOCKED;
	std::vector<std::string> preyList;
	uint16_t timeLeft = 0;
	uint64_t lastReroll = 0;
	BonusType bonusType = BONUS_NONE;
	uint8_t bonusGrade = 0;
	uint16_t bonusValue = 0;
	std::string preyMonster;

	PreyData() = default;
};

class Prey {
	public:
		bool loadFromXml();

		uint16_t getTimeToFreeReroll() {
			return timeToFreeReroll;
		}

		uint16_t getPreyDuration() {	
			return preyDuration;
		}

		uint16_t getRerollPricePerLevel() {
			return rerollPricePerLevel;
		}

		const std::vector<std::string>& getPreyNames() {
			return preyNames;
		}

		const std::vector<BonusEntry>& getAvailableBonuses() {
			return availableBonuses;
		}
			 
	protected:
		std::vector<std::string> preyNames;
		std::vector<BonusEntry> availableBonuses;
		uint16_t timeToFreeReroll;
		uint16_t preyDuration;
		uint16_t rerollPricePerLevel;

	private:
		void reset();
};

#endif
