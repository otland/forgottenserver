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

#ifndef FS_ITEMS_H_4E2221634ABA45FE85BA50F710669B3C
#define FS_ITEMS_H_4E2221634ABA45FE85BA50F710669B3C

#include "const.h"
#include "enums.h"
#include "itemloader.h"
#include "position.h"

#define SLOTP_WHEREEVER 0xFFFFFFFF
#define SLOTP_HEAD 1
#define	SLOTP_NECKLACE 2
#define	SLOTP_BACKPACK 4
#define	SLOTP_ARMOR 8
#define	SLOTP_RIGHT 16
#define	SLOTP_LEFT 32
#define	SLOTP_LEGS 64
#define	SLOTP_FEET 128
#define	SLOTP_RING 256
#define	SLOTP_AMMO 512
#define	SLOTP_DEPOT 1024
#define	SLOTP_TWO_HAND 2048
#define SLOTP_HAND (SLOTP_LEFT | SLOTP_RIGHT)

enum ItemTypes_t {
	ITEM_TYPE_NONE = 0,
	ITEM_TYPE_DEPOT,
	ITEM_TYPE_MAILBOX,
	ITEM_TYPE_TRASHHOLDER,
	ITEM_TYPE_CONTAINER,
	ITEM_TYPE_DOOR,
	ITEM_TYPE_MAGICFIELD,
	ITEM_TYPE_TELEPORT,
	ITEM_TYPE_BED,
	ITEM_TYPE_KEY,
	ITEM_TYPE_RUNE,
	ITEM_TYPE_LAST
};

struct Abilities {
	Abilities() : skills(), fieldAbsorbPercent(), absorbPercent(), stats(), statsPercent() {
		elementType = COMBAT_NONE;
		elementDamage = 0;

		speed = 0;
		manaShield = false;
		invisible = false;
		regeneration = false;

		healthGain = 0;
		healthTicks = 0;
		manaGain = 0;
		manaTicks = 0;

		conditionImmunities = 0;
		conditionSuppressions = 0;
	};

	//elemental damage
	CombatType_t elementType;
	int16_t elementDamage;

	//extra skill modifiers
	int32_t skills[SKILL_LAST + 1];

	// field damage abilities modifiers
	int16_t fieldAbsorbPercent[COMBAT_COUNT + 1];

	//damage abilities modifiers
	int16_t absorbPercent[COMBAT_COUNT + 1];

	//stats modifiers
	int32_t stats[STAT_LAST + 1];
	int32_t statsPercent[STAT_LAST + 1];

	int32_t speed;
	bool manaShield;
	bool invisible;
	bool regeneration;

	uint32_t healthGain;
	uint32_t healthTicks;
	uint32_t manaGain;
	uint32_t manaTicks;

	uint32_t conditionImmunities;
	uint32_t conditionSuppressions;
};

class Condition;

class ItemType
{
	public:
		ItemType();
		~ItemType();

		bool isGroundTile() const {
			return (group == ITEM_GROUP_GROUND);
		}
		bool isContainer() const {
			return (group == ITEM_GROUP_CONTAINER);
		}
		bool isSplash() const {
			return (group == ITEM_GROUP_SPLASH);
		}
		bool isFluidContainer() const {
			return (group == ITEM_GROUP_FLUID);
		}

		bool isDoor() const {
			return (type == ITEM_TYPE_DOOR);
		}
		bool isMagicField() const {
			return (type == ITEM_TYPE_MAGICFIELD);
		}
		bool isTeleport() const {
			return (type == ITEM_TYPE_TELEPORT);
		}
		bool isKey() const {
			return (type == ITEM_TYPE_KEY);
		}
		bool isDepot() const {
			return (type == ITEM_TYPE_DEPOT);
		}
		bool isMailbox() const {
			return (type == ITEM_TYPE_MAILBOX);
		}
		bool isTrashHolder() const {
			return (type == ITEM_TYPE_TRASHHOLDER);
		}
		bool isBed() const {
			return (type == ITEM_TYPE_BED);
		}
		bool isRune() const {
			return type == ITEM_TYPE_RUNE;
		}
		bool hasSubType() const {
			return (isFluidContainer() || isSplash() || stackable || charges != 0);
		}

