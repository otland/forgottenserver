// Copyright 2022 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "tools.h"

#include "configmanager.h"

#include <chrono>

extern ConfigManager g_config;

void printXMLError(const std::string& where, const std::string& fileName, const pugi::xml_parse_result& result)
{
	std::cout << '[' << where << "] Failed to load " << fileName << ": " << result.description() << std::endl;

	FILE* file = fopen(fileName.c_str(), "rb");
	if (!file) {
		return;
	}

	char buffer[32768];
	uint32_t currentLine = 1;
	std::string line;

	auto offset = static_cast<size_t>(result.offset);
	size_t lineOffsetPosition = 0;
	size_t index = 0;
	size_t bytes;
	do {
		bytes = fread(buffer, 1, 32768, file);
		for (size_t i = 0; i < bytes; ++i) {
			char ch = buffer[i];
			if (ch == '\n') {
				if ((index + i) >= offset) {
					lineOffsetPosition = line.length() - ((index + i) - offset);
					bytes = 0;
					break;
				}
				++currentLine;
				line.clear();
			} else {
				line.push_back(ch);
			}
		}
		index += bytes;
	} while (bytes == 32768);
	fclose(file);

	std::cout << "Line " << currentLine << ':' << std::endl;
	std::cout << line << std::endl;
	for (size_t i = 0; i < lineOffsetPosition; i++) {
		if (line[i] == '\t') {
			std::cout << '\t';
		} else {
			std::cout << ' ';
		}
	}
	std::cout << '^' << std::endl;
}

static uint32_t circularShift(int bits, uint32_t value) { return (value << bits) | (value >> (32 - bits)); }

static void processSHA1MessageBlock(const uint8_t* messageBlock, uint32_t* H)
{
	uint32_t W[80];
	for (int i = 0; i < 16; ++i) {
		const size_t offset = i << 2;
		W[i] = messageBlock[offset] << 24 | messageBlock[offset + 1] << 16 | messageBlock[offset + 2] << 8 |
		       messageBlock[offset + 3];
	}

	for (int i = 16; i < 80; ++i) {
		W[i] = circularShift(1, W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]);
	}

	uint32_t A = H[0], B = H[1], C = H[2], D = H[3], E = H[4];

	for (int i = 0; i < 20; ++i) {
		const uint32_t tmp = circularShift(5, A) + ((B & C) | ((~B) & D)) + E + W[i] + 0x5A827999;
		E = D;
		D = C;
		C = circularShift(30, B);
		B = A;
		A = tmp;
	}

	for (int i = 20; i < 40; ++i) {
		const uint32_t tmp = circularShift(5, A) + (B ^ C ^ D) + E + W[i] + 0x6ED9EBA1;
		E = D;
		D = C;
		C = circularShift(30, B);
		B = A;
		A = tmp;
	}

	for (int i = 40; i < 60; ++i) {
		const uint32_t tmp = circularShift(5, A) + ((B & C) | (B & D) | (C & D)) + E + W[i] + 0x8F1BBCDC;
		E = D;
		D = C;
		C = circularShift(30, B);
		B = A;
		A = tmp;
	}

	for (int i = 60; i < 80; ++i) {
		const uint32_t tmp = circularShift(5, A) + (B ^ C ^ D) + E + W[i] + 0xCA62C1D6;
		E = D;
		D = C;
		C = circularShift(30, B);
		B = A;
		A = tmp;
	}

	H[0] += A;
	H[1] += B;
	H[2] += C;
	H[3] += D;
	H[4] += E;
}

std::string transformToSHA1(const std::string& input)
{
	uint32_t H[] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};

	uint8_t messageBlock[64];
	size_t index = 0;

	uint32_t length_low = 0;
	uint32_t length_high = 0;
	for (char ch : input) {
		messageBlock[index++] = ch;

		length_low += 8;
		if (length_low == 0) {
			length_high++;
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
		index = hashByte << 1;
		hexstring[index] = hexDigits[byte >> 4];
		hexstring[index + 1] = hexDigits[byte & 15];
	}
	return std::string(hexstring, 40);
}

std::string generateToken(const std::string& key, uint32_t ticks)
{
	// generate message from ticks
	std::string message(8, 0);
	for (uint8_t i = 8; --i; ticks >>= 8) {
		message[i] = static_cast<char>(ticks & 0xFF);
	}

	// hmac key pad generation
	std::string iKeyPad(64, 0x36), oKeyPad(64, 0x5C);
	for (uint8_t i = 0; i < key.length(); ++i) {
		iKeyPad[i] ^= key[i];
		oKeyPad[i] ^= key[i];
	}

	oKeyPad.reserve(84);

	// hmac concat inner pad with message
	iKeyPad.append(message);

	// hmac first pass
	message.assign(transformToSHA1(iKeyPad));

	// hmac concat outer pad with message, conversion from hex to int needed
	for (uint8_t i = 0; i < message.length(); i += 2) {
		oKeyPad.push_back(static_cast<char>(std::strtoul(message.substr(i, 2).c_str(), nullptr, 16)));
	}

	// hmac second pass
	message.assign(transformToSHA1(oKeyPad));

	// calculate hmac offset
	uint32_t offset = static_cast<uint32_t>(std::strtoul(message.substr(39, 1).c_str(), nullptr, 16) & 0xF);

	// get truncated hash
	uint32_t truncHash =
	    static_cast<uint32_t>(std::strtoul(message.substr(2 * offset, 8).c_str(), nullptr, 16)) & 0x7FFFFFFF;
	message.assign(std::to_string(truncHash));

	// return only last AUTHENTICATOR_DIGITS (default 6) digits, also asserts exactly 6 digits
	uint32_t hashLen = message.length();
	message.assign(message.substr(hashLen - std::min(hashLen, AUTHENTICATOR_DIGITS)));
	message.insert(0, AUTHENTICATOR_DIGITS - std::min(hashLen, AUTHENTICATOR_DIGITS), '0');
	return message;
}

bool caseInsensitiveEqual(std::string_view str1, std::string_view str2)
{
	return str1.size() == str2.size() &&
	       std::equal(str1.begin(), str1.end(), str2.begin(), [](char a, char b) { return tolower(a) == tolower(b); });
}

bool caseInsensitiveStartsWith(std::string_view str, std::string_view prefix)
{
	return str.size() >= prefix.size() && std::equal(prefix.begin(), prefix.end(), str.begin(),
	                                                 [](char a, char b) { return tolower(a) == tolower(b); });
}

