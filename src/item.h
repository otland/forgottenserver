// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_ITEM_H
#define FS_ITEM_H

#include "cylinder.h"
#include "items.h"
#include "luascript.h"
#include "thing.h"

class BedItem;
class Container;
class Door;
class MagicField;
class Mailbox;
class Player;
class Podium;
class Teleport;
class TrashHolder;

enum ITEMPROPERTY
{
	CONST_PROP_BLOCKSOLID = 0,
	CONST_PROP_HASHEIGHT,
	CONST_PROP_BLOCKPROJECTILE,
	CONST_PROP_BLOCKPATH,
	CONST_PROP_ISVERTICAL,
	CONST_PROP_ISHORIZONTAL,
	CONST_PROP_MOVEABLE,
	CONST_PROP_IMMOVABLEBLOCKSOLID,
	CONST_PROP_IMMOVABLEBLOCKPATH,
	CONST_PROP_IMMOVABLENOFIELDBLOCKPATH,
	CONST_PROP_NOFIELDBLOCKPATH,
	CONST_PROP_SUPPORTHANGABLE,
};

enum TradeEvents_t
{
	ON_TRADE_TRANSFER,
	ON_TRADE_CANCEL,
};

enum ItemDecayState_t : uint8_t
{
	DECAYING_FALSE = 0,
	DECAYING_TRUE,
	DECAYING_PENDING,
};

enum AttrTypes_t
{
	// ATTR_DESCRIPTION = 1,
	// ATTR_EXT_FILE = 2,
	ATTR_TILE_FLAGS = 3,
	ATTR_ACTION_ID = 4,
	ATTR_UNIQUE_ID = 5,
	ATTR_TEXT = 6,
	ATTR_DESC = 7,
	ATTR_TELE_DEST = 8,
	ATTR_ITEM = 9,
	ATTR_DEPOT_ID = 10,
	// ATTR_EXT_SPAWN_FILE = 11,
	ATTR_RUNE_CHARGES = 12,
	// ATTR_EXT_HOUSE_FILE = 13,
	ATTR_HOUSEDOORID = 14,
	ATTR_COUNT = 15,
	ATTR_DURATION = 16,
	ATTR_DECAYING_STATE = 17,
	ATTR_WRITTENDATE = 18,
	ATTR_WRITTENBY = 19,
	ATTR_SLEEPERGUID = 20,
	ATTR_SLEEPSTART = 21,
	ATTR_CHARGES = 22,
	ATTR_CONTAINER_ITEMS = 23,
	ATTR_NAME = 24,
	ATTR_ARTICLE = 25,
	ATTR_PLURALNAME = 26,
	ATTR_WEIGHT = 27,
	ATTR_ATTACK = 28,
	ATTR_DEFENSE = 29,
	ATTR_EXTRADEFENSE = 30,
	ATTR_ARMOR = 31,
	ATTR_HITCHANCE = 32,
	ATTR_SHOOTRANGE = 33,
	ATTR_CUSTOM_ATTRIBUTES = 34,
	ATTR_DECAYTO = 35,
	ATTR_WRAPID = 36,
	ATTR_STOREITEM = 37,
	ATTR_ATTACK_SPEED = 38,
	ATTR_OPENCONTAINER = 39,
	ATTR_PODIUMOUTFIT = 40,
	// ATTR_TIER = 41, // mapeditor
	ATTR_REFLECT = 42,
	ATTR_BOOST = 43,
};

enum Attr_ReadValue
{
	ATTR_READ_CONTINUE,
	ATTR_READ_ERROR,
	ATTR_READ_END,
};

class ItemAttributes
{
public:
	ItemAttributes() = default;

	void setSpecialDescription(const std::string& desc) { setStrAttr(ITEM_ATTRIBUTE_DESCRIPTION, desc); }
	const std::string& getSpecialDescription() const { return getStrAttr(ITEM_ATTRIBUTE_DESCRIPTION); }

	void setText(const std::string& text) { setStrAttr(ITEM_ATTRIBUTE_TEXT, text); }
	void resetText() { removeAttribute(ITEM_ATTRIBUTE_TEXT); }
	const std::string& getText() const { return getStrAttr(ITEM_ATTRIBUTE_TEXT); }

	void setDate(int32_t n) { setIntAttr(ITEM_ATTRIBUTE_DATE, n); }
	void resetDate() { removeAttribute(ITEM_ATTRIBUTE_DATE); }
	time_t getDate() const { return static_cast<time_t>(getIntAttr(ITEM_ATTRIBUTE_DATE)); }

