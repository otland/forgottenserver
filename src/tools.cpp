// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "tools.h"

#include <chrono>
#include <openssl/evp.h>
#include <openssl/hmac.h>

void printXMLError(const std::string& where, std::string_view fileName, const pugi::xml_parse_result& result)
{
	std::cout << '[' << where << "] Failed to load " << fileName << ": " << result.description() << std::endl;

	FILE* file = fopen(fileName.data(), "rb");
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

std::string transformToSHA1(std::string_view input)
{
	std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)> ctx{EVP_MD_CTX_new(), EVP_MD_CTX_free};
	if (!ctx) {
		throw std::runtime_error("Failed to create EVP context");
	}

	std::unique_ptr<EVP_MD, decltype(&EVP_MD_free)> md{EVP_MD_fetch(nullptr, "SHA1", nullptr), EVP_MD_free};
	if (!md) {
		throw std::runtime_error("Failed to fetch SHA1");
	}

	if (!EVP_DigestInit_ex(ctx.get(), md.get(), nullptr)) {
		throw std::runtime_error("Message digest initialization failed");
	}

	if (!EVP_DigestUpdate(ctx.get(), input.data(), input.size())) {
		throw std::runtime_error("Message digest update failed");
	}

	unsigned int len = EVP_MD_size(md.get());
	std::string digest(static_cast<size_t>(len), '\0');
	if (!EVP_DigestFinal_ex(ctx.get(), reinterpret_cast<unsigned char*>(digest.data()), &len)) {
		throw std::runtime_error("Message digest finalization failed");
	}

	return digest;
}

std::string hmac(std::string_view algorithm, std::string_view key, std::string_view message)
{
	std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)> ctx{EVP_MD_CTX_new(), EVP_MD_CTX_free};
	if (!ctx) {
		throw std::runtime_error("Failed to create EVP context");
	}

	std::unique_ptr<EVP_MD, decltype(&EVP_MD_free)> md{EVP_MD_fetch(nullptr, algorithm.data(), nullptr), EVP_MD_free};
	if (!md) {
		throw std::runtime_error(fmt::format("Failed to fetch {:s}", algorithm));
	}

	std::array<unsigned char, EVP_MAX_MD_SIZE> result;
	unsigned int len;

	if (!HMAC(md.get(), key.data(), key.size(), reinterpret_cast<const unsigned char*>(message.data()), message.size(),
	          result.data(), &len)) {
		throw std::runtime_error("HMAC failed");
	}

	return {reinterpret_cast<char*>(result.data()), len};
}

std::string generateToken(std::string_view key, uint64_t counter, size_t length /*= AUTHENTICATOR_DIGITS*/)
{
	std::string mac(8, 0);
	for (uint8_t i = 8; --i; counter >>= 8) {
		mac[i] = static_cast<char>(counter % 256);
	}

	mac = hmac("SHA1", key, mac);

	// calculate hmac offset
	auto offset = mac.back() % 16u;

	// get truncated hash
	uint32_t p =
	    (static_cast<unsigned char>(mac[offset + 0]) << 24u) | (static_cast<unsigned char>(mac[offset + 1]) << 16u) |
	    (static_cast<unsigned char>(mac[offset + 2]) << 8u) | (static_cast<unsigned char>(mac[offset + 3]) << 0u);

	auto token = std::to_string(p & 0x7fffffff);
	return token.substr(token.size() - length);
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

std::vector<std::string_view> explodeString(std::string_view inString, const std::string& separator,
                                            int32_t limit /* = -1*/)
{
	std::vector<std::string_view> returnVector;
	std::string_view::size_type start = 0, end = 0;

	while (--limit != -1 && (end = inString.find(separator, start)) != std::string_view::npos) {
		returnVector.push_back(inString.substr(start, end - start));
		start = end + separator.size();
	}

	returnVector.push_back(inString.substr(start));
	return returnVector;
}

IntegerVector vectorAtoi(const std::vector<std::string_view>& stringVector)
{
	IntegerVector returnVector;
	for (const auto& string : stringVector) {
		returnVector.push_back(std::stoi(string.data()));
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

	float v;
	do {
		v = normalRand(getRandomGenerator());
	} while (v < 0.0 || v > 1.0);

	auto&& [a, b] = std::minmax(minNumber, maxNumber);
	return a + std::lround(v * (b - a));
}

bool boolean_random(double probability /* = 0.5*/)
{
	static std::bernoulli_distribution booleanRand;
	return booleanRand(getRandomGenerator(), std::bernoulli_distribution::param_type(probability));
}

std::string randomBytes(size_t length)
{
	static std::uniform_int_distribution<unsigned> distribution(0, 255);
	static auto& generator = getRandomGenerator();

	std::string bytes(length, '\x00');
	std::generate(bytes.begin(), bytes.end(), []() { return static_cast<char>(distribution(generator)); });
	return bytes;
}

std::string formatDateShort(time_t time)
{
	return std::format("{:%d %b %Y}", std::chrono::system_clock::from_time_t(time));
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

	int32_t x_offset = from.getOffsetX(to);
	if (x_offset < 0) {
		dir = DIRECTION_EAST;
		x_offset = std::abs(x_offset);
	} else {
		dir = DIRECTION_WEST;
	}

	int32_t y_offset = from.getOffsetY(to);
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
    {"foam", CONST_ME_FOAM},
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

std::vector<uint16_t> depotBoxes = {
    ITEM_DEPOT_BOX_I,   ITEM_DEPOT_BOX_II,   ITEM_DEPOT_BOX_III,   ITEM_DEPOT_BOX_IV,  ITEM_DEPOT_BOX_V,
    ITEM_DEPOT_BOX_VI,  ITEM_DEPOT_BOX_VII,  ITEM_DEPOT_BOX_VIII,  ITEM_DEPOT_BOX_IX,  ITEM_DEPOT_BOX_X,
    ITEM_DEPOT_BOX_XI,  ITEM_DEPOT_BOX_XII,  ITEM_DEPOT_BOX_XIII,  ITEM_DEPOT_BOX_XIV, ITEM_DEPOT_BOX_XV,
    ITEM_DEPOT_BOX_XVI, ITEM_DEPOT_BOX_XVII, ITEM_DEPOT_BOX_XVIII, ITEM_DEPOT_BOX_XIX, ITEM_DEPOT_BOX_XX,
};

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

bool booleanString(std::string_view str)
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
	return duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
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

const std::vector<Direction>& getShuffleDirections()
{
	static std::vector<Direction> dirList{DIRECTION_NORTH, DIRECTION_WEST, DIRECTION_EAST, DIRECTION_SOUTH};
	std::shuffle(dirList.begin(), dirList.end(), getRandomGenerator());
	return dirList;
}
