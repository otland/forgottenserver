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

#include "otpch.h"

#include "definitions.h"
#include "item.h"
#include "container.h"
#include "teleport.h"
#include "trashholder.h"
#include "mailbox.h"
#include "house.h"
#include "game.h"
#include "luascript.h"
#include "configmanager.h"
#include "weapons.h"
#include "beds.h"

#include "actions.h"
#include "combat.h"

#include <iostream>
#include <iomanip>

extern Game g_game;
extern ConfigManager g_config;
extern Weapons* g_weapons;

Items Item::items;

Item* Item::CreateItem(const uint16_t _type, uint16_t _count /*= 0*/)
{
	Item* newItem = NULL;

	const ItemType& it = Item::items[_type];

	if (it.group == ITEM_GROUP_DEPRECATED) {
		return NULL;
	}

	if (it.stackable && _count == 0) {
		_count = 1;
	}

	if (it.id != 0) {
		if (it.isDepot()) {
			newItem = new DepotLocker(_type);
		} else if (it.isContainer()) {
			newItem = new Container(_type);
		} else if (it.isTeleport()) {
			newItem = new Teleport(_type);
		} else if (it.isMagicField()) {
			newItem = new MagicField(_type);
		} else if (it.isDoor()) {
			newItem = new Door(_type);
		} else if (it.isTrashHolder()) {
			newItem = new TrashHolder(_type, it.magicEffect);
		} else if (it.isMailbox()) {
			newItem = new Mailbox(_type);
		} else if (it.isBed()) {
			newItem = new BedItem(_type);
		} else if (it.id >= 2210 && it.id <= 2212) {
			newItem = new Item(_type - 3, _count);
		} else if (it.id == 2215 || it.id == 2216) {
			newItem = new Item(_type - 2, _count);
		} else if (it.id >= 2202 && it.id <= 2206) {
			newItem = new Item(_type - 37, _count);
		} else if (it.id == 2640) {
			newItem = new Item(6132, _count);
		} else if (it.id == 6301) {
			newItem = new Item(6300, _count);
		} else if (it.id == 18528) {
			newItem = new Item(18408, _count);
		} else {
			newItem = new Item(_type, _count);
		}

		newItem->useThing2();
	}

	return newItem;
}

Item* Item::CreateItem(PropStream& propStream)
{
	uint16_t _id;

	if (!propStream.GET_USHORT(_id)) {
		return NULL;
	}

	switch (_id) {
		case ITEM_FIREFIELD_PVP_FULL:
			_id = ITEM_FIREFIELD_PERSISTENT_FULL;
			break;

		case ITEM_FIREFIELD_PVP_MEDIUM:
			_id = ITEM_FIREFIELD_PERSISTENT_MEDIUM;
			break;

		case ITEM_FIREFIELD_PVP_SMALL:
			_id = ITEM_FIREFIELD_PERSISTENT_SMALL;
			break;

		case ITEM_ENERGYFIELD_PVP:
			_id = ITEM_ENERGYFIELD_PERSISTENT;
			break;

		case ITEM_POISONFIELD_PVP:
			_id = ITEM_POISONFIELD_PERSISTENT;
			break;

		case ITEM_MAGICWALL:
			_id = ITEM_MAGICWALL_PERSISTENT;
			break;

		case ITEM_WILDGROWTH:
			_id = ITEM_WILDGROWTH_PERSISTENT;
			break;

		default:
			break;
	}

	return Item::CreateItem(_id, 0);
}

Item::Item(const uint16_t _type, uint16_t _count /*= 0*/)
{
	id = _type;
	attributes = NULL;

	const ItemType& it = items[id];

	setItemCount(1);

	if (it.isFluidContainer() || it.isSplash()) {
		setFluidType(_count);
	} else if (it.stackable) {
		if (_count != 0) {
			setItemCount(_count);
		} else if (it.charges != 0) {
			setItemCount(it.charges);
		}
	} else if (it.charges != 0) {
		if (_count != 0) {
			setCharges(_count);
		} else {
			setCharges(it.charges);
		}
	}

	loadedFromMap = false;
	setDefaultDuration();
}

Item::Item(const Item& i) :
	Thing()
{
	//std::cout << "Item copy constructor " << this << std::endl;
	id = i.id;
	count = i.count;
	loadedFromMap = i.loadedFromMap;

	if (i.attributes) {
		attributes = new ItemAttributes(*i.attributes);
	} else {
		attributes = NULL;
	}
}