	void setWriter(const std::string& writer) { setStrAttr(ITEM_ATTRIBUTE_WRITER, writer); }
	void resetWriter() { removeAttribute(ITEM_ATTRIBUTE_WRITER); }
	const std::string& getWriter() const { return getStrAttr(ITEM_ATTRIBUTE_WRITER); }

	void setActionId(uint16_t n) { setIntAttr(ITEM_ATTRIBUTE_ACTIONID, n); }
	uint16_t getActionId() const { return static_cast<uint16_t>(getIntAttr(ITEM_ATTRIBUTE_ACTIONID)); }

	void setUniqueId(uint16_t n) { setIntAttr(ITEM_ATTRIBUTE_UNIQUEID, n); }
	uint16_t getUniqueId() const { return static_cast<uint16_t>(getIntAttr(ITEM_ATTRIBUTE_UNIQUEID)); }

	void setCharges(uint16_t n) { setIntAttr(ITEM_ATTRIBUTE_CHARGES, n); }
	uint16_t getCharges() const { return static_cast<uint16_t>(getIntAttr(ITEM_ATTRIBUTE_CHARGES)); }

	void setFluidType(uint16_t n) { setIntAttr(ITEM_ATTRIBUTE_FLUIDTYPE, n); }
	uint16_t getFluidType() const { return static_cast<uint16_t>(getIntAttr(ITEM_ATTRIBUTE_FLUIDTYPE)); }

	void setOwner(uint32_t owner) { setIntAttr(ITEM_ATTRIBUTE_OWNER, owner); }
	uint32_t getOwner() const { return getIntAttr(ITEM_ATTRIBUTE_OWNER); }

	void setCorpseOwner(uint32_t corpseOwner) { setIntAttr(ITEM_ATTRIBUTE_CORPSEOWNER, corpseOwner); }
	uint32_t getCorpseOwner() const { return getIntAttr(ITEM_ATTRIBUTE_CORPSEOWNER); }

	void setDuration(int32_t time) { setIntAttr(ITEM_ATTRIBUTE_DURATION, time); }
	void decreaseDuration(int32_t time) { increaseIntAttr(ITEM_ATTRIBUTE_DURATION, -time); }
	uint32_t getDuration() const { return getIntAttr(ITEM_ATTRIBUTE_DURATION); }

	void setDecaying(ItemDecayState_t decayState) { setIntAttr(ITEM_ATTRIBUTE_DECAYSTATE, decayState); }
	ItemDecayState_t getDecaying() const
	{
		return static_cast<ItemDecayState_t>(getIntAttr(ITEM_ATTRIBUTE_DECAYSTATE));
	}

	struct CustomAttribute
	{
		typedef boost::variant<boost::blank, std::string, int64_t, double, bool> VariantAttribute;
		VariantAttribute value;

		CustomAttribute() : value(boost::blank()) {}

		bool operator==(const CustomAttribute& otherAttr) const { return value == otherAttr.value; }

		bool operator!=(const CustomAttribute& otherAttr) const { return value != otherAttr.value; }

		template <typename T>
		explicit CustomAttribute(const T& v) : value(v)
		{}

		template <typename T>
		void set(const T& v)
		{
			value = v;
		}

		template <typename T>
		const T& get();

		struct PushLuaVisitor : public boost::static_visitor<>
		{
			lua_State* L;

			explicit PushLuaVisitor(lua_State* L) : boost::static_visitor<>(), L(L) {}

			void operator()(const boost::blank&) const { lua_pushnil(L); }

			void operator()(std::string_view v) const { tfs::lua::pushString(L, v); }

			void operator()(bool v) const { tfs::lua::pushBoolean(L, v); }

			void operator()(const int64_t& v) const { lua_pushnumber(L, v); }

			void operator()(const double& v) const { lua_pushnumber(L, v); }
		};

		void pushToLua(lua_State* L) const { boost::apply_visitor(PushLuaVisitor(L), value); }

		struct SerializeVisitor : public boost::static_visitor<>
		{
			PropWriteStream& propWriteStream;

			explicit SerializeVisitor(PropWriteStream& propWriteStream) :
			    boost::static_visitor<>(), propWriteStream(propWriteStream)
			{}

			void operator()(const boost::blank&) const {}

			void operator()(const std::string& v) const { propWriteStream.writeString(v); }

			template <typename T>
			void operator()(const T& v) const
			{
				propWriteStream.write<T>(v);
			}
		};

