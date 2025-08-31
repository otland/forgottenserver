// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "items.h"

#include "movement.h"
#include "pugicast.h"
#include "weapons.h"

extern MoveEvents* g_moveEvents;
extern Weapons* g_weapons;

namespace {

constexpr uint8_t ROOT_ATTR_VERSION = 0x01;

enum itemattrib_t
{
	ITEM_ATTR_FIRST = 0x10,
	ITEM_ATTR_SERVERID = ITEM_ATTR_FIRST,
	ITEM_ATTR_CLIENTID,
	ITEM_ATTR_NAME,
	ITEM_ATTR_DESCR,
	ITEM_ATTR_SPEED,
	ITEM_ATTR_SLOT,
	ITEM_ATTR_MAXITEMS,
	ITEM_ATTR_WEIGHT,
	ITEM_ATTR_WEAPON,
	ITEM_ATTR_AMU,
	ITEM_ATTR_ARMOR,
	ITEM_ATTR_MAGLEVEL,
	ITEM_ATTR_MAGFIELDTYPE,
	ITEM_ATTR_WRITEABLE,
	ITEM_ATTR_ROTATETO,
	ITEM_ATTR_DECAY,
	ITEM_ATTR_SPRITEHASH,
	ITEM_ATTR_MINIMAPCOLOR,
	ITEM_ATTR_07,
	ITEM_ATTR_08,
	ITEM_ATTR_LIGHT,

	// 1-byte aligned
	ITEM_ATTR_DECAY2,     // deprecated
	ITEM_ATTR_WEAPON2,    // deprecated
	ITEM_ATTR_AMU2,       // deprecated
	ITEM_ATTR_ARMOR2,     // deprecated
	ITEM_ATTR_WRITEABLE2, // deprecated
	ITEM_ATTR_LIGHT2,
	ITEM_ATTR_TOPORDER,
	ITEM_ATTR_WRITEABLE3, // deprecated

	ITEM_ATTR_WAREID,
	ITEM_ATTR_CLASSIFICATION,

