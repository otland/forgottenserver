// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_ITEMS_H
#define FS_ITEMS_H

#include "const.h"
#include "enums.h"
#include "itemloader.h"
#include "position.h"

class ConditionDamage;

enum SlotPositionBits : uint32_t
{
	SLOTP_WHEREEVER = 0xFFFFFFFF,
	SLOTP_HEAD = 1 << 0,
	SLOTP_NECKLACE = 1 << 1,
	SLOTP_BACKPACK = 1 << 2,
	SLOTP_ARMOR = 1 << 3,
	SLOTP_RIGHT = 1 << 4,
	SLOTP_LEFT = 1 << 5,
	SLOTP_LEGS = 1 << 6,
	SLOTP_FEET = 1 << 7,
	SLOTP_RING = 1 << 8,
	SLOTP_AMMO = 1 << 9,
	SLOTP_DEPOT = 1 << 10,
	SLOTP_TWO_HAND = 1 << 11,
	SLOTP_HAND = (SLOTP_LEFT | SLOTP_RIGHT)
};

enum ItemTypes_t
{
	ITEM_TYPE_NONE,
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
	ITEM_TYPE_PODIUM,
	ITEM_TYPE_LAST
};

enum ItemParseAttributes_t
{
	ITEM_PARSE_TYPE,
	ITEM_PARSE_DESCRIPTION,
	ITEM_PARSE_RUNESPELLNAME,
	ITEM_PARSE_WEIGHT,
	ITEM_PARSE_SHOWCOUNT,
	ITEM_PARSE_ARMOR,
	ITEM_PARSE_DEFENSE,
	ITEM_PARSE_EXTRADEF,
	ITEM_PARSE_ATTACK,
	ITEM_PARSE_ATTACK_SPEED,
	ITEM_PARSE_ROTATETO,
	ITEM_PARSE_MOVEABLE,
	ITEM_PARSE_BLOCKPROJECTILE,
	ITEM_PARSE_PICKUPABLE,
	ITEM_PARSE_FORCESERIALIZE,
	ITEM_PARSE_FLOORCHANGE,
	ITEM_PARSE_CORPSETYPE,
	ITEM_PARSE_CONTAINERSIZE,
	ITEM_PARSE_FLUIDSOURCE,
	ITEM_PARSE_READABLE,
	ITEM_PARSE_WRITEABLE,
	ITEM_PARSE_MAXTEXTLEN,
	ITEM_PARSE_WRITEONCEITEMID,
	ITEM_PARSE_WEAPONTYPE,
	ITEM_PARSE_SLOTTYPE,
	ITEM_PARSE_AMMOTYPE,
	ITEM_PARSE_SHOOTTYPE,
	ITEM_PARSE_EFFECT,
	ITEM_PARSE_RANGE,
	ITEM_PARSE_STOPDURATION,
	ITEM_PARSE_DECAYTO,
	ITEM_PARSE_TRANSFORMEQUIPTO,
	ITEM_PARSE_TRANSFORMDEEQUIPTO,
	ITEM_PARSE_DURATION,
	ITEM_PARSE_SHOWDURATION,
	ITEM_PARSE_CHARGES,
	ITEM_PARSE_SHOWCHARGES,
	ITEM_PARSE_SHOWATTRIBUTES,
	ITEM_PARSE_HITCHANCE,
	ITEM_PARSE_MAXHITCHANCE,
	ITEM_PARSE_INVISIBLE,
	ITEM_PARSE_SPEED,
	ITEM_PARSE_HEALTHGAIN,
	ITEM_PARSE_HEALTHTICKS,
	ITEM_PARSE_MANAGAIN,
	ITEM_PARSE_MANATICKS,
	ITEM_PARSE_MANASHIELD,
	ITEM_PARSE_SKILLSWORD,
	ITEM_PARSE_SKILLAXE,
	ITEM_PARSE_SKILLCLUB,
	ITEM_PARSE_SKILLDIST,
	ITEM_PARSE_SKILLFISH,
	ITEM_PARSE_SKILLSHIELD,
	ITEM_PARSE_SKILLFIST,
	ITEM_PARSE_MAXHITPOINTS,
	ITEM_PARSE_MAXHITPOINTSPERCENT,
	ITEM_PARSE_MAXMANAPOINTS,
	ITEM_PARSE_MAXMANAPOINTSPERCENT,
	ITEM_PARSE_MAGICPOINTS,
	ITEM_PARSE_MAGICPOINTSPERCENT,
	ITEM_PARSE_CRITICALHITCHANCE,
	ITEM_PARSE_CRITICALHITAMOUNT,
	ITEM_PARSE_LIFELEECHCHANCE,
	ITEM_PARSE_LIFELEECHAMOUNT,
	ITEM_PARSE_MANALEECHCHANCE,
	ITEM_PARSE_MANALEECHAMOUNT,
	ITEM_PARSE_FIELDABSORBPERCENTENERGY,
	ITEM_PARSE_FIELDABSORBPERCENTFIRE,
	ITEM_PARSE_FIELDABSORBPERCENTPOISON,
	ITEM_PARSE_ABSORBPERCENTALL,
	ITEM_PARSE_ABSORBPERCENTELEMENTS,
	ITEM_PARSE_ABSORBPERCENTMAGIC,
	ITEM_PARSE_ABSORBPERCENTENERGY,
	ITEM_PARSE_ABSORBPERCENTFIRE,
	ITEM_PARSE_ABSORBPERCENTPOISON,
	ITEM_PARSE_ABSORBPERCENTICE,
	ITEM_PARSE_ABSORBPERCENTHOLY,
	ITEM_PARSE_ABSORBPERCENTDEATH,
	ITEM_PARSE_ABSORBPERCENTLIFEDRAIN,
	ITEM_PARSE_ABSORBPERCENTMANADRAIN,
	ITEM_PARSE_ABSORBPERCENTDROWN,
	ITEM_PARSE_ABSORBPERCENTPHYSICAL,
	ITEM_PARSE_ABSORBPERCENTHEALING,
	ITEM_PARSE_ABSORBPERCENTUNDEFINED,
	ITEM_PARSE_MAGICLEVELENERGY,
	ITEM_PARSE_MAGICLEVELFIRE,
	ITEM_PARSE_MAGICLEVELPOISON,
	ITEM_PARSE_MAGICLEVELICE,
	ITEM_PARSE_MAGICLEVELHOLY,
	ITEM_PARSE_MAGICLEVELDEATH,
	ITEM_PARSE_MAGICLEVELLIFEDRAIN,
	ITEM_PARSE_MAGICLEVELMANADRAIN,
	ITEM_PARSE_MAGICLEVELDROWN,
	ITEM_PARSE_MAGICLEVELPHYSICAL,
	ITEM_PARSE_MAGICLEVELHEALING,
	ITEM_PARSE_MAGICLEVELUNDEFINED,
	ITEM_PARSE_SUPPRESSDRUNK,
	ITEM_PARSE_SUPPRESSENERGY,
	ITEM_PARSE_SUPPRESSFIRE,
	ITEM_PARSE_SUPPRESSPOISON,
	ITEM_PARSE_SUPPRESSDROWN,
	ITEM_PARSE_SUPPRESSPHYSICAL,
	ITEM_PARSE_SUPPRESSFREEZE,
	ITEM_PARSE_SUPPRESSDAZZLE,
	ITEM_PARSE_SUPPRESSCURSE,
	ITEM_PARSE_FIELD,
	ITEM_PARSE_REPLACEABLE,
	ITEM_PARSE_PARTNERDIRECTION,
	ITEM_PARSE_LEVELDOOR,
	ITEM_PARSE_MALETRANSFORMTO,
	ITEM_PARSE_FEMALETRANSFORMTO,
	ITEM_PARSE_TRANSFORMTO,
	ITEM_PARSE_DESTROYTO,
	ITEM_PARSE_ELEMENTICE,
	ITEM_PARSE_ELEMENTEARTH,
	ITEM_PARSE_ELEMENTFIRE,
	ITEM_PARSE_ELEMENTENERGY,
	ITEM_PARSE_ELEMENTDEATH,
	ITEM_PARSE_ELEMENTHOLY,
	ITEM_PARSE_WALKSTACK,
	ITEM_PARSE_BLOCKING,
	ITEM_PARSE_ALLOWDISTREAD,
	ITEM_PARSE_STOREITEM,
	ITEM_PARSE_WORTH,
	ITEM_PARSE_REFLECTPERCENTALL,
	ITEM_PARSE_REFLECTPERCENTELEMENTS,
	ITEM_PARSE_REFLECTPERCENTMAGIC,
	ITEM_PARSE_REFLECTPERCENTENERGY,
	ITEM_PARSE_REFLECTPERCENTFIRE,
	ITEM_PARSE_REFLECTPERCENTEARTH,
	ITEM_PARSE_REFLECTPERCENTICE,
	ITEM_PARSE_REFLECTPERCENTHOLY,
	ITEM_PARSE_REFLECTPERCENTDEATH,
	ITEM_PARSE_REFLECTPERCENTLIFEDRAIN,
	ITEM_PARSE_REFLECTPERCENTMANADRAIN,
	ITEM_PARSE_REFLECTPERCENTDROWN,
	ITEM_PARSE_REFLECTPERCENTPHYSICAL,
	ITEM_PARSE_REFLECTPERCENTHEALING,
	ITEM_PARSE_REFLECTCHANCEALL,
	ITEM_PARSE_REFLECTCHANCEELEMENTS,
	ITEM_PARSE_REFLECTCHANCEMAGIC,
	ITEM_PARSE_REFLECTCHANCEENERGY,
	ITEM_PARSE_REFLECTCHANCEFIRE,
	ITEM_PARSE_REFLECTCHANCEEARTH,
	ITEM_PARSE_REFLECTCHANCEICE,
	ITEM_PARSE_REFLECTCHANCEHOLY,
	ITEM_PARSE_REFLECTCHANCEDEATH,
	ITEM_PARSE_REFLECTCHANCELIFEDRAIN,
	ITEM_PARSE_REFLECTCHANCEMANADRAIN,
	ITEM_PARSE_REFLECTCHANCEDROWN,
	ITEM_PARSE_REFLECTCHANCEPHYSICAL,
	ITEM_PARSE_REFLECTCHANCEHEALING,
	ITEM_PARSE_BOOSTPERCENTALL,
	ITEM_PARSE_BOOSTPERCENTELEMENTS,
	ITEM_PARSE_BOOSTPERCENTMAGIC,
	ITEM_PARSE_BOOSTPERCENTENERGY,
	ITEM_PARSE_BOOSTPERCENTFIRE,
	ITEM_PARSE_BOOSTPERCENTEARTH,
	ITEM_PARSE_BOOSTPERCENTICE,
	ITEM_PARSE_BOOSTPERCENTHOLY,
	ITEM_PARSE_BOOSTPERCENTDEATH,
	ITEM_PARSE_BOOSTPERCENTLIFEDRAIN,
	ITEM_PARSE_BOOSTPERCENTMANADRAIN,
	ITEM_PARSE_BOOSTPERCENTDROWN,
	ITEM_PARSE_BOOSTPERCENTPHYSICAL,
	ITEM_PARSE_BOOSTPERCENTHEALING,
	ITEM_PARSE_SUPPLY,
};