		void serialize(PropWriteStream& propWriteStream) const
		{
			propWriteStream.write<uint8_t>(static_cast<uint8_t>(value.which()));
			boost::apply_visitor(SerializeVisitor(propWriteStream), value);
		}

		bool unserialize(PropStream& propStream)
		{
			// This is hard-coded so it's not general, depends on the position of the variants.
			uint8_t pos;
			if (!propStream.read<uint8_t>(pos)) {
				return false;
			}

			switch (pos) {
				case 1: { // std::string
					auto [str, ok] = propStream.readString();
					if (!ok) {
						return false;
					}
					value = std::string{str};
					break;
				}

				case 2: { // int64_t
					int64_t tmp;
					if (!propStream.read<int64_t>(tmp)) {
						return false;
					}
					value = tmp;
					break;
				}

				case 3: { // double
					double tmp;
					if (!propStream.read<double>(tmp)) {
						return false;
					}
					value = tmp;
					break;
				}

				case 4: { // bool
					bool tmp;
					if (!propStream.read<bool>(tmp)) {
						return false;
					}
					value = tmp;
					break;
				}

				default: {
					value = boost::blank();
					return false;
				}
			}
			return true;
		}
	};

private:
	bool hasAttribute(itemAttrTypes type) const { return (type & attributeBits) != 0; }
	void removeAttribute(itemAttrTypes type);

	static std::string emptyString;
	static int64_t emptyInt;
	static double emptyDouble;
	static bool emptyBool;
	static Reflect emptyReflect;

	typedef std::unordered_map<std::string, CustomAttribute> CustomAttributeMap;

	struct Attribute
	{
		union
		{
			int64_t integer;
			std::string* string;
			CustomAttributeMap* custom;
		} value;
		itemAttrTypes type;

		explicit Attribute(itemAttrTypes type) : type(type) { memset(&value, 0, sizeof(value)); }
		Attribute(const Attribute& i)
		{
			type = i.type;
			if (ItemAttributes::isIntAttrType(type)) {
				value.integer = i.value.integer;
			} else if (ItemAttributes::isStrAttrType(type)) {
				value.string = new std::string(*i.value.string);
			} else if (ItemAttributes::isCustomAttrType(type)) {
				value.custom = new CustomAttributeMap(*i.value.custom);
			} else {
				memset(&value, 0, sizeof(value));
			}
		}
		Attribute(Attribute&& attribute) : value(attribute.value), type(attribute.type)
		{
			memset(&attribute.value, 0, sizeof(value));
			attribute.type = ITEM_ATTRIBUTE_NONE;
		}
		~Attribute()
		{
			if (ItemAttributes::isStrAttrType(type)) {
				delete value.string;
			} else if (ItemAttributes::isCustomAttrType(type)) {
				delete value.custom;
			}
		}
		Attribute& operator=(Attribute other)
		{
			Attribute::swap(*this, other);
			return *this;
		}
		Attribute& operator=(Attribute&& other)
		{
			if (this != &other) {
				if (ItemAttributes::isStrAttrType(type)) {
					delete value.string;
				} else if (ItemAttributes::isCustomAttrType(type)) {
					delete value.custom;
				}

				value = other.value;
				type = other.type;

				memset(&other.value, 0, sizeof(value));
				other.type = ITEM_ATTRIBUTE_NONE;
			}
			return *this;
		}

		static void swap(Attribute& first, Attribute& second)
		{
			std::swap(first.value, second.value);
			std::swap(first.type, second.type);
		}
	};

	std::vector<Attribute> attributes;
	uint32_t attributeBits = 0;

	std::map<CombatType_t, Reflect> reflect;
	std::map<CombatType_t, uint16_t> boostPercent;

	const Reflect& getReflect(CombatType_t combatType)
	{
		auto it = reflect.find(combatType);
		return it != reflect.end() ? it->second : emptyReflect;
	}
	int16_t getBoostPercent(CombatType_t combatType)
	{
		auto it = boostPercent.find(combatType);
		return it != boostPercent.end() ? it->second : 0;
	}

	const std::string& getStrAttr(itemAttrTypes type) const;
	void setStrAttr(itemAttrTypes type, std::string_view value);

	int64_t getIntAttr(itemAttrTypes type) const;
	void setIntAttr(itemAttrTypes type, int64_t value);
	void increaseIntAttr(itemAttrTypes type, int64_t value);

