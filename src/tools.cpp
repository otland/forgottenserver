/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2014  Mark Samman <mark.samman@gmail.com>
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

#include <cctype>

#include "tools.h"
#include "configmanager.h"

extern ConfigManager g_config;

static inline uint32_t circularShift(int bits, uint32_t value)
{
	return (value << bits) | (value >> (32 - bits));
}

static void processSHA1MessageBlock(const uint8_t* messageBlock, uint32_t* H)
{
	uint32_t W[80];
	for (int i = 0; i < 16; ++i) {
		const size_t offset = i * 4;
		W[i] = messageBlock[offset] << 24 | messageBlock[offset + 1] << 16 | messageBlock[offset + 2] << 8 | messageBlock[offset + 3];
	}

	for (int i = 16; i < 80; ++i) {
		W[i] = circularShift(1, W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]);
	}

	uint32_t A = H[0], B = H[1], C = H[2], D = H[3], E = H[4];

	for (int i = 0; i < 20; ++i) {
		const uint32_t tmp = circularShift(5, A) + ((B & C) | ((~B) & D)) + E + W[i] + 0x5A827999;
		E = D; D = C; C = circularShift(30, B); B = A; A = tmp;
	}

	for (int i = 20; i < 40; ++i) {
		const uint32_t tmp = circularShift(5, A) + (B ^ C ^ D) + E + W[i] + 0x6ED9EBA1;
		E = D; D = C; C = circularShift(30, B); B = A; A = tmp;
	}

	for (int i = 40; i < 60; ++i) {
		const uint32_t tmp = circularShift(5, A) + ((B & C) | (B & D) | (C & D)) + E + W[i] + 0x8F1BBCDC;
		E = D; D = C; C = circularShift(30, B); B = A; A = tmp;
	}

	for (int i = 60; i < 80; ++i) {
		const uint32_t tmp = circularShift(5, A) + (B ^ C ^ D) + E + W[i] + 0xCA62C1D6;
		E = D; D = C; C = circularShift(30, B); B = A; A = tmp;
	}

	H[0] += A;
	H[1] += B;
	H[2] += C;
	H[3] += D;
	H[4] += E;
}

std::string transformToSHA1(const std::string& input)
{
	uint32_t H[] = {
		0x67452301,
		0xEFCDAB89,
		0x98BADCFE,
		0x10325476,
		0xC3D2E1F0
	};

	uint8_t messageBlock[64];
	size_t index = 0;
	uint32_t length_low = 0;
	uint32_t length_high = 0;

	for (char ch : input) {
		messageBlock[index++] = ch;

		length_low += 8;
		if (length_low == 0) {
			if (++length_high == 0) {
				return std::string();
			}
		}

		if (index == 64) {
			processSHA1MessageBlock(messageBlock, H);
			index = 0;
		}
	}

	messageBlock[index++] = 0x80;

	if (index > 56) {
		while (index < 64) {
			messageBlock[index++] = 0;
		}

		processSHA1MessageBlock(messageBlock, H);
		index = 0;
	}

	while (index < 56) {
		messageBlock[index++] = 0;
	}

	messageBlock[56] = length_high >> 24;
	messageBlock[57] = length_high >> 16;
	messageBlock[58] = length_high >> 8;
	messageBlock[59] = length_high;

	messageBlock[60] = length_low >> 24;
	messageBlock[61] = length_low >> 16;
	messageBlock[62] = length_low >> 8;
	messageBlock[63] = length_low;

	processSHA1MessageBlock(messageBlock, H);

	char hexstring[41];
	static const char hexDigits[] = {"0123456789abcdef"};
	for (int hashByte = 20; --hashByte >= 0;) {
		const uint8_t byte = H[hashByte >> 2] >> (((3 - hashByte) & 3) << 3);
		size_t index = hashByte << 1;
		hexstring[index++] = hexDigits[byte >> 4];
		hexstring[index] = hexDigits[byte & 15];
	}
	return std::string(hexstring, 40);
}

void replaceString(std::string& str, const std::string& sought, const std::string& replacement)
{
	size_t pos = 0;
	size_t start = 0;
	size_t soughtLen = sought.length();
	size_t replaceLen = replacement.length();

	while ((pos = str.find(sought, start)) != std::string::npos) {
		str = str.substr(0, pos) + replacement + str.substr(pos + soughtLen);
		start = pos + replaceLen;
	}
}

void trim_right(std::string& source, char t)
{
	source.erase(source.find_last_not_of(t) + 1);
}

void trim_left(std::string& source, char t)
{
	source.erase(0, source.find_first_not_of(t));
}