Item* Item::clone() const
{
	Item* _item = Item::CreateItem(id, count);
	if (attributes) {
		_item->attributes = new ItemAttributes(*attributes);
	}
	return _item;
}

void Item::copyAttributes(Item* item)
{
	if (item->attributes) {
		attributes = new ItemAttributes(*item->attributes);
	}

	removeAttribute(ATTR_ITEM_DECAYING);
	removeAttribute(ATTR_ITEM_DURATION);
}

Item::~Item()
{
	//std::cout << "Item destructor " << this << std::endl;
	delete attributes;
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

	if (getUniqueId() != 0) {
		ScriptEnvironment::removeUniqueThing(this);
	}
}

void Item::setID(uint16_t newid)
{
	const ItemType& prevIt = Item::items[id];
	id = newid;

	const ItemType& it = Item::items[newid];
	uint32_t newDuration = it.decayTime * 1000;

	if (newDuration == 0 && !it.stopTime && it.decayTo == -1) {
		removeAttribute(ATTR_ITEM_DECAYING);
		removeAttribute(ATTR_ITEM_DURATION);
	}

	if (hasAttribute(ATTR_ITEM_CORPSEOWNER)) {
		removeAttribute(ATTR_ITEM_CORPSEOWNER);
	}

	if (newDuration > 0 && (!prevIt.stopTime || !hasAttribute(ATTR_ITEM_DURATION))) {
		setDecaying(DECAYING_FALSE);
		setDuration(newDuration);
	}
}

bool Item::hasSubType() const
{
	const ItemType& it = items[id];
	return it.hasSubType();
}

uint16_t Item::getSubType() const
{
	const ItemType& it = items[getID()];

	if (it.isFluidContainer() || it.isSplash()) {
		return getFluidType();
	} else if (it.stackable) {
		return getItemCount();
	} else if (it.charges != 0) {
		return getCharges();
	}

	return getItemCount();
}

Player* Item::getHoldingPlayer()
{
	Cylinder* p = getParent();

	while (p) {
		if (p->getCreature()) {
			return p->getCreature()->getPlayer();
		}

		p = p->getParent();
	}

	return NULL;
}