struct Abilities
{
	uint32_t healthGain = 0;
	uint32_t healthTicks = 0;
	uint32_t manaGain = 0;
	uint32_t manaTicks = 0;

	uint32_t conditionImmunities = 0;
	uint32_t conditionSuppressions = 0;

	// stats modifiers
	std::array<int32_t, STAT_LAST + 1> stats = {0};
	std::array<int32_t, STAT_LAST + 1> statsPercent = {0};

	// extra skill modifiers
	std::array<int32_t, SKILL_LAST + 1> skills = {0};
	std::array<int32_t, SPECIALSKILL_LAST + 1> specialSkills = {0};
	std::array<int16_t, COMBAT_COUNT> specialMagicLevelSkill = {0};
	int32_t speed = 0;

	// field damage abilities modifiers
	std::array<int16_t, COMBAT_COUNT> fieldAbsorbPercent = {0};

	// damage abilities modifiers
	std::array<int16_t, COMBAT_COUNT> absorbPercent = {0};

	std::array<Reflect, COMBAT_COUNT> reflect;

	int16_t boostPercent[COMBAT_COUNT] = {0};

	// elemental damage
	uint16_t elementDamage = 0;
	CombatType_t elementType = COMBAT_NONE;

	bool manaShield = false;
	bool invisible = false;
	bool regeneration = false;
};

