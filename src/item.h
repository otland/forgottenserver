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

#ifndef FS_ITEM_H_009A319FB13D477D9EEFFBBD9BB83562
#define FS_ITEM_H_009A319FB13D477D9EEFFBBD9BB83562

#include "cylinder.h"
#include "thing.h"
#include "items.h"

#include <deque>

class Creature;
class Player;
class Container;
class Depot;
class Teleport;
class TrashHolder;
class Mailbox;
class Door;
class MagicField;
class BedItem;

enum ITEMPROPERTY {
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
	CONST_PROP_SUPPORTHANGABLE
};

enum TradeEvents_t {
	ON_TRADE_TRANSFER,
	ON_TRADE_CANCEL,
};

enum ItemDecayState_t {
	DECAYING_FALSE = 0,
	DECAYING_TRUE,
	DECAYING_PENDING
};

enum AttrTypes_t {
	//ATTR_DESCRIPTION = 1,
	//ATTR_EXT_FILE = 2,
	ATTR_TILE_FLAGS = 3,
	ATTR_ACTION_ID = 4,
	ATTR_UNIQUE_ID = 5,
	ATTR_TEXT = 6,
	ATTR_DESC = 7,
	ATTR_TELE_DEST = 8,
	ATTR_ITEM = 9,
	ATTR_DEPOT_ID = 10,
	//ATTR_EXT_SPAWN_FILE = 11,
	ATTR_RUNE_CHARGES = 12,
	//ATTR_EXT_HOUSE_FILE = 13,
	ATTR_HOUSEDOORID = 14,
	ATTR_COUNT = 15,
	ATTR_DURATION = 16,
	ATTR_DECAYING_STATE = 17,
	ATTR_WRITTENDATE = 18,
	ATTR_WRITTENBY = 19,
	ATTR_SLEEPERGUID = 20,
	ATTR_SLEEPSTART = 21,
	ATTR_CHARGES = 22,
	ATTR_CONTAINER_ITEMS = 23
};

enum Attr_ReadValue {
	ATTR_READ_CONTINUE,
	ATTR_READ_ERROR,
	ATTR_READ_END
};

class ItemAttributes
{
	public:
		ItemAttributes() : attributeBits(0) {}

		void setSpecialDescription(const std::string& desc) {
			setStrAttr(ITEM_ATTRIBUTE_DESCRIPTION, desc);
		}
		const std::string& getSpecialDescription() const {
			return getStrAttr(ITEM_ATTRIBUTE_DESCRIPTION);
		}

		void setText(const std::string& text) {
			setStrAttr(ITEM_ATTRIBUTE_TEXT, text);
		}
		void resetText() {
			removeAttribute(ITEM_ATTRIBUTE_TEXT);
		}
		const std::string& getText() const {
			return getStrAttr(ITEM_ATTRIBUTE_TEXT);
		}

		void setDate(int32_t n) {
			setIntAttr(ITEM_ATTRIBUTE_DATE, n);
		}
		void resetDate() {
			removeAttribute(ITEM_ATTRIBUTE_DATE);
		}
		time_t getDate() const {
			return (time_t)getIntAttr(ITEM_ATTRIBUTE_DATE);
		}

		void setWriter(const std::string& _writer) {
			setStrAttr(ITEM_ATTRIBUTE_WRITER, _writer);
		}
		void resetWriter() {
			removeAttribute(ITEM_ATTRIBUTE_WRITER);
		}
		const std::string& getWriter() const {
			return getStrAttr(ITEM_ATTRIBUTE_WRITER);
		}

		void setActionId(uint16_t n) {
			if (n < 100) {
				n = 100;
			}

			setIntAttr(ITEM_ATTRIBUTE_ACTIONID, n);
		}
		uint16_t getActionId() const {
			return (uint16_t)getIntAttr(ITEM_ATTRIBUTE_ACTIONID);
		}

		void setUniqueId(uint16_t n) {
			if (n < 1000) {
				n = 1000;
			}

			setIntAttr(ITEM_ATTRIBUTE_UNIQUEID, n);
		}
		uint16_t getUniqueId() const {
			return (uint16_t)getIntAttr(ITEM_ATTRIBUTE_UNIQUEID);
		}

		void setCharges(uint16_t n) {
			setIntAttr(ITEM_ATTRIBUTE_CHARGES, n);
		}
		uint16_t getCharges() const {
			return (uint16_t)getIntAttr(ITEM_ATTRIBUTE_CHARGES);
		}