StringVector explodeString(const std::string& inString, const std::string& separator, int32_t limit /* = -1*/)
{
	StringVector returnVector;
	std::string::size_type start = 0, end = 0;

	while (--limit != -1 && (end = inString.find(separator, start)) != std::string::npos) {
		returnVector.push_back(inString.substr(start, end - start));
		start = end + separator.size();
	}

	returnVector.push_back(inString.substr(start));
	return returnVector;
}

IntegerVector vectorAtoi(const StringVector& stringVector)
{
	IntegerVector returnVector;
	for (const auto& string : stringVector) {
		returnVector.push_back(std::stoi(string));
	}
	return returnVector;
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

	int32_t increment;
	const int32_t diff = maxNumber - minNumber;
	const float v = normalRand(getRandomGenerator());
	if (v < 0.0) {
		increment = diff / 2;
	} else if (v > 1.0) {
		increment = (diff + 1) / 2;
	} else {
		increment = round(v * diff);
	}
	return minNumber + increment;
}

bool boolean_random(double probability /* = 0.5*/)
{
	static std::bernoulli_distribution booleanRand;
	return booleanRand(getRandomGenerator(), std::bernoulli_distribution::param_type(probability));
}

std::string formatDate(time_t time)
{
	const tm* tms = localtime(&time);
	if (!tms) {
		return {};
	}

	char buffer[20];
	int res = sprintf(buffer, "%02d/%02d/%04d %02d:%02d:%02d", tms->tm_mday, tms->tm_mon + 1, tms->tm_year + 1900,
	                  tms->tm_hour, tms->tm_min, tms->tm_sec);
	if (res < 0) {
		return {};
	}
	return {buffer, 19};
}

std::string formatDateShort(time_t time)
{
	const tm* tms = localtime(&time);
	if (!tms) {
		return {};
	}

	char buffer[12];
	size_t res = strftime(buffer, 12, "%d %b %Y", tms);
	if (res == 0) {
		return {};
	}
	return {buffer, 11};
}

Direction getDirection(const std::string& string)
{
	Direction direction = DIRECTION_NORTH;

	if (string == "north" || string == "n" || string == "0") {
		direction = DIRECTION_NORTH;
	} else if (string == "east" || string == "e" || string == "1") {
		direction = DIRECTION_EAST;
	} else if (string == "south" || string == "s" || string == "2") {
		direction = DIRECTION_SOUTH;
	} else if (string == "west" || string == "w" || string == "3") {
		direction = DIRECTION_WEST;
	} else if (string == "southwest" || string == "south west" || string == "south-west" || string == "sw" ||
	           string == "4") {
		direction = DIRECTION_SOUTHWEST;
	} else if (string == "southeast" || string == "south east" || string == "south-east" || string == "se" ||
	           string == "5") {
		direction = DIRECTION_SOUTHEAST;
	} else if (string == "northwest" || string == "north west" || string == "north-west" || string == "nw" ||
	           string == "6") {
		direction = DIRECTION_NORTHWEST;
	} else if (string == "northeast" || string == "north east" || string == "north-east" || string == "ne" ||
	           string == "7") {
		direction = DIRECTION_NORTHEAST;
	}

	return direction;
}

