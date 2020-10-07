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

#include "otpch.h"

#include "items.h"
#include "spells.h"
#include "movement.h"
#include "weapons.h"

#include "pugicast.h"

extern MoveEvents* g_moveEvents;
extern Weapons* g_weapons;

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
};

const std::unordered_map<std::string, ItemTypes_t> ItemTypesMap = {
	{"key", ITEM_TYPE_KEY},
	{"magicfield", ITEM_TYPE_MAGICFIELD},
	{"container", ITEM_TYPE_CONTAINER},
	{"depot", ITEM_TYPE_DEPOT},
	{"mailbox", ITEM_TYPE_MAILBOX},
	{"trashholder", ITEM_TYPE_TRASHHOLDER},
	{"teleport", ITEM_TYPE_TELEPORT},
	{"door", ITEM_TYPE_DOOR},
	{"bed", ITEM_TYPE_BED},
	{"rune", ITEM_TYPE_RUNE},
};

const std::unordered_map<std::string, tileflags_t> TileStatesMap = {
	{"down", TILESTATE_FLOORCHANGE_DOWN},
	{"north", TILESTATE_FLOORCHANGE_NORTH},
	{"south", TILESTATE_FLOORCHANGE_SOUTH},
	{"southalt", TILESTATE_FLOORCHANGE_SOUTH_ALT},
	{"west", TILESTATE_FLOORCHANGE_WEST},
	{"east", TILESTATE_FLOORCHANGE_EAST},
	{"eastalt", TILESTATE_FLOORCHANGE_EAST_ALT},
};

const std::unordered_map<std::string, RaceType_t> RaceTypesMap = {
	{"venom", RACE_VENOM},
	{"blood", RACE_BLOOD},
	{"undead", RACE_UNDEAD},
	{"fire", RACE_FIRE},
	{"energy", RACE_ENERGY},
};

const std::unordered_map<std::string, WeaponType_t> WeaponTypesMap = {
	{"sword", WEAPON_SWORD},
	{"club", WEAPON_CLUB},
	{"axe", WEAPON_AXE},
	{"shield", WEAPON_SHIELD},
	{"distance", WEAPON_DISTANCE},
	{"wand", WEAPON_WAND},
	{"ammunition", WEAPON_AMMO},
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
};


Items::Items()
{
	items.reserve(30000);
	nameToItems.reserve(30000);
}