const Player* Item::getHoldingPlayer() const
{
	return const_cast<Item*>(this)->getHoldingPlayer();
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
		case ATTR_COUNT: {
			uint8_t _count = 0;

			if (!propStream.GET_UCHAR(_count)) {
				return ATTR_READ_ERROR;
			}

			setSubType(_count);
			break;
		}

		case ATTR_ACTION_ID: {
			uint16_t _actionid = 0;

			if (!propStream.GET_USHORT(_actionid)) {
				return ATTR_READ_ERROR;
			}

			setActionId(_actionid);
			break;
		}

		case ATTR_UNIQUE_ID: {
			uint16_t _uniqueid;

			if (!propStream.GET_USHORT(_uniqueid)) {
				return ATTR_READ_ERROR;
			}

			setUniqueId(_uniqueid);
			break;
		}

		case ATTR_TEXT: {
			std::string _text;

			if (!propStream.GET_STRING(_text)) {
				return ATTR_READ_ERROR;
			}

			setText(_text);
			break;
		}

		case ATTR_WRITTENDATE: {
			uint32_t _writtenDate;

			if (!propStream.GET_ULONG(_writtenDate)) {
				return ATTR_READ_ERROR;
			}

			setDate(_writtenDate);
			break;
		}

		case ATTR_WRITTENBY: {
			std::string _writer;

			if (!propStream.GET_STRING(_writer)) {
				return ATTR_READ_ERROR;
			}

			setWriter(_writer);
			break;
		}

		case ATTR_DESC: {
			std::string _text;

			if (!propStream.GET_STRING(_text)) {
				return ATTR_READ_ERROR;
			}

			setSpecialDescription(_text);
			break;
		}

		case ATTR_RUNE_CHARGES: {
			uint8_t _charges = 1;

			if (!propStream.GET_UCHAR(_charges)) {
				return ATTR_READ_ERROR;
			}

			setSubType(_charges);
			break;
		}

		case ATTR_CHARGES: {
			uint16_t _charges = 1;

			if (!propStream.GET_USHORT(_charges)) {
				return ATTR_READ_ERROR;
			}

			setSubType(_charges);
			break;
		}

		case ATTR_DURATION: {
			uint32_t duration = 0;

			if (!propStream.GET_ULONG(duration)) {
				return ATTR_READ_ERROR;
			}

			if (((int32_t)duration) < 0) {
				duration = 0;
			}

			setDuration(duration);
			break;
		}

		case ATTR_DECAYING_STATE: {
			uint8_t state = 0;

			if (!propStream.GET_UCHAR(state)) {
				return ATTR_READ_ERROR;
			}

			if (state != DECAYING_FALSE) {
				setDecaying(DECAYING_PENDING);
			}

			break;
		}

		//these should be handled through derived classes
		//If these are called then something has changed in the items.xml since the map was saved
		//just read the values

		//Depot class
		case ATTR_DEPOT_ID: {
			uint16_t _depotId;

			if (!propStream.GET_USHORT(_depotId)) {
				return ATTR_READ_ERROR;
			}

			return ATTR_READ_CONTINUE;
		}

		//Door class
		case ATTR_HOUSEDOORID: {
			uint8_t _doorId;

			if (!propStream.GET_UCHAR(_doorId)) {
				return ATTR_READ_ERROR;
			}

			return ATTR_READ_CONTINUE;
		}

		//Bed class
		case ATTR_SLEEPERGUID: {
			uint32_t _guid;

			if (!propStream.GET_ULONG(_guid)) {
				return ATTR_READ_ERROR;
			}

			return ATTR_READ_CONTINUE;
		}

		case ATTR_SLEEPSTART: {
			uint32_t sleep_start;

			if (!propStream.GET_ULONG(sleep_start)) {
				return ATTR_READ_ERROR;
			}

			return ATTR_READ_CONTINUE;
		}

		//Teleport class
		case ATTR_TELE_DEST: {
			TeleportDest* tele_dest;

			if (!propStream.GET_STRUCT(tele_dest)) {
				return ATTR_READ_ERROR;
			}

			return ATTR_READ_CONTINUE;
		}

		//Container class
		case ATTR_CONTAINER_ITEMS: {
			uint32_t count;

			if (!propStream.GET_ULONG(count)) {
				return ATTR_READ_ERROR;
			}

			return ATTR_READ_ERROR;
		}

		default:
			return ATTR_READ_ERROR;
	}

	return ATTR_READ_CONTINUE;
}

bool Item::unserializeAttr(PropStream& propStream)
{
	uint8_t attr_type;

	while (propStream.GET_UCHAR(attr_type) && attr_type != 0) {
		Attr_ReadValue ret = readAttr((AttrTypes_t)attr_type, propStream);

		if (ret == ATTR_READ_ERROR) {
			return false;
		} else if (ret == ATTR_READ_END) {
			return true;
		}
	}

	return true;
}

bool Item::unserializeItemNode(FileLoader& f, NODE node, PropStream& propStream)
{
	return unserializeAttr(propStream);
}

bool Item::serializeAttr(PropWriteStream& propWriteStream) const
{
	if (isStackable() || isFluidContainer() || isSplash()) {
		uint8_t _count = getSubType();
		propWriteStream.ADD_UCHAR(ATTR_COUNT);
		propWriteStream.ADD_UCHAR(_count);
	}

	if (hasCharges()) {
		uint16_t _count = getCharges();
		propWriteStream.ADD_UCHAR(ATTR_CHARGES);
		propWriteStream.ADD_USHORT(_count);
	}

	if (!isNotMoveable()) {
		uint16_t _actionId = getActionId();

		if (_actionId) {
			propWriteStream.ADD_UCHAR(ATTR_ACTION_ID);
			propWriteStream.ADD_USHORT(_actionId);
		}
	}

	const std::string& _text = getText();

	if (_text.length() > 0) {
		propWriteStream.ADD_UCHAR(ATTR_TEXT);
		propWriteStream.ADD_STRING(_text);
	}

	const time_t _writtenDate = getDate();

	if (_writtenDate > 0) {
		propWriteStream.ADD_UCHAR(ATTR_WRITTENDATE);
		propWriteStream.ADD_ULONG(_writtenDate);
	}

	const std::string& _writer = getWriter();

	if (_writer.length() > 0) {
		propWriteStream.ADD_UCHAR(ATTR_WRITTENBY);
		propWriteStream.ADD_STRING(_writer);
	}

	const std::string& _specialDesc = getSpecialDescription();

	if (_specialDesc.length() > 0) {
		propWriteStream.ADD_UCHAR(ATTR_DESC);
		propWriteStream.ADD_STRING(_specialDesc);
	}

	if (hasAttribute(ATTR_ITEM_DURATION)) {
		uint32_t duration = getDuration();
		propWriteStream.ADD_UCHAR(ATTR_DURATION);
		propWriteStream.ADD_ULONG(duration);
	}

	ItemDecayState_t decayState = getDecaying();

	if (decayState == DECAYING_TRUE || decayState == DECAYING_PENDING) {
		propWriteStream.ADD_UCHAR(ATTR_DECAYING_STATE);
		propWriteStream.ADD_UCHAR(decayState);
	}

	return true;
}