class ItemType
{
public:
	ItemType() = default;

	// non-copyable
	ItemType(const ItemType& other) = delete;
	ItemType& operator=(const ItemType& other) = delete;

	ItemType(ItemType&& other) = default;
	ItemType& operator=(ItemType&& other) = default;

	bool isGroundTile() const { return group == ITEM_GROUP_GROUND; }
	bool isContainer() const { return group == ITEM_GROUP_CONTAINER; }
	bool isSplash() const { return group == ITEM_GROUP_SPLASH; }
	bool isFluidContainer() const { return group == ITEM_GROUP_FLUID; }

	bool isDoor() const { return (type == ITEM_TYPE_DOOR); }
	bool isMagicField() const { return (type == ITEM_TYPE_MAGICFIELD); }
	bool isTeleport() const { return (type == ITEM_TYPE_TELEPORT); }
	bool isKey() const { return (type == ITEM_TYPE_KEY); }
	bool isDepot() const { return (type == ITEM_TYPE_DEPOT); }
	bool isMailbox() const { return (type == ITEM_TYPE_MAILBOX); }
	bool isTrashHolder() const { return (type == ITEM_TYPE_TRASHHOLDER); }
	bool isBed() const { return (type == ITEM_TYPE_BED); }
	bool isRune() const { return (type == ITEM_TYPE_RUNE); }
	bool isPodium() const { return (type == ITEM_TYPE_PODIUM); }
	bool isPickupable() const { return (allowPickupable || pickupable); }
	bool isUseable() const { return (useable); }
	bool hasSubType() const { return (isFluidContainer() || isSplash() || stackable || charges != 0); }
	bool isSupply() const { return supply; }

