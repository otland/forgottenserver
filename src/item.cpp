/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2018  Mark Samman <mark.samman@gmail.com>
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

#include "item.h"
#include "container.h"
#include "teleport.h"
#include "trashholder.h"
#include "mailbox.h"
#include "house.h"
#include "game.h"
#include "bed.h"

#include "actions.h"
#include "spells.h"

extern Game g_game;
extern Spells* g_spells;
extern Vocations g_vocations;

Items Item::items;

Item* Item::CreateItem(const uint16_t type, uint16_t count /*= 0*/)
{
	Item* newItem = nullptr;

	const ItemType& it = Item::items[type];
	if (it.group == ITEM_GROUP_DEPRECATED) {
		return nullptr;
	}

	if (it.stackable && count == 0) {
		count = 1;
	}

	if (it.id != 0) {
		if (it.isDepot()) {
			newItem = new DepotLocker(type);
		} else if (it.isContainer()) {
			newItem = new Container(type);
		} else if (it.isTeleport()) {
			newItem = new Teleport(type);
		} else if (it.isMagicField()) {
			newItem = new MagicField(type);
		} else if (it.isDoor()) {
			newItem = new Door(type);
		} else if (it.isTrashHolder()) {
			newItem = new TrashHolder(type);
		} else if (it.isMailbox()) {
			newItem = new Mailbox(type);
		} else if (it.isBed()) {
			newItem = new BedItem(type);
		} else if (it.id >= 2210 && it.id <= 2212) {
			newItem = new Item(type - 3, count);
		} else if (it.id == 2215 || it.id == 2216) {
			newItem = new Item(type - 2, count);
		} else if (it.id >= 2202 && it.id <= 2206) {
			newItem = new Item(type - 37, count);
		} else if (it.id == 2640) {
			newItem = new Item(6132, count);
		} else if (it.id == 6301) {
			newItem = new Item(6300, count);
		} else if (it.id == 18528) {
			newItem = new Item(18408, count);
		} else {
			newItem = new Item(type, count);
		}

		newItem->incrementReferenceCounter();
	}

	return newItem;
}

Container* Item::CreateItemAsContainer(const uint16_t type, uint16_t size)
{
	const ItemType& it = Item::items[type];
	if (it.id == 0 || it.group == ITEM_GROUP_DEPRECATED || it.stackable || it.useable || it.moveable || it.pickupable || it.isDepot() || it.isSplash() || it.isDoor()) {
		return nullptr;
	}

	Container* newItem = new Container(type, size);
	newItem->incrementReferenceCounter();
	return newItem;
}

Item* Item::CreateItem(PropStream& propStream)
{
	uint16_t id;
	if (!propStream.read<uint16_t>(id)) {
		return nullptr;
	}

	switch (id) {
		case ITEM_FIREFIELD_PVP_FULL:
			id = ITEM_FIREFIELD_PERSISTENT_FULL;
			break;

		case ITEM_FIREFIELD_PVP_MEDIUM:
			id = ITEM_FIREFIELD_PERSISTENT_MEDIUM;
			break;

		case ITEM_FIREFIELD_PVP_SMALL:
			id = ITEM_FIREFIELD_PERSISTENT_SMALL;
			break;

		case ITEM_ENERGYFIELD_PVP:
			id = ITEM_ENERGYFIELD_PERSISTENT;
			break;

		case ITEM_POISONFIELD_PVP:
			id = ITEM_POISONFIELD_PERSISTENT;
			break;

		case ITEM_MAGICWALL:
			id = ITEM_MAGICWALL_PERSISTENT;
			break;

		case ITEM_WILDGROWTH:
			id = ITEM_WILDGROWTH_PERSISTENT;
			break;

		default:
			break;
	}

	return Item::CreateItem(id, 0);
}

Item::Item(const uint16_t type, uint16_t count /*= 0*/) :
	id(type)
{
	const ItemType& it = items[id];

	if (it.isFluidContainer() || it.isSplash()) {
		setFluidType(count);
	} else if (it.stackable) {
		if (count != 0) {
			setItemCount(count);
		} else if (it.charges != 0) {
			setItemCount(it.charges);
		}
	} else if (it.charges != 0) {
		if (count != 0) {
			setCharges(count);
		} else {
			setCharges(it.charges);
		}
	}

	setDefaultDuration();
}

Item::Item(const Item& i) :
	Thing(), id(i.id), count(i.count), loadedFromMap(i.loadedFromMap)
{
	if (i.attributes) {
		attributes.reset(new ItemAttributes(*i.attributes));
	}
	if (i.abilities) {
		abilities.reset(new ItemAbilities(*i.abilities));
	}
}

Item* Item::clone() const
{
	Item* item = Item::CreateItem(id, count);
	if (attributes) {
		item->attributes.reset(new ItemAttributes(*attributes));
	}
	if (abilities) {
		item->abilities.reset(new ItemAbilities(*abilities));
	}
	return item;
}

bool Item::equals(const Item* otherItem) const
{
	if (!otherItem || id != otherItem->id) {
		return false;
	}

	if (!attributes && !abilities) {
		return !otherItem->attributes && !otherItem->abilities;
	}

	if (attributes) {
		const auto& otherAttributes = otherItem->attributes;
		if (!otherAttributes || attributes->attributeBits != otherAttributes->attributeBits) {
			return false;
		}

		const auto& attributeList = attributes->attributes;
		const auto& otherAttributeList = otherAttributes->attributes;
		for (const auto& attribute : attributeList) {
			if (ItemAttributes::isStrAttrType(attribute.type)) {
				for (const auto& otherAttribute : otherAttributeList) {
					if (attribute.type == otherAttribute.type && *attribute.value.string != *otherAttribute.value.string) {
						return false;
					}
				}
			} else {
				for (const auto& otherAttribute : otherAttributeList) {
					if (attribute.type == otherAttribute.type && attribute.value.integer != otherAttribute.value.integer) {
						return false;
					}
				}
			}
		}
	}
	if (abilities) {
		const auto& otherAbilities = otherItem->abilities;
		if (!otherAbilities || abilities->abilityBits != otherAbilities->abilityBits) {
			return false;
		}

		const auto& abilityList = abilities->abilityList;
		const auto& otherAbilityList = otherAbilities->abilityList;
		for (const auto& ability : abilityList) {
			for (const auto& otherAbility : otherAbilityList) {
				if (ability.type == otherAbility.type && ability.integer != otherAbility.integer) {
					return false;
				}
			}
		}
	}
	return true;
}

void Item::setDefaultSubtype()
{
	const ItemType& it = items[id];

	setItemCount(1);

	if (it.charges != 0) {
		if (it.stackable) {
			setItemCount(it.charges);
		} else {
			setCharges(it.charges);
		}
	}
}

void Item::onRemoved()
{
	ScriptEnvironment::removeTempItem(this);

	if (hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
		g_game.removeUniqueItem(getUniqueId());
	}
}

void Item::setID(uint16_t newid)
{
	const ItemType& prevIt = Item::items[id];
	id = newid;

	const ItemType& it = Item::items[newid];
	uint32_t newDuration = it.decayTime * 1000;

	if (newDuration == 0 && !it.stopTime && it.decayTo < 0) {
		removeAttribute(ITEM_ATTRIBUTE_DECAYSTATE);
		removeAttribute(ITEM_ATTRIBUTE_DURATION);
	}

	removeAttribute(ITEM_ATTRIBUTE_CORPSEOWNER);

	if (newDuration > 0 && (!prevIt.stopTime || !hasAttribute(ITEM_ATTRIBUTE_DURATION))) {
		setDecaying(DECAYING_FALSE);
		setDuration(newDuration);
	}
}