void toLowerCaseString(std::string& source)
{
	std::transform(source.begin(), source.end(), source.begin(), tolower);
}

std::string asLowerCaseString(const std::string& source)
{
	std::string s = source;
	toLowerCaseString(s);
	return s;
}

std::string asUpperCaseString(const std::string& source)
{
	std::string s = source;
	std::transform(s.begin(), s.end(), s.begin(), toupper);
	return s;
}

StringVec explodeString(const std::string& inString, const std::string& separator, int32_t limit/* = -1*/)
{
	StringVec returnVector;
	std::string::size_type start = 0, end = 0;

	while (--limit != -1 && (end = inString.find(separator, start)) != std::string::npos) {
		returnVector.push_back(inString.substr(start, end - start));
		start = end + separator.size();
	}

	returnVector.push_back(inString.substr(start));
	return returnVector;
}

IntegerVec vectorAtoi(const StringVec& stringVector)
{
	IntegerVec returnVector;
	for (const auto& string : stringVector) {
		returnVector.push_back(std::stoi(string));
	}
	return returnVector;
}

bool hasBitSet(uint32_t flag, uint32_t flags)
{
	return ((flags & flag) == flag);
}

std::mt19937& getRandomGenerator()
{
	static std::random_device rd;
	static std::mt19937 generator(rd());
	return generator;
}

int32_t uniform_random(int32_t minNumber, int32_t maxNumber)
{
	static std::uniform_int_distribution<int32_t> uniformRand;
	if (minNumber == maxNumber) {
		return minNumber;
	} else if (minNumber > maxNumber) {
		std::swap(minNumber, maxNumber);
	}
	return uniformRand(getRandomGenerator(), std::uniform_int_distribution<int32_t>::param_type(minNumber, maxNumber));
}

int32_t normal_random(int32_t minNumber, int32_t maxNumber)
{
	static std::normal_distribution<float> normalRand(0.5f, 0.25f);
	if (minNumber == maxNumber) {
		return minNumber;
	} else if (minNumber > maxNumber) {
		std::swap(minNumber, maxNumber);
	}
	return minNumber + std::max<float>(0.f, std::min<float>(1.f, normalRand(getRandomGenerator()))) * (maxNumber - minNumber);
}

bool boolean_random(double probability/* = 0.5*/)
{
	static std::bernoulli_distribution booleanRand;
	return booleanRand(getRandomGenerator(), std::bernoulli_distribution::param_type(probability));
}

void trimString(std::string& str)
{
	str.erase(str.find_last_not_of(' ') + 1);
	str.erase(0, str.find_first_not_of(' '));
}

std::string convertIPToString(uint32_t ip)
{
	char buffer[17];

	int res = sprintf(buffer, "%u.%u.%u.%u", ip & 0xFF, (ip >> 8) & 0xFF, (ip >> 16) & 0xFF, (ip >> 24));
	if (res < 0) {
		return std::string();
	}

	return buffer;
}

std::string formatDate(time_t time)
{
	const tm* tms = localtime(&time);
	if (!tms) {
		return std::string();
	}

	char buffer[20];
	int res = sprintf(buffer, "%02d/%02d/%04d %02d:%02d:%02d", tms->tm_mday, tms->tm_mon + 1, tms->tm_year + 1900, tms->tm_hour, tms->tm_min, tms->tm_sec);
	if (res < 0) {
		return std::string();
	}
	return std::string(buffer, 19);
}

std::string formatDateShort(time_t time)
{
	const tm* tms = localtime(&time);
	if (!tms) {
		return std::string();
	}

	char buffer[12];
	size_t res = strftime(buffer, 12, "%d %b %Y", tms);
	if (res == 0) {
		return std::string();
	}
	return std::string(buffer, 11);
}

Direction getDirection(const std::string& string)
{
	Direction direction = NORTH;

	if (string == "north" || string == "n" || string == "0") {
		direction = NORTH;
	} else if (string == "east" || string == "e" || string == "1") {
		direction = EAST;
	} else if (string == "south" || string == "s" || string == "2") {
		direction = SOUTH;
	} else if (string == "west" || string == "w" || string == "3") {
		direction = WEST;
	} else if (string == "southwest" || string == "south west" || string == "south-west" || string == "sw" || string == "4") {
		direction = SOUTHWEST;
	} else if (string == "southeast" || string == "south east" || string == "south-east" || string == "se" || string == "5") {
		direction = SOUTHEAST;
	} else if (string == "northwest" || string == "north west" || string == "north-west" || string == "nw" || string == "6") {
		direction = NORTHWEST;
	} else if (string == "northeast" || string == "north east" || string == "north-east" || string == "ne" || string == "7") {
		direction = NORTHEAST;
	}

	return direction;
}