	ITEM_ATTR_LAST
};

enum itemflags_t
{
	FLAG_BLOCK_SOLID = 1 << 0,
	FLAG_BLOCK_PROJECTILE = 1 << 1,
	FLAG_BLOCK_PATHFIND = 1 << 2,
	FLAG_HAS_HEIGHT = 1 << 3,
	FLAG_USEABLE = 1 << 4,
	FLAG_PICKUPABLE = 1 << 5,
	FLAG_MOVEABLE = 1 << 6,
	FLAG_STACKABLE = 1 << 7,
	FLAG_FLOORCHANGEDOWN = 1 << 8,   // unused
	FLAG_FLOORCHANGENORTH = 1 << 9,  // unused
	FLAG_FLOORCHANGEEAST = 1 << 10,  // unused
	FLAG_FLOORCHANGESOUTH = 1 << 11, // unused
	FLAG_FLOORCHANGEWEST = 1 << 12,  // unused
	FLAG_ALWAYSONTOP = 1 << 13,
	FLAG_READABLE = 1 << 14,
	FLAG_ROTATABLE = 1 << 15,
	FLAG_HANGABLE = 1 << 16,
	FLAG_VERTICAL = 1 << 17,
	FLAG_HORIZONTAL = 1 << 18,
	FLAG_CANNOTDECAY = 1 << 19, // unused
	FLAG_ALLOWDISTREAD = 1 << 20,
	FLAG_CLIENTDURATION = 1 << 21,
	FLAG_CLIENTCHARGES = 1 << 22,
	FLAG_LOOKTHROUGH = 1 << 23,
	FLAG_ANIMATION = 1 << 24,
	FLAG_FULLTILE = 1 << 25, // unused
	FLAG_FORCEUSE = 1 << 26,
	FLAG_AMMO = 1 << 27,       // unused
	FLAG_REPORTABLE = 1 << 28, // unused
};

const std::unordered_map<std::string, ItemParseAttributes_t> ItemParseAttributesMap = {
    {"type", ITEM_PARSE_TYPE},
    {"description", ITEM_PARSE_DESCRIPTION},
    {"runespellname", ITEM_PARSE_RUNESPELLNAME},
    {"weight", ITEM_PARSE_WEIGHT},
    {"showcount", ITEM_PARSE_SHOWCOUNT},
    {"armor", ITEM_PARSE_ARMOR},
    {"defense", ITEM_PARSE_DEFENSE},
    {"extradef", ITEM_PARSE_EXTRADEF},
    {"attack", ITEM_PARSE_ATTACK},
    {"attackspeed", ITEM_PARSE_ATTACK_SPEED},
    {"rotateto", ITEM_PARSE_ROTATETO},
    {"moveable", ITEM_PARSE_MOVEABLE},
    {"movable", ITEM_PARSE_MOVEABLE},
    {"blockprojectile", ITEM_PARSE_BLOCKPROJECTILE},
    {"allowpickupable", ITEM_PARSE_PICKUPABLE},
    {"pickupable", ITEM_PARSE_PICKUPABLE},
    {"forceserialize", ITEM_PARSE_FORCESERIALIZE},
    {"forcesave", ITEM_PARSE_FORCESERIALIZE},
    {"floorchange", ITEM_PARSE_FLOORCHANGE},
    {"corpsetype", ITEM_PARSE_CORPSETYPE},
    {"containersize", ITEM_PARSE_CONTAINERSIZE},
    {"fluidsource", ITEM_PARSE_FLUIDSOURCE},
    {"readable", ITEM_PARSE_READABLE},
    {"writeable", ITEM_PARSE_WRITEABLE},
    {"maxtextlen", ITEM_PARSE_MAXTEXTLEN},
    {"writeonceitemid", ITEM_PARSE_WRITEONCEITEMID},
    {"weapontype", ITEM_PARSE_WEAPONTYPE},
    {"slottype", ITEM_PARSE_SLOTTYPE},
    {"ammotype", ITEM_PARSE_AMMOTYPE},
    {"shoottype", ITEM_PARSE_SHOOTTYPE},
    {"effect", ITEM_PARSE_EFFECT},
    {"range", ITEM_PARSE_RANGE},
    {"stopduration", ITEM_PARSE_STOPDURATION},
    {"decayto", ITEM_PARSE_DECAYTO},
    {"transformequipto", ITEM_PARSE_TRANSFORMEQUIPTO},
    {"transformdeequipto", ITEM_PARSE_TRANSFORMDEEQUIPTO},
    {"duration", ITEM_PARSE_DURATION},
    {"showduration", ITEM_PARSE_SHOWDURATION},
    {"charges", ITEM_PARSE_CHARGES},
    {"showcharges", ITEM_PARSE_SHOWCHARGES},
    {"showattributes", ITEM_PARSE_SHOWATTRIBUTES},
    {"hitchance", ITEM_PARSE_HITCHANCE},
    {"maxhitchance", ITEM_PARSE_MAXHITCHANCE},
    {"invisible", ITEM_PARSE_INVISIBLE},
    {"speed", ITEM_PARSE_SPEED},
    {"healthgain", ITEM_PARSE_HEALTHGAIN},
    {"healthticks", ITEM_PARSE_HEALTHTICKS},
    {"managain", ITEM_PARSE_MANAGAIN},
    {"manaticks", ITEM_PARSE_MANATICKS},
    {"manashield", ITEM_PARSE_MANASHIELD},
    {"skillsword", ITEM_PARSE_SKILLSWORD},
    {"skillaxe", ITEM_PARSE_SKILLAXE},
    {"skillclub", ITEM_PARSE_SKILLCLUB},
    {"skilldist", ITEM_PARSE_SKILLDIST},
    {"skillfish", ITEM_PARSE_SKILLFISH},
    {"skillshield", ITEM_PARSE_SKILLSHIELD},
    {"skillfist", ITEM_PARSE_SKILLFIST},
    {"maxhitpoints", ITEM_PARSE_MAXHITPOINTS},
    {"maxhitpointspercent", ITEM_PARSE_MAXHITPOINTSPERCENT},
    {"maxmanapoints", ITEM_PARSE_MAXMANAPOINTS},
    {"maxmanapointspercent", ITEM_PARSE_MAXMANAPOINTSPERCENT},
    {"magicpoints", ITEM_PARSE_MAGICPOINTS},
    {"magiclevelpoints", ITEM_PARSE_MAGICPOINTS},
    {"magicpointspercent", ITEM_PARSE_MAGICPOINTSPERCENT},
    {"criticalhitchance", ITEM_PARSE_CRITICALHITCHANCE},
    {"criticalhitamount", ITEM_PARSE_CRITICALHITAMOUNT},
    {"lifeleechchance", ITEM_PARSE_LIFELEECHCHANCE},
    {"lifeleechamount", ITEM_PARSE_LIFELEECHAMOUNT},
    {"manaleechchance", ITEM_PARSE_MANALEECHCHANCE},
    {"manaleechamount", ITEM_PARSE_MANALEECHAMOUNT},
    {"fieldabsorbpercentenergy", ITEM_PARSE_FIELDABSORBPERCENTENERGY},
    {"fieldabsorbpercentfire", ITEM_PARSE_FIELDABSORBPERCENTFIRE},
    {"fieldabsorbpercentpoison", ITEM_PARSE_FIELDABSORBPERCENTPOISON},
    {"fieldabsorbpercentearth", ITEM_PARSE_FIELDABSORBPERCENTPOISON},
    {"absorbpercentall", ITEM_PARSE_ABSORBPERCENTALL},
    {"absorbpercentallelements", ITEM_PARSE_ABSORBPERCENTALL},
    {"absorbpercentelements", ITEM_PARSE_ABSORBPERCENTELEMENTS},
    {"absorbpercentmagic", ITEM_PARSE_ABSORBPERCENTMAGIC},
    {"absorbpercentenergy", ITEM_PARSE_ABSORBPERCENTENERGY},
    {"absorbpercentfire", ITEM_PARSE_ABSORBPERCENTFIRE},
    {"absorbpercentpoison", ITEM_PARSE_ABSORBPERCENTPOISON},
    {"absorbpercentearth", ITEM_PARSE_ABSORBPERCENTPOISON},
    {"absorbpercentice", ITEM_PARSE_ABSORBPERCENTICE},
    {"absorbpercentholy", ITEM_PARSE_ABSORBPERCENTHOLY},
    {"absorbpercentdeath", ITEM_PARSE_ABSORBPERCENTDEATH},
    {"absorbpercentlifedrain", ITEM_PARSE_ABSORBPERCENTLIFEDRAIN},
    {"absorbpercentmanadrain", ITEM_PARSE_ABSORBPERCENTMANADRAIN},
    {"absorbpercentdrown", ITEM_PARSE_ABSORBPERCENTDROWN},
    {"absorbpercentphysical", ITEM_PARSE_ABSORBPERCENTPHYSICAL},
    {"absorbpercenthealing", ITEM_PARSE_ABSORBPERCENTHEALING},
    {"absorbpercentundefined", ITEM_PARSE_ABSORBPERCENTUNDEFINED},
    {"reflectpercentall", ITEM_PARSE_REFLECTPERCENTALL},
    {"reflectpercentallelements", ITEM_PARSE_REFLECTPERCENTALL},
    {"reflectpercentelements", ITEM_PARSE_REFLECTPERCENTELEMENTS},
    {"reflectpercentmagic", ITEM_PARSE_REFLECTPERCENTMAGIC},
    {"reflectpercentenergy", ITEM_PARSE_REFLECTPERCENTENERGY},
    {"reflectpercentfire", ITEM_PARSE_REFLECTPERCENTFIRE},
    {"reflectpercentpoison", ITEM_PARSE_REFLECTPERCENTEARTH},
    {"reflectpercentearth", ITEM_PARSE_REFLECTPERCENTEARTH},
    {"reflectpercentice", ITEM_PARSE_REFLECTPERCENTICE},
    {"reflectpercentholy", ITEM_PARSE_REFLECTPERCENTHOLY},
    {"reflectpercentdeath", ITEM_PARSE_REFLECTPERCENTDEATH},
    {"reflectpercentlifedrain", ITEM_PARSE_REFLECTPERCENTLIFEDRAIN},
    {"reflectpercentmanadrain", ITEM_PARSE_REFLECTPERCENTMANADRAIN},
    {"reflectpercentdrown", ITEM_PARSE_REFLECTPERCENTDROWN},
    {"reflectpercentphysical", ITEM_PARSE_REFLECTPERCENTPHYSICAL},
    {"reflectpercenthealing", ITEM_PARSE_REFLECTPERCENTHEALING},
    {"reflectchanceall", ITEM_PARSE_REFLECTCHANCEALL},
    {"reflectchanceallelements", ITEM_PARSE_REFLECTCHANCEALL},
    {"reflectchanceelements", ITEM_PARSE_REFLECTCHANCEELEMENTS},
    {"reflectchancemagic", ITEM_PARSE_REFLECTCHANCEMAGIC},
    {"reflectchanceenergy", ITEM_PARSE_REFLECTCHANCEENERGY},
    {"reflectchancefire", ITEM_PARSE_REFLECTCHANCEFIRE},
    {"reflectchancepoison", ITEM_PARSE_REFLECTCHANCEEARTH},
    {"reflectchanceearth", ITEM_PARSE_REFLECTCHANCEEARTH},
    {"reflectchanceice", ITEM_PARSE_REFLECTCHANCEICE},
    {"reflectchanceholy", ITEM_PARSE_REFLECTCHANCEHOLY},
    {"reflectchancedeath", ITEM_PARSE_REFLECTCHANCEDEATH},
    {"reflectchancelifedrain", ITEM_PARSE_REFLECTCHANCELIFEDRAIN},
    {"reflectchancemanadrain", ITEM_PARSE_REFLECTCHANCEMANADRAIN},
    {"reflectchancedrown", ITEM_PARSE_REFLECTCHANCEDROWN},
    {"reflectchancephysical", ITEM_PARSE_REFLECTCHANCEPHYSICAL},
    {"reflectchancehealing", ITEM_PARSE_REFLECTCHANCEHEALING},
    {"boostpercentall", ITEM_PARSE_BOOSTPERCENTALL},
    {"boostpercentallelements", ITEM_PARSE_BOOSTPERCENTALL},
    {"boostpercentelements", ITEM_PARSE_BOOSTPERCENTELEMENTS},
    {"boostpercentmagic", ITEM_PARSE_BOOSTPERCENTMAGIC},
    {"boostpercentenergy", ITEM_PARSE_BOOSTPERCENTENERGY},
    {"boostpercentfire", ITEM_PARSE_BOOSTPERCENTFIRE},
    {"boostpercentpoison", ITEM_PARSE_BOOSTPERCENTEARTH},
    {"boostpercentearth", ITEM_PARSE_BOOSTPERCENTEARTH},
    {"boostpercentice", ITEM_PARSE_BOOSTPERCENTICE},
    {"boostpercentholy", ITEM_PARSE_BOOSTPERCENTHOLY},
    {"boostpercentdeath", ITEM_PARSE_BOOSTPERCENTDEATH},
    {"boostpercentlifedrain", ITEM_PARSE_BOOSTPERCENTLIFEDRAIN},
    {"boostpercentmanadrain", ITEM_PARSE_BOOSTPERCENTMANADRAIN},
    {"boostpercentdrown", ITEM_PARSE_BOOSTPERCENTDROWN},
    {"boostpercentphysical", ITEM_PARSE_BOOSTPERCENTPHYSICAL},
    {"boostpercenthealing", ITEM_PARSE_BOOSTPERCENTHEALING},
    {"magiclevelenergy", ITEM_PARSE_MAGICLEVELENERGY},
    {"magiclevelfire", ITEM_PARSE_MAGICLEVELFIRE},
    {"magiclevelpoison", ITEM_PARSE_MAGICLEVELPOISON},
    {"magiclevelearth", ITEM_PARSE_MAGICLEVELPOISON},
    {"magiclevelice", ITEM_PARSE_MAGICLEVELICE},
    {"magiclevelholy", ITEM_PARSE_MAGICLEVELHOLY},
    {"magicleveldeath", ITEM_PARSE_MAGICLEVELDEATH},
    {"magiclevellifedrain", ITEM_PARSE_MAGICLEVELLIFEDRAIN},
    {"magiclevelmanadrain", ITEM_PARSE_MAGICLEVELMANADRAIN},
    {"magicleveldrown", ITEM_PARSE_MAGICLEVELDROWN},
    {"magiclevelphysical", ITEM_PARSE_MAGICLEVELPHYSICAL},
    {"magiclevelhealing", ITEM_PARSE_MAGICLEVELHEALING},
    {"magiclevelundefined", ITEM_PARSE_MAGICLEVELUNDEFINED},
    {"suppressdrunk", ITEM_PARSE_SUPPRESSDRUNK},
    {"suppressenergy", ITEM_PARSE_SUPPRESSENERGY},
    {"suppressfire", ITEM_PARSE_SUPPRESSFIRE},
    {"suppresspoison", ITEM_PARSE_SUPPRESSPOISON},
    {"suppressdrown", ITEM_PARSE_SUPPRESSDROWN},
    {"suppressphysical", ITEM_PARSE_SUPPRESSPHYSICAL},
    {"suppressfreeze", ITEM_PARSE_SUPPRESSFREEZE},
    {"suppressdazzle", ITEM_PARSE_SUPPRESSDAZZLE},
    {"suppresscurse", ITEM_PARSE_SUPPRESSCURSE},
    {"field", ITEM_PARSE_FIELD},
    {"replaceable", ITEM_PARSE_REPLACEABLE},
    {"partnerdirection", ITEM_PARSE_PARTNERDIRECTION},
    {"leveldoor", ITEM_PARSE_LEVELDOOR},
    {"maletransformto", ITEM_PARSE_MALETRANSFORMTO},
    {"malesleeper", ITEM_PARSE_MALETRANSFORMTO},
    {"femaletransformto", ITEM_PARSE_FEMALETRANSFORMTO},
    {"femalesleeper", ITEM_PARSE_FEMALETRANSFORMTO},
    {"transformto", ITEM_PARSE_TRANSFORMTO},
    {"destroyto", ITEM_PARSE_DESTROYTO},
    {"elementice", ITEM_PARSE_ELEMENTICE},
    {"elementearth", ITEM_PARSE_ELEMENTEARTH},
    {"elementfire", ITEM_PARSE_ELEMENTFIRE},
    {"elementenergy", ITEM_PARSE_ELEMENTENERGY},
    {"elementdeath", ITEM_PARSE_ELEMENTDEATH},
    {"elementholy", ITEM_PARSE_ELEMENTHOLY},
    {"walkstack", ITEM_PARSE_WALKSTACK},
    {"blocking", ITEM_PARSE_BLOCKING},
    {"allowdistread", ITEM_PARSE_ALLOWDISTREAD},
    {"storeitem", ITEM_PARSE_STOREITEM},
    {"worth", ITEM_PARSE_WORTH},
    {"supply", ITEM_PARSE_SUPPLY},
};

const std::unordered_map<std::string, ItemTypes_t> ItemTypesMap = {{"key", ITEM_TYPE_KEY},
                                                                   {"magicfield", ITEM_TYPE_MAGICFIELD},
                                                                   {"container", ITEM_TYPE_CONTAINER},
                                                                   {"depot", ITEM_TYPE_DEPOT},
                                                                   {"mailbox", ITEM_TYPE_MAILBOX},
                                                                   {"trashholder", ITEM_TYPE_TRASHHOLDER},
                                                                   {"teleport", ITEM_TYPE_TELEPORT},
                                                                   {"door", ITEM_TYPE_DOOR},
                                                                   {"bed", ITEM_TYPE_BED},
                                                                   {"rune", ITEM_TYPE_RUNE},
                                                                   {"podium", ITEM_TYPE_PODIUM}};

const std::unordered_map<std::string, tileflags_t> TileStatesMap = {
    {"down", TILESTATE_FLOORCHANGE_DOWN},        {"north", TILESTATE_FLOORCHANGE_NORTH},
    {"south", TILESTATE_FLOORCHANGE_SOUTH},      {"southalt", TILESTATE_FLOORCHANGE_SOUTH_ALT},
    {"west", TILESTATE_FLOORCHANGE_WEST},        {"east", TILESTATE_FLOORCHANGE_EAST},
    {"eastalt", TILESTATE_FLOORCHANGE_EAST_ALT},
};

const std::unordered_map<std::string, RaceType_t> RaceTypesMap = {
    {"venom", RACE_VENOM}, {"blood", RACE_BLOOD},   {"undead", RACE_UNDEAD},
    {"fire", RACE_FIRE},   {"energy", RACE_ENERGY}, {"ink", RACE_INK},
};

const std::unordered_map<std::string, WeaponType_t> WeaponTypesMap = {
    {"sword", WEAPON_SWORD},       {"club", WEAPON_CLUB}, {"axe", WEAPON_AXE},         {"shield", WEAPON_SHIELD},
    {"distance", WEAPON_DISTANCE}, {"wand", WEAPON_WAND}, {"ammunition", WEAPON_AMMO}, {"quiver", WEAPON_QUIVER},
};

const std::unordered_map<std::string, FluidTypes_t> FluidTypesMap = {
    {"water", FLUID_WATER},
    {"blood", FLUID_BLOOD},
    {"beer", FLUID_BEER},
    {"slime", FLUID_SLIME},
    {"lemonade", FLUID_LEMONADE},
    {"milk", FLUID_MILK},
    {"mana", FLUID_MANA},
    {"life", FLUID_LIFE},
    {"oil", FLUID_OIL},
    {"urine", FLUID_URINE},
    {"coconut", FLUID_COCONUTMILK},
    {"wine", FLUID_WINE},
    {"mud", FLUID_MUD},
    {"fruitjuice", FLUID_FRUITJUICE},
    {"lava", FLUID_LAVA},
    {"rum", FLUID_RUM},
    {"swamp", FLUID_SWAMP},
    {"tea", FLUID_TEA},
    {"mead", FLUID_MEAD},
    {"ink", FLUID_INK},
};

const std::unordered_map<std::string_view, Direction> DirectionsMap = {
    {"north", DIRECTION_NORTH},
    {"n", DIRECTION_NORTH},
    {"0", DIRECTION_NORTH},
    {"east", DIRECTION_EAST},
    {"e", DIRECTION_EAST},
    {"1", DIRECTION_EAST},
    {"south", DIRECTION_SOUTH},
    {"s", DIRECTION_SOUTH},
    {"2", DIRECTION_SOUTH},
    {"west", DIRECTION_WEST},
    {"w", DIRECTION_WEST},
    {"3", DIRECTION_WEST},
    {"southwest", DIRECTION_SOUTHWEST},
    {"south west", DIRECTION_SOUTHWEST},
    {"south-west", DIRECTION_SOUTHWEST},
    {"sw", DIRECTION_SOUTHWEST},
    {"4", DIRECTION_SOUTHWEST},
    {"southeast", DIRECTION_SOUTHEAST},
    {"south east", DIRECTION_SOUTHEAST},
    {"south-east", DIRECTION_SOUTHEAST},
    {"se", DIRECTION_SOUTHEAST},
    {"5", DIRECTION_SOUTHEAST},
    {"northwest", DIRECTION_NORTHWEST},
    {"north west", DIRECTION_NORTHWEST},
    {"north-west", DIRECTION_NORTHWEST},
    {"nw", DIRECTION_NORTHWEST},
    {"6", DIRECTION_NORTHWEST},
    {"northeast", DIRECTION_NORTHEAST},
    {"north east", DIRECTION_NORTHEAST},
    {"north-east", DIRECTION_NORTHEAST},
    {"ne", DIRECTION_NORTHEAST},
    {"7", DIRECTION_NORTHEAST},
};

Direction getDirection(std::string_view string)
{
	if (auto it = DirectionsMap.find(string); it != DirectionsMap.end()) {
		return it->second;
	}
	fmt::print("[Warning - getDirection] Invalid direction: {}\n", string);
	return DIRECTION_NORTH;
}

} // namespace