	const Attribute* getExistingAttr(itemAttrTypes type) const;
	Attribute& getAttr(itemAttrTypes type);

	CustomAttributeMap* getCustomAttributeMap()
	{
		if (!hasAttribute(ITEM_ATTRIBUTE_CUSTOM)) {
			return nullptr;
		}

		return getAttr(ITEM_ATTRIBUTE_CUSTOM).value.custom;
	}

	template <typename R>
	void setCustomAttribute(int64_t key, R value)
	{
		auto tmp = std::to_string(key);
		setCustomAttribute(tmp, value);
	}

	void setCustomAttribute(int64_t key, CustomAttribute& value)
	{
		auto tmp = std::to_string(key);
		setCustomAttribute(tmp, value);
	}

	template <typename R>
	void setCustomAttribute(std::string_view key, R value)
	{
		if (hasAttribute(ITEM_ATTRIBUTE_CUSTOM)) {
			removeCustomAttribute(key);
		} else {
			getAttr(ITEM_ATTRIBUTE_CUSTOM).value.custom = new CustomAttributeMap();
		}
		auto lowercaseKey = boost::algorithm::to_lower_copy(std::string{key});
		getAttr(ITEM_ATTRIBUTE_CUSTOM).value.custom->emplace(lowercaseKey, value);
	}

	void setCustomAttribute(std::string_view key, const CustomAttribute& value)
	{
		if (hasAttribute(ITEM_ATTRIBUTE_CUSTOM)) {
			removeCustomAttribute(key);
		} else {
			getAttr(ITEM_ATTRIBUTE_CUSTOM).value.custom = new CustomAttributeMap();
		}
		auto lowercaseKey = boost::algorithm::to_lower_copy(std::string{key});
		getAttr(ITEM_ATTRIBUTE_CUSTOM).value.custom->emplace(lowercaseKey, value);
	}

	const CustomAttribute* getCustomAttribute(int64_t key)
	{
		auto tmp = std::to_string(key);
		return getCustomAttribute(tmp);
	}

	const CustomAttribute* getCustomAttribute(std::string_view key)
	{
		if (const CustomAttributeMap* customAttrMap = getCustomAttributeMap()) {
			auto lowercaseKey = boost::algorithm::to_lower_copy(std::string{key});
			if (auto it = customAttrMap->find(lowercaseKey); it != customAttrMap->end()) {
				return &(it->second);
			}
		}
		return nullptr;
	}

	bool removeCustomAttribute(int64_t key)
	{
		auto tmp = std::to_string(key);
		return removeCustomAttribute(tmp);
	}

	bool removeCustomAttribute(std::string_view key)
	{
		if (CustomAttributeMap* customAttrMap = getCustomAttributeMap()) {
			auto lowercaseKey = boost::algorithm::to_lower_copy(std::string{key});
			if (auto it = customAttrMap->find(lowercaseKey); it != customAttrMap->end()) {
				customAttrMap->erase(it);
				return true;
			}
		}
		return false;
	}

	const static uint32_t intAttributeTypes =
	    ITEM_ATTRIBUTE_ACTIONID | ITEM_ATTRIBUTE_UNIQUEID | ITEM_ATTRIBUTE_DATE | ITEM_ATTRIBUTE_WEIGHT |
	    ITEM_ATTRIBUTE_ATTACK | ITEM_ATTRIBUTE_DEFENSE | ITEM_ATTRIBUTE_EXTRADEFENSE | ITEM_ATTRIBUTE_ARMOR |
	    ITEM_ATTRIBUTE_HITCHANCE | ITEM_ATTRIBUTE_SHOOTRANGE | ITEM_ATTRIBUTE_OWNER | ITEM_ATTRIBUTE_DURATION |
	    ITEM_ATTRIBUTE_DECAYSTATE | ITEM_ATTRIBUTE_CORPSEOWNER | ITEM_ATTRIBUTE_CHARGES | ITEM_ATTRIBUTE_FLUIDTYPE |
	    ITEM_ATTRIBUTE_DOORID | ITEM_ATTRIBUTE_DECAYTO | ITEM_ATTRIBUTE_WRAPID | ITEM_ATTRIBUTE_STOREITEM |
	    ITEM_ATTRIBUTE_ATTACK_SPEED | ITEM_ATTRIBUTE_OPENCONTAINER;
	const static uint32_t stringAttributeTypes = ITEM_ATTRIBUTE_DESCRIPTION | ITEM_ATTRIBUTE_TEXT |
	                                             ITEM_ATTRIBUTE_WRITER | ITEM_ATTRIBUTE_NAME | ITEM_ATTRIBUTE_ARTICLE |
	                                             ITEM_ATTRIBUTE_PLURALNAME;

public:
	static bool isIntAttrType(itemAttrTypes type) { return (type & intAttributeTypes) == type; }
	static bool isStrAttrType(itemAttrTypes type) { return (type & stringAttributeTypes) == type; }
	inline static bool isCustomAttrType(itemAttrTypes type) { return (type & ITEM_ATTRIBUTE_CUSTOM) == type; }