Position getNextPosition(Direction direction, Position pos)
{
	switch (direction) {
		case DIRECTION_NORTH:
			pos.y--;
			break;

		case DIRECTION_SOUTH:
			pos.y++;
			break;

		case DIRECTION_WEST:
			pos.x--;
			break;

		case DIRECTION_EAST:
			pos.x++;
			break;

		case DIRECTION_SOUTHWEST:
			pos.x--;
			pos.y++;
			break;

		case DIRECTION_NORTHWEST:
			pos.x--;
			pos.y--;
			break;

		case DIRECTION_NORTHEAST:
			pos.x++;
			pos.y--;
			break;

		case DIRECTION_SOUTHEAST:
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
	if (from == to) {
		return DIRECTION_NONE;
	}

	Direction dir;

	int32_t x_offset = Position::getOffsetX(from, to);
	if (x_offset < 0) {
		dir = DIRECTION_EAST;
		x_offset = std::abs(x_offset);
	} else {
		dir = DIRECTION_WEST;
	}

	int32_t y_offset = Position::getOffsetY(from, to);
	if (y_offset >= 0) {
		if (y_offset > x_offset) {
			dir = DIRECTION_NORTH;
		} else if (y_offset == x_offset) {
			if (dir == DIRECTION_EAST) {
				dir = DIRECTION_NORTHEAST;
			} else {
				dir = DIRECTION_NORTHWEST;
			}
		}
	} else {
		y_offset = std::abs(y_offset);
		if (y_offset > x_offset) {
			dir = DIRECTION_SOUTH;
		} else if (y_offset == x_offset) {
			if (dir == DIRECTION_EAST) {
				dir = DIRECTION_SOUTHEAST;
			} else {
				dir = DIRECTION_SOUTHWEST;
			}
		}
	}
	return dir;
}

using MagicEffectNames = std::unordered_map<std::string, MagicEffectClasses>;
using ShootTypeNames = std::unordered_map<std::string, ShootType_t>;
using CombatTypeNames = std::unordered_map<CombatType_t, std::string, std::hash<int32_t>>;
using AmmoTypeNames = std::unordered_map<std::string, Ammo_t>;
using WeaponActionNames = std::unordered_map<std::string, WeaponAction_t>;
using SkullNames = std::unordered_map<std::string, Skulls_t>;

MagicEffectNames magicEffectNames = {
    {"redspark", CONST_ME_DRAWBLOOD},
    {"bluebubble", CONST_ME_LOSEENERGY},
    {"poff", CONST_ME_POFF},
    {"yellowspark", CONST_ME_BLOCKHIT},
    {"explosionarea", CONST_ME_EXPLOSIONAREA},
    {"explosion", CONST_ME_EXPLOSIONHIT},
    {"firearea", CONST_ME_FIREAREA},
    {"yellowbubble", CONST_ME_YELLOW_RINGS},
    {"greenbubble", CONST_ME_GREEN_RINGS},
    {"blackspark", CONST_ME_HITAREA},
    {"teleport", CONST_ME_TELEPORT},
    {"energy", CONST_ME_ENERGYHIT},
    {"blueshimmer", CONST_ME_MAGIC_BLUE},
    {"redshimmer", CONST_ME_MAGIC_RED},
    {"greenshimmer", CONST_ME_MAGIC_GREEN},
    {"fire", CONST_ME_HITBYFIRE},
    {"greenspark", CONST_ME_HITBYPOISON},
    {"mortarea", CONST_ME_MORTAREA},
    {"greennote", CONST_ME_SOUND_GREEN},
    {"rednote", CONST_ME_SOUND_RED},
    {"poison", CONST_ME_POISONAREA},
    {"yellownote", CONST_ME_SOUND_YELLOW},
    {"purplenote", CONST_ME_SOUND_PURPLE},
    {"bluenote", CONST_ME_SOUND_BLUE},
    {"whitenote", CONST_ME_SOUND_WHITE},
    {"bubbles", CONST_ME_BUBBLES},
    {"dice", CONST_ME_CRAPS},
    {"giftwraps", CONST_ME_GIFT_WRAPS},
    {"yellowfirework", CONST_ME_FIREWORK_YELLOW},
    {"redfirework", CONST_ME_FIREWORK_RED},
    {"bluefirework", CONST_ME_FIREWORK_BLUE},
    {"stun", CONST_ME_STUN},
    {"sleep", CONST_ME_SLEEP},
    {"watercreature", CONST_ME_WATERCREATURE},
    {"groundshaker", CONST_ME_GROUNDSHAKER},
    {"hearts", CONST_ME_HEARTS},
    {"fireattack", CONST_ME_FIREATTACK},
    {"energyarea", CONST_ME_ENERGYAREA},
    {"smallclouds", CONST_ME_SMALLCLOUDS},
    {"holydamage", CONST_ME_HOLYDAMAGE},
    {"bigclouds", CONST_ME_BIGCLOUDS},
    {"icearea", CONST_ME_ICEAREA},
    {"icetornado", CONST_ME_ICETORNADO},
    {"iceattack", CONST_ME_ICEATTACK},
    {"stones", CONST_ME_STONES},
    {"smallplants", CONST_ME_SMALLPLANTS},
    {"carniphila", CONST_ME_CARNIPHILA},
    {"purpleenergy", CONST_ME_PURPLEENERGY},
    {"yellowenergy", CONST_ME_YELLOWENERGY},
    {"holyarea", CONST_ME_HOLYAREA},
    {"bigplants", CONST_ME_BIGPLANTS},
    {"cake", CONST_ME_CAKE},
    {"giantice", CONST_ME_GIANTICE},
    {"watersplash", CONST_ME_WATERSPLASH},
    {"plantattack", CONST_ME_PLANTATTACK},
    {"tutorialarrow", CONST_ME_TUTORIALARROW},
    {"tutorialsquare", CONST_ME_TUTORIALSQUARE},
    {"mirrorhorizontal", CONST_ME_MIRRORHORIZONTAL},
    {"mirrorvertical", CONST_ME_MIRRORVERTICAL},
    {"skullhorizontal", CONST_ME_SKULLHORIZONTAL},
    {"skullvertical", CONST_ME_SKULLVERTICAL},
    {"assassin", CONST_ME_ASSASSIN},
    {"stepshorizontal", CONST_ME_STEPSHORIZONTAL},
    {"bloodysteps", CONST_ME_BLOODYSTEPS},
    {"stepsvertical", CONST_ME_STEPSVERTICAL},
    {"yalaharighost", CONST_ME_YALAHARIGHOST},
    {"bats", CONST_ME_BATS},
    {"smoke", CONST_ME_SMOKE},
    {"insects", CONST_ME_INSECTS},
    {"dragonhead", CONST_ME_DRAGONHEAD},
    {"orcshaman", CONST_ME_ORCSHAMAN},
    {"orcshamanfire", CONST_ME_ORCSHAMAN_FIRE},
    {"thunder", CONST_ME_THUNDER},
    {"ferumbras", CONST_ME_FERUMBRAS},
    {"confettihorizontal", CONST_ME_CONFETTI_HORIZONTAL},
    {"confettivertical", CONST_ME_CONFETTI_VERTICAL},
    {"blacksmoke", CONST_ME_BLACKSMOKE},
    {"redsmoke", CONST_ME_REDSMOKE},
    {"yellowsmoke", CONST_ME_YELLOWSMOKE},
    {"greensmoke", CONST_ME_GREENSMOKE},
    {"purplesmoke", CONST_ME_PURPLESMOKE},
    {"earlythunder", CONST_ME_EARLY_THUNDER},
    {"bonecapsule", CONST_ME_RAGIAZ_BONECAPSULE},
    {"criticaldamage", CONST_ME_CRITICAL_DAMAGE},
    {"plungingfish", CONST_ME_PLUNGING_FISH},
    {"bluechain", CONST_ME_BLUECHAIN},
    {"orangechain", CONST_ME_ORANGECHAIN},
    {"greenchain", CONST_ME_GREENCHAIN},
    {"purplechain", CONST_ME_PURPLECHAIN},
    {"greychain", CONST_ME_GREYCHAIN},
    {"yellowchain", CONST_ME_YELLOWCHAIN},
    {"yellowsparkles", CONST_ME_YELLOWSPARKLES},
    {"faeexplosion", CONST_ME_FAEEXPLOSION},
    {"faecoming", CONST_ME_FAECOMING},
    {"faegoing", CONST_ME_FAEGOING},
    {"bigcloudssinglespace", CONST_ME_BIGCLOUDSSINGLESPACE},
    {"stonessinglespace", CONST_ME_STONESSINGLESPACE},
    {"blueghost", CONST_ME_BLUEGHOST},
    {"pointofinterest", CONST_ME_POINTOFINTEREST},
    {"mapeffect", CONST_ME_MAPEFFECT},
    {"pinkspark", CONST_ME_PINKSPARK},
    {"greenfirework", CONST_ME_FIREWORK_GREEN},
    {"orangefirework", CONST_ME_FIREWORK_ORANGE},
    {"purplefirework", CONST_ME_FIREWORK_PURPLE},
    {"turquoisefirework", CONST_ME_FIREWORK_TURQUOISE},
    {"thecube", CONST_ME_THECUBE},
    {"drawink", CONST_ME_DRAWINK},
    {"prismaticsparkles", CONST_ME_PRISMATICSPARKLES},
    {"thaian", CONST_ME_THAIAN},
    {"thaianghost", CONST_ME_THAIANGHOST},
    {"ghostsmoke", CONST_ME_GHOSTSMOKE},
    {"floatingblock", CONST_ME_FLOATINGBLOCK},
    {"block", CONST_ME_BLOCK},
    {"rooting", CONST_ME_ROOTING},
    {"ghostlyscratch", CONST_ME_GHOSTLYSCRATCH},
    {"ghostlybite", CONST_ME_GHOSTLYBITE},
    {"bigscratching", CONST_ME_BIGSCRATCHING},
    {"slash", CONST_ME_SLASH},
    {"bite", CONST_ME_BITE},
    {"chivalriouschallenge", CONST_ME_CHIVALRIOUSCHALLENGE},
    {"divinedazzle", CONST_ME_DIVINEDAZZLE},
    {"electricalspark", CONST_ME_ELECTRICALSPARK},
    {"purpleteleport", CONST_ME_PURPLETELEPORT},
    {"redteleport", CONST_ME_REDTELEPORT},
    {"orangeteleport", CONST_ME_ORANGETELEPORT},
    {"greyteleport", CONST_ME_GREYTELEPORT},
    {"lightblueteleport", CONST_ME_LIGHTBLUETELEPORT},
    {"fatal", CONST_ME_FATAL},
    {"dodge", CONST_ME_DODGE},
    {"hourglass", CONST_ME_HOURGLASS},
    {"fireworksstar", CONST_ME_FIREWORKSSTAR},
    {"fireworkscircle", CONST_ME_FIREWORKSCIRCLE},
    {"ferumbras1", CONST_ME_FERUMBRAS_1},
    {"gazharagoth", CONST_ME_GAZHARAGOTH},
    {"madmage", CONST_ME_MAD_MAGE},
    {"horestis", CONST_ME_HORESTIS},
    {"devovorga", CONST_ME_DEVOVORGA},
    {"ferumbras2", CONST_ME_FERUMBRAS_2},
};

ShootTypeNames shootTypeNames = {
    {"spear", CONST_ANI_SPEAR},
    {"bolt", CONST_ANI_BOLT},
    {"arrow", CONST_ANI_ARROW},
    {"fire", CONST_ANI_FIRE},
    {"energy", CONST_ANI_ENERGY},
    {"poisonarrow", CONST_ANI_POISONARROW},
    {"burstarrow", CONST_ANI_BURSTARROW},
    {"throwingstar", CONST_ANI_THROWINGSTAR},
    {"throwingknife", CONST_ANI_THROWINGKNIFE},
    {"smallstone", CONST_ANI_SMALLSTONE},
    {"death", CONST_ANI_DEATH},
    {"largerock", CONST_ANI_LARGEROCK},
    {"snowball", CONST_ANI_SNOWBALL},
    {"powerbolt", CONST_ANI_POWERBOLT},
    {"poison", CONST_ANI_POISON},
    {"infernalbolt", CONST_ANI_INFERNALBOLT},
    {"huntingspear", CONST_ANI_HUNTINGSPEAR},
    {"enchantedspear", CONST_ANI_ENCHANTEDSPEAR},
    {"redstar", CONST_ANI_REDSTAR},
    {"greenstar", CONST_ANI_GREENSTAR},
    {"royalspear", CONST_ANI_ROYALSPEAR},
    {"sniperarrow", CONST_ANI_SNIPERARROW},
    {"onyxarrow", CONST_ANI_ONYXARROW},
    {"piercingbolt", CONST_ANI_PIERCINGBOLT},
    {"whirlwindsword", CONST_ANI_WHIRLWINDSWORD},
    {"whirlwindaxe", CONST_ANI_WHIRLWINDAXE},
    {"whirlwindclub", CONST_ANI_WHIRLWINDCLUB},
    {"etherealspear", CONST_ANI_ETHEREALSPEAR},
    {"ice", CONST_ANI_ICE},
    {"earth", CONST_ANI_EARTH},
    {"holy", CONST_ANI_HOLY},
    {"suddendeath", CONST_ANI_SUDDENDEATH},
    {"flasharrow", CONST_ANI_FLASHARROW},
    {"flammingarrow", CONST_ANI_FLAMMINGARROW},
    {"shiverarrow", CONST_ANI_SHIVERARROW},
    {"energyball", CONST_ANI_ENERGYBALL},
    {"smallice", CONST_ANI_SMALLICE},
    {"smallholy", CONST_ANI_SMALLHOLY},
    {"smallearth", CONST_ANI_SMALLEARTH},
    {"eartharrow", CONST_ANI_EARTHARROW},
    {"explosion", CONST_ANI_EXPLOSION},
    {"cake", CONST_ANI_CAKE},
    {"tarsalarrow", CONST_ANI_TARSALARROW},
    {"vortexbolt", CONST_ANI_VORTEXBOLT},
    {"prismaticbolt", CONST_ANI_PRISMATICBOLT},
    {"crystallinearrow", CONST_ANI_CRYSTALLINEARROW},
    {"drillbolt", CONST_ANI_DRILLBOLT},
    {"envenomedarrow", CONST_ANI_ENVENOMEDARROW},
    {"gloothspear", CONST_ANI_GLOOTHSPEAR},
    {"simplearrow", CONST_ANI_SIMPLEARROW},
    {"leafstar", CONST_ANI_LEAFSTAR},
    {"diamondarrow", CONST_ANI_DIAMONDARROW},
    {"spectralbolt", CONST_ANI_SPECTRALBOLT},
    {"royalstar", CONST_ANI_ROYALSTAR},
};

CombatTypeNames combatTypeNames = {
    {COMBAT_PHYSICALDAMAGE, "physical"}, {COMBAT_ENERGYDAMAGE, "energy"},       {COMBAT_EARTHDAMAGE, "earth"},
    {COMBAT_FIREDAMAGE, "fire"},         {COMBAT_UNDEFINEDDAMAGE, "undefined"}, {COMBAT_LIFEDRAIN, "lifedrain"},
    {COMBAT_MANADRAIN, "manadrain"},     {COMBAT_HEALING, "healing"},           {COMBAT_DROWNDAMAGE, "drown"},
    {COMBAT_ICEDAMAGE, "ice"},           {COMBAT_HOLYDAMAGE, "holy"},           {COMBAT_DEATHDAMAGE, "death"},
};

AmmoTypeNames ammoTypeNames = {
    {"spear", AMMO_SPEAR},
    {"bolt", AMMO_BOLT},
    {"arrow", AMMO_ARROW},
    {"poisonarrow", AMMO_ARROW},
    {"burstarrow", AMMO_ARROW},
    {"throwingstar", AMMO_THROWINGSTAR},
    {"throwingknife", AMMO_THROWINGKNIFE},
    {"smallstone", AMMO_STONE},
    {"largerock", AMMO_STONE},
    {"snowball", AMMO_SNOWBALL},
    {"powerbolt", AMMO_BOLT},
    {"infernalbolt", AMMO_BOLT},
    {"huntingspear", AMMO_SPEAR},
    {"enchantedspear", AMMO_SPEAR},
    {"royalspear", AMMO_SPEAR},
    {"sniperarrow", AMMO_ARROW},
    {"onyxarrow", AMMO_ARROW},
    {"piercingbolt", AMMO_BOLT},
    {"etherealspear", AMMO_SPEAR},
    {"flasharrow", AMMO_ARROW},
    {"flammingarrow", AMMO_ARROW},
    {"shiverarrow", AMMO_ARROW},
    {"eartharrow", AMMO_ARROW},
    {"tarsalarrow", AMMO_ARROW},
    {"vortexbolt", AMMO_BOLT},
    {"prismaticbolt", AMMO_BOLT},
    {"crystallinearrow", AMMO_ARROW},
    {"drillbolt", AMMO_BOLT},
    {"envenomedarrow", AMMO_ARROW},
    {"gloothspear", AMMO_SPEAR},
    {"simplearrow", AMMO_ARROW},
    {"redstar", AMMO_THROWINGSTAR},
    {"greenstar", AMMO_THROWINGSTAR},
    {"leafstar", AMMO_THROWINGSTAR},
    {"diamondarrow", AMMO_ARROW},
    {"spectralbolt", AMMO_BOLT},
    {"royalstar", AMMO_THROWINGSTAR},
};

WeaponActionNames weaponActionNames = {
    {"move", WEAPONACTION_MOVE},
    {"removecharge", WEAPONACTION_REMOVECHARGE},
    {"removecount", WEAPONACTION_REMOVECOUNT},
};

SkullNames skullNames = {
    {"none", SKULL_NONE}, {"yellow", SKULL_YELLOW}, {"green", SKULL_GREEN},   {"white", SKULL_WHITE},
    {"red", SKULL_RED},   {"black", SKULL_BLACK},   {"orange", SKULL_ORANGE},
};

std::vector<uint16_t> depotBoxes = {ITEM_DEPOT_BOX_I,    ITEM_DEPOT_BOX_II,   ITEM_DEPOT_BOX_III, ITEM_DEPOT_BOX_IV,
                                    ITEM_DEPOT_BOX_V,    ITEM_DEPOT_BOX_VI,   ITEM_DEPOT_BOX_VII, ITEM_DEPOT_BOX_VIII,
                                    ITEM_DEPOT_BOX_IX,   ITEM_DEPOT_BOX_X,    ITEM_DEPOT_BOX_XI,  ITEM_DEPOT_BOX_XII,
                                    ITEM_DEPOT_BOX_XIII, ITEM_DEPOT_BOX_XIV,  ITEM_DEPOT_BOX_XV,  ITEM_DEPOT_BOX_XVI,
                                    ITEM_DEPOT_BOX_XVII, ITEM_DEPOT_BOX_XVIII};

uint16_t getDepotBoxId(uint16_t index)
{
	if (index >= depotBoxes.size()) {
		return 0;
	}
	return depotBoxes[index];
}

MagicEffectClasses getMagicEffect(const std::string& strValue)
{
	auto magicEffect = magicEffectNames.find(strValue);
	if (magicEffect != magicEffectNames.end()) {
		return magicEffect->second;
	}
	return CONST_ME_NONE;
}

ShootType_t getShootType(const std::string& strValue)
{
	auto shootType = shootTypeNames.find(strValue);
	if (shootType != shootTypeNames.end()) {
		return shootType->second;
	}
	return CONST_ANI_NONE;
}

std::string getCombatName(CombatType_t combatType)
{
	auto combatName = combatTypeNames.find(combatType);
	if (combatName != combatTypeNames.end()) {
		return combatName->second;
	}
	return "unknown";
}

Ammo_t getAmmoType(const std::string& strValue)
{
	auto ammoType = ammoTypeNames.find(strValue);
	if (ammoType != ammoTypeNames.end()) {
		return ammoType->second;
	}
	return AMMO_NONE;
}

WeaponAction_t getWeaponAction(const std::string& strValue)
{
	auto weaponAction = weaponActionNames.find(strValue);
	if (weaponAction != weaponActionNames.end()) {
		return weaponAction->second;
	}
	return WEAPONACTION_NONE;
}

Skulls_t getSkullType(const std::string& strValue)
{
	auto skullType = skullNames.find(strValue);
	if (skullType != skullNames.end()) {
		return skullType->second;
	}
	return SKULL_NONE;
}

std::string getSpecialSkillName(uint8_t skillid)
{
	switch (skillid) {
		case SPECIALSKILL_CRITICALHITCHANCE:
			return "critical hit chance";

		case SPECIALSKILL_CRITICALHITAMOUNT:
			return "critical extra damage";

		case SPECIALSKILL_LIFELEECHCHANCE:
			return "hitpoints leech chance";

		case SPECIALSKILL_LIFELEECHAMOUNT:
			return "hitpoints leech amount";

		case SPECIALSKILL_MANALEECHCHANCE:
			return "manapoints leech chance";

		case SPECIALSKILL_MANALEECHAMOUNT:
			return "mana points leech amount";

		default:
			return "unknown";
	}
}

std::string getSkillName(uint8_t skillid)
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

uint32_t adlerChecksum(const uint8_t* data, size_t length)
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
	for (char& i : str) {
		if (i != ' ') {
			i = toupper(i);
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

size_t combatTypeToIndex(CombatType_t combatType)
{
	switch (combatType) {
		case COMBAT_PHYSICALDAMAGE:
			return 0;
		case COMBAT_ENERGYDAMAGE:
			return 1;
		case COMBAT_EARTHDAMAGE:
			return 2;
		case COMBAT_FIREDAMAGE:
			return 3;
		case COMBAT_UNDEFINEDDAMAGE:
			return 4;
		case COMBAT_LIFEDRAIN:
			return 5;
		case COMBAT_MANADRAIN:
			return 6;
		case COMBAT_HEALING:
			return 7;
		case COMBAT_DROWNDAMAGE:
			return 8;
		case COMBAT_ICEDAMAGE:
			return 9;
		case COMBAT_HOLYDAMAGE:
			return 10;
		case COMBAT_DEATHDAMAGE:
			return 11;
		default:
			return 0;
	}
}

CombatType_t indexToCombatType(size_t v) { return static_cast<CombatType_t>(1 << v); }

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
	uint8_t size = sizeof(clientToServerFluidMap) / sizeof(uint8_t);
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
	} else if (str == "name") {
		return ITEM_ATTRIBUTE_NAME;
	} else if (str == "article") {
		return ITEM_ATTRIBUTE_ARTICLE;
	} else if (str == "pluralname") {
		return ITEM_ATTRIBUTE_PLURALNAME;
	} else if (str == "weight") {
		return ITEM_ATTRIBUTE_WEIGHT;
	} else if (str == "attack") {
		return ITEM_ATTRIBUTE_ATTACK;
	} else if (str == "defense") {
		return ITEM_ATTRIBUTE_DEFENSE;
	} else if (str == "extradefense") {
		return ITEM_ATTRIBUTE_EXTRADEFENSE;
	} else if (str == "armor") {
		return ITEM_ATTRIBUTE_ARMOR;
	} else if (str == "hitchance") {
		return ITEM_ATTRIBUTE_HITCHANCE;
	} else if (str == "shootrange") {
		return ITEM_ATTRIBUTE_SHOOTRANGE;
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
	} else if (str == "decayto") {
		return ITEM_ATTRIBUTE_DECAYTO;
	} else if (str == "wrapid") {
		return ITEM_ATTRIBUTE_WRAPID;
	} else if (str == "storeitem") {
		return ITEM_ATTRIBUTE_STOREITEM;
	} else if (str == "attackspeed") {
		return ITEM_ATTRIBUTE_ATTACK_SPEED;
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
		case RETURNVALUE_DESTINATIONOUTOFREACH:
			return "Destination is out of range.";

		case RETURNVALUE_NOTMOVEABLE:
			return "You cannot move this object.";

		case RETURNVALUE_DROPTWOHANDEDITEM:
			return "Drop the double-handed object first.";

		case RETURNVALUE_BOTHHANDSNEEDTOBEFREE:
			return "Both hands need to be free.";

		case RETURNVALUE_CANNOTBEDRESSED:
			return "You cannot dress this object there.";

		case RETURNVALUE_PUTTHISOBJECTINYOURHAND:
			return "Put this object in your hand.";

		case RETURNVALUE_PUTTHISOBJECTINBOTHHANDS:
			return "Put this object in both hands.";

		case RETURNVALUE_CANONLYUSEONEWEAPON:
			return "You may only use one weapon.";

		case RETURNVALUE_TOOFARAWAY:
			return "You are too far away.";

		case RETURNVALUE_FIRSTGODOWNSTAIRS:
			return "First go downstairs.";

		case RETURNVALUE_FIRSTGOUPSTAIRS:
			return "First go upstairs.";

		case RETURNVALUE_NOTENOUGHCAPACITY:
			return "This object is too heavy for you to carry.";

		case RETURNVALUE_CONTAINERNOTENOUGHROOM:
			return "You cannot put more objects in this container.";

		case RETURNVALUE_NEEDEXCHANGE:
		case RETURNVALUE_NOTENOUGHROOM:
			return "There is not enough room.";

		case RETURNVALUE_CANNOTPICKUP:
			return "You cannot take this object.";

		case RETURNVALUE_CANNOTTHROW:
			return "You cannot throw there.";

		case RETURNVALUE_THEREISNOWAY:
			return "There is no way.";

		case RETURNVALUE_THISISIMPOSSIBLE:
			return "This is impossible.";

		case RETURNVALUE_PLAYERISPZLOCKED:
			return "You can not enter a protection zone after attacking another player.";

		case RETURNVALUE_PLAYERISNOTINVITED:
			return "You are not invited.";

		case RETURNVALUE_CREATUREDOESNOTEXIST:
			return "Creature does not exist.";

		case RETURNVALUE_DEPOTISFULL:
			return "You cannot put more items in this depot.";

		case RETURNVALUE_CANNOTUSETHISOBJECT:
			return "You cannot use this object.";

		case RETURNVALUE_PLAYERWITHTHISNAMEISNOTONLINE:
			return "A player with this name is not online.";

		case RETURNVALUE_NOTREQUIREDLEVELTOUSERUNE:
			return "You do not have the required magic level to use this rune.";

		case RETURNVALUE_YOUAREALREADYTRADING:
			return "You are already trading. Finish this trade first.";

		case RETURNVALUE_THISPLAYERISALREADYTRADING:
			return "This player is already trading.";

		case RETURNVALUE_YOUMAYNOTLOGOUTDURINGAFIGHT:
			return "You may not logout during or immediately after a fight!";

		case RETURNVALUE_DIRECTPLAYERSHOOT:
			return "You are not allowed to shoot directly on players.";

		case RETURNVALUE_NOTENOUGHLEVEL:
			return "Your level is too low.";

		case RETURNVALUE_NOTENOUGHMAGICLEVEL:
			return "You do not have enough magic level.";

		case RETURNVALUE_NOTENOUGHMANA:
			return "You do not have enough mana.";

		case RETURNVALUE_NOTENOUGHSOUL:
			return "You do not have enough soul.";

		case RETURNVALUE_YOUAREEXHAUSTED:
			return "You are exhausted.";

		case RETURNVALUE_YOUCANNOTUSEOBJECTSTHATFAST:
			return "You cannot use objects that fast.";

		case RETURNVALUE_CANONLYUSETHISRUNEONCREATURES:
			return "You can only use it on creatures.";

		case RETURNVALUE_PLAYERISNOTREACHABLE:
			return "Player is not reachable.";

		case RETURNVALUE_CREATUREISNOTREACHABLE:
			return "Creature is not reachable.";

		case RETURNVALUE_ACTIONNOTPERMITTEDINPROTECTIONZONE:
			return "This action is not permitted in a protection zone.";

		case RETURNVALUE_YOUMAYNOTATTACKTHISPLAYER:
			return "You may not attack this person.";

		case RETURNVALUE_YOUMAYNOTATTACKTHISCREATURE:
			return "You may not attack this creature.";

		case RETURNVALUE_YOUMAYNOTATTACKAPERSONINPROTECTIONZONE:
			return "You may not attack a person in a protection zone.";

		case RETURNVALUE_YOUMAYNOTATTACKAPERSONWHILEINPROTECTIONZONE:
			return "You may not attack a person while you are in a protection zone.";

		case RETURNVALUE_YOUCANONLYUSEITONCREATURES:
			return "You can only use it on creatures.";

		case RETURNVALUE_TURNSECUREMODETOATTACKUNMARKEDPLAYERS:
			return "Turn secure mode off if you really want to attack unmarked players.";

		case RETURNVALUE_YOUNEEDPREMIUMACCOUNT:
			return "You need a premium account.";

		case RETURNVALUE_YOUNEEDTOLEARNTHISSPELL:
			return "You must learn this spell first.";

		case RETURNVALUE_YOURVOCATIONCANNOTUSETHISSPELL:
			return "You have the wrong vocation to cast this spell.";

		case RETURNVALUE_YOUNEEDAWEAPONTOUSETHISSPELL:
			return "You need to equip a weapon to use this spell.";

		case RETURNVALUE_PLAYERISPZLOCKEDLEAVEPVPZONE:
			return "You can not leave a pvp zone after attacking another player.";

		case RETURNVALUE_PLAYERISPZLOCKEDENTERPVPZONE:
			return "You can not enter a pvp zone after attacking another player.";

		case RETURNVALUE_ACTIONNOTPERMITTEDINANOPVPZONE:
			return "This action is not permitted in a non pvp zone.";

		case RETURNVALUE_YOUCANNOTLOGOUTHERE:
			return "You can not logout here.";

		case RETURNVALUE_YOUNEEDAMAGICITEMTOCASTSPELL:
			return "You need a magic item to cast this spell.";

		case RETURNVALUE_NAMEISTOOAMBIGUOUS:
			return "Player name is ambiguous.";

		case RETURNVALUE_CANONLYUSEONESHIELD:
			return "You may use only one shield.";

		case RETURNVALUE_NOPARTYMEMBERSINRANGE:
			return "No party members in range.";

		case RETURNVALUE_YOUARENOTTHEOWNER:
			return "You are not the owner.";

		case RETURNVALUE_NOSUCHRAIDEXISTS:
			return "No such raid exists.";

		case RETURNVALUE_ANOTHERRAIDISALREADYEXECUTING:
			return "Another raid is already executing.";

		case RETURNVALUE_TRADEPLAYERFARAWAY:
			return "Trade player is too far away.";

		case RETURNVALUE_YOUDONTOWNTHISHOUSE:
			return "You don't own this house.";

		case RETURNVALUE_TRADEPLAYERALREADYOWNSAHOUSE:
			return "Trade player already owns a house.";

		case RETURNVALUE_TRADEPLAYERHIGHESTBIDDER:
			return "Trade player is currently the highest bidder of an auctioned house.";

		case RETURNVALUE_YOUCANNOTTRADETHISHOUSE:
			return "You can not trade this house.";

		case RETURNVALUE_YOUDONTHAVEREQUIREDPROFESSION:
			return "You don't have the required profession.";

		case RETURNVALUE_CANNOTMOVEITEMISNOTSTOREITEM:
			return "You cannot move this item into your Store inbox as it was not bought in the Store.";

		case RETURNVALUE_ITEMCANNOTBEMOVEDTHERE:
			return "This item cannot be moved there.";

		case RETURNVALUE_YOUCANNOTUSETHISBED:
			return "This bed can't be used, but Premium Account players can rent houses and sleep in beds there to regain health and mana.";

		case RETURNVALUE_QUIVERAMMOONLY:
			return "This quiver only holds arrows and bolts.\nYou cannot put any other items in it.";

		default: // RETURNVALUE_NOTPOSSIBLE, etc
			return "Sorry, not possible.";
	}
}

int64_t OTSYS_TIME()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
	    .count();
}