Items::Items()
{
	items.reserve(45000);
	nameToItems.reserve(45000);
}

void Items::clear()
{
	items.clear();
	clientIdToServerIdMap.clear();
	nameToItems.clear();
	currencyItems.clear();
	inventory.clear();
}

bool Items::reload()
{
	clear();
	loadFromOtb("data/items/items.otb");

	if (!loadFromXml()) {
		return false;
	}

	g_moveEvents->reload();
	g_weapons->reload();
	g_weapons->loadDefaults();
	return true;
}

bool Items::loadFromOtb(const std::string& file)
{
	auto loader = OTB::load(file, "OTBI");

	auto first = loader.begin();
	const auto last = loader.end();

	// 4 byte flags
	// attributes
	// 0x01 = version data
	// uint32_t flags = OTB::read<uint32_t>(first, last); // unused
	OTB::skip(first, last, sizeof(uint32_t));
	uint8_t attr = OTB::read<uint8_t>(first, last);

	if (attr == ROOT_ATTR_VERSION) {
		constexpr auto VERSION_INFO_SIZE = 140;
		auto length = OTB::read<uint16_t>(first, last);
		if (length != VERSION_INFO_SIZE) {
			throw std::invalid_argument(
			    fmt::format("Invalid data length for version info: expected 140, got {:d}", length));
		}

		majorVersion = OTB::read<uint32_t>(first, last); // items otb format file version
		minorVersion = OTB::read<uint32_t>(first, last); // client version
		buildNumber = OTB::read<uint32_t>(first, last);  // revision
		OTB::skip(first, last, VERSION_INFO_SIZE - 3 * sizeof(uint32_t));
	}

	if (majorVersion == std::numeric_limits<uint32_t>::max()) {
		std::cout << "[Warning - Items::loadFromOtb] items.otb using generic client version." << std::endl;
	} else if (majorVersion != 3) {
		std::cout << "Old version detected, a newer version of items.otb is required." << std::endl;
		return false;
	} else if (minorVersion < CLIENT_VERSION_LAST) {
		std::cout << "A newer version of items.otb is required." << std::endl;
		return false;
	}

	for (auto& itemNode : loader.children()) {
		auto first = itemNode.propsBegin;
		const auto last = itemNode.propsEnd;

		uint32_t flags = OTB::read<uint32_t>(first, last);

		uint16_t serverId = 0;
		uint16_t clientId = 0;
		uint16_t speed = 0;
		uint16_t wareId = 0;
		uint8_t lightLevel = 0;
		uint8_t lightColor = 0;
		uint8_t alwaysOnTopOrder = 0;
		uint8_t classification = 0;

		while (first != last) {
			uint8_t attr = OTB::read<uint8_t>(first, last);
			uint16_t length = OTB::read<uint16_t>(first, last);

			switch (attr) {
				case ITEM_ATTR_SERVERID: {
					if (length != sizeof(uint16_t)) {
						throw std::invalid_argument(fmt::format(
						    "Invalid server ID attribute length: expected {:d}, got {:d}", sizeof(uint16_t), length));
					}

					serverId = OTB::read<uint16_t>(first, last);
					break;
				}

				case ITEM_ATTR_CLIENTID: {
					if (length != sizeof(uint16_t)) {
						throw std::invalid_argument(fmt::format(
						    "Invalid client ID attribute length: expected {:d}, got {:d}", sizeof(uint16_t), length));
					}

					clientId = OTB::read<uint16_t>(first, last);
					break;
				}

				case ITEM_ATTR_SPEED: {
					if (length != sizeof(uint16_t)) {
						throw std::invalid_argument(fmt::format(
						    "Invalid speed attribute length: expected {:d}, got {:d}", sizeof(uint16_t), length));
					}

					speed = OTB::read<uint16_t>(first, last);
					break;
				}

				case ITEM_ATTR_LIGHT2: {
					if (length != 2 * sizeof(uint16_t)) {
						throw std::invalid_argument(fmt::format(
						    "Invalid light2 attribute length: expected {:d}, got {:d}", 2 * sizeof(uint16_t), length));
					}

					lightLevel = static_cast<uint8_t>(OTB::read<uint16_t>(first, last));
					lightColor = static_cast<uint8_t>(OTB::read<uint16_t>(first, last));
					break;
				}

				case ITEM_ATTR_TOPORDER: {
					if (length != sizeof(uint8_t)) {
						throw std::invalid_argument(fmt::format(
						    "Invalid top order attribute length: expected {:d}, got {:d}", sizeof(uint8_t), length));
					}

					alwaysOnTopOrder = OTB::read<uint8_t>(first, last);
					break;
				}

				case ITEM_ATTR_WAREID: {
					if (length != sizeof(uint16_t)) {
						throw std::invalid_argument(fmt::format(
						    "Invalid ware ID attribute length: expected {:d}, got {:d}", sizeof(uint16_t), length));
					}

					wareId = OTB::read<uint16_t>(first, last);
					break;
				}

				case ITEM_ATTR_CLASSIFICATION: {
					if (length != sizeof(uint8_t)) {
						throw std::invalid_argument(
						    fmt::format("Invalid classification attribute length: expected {:d}, got {:d}",
						                sizeof(uint16_t), length));
					}

					classification = OTB::read<uint8_t>(first, last);
					break;
				}

				default: {
					// skip unknown attributes
					OTB::skip(first, last, length);
					break;
				}
			}
		}

		clientIdToServerIdMap.emplace(clientId, serverId);

		// store the found item
		if (serverId >= items.size()) {
			items.resize(serverId + 1);
		}
		ItemType& iType = items[serverId];

		iType.group = static_cast<itemgroup_t>(itemNode.type);
		switch (itemNode.type) {
			case ITEM_GROUP_CONTAINER:
				iType.type = ITEM_TYPE_CONTAINER;
				break;
			case ITEM_GROUP_DOOR:
				// not used
				iType.type = ITEM_TYPE_DOOR;
				break;
			case ITEM_GROUP_MAGICFIELD:
				// not used
				iType.type = ITEM_TYPE_MAGICFIELD;
				break;
			case ITEM_GROUP_TELEPORT:
				// not used
				iType.type = ITEM_TYPE_TELEPORT;
				break;
			case ITEM_GROUP_NONE:
			case ITEM_GROUP_GROUND:
			case ITEM_GROUP_SPLASH:
			case ITEM_GROUP_FLUID:
			case ITEM_GROUP_CHARGES:
			case ITEM_GROUP_DEPRECATED:
				break;
			case ITEM_GROUP_PODIUM:
				iType.type = ITEM_TYPE_PODIUM;
				break;
			default:
				return false;
		}

		iType.blockSolid = hasBitSet(FLAG_BLOCK_SOLID, flags);
		iType.blockProjectile = hasBitSet(FLAG_BLOCK_PROJECTILE, flags);
		iType.blockPathFind = hasBitSet(FLAG_BLOCK_PATHFIND, flags);
		iType.hasHeight = hasBitSet(FLAG_HAS_HEIGHT, flags);
		iType.useable = hasBitSet(FLAG_USEABLE, flags);
		iType.pickupable = hasBitSet(FLAG_PICKUPABLE, flags);
		iType.moveable = hasBitSet(FLAG_MOVEABLE, flags);
		iType.stackable = hasBitSet(FLAG_STACKABLE, flags);

		iType.alwaysOnTop = hasBitSet(FLAG_ALWAYSONTOP, flags);
		iType.isVertical = hasBitSet(FLAG_VERTICAL, flags);
		iType.isHorizontal = hasBitSet(FLAG_HORIZONTAL, flags);
		iType.isHangable = hasBitSet(FLAG_HANGABLE, flags);
		iType.allowDistRead = hasBitSet(FLAG_ALLOWDISTREAD, flags);
		iType.rotatable = hasBitSet(FLAG_ROTATABLE, flags);
		iType.canReadText = hasBitSet(FLAG_READABLE, flags);
		iType.lookThrough = hasBitSet(FLAG_LOOKTHROUGH, flags);
		iType.isAnimation = hasBitSet(FLAG_ANIMATION, flags);
		// iType.walkStack = !hasBitSet(FLAG_FULLTILE, flags);
		iType.forceUse = hasBitSet(FLAG_FORCEUSE, flags);
		iType.showClientCharges = hasBitSet(FLAG_CLIENTCHARGES, flags);
		iType.showClientDuration = hasBitSet(FLAG_CLIENTDURATION, flags);

		iType.id = serverId;
		iType.clientId = clientId;
		iType.speed = speed;
		iType.lightLevel = lightLevel;
		iType.lightColor = lightColor;
		iType.wareId = wareId;
		iType.classification = classification;
		iType.alwaysOnTopOrder = alwaysOnTopOrder;
	}

	items.shrink_to_fit();
	return true;
}