	Abilities& getAbilities()
	{
		if (!abilities) {
			abilities.reset(new Abilities());
		}
		return *abilities;
	}

	std::string getPluralName() const
	{
		if (!pluralName.empty()) {
			return pluralName;
		}

		if (showCount == 0) {
			return name;
		}

		if (name.empty() || name.back() == 's') {
			return name;
		}

		std::string str;
		str.reserve(name.length() + 1);
		str.assign(name);
		str += 's';
		return str;
	}

	itemgroup_t group = ITEM_GROUP_NONE;
	ItemTypes_t type = ITEM_TYPE_NONE;
	uint16_t id = 0;
	uint16_t clientId = 0;
	bool stackable = false;
	bool isAnimation = false;

	std::string name;
	std::string article;
	std::string pluralName;
	std::string description;
	std::string runeSpellName;
	std::string vocationString;

	std::unique_ptr<Abilities> abilities;
	std::shared_ptr<ConditionDamage> conditionDamage;

	uint32_t attackSpeed = 0;
	uint32_t weight = 0;
	uint32_t levelDoor = 0;
	uint32_t decayTimeMin = 0;
	uint32_t decayTimeMax = 0;
	uint32_t wieldInfo = 0;
	uint32_t minReqLevel = 0;
	uint32_t minReqMagicLevel = 0;
	uint32_t charges = 0;
	int32_t maxHitChance = -1;
	int32_t decayTo = -1;
	int32_t attack = 0;
	int32_t defense = 0;
	int32_t extraDefense = 0;
	int32_t armor = 0;
	uint16_t rotateTo = 0;
	int32_t runeMagLevel = 0;
	int32_t runeLevel = 0;
	uint64_t worth = 0;

