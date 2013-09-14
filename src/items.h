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

#ifndef __OTSERV_ITEMS_H__
#define __OTSERV_ITEMS_H__

#include "definitions.h"
#include "const.h"
#include "enums.h"
#include "itemloader.h"
#include "position.h"
#include <map>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

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
	Abilities() {
		elementType = COMBAT_NONE;
		elementDamage = 0;
		memset(skills, 0, sizeof(skills));
		memset(absorbPercent, 0, sizeof(absorbPercent));
		memset(stats, 0 , sizeof(stats));
		memset(statsPercent, 0, sizeof(statsPercent));

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
	private:
		ItemType(const ItemType& it) {}

	public:
		ItemType();
		virtual ~ItemType();

		itemgroup_t group;
		ItemTypes_t type;

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
			if (abilities == NULL) {
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

		Direction bedPartnerDir;
		uint16_t transformToOnUse[2];
		uint16_t transformToFree;

		uint16_t id;
		uint16_t clientId;

		std::string name;
		std::string article;
		std::string pluralName;
		std::string description;
		uint16_t maxItems;
		float weight;
		bool showCount;
		WeaponType_t weaponType;
		Ammo_t ammoType;
		ShootType_t shootType;
		MagicEffectClasses magicEffect;
		int32_t attack;
		int32_t defense;
		int32_t extraDefense;
		int32_t armor;
		uint16_t slotPosition;
		uint32_t levelDoor;
		bool isVertical;
		bool isHorizontal;
		bool isHangable;
		bool allowDistRead;
		bool lookThrough;
		bool isAnimation;
		uint16_t speed;
		int32_t decayTo;
		uint32_t decayTime;
		bool stopTime;
		RaceType_t corpseType;

		bool canReadText;
		bool canWriteText;
		uint16_t maxTextLen;
		uint16_t writeOnceItemId;

		bool stackable;
		bool useable;
		bool moveable;
		bool alwaysOnTop;
		int32_t alwaysOnTopOrder;
		bool pickupable;
		bool rotable;
		int32_t rotateTo;

		int32_t runeMagLevel;
		int32_t runeLevel;
		std::string runeSpellName;

		uint32_t wieldInfo;
		std::string vocationString;
		uint32_t minReqLevel;
		uint32_t minReqMagicLevel;

		int32_t lightLevel;
		int32_t lightColor;

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

		unsigned short transformEquipTo;
		unsigned short transformDeEquipTo;
		bool showDuration;
		bool showCharges;
		bool showAttributes;
		uint32_t charges;
		int32_t breakChance;
		int32_t hitChance;
		int32_t maxHitChance;
		uint32_t shootRange;
		AmmoAction_t ammoAction;
		FluidTypes_t fluidSource;

		Abilities* abilities;

		Condition* condition;
		CombatType_t combatType;
		bool replaceable;
		bool ware;

		//		std::string marketName;
};

template<typename A>
class Array
{
	public:
		Array(uint32_t n);
		~Array();

		A getElement(uint32_t id);
		const A getElement(uint32_t id) const;
		void addElement(A a, uint32_t pos);

		void reset();

		uint32_t size() {
			return m_size;
		}

	private:
		A* m_data;
		uint32_t m_size;
};

class Items
{
	public:
		Items();
		~Items();

		bool reload();
		void clear();

		int32_t loadFromOtb(const std::string& file);

		const ItemType& operator[](int32_t id) const {
			return getItemType(id);
		}
		const ItemType& getItemType(int32_t id) const;
		ItemType& getItemType(int32_t id);
		const ItemType& getItemIdByClientId(int32_t spriteId) const;

		int32_t getItemIdByName(const std::string& name);

		static uint32_t dwMajorVersion;
		static uint32_t dwMinorVersion;
		static uint32_t dwBuildNumber;

		bool loadFromXml();
		bool parseItemNode(xmlNodePtr itemNode, uint32_t id);

		void addItemType(ItemType* iType);

		const ItemType* getElement(uint32_t id) const {
			return items->getElement(id);
		}
		uint32_t size() {
			return items->size();
		}

	protected:
		typedef std::map<int32_t, int32_t> ReverseItemMap;
		ReverseItemMap reverseItemMap;

		Array<ItemType*>* items;
};

template<typename A>
Array<A>::Array(uint32_t n)
{
	m_data = (A*)malloc(sizeof(A) * n);
	memset(m_data, 0, sizeof(A)*n);
	m_size = n;
}

template<typename A>
Array<A>::~Array()
{
	free(m_data);
}

template<typename A>
A Array<A>::getElement(uint32_t id)
{
	if (id < m_size) {
		return m_data[id];
	}

	return 0;
}

template<typename A>
const A Array<A>::getElement(uint32_t id) const
{
	if (id < m_size) {
		return m_data[id];
	}

	return 0;
}

template<typename A>
void Array<A>::addElement(A a, uint32_t pos)
{
#define INCREMENT 5000

	if (pos >= m_size) {
		m_data = (A*)realloc(m_data, sizeof(A) * (pos + INCREMENT));
		memset(m_data + m_size, 0, sizeof(A) * (pos + INCREMENT - m_size));
		m_size = pos + INCREMENT;
	}

	m_data[pos] = a;
}

template<typename A>
void Array<A>::reset()
{
	for (uint32_t i = 0; i < m_size; i++) {
		delete m_data[i];
		m_data[i] = NULL;
	}

	memset(this->m_data, 0, sizeof(A) * this->m_size);
}
#endif