		void setFluidType(uint16_t n) {
			setIntAttr(ITEM_ATTRIBUTE_FLUIDTYPE, n);
		}
		uint16_t getFluidType() const {
			return (uint16_t)getIntAttr(ITEM_ATTRIBUTE_FLUIDTYPE);
		}

		void setOwner(uint32_t _owner) {
			setIntAttr(ITEM_ATTRIBUTE_OWNER, _owner);
		}
		uint32_t getOwner() const {
			return getIntAttr(ITEM_ATTRIBUTE_OWNER);
		}

		void setCorpseOwner(uint32_t _corpseOwner) {
			setIntAttr(ITEM_ATTRIBUTE_CORPSEOWNER, _corpseOwner);
		}
		uint32_t getCorpseOwner() const {
			return getIntAttr(ITEM_ATTRIBUTE_CORPSEOWNER);
		}

		void setDuration(int32_t time) {
			setIntAttr(ITEM_ATTRIBUTE_DURATION, time);
		}
		void decreaseDuration(int32_t time) {
			increaseIntAttr(ITEM_ATTRIBUTE_DURATION, -time);
		}
		uint32_t getDuration() const {
			return getIntAttr(ITEM_ATTRIBUTE_DURATION);
		}

		void setDecaying(ItemDecayState_t decayState) {
			setIntAttr(ITEM_ATTRIBUTE_DECAYSTATE, decayState);
		}
		ItemDecayState_t getDecaying() const {
			return (ItemDecayState_t)getIntAttr(ITEM_ATTRIBUTE_DECAYSTATE);
		}

	protected:
		inline bool hasAttribute(itemAttrTypes type) const {
			return (type & attributeBits) != 0;
		}
		void removeAttribute(itemAttrTypes type);

		static std::string emptyString;

		struct Attribute
		{
			uint8_t* value;
			itemAttrTypes type;

			Attribute(itemAttrTypes type) : value(nullptr), type(type) {}
			Attribute(const Attribute& i) {
				type = i.type;
				if (ItemAttributes::validateIntAttrType(type)) {
					value = i.value;
				} else if (ItemAttributes::validateStrAttrType(type)) {
					value = (uint8_t*)new std::string( *((std::string*)i.value) );
				} else {
					value = nullptr;
				}
			}
			Attribute(Attribute&& attribute) : value(attribute.value), type(attribute.type) {
				attribute.value = nullptr;
				attribute.type = ITEM_ATTRIBUTE_NONE;
			}
			~Attribute() {
				if (ItemAttributes::validateStrAttrType(type)) {
					delete (std::string*)value;
				}
			}
			Attribute& operator=(Attribute other) {
				swap(*this, other);
				return *this;
			}
			Attribute& operator=(Attribute&& other) {
				if (this != &other) {
					if (ItemAttributes::validateStrAttrType(type)) {
						delete (std::string*)value;
					}

					value = other.value;
					type = other.type;
					other.value = nullptr;
					other.type = ITEM_ATTRIBUTE_NONE;
				}
				return *this;
			}

			void swap(Attribute& first, Attribute &second) {
				std::swap(first.value, second.value);
				std::swap(first.type, second.type);
			}
		};

		std::forward_list<Attribute> attributes;
		uint32_t attributeBits;

		const std::string& getStrAttr(itemAttrTypes type) const;
		void setStrAttr(itemAttrTypes type, const std::string& value);

		int32_t getIntAttr(itemAttrTypes type) const;
		void setIntAttr(itemAttrTypes type, int32_t value);
		void increaseIntAttr(itemAttrTypes type, int32_t value);

		static bool validateIntAttrType(itemAttrTypes type);
		static bool validateStrAttrType(itemAttrTypes type);

		void addAttr(Attribute* attr);
		Attribute* getAttrConst(itemAttrTypes type) const;
		Attribute& getAttr(itemAttrTypes type);

	public:
		const std::forward_list<Attribute>& getList() const {
			return attributes;
		}

	friend class Item;
};

class Item : virtual public Thing
{
	public:
		//Factory member to create item of right type based on type
		static Item* CreateItem(const uint16_t _type, uint16_t _count = 0);
		static Item* CreateItem(PropStream& propStream);
		static Items items;

		// Constructor for items
		Item(const uint16_t _type, uint16_t _count = 0);
		Item(const Item& i);
		virtual Item* clone() const;
		virtual void stealAttributes(Item* item);

		virtual ~Item();