bool Item::hasProperty(enum ITEMPROPERTY prop) const
{
	const ItemType& it = items[id];

	switch (prop) {
		case BLOCKSOLID:

			if (it.blockSolid) {
				return true;
			}

			break;

		case MOVEABLE:

			if (it.moveable && getUniqueId() == 0) {
				return true;
			}

			break;

		case HASHEIGHT:

			if (it.hasHeight) {
				return true;
			}

			break;

		case BLOCKPROJECTILE:

			if (it.blockProjectile) {
				return true;
			}

			break;

		case BLOCKPATH:

			if (it.blockPathFind) {
				return true;
			}

			break;

		case ISVERTICAL:

			if (it.isVertical) {
				return true;
			}

			break;

		case ISHORIZONTAL:

			if (it.isHorizontal) {
				return true;
			}

			break;

		case IMMOVABLEBLOCKSOLID:

			if (it.blockSolid && (!it.moveable || getUniqueId() != 0)) {
				return true;
			}

			break;

		case IMMOVABLEBLOCKPATH:

			if (it.blockPathFind && (!it.moveable || getUniqueId() != 0)) {
				return true;
			}

			break;

		case SUPPORTHANGABLE:

			if (it.isHorizontal || it.isVertical) {
				return true;
			}

			break;

		case IMMOVABLENOFIELDBLOCKPATH:

			if (!it.isMagicField() && it.blockPathFind && (!it.moveable || getUniqueId() != 0)) {
				return true;
			}

			break;

		case NOFIELDBLOCKPATH:

			if (!it.isMagicField() && it.blockPathFind) {
				return true;
			}

			break;

		default:
			return false;
			break;
	}

	return false;
}

double Item::getWeight() const
{
	if (isStackable()) {
		return items[id].weight * std::max<int32_t>(1, getItemCount());
	}

	return items[id].weight;
}