	const std::vector<Attribute>& getList() const { return attributes; }

	friend class Item;
};

class Item : virtual public Thing
{
public:
	// Factory member to create item of right type based on type
	static Item* CreateItem(const uint16_t type, uint16_t count = 0);
	static Container* CreateItemAsContainer(const uint16_t type, uint16_t size);
	static Item* CreateItem(PropStream& propStream);
	static Items items;

	// Constructor for items
	Item(const uint16_t type, uint16_t count = 0);
	Item(const Item& i);
	virtual Item* clone() const;

	virtual ~Item() = default;

	// non-assignable
	Item& operator=(const Item&) = delete;

	bool equals(const Item* otherItem) const;

	Item* getItem() override final { return this; }
	const Item* getItem() const override final { return this; }
	virtual Teleport* getTeleport() { return nullptr; }
	virtual const Teleport* getTeleport() const { return nullptr; }
	virtual TrashHolder* getTrashHolder() { return nullptr; }
	virtual const TrashHolder* getTrashHolder() const { return nullptr; }
	virtual Mailbox* getMailbox() { return nullptr; }
	virtual const Mailbox* getMailbox() const { return nullptr; }
	virtual Door* getDoor() { return nullptr; }
	virtual const Door* getDoor() const { return nullptr; }
	virtual MagicField* getMagicField() { return nullptr; }
	virtual const MagicField* getMagicField() const { return nullptr; }
	virtual BedItem* getBed() { return nullptr; }
	virtual const BedItem* getBed() const { return nullptr; }
	virtual Podium* getPodium() { return nullptr; }
	virtual const Podium* getPodium() const { return nullptr; }

	const std::string& getStrAttr(itemAttrTypes type) const
	{
		if (!attributes) {
			return ItemAttributes::emptyString;
		}
		return attributes->getStrAttr(type);
	}
	void setStrAttr(itemAttrTypes type, std::string_view value) { getAttributes()->setStrAttr(type, value); }

	int64_t getIntAttr(itemAttrTypes type) const
	{
		if (!attributes) {
			return 0;
		}
		return attributes->getIntAttr(type);
	}
	void setIntAttr(itemAttrTypes type, int64_t value) { getAttributes()->setIntAttr(type, value); }
	void increaseIntAttr(itemAttrTypes type, int64_t value) { getAttributes()->increaseIntAttr(type, value); }

	void removeAttribute(itemAttrTypes type)
	{
		if (attributes) {
			attributes->removeAttribute(type);
		}
	}
	bool hasAttribute(itemAttrTypes type) const
	{
		if (!attributes) {
			return false;
		}
		return attributes->hasAttribute(type);
	}

	template <typename R>
	void setCustomAttribute(std::string_view key, R value)
	{
		getAttributes()->setCustomAttribute(key, value);
	}

	void setCustomAttribute(std::string_view key, ItemAttributes::CustomAttribute& value)
	{
		getAttributes()->setCustomAttribute(key, value);
	}

	const ItemAttributes::CustomAttribute* getCustomAttribute(int64_t key)
	{
		if (!attributes) {
			return nullptr;
		}
		return getAttributes()->getCustomAttribute(key);
	}

	const ItemAttributes::CustomAttribute* getCustomAttribute(const std::string& key)
	{
		if (!attributes) {
			return nullptr;
		}
		return getAttributes()->getCustomAttribute(key);
	}

	bool removeCustomAttribute(int64_t key)
	{
		if (!attributes) {
			return false;
		}
		return getAttributes()->removeCustomAttribute(key);
	}

	bool removeCustomAttribute(std::string_view key)
	{
		if (!attributes) {
			return false;
		}
		return getAttributes()->removeCustomAttribute(key);
	}

	void setSpecialDescription(std::string_view desc) { setStrAttr(ITEM_ATTRIBUTE_DESCRIPTION, desc); }
	const std::string& getSpecialDescription() const { return getStrAttr(ITEM_ATTRIBUTE_DESCRIPTION); }