		virtual Item* getItem() {
			return this;
		}
		virtual const Item* getItem() const {
			return this;
		}
		virtual Container* getContainer() {
			return nullptr;
		}
		virtual const Container* getContainer() const {
			return nullptr;
		}
		virtual Teleport* getTeleport() {
			return nullptr;
		}
		virtual const Teleport* getTeleport() const {
			return nullptr;
		}
		virtual TrashHolder* getTrashHolder() {
			return nullptr;
		}
		virtual const TrashHolder* getTrashHolder() const {
			return nullptr;
		}
		virtual Mailbox* getMailbox() {
			return nullptr;
		}
		virtual const Mailbox* getMailbox() const {
			return nullptr;
		}
		virtual Door* getDoor() {
			return nullptr;
		}
		virtual const Door* getDoor() const {
			return nullptr;
		}
		virtual MagicField* getMagicField() {
			return nullptr;
		}
		virtual const MagicField* getMagicField() const {
			return nullptr;
		}
		virtual BedItem* getBed() {
			return nullptr;
		}
		virtual const BedItem* getBed() const {
			return nullptr;
		}

		const std::string& getStrAttr(itemAttrTypes type) const {
			if (!attributes) {
				return ItemAttributes::emptyString;
			}
			return attributes->getStrAttr(type);
		}
		void setStrAttr(itemAttrTypes type, const std::string& value) {
			getAttributes()->setStrAttr(type, value);
		}

		int32_t getIntAttr(itemAttrTypes type) const {
			if (!attributes) {
				return 0;
			}
			return attributes->getIntAttr(type);
		}
		void setIntAttr(itemAttrTypes type, int32_t value) {
			getAttributes()->setIntAttr(type, value);
		}
		void increaseIntAttr(itemAttrTypes type, int32_t value) {
			getAttributes()->increaseIntAttr(type, value);
		}

		void removeAttribute(itemAttrTypes type) {
			if (attributes) {
				attributes->removeAttribute(type);
			}
		}
		bool hasAttribute(itemAttrTypes type) const {
			if (!attributes) {
				return false;
			}
			return attributes->hasAttribute(type);
		}

		void setSpecialDescription(const std::string& desc) {
			setStrAttr(ITEM_ATTRIBUTE_DESCRIPTION, desc);
		}
		const std::string& getSpecialDescription() const {
			return getStrAttr(ITEM_ATTRIBUTE_DESCRIPTION);
		}

		void setText(const std::string& text) {
			setStrAttr(ITEM_ATTRIBUTE_TEXT, text);
		}
		void resetText() {
			removeAttribute(ITEM_ATTRIBUTE_TEXT);
		}
		const std::string& getText() const {
			return getStrAttr(ITEM_ATTRIBUTE_TEXT);
		}

		void setDate(int32_t n) {
			setIntAttr(ITEM_ATTRIBUTE_DATE, n);
		}
		void resetDate() {
			removeAttribute(ITEM_ATTRIBUTE_DATE);
		}
		time_t getDate() const {
			return (time_t)getIntAttr(ITEM_ATTRIBUTE_DATE);
		}

		void setWriter(const std::string& _writer) {
			setStrAttr(ITEM_ATTRIBUTE_WRITER, _writer);
		}
		void resetWriter() {
			removeAttribute(ITEM_ATTRIBUTE_WRITER);
		}
		const std::string& getWriter() const {
			return getStrAttr(ITEM_ATTRIBUTE_WRITER);
		}

		void setActionId(uint16_t n) {
			if (n < 100) {
				n = 100;
			}

			setIntAttr(ITEM_ATTRIBUTE_ACTIONID, n);
		}
		uint16_t getActionId() const {
			if (!attributes) {
				return 0;
			}
			return (uint16_t)getIntAttr(ITEM_ATTRIBUTE_ACTIONID);
		}

		uint16_t getUniqueId() const {
			if (!attributes) {
				return 0;
			}
			return (uint16_t)getIntAttr(ITEM_ATTRIBUTE_UNIQUEID);
		}

		void setCharges(uint16_t n) {
			setIntAttr(ITEM_ATTRIBUTE_CHARGES, n);
		}
		uint16_t getCharges() const {
			if (!attributes) {
				return 0;
			}
			return (uint16_t)getIntAttr(ITEM_ATTRIBUTE_CHARGES);
		}