Position getNextPosition(Direction direction, Position pos)
{
	switch (direction) {
		case NORTH:
			pos.y--;
			break;

		case SOUTH:
			pos.y++;
			break;

		case WEST:
			pos.x--;
			break;

		case EAST:
			pos.x++;
			break;

		case SOUTHWEST:
			pos.x--;
			pos.y++;
			break;

		case NORTHWEST:
			pos.x--;
			pos.y--;
			break;

		case NORTHEAST:
			pos.x++;
			pos.y--;
			break;

		case SOUTHEAST:
			pos.x++;
			pos.y++;
			break;

		default:
			break;
	}

	return pos;
}

Direction getDirectionTo(const Position& from, const Position& to)
{
	Direction dir;

	int32_t x_offset = Position::getOffsetX(from, to);
	if (x_offset < 0) {
		dir = EAST;
		x_offset = std::abs(x_offset);
	} else {
		dir = WEST;
	}

	int32_t y_offset = Position::getOffsetY(from, to);
	if (y_offset >= 0) {
		if (y_offset > x_offset) {
			dir = NORTH;
		} else if (y_offset == x_offset) {
			if (dir == EAST) {
				dir = NORTHEAST;
			} else {
				dir = NORTHWEST;
			}
		}
	} else {
		y_offset = std::abs(y_offset);
		if (y_offset > x_offset) {
			dir = SOUTH;
		} else if (y_offset == x_offset) {
			if (dir == EAST) {
				dir = SOUTHEAST;
			} else {
				dir = SOUTHWEST;
			}
		}
	}
	return dir;
}

struct MagicEffectNames {
	const char* name;
	MagicEffectClasses effect;
};

struct ShootTypeNames {
	const char* name;
	ShootType_t shoot;
};

struct CombatTypeNames {
	const char* name;
	CombatType_t combat;
};

struct AmmoTypeNames {
	const char* name;
	Ammo_t ammoType;
};

struct AmmoActionNames {
	const char* name;
	AmmoAction_t ammoAction;
};