	void setText(std::string_view text) { setStrAttr(ITEM_ATTRIBUTE_TEXT, text); }
	void resetText() { removeAttribute(ITEM_ATTRIBUTE_TEXT); }
	const std::string& getText() const { return getStrAttr(ITEM_ATTRIBUTE_TEXT); }

	void setDate(int32_t n) { setIntAttr(ITEM_ATTRIBUTE_DATE, n); }
	void resetDate() { removeAttribute(ITEM_ATTRIBUTE_DATE); }
	time_t getDate() const { return static_cast<time_t>(getIntAttr(ITEM_ATTRIBUTE_DATE)); }

	void setWriter(std::string_view writer) { setStrAttr(ITEM_ATTRIBUTE_WRITER, writer); }
	void resetWriter() { removeAttribute(ITEM_ATTRIBUTE_WRITER); }
	const std::string& getWriter() const { return getStrAttr(ITEM_ATTRIBUTE_WRITER); }

	void setActionId(uint16_t n)
	{
		if (n < 100) {
			n = 100;
		}

		setIntAttr(ITEM_ATTRIBUTE_ACTIONID, n);
	}
	uint16_t getActionId() const
	{
		if (!attributes) {
			return 0;
		}
		return static_cast<uint16_t>(getIntAttr(ITEM_ATTRIBUTE_ACTIONID));
	}

	uint16_t getUniqueId() const
	{
		if (!attributes) {
			return 0;
		}
		return static_cast<uint16_t>(getIntAttr(ITEM_ATTRIBUTE_UNIQUEID));
	}

	void setCharges(uint16_t n) { setIntAttr(ITEM_ATTRIBUTE_CHARGES, n); }
	uint16_t getCharges() const
	{
		if (!attributes) {
			return 0;
		}
		return static_cast<uint16_t>(getIntAttr(ITEM_ATTRIBUTE_CHARGES));
	}

	void setFluidType(uint16_t n) { setIntAttr(ITEM_ATTRIBUTE_FLUIDTYPE, n); }
	uint16_t getFluidType() const
	{
		if (!attributes) {
			return 0;
		}
		return static_cast<uint16_t>(getIntAttr(ITEM_ATTRIBUTE_FLUIDTYPE));
	}

	void setOwner(uint32_t owner) { setIntAttr(ITEM_ATTRIBUTE_OWNER, owner); }
	uint32_t getOwner() const
	{
		if (!attributes) {
			return 0;
		}
		return getIntAttr(ITEM_ATTRIBUTE_OWNER);
	}

	void setCorpseOwner(uint32_t corpseOwner) { setIntAttr(ITEM_ATTRIBUTE_CORPSEOWNER, corpseOwner); }
	uint32_t getCorpseOwner() const
	{
		if (!attributes) {
			return 0;
		}
		return getIntAttr(ITEM_ATTRIBUTE_CORPSEOWNER);
	}

	void setDuration(int32_t time) { setIntAttr(ITEM_ATTRIBUTE_DURATION, time); }
	void decreaseDuration(int32_t time) { increaseIntAttr(ITEM_ATTRIBUTE_DURATION, -time); }
	uint32_t getDuration() const
	{
		if (!attributes) {
			return 0;
		}
		return getIntAttr(ITEM_ATTRIBUTE_DURATION);
	}

	void setDecaying(ItemDecayState_t decayState) { setIntAttr(ITEM_ATTRIBUTE_DECAYSTATE, decayState); }
	ItemDecayState_t getDecaying() const
	{
		if (!attributes) {
			return DECAYING_FALSE;
		}
		return static_cast<ItemDecayState_t>(getIntAttr(ITEM_ATTRIBUTE_DECAYSTATE));
	}

	int32_t getDecayTimeMin() const
	{
		if (hasAttribute(ITEM_ATTRIBUTE_DURATION_MIN)) {
			return getIntAttr(ITEM_ATTRIBUTE_DURATION_MIN);
		}
		return items[id].decayTimeMin;
	}
	int32_t getDecayTimeMax() const
	{
		if (hasAttribute(ITEM_ATTRIBUTE_DURATION_MAX)) {
			return getIntAttr(ITEM_ATTRIBUTE_DURATION_MAX);
		}
		return items[id].decayTimeMax;
	}