Cylinder* Item::getTopParent()
{
	Cylinder* aux = getParent();
	Cylinder* prevaux = dynamic_cast<Cylinder*>(this);
	if (!aux) {
		return prevaux;
	}

	while (aux->getParent() != nullptr) {
		prevaux = aux;
		aux = aux->getParent();
	}

	if (prevaux) {
		return prevaux;
	}
	return aux;
}

const Cylinder* Item::getTopParent() const
{
	const Cylinder* aux = getParent();
	const Cylinder* prevaux = dynamic_cast<const Cylinder*>(this);
	if (!aux) {
		return prevaux;
	}

	while (aux->getParent() != nullptr) {
		prevaux = aux;
		aux = aux->getParent();
	}

	if (prevaux) {
		return prevaux;
	}
	return aux;
}

Tile* Item::getTile()
{
	Cylinder* cylinder = getTopParent();
	//get root cylinder
	if (cylinder && cylinder->getParent()) {
		cylinder = cylinder->getParent();
	}
	return dynamic_cast<Tile*>(cylinder);
}

const Tile* Item::getTile() const
{
	const Cylinder* cylinder = getTopParent();
	//get root cylinder
	if (cylinder && cylinder->getParent()) {
		cylinder = cylinder->getParent();
	}
	return dynamic_cast<const Tile*>(cylinder);
}

uint16_t Item::getSubType() const
{
	const ItemType& it = items[id];
	if (it.isFluidContainer() || it.isSplash()) {
		return getFluidType();
	} else if (it.stackable) {
		return count;
	} else if (it.charges != 0) {
		return getCharges();
	}
	return count;
}

Player* Item::getHoldingPlayer() const
{
	Cylinder* p = getParent();
	while (p) {
		if (p->getCreature()) {
			return p->getCreature()->getPlayer();
		}

		p = p->getParent();
	}
	return nullptr;
}

void Item::setSubType(uint16_t n)
{
	const ItemType& it = items[id];
	if (it.isFluidContainer() || it.isSplash()) {
		setFluidType(n);
	} else if (it.stackable) {
		setItemCount(n);
	} else if (it.charges != 0) {
		setCharges(n);
	} else {
		setItemCount(n);
	}
}

Attr_ReadValue Item::readAttr(AttrTypes_t attr, PropStream& propStream)
{
	switch (attr) {
		case ATTR_COUNT:
		case ATTR_RUNE_CHARGES: {
			uint8_t count;
			if (!propStream.read<uint8_t>(count)) {
				return ATTR_READ_ERROR;
			}

			setSubType(count);
			break;
		}

		case ATTR_ACTION_ID: {
			uint16_t actionId;
			if (!propStream.read<uint16_t>(actionId)) {
				return ATTR_READ_ERROR;
			}

			setActionId(actionId);
			break;
		}

		case ATTR_UNIQUE_ID: {
			uint16_t uniqueId;
			if (!propStream.read<uint16_t>(uniqueId)) {
				return ATTR_READ_ERROR;
			}

			setUniqueId(uniqueId);
			break;
		}

		case ATTR_TEXT: {
			std::string text;
			if (!propStream.readString(text)) {
				return ATTR_READ_ERROR;
			}

			setText(text);
			break;
		}

		case ATTR_WRITTENDATE: {
			uint32_t writtenDate;
			if (!propStream.read<uint32_t>(writtenDate)) {
				return ATTR_READ_ERROR;
			}

			setDate(writtenDate);
			break;
		}

		case ATTR_WRITTENBY: {
			std::string writer;
			if (!propStream.readString(writer)) {
				return ATTR_READ_ERROR;
			}

			setWriter(writer);
			break;
		}

		case ATTR_DESC: {
			std::string text;
			if (!propStream.readString(text)) {
				return ATTR_READ_ERROR;
			}

			setSpecialDescription(text);
			break;
		}

		case ATTR_CHARGES: {
			uint16_t charges;
			if (!propStream.read<uint16_t>(charges)) {
				return ATTR_READ_ERROR;
			}

			setSubType(charges);
			break;
		}

		case ATTR_DURATION: {
			int32_t duration;
			if (!propStream.read<int32_t>(duration)) {
				return ATTR_READ_ERROR;
			}

			setDuration(std::max<int32_t>(0, duration));
			break;
		}

		case ATTR_DECAYING_STATE: {
			uint8_t state;
			if (!propStream.read<uint8_t>(state)) {
				return ATTR_READ_ERROR;
			}

			if (state != DECAYING_FALSE) {
				setDecaying(DECAYING_PENDING);
			}
			break;
		}

		case ATTR_NAME: {
			std::string name;
			if (!propStream.readString(name)) {
				return ATTR_READ_ERROR;
			}

			setStrAttr(ITEM_ATTRIBUTE_NAME, name);
			break;
		}

		case ATTR_ARTICLE: {
			std::string article;
			if (!propStream.readString(article)) {
				return ATTR_READ_ERROR;
			}

			setStrAttr(ITEM_ATTRIBUTE_ARTICLE, article);
			break;
		}

		case ATTR_PLURALNAME: {
			std::string pluralName;
			if (!propStream.readString(pluralName)) {
				return ATTR_READ_ERROR;
			}

			setStrAttr(ITEM_ATTRIBUTE_PLURALNAME, pluralName);
			break;
		}

		case ATTR_WEIGHT: {
			uint32_t weight;
			if (!propStream.read<uint32_t>(weight)) {
				return ATTR_READ_ERROR;
			}

			setIntAttr(ITEM_ATTRIBUTE_WEIGHT, weight);
			break;
		}

		case ATTR_ATTACK: {
			int32_t attack;
			if (!propStream.read<int32_t>(attack)) {
				return ATTR_READ_ERROR;
			}

			setIntAttr(ITEM_ATTRIBUTE_ATTACK, attack);
			break;
		}

		case ATTR_DEFENSE: {
			int32_t defense;
			if (!propStream.read<int32_t>(defense)) {
				return ATTR_READ_ERROR;
			}

			setIntAttr(ITEM_ATTRIBUTE_DEFENSE, defense);
			break;
		}

		case ATTR_EXTRADEFENSE: {
			int32_t extraDefense;
			if (!propStream.read<int32_t>(extraDefense)) {
				return ATTR_READ_ERROR;
			}

			setIntAttr(ITEM_ATTRIBUTE_EXTRADEFENSE, extraDefense);
			break;
		}

		case ATTR_ARMOR: {
			int32_t armor;
			if (!propStream.read<int32_t>(armor)) {
				return ATTR_READ_ERROR;
			}

			setIntAttr(ITEM_ATTRIBUTE_ARMOR, armor);
			break;
		}

		case ATTR_HITCHANCE: {
			int8_t hitChance;
			if (!propStream.read<int8_t>(hitChance)) {
				return ATTR_READ_ERROR;
			}

			setIntAttr(ITEM_ATTRIBUTE_HITCHANCE, hitChance);
			break;
		}

		case ATTR_SHOOTRANGE: {
			uint8_t shootRange;
			if (!propStream.read<uint8_t>(shootRange)) {
				return ATTR_READ_ERROR;
			}

			setIntAttr(ITEM_ATTRIBUTE_SHOOTRANGE, shootRange);
			break;
		}

		//these should be handled through derived classes
		//If these are called then something has changed in the items.xml since the map was saved
		//just read the values

		//Depot class
		case ATTR_DEPOT_ID: {
			if (!propStream.skip(2)) {
				return ATTR_READ_ERROR;
			}
			break;
		}

		//Door class
		case ATTR_HOUSEDOORID: {
			if (!propStream.skip(1)) {
				return ATTR_READ_ERROR;
			}
			break;
		}

		//Bed class
		case ATTR_SLEEPERGUID: {
			if (!propStream.skip(4)) {
				return ATTR_READ_ERROR;
			}
			break;
		}

		case ATTR_SLEEPSTART: {
			if (!propStream.skip(4)) {
				return ATTR_READ_ERROR;
			}
			break;
		}

		//Teleport class
		case ATTR_TELE_DEST: {
			if (!propStream.skip(5)) {
				return ATTR_READ_ERROR;
			}
			break;
		}

		//Container class
		case ATTR_CONTAINER_ITEMS: {
			return ATTR_READ_ERROR;
		}

		case ATTR_CUSTOM_ATTRIBUTES: {
			uint64_t size;
			if (!propStream.read<uint64_t>(size)) {
				return ATTR_READ_ERROR;
			}

			for (uint64_t i = 0; i < size; i++) {
				// Unserialize key type and value
				std::string key;
				if (!propStream.readString(key)) {
					return ATTR_READ_ERROR;
				};

				// Unserialize value type and value
				ItemAttributes::CustomAttribute val;
				if (!val.unserialize(propStream)) {
					return ATTR_READ_ERROR;
				}

				setCustomAttribute(key, val);
			}
			break;
		}

		default:
			return ATTR_READ_ERROR;
	}

	return ATTR_READ_CONTINUE;
}