		Abilities* getAbilities() {
			if (abilities == nullptr) {
				abilities = new Abilities();
			}

			return abilities;
		}

		std::string getPluralName() const {
			if (!pluralName.empty()) {
				return pluralName;
			}

			if (showCount == 0) {
				return name;
			}

			std::string str;
			str.reserve(name.length() + 1);
			str.assign(name);
			str += 's';
			return str;
		}

		itemgroup_t group;
		ItemTypes_t type;
		uint16_t id;
		uint16_t clientId;
		bool stackable;
		bool isAnimation;

		std::string name;
		std::string article;
		std::string pluralName;
		std::string description;
		std::string runeSpellName;
		std::string vocationString;

		Abilities* abilities;
		Condition* condition;

		float weight;

		uint32_t levelDoor;
		uint32_t decayTime;
		uint32_t wieldInfo;
		uint32_t minReqLevel;
		uint32_t minReqMagicLevel;
		uint32_t charges;
		uint32_t shootRange;
		int32_t breakChance;
		int32_t hitChance;
		int32_t maxHitChance;
		int32_t decayTo;
		int32_t attack;
		int32_t defense;
		int32_t extraDefense;
		int32_t armor;
		int32_t rotateTo;
		int32_t runeMagLevel;
		int32_t runeLevel;

		WeaponType_t weaponType;
		Ammo_t ammoType;
		ShootType_t shootType;
		MagicEffectClasses magicEffect;
		RaceType_t corpseType;
		Direction bedPartnerDir;
		AmmoAction_t ammoAction;
		FluidTypes_t fluidSource;
		CombatType_t combatType;

		uint16_t transformToOnUse[2];
		uint16_t transformToFree;
		uint16_t maxTextLen;
		uint16_t writeOnceItemId;
		uint16_t transformEquipTo;
		uint16_t transformDeEquipTo;
		uint16_t maxItems;
		uint16_t slotPosition;
		uint16_t speed;
		uint16_t wareId;
		uint16_t lightLevel;
		uint16_t lightColor;

		uint8_t alwaysOnTopOrder;

		bool floorChangeDown;
		bool floorChangeNorth;
		bool floorChangeSouth;
		bool floorChangeSouthAlt;
		bool floorChangeEast;
		bool floorChangeEastAlt;
		bool floorChangeWest;
		bool hasHeight;
		bool walkStack;
		bool blockSolid;
		bool blockPickupable;
		bool blockProjectile;
		bool blockPathFind;
		bool allowPickupable;
		bool showDuration;
		bool showCharges;
		bool showAttributes;
		bool replaceable;
		bool pickupable;
		bool rotable;
		bool useable;
		bool moveable;
		bool alwaysOnTop;
		bool canReadText;
		bool canWriteText;
		bool isVertical;
		bool isHorizontal;
		bool isHangable;
		bool allowDistRead;
		bool lookThrough;
		bool stopTime;
		bool showCount;
};

class Items
{
	public:
		Items();
		~Items();

		bool reload();
		void clear();

		int32_t loadFromOtb(const std::string& file);

		const ItemType& operator[](size_t id) const {
			return getItemType(id);
		}
		const ItemType& getItemType(size_t id) const;
		ItemType& getItemType(size_t id);
		const ItemType& getItemIdByClientId(uint16_t spriteId) const;

		uint16_t getItemIdByName(const std::string& name);

		static uint32_t dwMajorVersion;
		static uint32_t dwMinorVersion;
		static uint32_t dwBuildNumber;

		bool loadFromXml();
		void parseItemNode(const pugi::xml_node& itemNode, uint32_t id);

		inline size_t size() const {
			return items.size();
		}

	protected:
		std::map<uint16_t, uint16_t> reverseItemMap;
		std::vector<ItemType> items;
};
#endif