MagicEffectNames magicEffectNames[] = {
	{"redspark",		CONST_ME_DRAWBLOOD},
	{"bluebubble",		CONST_ME_LOSEENERGY},
	{"poff",		CONST_ME_POFF},
	{"yellowspark",		CONST_ME_BLOCKHIT},
	{"explosionarea",	CONST_ME_EXPLOSIONAREA},
	{"explosion",		CONST_ME_EXPLOSIONHIT},
	{"firearea",		CONST_ME_FIREAREA},
	{"yellowbubble",	CONST_ME_YELLOW_RINGS},
	{"greenbubble",		CONST_ME_GREEN_RINGS},
	{"blackspark",		CONST_ME_HITAREA},
	{"teleport",		CONST_ME_TELEPORT},
	{"energy",		CONST_ME_ENERGYHIT},
	{"blueshimmer",		CONST_ME_MAGIC_BLUE},
	{"redshimmer",		CONST_ME_MAGIC_RED},
	{"greenshimmer",	CONST_ME_MAGIC_GREEN},
	{"fire",		CONST_ME_HITBYFIRE},
	{"greenspark",		CONST_ME_HITBYPOISON},
	{"mortarea",		CONST_ME_MORTAREA},
	{"greennote",		CONST_ME_SOUND_GREEN},
	{"rednote",		CONST_ME_SOUND_RED},
	{"poison",		CONST_ME_POISONAREA},
	{"yellownote",		CONST_ME_SOUND_YELLOW},
	{"purplenote",		CONST_ME_SOUND_PURPLE},
	{"bluenote",		CONST_ME_SOUND_BLUE},
	{"whitenote",		CONST_ME_SOUND_WHITE},
	{"bubbles",		CONST_ME_BUBBLES},
	{"dice",		CONST_ME_CRAPS},
	{"giftwraps",		CONST_ME_GIFT_WRAPS},
	{"yellowfirework",	CONST_ME_FIREWORK_YELLOW},
	{"redfirework",		CONST_ME_FIREWORK_RED},
	{"bluefirework",	CONST_ME_FIREWORK_BLUE},
	{"stun",		CONST_ME_STUN},
	{"sleep",		CONST_ME_SLEEP},
	{"watercreature",	CONST_ME_WATERCREATURE},
	{"groundshaker",	CONST_ME_GROUNDSHAKER},
	{"hearts",		CONST_ME_HEARTS},
	{"fireattack",		CONST_ME_FIREATTACK},
	{"energyarea",		CONST_ME_ENERGYAREA},
	{"smallclouds",		CONST_ME_SMALLCLOUDS},
	{"holydamage",		CONST_ME_HOLYDAMAGE},
	{"bigclouds",		CONST_ME_BIGCLOUDS},
	{"icearea",		CONST_ME_ICEAREA},
	{"icetornado",		CONST_ME_ICETORNADO},
	{"iceattack",		CONST_ME_ICEATTACK},
	{"stones",		CONST_ME_STONES},
	{"smallplants",		CONST_ME_SMALLPLANTS},
	{"carniphila",		CONST_ME_CARNIPHILA},
	{"purpleenergy",	CONST_ME_PURPLEENERGY},
	{"yellowenergy",	CONST_ME_YELLOWENERGY},
	{"holyarea",		CONST_ME_HOLYAREA},
	{"bigplants",		CONST_ME_BIGPLANTS},
	{"cake",		CONST_ME_CAKE},
	{"giantice",		CONST_ME_GIANTICE},
	{"watersplash",		CONST_ME_WATERSPLASH},
	{"plantattack",		CONST_ME_PLANTATTACK},
	{"tutorialarrow",	CONST_ME_TUTORIALARROW},
	{"tutorialsquare",	CONST_ME_TUTORIALSQUARE},
	{"mirrorhorizontal",	CONST_ME_MIRRORHORIZONTAL},
	{"mirrorvertical",	CONST_ME_MIRRORVERTICAL},
	{"skullhorizontal",	CONST_ME_SKULLHORIZONTAL},
	{"skullvertical",	CONST_ME_SKULLVERTICAL},
	{"assassin",		CONST_ME_ASSASSIN},
	{"stepshorizontal",	CONST_ME_STEPSHORIZONTAL},
	{"bloodysteps",		CONST_ME_BLOODYSTEPS},
	{"stepsvertical",	CONST_ME_STEPSVERTICAL},
	{"yalaharighost",	CONST_ME_YALAHARIGHOST},
	{"bats",		CONST_ME_BATS},
	{"smoke",		CONST_ME_SMOKE},
	{"insects",		CONST_ME_INSECTS},
	{"dragonhead",		CONST_ME_DRAGONHEAD},
	{"orcshaman",		CONST_ME_ORCSHAMAN},
	{"orcshamanfire",	CONST_ME_ORCSHAMAN_FIRE},
	{"thunder",		CONST_ME_THUNDER},
	{"ferumbras",		CONST_ME_FERUMBRAS},
	{"confettihorizontal",	CONST_ME_CONFETTI_HORIZONTAL},
	{"confettivertical",	CONST_ME_CONFETTI_VERTICAL},
	{"blacksmoke",		CONST_ME_BLACKSMOKE}
};

ShootTypeNames shootTypeNames[] = {
	{"spear",		CONST_ANI_SPEAR},
	{"bolt",		CONST_ANI_BOLT},
	{"arrow",		CONST_ANI_ARROW},
	{"fire",		CONST_ANI_FIRE},
	{"energy",		CONST_ANI_ENERGY},
	{"poisonarrow",		CONST_ANI_POISONARROW},
	{"burstarrow",		CONST_ANI_BURSTARROW},
	{"throwingstar",	CONST_ANI_THROWINGSTAR},
	{"throwingknife",	CONST_ANI_THROWINGKNIFE},
	{"smallstone",		CONST_ANI_SMALLSTONE},
	{"death",		CONST_ANI_DEATH},
	{"largerock",		CONST_ANI_LARGEROCK},
	{"snowball",		CONST_ANI_SNOWBALL},
	{"powerbolt",		CONST_ANI_POWERBOLT},
	{"poison",		CONST_ANI_POISON},
	{"infernalbolt",	CONST_ANI_INFERNALBOLT},
	{"huntingspear",	CONST_ANI_HUNTINGSPEAR},
	{"enchantedspear",	CONST_ANI_ENCHANTEDSPEAR},
	{"redstar",		CONST_ANI_REDSTAR},
	{"greenstar",		CONST_ANI_GREENSTAR},
	{"royalspear",		CONST_ANI_ROYALSPEAR},
	{"sniperarrow",		CONST_ANI_SNIPERARROW},
	{"onyxarrow",		CONST_ANI_ONYXARROW},
	{"piercingbolt",	CONST_ANI_PIERCINGBOLT},
	{"whirlwindsword",	CONST_ANI_WHIRLWINDSWORD},
	{"whirlwindaxe",	CONST_ANI_WHIRLWINDAXE},
	{"whirlwindclub",	CONST_ANI_WHIRLWINDCLUB},
	{"etherealspear",	CONST_ANI_ETHEREALSPEAR},
	{"ice",			CONST_ANI_ICE},
	{"earth",		CONST_ANI_EARTH},
	{"holy",		CONST_ANI_HOLY},
	{"suddendeath",		CONST_ANI_SUDDENDEATH},
	{"flasharrow",		CONST_ANI_FLASHARROW},
	{"flammingarrow",	CONST_ANI_FLAMMINGARROW},
	{"shiverarrow",		CONST_ANI_SHIVERARROW},
	{"energyball",		CONST_ANI_ENERGYBALL},
	{"smallice",		CONST_ANI_SMALLICE},
	{"smallholy",		CONST_ANI_SMALLHOLY},
	{"smallearth",		CONST_ANI_SMALLEARTH},
	{"eartharrow",		CONST_ANI_EARTHARROW},
	{"explosion",		CONST_ANI_EXPLOSION},
	{"cake",		CONST_ANI_CAKE},
	{"tarsalarrow",		CONST_ANI_TARSALARROW},
	{"vortexbolt",		CONST_ANI_VORTEXBOLT},
	{"prismaticbolt",	CONST_ANI_PRISMATICBOLT},
	{"crystallinearrow",	CONST_ANI_CRYSTALLINEARROW},
	{"drillbolt",		CONST_ANI_DRILLBOLT},
	{"envenomedarrow",	CONST_ANI_ENVENOMEDARROW}
};