bool Item::unserializeAttr(PropStream& propStream)
{
	uint8_t attr_type;
	while (propStream.read<uint8_t>(attr_type) && attr_type != 0) {
		Attr_ReadValue ret = readAttr(static_cast<AttrTypes_t>(attr_type), propStream);
		if (ret == ATTR_READ_ERROR) {
			return false;
		} else if (ret == ATTR_READ_END) {
			return true;
		}
	}
	return true;
}

bool Item::unserializeItemNode(OTB::Loader&, const OTB::Node&, PropStream& propStream)
{
	return unserializeAttr(propStream);
}

Ability_ReadValue Item::readAbility(AbilityTypes_t type, PropStream& propStream)
{
	switch (type) {
		case ABILITY_HEALTHGAIN: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_HEALTHGAIN, value);
			break;
		}
		case ABILITY_HEALTHTICKS: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_HEALTHTICKS, value);
			break;
		}
		case ABILITY_MANAGAIN: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_MANAGAIN, value);
			break;
		}
		case ABILITY_MANATICKS: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_MANATICKS, value);
			break;
		}
		case ABILITY_CONDITIONSUPPRESSIONS: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_CONDITIONSUPPRESSIONS, value);
			break;
		}
		case ABILITY_MAXHITPOINTS: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_MAXHITPOINTS, value);
			break;
		}
		case ABILITY_MAXMANAPOINTS: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_MAXMANAPOINTS, value);
			break;
		}
		case ABILITY_MAGICPOINTS: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_MAGICPOINTS, value);
			break;
		}
		case ABILITY_MAXHITPOINTSPERCENT: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_MAXHITPOINTSPERCENT, value);
			break;
		}
		case ABILITY_MAXMANAPOINTSPERCENT: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_MAXMANAPOINTSPERCENT, value);
			break;
		}
		case ABILITY_MAGICPOINTSPERCENT: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_MAGICPOINTSPERCENT, value);
			break;
		}
		case ABILITY_SKILLFIST: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_SKILLFIST, value);
			break;
		}
		case ABILITY_SKILLCLUB: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_SKILLCLUB, value);
			break;
		}
		case ABILITY_SKILLSWORD: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_SKILLSWORD, value);
			break;
		}
		case ABILITY_SKILLAXE: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_SKILLAXE, value);
			break;
		}
		case ABILITY_SKILLDISTANCE: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_SKILLDISTANCE, value);
			break;
		}
		case ABILITY_SKILLSHIELD: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_SKILLSHIELD, value);
			break;
		}
		case ABILITY_SKILLFISHING: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_SKILLFISHING, value);
			break;
		}
		case ABILITY_CRITICALHITCHANCE: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_CRITICALHITCHANCE, value);
			break;
		}
		case ABILITY_CRITICALHITAMOUNT: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_CRITICALHITAMOUNT, value);
			break;
		}
		case ABILITY_LIFELEECHCHANCE: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_LIFELEECHCHANCE, value);
			break;
		}
		case ABILITY_LIFELEECHAMOUNT: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_LIFELEECHAMOUNT, value);
			break;
		}
		case ABILITY_MANALEECHCHANCE: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_MANALEECHCHANCE, value);
			break;
		}
		case ABILITY_MANALEECHAMOUNT: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_MANALEECHAMOUNT, value);
			break;
		}
		case ABILITY_SPEED: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_SPEED, value);
			break;
		}
		case ABILITY_ABSORBPHYSICAL: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_ABSORBPHYSICAL, value);
			break;
		}
		case ABILITY_ABSORBENERGY: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_ABSORBENERGY, value);
			break;
		}
		case ABILITY_ABSORBEARTH: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_ABSORBEARTH, value);
			break;
		}
		case ABILITY_ABSORBFIRE: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_ABSORBFIRE, value);
			break;
		}
		case ABILITY_ABSORBWATER: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_ABSORBWATER, value);
			break;
		}
		case ABILITY_ABSORBICE: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_ABSORBICE, value);
			break;
		}
		case ABILITY_ABSORBHOLY: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_ABSORBHOLY, value);
			break;
		}
		case ABILITY_ABSORBDEATH: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_ABSORBDEATH, value);
			break;
		}
		case ABILITY_FIELDABSORBPHYSICAL: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_FIELDABSORBPHYSICAL, value);
			break;
		}
		case ABILITY_FIELDABSORBENERGY: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_FIELDABSORBENERGY, value);
			break;
		}
		case ABILITY_FIELDABSORBEARTH: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_FIELDABSORBEARTH, value);
			break;
		}
		case ABILITY_FIELDABSORBFIRE: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_FIELDABSORBFIRE, value);
			break;
		}
		case ABILITY_FIELDABSORBWATER: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_FIELDABSORBWATER, value);
			break;
		}
		case ABILITY_FIELDABSORBICE: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_FIELDABSORBICE, value);
			break;
		}
		case ABILITY_FIELDABSORBHOLY: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_FIELDABSORBHOLY, value);
			break;
		}
		case ABILITY_FIELDABSORBDEATH: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_FIELDABSORBDEATH, value);
			break;
		}
		case ABILITY_ELEMENTTYPE: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_ELEMENTTYPE, value);
			break;
		}
		case ABILITY_ELEMENTDAMAGE: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_ELEMENTDAMAGE, value);
			break;
		}
		case ABILITY_MANASHIELD: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_MANASHIELD, value);
			break;
		}
		case ABILITY_INVISIBLE: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_INVISIBLE, value);
			break;
		}
		case ABILITY_REGENERATION: {
			int64_t value;
			if (!propStream.read<int64_t>(value)) {
				return ABILITY_READ_ERROR;
			}
			setAbilityInt(ITEM_ABILITY_REGENERATION, value);
			break;
		}
		default:
			return ABILITY_READ_ERROR;
	}
	return ABILITY_READ_CONTINUE;
}