	CombatType_t combatType = COMBAT_NONE;

	uint16_t transformToOnUse[2] = {0, 0};
	uint16_t transformToFree = 0;
	uint16_t destroyTo = 0;
	uint16_t maxTextLen = 0;
	uint16_t writeOnceItemId = 0;
	uint16_t transformEquipTo = 0;
	uint16_t transformDeEquipTo = 0;
	uint16_t maxItems = 8;
	uint16_t slotPosition = SLOTP_HAND;
	uint16_t speed = 0;
	uint16_t wareId = 0;

	MagicEffectClasses magicEffect = CONST_ME_NONE;
	Direction bedPartnerDir = DIRECTION_NONE;
	WeaponType_t weaponType = WEAPON_NONE;
	Ammo_t ammoType = AMMO_NONE;
	ShootType_t shootType = CONST_ANI_NONE;
	RaceType_t corpseType = RACE_NONE;
	FluidTypes_t fluidSource = FLUID_NONE;

	uint8_t floorChange = 0;
	uint8_t alwaysOnTopOrder = 0;
	uint8_t lightLevel = 0;
	uint8_t lightColor = 0;
	uint8_t shootRange = 1;
	uint8_t classification = 0;
	int8_t hitChance = 0;

	bool storeItem = false;
	bool forceUse = false;
	bool forceSerialize = false;
	bool hasHeight = false;
	bool walkStack = true;
	bool blockSolid = false;
	bool blockPickupable = false;
	bool blockProjectile = false;
	bool blockPathFind = false;
	bool allowPickupable = false;
	bool showDuration = false;
	bool showCharges = false;
	bool showAttributes = false;
	bool replaceable = true;
	bool pickupable = false;
	bool rotatable = false;
	bool useable = false;
	bool moveable = false;
	bool alwaysOnTop = false;
	bool canReadText = false;
	bool canWriteText = false;
	bool isVertical = false;
	bool isHorizontal = false;
	bool isHangable = false;
	bool allowDistRead = false;
	bool lookThrough = false;
	bool stopTime = false;
	bool showCount = true;
	bool supply = false;
	bool showClientCharges = false;
	bool showClientDuration = false;
};

class Items
{
public:
	using NameMap = std::unordered_map<std::string, uint16_t>;
	using InventoryVector = std::vector<uint16_t>;

	using CurrencyMap = std::map<uint64_t, uint16_t, std::greater<uint64_t>>;

	Items();

	// non-copyable
	Items(const Items&) = delete;
	Items& operator=(const Items&) = delete;

	bool reload();
	void clear();

	bool loadFromOtb(const std::string& file);

	const ItemType& operator[](size_t id) const { return getItemType(id); }
	const ItemType& getItemType(size_t id) const;
	ItemType& getItemType(size_t id);
	const ItemType& getItemIdByClientId(uint16_t spriteId) const;

	uint16_t getItemIdByName(const std::string& name);

	uint32_t majorVersion = 0;
	uint32_t minorVersion = 0;
	uint32_t buildNumber = 0;

	bool loadFromXml();
	void parseItemNode(const pugi::xml_node& itemNode, uint16_t id);

	size_t size() const { return items.size(); }

	NameMap nameToItems;
	CurrencyMap currencyItems;

private:
	std::vector<ItemType> items;
	InventoryVector inventory;
	class ClientIdToServerIdMap
	{
	public:
		ClientIdToServerIdMap() { vec.reserve(45000); }

		void emplace(uint16_t clientId, uint16_t serverId)
		{
			if (clientId >= vec.size()) {
				vec.resize(clientId + 1, 0);
			}
			if (vec[clientId] == 0) {
				vec[clientId] = serverId;
			}
		}

		uint16_t getServerId(uint16_t clientId) const
		{
			uint16_t serverId = 0;
			if (clientId < vec.size()) {
				serverId = vec[clientId];
			}
			return serverId;
		}

		void clear() { vec.clear(); }

	private:
		std::vector<uint16_t> vec;
	} clientIdToServerIdMap;
};

#endif // FS_ITEMS_H