CombatTypeNames combatTypeNames[] = {
	{"physical",		COMBAT_PHYSICALDAMAGE},
	{"energy",		COMBAT_ENERGYDAMAGE},
	{"earth",		COMBAT_EARTHDAMAGE},
	{"fire",		COMBAT_FIREDAMAGE},
	{"undefined",		COMBAT_UNDEFINEDDAMAGE},
	{"lifedrain",		COMBAT_LIFEDRAIN},
	{"manadrain",		COMBAT_MANADRAIN},
	{"healing",		COMBAT_HEALING},
	{"drown",		COMBAT_DROWNDAMAGE},
	{"ice",			COMBAT_ICEDAMAGE},
	{"holy",		COMBAT_HOLYDAMAGE},
	{"death",		COMBAT_DEATHDAMAGE}
};

AmmoTypeNames ammoTypeNames[] = {
	{"spear",		AMMO_SPEAR},
	{"bolt",		AMMO_BOLT},
	{"arrow",		AMMO_ARROW},
	{"poisonarrow",		AMMO_ARROW},
	{"burstarrow",		AMMO_ARROW},
	{"throwingstar",	AMMO_THROWINGSTAR},
	{"throwingknife",	AMMO_THROWINGKNIFE},
	{"smallstone",		AMMO_STONE},
	{"largerock",		AMMO_STONE},
	{"snowball",		AMMO_SNOWBALL},
	{"powerbolt",		AMMO_BOLT},
	{"infernalbolt",	AMMO_BOLT},
	{"huntingspear",	AMMO_SPEAR},
	{"enchantedspear",	AMMO_SPEAR},
	{"royalspear",		AMMO_SPEAR},
	{"sniperarrow",		AMMO_ARROW},
	{"onyxarrow",		AMMO_ARROW},
	{"piercingbolt",	AMMO_BOLT},
	{"etherealspear",	AMMO_SPEAR},
	{"flasharrow",		AMMO_ARROW},
	{"flammingarrow",	AMMO_ARROW},
	{"shiverarrow",		AMMO_ARROW},
	{"eartharrow",		AMMO_ARROW}
};

AmmoActionNames ammoActionNames[] = {
	{"move",		AMMOACTION_MOVE},
	{"moveback",		AMMOACTION_MOVEBACK},
	{"removecharge",	AMMOACTION_REMOVECHARGE},
	{"removecount",		AMMOACTION_REMOVECOUNT}
};

MagicEffectClasses getMagicEffect(const std::string& strValue)
{
	for (size_t i = 0; i < sizeof(magicEffectNames) / sizeof(MagicEffectNames); ++i) {
		if (strcasecmp(strValue.c_str(), magicEffectNames[i].name) == 0) {
			return magicEffectNames[i].effect;
		}
	}
	return CONST_ME_UNK;
}