bool Item::unserializeAbility(PropStream& propStream)
{
	uint8_t ability_type;
	while (propStream.read<uint8_t>(ability_type) && ability_type != 0) {
		Ability_ReadValue ret = readAbility(static_cast<AbilityTypes_t>(ability_type), propStream);
		if (ret == ABILITY_READ_ERROR) {
			return false;
		} else if (ret == ABILITY_READ_END) {
			return true;
		}
	}
	return true;
}

void Item::serializeAbility(PropWriteStream& propWriteStream) const
{
	for (uint8_t i = ABILITY_START; i <= ABILITY_END; i++) {
		itemAbilityTypes itemAbility = static_cast<itemAbilityTypes>(1ULL << (i - 1));
		if (hasAbility(itemAbility)) {
			propWriteStream.write<uint8_t>(static_cast<AbilityTypes_t>(i));
			propWriteStream.write<int64_t>(getAbilityInt(itemAbility));
		}
	}
}

void Item::serializeAttr(PropWriteStream& propWriteStream) const
{
	const ItemType& it = items[id];
	if (it.stackable || it.isFluidContainer() || it.isSplash()) {
		propWriteStream.write<uint8_t>(ATTR_COUNT);
		propWriteStream.write<uint8_t>(getSubType());
	}

	uint16_t charges = getCharges();
	if (charges != 0) {
		propWriteStream.write<uint8_t>(ATTR_CHARGES);
		propWriteStream.write<uint16_t>(charges);
	}

	if (it.moveable) {
		uint16_t actionId = getActionId();
		if (actionId != 0) {
			propWriteStream.write<uint8_t>(ATTR_ACTION_ID);
			propWriteStream.write<uint16_t>(actionId);
		}
	}

	const std::string& text = getText();
	if (!text.empty()) {
		propWriteStream.write<uint8_t>(ATTR_TEXT);
		propWriteStream.writeString(text);
	}

	const time_t writtenDate = getDate();
	if (writtenDate != 0) {
		propWriteStream.write<uint8_t>(ATTR_WRITTENDATE);
		propWriteStream.write<uint32_t>(writtenDate);
	}

	const std::string& writer = getWriter();
	if (!writer.empty()) {
		propWriteStream.write<uint8_t>(ATTR_WRITTENBY);
		propWriteStream.writeString(writer);
	}

	const std::string& specialDesc = getSpecialDescription();
	if (!specialDesc.empty()) {
		propWriteStream.write<uint8_t>(ATTR_DESC);
		propWriteStream.writeString(specialDesc);
	}

	if (hasAttribute(ITEM_ATTRIBUTE_DURATION)) {
		propWriteStream.write<uint8_t>(ATTR_DURATION);
		propWriteStream.write<uint32_t>(getIntAttr(ITEM_ATTRIBUTE_DURATION));
	}

	ItemDecayState_t decayState = getDecaying();
	if (decayState == DECAYING_TRUE || decayState == DECAYING_PENDING) {
		propWriteStream.write<uint8_t>(ATTR_DECAYING_STATE);
		propWriteStream.write<uint8_t>(decayState);
	}

	if (hasAttribute(ITEM_ATTRIBUTE_NAME)) {
		propWriteStream.write<uint8_t>(ATTR_NAME);
		propWriteStream.writeString(getStrAttr(ITEM_ATTRIBUTE_NAME));
	}

	if (hasAttribute(ITEM_ATTRIBUTE_ARTICLE)) {
		propWriteStream.write<uint8_t>(ATTR_ARTICLE);
		propWriteStream.writeString(getStrAttr(ITEM_ATTRIBUTE_ARTICLE));
	}

	if (hasAttribute(ITEM_ATTRIBUTE_PLURALNAME)) {
		propWriteStream.write<uint8_t>(ATTR_PLURALNAME);
		propWriteStream.writeString(getStrAttr(ITEM_ATTRIBUTE_PLURALNAME));
	}

	if (hasAttribute(ITEM_ATTRIBUTE_WEIGHT)) {
		propWriteStream.write<uint8_t>(ATTR_WEIGHT);
		propWriteStream.write<uint32_t>(getIntAttr(ITEM_ATTRIBUTE_WEIGHT));
	}

	if (hasAttribute(ITEM_ATTRIBUTE_ATTACK)) {
		propWriteStream.write<uint8_t>(ATTR_ATTACK);
		propWriteStream.write<int32_t>(getIntAttr(ITEM_ATTRIBUTE_ATTACK));
	}

	if (hasAttribute(ITEM_ATTRIBUTE_DEFENSE)) {
		propWriteStream.write<uint8_t>(ATTR_DEFENSE);
		propWriteStream.write<int32_t>(getIntAttr(ITEM_ATTRIBUTE_DEFENSE));
	}

	if (hasAttribute(ITEM_ATTRIBUTE_EXTRADEFENSE)) {
		propWriteStream.write<uint8_t>(ATTR_EXTRADEFENSE);
		propWriteStream.write<int32_t>(getIntAttr(ITEM_ATTRIBUTE_EXTRADEFENSE));
	}

	if (hasAttribute(ITEM_ATTRIBUTE_ARMOR)) {
		propWriteStream.write<uint8_t>(ATTR_ARMOR);
		propWriteStream.write<int32_t>(getIntAttr(ITEM_ATTRIBUTE_ARMOR));
	}

	if (hasAttribute(ITEM_ATTRIBUTE_HITCHANCE)) {
		propWriteStream.write<uint8_t>(ATTR_HITCHANCE);
		propWriteStream.write<int8_t>(getIntAttr(ITEM_ATTRIBUTE_HITCHANCE));
	}

	if (hasAttribute(ITEM_ATTRIBUTE_SHOOTRANGE)) {
		propWriteStream.write<uint8_t>(ATTR_SHOOTRANGE);
		propWriteStream.write<uint8_t>(getIntAttr(ITEM_ATTRIBUTE_SHOOTRANGE));
	}

	if (hasAttribute(ITEM_ATTRIBUTE_CUSTOM)) {
		const ItemAttributes::CustomAttributeMap* customAttrMap = attributes->getCustomAttributeMap();
		propWriteStream.write<uint8_t>(ATTR_CUSTOM_ATTRIBUTES);
		propWriteStream.write<uint64_t>(static_cast<uint64_t>(customAttrMap->size()));
		for (const auto &entry : *customAttrMap) {
			// Serializing key type and value
			propWriteStream.writeString(entry.first);

			// Serializing value type and value
			entry.second.serialize(propWriteStream);
		}
	}
}

bool Item::hasProperty(ITEMPROPERTY prop) const
{
	const ItemType& it = items[id];
	switch (prop) {
		case CONST_PROP_BLOCKSOLID: return it.blockSolid;
		case CONST_PROP_MOVEABLE: return it.moveable && !hasAttribute(ITEM_ATTRIBUTE_UNIQUEID);
		case CONST_PROP_HASHEIGHT: return it.hasHeight;
		case CONST_PROP_BLOCKPROJECTILE: return it.blockProjectile;
		case CONST_PROP_BLOCKPATH: return it.blockPathFind;
		case CONST_PROP_ISVERTICAL: return it.isVertical;
		case CONST_PROP_ISHORIZONTAL: return it.isHorizontal;
		case CONST_PROP_IMMOVABLEBLOCKSOLID: return it.blockSolid && (!it.moveable || hasAttribute(ITEM_ATTRIBUTE_UNIQUEID));
		case CONST_PROP_IMMOVABLEBLOCKPATH: return it.blockPathFind && (!it.moveable || hasAttribute(ITEM_ATTRIBUTE_UNIQUEID));
		case CONST_PROP_IMMOVABLENOFIELDBLOCKPATH: return !it.isMagicField() && it.blockPathFind && (!it.moveable || hasAttribute(ITEM_ATTRIBUTE_UNIQUEID));
		case CONST_PROP_NOFIELDBLOCKPATH: return !it.isMagicField() && it.blockPathFind;
		case CONST_PROP_SUPPORTHANGABLE: return it.isHorizontal || it.isVertical;
		default: return false;
	}
}