		void setFluidType(uint16_t n) {
			setIntAttr(ITEM_ATTRIBUTE_FLUIDTYPE, n);
		}
		uint16_t getFluidType() const {
			if (!attributes) {
				return 0;
			}
			return (uint16_t)getIntAttr(ITEM_ATTRIBUTE_FLUIDTYPE);
		}

		void setOwner(uint32_t _owner) {
			setIntAttr(ITEM_ATTRIBUTE_OWNER, _owner);
		}
		uint32_t getOwner() const {
			if (!attributes) {
				return 0;
			}
			return getIntAttr(ITEM_ATTRIBUTE_OWNER);
		}

		void setCorpseOwner(uint32_t _corpseOwner) {
			setIntAttr(ITEM_ATTRIBUTE_CORPSEOWNER, _corpseOwner);
		}
		uint32_t getCorpseOwner() const {
			if (!attributes) {
				return 0;
			}
			return getIntAttr(ITEM_ATTRIBUTE_CORPSEOWNER);
		}

		void setDuration(int32_t time) {
			setIntAttr(ITEM_ATTRIBUTE_DURATION, time);
		}
		void decreaseDuration(int32_t time) {
			increaseIntAttr(ITEM_ATTRIBUTE_DURATION, -time);
		}
		uint32_t getDuration() const {
			if (!attributes) {
				return 0;
			}
			return getIntAttr(ITEM_ATTRIBUTE_DURATION);
		}

		void setDecaying(ItemDecayState_t decayState) {
			setIntAttr(ITEM_ATTRIBUTE_DECAYSTATE, decayState);
		}
		ItemDecayState_t getDecaying() const {
			if (!attributes) {
				return (ItemDecayState_t)0;
			}
			return (ItemDecayState_t)getIntAttr(ITEM_ATTRIBUTE_DECAYSTATE);
		}

		static std::string getDescription(const ItemType& it, int32_t lookDistance, const Item* item = nullptr, int32_t subType = -1, bool addArticle = true);
		static std::string getNameDescription(const ItemType& it, const Item* item = nullptr, int32_t subType = -1, bool addArticle = true);
		static std::string getWeightDescription(const ItemType& it, double weight, uint32_t count = 1);

		virtual std::string getDescription(int32_t lookDistance) const;
		std::string getNameDescription() const;
		std::string getWeightDescription() const;

		//serialization
		virtual Attr_ReadValue readAttr(AttrTypes_t attr, PropStream& propStream);
		virtual bool unserializeAttr(PropStream& propStream);
		virtual bool unserializeItemNode(FileLoader& f, NODE node, PropStream& propStream);

		virtual bool serializeAttr(PropWriteStream& propWriteStream) const;

		virtual bool isPushable() const {
			return isMoveable();
		}
		virtual int32_t getThrowRange() const {
			return (isPickupable() ? 15 : 2);
		}

		uint16_t getID() const {
			return id;
		}
		uint16_t getClientID() const {
			return items[id].clientId;
		}
		uint16_t getWareID() const {
			return items[id].wareId;
		}
		void setID(uint16_t newid);

		// Returns the player that is holding this item in his inventory
		Player* getHoldingPlayer();
		const Player* getHoldingPlayer() const;

		WeaponType_t getWeaponType() const {
			return items[id].weaponType;
		}
		Ammo_t	getAmmoType() const {
			return items[id].ammoType;
		}
		int32_t getShootRange() const {
			return items[id].shootRange;
		}

		virtual double getWeight() const;
		int32_t getAttack() const {
			return items[id].attack;
		}
		int32_t getArmor() const {
			return items[id].armor;
		}
		int32_t getDefense() const {
			return items[id].defense;
		}
		int32_t getExtraDefense() const {
			return items[id].extraDefense;
		}
		int32_t getSlotPosition() const {
			return items[id].slotPosition;
		}
		int32_t getHitChance() const {
			return items[id].hitChance;
		}

		bool isReadable() const {
			return items[id].canReadText;
		}
		bool canWriteText() const {
			return items[id].canWriteText;
		}
		uint16_t getMaxWriteLength() const {
			return items[id].maxTextLen;
		}

		int32_t getWorth() const;
		void getLight(LightInfo& lightInfo) const;