ShootType_t getShootType(const std::string& strValue)
{
	for (size_t i = 0, size = sizeof(shootTypeNames) / sizeof(ShootTypeNames); i < size; ++i) {
		if (strcasecmp(strValue.c_str(), shootTypeNames[i].name) == 0) {
			return shootTypeNames[i].shoot;
		}
	}
	return CONST_ANI_UNK;
}

CombatType_t getCombatType(const std::string& strValue)
{
	for (size_t i = 0, size = sizeof(combatTypeNames) / sizeof(CombatTypeNames); i < size; ++i) {
		if (strcasecmp(strValue.c_str(), combatTypeNames[i].name) == 0) {
			return combatTypeNames[i].combat;
		}
	}
	return COMBAT_NONE;
}

std::string getCombatName(CombatType_t combatType)
{
	for (size_t i = 0, size = sizeof(combatTypeNames) / sizeof(CombatTypeNames); i < size; ++i) {
		if (combatTypeNames[i].combat == combatType) {
			return combatTypeNames[i].name;
		}
	}
	return "unknown";
}

Ammo_t getAmmoType(const std::string& strValue)
{
	for (size_t i = 0, size = sizeof(ammoTypeNames) / sizeof(AmmoTypeNames); i < size; ++i) {
		if (strcasecmp(strValue.c_str(), ammoTypeNames[i].name) == 0) {
			return ammoTypeNames[i].ammoType;
		}
	}
	return AMMO_NONE;
}

AmmoAction_t getAmmoAction(const std::string& strValue)
{
	for (size_t i = 0, size = sizeof(ammoActionNames) / sizeof(AmmoActionNames); i < size; ++i) {
		if (strcasecmp(strValue.c_str(), ammoActionNames[i].name) == 0) {
			return ammoActionNames[i].ammoAction;
		}
	}
	return AMMOACTION_NONE;
}

std::string getSkillName(uint16_t skillid)
{
	switch (skillid) {
		case SKILL_FIST:
			return "fist fighting";

		case SKILL_CLUB:
			return "club fighting";

		case SKILL_SWORD:
			return "sword fighting";

		case SKILL_AXE:
			return "axe fighting";

		case SKILL_DISTANCE:
			return "distance fighting";

		case SKILL_SHIELD:
			return "shielding";

		case SKILL_FISHING:
			return "fishing";

		case SKILL_MAGLEVEL:
			return "magic level";

		case SKILL_LEVEL:
			return "level";

		default:
			return "unknown";
	}
}

uint32_t adlerChecksum(uint8_t* data, size_t length)
{
	if (length > NETWORKMESSAGE_MAXSIZE) {
		return 0;
	}

	const uint16_t adler = 65521;

	uint32_t a = 1, b = 0;

	while (length > 0) {
		size_t tmp = length > 5552 ? 5552 : length;
		length -= tmp;

		do {
			a += *data++;
			b += a;
		} while (--tmp);

		a %= adler;
		b %= adler;
	}

	return (b << 16) | a;
}

std::string ucfirst(std::string str)
{
	for (size_t i = 0; i < str.length(); ++i) {
		if (str[i] != ' ') {
			str[i] = toupper(str[i]);
			break;
		}
	}
	return str;
}

std::string ucwords(std::string str)
{
	size_t strLength = str.length();
	if (strLength == 0) {
		return str;
	}

	str[0] = toupper(str.front());
	for (size_t i = 1; i < strLength; ++i) {
		if (str[i - 1] == ' ') {
			str[i] = toupper(str[i]);
		}
	}

	return str;
}

bool booleanString(const std::string& str)
{
	if (str.empty()) {
		return false;
	}

	char ch = tolower(str.front());
	return ch != 'f' && ch != 'n' && ch != '0';
}

std::string getWeaponName(WeaponType_t weaponType)
{
	switch (weaponType) {
		case WEAPON_SWORD: return "sword";
		case WEAPON_CLUB: return "club";
		case WEAPON_AXE: return "axe";
		case WEAPON_DISTANCE: return "distance";
		case WEAPON_WAND: return "wand";
		case WEAPON_AMMO: return "ammunition";
		default: return std::string();
	}
}

uint32_t combatTypeToIndex(CombatType_t combatType)
{
	switch (combatType) {
		case COMBAT_NONE:
			return 0;
		case COMBAT_PHYSICALDAMAGE:
			return 1;
		case COMBAT_ENERGYDAMAGE:
			return 2;
		case COMBAT_EARTHDAMAGE:
			return 3;
		case COMBAT_FIREDAMAGE:
			return 4;
		case COMBAT_UNDEFINEDDAMAGE:
			return 5;
		case COMBAT_LIFEDRAIN:
			return 6;
		case COMBAT_MANADRAIN:
			return 7;
		case COMBAT_HEALING:
			return 8;
		case COMBAT_DROWNDAMAGE:
			return 9;
		case COMBAT_ICEDAMAGE:
			return 10;
		case COMBAT_HOLYDAMAGE:
			return 11;
		case COMBAT_DEATHDAMAGE:
			return 12;
		default:
			return 0;
	}
}

