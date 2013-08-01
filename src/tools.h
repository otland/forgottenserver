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

#ifndef __OTSERV_TOOLS_H__
#define __OTSERV_TOOLS_H__

#include "definitions.h"
#include "position.h"
#include "const.h"
#include "enums.h"

#include <string>
#include <algorithm>

#include <libxml/parser.h>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

enum DistributionType_t {
	DISTRO_UNIFORM,
	DISTRO_SQUARE,
	DISTRO_NORMAL
};

std::string transformToMD5(const std::string& plainText);
std::string transformToSHA1(const std::string& plainText);
bool passwordTest(const std::string& plain, const std::string& hash);

void replaceString(std::string& str, const std::string& sought, const std::string& replacement);
void trim_right(std::string& source, const std::string& t);
void trim_left(std::string& source, const std::string& t);
void toLowerCaseString(std::string& source);
void toUpperCaseString(std::string& source);
std::string asLowerCaseString(const std::string& source);
std::string asUpperCaseString(const std::string& source);

bool utf8ToLatin1(const char* intext, std::string& outtext);
bool readXMLInteger(xmlNodePtr node, const char* tag, int32_t& value);
template<typename T>
T readXMLValue(xmlNodePtr node, const char* tag)
{
	char* nodeValue = (char*)xmlGetProp(node, (xmlChar*)tag);
	if (!nodeValue) {
		return T();
	}

	T value;
	try {
		value = boost::lexical_cast<T>(nodeValue);
	} catch(boost::bad_lexical_cast&) {}
	xmlFree(nodeValue);
	return value;
}
bool readXMLFloat(xmlNodePtr node, const char* tag, float& value);
bool readXMLString(xmlNodePtr node, const char* tag, std::string& value);
bool readXMLContentString(xmlNodePtr node, std::string& value);

typedef std::vector<std::string> StringVec;
typedef std::vector<int32_t> IntegerVec;

StringVec explodeString(const std::string& inString, const std::string& separator, int32_t limit = -1);
IntegerVec vectorAtoi(const StringVec& stringVector);
bool hasBitSet(uint32_t flag, uint32_t flags);

bool isNumber(char character);

int random_range(int lowest_number, int highest_number, DistributionType_t type = DISTRO_UNIFORM);

Direction getDirection(const std::string& string);
Position getNextPosition(Direction direction, Position pos);
Direction getDirectionTo(const Position& from, const Position& to);

std::string getFirstLine(const std::string& str);

std::string parseParams(tokenizer::iterator& it, tokenizer::iterator end);

std::string formatDate(time_t time);
std::string formatDateShort(time_t time);
std::string convertIPToString(uint32_t ip);

std::string trimString(std::string& str);

MagicEffectClasses getMagicEffect(const std::string& strValue);
ShootType_t getShootType(const std::string& strValue);
Ammo_t getAmmoType(const std::string& strValue);
AmmoAction_t getAmmoAction(const std::string& strValue);
CombatType_t getCombatType(const std::string& strValue);
std::string getCombatName(CombatType_t combatType);

std::string getSkillName(uint16_t skillid);
skills_t getSkillId(const std::string& param);

uint32_t adlerChecksum(uint8_t* data, size_t len);

std::string ucfirst(std::string str);
std::string ucwords(std::string str);
bool booleanString(const std::string& str);

std::string getWeaponName(WeaponType_t weaponType);

uint32_t combatTypeToIndex(CombatType_t combatType);
CombatType_t indexToCombatType(uint32_t v);

uint8_t serverFluidToClient(uint8_t serverFluid);
uint8_t clientFluidToServer(uint8_t clientFluid);

#endif