uint32_t Item::getWeight() const
{
	uint32_t weight = getBaseWeight();
	if (isStackable()) {
		return weight * std::max<uint32_t>(1, getItemCount());
	}
	return weight;
}

std::string Item::getDescription(const ItemType& it, int32_t lookDistance,
                                 const Item* item /*= nullptr*/, int32_t subType /*= -1*/, bool addArticle /*= true*/)
{
	const std::string* text = nullptr;

	std::ostringstream s;
	s << getNameDescription(it, item, subType, addArticle);

	if (item) {
		subType = item->getSubType();
	}

	if (it.isRune()) {
		if (it.runeLevel > 0 || it.runeMagLevel > 0) {
			if (RuneSpell* rune = g_spells->getRuneSpell(it.id)) {
				int32_t tmpSubType = subType;
				if (item) {
					tmpSubType = item->getSubType();
				}
				s << ". " << (it.stackable && tmpSubType > 1 ? "They" : "It") << " can only be used by ";

				const VocSpellMap& vocMap = rune->getVocMap();
				std::vector<Vocation*> showVocMap;

				// vocations are usually listed with the unpromoted and promoted version, the latter being
				// hidden from description, so `total / 2` is most likely the amount of vocations to be shown.
				showVocMap.reserve(vocMap.size() / 2);
				for (const auto& voc : vocMap) {
					if (voc.second) {
						showVocMap.push_back(g_vocations.getVocation(voc.first));
					}
				}

				if (!showVocMap.empty()) {
					auto vocIt = showVocMap.begin(), vocLast = (showVocMap.end() - 1);
					while (vocIt != vocLast) {
						s << asLowerCaseString((*vocIt)->getVocName()) << "s";
						if (++vocIt == vocLast) {
							s << " and ";
						} else {
							s << ", ";
						}
					}
					s << asLowerCaseString((*vocLast)->getVocName()) << "s";
				} else {
					s << "players";
				}

				s << " with";

				if (it.runeLevel > 0) {
					s << " level " << it.runeLevel;
				}

				if (it.runeMagLevel > 0) {
					if (it.runeLevel > 0) {
						s << " and";
					}

					s << " magic level " << it.runeMagLevel;
				}

				s << " or higher";
			}
		}
	} else {
		if (it.weaponType != WEAPON_NONE) {
			bool begin = true;
			if (it.weaponType == WEAPON_DISTANCE && it.ammoType != AMMO_NONE) {
				s << " (Range:" << static_cast<uint16_t>(item ? item->getShootRange() : it.shootRange);

				int32_t attack;
				int8_t hitChance;
				if (item) {
					attack = item->getAttack();
					hitChance = item->getHitChance();
				} else {
					attack = it.attack;
					hitChance = it.hitChance;
				}

				if (attack != 0) {
					s << ", Atk" << std::showpos << attack << std::noshowpos;
				}

				if (hitChance != 0) {
					s << ", Hit%" << std::showpos << static_cast<int16_t>(hitChance) << std::noshowpos;
				}

				begin = false;
			} else if (it.weaponType != WEAPON_AMMO) {

				int32_t attack, defense, extraDefense;
				if (item) {
					attack = item->getAttack();
					defense = item->getDefense();
					extraDefense = item->getExtraDefense();
				} else {
					attack = it.attack;
					defense = it.defense;
					extraDefense = it.extraDefense;
				}

				if (attack != 0) {
					begin = false;
					s << " (Atk:" << attack;
					int64_t elementType = item ? item->getAbilityValue(ITEM_ABILITY_ELEMENTTYPE) : it.abilities->elementType;
					int64_t elementDamage = item ? item->getAbilityValue(ITEM_ABILITY_ELEMENTDAMAGE) : it.abilities->elementDamage;
					if (elementType != COMBAT_NONE && elementDamage != 0) {
						s << " physical + " << elementDamage << ' ' << getCombatName(static_cast<CombatType_t>(elementType));
					}
				}

				if (defense != 0 || extraDefense != 0) {
					if (begin) {
						begin = false;
						s << " (";
					} else {
						s << ", ";
					}

					s << "Def:" << defense;
					if (extraDefense != 0) {
						s << ' ' << std::showpos << extraDefense << std::noshowpos;
					}
				}
			}
		}

		bool begin = true;

		if (it.isContainer() || item && item->getContainer()) {
			uint32_t volume = 0;
			if (!item || !item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
				if (it.isContainer()) {
					volume = it.maxItems;
				} else {
					volume = item->getContainer()->capacity();
				}
			}

			if (volume != 0) {
				s << " (Vol:" << volume << ')';
			}
		}

		int32_t armor = (item ? item->getArmor() : it.armor);
		if (armor != 0) {
			s << " (Arm:" << armor;
			begin = false;
		}

		if (item && item->abilities || it.abilities) {
			for (uint8_t i = SKILL_FIRST; i <= SKILL_LAST; i++) {
				const int16_t skillValue = item ? item->getAbilityValue(skillToAbility(i)) : (it.abilities ? it.abilities->skills[i] : 0);
				if (!skillValue) {
					continue;
				}

				if (begin) {
					begin = false;
					s << " (";
				} else {
					s << ", ";
				}

				s << getSkillName(i) << ' ' << std::showpos << skillValue << std::noshowpos;
			}

			for (uint8_t i = SPECIALSKILL_FIRST; i <= SPECIALSKILL_LAST; i++) {
				const int16_t specialSkillValue = item ? item->getAbilityValue(specialSkillToAbility(i)) : (it.abilities ? it.abilities->specialSkills[i] : 0);
				if (!specialSkillValue) {
					continue;
				}

				if (begin) {
					begin = false;
					s << " (";
				} else {
					s << ", ";
				}

				s << getSpecialSkillName(i) << ' ' << std::showpos << specialSkillValue << '%' << std::noshowpos;
			}

			const int16_t magicPoints = item ? item->getAbilityValue(ITEM_ABILITY_MAGICPOINTS) : (it.abilities ? it.abilities->stats[STAT_MAGICPOINTS] : 0);
			if (magicPoints) {
				if (begin) {
					begin = false;
					s << " (";
				} else {
					s << ", ";
				}

				s << "magic level " << std::showpos << magicPoints << std::noshowpos;
			}

			const int16_t magicPointsPercent = item ? item->getAbilityValue(ITEM_ABILITY_MAGICPOINTSPERCENT) : (it.abilities ? it.abilities->statsPercent[STAT_MAGICPOINTS] : 0);
			if (magicPoints) {
				if (begin) {
					begin = false;
					s << " (";
				} else {
					s << ", ";
				}

				s << "magic level percent " << std::showpos << magicPoints << '%' << std::noshowpos;
			}

			int16_t show = item ? item->getAbilityValue(ITEM_ABILITY_ABSORBPHYSICAL) : (it.abilities ? it.abilities->absorbPercent[0] : 0);
			if (show != 0) {
				for (size_t i = 1; i < COMBAT_COUNT; ++i) {
					if (item && item->getAbilityValue(combatToAbsorb(indexToCombatType(i))) != show || it.abilities && it.abilities->absorbPercent[indexToCombatType(i)] != show) {
						show = 0;
						break;
					}
				}
			}

			if (!show) {
				bool protectionBegin = true;
				for (size_t i = 0; i < COMBAT_COUNT; ++i) {
					const int16_t absorbPercent = item ? item->getAbilityValue(combatToAbsorb(indexToCombatType(i))) : (it.abilities ? it.abilities->absorbPercent[i] : 0);
					if (absorbPercent == 0) {
						continue;
					}

					if (protectionBegin) {
						protectionBegin = false;

						if (begin) {
							begin = false;
							s << " (";
						} else {
							s << ", ";
						}

						s << "protection ";
					} else {
						s << ", ";
					}

					s << getCombatName(indexToCombatType(i)) << ' ' << std::showpos << absorbPercent << std::noshowpos << '%';
				}
			} else {
				if (begin) {
					begin = false;
					s << " (";
				} else {
					s << ", ";
				}

				s << "protection all " << std::showpos << show << std::noshowpos << '%';
			}

			show = item ? item->getAbilityValue(ITEM_ABILITY_FIELDABSORBPHYSICAL) : (it.abilities ? it.abilities->fieldAbsorbPercent[0] : 0);
			if (show != 0) {
				for (size_t i = 0; i < COMBAT_COUNT; ++i) {
					if (item && item->getAbilityValue(combatToFieldAbsorb(indexToCombatType(i))) != show || it.abilities && it.abilities->fieldAbsorbPercent[indexToCombatType(i)] != show) {
						show = 0;
						break;
					}
				}
			}

			if (!show) {
				bool tmp = true;

				for (size_t i = 0; i < COMBAT_COUNT; ++i) {
					const int16_t fieldAbsorbPercent = item ? item->getAbilityValue(combatToFieldAbsorb(indexToCombatType(i))): (it.abilities ? it.abilities->fieldAbsorbPercent[i] : 0);
					if (fieldAbsorbPercent == 0) {
						continue;
					}

					if (tmp) {
						tmp = false;

						if (begin) {
							begin = false;
							s << " (";
						} else {
							s << ", ";
						}

						s << "protection ";
					} else {
						s << ", ";
					}

					s << getCombatName(indexToCombatType(i)) << " field " << std::showpos << fieldAbsorbPercent << std::noshowpos << '%';
				}
			} else {
				if (begin) {
					begin = false;
					s << " (";
				} else {
					s << ", ";
				}

				s << "protection all fields " << std::showpos << show << std::noshowpos << '%';
			}

			int64_t speed = item ? item->getAbilityValue(ITEM_ABILITY_SPEED) : (it.abilities ? it.abilities->speed : 0);
			if (speed) {
				if (begin) {
					begin = false;
					s << " (";
				} else {
					s << ", ";
				}

				s << "speed " << std::showpos << (speed >> 1) << std::noshowpos;
			}
		}

		if (!begin) {
			s << ')';
		}
		bool found = false;
		if (hasBitSet(CONDITION_DRUNK, item ? item->getAbilityValue(ITEM_ABILITY_CONDITIONSUPPRESSIONS) : (it.abilities ? it.abilities->conditionSuppressions : 0))) {
			s << " (hard drinking)";
			found = true;
		}
		if (item ? item->getAbilityValue(ITEM_ABILITY_INVISIBLE) : (it.abilities ? it.abilities->invisible : false)) {
			s << " (invisibility)";
			found = true;
		}
		if (item ? item->getAbilityValue(ITEM_ABILITY_REGENERATION) : (it.abilities ? it.abilities->regeneration : false)) {
			s << " (faster regeneration)";
			found = true;
		}
		if (item ? item->getAbilityValue(ITEM_ABILITY_MANASHIELD) : (it.abilities ? it.abilities->manaShield : false)) {
			s << " (mana shield)";
			found = true;
		}

		if (!found) {
			if (it.isKey()) {
				s << " (Key:" << (item ? item->getActionId() : 0) << ')';
			} else if (it.isFluidContainer()) {
				if (subType > 0) {
					const std::string& itemName = items[subType].name;
					s << " of " << (!itemName.empty() ? itemName : "unknown");
				} else {
					s << ". It is empty";
				}
			} else if (it.isSplash()) {
				s << " of ";

				if (subType > 0 && !items[subType].name.empty()) {
					s << items[subType].name;
				} else {
					s << "unknown";
				}
			} else if (it.allowDistRead && (it.id < 7369 || it.id > 7371)) {
				s << ".\n";

				if (lookDistance <= 4) {
					if (item) {
						text = &item->getText();
						if (!text->empty()) {
							const std::string& writer = item->getWriter();
							if (!writer.empty()) {
								s << writer << " wrote";
								time_t date = item->getDate();
								if (date != 0) {
									s << " on " << formatDateShort(date);
								}
								s << ": ";
							} else {
								s << "You read: ";
							}
							s << *text;
						} else {
							s << "Nothing is written on it";
						}
					} else {
						s << "Nothing is written on it";
					}
				} else {
					s << "You are too far away to read it";
				}
			} else if (it.levelDoor != 0 && item) {
				uint16_t actionId = item->getActionId();
				if (actionId >= it.levelDoor) {
					s << " for level " << (actionId - it.levelDoor);
				}
			}
		}
	}

	if (it.showCharges) {
		s << " that has " << subType << " charge" << (subType != 1 ? "s" : "") << " left";
	}

	if (it.showDuration) {
		if (item && item->hasAttribute(ITEM_ATTRIBUTE_DURATION)) {
			uint32_t duration = item->getDuration() / 1000;
			s << " that will expire in ";

			if (duration >= 86400) {
				uint16_t days = duration / 86400;
				uint16_t hours = (duration % 86400) / 3600;
				s << days << " day" << (days != 1 ? "s" : "");

				if (hours > 0) {
					s << " and " << hours << " hour" << (hours != 1 ? "s" : "");
				}
			} else if (duration >= 3600) {
				uint16_t hours = duration / 3600;
				uint16_t minutes = (duration % 3600) / 60;
				s << hours << " hour" << (hours != 1 ? "s" : "");

				if (minutes > 0) {
					s << " and " << minutes << " minute" << (minutes != 1 ? "s" : "");
				}
			} else if (duration >= 60) {
				uint16_t minutes = duration / 60;
				s << minutes << " minute" << (minutes != 1 ? "s" : "");
				uint16_t seconds = duration % 60;

				if (seconds > 0) {
					s << " and " << seconds << " second" << (seconds != 1 ? "s" : "");
				}
			} else {
				s << duration << " second" << (duration != 1 ? "s" : "");
			}
		} else {
			s << " that is brand-new";
		}
	}

	if (!it.allowDistRead || (it.id >= 7369 && it.id <= 7371)) {
		s << '.';
	} else {
		if (!text && item) {
			text = &item->getText();
		}

		if (!text || text->empty()) {
			s << '.';
		}
	}

	if (it.wieldInfo != 0) {
		s << "\nIt can only be wielded properly by ";

		if (it.wieldInfo & WIELDINFO_PREMIUM) {
			s << "premium ";
		}

		if (!it.vocationString.empty()) {
			s << it.vocationString;
		} else {
			s << "players";
		}

		if (it.wieldInfo & WIELDINFO_LEVEL) {
			s << " of level " << it.minReqLevel << " or higher";
		}

		if (it.wieldInfo & WIELDINFO_MAGLV) {
			if (it.wieldInfo & WIELDINFO_LEVEL) {
				s << " and";
			} else {
				s << " of";
			}

			s << " magic level " << it.minReqMagicLevel << " or higher";
		}

		s << '.';
	}

	if (lookDistance <= 1) {
		if (item) {
			const uint32_t weight = item->getWeight();
			if (weight != 0 && it.pickupable) {
				s << '\n' << getWeightDescription(it, weight, item->getItemCount());
			}
		} else if (it.weight != 0 && it.pickupable) {
			s << '\n' << getWeightDescription(it, it.weight);
		}
	}

	if (item) {
		const std::string& specialDescription = item->getSpecialDescription();
		if (!specialDescription.empty()) {
			s << '\n' << specialDescription;
		} else if (lookDistance <= 1 && !it.description.empty()) {
			s << '\n' << it.description;
		}
	} else if (lookDistance <= 1 && !it.description.empty()) {
		s << '\n' << it.description;
	}

	if (it.allowDistRead && it.id >= 7369 && it.id <= 7371) {
		if (!text && item) {
			text = &item->getText();
		}

		if (text && !text->empty()) {
			s << '\n' << *text;
		}
	}
	return s.str();
}