CombatType_t indexToCombatType(uint32_t v)
{
	if (v == 0) {
		return COMBAT_FIRST;
	}

	return (CombatType_t)(1 << (v - 1));
}

uint8_t serverFluidToClient(uint8_t serverFluid)
{
	uint8_t size = sizeof(clientToServerFluidMap) / sizeof(uint8_t);

	for (uint8_t i = 0; i < size; ++i) {
		if (clientToServerFluidMap[i] == serverFluid) {
			return i;
		}
	}

	return 0;
}

uint8_t clientFluidToServer(uint8_t clientFluid)
{
	uint8_t size = sizeof(clientToServerFluidMap) / sizeof(int8_t);

	if (clientFluid >= size) {
		return 0;
	}

	return clientToServerFluidMap[clientFluid];
}

itemAttrTypes stringToItemAttribute(const std::string& str)
{
	if (str == "aid") {
		return ITEM_ATTRIBUTE_ACTIONID;
	} else if (str == "uid") {
		return ITEM_ATTRIBUTE_UNIQUEID;
	} else if (str == "description") {
		return ITEM_ATTRIBUTE_DESCRIPTION;
	} else if (str == "text") {
		return ITEM_ATTRIBUTE_TEXT;
	} else if (str == "date") {
		return ITEM_ATTRIBUTE_DATE;
	} else if (str == "writer") {
		return ITEM_ATTRIBUTE_WRITER;
	} else if (str == "owner") {
		return ITEM_ATTRIBUTE_OWNER;
	} else if (str == "duration") {
		return ITEM_ATTRIBUTE_DURATION;
	} else if (str == "decaystate") {
		return ITEM_ATTRIBUTE_DECAYSTATE;
	} else if (str == "corpseowner") {
		return ITEM_ATTRIBUTE_CORPSEOWNER;
	} else if (str == "charges") {
		return ITEM_ATTRIBUTE_CHARGES;
	} else if (str == "fluidtype") {
		return ITEM_ATTRIBUTE_FLUIDTYPE;
	} else if (str == "doorid") {
		return ITEM_ATTRIBUTE_DOORID;
	}
	return ITEM_ATTRIBUTE_NONE;
}

std::string getFirstLine(const std::string& str)
{
	std::string firstLine;
	firstLine.reserve(str.length());
	for (const char c : str) {
		if (c == '\n') {
			break;
		}
		firstLine.push_back(c);
	}
	return firstLine;
}