bool Items::loadFromXml()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/items/items.xml");
	if (!result) {
		printXMLError("Error - Items::loadFromXml", "data/items/items.xml", result);
		return false;
	}

	for (auto itemNode : doc.child("items").children()) {
		pugi::xml_attribute idAttribute = itemNode.attribute("id");
		if (idAttribute) {
			parseItemNode(itemNode, pugi::cast<uint16_t>(idAttribute.value()));
			continue;
		}

		pugi::xml_attribute fromIdAttribute = itemNode.attribute("fromid");
		if (!fromIdAttribute) {
			std::cout << "[Warning - Items::loadFromXml] No item id found" << std::endl;
			continue;
		}

		pugi::xml_attribute toIdAttribute = itemNode.attribute("toid");
		if (!toIdAttribute) {
			std::cout << "[Warning - Items::loadFromXml] fromid (" << fromIdAttribute.value() << ") without toid"
			          << std::endl;
			continue;
		}

		uint16_t id = pugi::cast<uint16_t>(fromIdAttribute.value());
		uint16_t toId = pugi::cast<uint16_t>(toIdAttribute.value());
		while (id <= toId) {
			parseItemNode(itemNode, id++);
		}
	}

	return true;
}

void Items::parseItemNode(const pugi::xml_node& itemNode, uint16_t id)
{
	if (id > 0 && id < 100) {
		ItemType& iType = items[id];
		iType.id = id;
	}

	ItemType& it = getItemType(id);
	if (it.id == 0) {
		return;
	}

	if (!it.name.empty()) {
		std::cout << "[Warning - Items::parseItemNode] Duplicate item with id: " << id << std::endl;
		return;
	}

	it.name = itemNode.attribute("name").as_string();

	if (!it.name.empty()) {
		std::string lowerCaseName = boost::algorithm::to_lower_copy(it.name);
		if (nameToItems.find(lowerCaseName) == nameToItems.end()) {
			nameToItems.emplace(std::move(lowerCaseName), id);
		}
	}

	pugi::xml_attribute articleAttribute = itemNode.attribute("article");
	if (articleAttribute) {
		it.article = articleAttribute.as_string();
	}

	pugi::xml_attribute pluralAttribute = itemNode.attribute("plural");
	if (pluralAttribute) {
		it.pluralName = pluralAttribute.as_string();
	}

	Abilities& abilities = it.getAbilities();

	for (auto attributeNode : itemNode.children()) {
		pugi::xml_attribute keyAttribute = attributeNode.attribute("key");
		if (!keyAttribute) {
			continue;
		}

		pugi::xml_attribute valueAttribute = attributeNode.attribute("value");
		pugi::xml_attribute maxValueAttr;
		if (!valueAttribute) {
			valueAttribute = attributeNode.attribute("minvalue");
			if (!valueAttribute) {
				continue;
			}

			maxValueAttr = attributeNode.attribute("maxvalue");
			if (!maxValueAttr) {
				continue;
			}
		}

		std::string tmpStrValue = boost::algorithm::to_lower_copy<std::string>(keyAttribute.as_string());
		auto parseAttribute = ItemParseAttributesMap.find(tmpStrValue);
		if (parseAttribute != ItemParseAttributesMap.end()) {
			ItemParseAttributes_t parseType = parseAttribute->second;
			switch (parseType) {
				case ITEM_PARSE_TYPE: {
					tmpStrValue = boost::algorithm::to_lower_copy<std::string>(valueAttribute.as_string());
					auto it2 = ItemTypesMap.find(tmpStrValue);
					if (it2 != ItemTypesMap.end()) {
						it.type = it2->second;
						if (it.type == ITEM_TYPE_CONTAINER) {
							it.group = ITEM_GROUP_CONTAINER;
						}
					} else {
						std::cout << "[Warning - Items::parseItemNode] Unknown type: " << valueAttribute.as_string()
						          << std::endl;
					}
					break;
				}

				case ITEM_PARSE_DESCRIPTION: {
					it.description = valueAttribute.as_string();
					break;
				}

				case ITEM_PARSE_RUNESPELLNAME: {
					it.runeSpellName = valueAttribute.as_string();
					break;
				}

				case ITEM_PARSE_WEIGHT: {
					it.weight = pugi::cast<uint32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_SHOWCOUNT: {
					it.showCount = valueAttribute.as_bool();
					break;
				}

				case ITEM_PARSE_SUPPLY: {
					it.supply = valueAttribute.as_bool();
					break;
				}

				case ITEM_PARSE_ARMOR: {
					it.armor = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_DEFENSE: {
					it.defense = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_EXTRADEF: {
					it.extraDefense = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ATTACK: {
					it.attack = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ATTACK_SPEED: {
					it.attackSpeed = pugi::cast<uint32_t>(valueAttribute.value());
					if (it.attackSpeed > 0 && it.attackSpeed < 100) {
						std::cout << "[Warning - Items::parseItemNode] AttackSpeed lower than 100 for item: " << it.id
						          << std::endl;
						it.attackSpeed = 100;
					}
					break;
				}

				case ITEM_PARSE_ROTATETO: {
					it.rotateTo = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MOVEABLE: {
					it.moveable = valueAttribute.as_bool();
					break;
				}

				case ITEM_PARSE_BLOCKPROJECTILE: {
					it.blockProjectile = valueAttribute.as_bool();
					break;
				}

				case ITEM_PARSE_PICKUPABLE: {
					it.allowPickupable = valueAttribute.as_bool();
					break;
				}

				case ITEM_PARSE_FORCESERIALIZE: {
					it.forceSerialize = valueAttribute.as_bool();
					break;
				}

				case ITEM_PARSE_FLOORCHANGE: {
					tmpStrValue = boost::algorithm::to_lower_copy<std::string>(valueAttribute.as_string());
					auto it2 = TileStatesMap.find(tmpStrValue);
					if (it2 != TileStatesMap.end()) {
						it.floorChange |= it2->second;
					} else {
						std::cout << "[Warning - Items::parseItemNode] Unknown floorChange: "
						          << valueAttribute.as_string() << std::endl;
					}
					break;
				}

				case ITEM_PARSE_CORPSETYPE: {
					tmpStrValue = boost::algorithm::to_lower_copy<std::string>(valueAttribute.as_string());
					auto it2 = RaceTypesMap.find(tmpStrValue);
					if (it2 != RaceTypesMap.end()) {
						it.corpseType = it2->second;
					} else {
						std::cout << "[Warning - Items::parseItemNode] Unknown corpseType: "
						          << valueAttribute.as_string() << std::endl;
					}
					break;
				}

				case ITEM_PARSE_CONTAINERSIZE: {
					it.maxItems = pugi::cast<uint16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_FLUIDSOURCE: {
					tmpStrValue = boost::algorithm::to_lower_copy<std::string>(valueAttribute.as_string());
					auto it2 = FluidTypesMap.find(tmpStrValue);
					if (it2 != FluidTypesMap.end()) {
						it.fluidSource = it2->second;
					} else {
						std::cout << "[Warning - Items::parseItemNode] Unknown fluidSource: "
						          << valueAttribute.as_string() << std::endl;
					}
					break;
				}

				case ITEM_PARSE_READABLE: {
					it.canReadText = valueAttribute.as_bool();
					break;
				}

				case ITEM_PARSE_WRITEABLE: {
					it.canWriteText = valueAttribute.as_bool();
					it.canReadText = it.canWriteText;
					break;
				}

				case ITEM_PARSE_MAXTEXTLEN: {
					it.maxTextLen = pugi::cast<uint16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_WRITEONCEITEMID: {
					it.writeOnceItemId = pugi::cast<uint16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_WEAPONTYPE: {
					tmpStrValue = boost::algorithm::to_lower_copy<std::string>(valueAttribute.as_string());
					auto it2 = WeaponTypesMap.find(tmpStrValue);
					if (it2 != WeaponTypesMap.end()) {
						it.weaponType = it2->second;
					} else {
						std::cout << "[Warning - Items::parseItemNode] Unknown weaponType: "
						          << valueAttribute.as_string() << std::endl;
					}
					break;
				}

				case ITEM_PARSE_SLOTTYPE: {
					tmpStrValue = boost::algorithm::to_lower_copy<std::string>(valueAttribute.as_string());
					if (tmpStrValue == "head") {
						it.slotPosition |= SLOTP_HEAD;
					} else if (tmpStrValue == "body") {
						it.slotPosition |= SLOTP_ARMOR;
					} else if (tmpStrValue == "legs") {
						it.slotPosition |= SLOTP_LEGS;
					} else if (tmpStrValue == "feet") {
						it.slotPosition |= SLOTP_FEET;
					} else if (tmpStrValue == "backpack") {
						it.slotPosition |= SLOTP_BACKPACK;
					} else if (tmpStrValue == "two-handed") {
						it.slotPosition |= SLOTP_TWO_HAND;
					} else if (tmpStrValue == "right-hand") {
						it.slotPosition &= ~SLOTP_LEFT;
					} else if (tmpStrValue == "left-hand") {
						it.slotPosition &= ~SLOTP_RIGHT;
					} else if (tmpStrValue == "necklace") {
						it.slotPosition |= SLOTP_NECKLACE;
					} else if (tmpStrValue == "ring") {
						it.slotPosition |= SLOTP_RING;
					} else if (tmpStrValue == "ammo") {
						it.slotPosition |= SLOTP_AMMO;
					} else if (tmpStrValue == "hand") {
						it.slotPosition |= SLOTP_HAND;
					} else {
						std::cout << "[Warning - Items::parseItemNode] Unknown slotType: " << valueAttribute.as_string()
						          << std::endl;
					}
					break;
				}

				case ITEM_PARSE_AMMOTYPE: {
					it.ammoType = getAmmoType(boost::algorithm::to_lower_copy<std::string>(valueAttribute.as_string()));
					if (it.ammoType == AMMO_NONE) {
						std::cout << "[Warning - Items::parseItemNode] Unknown ammoType: " << valueAttribute.as_string()
						          << std::endl;
					}
					break;
				}

				case ITEM_PARSE_SHOOTTYPE: {
					ShootType_t shoot =
					    getShootType(boost::algorithm::to_lower_copy<std::string>(valueAttribute.as_string()));
					if (shoot != CONST_ANI_NONE) {
						it.shootType = shoot;
					} else {
						std::cout << "[Warning - Items::parseItemNode] Unknown shootType: "
						          << valueAttribute.as_string() << std::endl;
					}
					break;
				}

				case ITEM_PARSE_EFFECT: {
					MagicEffectClasses effect =
					    getMagicEffect(boost::algorithm::to_lower_copy<std::string>(valueAttribute.as_string()));
					if (effect != CONST_ME_NONE) {
						it.magicEffect = effect;
					} else {
						std::cout << "[Warning - Items::parseItemNode] Unknown effect: " << valueAttribute.as_string()
						          << std::endl;
					}
					break;
				}

				case ITEM_PARSE_RANGE: {
					it.shootRange = pugi::cast<uint16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_STOPDURATION: {
					it.stopTime = valueAttribute.as_bool();
					break;
				}

				case ITEM_PARSE_DECAYTO: {
					it.decayTo = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_TRANSFORMEQUIPTO: {
					it.transformEquipTo = pugi::cast<uint16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_TRANSFORMDEEQUIPTO: {
					it.transformDeEquipTo = pugi::cast<uint16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_DURATION: {
					it.decayTimeMin = pugi::cast<uint32_t>(valueAttribute.value());

					if (maxValueAttr) {
						it.decayTimeMax = pugi::cast<uint32_t>(maxValueAttr.value());
					} else {
						it.decayTimeMax = pugi::cast<uint32_t>(valueAttribute.value());
					}
					break;
				}

				case ITEM_PARSE_SHOWDURATION: {
					it.showDuration = valueAttribute.as_bool();
					break;
				}

				case ITEM_PARSE_CHARGES: {
					it.charges = pugi::cast<uint32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_SHOWCHARGES: {
					it.showCharges = valueAttribute.as_bool();
					break;
				}

				case ITEM_PARSE_SHOWATTRIBUTES: {
					it.showAttributes = valueAttribute.as_bool();
					break;
				}

				case ITEM_PARSE_HITCHANCE: {
					it.hitChance =
					    std::min<int8_t>(100, std::max<int8_t>(-100, pugi::cast<int16_t>(valueAttribute.value())));
					break;
				}

				case ITEM_PARSE_MAXHITCHANCE: {
					it.maxHitChance = std::min<uint32_t>(100, pugi::cast<uint32_t>(valueAttribute.value()));
					break;
				}

				case ITEM_PARSE_INVISIBLE: {
					abilities.invisible = valueAttribute.as_bool();
					break;
				}

				case ITEM_PARSE_SPEED: {
					abilities.speed = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_HEALTHGAIN: {
					abilities.regeneration = true;
					abilities.healthGain = pugi::cast<uint32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_HEALTHTICKS: {
					abilities.regeneration = true;
					abilities.healthTicks = pugi::cast<uint32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MANAGAIN: {
					abilities.regeneration = true;
					abilities.manaGain = pugi::cast<uint32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MANATICKS: {
					abilities.regeneration = true;
					abilities.manaTicks = pugi::cast<uint32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MANASHIELD: {
					abilities.manaShield = valueAttribute.as_bool();
					break;
				}

				case ITEM_PARSE_SKILLSWORD: {
					abilities.skills[SKILL_SWORD] = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_SKILLAXE: {
					abilities.skills[SKILL_AXE] = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_SKILLCLUB: {
					abilities.skills[SKILL_CLUB] = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_SKILLDIST: {
					abilities.skills[SKILL_DISTANCE] = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_SKILLFISH: {
					abilities.skills[SKILL_FISHING] = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_SKILLSHIELD: {
					abilities.skills[SKILL_SHIELD] = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_SKILLFIST: {
					abilities.skills[SKILL_FIST] = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_CRITICALHITAMOUNT: {
					abilities.specialSkills[SPECIALSKILL_CRITICALHITAMOUNT] =
					    pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_CRITICALHITCHANCE: {
					abilities.specialSkills[SPECIALSKILL_CRITICALHITCHANCE] =
					    pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MANALEECHAMOUNT: {
					abilities.specialSkills[SPECIALSKILL_MANALEECHAMOUNT] = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MANALEECHCHANCE: {
					abilities.specialSkills[SPECIALSKILL_MANALEECHCHANCE] = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_LIFELEECHAMOUNT: {
					abilities.specialSkills[SPECIALSKILL_LIFELEECHAMOUNT] = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_LIFELEECHCHANCE: {
					abilities.specialSkills[SPECIALSKILL_LIFELEECHCHANCE] = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MAXHITPOINTS: {
					abilities.stats[STAT_MAXHITPOINTS] = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MAXHITPOINTSPERCENT: {
					abilities.statsPercent[STAT_MAXHITPOINTS] = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MAXMANAPOINTS: {
					abilities.stats[STAT_MAXMANAPOINTS] = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MAXMANAPOINTSPERCENT: {
					abilities.statsPercent[STAT_MAXMANAPOINTS] = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MAGICPOINTS: {
					abilities.stats[STAT_MAGICPOINTS] = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MAGICPOINTSPERCENT: {
					abilities.statsPercent[STAT_MAGICPOINTS] = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_FIELDABSORBPERCENTENERGY: {
					abilities.fieldAbsorbPercent[combatTypeToIndex(COMBAT_ENERGYDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_FIELDABSORBPERCENTFIRE: {
					abilities.fieldAbsorbPercent[combatTypeToIndex(COMBAT_FIREDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_FIELDABSORBPERCENTPOISON: {
					abilities.fieldAbsorbPercent[combatTypeToIndex(COMBAT_EARTHDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTALL: {
					int16_t value = pugi::cast<int16_t>(valueAttribute.value());
					for (auto& i : abilities.absorbPercent) {
						i += value;
					}
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTELEMENTS: {
					int16_t value = pugi::cast<int16_t>(valueAttribute.value());
					abilities.absorbPercent[combatTypeToIndex(COMBAT_ENERGYDAMAGE)] += value;
					abilities.absorbPercent[combatTypeToIndex(COMBAT_FIREDAMAGE)] += value;
					abilities.absorbPercent[combatTypeToIndex(COMBAT_EARTHDAMAGE)] += value;
					abilities.absorbPercent[combatTypeToIndex(COMBAT_ICEDAMAGE)] += value;
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTMAGIC: {
					int16_t value = pugi::cast<int16_t>(valueAttribute.value());
					abilities.absorbPercent[combatTypeToIndex(COMBAT_ENERGYDAMAGE)] += value;
					abilities.absorbPercent[combatTypeToIndex(COMBAT_FIREDAMAGE)] += value;
					abilities.absorbPercent[combatTypeToIndex(COMBAT_EARTHDAMAGE)] += value;
					abilities.absorbPercent[combatTypeToIndex(COMBAT_ICEDAMAGE)] += value;
					abilities.absorbPercent[combatTypeToIndex(COMBAT_HOLYDAMAGE)] += value;
					abilities.absorbPercent[combatTypeToIndex(COMBAT_DEATHDAMAGE)] += value;
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTENERGY: {
					abilities.absorbPercent[combatTypeToIndex(COMBAT_ENERGYDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTFIRE: {
					abilities.absorbPercent[combatTypeToIndex(COMBAT_FIREDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTPOISON: {
					abilities.absorbPercent[combatTypeToIndex(COMBAT_EARTHDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTICE: {
					abilities.absorbPercent[combatTypeToIndex(COMBAT_ICEDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTHOLY: {
					abilities.absorbPercent[combatTypeToIndex(COMBAT_HOLYDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTDEATH: {
					abilities.absorbPercent[combatTypeToIndex(COMBAT_DEATHDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTLIFEDRAIN: {
					abilities.absorbPercent[combatTypeToIndex(COMBAT_LIFEDRAIN)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTMANADRAIN: {
					abilities.absorbPercent[combatTypeToIndex(COMBAT_MANADRAIN)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTDROWN: {
					abilities.absorbPercent[combatTypeToIndex(COMBAT_DROWNDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTPHYSICAL: {
					abilities.absorbPercent[combatTypeToIndex(COMBAT_PHYSICALDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTHEALING: {
					abilities.absorbPercent[combatTypeToIndex(COMBAT_HEALING)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTUNDEFINED: {
					abilities.absorbPercent[combatTypeToIndex(COMBAT_UNDEFINEDDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_REFLECTPERCENTALL: {
					int16_t value = pugi::cast<int16_t>(valueAttribute.value());
					for (auto& i : abilities.reflect) {
						i.percent += value;
					}
					break;
				}

				case ITEM_PARSE_REFLECTPERCENTELEMENTS: {
					int16_t value = pugi::cast<int16_t>(valueAttribute.value());
					abilities.reflect[combatTypeToIndex(COMBAT_ENERGYDAMAGE)].percent += value;
					abilities.reflect[combatTypeToIndex(COMBAT_FIREDAMAGE)].percent += value;
					abilities.reflect[combatTypeToIndex(COMBAT_EARTHDAMAGE)].percent += value;
					abilities.reflect[combatTypeToIndex(COMBAT_ICEDAMAGE)].percent += value;
					break;
				}

				case ITEM_PARSE_REFLECTPERCENTMAGIC: {
					int16_t value = pugi::cast<int16_t>(valueAttribute.value());
					abilities.reflect[combatTypeToIndex(COMBAT_ENERGYDAMAGE)].percent += value;
					abilities.reflect[combatTypeToIndex(COMBAT_FIREDAMAGE)].percent += value;
					abilities.reflect[combatTypeToIndex(COMBAT_EARTHDAMAGE)].percent += value;
					abilities.reflect[combatTypeToIndex(COMBAT_ICEDAMAGE)].percent += value;
					abilities.reflect[combatTypeToIndex(COMBAT_HOLYDAMAGE)].percent += value;
					abilities.reflect[combatTypeToIndex(COMBAT_DEATHDAMAGE)].percent += value;
					break;
				}

				case ITEM_PARSE_REFLECTPERCENTENERGY: {
					abilities.reflect[combatTypeToIndex(COMBAT_ENERGYDAMAGE)].percent +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_REFLECTPERCENTFIRE: {
					abilities.reflect[combatTypeToIndex(COMBAT_FIREDAMAGE)].percent +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_REFLECTPERCENTEARTH: {
					abilities.reflect[combatTypeToIndex(COMBAT_EARTHDAMAGE)].percent +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_REFLECTPERCENTICE: {
					abilities.reflect[combatTypeToIndex(COMBAT_ICEDAMAGE)].percent +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_REFLECTPERCENTHOLY: {
					abilities.reflect[combatTypeToIndex(COMBAT_HOLYDAMAGE)].percent +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_REFLECTPERCENTDEATH: {
					abilities.reflect[combatTypeToIndex(COMBAT_DEATHDAMAGE)].percent +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_REFLECTPERCENTLIFEDRAIN: {
					abilities.reflect[combatTypeToIndex(COMBAT_LIFEDRAIN)].percent +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_REFLECTPERCENTMANADRAIN: {
					abilities.reflect[combatTypeToIndex(COMBAT_MANADRAIN)].percent +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_REFLECTPERCENTDROWN: {
					abilities.reflect[combatTypeToIndex(COMBAT_DROWNDAMAGE)].percent +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_REFLECTPERCENTPHYSICAL: {
					abilities.reflect[combatTypeToIndex(COMBAT_PHYSICALDAMAGE)].percent +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_REFLECTPERCENTHEALING: {
					abilities.reflect[combatTypeToIndex(COMBAT_HEALING)].percent +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_REFLECTCHANCEALL: {
					int16_t value = pugi::cast<int16_t>(valueAttribute.value());
					for (auto& i : abilities.reflect) {
						i.chance += value;
					}
					break;
				}

				case ITEM_PARSE_REFLECTCHANCEELEMENTS: {
					int16_t value = pugi::cast<int16_t>(valueAttribute.value());
					abilities.reflect[combatTypeToIndex(COMBAT_ENERGYDAMAGE)].chance += value;
					abilities.reflect[combatTypeToIndex(COMBAT_FIREDAMAGE)].chance += value;
					abilities.reflect[combatTypeToIndex(COMBAT_EARTHDAMAGE)].chance += value;
					abilities.reflect[combatTypeToIndex(COMBAT_ICEDAMAGE)].chance += value;
					break;
				}

				case ITEM_PARSE_REFLECTCHANCEMAGIC: {
					int16_t value = pugi::cast<int16_t>(valueAttribute.value());
					abilities.reflect[combatTypeToIndex(COMBAT_ENERGYDAMAGE)].chance += value;
					abilities.reflect[combatTypeToIndex(COMBAT_FIREDAMAGE)].chance += value;
					abilities.reflect[combatTypeToIndex(COMBAT_EARTHDAMAGE)].chance += value;
					abilities.reflect[combatTypeToIndex(COMBAT_ICEDAMAGE)].chance += value;
					abilities.reflect[combatTypeToIndex(COMBAT_HOLYDAMAGE)].chance += value;
					abilities.reflect[combatTypeToIndex(COMBAT_DEATHDAMAGE)].chance += value;
					break;
				}

				case ITEM_PARSE_REFLECTCHANCEENERGY: {
					abilities.reflect[combatTypeToIndex(COMBAT_ENERGYDAMAGE)].chance +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_REFLECTCHANCEFIRE: {
					abilities.reflect[combatTypeToIndex(COMBAT_FIREDAMAGE)].chance +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_REFLECTCHANCEEARTH: {
					abilities.reflect[combatTypeToIndex(COMBAT_EARTHDAMAGE)].chance +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_REFLECTCHANCEICE: {
					abilities.reflect[combatTypeToIndex(COMBAT_ICEDAMAGE)].chance +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_REFLECTCHANCEHOLY: {
					abilities.reflect[combatTypeToIndex(COMBAT_HOLYDAMAGE)].chance +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_REFLECTCHANCEDEATH: {
					abilities.reflect[combatTypeToIndex(COMBAT_DEATHDAMAGE)].chance +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_REFLECTCHANCELIFEDRAIN: {
					abilities.reflect[combatTypeToIndex(COMBAT_LIFEDRAIN)].chance +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_REFLECTCHANCEMANADRAIN: {
					abilities.reflect[combatTypeToIndex(COMBAT_MANADRAIN)].chance +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_REFLECTCHANCEDROWN: {
					abilities.reflect[combatTypeToIndex(COMBAT_DROWNDAMAGE)].chance +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_REFLECTCHANCEPHYSICAL: {
					abilities.reflect[combatTypeToIndex(COMBAT_PHYSICALDAMAGE)].chance +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_REFLECTCHANCEHEALING: {
					abilities.reflect[combatTypeToIndex(COMBAT_HEALING)].chance +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_BOOSTPERCENTALL: {
					int16_t value = pugi::cast<int16_t>(valueAttribute.value());
					for (auto& i : abilities.boostPercent) {
						i += value;
					}
					break;
				}

				case ITEM_PARSE_BOOSTPERCENTELEMENTS: {
					int16_t value = pugi::cast<int16_t>(valueAttribute.value());
					abilities.boostPercent[combatTypeToIndex(COMBAT_ENERGYDAMAGE)] += value;
					abilities.boostPercent[combatTypeToIndex(COMBAT_FIREDAMAGE)] += value;
					abilities.boostPercent[combatTypeToIndex(COMBAT_EARTHDAMAGE)] += value;
					abilities.boostPercent[combatTypeToIndex(COMBAT_ICEDAMAGE)] += value;
					break;
				}

				case ITEM_PARSE_BOOSTPERCENTMAGIC: {
					int16_t value = pugi::cast<int16_t>(valueAttribute.value());
					abilities.boostPercent[combatTypeToIndex(COMBAT_ENERGYDAMAGE)] += value;
					abilities.boostPercent[combatTypeToIndex(COMBAT_FIREDAMAGE)] += value;
					abilities.boostPercent[combatTypeToIndex(COMBAT_EARTHDAMAGE)] += value;
					abilities.boostPercent[combatTypeToIndex(COMBAT_ICEDAMAGE)] += value;
					abilities.boostPercent[combatTypeToIndex(COMBAT_HOLYDAMAGE)] += value;
					abilities.boostPercent[combatTypeToIndex(COMBAT_DEATHDAMAGE)] += value;
					break;
				}

				case ITEM_PARSE_BOOSTPERCENTENERGY: {
					abilities.boostPercent[combatTypeToIndex(COMBAT_ENERGYDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_BOOSTPERCENTFIRE: {
					abilities.boostPercent[combatTypeToIndex(COMBAT_FIREDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_BOOSTPERCENTEARTH: {
					abilities.boostPercent[combatTypeToIndex(COMBAT_EARTHDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_BOOSTPERCENTICE: {
					abilities.boostPercent[combatTypeToIndex(COMBAT_ICEDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_BOOSTPERCENTHOLY: {
					abilities.boostPercent[combatTypeToIndex(COMBAT_HOLYDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_BOOSTPERCENTDEATH: {
					abilities.boostPercent[combatTypeToIndex(COMBAT_DEATHDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_BOOSTPERCENTLIFEDRAIN: {
					abilities.boostPercent[combatTypeToIndex(COMBAT_LIFEDRAIN)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_BOOSTPERCENTMANADRAIN: {
					abilities.boostPercent[combatTypeToIndex(COMBAT_MANADRAIN)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_BOOSTPERCENTDROWN: {
					abilities.boostPercent[combatTypeToIndex(COMBAT_DROWNDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_BOOSTPERCENTPHYSICAL: {
					abilities.boostPercent[combatTypeToIndex(COMBAT_PHYSICALDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_BOOSTPERCENTHEALING: {
					abilities.boostPercent[combatTypeToIndex(COMBAT_HEALING)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MAGICLEVELENERGY: {
					abilities.specialMagicLevelSkill[combatTypeToIndex(COMBAT_ENERGYDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MAGICLEVELFIRE: {
					abilities.specialMagicLevelSkill[combatTypeToIndex(COMBAT_FIREDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MAGICLEVELPOISON: {
					abilities.specialMagicLevelSkill[combatTypeToIndex(COMBAT_EARTHDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MAGICLEVELICE: {
					abilities.specialMagicLevelSkill[combatTypeToIndex(COMBAT_ICEDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MAGICLEVELHOLY: {
					abilities.specialMagicLevelSkill[combatTypeToIndex(COMBAT_HOLYDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MAGICLEVELDEATH: {
					abilities.specialMagicLevelSkill[combatTypeToIndex(COMBAT_DEATHDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MAGICLEVELLIFEDRAIN: {
					abilities.specialMagicLevelSkill[combatTypeToIndex(COMBAT_LIFEDRAIN)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MAGICLEVELMANADRAIN: {
					abilities.specialMagicLevelSkill[combatTypeToIndex(COMBAT_MANADRAIN)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MAGICLEVELDROWN: {
					abilities.specialMagicLevelSkill[combatTypeToIndex(COMBAT_DROWNDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MAGICLEVELPHYSICAL: {
					abilities.specialMagicLevelSkill[combatTypeToIndex(COMBAT_PHYSICALDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MAGICLEVELHEALING: {
					abilities.specialMagicLevelSkill[combatTypeToIndex(COMBAT_HEALING)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MAGICLEVELUNDEFINED: {
					abilities.specialMagicLevelSkill[combatTypeToIndex(COMBAT_UNDEFINEDDAMAGE)] +=
					    pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_SUPPRESSDRUNK: {
					if (valueAttribute.as_bool()) {
						abilities.conditionSuppressions |= CONDITION_DRUNK;
					}
					break;
				}

				case ITEM_PARSE_SUPPRESSENERGY: {
					if (valueAttribute.as_bool()) {
						abilities.conditionSuppressions |= CONDITION_ENERGY;
					}
					break;
				}

				case ITEM_PARSE_SUPPRESSFIRE: {
					if (valueAttribute.as_bool()) {
						abilities.conditionSuppressions |= CONDITION_FIRE;
					}
					break;
				}

				case ITEM_PARSE_SUPPRESSPOISON: {
					if (valueAttribute.as_bool()) {
						abilities.conditionSuppressions |= CONDITION_POISON;
					}
					break;
				}

				case ITEM_PARSE_SUPPRESSDROWN: {
					if (valueAttribute.as_bool()) {
						abilities.conditionSuppressions |= CONDITION_DROWN;
					}
					break;
				}

				case ITEM_PARSE_SUPPRESSPHYSICAL: {
					if (valueAttribute.as_bool()) {
						abilities.conditionSuppressions |= CONDITION_BLEEDING;
					}
					break;
				}

				case ITEM_PARSE_SUPPRESSFREEZE: {
					if (valueAttribute.as_bool()) {
						abilities.conditionSuppressions |= CONDITION_FREEZING;
					}
					break;
				}

				case ITEM_PARSE_SUPPRESSDAZZLE: {
					if (valueAttribute.as_bool()) {
						abilities.conditionSuppressions |= CONDITION_DAZZLED;
					}
					break;
				}

				case ITEM_PARSE_SUPPRESSCURSE: {
					if (valueAttribute.as_bool()) {
						abilities.conditionSuppressions |= CONDITION_CURSED;
					}
					break;
				}

				case ITEM_PARSE_FIELD: {
					it.group = ITEM_GROUP_MAGICFIELD;
					it.type = ITEM_TYPE_MAGICFIELD;

					CombatType_t combatType = COMBAT_NONE;
					ConditionDamage* conditionDamage = nullptr;

					tmpStrValue = boost::algorithm::to_lower_copy<std::string>(valueAttribute.as_string());
					if (tmpStrValue == "fire") {
						conditionDamage = new ConditionDamage(CONDITIONID_COMBAT, CONDITION_FIRE);
						combatType = COMBAT_FIREDAMAGE;
					} else if (tmpStrValue == "energy") {
						conditionDamage = new ConditionDamage(CONDITIONID_COMBAT, CONDITION_ENERGY);
						combatType = COMBAT_ENERGYDAMAGE;
					} else if (tmpStrValue == "poison") {
						conditionDamage = new ConditionDamage(CONDITIONID_COMBAT, CONDITION_POISON);
						combatType = COMBAT_EARTHDAMAGE;
					} else if (tmpStrValue == "drown") {
						conditionDamage = new ConditionDamage(CONDITIONID_COMBAT, CONDITION_DROWN);
						combatType = COMBAT_DROWNDAMAGE;
					} else if (tmpStrValue == "physical") {
						conditionDamage = new ConditionDamage(CONDITIONID_COMBAT, CONDITION_BLEEDING);
						combatType = COMBAT_PHYSICALDAMAGE;
					} else {
						std::cout << "[Warning - Items::parseItemNode] Unknown field value: "
						          << valueAttribute.as_string() << std::endl;
					}

					if (combatType != COMBAT_NONE) {
						it.combatType = combatType;
						it.conditionDamage.reset(conditionDamage);

						uint32_t ticks = 0;
						int32_t start = 0;
						int32_t count = 1;
						int32_t initDamage = -1;
						int32_t damage = 0;
						for (auto subAttributeNode : attributeNode.children()) {
							pugi::xml_attribute subKeyAttribute = subAttributeNode.attribute("key");
							if (!subKeyAttribute) {
								continue;
							}

							pugi::xml_attribute subValueAttribute = subAttributeNode.attribute("value");
							if (!subValueAttribute) {
								continue;
							}

							tmpStrValue = boost::algorithm::to_lower_copy<std::string>(subKeyAttribute.as_string());
							if (tmpStrValue == "initdamage") {
								initDamage = pugi::cast<int32_t>(subValueAttribute.value());
							} else if (tmpStrValue == "ticks") {
								ticks = pugi::cast<uint32_t>(subValueAttribute.value());
							} else if (tmpStrValue == "count") {
								count = std::max<int32_t>(1, pugi::cast<int32_t>(subValueAttribute.value()));
							} else if (tmpStrValue == "start") {
								start = std::max<int32_t>(0, pugi::cast<int32_t>(subValueAttribute.value()));
							} else if (tmpStrValue == "damage") {
								damage = -pugi::cast<int32_t>(subValueAttribute.value());
								if (start > 0) {
									std::list<int32_t> damageList;
									ConditionDamage::generateDamageList(damage, start, damageList);
									for (int32_t damageValue : damageList) {
										conditionDamage->addDamage(1, ticks, -damageValue);
									}

									start = 0;
								} else {
									conditionDamage->addDamage(count, ticks, damage);
								}
							}
						}

						// datapack compatibility, presume damage to be initialdamage if initialdamage is not declared.
						// initDamage = 0 (don't override initDamage with damage, don't set any initDamage)
						// initDamage = -1 (undefined, override initDamage with damage)
						if (initDamage > 0 || initDamage < -1) {
							conditionDamage->setInitDamage(-initDamage);
						} else if (initDamage == -1 && start != 0) {
							conditionDamage->setInitDamage(start);
						}

						conditionDamage->setParam(CONDITION_PARAM_FIELD, 1);

						if (conditionDamage->getTotalDamage() > 0) {
							conditionDamage->setParam(CONDITION_PARAM_FORCEUPDATE, 1);
						}
					}
					break;
				}

				case ITEM_PARSE_REPLACEABLE: {
					it.replaceable = valueAttribute.as_bool();
					break;
				}

				case ITEM_PARSE_PARTNERDIRECTION: {
					it.bedPartnerDir = getDirection(valueAttribute.as_string());
					break;
				}

				case ITEM_PARSE_LEVELDOOR: {
					it.levelDoor = pugi::cast<uint32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_MALETRANSFORMTO: {
					uint16_t value = pugi::cast<uint16_t>(valueAttribute.value());
					it.transformToOnUse[PLAYERSEX_MALE] = value;
					ItemType& other = getItemType(value);
					if (other.transformToFree == 0) {
						other.transformToFree = it.id;
					}

					if (it.transformToOnUse[PLAYERSEX_FEMALE] == 0) {
						it.transformToOnUse[PLAYERSEX_FEMALE] = value;
					}
					break;
				}

				case ITEM_PARSE_FEMALETRANSFORMTO: {
					uint16_t value = pugi::cast<uint16_t>(valueAttribute.value());
					it.transformToOnUse[PLAYERSEX_FEMALE] = value;

					ItemType& other = getItemType(value);
					if (other.transformToFree == 0) {
						other.transformToFree = it.id;
					}

					if (it.transformToOnUse[PLAYERSEX_MALE] == 0) {
						it.transformToOnUse[PLAYERSEX_MALE] = value;
					}
					break;
				}

				case ITEM_PARSE_TRANSFORMTO: {
					it.transformToFree = pugi::cast<uint16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_DESTROYTO: {
					it.destroyTo = pugi::cast<uint16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ELEMENTICE: {
					abilities.elementDamage = pugi::cast<uint16_t>(valueAttribute.value());
					abilities.elementType = COMBAT_ICEDAMAGE;
					break;
				}

				case ITEM_PARSE_ELEMENTEARTH: {
					abilities.elementDamage = pugi::cast<uint16_t>(valueAttribute.value());
					abilities.elementType = COMBAT_EARTHDAMAGE;
					break;
				}

				case ITEM_PARSE_ELEMENTFIRE: {
					abilities.elementDamage = pugi::cast<uint16_t>(valueAttribute.value());
					abilities.elementType = COMBAT_FIREDAMAGE;
					break;
				}

				case ITEM_PARSE_ELEMENTENERGY: {
					abilities.elementDamage = pugi::cast<uint16_t>(valueAttribute.value());
					abilities.elementType = COMBAT_ENERGYDAMAGE;
					break;
				}

				case ITEM_PARSE_ELEMENTDEATH: {
					abilities.elementDamage = pugi::cast<uint16_t>(valueAttribute.value());
					abilities.elementType = COMBAT_DEATHDAMAGE;
					break;
				}

				case ITEM_PARSE_ELEMENTHOLY: {
					abilities.elementDamage = pugi::cast<uint16_t>(valueAttribute.value());
					abilities.elementType = COMBAT_HOLYDAMAGE;
					break;
				}

				case ITEM_PARSE_WALKSTACK: {
					it.walkStack = valueAttribute.as_bool();
					break;
				}

				case ITEM_PARSE_BLOCKING: {
					it.blockSolid = valueAttribute.as_bool();
					break;
				}

				case ITEM_PARSE_ALLOWDISTREAD: {
					it.allowDistRead = booleanString(valueAttribute.as_string());
					break;
				}

				case ITEM_PARSE_STOREITEM: {
					it.storeItem = booleanString(valueAttribute.as_string());
					break;
				}

				case ITEM_PARSE_WORTH: {
					uint64_t worth = pugi::cast<uint64_t>(valueAttribute.value());
					if (currencyItems.find(worth) != currencyItems.end()) {
						std::cout << "[Warning - Items::parseItemNode] Duplicated currency worth. Item " << id
						          << " redefines worth " << worth << std::endl;
					} else {
						currencyItems.insert(CurrencyMap::value_type(worth, id));
						it.worth = worth;
					}
					break;
				}

				default: {
					// It should not ever get to here, only if you add a new key to the map and don't configure a case
					// for it.
					std::cout << "[Warning - Items::parseItemNode] Not configured key value: "
					          << keyAttribute.as_string() << std::endl;
					break;
				}
			}
		} else {
			std::cout << "[Warning - Items::parseItemNode] Unknown key value: " << keyAttribute.as_string()
			          << std::endl;
		}
	}

	// check bed items
	if ((it.transformToFree != 0 || it.transformToOnUse[PLAYERSEX_FEMALE] != 0 ||
	     it.transformToOnUse[PLAYERSEX_MALE] != 0) &&
	    it.type != ITEM_TYPE_BED) {
		std::cout << "[Warning - Items::parseItemNode] Item " << it.id << " is not set as a bed-type" << std::endl;
	}
}

ItemType& Items::getItemType(size_t id)
{
	if (id < items.size()) {
		return items[id];
	}
	return items.front();
}

const ItemType& Items::getItemType(size_t id) const
{
	if (id < items.size()) {
		return items[id];
	}
	return items.front();
}

const ItemType& Items::getItemIdByClientId(uint16_t spriteId) const
{
	if (spriteId >= 100) {
		if (uint16_t serverId = clientIdToServerIdMap.getServerId(spriteId)) {
			return getItemType(serverId);
		}
	}
	return items.front();
}

uint16_t Items::getItemIdByName(const std::string& name)
{
	if (name.empty()) {
		return 0;
	}

	auto result = nameToItems.find(boost::algorithm::to_lower_copy(name));
	if (result == nameToItems.end()) return 0;

	return result->second;
}