		bool hasProperty(ITEMPROPERTY prop) const;
		bool isBlocking() const {
			return items[id].blockSolid;
		}
		bool isStackable() const {
			return items[id].stackable;
		}
		bool isRune() const {
			return items[id].isRune();
		}
		bool isFluidContainer() const {
			return (items[id].isFluidContainer());
		}
		bool isAlwaysOnTop() const {
			return items[id].alwaysOnTop;
		}
		bool isGroundTile() const {
			return items[id].isGroundTile();
		}
		bool isSplash() const {
			return items[id].isSplash();
		}
		bool isMagicField() const {
			return items[id].isMagicField();
		}
		bool isMoveable() const {
			return items[id].moveable;
		}
		bool isPickupable() const {
			return items[id].pickupable;
		}
		bool isWeapon() const {
			return (items[id].weaponType != WEAPON_NONE);
		}
		bool isUseable() const {
			return items[id].useable;
		}
		bool isHangable() const {
			return items[id].isHangable;
		}
		bool isRoteable() const {
			const ItemType& it = items[id];
			return it.rotable && it.rotateTo;
		}
		bool isDoor() const {
			return items[id].isDoor();
		}
		bool isBed() const {
			return items[id].isBed();
		}
		bool hasWalkStack() const {
			return items[id].walkStack;
		}

		bool floorChangeDown() const {
			return items[id].floorChangeDown;
		}
		bool floorChangeNorth() const {
			return items[id].floorChangeNorth;
		}
		bool floorChangeSouth() const {
			return items[id].floorChangeSouth;
		}
		bool floorChangeSouthAlt() const {
			return items[id].floorChangeSouthAlt;
		}
		bool floorChangeEast() const {
			return items[id].floorChangeEast;
		}
		bool floorChangeEastAlt() const {
			return items[id].floorChangeEastAlt;
		}
		bool floorChangeWest() const {
			return items[id].floorChangeWest;
		}

		const std::string& getName() const {
			return items[id].name;
		}
		const std::string getPluralName() const {
			return items[id].getPluralName();
		}
		const std::string& getArticle() const {
			return items[id].article;
		}

		// get the number of items
		uint16_t getItemCount() const {
			return count;
		}
		void setItemCount(uint8_t n) {
			count = n;
		}

		static uint32_t countByType(const Item* i, int32_t subType);

		void setDefaultSubtype();
		uint16_t getSubType() const;
		void setSubType(uint16_t n);

		void setUniqueId(uint16_t n);

		void setDefaultDuration() {
			uint32_t duration = getDefaultDuration();
			if (duration != 0) {
				setDuration(duration);
			}
		}
		uint32_t getDefaultDuration() const {
			return items[id].decayTime * 1000;
		}
		bool canDecay() const;

		virtual bool canRemove() const {
			return true;
		}
		virtual bool canTransform() const {
			return true;
		}
		virtual void onRemoved();
		virtual bool onTradeEvent(TradeEvents_t, Player*) {
			return true;
		}

		virtual void __startDecaying();

		bool isLoadedFromMap() const {
			return loadedFromMap;
		}
		void setLoadedFromMap(bool value) {
			loadedFromMap = value;
		}
		bool isCleanable() const {
			return !loadedFromMap && canRemove() && isPickupable() && !hasAttribute(ITEM_ATTRIBUTE_UNIQUEID) && !hasAttribute(ITEM_ATTRIBUTE_ACTIONID);
		}

		bool hasAttributes() const {
			return attributes != nullptr;
		}

		ItemAttributes* getAttributes() {
			if (!attributes) {
				attributes = new ItemAttributes();
			}
			return attributes;
		}

		void useThing2() {
			++useCount;
		}
		void releaseThing2() {
			if (--useCount == 0) {
				delete this;
			}
		}

		Cylinder* getParent() {
			return parent;
		}
		const Cylinder* getParent() const {
			return parent;
		}
		void setParent(Cylinder* cylinder) {
			parent = cylinder;
		}
		Cylinder* getTopParent();
		const Cylinder* getTopParent() const;
		Tile* getTile();
		const Tile* getTile() const;
		bool isRemoved() const {
			return !parent || parent->isRemoved();
		}

	protected:
		std::string getWeightDescription(double weight) const;

		Cylinder* parent;
		ItemAttributes* attributes;

		uint32_t useCount;

		uint16_t id;  // the same id as in ItemType
		uint8_t count; // number of stacked items

		bool loadedFromMap;

		//Don't add variables here, use the ItemAttribute class.
};

typedef std::list<Item*> ItemList;
typedef std::deque<Item*> ItemDeque;

inline uint32_t Item::countByType(const Item* i, int32_t subType)
{
	if (subType == -1 || subType == i->getSubType()) {
		return i->getItemCount();
	}

	return 0;
}

#endif
