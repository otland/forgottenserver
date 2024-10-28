// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_VOCATION_H
#define FS_VOCATION_H

#include "configmanager.h"
#include "enums.h"

class Vocation
{
public:
	explicit Vocation(uint16_t id) : id(id) {}

	const std::string& getVocName() const { return name; }
	const std::string& getVocDescription() const { return description; }
	uint64_t getReqSkillTries(uint8_t skill, uint16_t level);
	uint64_t getReqMana(uint32_t magLevel);

	uint16_t getId() const { return id; }

	uint8_t getClientId() const { return clientId; }

	uint32_t getHPGain() const { return gainHP; }
	uint32_t getManaGain() const { return gainMana; }
	uint32_t getCapGain() const { return gainCap; }

	uint32_t getManaGainTicks() const { return gainManaTicks; }
	uint32_t getManaGainAmount() const { return gainManaAmount; }
	uint32_t getHealthGainTicks() const { return gainHealthTicks; }
	uint32_t getHealthGainAmount() const { return gainHealthAmount; }

	uint8_t getSoulMax() const { return soulMax; }
	uint16_t getSoulGainTicks() const { return gainSoulTicks; }

	uint32_t getAttackSpeed() const { return attackSpeed; }
	uint32_t getBaseSpeed() const { return baseSpeed; }

	uint32_t getFromVocation() const { return fromVocation; }

	uint32_t getNoPongKickTime() const { return noPongKickTime; }

	bool allowsPvp() const { return allowPvp; }

	bool getMagicShield() const
	{
		if (!getBoolean(ConfigManager::MANASHIELD_BREAKABLE)) {
			return false;
		}
		return magicShield;
	}

	float meleeDamageMultiplier = 1.0f;
	float distDamageMultiplier = 1.0f;
	float defenseMultiplier = 1.0f;
	float armorMultiplier = 1.0f;

private:
	friend class Vocations;

	std::string name = "none";
	std::string description;

	double skillMultipliers[SKILL_LAST + 1] = {1.5, 2.0, 2.0, 2.0, 2.0, 1.5, 1.1};
	float manaMultiplier = 4.0f;

	uint32_t gainHealthTicks = 6;
	uint32_t gainHealthAmount = 1;
	uint32_t gainManaTicks = 6;
	uint32_t gainManaAmount = 1;
	uint32_t gainCap = 500;
	uint32_t gainMana = 5;
	uint32_t gainHP = 5;
	uint32_t fromVocation = VOCATION_NONE;
	uint32_t attackSpeed = 1500;
	uint32_t baseSpeed = 220;
	uint32_t noPongKickTime = 60000;

	uint16_t id;
	uint16_t gainSoulTicks = 120;

	uint8_t soulMax = 100;
	uint8_t clientId = 0;

	bool allowPvp = true;

	bool magicShield = false;
};

using VocationMap = std::map<uint16_t, Vocation>;

class Vocations
{
public:
	bool loadFromXml(std::istream& is, std::string_view filename);

	Vocation* getVocation(uint16_t id);
	int32_t getVocationId(std::string_view name) const;
	uint16_t getPromotedVocation(uint16_t vocationId) const;
	const VocationMap& getVocations() const { return vocationsMap; }

private:
	VocationMap vocationsMap;
};

#endif // FS_VOCATION_H