std::string Item::getDescription(int32_t lookDistance) const
{
	const ItemType& it = items[id];
	return getDescription(it, lookDistance, this);
}

std::string Item::getNameDescription(const ItemType& it, const Item* item /*= nullptr*/, int32_t subType /*= -1*/, bool addArticle /*= true*/)
{
	if (item) {
		subType = item->getSubType();
	}

	std::ostringstream s;

	const std::string& name = (item ? item->getName() : it.name);
	if (!name.empty()) {
		if (it.stackable && subType > 1) {
			if (it.showCount) {
				s << subType << ' ';
			}

			s << (item ? item->getPluralName() : it.getPluralName());
		} else {
			if (addArticle) {
				const std::string& article = (item ? item->getArticle() : it.article);
				if (!article.empty()) {
					s << article << ' ';
				}
			}

			s << name;
		}
	} else {
		s << "an item of type " << it.id;
	}
	return s.str();
}

std::string Item::getNameDescription() const
{
	const ItemType& it = items[id];
	return getNameDescription(it, this);
}

std::string Item::getWeightDescription(const ItemType& it, uint32_t weight, uint32_t count /*= 1*/)
{
	std::ostringstream ss;
	if (it.stackable && count > 1 && it.showCount != 0) {
		ss << "They weigh ";
	} else {
		ss << "It weighs ";
	}

	if (weight < 10) {
		ss << "0.0" << weight;
	} else if (weight < 100) {
		ss << "0." << weight;
	} else {
		std::string weightString = std::to_string(weight);
		weightString.insert(weightString.end() - 2, '.');
		ss << weightString;
	}

	ss << " oz.";
	return ss.str();
}

