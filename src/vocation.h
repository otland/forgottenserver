// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_VOCATION_H
#define FS_VOCATION_H

#include "configmanager.h"
#include "enums.h"

#include <memory>

class Vocation final : public std::enable_shared_from_this<Vocation>
{
public:
	explicit Vocation(uint16_t id) : id(id) {}

	/// Deleted copy constructor to ensure Vocation is non-copyable.
	Vocation(const Vocation&) = delete;
	/// Deleted assignment operator to ensure Vocation is non-copyable.
	Vocation& operator=(const Vocation&) = delete;

	uint64_t getReqSkillTries(uint8_t skill, uint16_t level);
	uint64_t getReqMana(uint32_t magLevel);

	bool isNone() const { return id == VOCATION_NONE; }

	float meleeDamageMultiplier = 1.0f;
	float distDamageMultiplier = 1.0f;
	float defenseMultiplier = 1.0f;
	float armorMultiplier = 1.0f;

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

	bool operator==(const Vocation& other) const { return id == other.id; }
};

using Vocation_ptr = std::shared_ptr<Vocation>;

namespace {

std::set<Vocation_ptr> loaded_vocations;

} // namespace

namespace tfs::game::vocations {

/// @brief Loads vocation data from an XML file.
/// @param {reload} Specifies whether to reload existing data.
/// @return True if the vocations were loaded successfully, false otherwise.
bool load_from_xml(std::istream& is, std::string_view filename, bool reload = false);

/// @brief Finds a vocation by its unique ID.
/// @param {id} The unique identifier of the vocation.
/// @return A shared pointer to the vocation, or nullptr if not found.
Vocation_ptr find_by_id(uint16_t id);

/// @brief Finds a vocation by its name.
/// @param {name} The name of the vocation.
/// @return A shared pointer to the vocation, or nullptr if not found.
Vocation_ptr find_by_name(std::string_view name);

/// @brief Finds a vocation by its promoted vocation ID.
/// @param {id} The promoted vocation ID.
/// @return A shared pointer to the vocation, or nullptr if not found.
Vocation_ptr find_by_promoted_id(uint16_t id);

/// @brief Gets the set of all loaded vocations.
/// @return A constant reference to the set of loaded vocations.
const std::set<Vocation_ptr>& find_vocations();

} // namespace tfs::game::vocations

#endif // FS_VOCATION_H