std::string Item::getDescription(const ItemType& it, int32_t lookDistance,
                                 const Item* item /*= NULL*/, int32_t subType /*= -1*/, bool addArticle /*= true*/)
{
	std::ostringstream s;
	s << getNameDescription(it, item, subType, addArticle);

	if (item) {
		subType = item->getSubType();
	}

	if (it.isRune()) {
		if (!it.runeSpellName.empty()) {
			s << " (\"" << it.runeSpellName << "\")";
		}

		if (it.runeLevel > 0 || it.runeMagLevel > 0) {
			int32_t tmpSubType = subType;

			if (item) {
				tmpSubType = item->getSubType();
			}

			s << ". " << (it.stackable && tmpSubType > 1 ? "They" : "It") << " can only be used with";

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
	} else if (it.weaponType != WEAPON_NONE) {
		if (it.weaponType == WEAPON_DIST && it.ammoType != AMMO_NONE) {
			s << " (Range:" << it.shootRange;

			if (it.attack != 0) {
				s << ", Atk " << std::showpos << it.attack << std::noshowpos;
			}

			if (it.hitChance != 0) {
				s << ", Hit% " << std::showpos << it.hitChance << std::noshowpos;
			}

			s << ")";
		} else if (it.weaponType != WEAPON_AMMO) {
			bool begin = true;

			if (it.attack != 0) {
				begin = false;
				s << " (Atk:" << it.attack;

				if (it.abilities && it.abilities->elementType != COMBAT_NONE && it.abilities->elementDamage != 0) {
					s << " physical + " << it.abilities->elementDamage << " " << getCombatName(it.abilities->elementType);
				}
			}

			if (it.defense != 0 || it.extraDefense != 0) {
				if (begin) {
					begin = false;
					s << " (";
				} else {
					s << ", ";
				}

				s << "Def:" << it.defense;

				if (it.extraDefense != 0 || (item && item->getExtraDefense() != 0)) {
					s << " " << std::showpos << it.extraDefense << std::noshowpos;
				}
			}

			if (it.abilities) {
				for (uint16_t i = SKILL_FIRST; i <= SKILL_LAST; i++) {
					if (!it.abilities->skills[i]) {
						continue;
					}

					if (begin) {
						begin = false;
						s << " (";
					} else {
						s << ", ";
					}

					s << getSkillName(i) << " " << std::showpos << it.abilities->skills[i] << std::noshowpos;
				}

				if (it.abilities->stats[STAT_MAGICPOINTS]) {
					if (begin) {
						begin = false;
						s << " (";
					} else {
						s << ", ";
					}

					s << "magic level " << std::showpos << it.abilities->stats[STAT_MAGICPOINTS] << std::noshowpos;
				}

				int32_t show = it.abilities->absorbPercent[COMBAT_FIRST];

				for (uint32_t i = (COMBAT_FIRST + 1); i <= COMBAT_COUNT; ++i) {
					if (it.abilities->absorbPercent[i] == show) {
						continue;
					}

					show = 0;
					break;
				}

				if (!show) {
					bool tmp = true;

					for (uint32_t i = COMBAT_FIRST; i <= COMBAT_COUNT; i++) {
						if (!it.abilities->absorbPercent[i]) {
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

						s << getCombatName(indexToCombatType(i)) << " " << std::showpos << it.abilities->absorbPercent[i] << std::noshowpos << "%";
					}
				} else {
					if (begin) {
						begin = false;
						s << " (";
					} else {
						s << ", ";
					}

					s << "protection all " << std::showpos << show << std::noshowpos << "%";
				}

				if (it.abilities->speed) {
					if (begin) {
						begin = false;
						s << " (";
					} else {
						s << ", ";
					}

					s << "speed " << std::showpos << (int32_t)(it.abilities->speed / 2) << std::noshowpos;
				}
			}

			if (!begin) {
				s << ")";
			}
		}
	} else if (it.armor || (item && item->getArmor()) || it.showAttributes) {
		int32_t tmp = it.armor;

		if (item) {
			tmp = item->getArmor();
		}

		bool begin = true;

		if (tmp != 0) {
			s << " (Arm:" << tmp;
			begin = false;
		}

		if (it.abilities) {
			for (uint16_t i = SKILL_FIRST; i <= SKILL_LAST; i++) {
				if (!it.abilities->skills[i]) {
					continue;
				}

				if (begin) {
					begin = false;
					s << " (";
				} else {
					s << ", ";
				}

				s << getSkillName(i) << " " << std::showpos << it.abilities->skills[i] << std::noshowpos;
			}

			if (it.abilities->stats[STAT_MAGICPOINTS]) {
				if (begin) {
					begin = false;
					s << " (";
				} else {
					s << ", ";
				}

				s << "magic level " << std::showpos << it.abilities->stats[STAT_MAGICPOINTS] << std::noshowpos;
			}

			int32_t show = it.abilities->absorbPercent[COMBAT_FIRST];

			for (uint32_t i = (COMBAT_FIRST + 1); i <= COMBAT_COUNT; ++i) {
				if (it.abilities->absorbPercent[i] == show) {
					continue;
				}

				show = 0;
				break;
			}

			if (!show) {
				bool protectionBegin = true;
				for (uint32_t i = COMBAT_FIRST; i <= COMBAT_COUNT; i++) {
					if (!it.abilities->absorbPercent[i]) {
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

					s << getCombatName(indexToCombatType(i)) << " " << std::showpos << it.abilities->absorbPercent[i] << std::noshowpos << "%";
				}
			} else {
				if (begin) {
					begin = false;
					s << " (";
				} else {
					s << ", ";
				}

				s << "protection all " << std::showpos << show << std::noshowpos << "%";
			}

			if (it.abilities->speed) {
				if (begin) {
					begin = false;
					s << " (";
				} else {
					s << ", ";
				}

				s << "speed " << std::showpos << (int32_t)(it.abilities->speed / 2) << std::noshowpos;
			}
		}

		if (!begin) {
			s << ")";
		}
	} else if (it.isContainer()) {
		s << " (Vol:" << (int32_t)it.maxItems << ")";
	} else {
		bool found = true;

		if (it.abilities) {
			if (it.abilities->speed > 0) {
				s << " (speed " << std::showpos << (it.abilities->speed / 2) << std::noshowpos << ")";
			} else if (it.abilities && hasBitSet(CONDITION_DRUNK, it.abilities->conditionSuppressions)) {
				s << " (hard drinking)";
			} else if (it.abilities->invisible) {
				s << " (invisibility)";
			} else if (it.abilities->regeneration) {
				s << " (faster regeneration)";
			} else if (it.abilities->manaShield) {
				s << " (mana shield)";
			} else {
				found = false;
			}
		} else {
			found = false;
		}

		if (!found) {
			if (it.isKey()) {
				s << " (Key:" << (item ? (int32_t)item->getActionId() : 0) << ")";
			} else if (it.isFluidContainer()) {
				if (subType > 0) {
					const std::string& itemName = items[subType].name;
					s << " of " << (itemName.length() ? itemName : "unknown");
				} else {
					s << ". It is empty";
				}
			} else if (it.isSplash()) {
				s << " of ";

				if (subType > 0 && items[subType].name.length()) {
					s << items[subType].name;
				} else {
					s << "unknown";
				}
			} else if (it.allowDistRead && it.id != 7369 && it.id != 7370 && it.id != 7371) {
				s << "." << std::endl;

				if (lookDistance <= 4) {
					if (item && item->getText() != "") {
						if (item->getWriter().length()) {
							s << item->getWriter() << " wrote";
							time_t date = item->getDate();

							if (date > 0) {
								s << " on " << formatDateShort(date);
							}

							s << ": ";
						} else {
							s << "You read: ";
						}

						std::string outtext;

						if (utf8ToLatin1(item->getText().c_str(), outtext)) {
							s << outtext;
						} else {
							s << item->getText();
						}
					} else {
						s << "Nothing is written on it";
					}
				} else {
					s << "You are too far away to read it";
				}
			} else if (it.levelDoor && item && item->getActionId() >= (int32_t)it.levelDoor) {
				s << " for level " << item->getActionId() - it.levelDoor;
			}
		}
	}

	if (it.showCharges) {
		s << " that has " << subType << " charge" << (subType != 1 ? "s" : "") << " left";
	}

	if (it.showDuration) {
		if (item && item->hasAttribute(ATTR_ITEM_DURATION)) {
			int32_t duration = item->getDuration() / 1000;
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

	if (!it.allowDistRead || item->getText() == "" || (it.id >= 7369 && it.id <= 7371)) {
		s << ".";
	}

	if (it.wieldInfo != 0) {
		s << std::endl << "It can only be wielded properly by ";

		if (it.wieldInfo & WIELDINFO_PREMIUM) {
			s << "premium ";
		}

		if (it.wieldInfo & WIELDINFO_VOCREQ) {
			s << it.vocationString;
		} else {
			s << "players";
		}

		if (it.wieldInfo & WIELDINFO_LEVEL) {
			s << " of level " << (int32_t)it.minReqLevel << " or higher";
		}

		if (it.wieldInfo & WIELDINFO_MAGLV) {
			if (it.wieldInfo & WIELDINFO_LEVEL) {
				s << " and";
			} else {
				s << " of";
			}

			s << " magic level " << (int32_t)it.minReqMagicLevel << " or higher";
		}

		s << ".";
	}

	if (lookDistance <= 1) {
		double weight = (item == NULL ? it.weight : item->getWeight());

		if (weight > 0 && it.pickupable) {
			int32_t count = weight / it.weight;
			s << std::endl << getWeightDescription(it, weight, count);
		}
	}

	if (item && item->getSpecialDescription() != "") {
		s << std::endl << item->getSpecialDescription().c_str();
	} else if (it.description.length() && lookDistance <= 1) {
		s << std::endl << it.description;
	}

	if (it.allowDistRead && it.id >= 7369 && it.id <= 7371 && item->getText() != "") {
		s << std::endl << item->getText();
	}

	return s.str();
}

std::string Item::getDescription(int32_t lookDistance) const
{
	const ItemType& it = items[id];
	return getDescription(it, lookDistance, this);
}

std::string Item::getNameDescription(const ItemType& it, const Item* item /*= NULL*/, int32_t subType /*= -1*/, bool addArticle /*= true*/)
{
	if (item) {
		subType = item->getSubType();
	}

	std::ostringstream s;

	if (it.name.length()) {
		if (it.stackable && subType > 1) {
			if (it.showCount) {
				s << subType << " ";
			}

			s << it.getPluralName();
		} else {
			if (addArticle && !it.article.empty()) {
				s << it.article << " ";
			}

			s << it.name;
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

std::string Item::getWeightDescription(const ItemType& it, double weight, uint32_t _count /*= 1*/)
{
	std::ostringstream ss;

	if (it.stackable && _count > 1 && it.showCount != 0) {
		ss << "They weigh " << std::fixed << std::setprecision(2) << weight << " oz.";
	} else {
		ss << "It weighs " << std::fixed << std::setprecision(2) << weight << " oz.";
	}

	return ss.str();
}

std::string Item::getWeightDescription(double weight) const
{
	const ItemType& it = Item::items[id];
	return getWeightDescription(it, weight, getItemCount());
}

std::string Item::getWeightDescription() const
{
	double weight = getWeight();
	return (weight > 0 ? getWeightDescription(weight) : "");
}

void Item::setUniqueId(uint16_t n)
{
	if (getUniqueId() != 0) {
		return;
	}

	getAttributes()->setUniqueId(n);
	ScriptEnvironment::addUniqueThing(this);
}

bool Item::canDecay()
{
	if (isRemoved()) {
		return false;
	}

	if (getUniqueId() != 0) {
		return false;
	}

	const ItemType& it = Item::items[id];

	if (it.decayTo == -1 || it.decayTime == 0) {
		return false;
	}

	return true;
}

int32_t Item::getWorth() const
{
	switch (getID()) {
		case ITEM_COINS_GOLD:
			return getItemCount();

		case ITEM_COINS_PLATINUM:
			return getItemCount() * 100;

		case ITEM_COINS_CRYSTAL:
			return getItemCount() * 10000;

		default:
			return 0;
	}
}

void Item::getLight(LightInfo& lightInfo)
{
	const ItemType& it = items[id];
	lightInfo.color = it.lightColor;
	lightInfo.level = it.lightLevel;
}

std::string ItemAttributes::emptyString("");

const std::string& ItemAttributes::getStrAttr(itemAttrTypes type) const
{
	if (!validateStrAttrType(type)) {
		return emptyString;
	}

	Attribute* attr = getAttrConst(type);

	if (attr) {
		return *(std::string*)attr->value;
	} else {
		return emptyString;
	}
}

void ItemAttributes::setStrAttr(itemAttrTypes type, const std::string& value)
{
	if (!validateStrAttrType(type)) {
		return;
	}

	if (value.length() == 0) {
		return;
	}

	Attribute* attr = getAttr(type);

	if (attr) {
		if (attr->value) {
			delete (std::string*)attr->value;
		}

		attr->value = (void*)new std::string(value);
	}
}

bool ItemAttributes::hasAttribute(itemAttrTypes type) const
{
	if (!validateIntAttrType(type)) {
		return false;
	}

	Attribute* attr = getAttrConst(type);

	if (attr) {
		return true;
	}

	return false;
}

void ItemAttributes::removeAttribute(itemAttrTypes type)
{
	//check if we have it
	if ((type & m_attributes) != 0) {
		//go trough the linked list until find it
		Attribute* prevAttr = NULL;
		Attribute* curAttr = m_firstAttr;

		while (curAttr != NULL) {
			if (curAttr->type == type) {
				//found so remove it from the linked list
				if (prevAttr) {
					prevAttr->next = curAttr->next;
				} else {
					m_firstAttr = curAttr->next;
				}

				//remove it from flags
				m_attributes = m_attributes & ~type;

				//delete string if it is string type
				if (validateStrAttrType(type)) {
					delete (std::string*)curAttr->value;
				}

				//finally delete the attribute and return
				delete curAttr;
				return;
			}

			//advance in the linked list
			prevAttr = curAttr;
			curAttr = curAttr->next;
		}
	}
}

uint32_t ItemAttributes::getIntAttr(itemAttrTypes type) const
{
	if (!validateIntAttrType(type)) {
		return 0;
	}

	Attribute* attr = getAttrConst(type);

	if (attr) {
		return static_cast<uint32_t>(0xFFFFFFFF & reinterpret_cast<ptrdiff_t>(attr->value));
	} else {
		return 0;
	}
}

void ItemAttributes::setIntAttr(itemAttrTypes type, int32_t value)
{
	if (!validateIntAttrType(type)) {
		return;
	}

	Attribute* attr = getAttr(type);

	if (attr) {
		attr->value = reinterpret_cast<void*>(static_cast<ptrdiff_t>(value));
	}
}

void ItemAttributes::increaseIntAttr(itemAttrTypes type, int32_t value)
{
	if (!validateIntAttrType(type)) {
		return;
	}

	Attribute* attr = getAttr(type);

	if (attr) {
		attr->value = reinterpret_cast<void*>(static_cast<ptrdiff_t>(static_cast<uint32_t>(0xFFFFFFFF & reinterpret_cast<ptrdiff_t>(attr->value)) + value));
	}
}

bool ItemAttributes::validateIntAttrType(itemAttrTypes type)
{
	switch (type) {
		case ATTR_ITEM_ACTIONID:
		case ATTR_ITEM_UNIQUEID:
		case ATTR_ITEM_OWNER:
		case ATTR_ITEM_DURATION:
		case ATTR_ITEM_DECAYING:
		case ATTR_ITEM_WRITTENDATE:
		case ATTR_ITEM_CORPSEOWNER:
		case ATTR_ITEM_CHARGES:
		case ATTR_ITEM_FLUIDTYPE:
		case ATTR_ITEM_DOORID:
			return true;

		default:
			break;
	}

	return false;
}

bool ItemAttributes::validateStrAttrType(itemAttrTypes type)
{
	switch (type) {
		case ATTR_ITEM_DESC:
		case ATTR_ITEM_TEXT:
		case ATTR_ITEM_WRITTENBY:
			return true;

		default:
			break;
	}

	return false;
}

void ItemAttributes::addAttr(Attribute* attr)
{
	if (m_firstAttr) {
		Attribute* curAttr = m_firstAttr;

		while (curAttr->next) {
			curAttr = curAttr->next;
		}

		curAttr->next = attr;
	} else {
		m_firstAttr = attr;
	}

	m_attributes = m_attributes | attr->type;
}

ItemAttributes::Attribute* ItemAttributes::getAttrConst(itemAttrTypes type) const
{
	if ((type & m_attributes) == 0) {
		return NULL;
	}

	Attribute* curAttr = m_firstAttr;

	while (curAttr) {
		if (curAttr->type == type) {
			return curAttr;
		}

		curAttr = curAttr->next;
	}

	std::cout << "Warning: [ItemAttributes::getAttrConst] (type & m_attributes) != 0 but attribute not found" << std::endl;
	return NULL;
}

ItemAttributes::Attribute* ItemAttributes::getAttr(itemAttrTypes type)
{
	Attribute* curAttr;

	if ((type & m_attributes) == 0) {
		curAttr = new Attribute(type);
		addAttr(curAttr);
		return curAttr;
	} else {
		curAttr = m_firstAttr;

		while (curAttr) {
			if (curAttr->type == type) {
				return curAttr;
			}

			curAttr = curAttr->next;
		}
	}

	std::cout << "Warning: [ItemAttributes::getAttr] (type & m_attributes) != 0 but attribute not found" << std::endl;
	curAttr = new Attribute(type);
	addAttr(curAttr);
	return curAttr;
}

void ItemAttributes::deleteAttrs(Attribute* attr)
{
	if (attr) {
		if (validateStrAttrType(attr->type)) {
			delete (std::string*)attr->value;
		}

		Attribute* next_attr = attr->next;
		attr->next = NULL;

		if (next_attr) {
			deleteAttrs(next_attr);
		}

		delete attr;
	}
}

void Item::__startDecaying()
{
	g_game.startDecay(this);
}