std::string Item::getWeightDescription(uint32_t weight) const
{
	const ItemType& it = Item::items[id];
	return getWeightDescription(it, weight, getItemCount());
}

std::string Item::getWeightDescription() const
{
	uint32_t weight = getWeight();
	if (weight == 0) {
		return std::string();
	}
	return getWeightDescription(weight);
}

void Item::setUniqueId(uint16_t n)
{
	if (hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
		return;
	}

	if (g_game.addUniqueItem(n, this)) {
		getAttributes()->setUniqueId(n);
	}
}

bool Item::canDecay() const
{
	if (isRemoved()) {
		return false;
	}

	const ItemType& it = Item::items[id];
	if (it.decayTo < 0 || it.decayTime == 0) {
		return false;
	}

	if (hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
		return false;
	}

	return true;
}

uint32_t Item::getWorth() const
{
	switch (id) {
		case ITEM_GOLD_COIN:
			return count;

		case ITEM_PLATINUM_COIN:
			return count * 100;

		case ITEM_CRYSTAL_COIN:
			return count * 10000;

		default:
			return 0;
	}
}

LightInfo Item::getLightInfo() const
{
	const ItemType& it = items[id];
	return {it.lightLevel, it.lightColor};
}

const ItemAbilities::Ability* ItemAbilities::getExistingAbility(itemAbilityTypes type) const
{
	if (hasAbility(type)) {
		for (const Ability& ability : abilityList) {
			if (ability.type == type) {
				return &ability;
			}
		}
	}
	return nullptr;
}

ItemAbilities::Ability& ItemAbilities::getAbility(itemAbilityTypes type)
{
	if (hasAbility(type)) {
		for (Ability& ability : abilityList) {
			if (ability.type == type) {
				return ability;
			}
		}
	}
	abilityBits |= type;
	abilityList.emplace_front(type);
	return abilityList.front();
}

int64_t Item::getAbilityValue(itemAbilityTypes type) const
{
	const ItemType& it = Item::items[id];
	if ((!it.abilities && abilities) || (abilities && hasAbility(type))) {
		return getAbilityInt(type);
	}
	if (!it.abilities) {
		return 0;
	}
	switch (type) {
		case ITEM_ABILITY_HEALTHGAIN:
			return it.abilities->healthGain;
		case ITEM_ABILITY_HEALTHTICKS:
			return it.abilities->healthTicks;
		case ITEM_ABILITY_MANAGAIN:
			return it.abilities->manaGain;
		case ITEM_ABILITY_CONDITIONSUPPRESSIONS:
			return it.abilities->conditionSuppressions;
		case ITEM_ABILITY_MAXHITPOINTS:
			return it.abilities->stats[STAT_MAXHITPOINTS];
		case ITEM_ABILITY_MAXMANAPOINTS:
			return it.abilities->stats[STAT_MAXMANAPOINTS];
		case ITEM_ABILITY_MAGICPOINTS:
			return it.abilities->stats[STAT_MAGICPOINTS];
		case ITEM_ABILITY_MAXHITPOINTSPERCENT:
			return it.abilities->statsPercent[STAT_MAXHITPOINTS];
		case ITEM_ABILITY_MAXMANAPOINTSPERCENT:
			return it.abilities->statsPercent[STAT_MAXMANAPOINTS];
		case ITEM_ABILITY_MAGICPOINTSPERCENT:
			return it.abilities->statsPercent[STAT_MAGICPOINTS];
		case ITEM_ABILITY_SKILLFIST:
			return it.abilities->skills[SKILL_FIST];
		case ITEM_ABILITY_SKILLCLUB:
			return it.abilities->skills[SKILL_CLUB];
		case ITEM_ABILITY_SKILLSWORD:
			return it.abilities->skills[SKILL_SWORD];
		case ITEM_ABILITY_SKILLAXE:
			return it.abilities->skills[SKILL_AXE];
		case ITEM_ABILITY_SKILLDISTANCE:
			return it.abilities->skills[SKILL_DISTANCE];
		case ITEM_ABILITY_SKILLSHIELD:
			return it.abilities->skills[SKILL_SHIELD];
		case ITEM_ABILITY_SKILLFISHING:
			return it.abilities->skills[SKILL_FISHING];
		case ITEM_ABILITY_CRITICALHITCHANCE:
			return it.abilities->specialSkills[SPECIALSKILL_CRITICALHITCHANCE];
		case ITEM_ABILITY_CRITICALHITAMOUNT:
			return it.abilities->specialSkills[SPECIALSKILL_CRITICALHITAMOUNT];
		case ITEM_ABILITY_LIFELEECHCHANCE:
			return it.abilities->specialSkills[SPECIALSKILL_HITPOINTSLEECHCHANCE];
		case ITEM_ABILITY_LIFELEECHAMOUNT:
			return it.abilities->specialSkills[SPECIALSKILL_HITPOINTSLEECHAMOUNT];
		case ITEM_ABILITY_MANALEECHCHANCE:
			return it.abilities->specialSkills[SPECIALSKILL_MANAPOINTSLEECHCHANCE];
		case ITEM_ABILITY_MANALEECHAMOUNT:
			return it.abilities->specialSkills[SPECIALSKILL_MANAPOINTSLEECHAMOUNT];
		case ITEM_ABILITY_SPEED:
			return it.abilities->speed;
		case ITEM_ABILITY_ABSORBPHYSICAL:
			return it.abilities->absorbPercent[0];
		case ITEM_ABILITY_ABSORBENERGY:
			return it.abilities->absorbPercent[1];
		case ITEM_ABILITY_ABSORBEARTH:
			return it.abilities->absorbPercent[2];
		case ITEM_ABILITY_ABSORBFIRE:
			return it.abilities->absorbPercent[3];
		case ITEM_ABILITY_ABSORBWATER:
			return it.abilities->absorbPercent[4];
		case ITEM_ABILITY_ABSORBICE:
			return it.abilities->absorbPercent[5];
		case ITEM_ABILITY_ABSORBHOLY:
			return it.abilities->absorbPercent[6];
		case ITEM_ABILITY_ABSORBDEATH:
			return it.abilities->absorbPercent[7];
		case ITEM_ABILITY_FIELDABSORBPHYSICAL:
			return it.abilities->fieldAbsorbPercent[0];
		case ITEM_ABILITY_FIELDABSORBENERGY:
			return it.abilities->fieldAbsorbPercent[1];
		case ITEM_ABILITY_FIELDABSORBEARTH:
			return it.abilities->fieldAbsorbPercent[2];
		case ITEM_ABILITY_FIELDABSORBFIRE:
			return it.abilities->fieldAbsorbPercent[3];
		case ITEM_ABILITY_FIELDABSORBWATER:
			return it.abilities->fieldAbsorbPercent[4];
		case ITEM_ABILITY_FIELDABSORBICE:
			return it.abilities->fieldAbsorbPercent[5];
		case ITEM_ABILITY_FIELDABSORBHOLY:
			return it.abilities->fieldAbsorbPercent[6];
		case ITEM_ABILITY_FIELDABSORBDEATH:
			return it.abilities->fieldAbsorbPercent[7];
		case ITEM_ABILITY_ELEMENTTYPE:
			return it.abilities->elementType;
		case ITEM_ABILITY_ELEMENTDAMAGE:
			return it.abilities->elementDamage;
		case ITEM_ABILITY_MANASHIELD:
			return static_cast<int64_t>(it.abilities->manaShield);
		case ITEM_ABILITY_INVISIBLE:
			return static_cast<int64_t>(it.abilities->invisible);
		case ITEM_ABILITY_REGENERATION:
			return static_cast<int64_t>(it.abilities->regeneration);
		default:
			return 0;
	}
}