void Items::clear()
{
	items.clear();
	clientIdToServerIdMap.clear();
	nameToItems.clear();
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

constexpr auto OTBI = OTB::Identifier{{'O','T', 'B', 'I'}};

bool Items::loadFromOtb(const std::string& file)
{
	OTB::Loader loader{file, OTBI};

	auto& root = loader.parseTree();

	PropStream props;
	if (loader.getProps(root, props)) {
		//4 byte flags
		//attributes
		//0x01 = version data
		uint32_t flags;
		if (!props.read<uint32_t>(flags)) {
			return false;
		}

		uint8_t attr;
		if (!props.read<uint8_t>(attr)) {
			return false;
		}

		if (attr == ROOT_ATTR_VERSION) {
			uint16_t datalen;
			if (!props.read<uint16_t>(datalen)) {
				return false;
			}

			if (datalen != sizeof(VERSIONINFO)) {
				return false;
			}

			VERSIONINFO vi;
			if (!props.read(vi)) {
				return false;
			}

			majorVersion = vi.dwMajorVersion; //items otb format file version
			minorVersion = vi.dwMinorVersion; //client version
			buildNumber = vi.dwBuildNumber; //revision
		}
	}

	if (majorVersion == 0xFFFFFFFF) {
		std::cout << "[Warning - Items::loadFromOtb] items.otb using generic client version." << std::endl;
	} else if (majorVersion != 3) {
		std::cout << "Old version detected, a newer version of items.otb is required." << std::endl;
		return false;
	} else if (minorVersion < CLIENT_VERSION_1098) {
		std::cout << "A newer version of items.otb is required." << std::endl;
		return false;
	}

	for (auto& itemNode : root.children) {
		PropStream stream;
		if (!loader.getProps(itemNode, stream)) {
			return false;
		}

		uint32_t flags;
		if (!stream.read<uint32_t>(flags)) {
			return false;
		}

		uint16_t serverId = 0;
		uint16_t clientId = 0;
		uint16_t speed = 0;
		uint16_t wareId = 0;
		uint8_t lightLevel = 0;
		uint8_t lightColor = 0;
		uint8_t alwaysOnTopOrder = 0;

		uint8_t attrib;
		while (stream.read<uint8_t>(attrib)) {
			uint16_t datalen;
			if (!stream.read<uint16_t>(datalen)) {
				return false;
			}

			switch (attrib) {
				case ITEM_ATTR_SERVERID: {
					if (datalen != sizeof(uint16_t)) {
						return false;
					}

					if (!stream.read<uint16_t>(serverId)) {
						return false;
					}

					if (serverId > 30000 && serverId < 30100) {
						serverId -= 30000;
					}
					break;
				}

				case ITEM_ATTR_CLIENTID: {
					if (datalen != sizeof(uint16_t)) {
						return false;
					}

					if (!stream.read<uint16_t>(clientId)) {
						return false;
					}
					break;
				}

				case ITEM_ATTR_SPEED: {
					if (datalen != sizeof(uint16_t)) {
						return false;
					}

					if (!stream.read<uint16_t>(speed)) {
						return false;
					}
					break;
				}

				case ITEM_ATTR_LIGHT2: {
					if (datalen != sizeof(lightBlock2)) {
						return false;
					}

					lightBlock2 lb2;
					if (!stream.read(lb2)) {
						return false;
					}

					lightLevel = static_cast<uint8_t>(lb2.lightLevel);
					lightColor = static_cast<uint8_t>(lb2.lightColor);
					break;
				}

				case ITEM_ATTR_TOPORDER: {
					if (datalen != sizeof(uint8_t)) {
						return false;
					}

					if (!stream.read<uint8_t>(alwaysOnTopOrder)) {
						return false;
					}
					break;
				}

				case ITEM_ATTR_WAREID: {
					if (datalen != sizeof(uint16_t)) {
						return false;
					}

					if (!stream.read<uint16_t>(wareId)) {
						return false;
					}
					break;
				}

				default: {
					//skip unknown attributes
					if (!stream.skip(datalen)) {
						return false;
					}
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
				//not used
				iType.type = ITEM_TYPE_DOOR;
				break;
			case ITEM_GROUP_MAGICFIELD:
				//not used
				iType.type = ITEM_TYPE_MAGICFIELD;
				break;
			case ITEM_GROUP_TELEPORT:
				//not used
				iType.type = ITEM_TYPE_TELEPORT;
				break;
			case ITEM_GROUP_NONE:
			case ITEM_GROUP_GROUND:
			case ITEM_GROUP_SPLASH:
			case ITEM_GROUP_FLUID:
			case ITEM_GROUP_CHARGES:
			case ITEM_GROUP_DEPRECATED:
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

		iType.id = serverId;
		iType.clientId = clientId;
		iType.speed = speed;
		iType.lightLevel = lightLevel;
		iType.lightColor = lightColor;
		iType.wareId = wareId;
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
			std::cout << "[Warning - Items::loadFromXml] fromid (" << fromIdAttribute.value() << ") without toid" << std::endl;
			continue;
		}

		uint16_t id = pugi::cast<uint16_t>(fromIdAttribute.value());
		uint16_t toId = pugi::cast<uint16_t>(toIdAttribute.value());
		while (id <= toId) {
			parseItemNode(itemNode, id++);
		}
	}

	buildInventoryList();
	return true;
}

void Items::buildInventoryList()
{
	inventory.reserve(items.size());
	for (const auto& type: items) {
		if (type.weaponType != WEAPON_NONE || type.ammoType != AMMO_NONE ||
			type.attack != 0 || type.defense != 0 ||
			type.extraDefense != 0 || type.armor != 0 ||
			type.slotPosition & SLOTP_NECKLACE ||
			type.slotPosition & SLOTP_RING ||
			type.slotPosition & SLOTP_AMMO ||
			type.slotPosition & SLOTP_FEET ||
			type.slotPosition & SLOTP_HEAD ||
			type.slotPosition & SLOTP_ARMOR ||
			type.slotPosition & SLOTP_LEGS)
		{
			inventory.push_back(type.clientId);
		}
	}
	inventory.shrink_to_fit();
	std::sort(inventory.begin(), inventory.end());
}

void Items::parseItemNode(const pugi::xml_node& itemNode, uint16_t id)
{
	if (id > 30000 && id < 30100) {
		id -= 30000;

		if (id >= items.size()) {
			items.resize(id + 1);
		}
		ItemType& iType = items[id];
		iType.id = id;
	}

	ItemType& it = getItemType(id);
	if (it.id == 0) {
		return;
	}

	it.name = itemNode.attribute("name").as_string();

	nameToItems.insert({ asLowerCaseString(it.name), id });

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
		if (!valueAttribute) {
			continue;
		}

		std::string tmpStrValue = asLowerCaseString(keyAttribute.as_string());
		auto parseAttribute = ItemParseAttributesMap.find(tmpStrValue);
		if (parseAttribute != ItemParseAttributesMap.end()) {
			ItemParseAttributes_t parseType = parseAttribute->second;
			switch (parseType) {
				case ITEM_PARSE_TYPE: {
					tmpStrValue = asLowerCaseString(valueAttribute.as_string());
					auto it2 = ItemTypesMap.find(tmpStrValue);
					if (it2 != ItemTypesMap.end()) {
						it.type = it2->second;
						if (it.type == ITEM_TYPE_CONTAINER) {
							it.group = ITEM_GROUP_CONTAINER;
						}
					} else {
						std::cout << "[Warning - Items::parseItemNode] Unknown type: " << valueAttribute.as_string() << std::endl;
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
					tmpStrValue = asLowerCaseString(valueAttribute.as_string());
					auto it2 = TileStatesMap.find(tmpStrValue);
					if (it2 != TileStatesMap.end()) {
						it.floorChange |= it2->second;
					} else {
						std::cout << "[Warning - Items::parseItemNode] Unknown floorChange: " << valueAttribute.as_string() << std::endl;
					}
					break;
				}

				case ITEM_PARSE_CORPSETYPE: {
					tmpStrValue = asLowerCaseString(valueAttribute.as_string());
					auto it2 = RaceTypesMap.find(tmpStrValue);
					if (it2 != RaceTypesMap.end()) {
						it.corpseType = it2->second;
					} else {
						std::cout << "[Warning - Items::parseItemNode] Unknown corpseType: " << valueAttribute.as_string() << std::endl;
					}
					break;
				}

				case ITEM_PARSE_CONTAINERSIZE: {
					it.maxItems = pugi::cast<uint16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_FLUIDSOURCE: {
					tmpStrValue = asLowerCaseString(valueAttribute.as_string());
					auto it2 = FluidTypesMap.find(tmpStrValue);
					if (it2 != FluidTypesMap.end()) {
						it.fluidSource = it2->second;
					} else {
						std::cout << "[Warning - Items::parseItemNode] Unknown fluidSource: " << valueAttribute.as_string() << std::endl;
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
					tmpStrValue = asLowerCaseString(valueAttribute.as_string());
					auto it2 = WeaponTypesMap.find(tmpStrValue);
					if (it2 != WeaponTypesMap.end()) {
						it.weaponType = it2->second;
					} else {
						std::cout << "[Warning - Items::parseItemNode] Unknown weaponType: " << valueAttribute.as_string() << std::endl;
					}
					break;
				}

				case ITEM_PARSE_SLOTTYPE: {
					tmpStrValue = asLowerCaseString(valueAttribute.as_string());
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
						std::cout << "[Warning - Items::parseItemNode] Unknown slotType: " << valueAttribute.as_string() << std::endl;
					}
					break;
				}

				case ITEM_PARSE_AMMOTYPE: {
					it.ammoType = getAmmoType(asLowerCaseString(valueAttribute.as_string()));
					if (it.ammoType == AMMO_NONE) {
						std::cout << "[Warning - Items::parseItemNode] Unknown ammoType: " << valueAttribute.as_string() << std::endl;
					}
					break;
				}

				case ITEM_PARSE_SHOOTTYPE: {
					ShootType_t shoot = getShootType(asLowerCaseString(valueAttribute.as_string()));
					if (shoot != CONST_ANI_NONE) {
						it.shootType = shoot;
					} else {
						std::cout << "[Warning - Items::parseItemNode] Unknown shootType: " << valueAttribute.as_string() << std::endl;
					}
					break;
				}

				case ITEM_PARSE_EFFECT: {
					MagicEffectClasses effect = getMagicEffect(asLowerCaseString(valueAttribute.as_string()));
					if (effect != CONST_ME_NONE) {
						it.magicEffect = effect;
					} else {
						std::cout << "[Warning - Items::parseItemNode] Unknown effect: " << valueAttribute.as_string() << std::endl;
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
					it.decayTime = pugi::cast<uint32_t>(valueAttribute.value());
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
					it.hitChance = std::min<int8_t>(100, std::max<int8_t>(-100, pugi::cast<int16_t>(valueAttribute.value())));
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
					abilities.specialSkills[SPECIALSKILL_CRITICALHITAMOUNT] = pugi::cast<int32_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_CRITICALHITCHANCE: {
					abilities.specialSkills[SPECIALSKILL_CRITICALHITCHANCE] = pugi::cast<int32_t>(valueAttribute.value());
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
					abilities.fieldAbsorbPercent[combatTypeToIndex(COMBAT_ENERGYDAMAGE)] += pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_FIELDABSORBPERCENTFIRE: {
					abilities.fieldAbsorbPercent[combatTypeToIndex(COMBAT_FIREDAMAGE)] += pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_FIELDABSORBPERCENTPOISON: {
					abilities.fieldAbsorbPercent[combatTypeToIndex(COMBAT_EARTHDAMAGE)] += pugi::cast<int16_t>(valueAttribute.value());
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
					abilities.absorbPercent[combatTypeToIndex(COMBAT_ENERGYDAMAGE)] += pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTFIRE: {
					abilities.absorbPercent[combatTypeToIndex(COMBAT_FIREDAMAGE)] += pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTPOISON: {
					abilities.absorbPercent[combatTypeToIndex(COMBAT_EARTHDAMAGE)] += pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTICE: {
					abilities.absorbPercent[combatTypeToIndex(COMBAT_ICEDAMAGE)] += pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTHOLY: {
					abilities.absorbPercent[combatTypeToIndex(COMBAT_HOLYDAMAGE)] += pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTDEATH: {
					abilities.absorbPercent[combatTypeToIndex(COMBAT_DEATHDAMAGE)] += pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTLIFEDRAIN: {
					abilities.absorbPercent[combatTypeToIndex(COMBAT_LIFEDRAIN)] += pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTMANADRAIN: {
					abilities.absorbPercent[combatTypeToIndex(COMBAT_MANADRAIN)] += pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTDROWN: {
					abilities.absorbPercent[combatTypeToIndex(COMBAT_DROWNDAMAGE)] += pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTPHYSICAL: {
					abilities.absorbPercent[combatTypeToIndex(COMBAT_PHYSICALDAMAGE)] += pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTHEALING: {
					abilities.absorbPercent[combatTypeToIndex(COMBAT_HEALING)] += pugi::cast<int16_t>(valueAttribute.value());
					break;
				}

				case ITEM_PARSE_ABSORBPERCENTUNDEFINED: {
					abilities.absorbPercent[combatTypeToIndex(COMBAT_UNDEFINEDDAMAGE)] += pugi::cast<int16_t>(valueAttribute.value());
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

					tmpStrValue = asLowerCaseString(valueAttribute.as_string());
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
						std::cout << "[Warning - Items::parseItemNode] Unknown field value: " << valueAttribute.as_string() << std::endl;
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

							tmpStrValue = asLowerCaseString(subKeyAttribute.as_string());
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
						// initDamage = 0 (dont override initDamage with damage, dont set any initDamage)
						// initDamage = -1 (undefined, override initDamage with damage)
						if (initDamage > 0 || initDamage < -1) {
							conditionDamage->setInitDamage(-initDamage);
						} else if (initDamage == -1 && damage != 0) {
							conditionDamage->setInitDamage(damage);
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

				default: {
					// It should not ever get to here, only if you add a new key to the map and don't configure a case for it.
					std::cout << "[Warning - Items::parseItemNode] Not configured key value: " << keyAttribute.as_string() << std::endl;
					break;
				}
			}
		} else {
			std::cout << "[Warning - Items::parseItemNode] Unknown key value: " << keyAttribute.as_string() << std::endl;
		}
	}

	//check bed items
	if ((it.transformToFree != 0 || it.transformToOnUse[PLAYERSEX_FEMALE] != 0 || it.transformToOnUse[PLAYERSEX_MALE] != 0) && it.type != ITEM_TYPE_BED) {
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
	auto result = nameToItems.find(asLowerCaseString(name));

	if (result == nameToItems.end())
		return 0;

	return result->second;
}