SpellGroup_t stringToSpellGroup(const std::string& value)
{
	std::string tmpStr = boost::algorithm::to_lower_copy(value);
	if (tmpStr == "attack" || tmpStr == "1") {
		return SPELLGROUP_ATTACK;
	} else if (tmpStr == "healing" || tmpStr == "2") {
		return SPELLGROUP_HEALING;
	} else if (tmpStr == "support" || tmpStr == "3") {
		return SPELLGROUP_SUPPORT;
	} else if (tmpStr == "special" || tmpStr == "4") {
		return SPELLGROUP_SPECIAL;
	}

	return SPELLGROUP_NONE;
}

std::vector<uint16_t> validLookTypes = {
    0,    2,    3,    4,    5,    6,    7,    8,    9,    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
    39,   40,   41,   42,   43,   44,   45,   46,   47,   48,   49,   50,   51,   52,   53,   54,   55,   56,   57,
    58,   59,   60,   61,   62,   63,   64,   65,   66,   67,   68,   69,   70,   71,   72,   73,   74,   75,   76,
    77,   78,   79,   80,   81,   82,   83,   84,   85,   86,   87,   88,   89,   90,   91,   92,   93,   94,   95,
    96,   97,   98,   99,   100,  101,  102,  103,  104,  105,  106,  107,  108,  109,  110,  111,  112,  113,  114,
    115,  116,  117,  118,  119,  120,  121,  122,  123,  124,  125,  126,  127,  128,  129,  130,  131,  132,  133,
    134,  136,  137,  138,  139,  140,  141,  142,  143,  144,  145,  146,  147,  148,  149,  150,  151,  152,  153,
    154,  155,  156,  157,  158,  159,  160,  192,  193,  194,  195,  196,  197,  198,  199,  200,  201,  202,  203,
    204,  205,  206,  207,  208,  209,  210,  211,  212,  213,  214,  215,  216,  217,  218,  219,  220,  221,  222,
    223,  224,  225,  226,  227,  228,  229,  230,  231,  232,  233,  234,  235,  236,  237,  238,  239,  240,  241,
    242,  243,  244,  245,  246,  247,  248,  249,  250,  251,  252,  253,  254,  255,  256,  257,  258,  259,  260,
    261,  262,  263,  264,  265,  266,  267,  268,  269,  270,  271,  272,  273,  274,  275,  276,  277,  278,  279,
    280,  281,  282,  283,  284,  285,  286,  287,  288,  289,  290,  291,  292,  293,  294,  295,  296,  297,  298,
    299,  300,  301,  302,  303,  304,  305,  306,  307,  308,  309,  310,  311,  312,  313,  314,  315,  316,  317,
    318,  319,  320,  321,  322,  323,  324,  325,  326,  327,  328,  329,  330,  331,  332,  333,  334,  335,  336,
    337,  338,  339,  340,  341,  342,  343,  344,  345,  346,  347,  348,  349,  350,  351,  352,  353,  354,  355,
    356,  357,  358,  359,  360,  361,  362,  363,  364,  365,  366,  367,  368,  369,  370,  371,  372,  373,  374,
    375,  376,  377,  378,  379,  380,  381,  382,  383,  384,  385,  386,  387,  388,  389,  390,  391,  392,  393,
    394,  395,  396,  397,  398,  399,  400,  401,  402,  403,  404,  405,  406,  407,  408,  409,  410,  412,  413,
    414,  416,  417,  418,  419,  420,  421,  422,  423,  425,  426,  427,  428,  429,  430,  431,  432,  433,  434,
    435,  436,  437,  438,  441,  442,  443,  444,  445,  446,  447,  448,  449,  450,  451,  452,  453,  454,  455,
    456,  457,  458,  459,  460,  461,  462,  463,  464,  465,  466,  467,  470,  471,  472,  473,  486,  487,  488,
    489,  490,  491,  492,  493,  494,  495,  496,  497,  498,  499,  500,  502,  503,  504,  505,  506,  507,  508,
    509,  510,  511,  512,  513,  514,  515,  516,  517,  521,  522,  523,  526,  527,  528,  529,  530,  531,  532,
    533,  534,  535,  537,  538,  539,  540,  541,  542,  544,  545,  546,  547,  548,  550,  551,  552,  553,  554,
    555,  556,  557,  558,  559,  560,  561,  562,  563,  564,  565,  566,  567,  568,  569,  570,  571,  572,  573,
    574,  575,  577,  578,  579,  580,  583,  584,  585,  586,  587,  588,  589,  590,  591,  592,  593,  594,  595,
    596,  598,  599,  600,  601,  602,  603,  604,  605,  606,  607,  608,  609,  610,  611,  612,  613,  614,  615,
    617,  618,  619,  620,  621,  622,  624,  626,  627,  628,  629,  630,  631,  632,  633,  634,  635,  636,  637,
    644,  647,  648,  649,  650,  651,  664,  665,  666,  667,  668,  669,  670,  671,  672,  673,  674,  675,  676,
    677,  679,  680,  681,  682,  683,  684,  685,  686,  687,  688,  689,  690,  691,  692,  693,  694,  695,  696,
    697,  698,  699,  712,  714,  716,  717,  720,  721,  724,  725,  726,  727,  728,  729,  730,  731,  732,  733,
    734,  735,  736,  738,  739,  740,  745,  746,  747,  749,  750,  759,  760,  761,  762,  763,  842,  843,  844,
    845,  846,  848,  849,  850,  851,  852,  853,  854,  855,  856,  857,  858,  859,  860,  862,  863,  868,  869,
    870,  873,  874,  875,  876,  877,  878,  879,  881,  882,  883,  884,  885,  886,  887,  888,  889,  890,  899,
    900,  901,  902,  903,  904,  905,  906,  907,  908,  909,  910,  913,  914,  915,  916,  918,  919,  920,  921,
    922,  923,  924,  925,  926,  927,  928,  929,  931,  932,  933,  934,  935,  936,  937,  938,  939,  940,  943,
    944,  945,  947,  948,  949,  950,  951,  952,  955,  956,  957,  958,  959,  960,  961,  962,  963,  964,  965,
    966,  967,  968,  969,  970,  971,  972,  973,  974,  975,  976,  977,  978,  979,  980,  981,  982,  984,  985,
    986,  987,  988,  989,  990,  991,  992,  993,  994,  1011, 1013, 1016, 1017, 1018, 1019, 1020, 1021, 1023, 1024,
    1025, 1026, 1027, 1029, 1030, 1031, 1032, 1033, 1034, 1035, 1036, 1037, 1038, 1039, 1040, 1041, 1042, 1043, 1044,
    1045, 1046, 1047, 1048, 1049, 1050, 1051, 1052, 1053, 1054, 1055, 1056, 1057, 1058, 1059, 1060, 1061, 1062, 1063,
    1064, 1065, 1066, 1067, 1068, 1069, 1070, 1071, 1072, 1073, 1076, 1077, 1078, 1079, 1088, 1091, 1092, 1093, 1094,
    1095, 1101, 1102, 1103, 1104, 1105, 1106, 1107, 1108, 1109, 1110, 1111, 1112, 1113, 1114, 1115, 1116, 1117, 1118,
    1119, 1120, 1121, 1122, 1123, 1124, 1125, 1126, 1127, 1128, 1129, 1130, 1131, 1132, 1133, 1134, 1135, 1136, 1137,
    1138, 1139, 1140, 1142, 1143, 1144, 1146, 1147, 1148, 1149, 1150, 1151, 1152, 1157, 1158, 1159, 1161, 1162, 1163,
    1164, 1165, 1166, 1167, 1168, 1169, 1173, 1174, 1175, 1179, 1180, 1181, 1183, 1184, 1185, 1186, 1187, 1188, 1189,
    1190, 1191, 1195, 1196, 1197, 1199, 1200, 1201, 1202, 1203, 1204, 1205, 1206, 1207, 1208, 1209, 1210, 1211, 1212,
    1213, 1214, 1217, 1218, 1219, 1220, 1221, 1222, 1223, 1224, 1229, 1230, 1231, 1232, 1233, 1234, 1243, 1244, 1245,
    1246, 1247, 1248, 1249, 1251, 1252, 1253, 1255, 1256, 1257, 1258, 1259, 1260, 1261, 1262, 1264, 1265, 1266, 1268,
    1269, 1270, 1271, 1272, 1275, 1276, 1277, 1278, 1279, 1280, 1281, 1282, 1283, 1284, 1285, 1286, 1288, 1289, 1290,
    1291, 1292, 1293, 1294, 1295, 1296, 1297, 1298, 1299, 1300, 1301, 1303, 1304, 1305, 1306, 1307, 1308, 1309, 1310,
    1311, 1312, 1313, 1314, 1315, 1316, 1317, 1321, 1322, 1323, 1324, 1325, 1326, 1331, 1332, 1333, 1334, 1335, 1336,
    1337, 1338, 1339, 1341, 1342, 1344, 1346, 1363, 1364, 1365, 1366, 1367, 1371, 1372, 1373, 1377, 1378, 1379, 1380,
    1381, 1382, 1383, 1384, 1385, 1386, 1387, 1389, 1390, 1391, 1393, 1394, 1396, 1397, 1398, 1399, 1401, 1403, 1405,
    1406, 1407, 1408, 1412, 1413, 1414, 1415, 1416, 1417, 1418, 1419, 1422, 1423, 1424, 1426, 1430, 1431, 1436, 1437,
    1439, 1440, 1441, 1444, 1445, 1446, 1447, 1448, 1449, 1450, 1453, 1454, 1455, 1456, 1457, 1459, 1460, 1461};

const std::vector<uint16_t> getValidLookTypes() { return validLookTypes; }

bool isValidLookType(uint16_t lookTypeId)
{
	auto it = std::find(validLookTypes.begin(), validLookTypes.end(), lookTypeId);
	return it != std::end(validLookTypes);
}
