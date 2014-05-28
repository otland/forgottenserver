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

#include "otpch.h"

#include "item.h"
#include "container.h"
#include "teleport.h"
#include "trashholder.h"
#include "mailbox.h"
#include "house.h"
#include "game.h"
#include "luascript.h"
#include "beds.h"

#include "actions.h"
#include "combat.h"

extern Game g_game;

Items Item::items;

Item* Item::CreateItem(const uint16_t _type, uint16_t _count /*= 0*/)
{
	Item* newItem = nullptr;

	const ItemType& it = Item::items[_type];
	if (it.group == ITEM_GROUP_DEPRECATED) {
		return nullptr;
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
			newItem = new TrashHolder(_type);
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
		return nullptr;
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
	parent = nullptr;
	useCount = 0;

	id = _type;
	attributes = nullptr;

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
	parent = nullptr;
	useCount = 0;

	//std::cout << "Item copy constructor " << this << std::endl;
	id = i.id;
	count = i.count;
	loadedFromMap = i.loadedFromMap;

	if (i.attributes) {
		attributes = new ItemAttributes(*i.attributes);
	} else {
		attributes = nullptr;
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

void Item::stealAttributes(Item* item)
{
	if (item->attributes) {
		attributes = item->attributes;
		item->attributes = nullptr;
	}

	removeAttribute(ITEM_ATTRIBUTE_DECAYSTATE);
	removeAttribute(ITEM_ATTRIBUTE_DURATION);
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

	if (hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
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
	if (cylinder->getParent()) {
		cylinder = cylinder->getParent();
	}
	return dynamic_cast<Tile*>(cylinder);
}

const Tile* Item::getTile() const
{
	const Cylinder* cylinder = getTopParent();
	//get root cylinder
	if (cylinder->getParent()) {
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

Player* Item::getHoldingPlayer()
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
			uint8_t _count;
			if (!propStream.GET_UCHAR(_count)) {
				return ATTR_READ_ERROR;
			}

			setSubType(_count);
			break;
		}

		case ATTR_ACTION_ID: {
			uint16_t _actionid;
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
			uint8_t _charges;
			if (!propStream.GET_UCHAR(_charges)) {
				return ATTR_READ_ERROR;
			}

			setSubType(_charges);
			break;
		}

		case ATTR_CHARGES: {
			uint16_t _charges;
			if (!propStream.GET_USHORT(_charges)) {
				return ATTR_READ_ERROR;
			}

			setSubType(_charges);
			break;
		}

		case ATTR_DURATION: {
			int32_t duration;
			if (!propStream.GET_VALUE(duration)) {
				return ATTR_READ_ERROR;
			}

			setDuration(std::max<int32_t>(0, duration));
			break;
		}

		case ATTR_DECAYING_STATE: {
			uint8_t state;
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
			Position* tele_dest;
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

bool Item::unserializeItemNode(FileLoader&, NODE, PropStream& propStream)
{
	return unserializeAttr(propStream);
}

bool Item::serializeAttr(PropWriteStream& propWriteStream) const
{
	if (isStackable() || isFluidContainer() || isSplash()) {
		propWriteStream.ADD_UCHAR(ATTR_COUNT);
		propWriteStream.ADD_UCHAR(getSubType());
	}

	uint16_t charges = getCharges();
	if (charges != 0) {
		propWriteStream.ADD_UCHAR(ATTR_CHARGES);
		propWriteStream.ADD_USHORT(charges);
	}

	if (isMoveable()) {
		uint16_t _actionId = getActionId();
		if (_actionId != 0) {
			propWriteStream.ADD_UCHAR(ATTR_ACTION_ID);
			propWriteStream.ADD_USHORT(_actionId);
		}
	}

	const std::string& _text = getText();
	if (!_text.empty()) {
		propWriteStream.ADD_UCHAR(ATTR_TEXT);
		propWriteStream.ADD_STRING(_text);
	}

	const time_t _writtenDate = getDate();
	if (_writtenDate > 0) {
		propWriteStream.ADD_UCHAR(ATTR_WRITTENDATE);
		propWriteStream.ADD_ULONG(_writtenDate);
	}

	const std::string& _writer = getWriter();
	if (!_writer.empty()) {
		propWriteStream.ADD_UCHAR(ATTR_WRITTENBY);
		propWriteStream.ADD_STRING(_writer);
	}

	const std::string& _specialDesc = getSpecialDescription();
	if (!_specialDesc.empty()) {
		propWriteStream.ADD_UCHAR(ATTR_DESC);
		propWriteStream.ADD_STRING(_specialDesc);
	}

	if (hasAttribute(ITEM_ATTRIBUTE_DURATION)) {
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

double Item::getWeight() const
{
	if (isStackable()) {
		return items[id].weight * std::max<int32_t>(1, getItemCount());
	}
	return items[id].weight;
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
		if (it.weaponType == WEAPON_DISTANCE && it.ammoType != AMMO_NONE) {
			s << " (Range:" << it.shootRange;

			if (it.attack != 0) {
				s << ", Atk" << std::showpos << it.attack << std::noshowpos;
			}

			if (it.hitChance != 0) {
				s << ", Hit%" << std::showpos << it.hitChance << std::noshowpos;
			}

			s << ')';
		} else if (it.weaponType != WEAPON_AMMO) {
			bool begin = true;

			if (it.attack != 0) {
				begin = false;
				s << " (Atk:" << it.attack;

				if (it.abilities && it.abilities->elementType != COMBAT_NONE && it.abilities->elementDamage != 0) {
					s << " physical + " << it.abilities->elementDamage << ' ' << getCombatName(it.abilities->elementType);
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
					s << ' ' << std::showpos << it.extraDefense << std::noshowpos;
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

					s << getSkillName(i) << ' ' << std::showpos << it.abilities->skills[i] << std::noshowpos;
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

						s << getCombatName(indexToCombatType(i)) << ' ' << std::showpos << it.abilities->absorbPercent[i] << std::noshowpos << '%';
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

				show = it.abilities->fieldAbsorbPercent[COMBAT_FIRST];

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
						if (!it.abilities->fieldAbsorbPercent[i]) {
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

						s << getCombatName(indexToCombatType(i)) << " field " << std::showpos << it.abilities->fieldAbsorbPercent[i] << std::noshowpos << '%';
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

				if (it.abilities->speed) {
					if (begin) {
						begin = false;
						s << " (";
					} else {
						s << ", ";
					}

					s << "speed " << std::showpos << (it.abilities->speed >> 1) << std::noshowpos;
				}
			}

			if (!begin) {
				s << ')';
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

				s << getSkillName(i) << ' ' << std::showpos << it.abilities->skills[i] << std::noshowpos;
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

					s << getCombatName(indexToCombatType(i)) << ' ' << std::showpos << it.abilities->absorbPercent[i] << std::noshowpos << '%';
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

			show = it.abilities->fieldAbsorbPercent[COMBAT_FIRST];

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
					if (!it.abilities->fieldAbsorbPercent[i]) {
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

					s << getCombatName(indexToCombatType(i)) << " field " << std::showpos << it.abilities->fieldAbsorbPercent[i] << std::noshowpos << '%';
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

			if (it.abilities->speed) {
				if (begin) {
					begin = false;
					s << " (";
				} else {
					s << ", ";
				}

				s << "speed " << std::showpos << (it.abilities->speed >> 1) << std::noshowpos;
			}
		}

		if (!begin) {
			s << ')';
		}
	} else if (it.isContainer()) {
		s << " (Vol:" << it.maxItems << ')';
	} else {
		bool found = true;

		if (it.abilities) {
			if (it.abilities->speed > 0) {
				s << " (speed " << std::showpos << (it.abilities->speed / 2) << std::noshowpos << ')';
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
				s << '.' << std::endl;

				if (lookDistance <= 4) {
					if (item) {
						text = &item->getText();
						if (!text->empty()) {
							const std::string& writer = item->getWriter();
							if (!writer.empty()) {
								s << writer << " wrote";
								time_t date = item->getDate();
								if (date > 0) {
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
		double weight = (item == nullptr ? it.weight : item->getWeight());
		if (weight > 0 && it.pickupable) {
			int32_t amount = weight / it.weight;
			s << std::endl << getWeightDescription(it, weight, amount);
		}
	}

	if (item) {
		const std::string& specialDescription = item->getSpecialDescription();
		if (!specialDescription.empty()) {
			s << std::endl << specialDescription;
		} else if (lookDistance <= 1 && !it.description.empty()) {
			s << std::endl << it.description;
		}
	} else if (lookDistance <= 1 && !it.description.empty()) {
		s << std::endl << it.description;
	}

	if (it.allowDistRead && it.id >= 7369 && it.id <= 7371) {
		if (!text && item) {
			text = &item->getText();
		}

		if (text && !text->empty()) {
			s << std::endl << *text;
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

	if (!it.name.empty()) {
		if (it.stackable && subType > 1) {
			if (it.showCount) {
				s << subType << ' ';
			}

			s << it.getPluralName();
		} else {
			if (addArticle && !it.article.empty()) {
				s << it.article << ' ';
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
	if (weight <= 0) {
		return std::string();
	}
	return getWeightDescription(weight);
}

void Item::setUniqueId(uint16_t n)
{
	if (hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
		return;
	}

	getAttributes()->setUniqueId(n);
	ScriptEnvironment::addUniqueThing(this);
}

bool Item::canDecay() const
{
	if (isRemoved()) {
		return false;
	}

	const ItemType& it = Item::items[id];
	if (it.decayTo == -1 || it.decayTime == 0) {
		return false;
	}

	if (hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
		return false;
	}

	return true;
}

int32_t Item::getWorth() const
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

void Item::getLight(LightInfo& lightInfo) const
{
	const ItemType& it = items[id];
	lightInfo.color = it.lightColor;
	lightInfo.level = it.lightLevel;
}

std::string ItemAttributes::emptyString;

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

	if (value.empty()) {
		return;
	}

	Attribute& attr = getAttr(type);
	if (attr.value) {
		delete (std::string*)attr.value;
	}
	attr.value = (uint8_t*)new std::string(value);
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

int32_t ItemAttributes::getIntAttr(itemAttrTypes type) const
{
	if (!validateIntAttrType(type)) {
		return 0;
	}

	Attribute* attr = getAttrConst(type);
	if (attr) {
		return reinterpret_cast<ptrdiff_t>(attr->value);
	} else {
		return 0;
	}
}

void ItemAttributes::setIntAttr(itemAttrTypes type, int32_t value)
{
	if (!validateIntAttrType(type)) {
		return;
	}

	getAttr(type).value = reinterpret_cast<uint8_t*>(value);
}

void ItemAttributes::increaseIntAttr(itemAttrTypes type, int32_t value)
{
	if (!validateIntAttrType(type)) {
		return;
	}

	getAttr(type).value += value;
}

bool ItemAttributes::validateIntAttrType(itemAttrTypes type)
{
	switch (type) {
		case ITEM_ATTRIBUTE_ACTIONID:
		case ITEM_ATTRIBUTE_UNIQUEID:
		case ITEM_ATTRIBUTE_OWNER:
		case ITEM_ATTRIBUTE_DURATION:
		case ITEM_ATTRIBUTE_DECAYSTATE:
		case ITEM_ATTRIBUTE_DATE:
		case ITEM_ATTRIBUTE_CORPSEOWNER:
		case ITEM_ATTRIBUTE_CHARGES:
		case ITEM_ATTRIBUTE_FLUIDTYPE:
		case ITEM_ATTRIBUTE_DOORID:
			return true;

		default:
			break;
	}
	return false;
}

bool ItemAttributes::validateStrAttrType(itemAttrTypes type)
{
	switch (type) {
		case ITEM_ATTRIBUTE_DESCRIPTION:
		case ITEM_ATTRIBUTE_TEXT:
		case ITEM_ATTRIBUTE_WRITER:
			return true;

		default:
			break;
	}
	return false;
}

ItemAttributes::Attribute* ItemAttributes::getAttrConst(itemAttrTypes type) const
{
	if (hasAttribute(type)) {
		for (const Attribute& attribute : attributes) {
			if (attribute.type == type) {
				return const_cast<ItemAttributes::Attribute*>(&attribute);
			}
		}
	}
	return nullptr;
}

ItemAttributes::Attribute& ItemAttributes::getAttr(itemAttrTypes type)
{
	Attribute* curAttr = getAttrConst(type);
	if (curAttr) {
		return *curAttr;
	}

	attributeBits |= type;
	attributes.emplace_front(type);
	return attributes.front();
}

void Item::__startDecaying()
{
	g_game.startDecay(this);
}