void ItemAbilities::setAbilityInt(itemAbilityTypes type, int64_t value)
{
	getAbility(type).integer = value;
}

void ItemAbilities::removeAbility(itemAbilityTypes type)
{
	if (!hasAbility(type)) {
		return;
	}

	auto prev_it = abilityList.cbegin();
	if ((*prev_it).type == type) {
		abilityList.pop_front();
	} else {
		auto it = prev_it, end = abilityList.cend();
		while (++it != end) {
			if ((*it).type == type) {
				abilityList.erase_after(prev_it);
				break;
			}
			prev_it = it;
		}
	}
	abilityBits &= ~type;
}

std::string ItemAttributes::emptyString;
int64_t ItemAttributes::emptyInt;
double ItemAttributes::emptyDouble;
bool ItemAttributes::emptyBool;

const std::string& ItemAttributes::getStrAttr(itemAttrTypes type) const
{
	if (!isStrAttrType(type)) {
		return emptyString;
	}

	const Attribute* attr = getExistingAttr(type);
	if (!attr) {
		return emptyString;
	}
	return *attr->value.string;
}

void ItemAttributes::setStrAttr(itemAttrTypes type, const std::string& value)
{
	if (!isStrAttrType(type)) {
		return;
	}

	if (value.empty()) {
		return;
	}

	Attribute& attr = getAttr(type);
	delete attr.value.string;
	attr.value.string = new std::string(value);
}

void ItemAttributes::removeAttribute(itemAttrTypes type)
{
	if (!hasAttribute(type)) {
		return;
	}

	auto prev_it = attributes.cbegin();
	if ((*prev_it).type == type) {
		attributes.pop_front();
	} else {
		auto it = prev_it, end = attributes.cend();
		while (++it != end) {
			if ((*it).type == type) {
				attributes.erase_after(prev_it);
				break;
			}
			prev_it = it;
		}
	}
	attributeBits &= ~type;
}

int64_t ItemAttributes::getIntAttr(itemAttrTypes type) const
{
	if (!isIntAttrType(type)) {
		return 0;
	}

	const Attribute* attr = getExistingAttr(type);
	if (!attr) {
		return 0;
	}
	return attr->value.integer;
}

void ItemAttributes::setIntAttr(itemAttrTypes type, int64_t value)
{
	if (!isIntAttrType(type)) {
		return;
	}

	getAttr(type).value.integer = value;
}

void ItemAttributes::increaseIntAttr(itemAttrTypes type, int64_t value)
{
	if (!isIntAttrType(type)) {
		return;
	}

	getAttr(type).value.integer += value;
}

const ItemAttributes::Attribute* ItemAttributes::getExistingAttr(itemAttrTypes type) const
{
	if (hasAttribute(type)) {
		for (const Attribute& attribute : attributes) {
			if (attribute.type == type) {
				return &attribute;
			}
		}
	}
	return nullptr;
}

ItemAttributes::Attribute& ItemAttributes::getAttr(itemAttrTypes type)
{
	if (hasAttribute(type)) {
		for (Attribute& attribute : attributes) {
			if (attribute.type == type) {
				return attribute;
			}
		}
	}

	attributeBits |= type;
	attributes.emplace_front(type);
	return attributes.front();
}

void Item::startDecaying()
{
	g_game.startDecay(this);
}

bool Item::hasMarketAttributes() const
{
	if (attributes == nullptr) {
		return true;
	}

	for (const auto& attr : attributes->getList()) {
		if (attr.type == ITEM_ATTRIBUTE_CHARGES) {
			uint16_t charges = static_cast<uint16_t>(attr.value.integer);
			if (charges != items[id].charges) {
				return false;
			}
		} else if (attr.type == ITEM_ATTRIBUTE_DURATION) {
			uint32_t duration = static_cast<uint32_t>(attr.value.integer);
			if (duration != getDefaultDuration()) {
				return false;
			}
		} else {
			return false;
		}
	}
	return true;
}

template<>
const std::string& ItemAttributes::CustomAttribute::get<std::string>() {
	if (value.type() == typeid(std::string)) {
		return boost::get<std::string>(value);
	}

	return emptyString;
}

template<>
const int64_t& ItemAttributes::CustomAttribute::get<int64_t>() {
	if (value.type() == typeid(int64_t)) {
		return boost::get<int64_t>(value);
	}

	return emptyInt;
}

template<>
const double& ItemAttributes::CustomAttribute::get<double>() {
	if (value.type() == typeid(double)) {
		return boost::get<double>(value);
	}

	return emptyDouble;
}

template<>
const bool& ItemAttributes::CustomAttribute::get<bool>() {
	if (value.type() == typeid(bool)) {
		return boost::get<bool>(value);
	}

	return emptyBool;
}
