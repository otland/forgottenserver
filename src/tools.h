// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_TOOLS_H
#define FS_TOOLS_H

#include "const.h"
#include "enums.h"
#include "position.h"

void printXMLError(const std::string& where, std::string_view fileName, const pugi::xml_parse_result& result);

std::string transformToSHA1(std::string_view input);
std::string hmac(std::string_view algorithm, std::string_view key, std::string_view message);
std::string generateToken(std::string_view key, uint64_t counter, size_t length = AUTHENTICATOR_DIGITS);

using StringVector = std::vector<std::string>;
using IntegerVector = std::vector<int32_t>;

std::vector<std::string_view> explodeString(std::string_view inString, const std::string& separator,
                                            int32_t limit = -1);
IntegerVector vectorAtoi(const std::vector<std::string_view>& stringVector);
constexpr bool hasBitSet(uint32_t flag, uint32_t flags) { return (flags & flag) != 0; }

std::mt19937& getRandomGenerator();
int32_t uniform_random(int32_t minNumber, int32_t maxNumber);
int32_t normal_random(int32_t minNumber, int32_t maxNumber);
bool boolean_random(double probability = 0.5);
std::string randomBytes(size_t length);

Position getNextPosition(Direction direction, Position pos);
Direction getDirectionTo(const Position& from, const Position& to);

std::string getFirstLine(const std::string& str);

std::string formatDateShort(time_t time);

uint16_t getDepotBoxId(uint16_t index);
MagicEffectClasses getMagicEffect(const std::string& strValue);
ShootType_t getShootType(const std::string& strValue);
Ammo_t getAmmoType(const std::string& strValue);
WeaponAction_t getWeaponAction(const std::string& strValue);
Skulls_t getSkullType(const std::string& strValue);
std::string getCombatName(CombatType_t combatType);

std::string getSpecialSkillName(uint8_t skillid);
std::string getSkillName(uint8_t skillid);

uint32_t adlerChecksum(const uint8_t* data, size_t length);

std::string ucfirst(std::string str);
std::string ucwords(std::string str);
bool booleanString(std::string_view str);

size_t combatTypeToIndex(CombatType_t combatType);
CombatType_t indexToCombatType(size_t v);

uint8_t serverFluidToClient(uint8_t serverFluid);
uint8_t clientFluidToServer(uint8_t clientFluid);

itemAttrTypes stringToItemAttribute(const std::string& str);

const char* getReturnMessage(ReturnValue value);

int64_t OTSYS_TIME();

SpellGroup_t stringToSpellGroup(const std::string& value);

const std::vector<Direction>& getShuffleDirections();

#endif // FS_TOOLS_H