const char* getReturnMessage(ReturnValue value)
{
	switch (value) {
		case RET_DESTINATIONOUTOFREACH:
			return "Destination is out of reach.";

		case RET_NOTMOVEABLE:
			return "You cannot move this object.";

		case RET_DROPTWOHANDEDITEM:
			return "Drop the double-handed object first.";

		case RET_BOTHHANDSNEEDTOBEFREE:
			return "Both hands need to be free.";

		case RET_CANNOTBEDRESSED:
			return "You cannot dress this object there.";

		case RET_PUTTHISOBJECTINYOURHAND:
			return "Put this object in your hand.";

		case RET_PUTTHISOBJECTINBOTHHANDS:
			return "Put this object in both hands.";

		case RET_CANONLYUSEONEWEAPON:
			return "You may only use one weapon.";

		case RET_TOOFARAWAY:
			return "Too far away.";

		case RET_FIRSTGODOWNSTAIRS:
			return "First go downstairs.";

		case RET_FIRSTGOUPSTAIRS:
			return "First go upstairs.";

		case RET_NOTENOUGHCAPACITY:
			return "This object is too heavy for you to carry.";

		case RET_CONTAINERNOTENOUGHROOM:
			return "You cannot put more objects in this container.";

		case RET_NEEDEXCHANGE:
		case RET_NOTENOUGHROOM:
			return "There is not enough room.";

		case RET_CANNOTPICKUP:
			return "You cannot take this object.";

		case RET_CANNOTTHROW:
			return "You cannot throw there.";

		case RET_THEREISNOWAY:
			return "There is no way.";

		case RET_THISISIMPOSSIBLE:
			return "This is impossible.";

		case RET_PLAYERISPZLOCKED:
			return "You can not enter a protection zone after attacking another player.";

		case RET_PLAYERISNOTINVITED:
			return "You are not invited.";

		case RET_CREATUREDOESNOTEXIST:
			return "Creature does not exist.";

		case RET_DEPOTISFULL:
			return "You cannot put more items in this depot.";

		case RET_CANNOTUSETHISOBJECT:
			return "You cannot use this object.";

		case RET_PLAYERWITHTHISNAMEISNOTONLINE:
			return "A player with this name is not online.";

		case RET_NOTREQUIREDLEVELTOUSERUNE:
			return "You do not have the required magic level to use this rune.";

		case RET_YOUAREALREADYTRADING:
			return "You are already trading.";

		case RET_THISPLAYERISALREADYTRADING:
			return "This player is already trading.";

		case RET_YOUMAYNOTLOGOUTDURINGAFIGHT:
			return "You may not logout during or immediately after a fight!";

		case RET_DIRECTPLAYERSHOOT:
			return "You are not allowed to shoot directly on players.";

		case RET_NOTENOUGHLEVEL:
			return "You do not have enough level.";

		case RET_NOTENOUGHMAGICLEVEL:
			return "You do not have enough magic level.";

		case RET_NOTENOUGHMANA:
			return "You do not have enough mana.";

		case RET_NOTENOUGHSOUL:
			return "You do not have enough soul.";

		case RET_YOUAREEXHAUSTED:
			return "You are exhausted.";

		case RET_CANONLYUSETHISRUNEONCREATURES:
			return "You can only use this rune on creatures.";

		case RET_PLAYERISNOTREACHABLE:
			return "Player is not reachable.";

		case RET_CREATUREISNOTREACHABLE:
			return "Creature is not reachable.";

		case RET_ACTIONNOTPERMITTEDINPROTECTIONZONE:
			return "This action is not permitted in a protection zone.";

		case RET_YOUMAYNOTATTACKTHISPLAYER:
			return "You may not attack this player.";

		case RET_YOUMAYNOTATTACKTHISCREATURE:
			return "You may not attack this creature.";

		case RET_YOUMAYNOTATTACKAPERSONINPROTECTIONZONE:
			return "You may not attack a person in a protection zone.";

		case RET_YOUMAYNOTATTACKAPERSONWHILEINPROTECTIONZONE:
			return "You may not attack a person while you are in a protection zone.";

		case RET_YOUCANONLYUSEITONCREATURES:
			return "You can only use it on creatures.";

		case RET_TURNSECUREMODETOATTACKUNMARKEDPLAYERS:
			return "Turn secure mode off if you really want to attack unmarked players.";

		case RET_YOUNEEDPREMIUMACCOUNT:
			return "You need a premium account.";

		case RET_YOUNEEDTOLEARNTHISSPELL:
			return "You need to learn this spell first.";

		case RET_YOURVOCATIONCANNOTUSETHISSPELL:
			return "Your vocation cannot use this spell.";

		case RET_YOUNEEDAWEAPONTOUSETHISSPELL:
			return "You need to equip a weapon to use this spell.";

		case RET_PLAYERISPZLOCKEDLEAVEPVPZONE:
			return "You can not leave a pvp zone after attacking another player.";

		case RET_PLAYERISPZLOCKEDENTERPVPZONE:
			return "You can not enter a pvp zone after attacking another player.";

		case RET_ACTIONNOTPERMITTEDINANOPVPZONE:
			return "This action is not permitted in a non pvp zone.";

		case RET_YOUCANNOTLOGOUTHERE:
			return "You can not logout here.";

		case RET_YOUNEEDAMAGICITEMTOCASTSPELL:
			return "You need a magic item to cast this spell.";

		case RET_CANNOTCONJUREITEMHERE:
			return "You cannot conjure items here.";

		case RET_YOUNEEDTOSPLITYOURSPEARS:
			return "You need to split your spears first.";

		case RET_NAMEISTOOAMBIGIOUS:
			return "Name is too ambigious.";

		case RET_CANONLYUSEONESHIELD:
			return "You may use only one shield.";

		case RET_NOPARTYMEMBERSINRANGE:
			return "No party members in range.";

		case RET_YOUARENOTTHEOWNER:
			return "You are not the owner.";

		default: // RET_NOTPOSSIBLE, etc
			return "Sorry, not possible.";
	}
}

#if !defined(_MSC_VER) || _MSC_VER < 1800
double round(double v)
{
	if (v >= 0.0) {
		return std::floor(v + 0.5);
	} else {
		return std::ceil(v - 0.5);
	}
}
#endif