	void setDecayTo(int32_t decayTo) { setIntAttr(ITEM_ATTRIBUTE_DECAYTO, decayTo); }
	int32_t getDecayTo() const
	{
		if (hasAttribute(ITEM_ATTRIBUTE_DECAYTO)) {
			return getIntAttr(ITEM_ATTRIBUTE_DECAYTO);
		}
		return items[id].decayTo;
	}

	static std::string getNameDescription(const ItemType& it, const Item* item = nullptr, int32_t subType = -1,
	                                      bool addArticle = true);
	static std::string getWeightDescription(const ItemType& it, uint32_t weight, uint32_t count = 1);

	std::string getDescription(int32_t lookDistance) const override final;
	std::string getNameDescription() const;
	std::string getWeightDescription() const;

	// serialization
	virtual Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream);
	bool unserializeAttr(PropStream& propStream);
	virtual bool unserializeItemNode(OTB::Loader&, const OTB::Node&, PropStream& propStream);

	virtual void serializeAttr(PropWriteStream& propWriteStream) const;

	bool isPushable() const override final { return isMoveable(); }
	int32_t getThrowRange() const override final { return (isPickupable() ? 15 : 2); }

	uint16_t getID() const { return id; }
	uint16_t getClientID() const { return items[id].clientId; }
	void setID(uint16_t newid);

	// Returns the player that is holding this item in his inventory
	const Player* getHoldingPlayer() const;

	WeaponType_t getWeaponType() const { return items[id].weaponType; }
	Ammo_t getAmmoType() const { return items[id].ammoType; }
	uint8_t getShootRange() const
	{
		if (hasAttribute(ITEM_ATTRIBUTE_SHOOTRANGE)) {
			return getIntAttr(ITEM_ATTRIBUTE_SHOOTRANGE);
		}
		return items[id].shootRange;
	}

	virtual uint32_t getWeight() const;
	uint32_t getBaseWeight() const
	{
		if (hasAttribute(ITEM_ATTRIBUTE_WEIGHT)) {
			return getIntAttr(ITEM_ATTRIBUTE_WEIGHT);
		}
		return items[id].weight;
	}
	int32_t getAttack() const
	{
		if (hasAttribute(ITEM_ATTRIBUTE_ATTACK)) {
			return getIntAttr(ITEM_ATTRIBUTE_ATTACK);
		}
		return items[id].attack;
	}
	uint32_t getAttackSpeed() const
	{
		if (hasAttribute(ITEM_ATTRIBUTE_ATTACK_SPEED)) {
			return getIntAttr(ITEM_ATTRIBUTE_ATTACK_SPEED);
		}
		return items[id].attackSpeed;
	}
	int32_t getArmor() const
	{
		if (hasAttribute(ITEM_ATTRIBUTE_ARMOR)) {
			return getIntAttr(ITEM_ATTRIBUTE_ARMOR);
		}
		return items[id].armor;
	}
	int32_t getDefense() const
	{
		if (hasAttribute(ITEM_ATTRIBUTE_DEFENSE)) {
			return getIntAttr(ITEM_ATTRIBUTE_DEFENSE);
		}
		return items[id].defense;
	}
	int32_t getExtraDefense() const
	{
		if (hasAttribute(ITEM_ATTRIBUTE_EXTRADEFENSE)) {
			return getIntAttr(ITEM_ATTRIBUTE_EXTRADEFENSE);
		}
		return items[id].extraDefense;
	}
	int32_t getSlotPosition() const { return items[id].slotPosition; }
	int8_t getHitChance() const
	{
		if (hasAttribute(ITEM_ATTRIBUTE_HITCHANCE)) {
			return getIntAttr(ITEM_ATTRIBUTE_HITCHANCE);
		}
		return items[id].hitChance;
	}

	uint32_t getWorth() const;
	LightInfo getLightInfo() const;

	void setReflect(CombatType_t combatType, const Reflect& reflect) { getAttributes()->reflect[combatType] = reflect; }
	Reflect getReflect(CombatType_t combatType, bool total = true) const;

	void setBoostPercent(CombatType_t combatType, uint16_t value) { getAttributes()->boostPercent[combatType] = value; }
	uint16_t getBoostPercent(CombatType_t combatType, bool total = true) const;

	bool hasProperty(ITEMPROPERTY prop) const;
	bool isBlocking() const { return items[id].blockSolid; }
	bool isStackable() const { return items[id].stackable; }
	bool isAlwaysOnTop() const { return items[id].alwaysOnTop; }
	bool isGroundTile() const { return items[id].isGroundTile(); }
	bool isMagicField() const { return items[id].isMagicField(); }
	bool isMoveable() const { return items[id].moveable; }
	bool isPickupable() const { return items[id].pickupable; }
	bool isUseable() const { return items[id].useable; }
	bool isHangable() const { return items[id].isHangable; }
	bool isRotatable() const
	{
		const ItemType& it = items[id];
		return it.rotatable && it.rotateTo;
	}
	bool isPodium() const { return items[id].isPodium(); }
	bool hasWalkStack() const { return items[id].walkStack; }
	bool isSupply() const { return items[id].isSupply(); }

	void setStoreItem(bool storeItem) { setIntAttr(ITEM_ATTRIBUTE_STOREITEM, static_cast<int64_t>(storeItem)); }
	bool isStoreItem() const
	{
		if (hasAttribute(ITEM_ATTRIBUTE_STOREITEM)) {
			return getIntAttr(ITEM_ATTRIBUTE_STOREITEM) == 1;
		}
		return items[id].storeItem;
	}
	const std::string& getName() const
	{
		if (hasAttribute(ITEM_ATTRIBUTE_NAME)) {
			return getStrAttr(ITEM_ATTRIBUTE_NAME);
		}
		return items[id].name;
	}
	const std::string getPluralName() const
	{
		if (hasAttribute(ITEM_ATTRIBUTE_PLURALNAME)) {
			return getStrAttr(ITEM_ATTRIBUTE_PLURALNAME);
		}
		return items[id].getPluralName();
	}
	const std::string& getArticle() const
	{
		if (hasAttribute(ITEM_ATTRIBUTE_ARTICLE)) {
			return getStrAttr(ITEM_ATTRIBUTE_ARTICLE);
		}
		return items[id].article;
	}

	// get the number of items
	uint16_t getItemCount() const { return count; }
	void setItemCount(uint8_t n) { count = n; }

	static uint32_t countByType(const Item* i, int32_t subType)
	{
		if (subType == -1 || subType == i->getSubType()) {
			return i->getItemCount();
		}

		return 0;
	}

	void setDefaultSubtype();
	uint16_t getSubType() const;
	void setSubType(uint16_t n);

	void setUniqueId(uint16_t n);

	void setDefaultDuration();
	uint32_t getDefaultDurationMin() const { return items[id].decayTimeMin * 1000; }
	uint32_t getDefaultDurationMax() const { return items[id].decayTimeMax * 1000; }
	bool canDecay() const;

	virtual bool canRemove() const { return true; }
	virtual bool canTransform() const { return true; }
	virtual void onRemoved();
	virtual void onTradeEvent(TradeEvents_t, Player*) {}

	virtual void startDecaying();

	bool isLoadedFromMap() const { return loadedFromMap; }
	void setLoadedFromMap(bool value) { loadedFromMap = value; }
	bool isCleanable() const
	{
		return !loadedFromMap && canRemove() && isPickupable() && !hasAttribute(ITEM_ATTRIBUTE_UNIQUEID) &&
		       !hasAttribute(ITEM_ATTRIBUTE_ACTIONID);
	}

	bool hasMarketAttributes() const;

	std::unique_ptr<ItemAttributes>& getAttributes()
	{
		if (!attributes) {
			attributes.reset(new ItemAttributes());
		}
		return attributes;
	}

	void incrementReferenceCounter() { ++referenceCounter; }
	void decrementReferenceCounter()
	{
		if (--referenceCounter == 0) {
			delete this;
		}
	}

	bool hasParent() const override { return getParent(); }
	Cylinder* getParent() const override { return parent; }
	void setParent(Cylinder* cylinder) override { parent = cylinder; }
	Cylinder* getTopParent();
	const Cylinder* getTopParent() const;
	Tile* getTile() override;
	const Tile* getTile() const override;
	bool isRemoved() const override { return !parent || parent->isRemoved(); }

protected:
	Cylinder* parent = nullptr;

	uint16_t id; // the same id as in ItemType

private:
	std::string getWeightDescription(uint32_t weight) const;

	std::unique_ptr<ItemAttributes> attributes;

	uint32_t referenceCounter = 0;

	uint8_t count = 1; // number of stacked items

	bool loadedFromMap = false;

	// Don't add variables here, use the ItemAttribute class.
};

using ItemList = std::list<Item*>;
using ItemDeque = std::deque<Item*>;
using StashContainerList = std::vector<std::pair<Item*, uint32_t>>;

#endif // FS_ITEM_H
