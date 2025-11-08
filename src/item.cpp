// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "item.h"

#include "bed.h"
#include "combat.h"
#include "container.h"
#include "game.h"
#include "house.h"
#include "mailbox.h"
#include "podium.h"
#include "teleport.h"
#include "trashholder.h"

class Spells;

extern Game g_game;
extern Spells* g_spells;
extern Vocations g_vocations;

Items Item::items;

Item* Item::CreateItem(const uint16_t type, uint16_t count /*= 0*/)
{
	const ItemType& it = Item::items[type];
	if (it.group == ITEM_GROUP_DEPRECATED) [[unlikely]] {
		return nullptr;
	}

	Item* newItem = nullptr;
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
		} else if (it.isPodium()) {
			newItem = new Podium(type);
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
	if (it.id == 0 || it.group == ITEM_GROUP_DEPRECATED || it.stackable || it.useable || it.moveable || it.pickupable ||
	    it.isDepot() || it.isSplash() || it.isDoor()) {
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

	return Item::CreateItem(getPersistentId(id));
}

uint16_t Item::getPersistentId(uint16_t id)
{
	switch (id) {
		case ITEM_FIREFIELD_PVP_FULL:
			return ITEM_FIREFIELD_PERSISTENT_FULL;

		case ITEM_FIREFIELD_PVP_MEDIUM:
			return ITEM_FIREFIELD_PERSISTENT_MEDIUM;

		case ITEM_FIREFIELD_PVP_SMALL:
			return ITEM_FIREFIELD_PERSISTENT_SMALL;

		case ITEM_ENERGYFIELD_PVP:
			return ITEM_ENERGYFIELD_PERSISTENT;

		case ITEM_POISONFIELD_PVP:
			return ITEM_POISONFIELD_PERSISTENT;

		case ITEM_MAGICWALL:
			return ITEM_MAGICWALL_PERSISTENT;

		case ITEM_WILDGROWTH:
			return ITEM_WILDGROWTH_PERSISTENT;

		default:
			return id;
	}
}

Item::Item(const uint16_t type, uint16_t count /*= 0*/) : id(type)
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

Item::Item(const Item& i) : Thing(), id(i.id), count(i.count), loadedFromMap(i.loadedFromMap)
{
	if (i.attributes) {
		attributes.reset(new ItemAttributes(*i.attributes));
	}
}

Item* Item::clone() const
{
	Item* item = Item::CreateItem(id, count);
	if (attributes) {
		item->attributes.reset(new ItemAttributes(*attributes));
		if (item->getDuration() > 0) {
			item->incrementReferenceCounter();
			item->setDecaying(DECAYING_TRUE);
			g_game.toDecayItems.push_front(item);
		}
	}
	return item;
}

bool Item::equals(const Item* otherItem) const
{
	if (!otherItem || id != otherItem->id) {
		return false;
	}

	const auto& otherAttributes = otherItem->attributes;
	if (!attributes) {
		return !otherAttributes || (otherAttributes->attributeBits == 0);
	} else if (!otherAttributes) {
		return (attributes->attributeBits == 0);
	}

	if (attributes->attributeBits != otherAttributes->attributeBits) {
		return false;
	}

	const auto& attributeList = attributes->attributes;
	const auto& otherAttributeList = otherAttributes->attributes;
	for (const auto& attribute : attributeList) {
		if (ItemAttributes::isIntAttrType(attribute.type)) {
			for (const auto& otherAttribute : otherAttributeList) {
				if (attribute.type == otherAttribute.type && attribute.value.integer != otherAttribute.value.integer) {
					return false;
				}
			}
		} else if (ItemAttributes::isStrAttrType(attribute.type)) {
			for (const auto& otherAttribute : otherAttributeList) {
				if (attribute.type == otherAttribute.type && *attribute.value.string != *otherAttribute.value.string) {
					return false;
				}
			}
		} else {
			for (const auto& otherAttribute : otherAttributeList) {
				if (attribute.type == otherAttribute.type && *attribute.value.custom != *otherAttribute.value.custom) {
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
	tfs::lua::removeTempItem(this);

	if (hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
		g_game.removeUniqueItem(getUniqueId());
	}
}

void Item::setID(uint16_t newid)
{
	const ItemType& prevIt = Item::items[id];
	id = newid;

	const ItemType& it = Item::items[newid];
	uint32_t newDuration = normal_random(it.decayTimeMin, it.decayTimeMax) * 1000;

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

	while (aux->hasParent()) {
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

	while (aux->hasParent()) {
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
	// get root cylinder
	if (cylinder && cylinder->hasParent()) {
		cylinder = cylinder->getParent();
	}
	return dynamic_cast<Tile*>(cylinder);
}

const Tile* Item::getTile() const
{
	const Cylinder* cylinder = getTopParent();
	// get root cylinder
	if (cylinder && cylinder->hasParent()) {
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

const Player* Item::getHoldingPlayer() const
{
	const auto topParent = getTopParent();
	if (!topParent) {
		return nullptr;
	}

	if (const auto creature = topParent->getCreature()) {
		return creature->getPlayer();
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

void Item::readAttr(AttrTypes_t attr, OTB::iterator& first, const OTB::iterator& last)
{
	switch (attr) {
		case ATTR_CHARGES:
		case ATTR_COUNT:
		case ATTR_RUNE_CHARGES: {
			auto count = OTB::read<uint8_t>(first, last);
			setSubType(count);
			break;
		}

		case ATTR_ACTION_ID: {
			auto actionId = OTB::read<uint16_t>(first, last);
			setActionId(actionId);
			break;
		}

		case ATTR_UNIQUE_ID: {
			auto uniqueId = OTB::read<uint16_t>(first, last);
			setUniqueId(uniqueId);
			break;
		}

		case ATTR_TEXT: {
			auto text = OTB::readString(first, last);
			setText(text);
			break;
		}

		case ATTR_WRITTENDATE: {
			auto writtenDate = OTB::read<uint32_t>(first, last);
			setDate(writtenDate);
			break;
		}

		case ATTR_WRITTENBY: {
			auto writer = OTB::readString(first, last);
			setWriter(writer);
			break;
		}

		case ATTR_DESC: {
			auto text = OTB::readString(first, last);
			setSpecialDescription(text);
			break;
		}

		case ATTR_DURATION: {
			auto duration = OTB::read<int32_t>(first, last);
			setDuration(std::max<int32_t>(0, duration));
			break;
		}

		case ATTR_DECAYING_STATE: {
			auto state = OTB::read<uint8_t>(first, last);
			if (state != DECAYING_FALSE) {
				setDecaying(DECAYING_PENDING);
			}
			break;
		}

		case ATTR_NAME: {
			auto name = OTB::readString(first, last);
			setStrAttr(ITEM_ATTRIBUTE_NAME, name);
			break;
		}

		case ATTR_ARTICLE: {
			auto article = OTB::readString(first, last);
			setStrAttr(ITEM_ATTRIBUTE_ARTICLE, article);
			break;
		}

		case ATTR_PLURALNAME: {
			auto pluralName = OTB::readString(first, last);
			setStrAttr(ITEM_ATTRIBUTE_PLURALNAME, pluralName);
			break;
		}

		case ATTR_WEIGHT: {
			auto weight = OTB::read<uint32_t>(first, last);
			setIntAttr(ITEM_ATTRIBUTE_WEIGHT, weight);
			break;
		}

		case ATTR_ATTACK: {
			auto attack = OTB::read<int32_t>(first, last);
			setIntAttr(ITEM_ATTRIBUTE_ATTACK, attack);
			break;
		}

		case ATTR_ATTACK_SPEED: {
			auto attackSpeed = OTB::read<uint32_t>(first, last);
			setIntAttr(ITEM_ATTRIBUTE_ATTACK_SPEED, attackSpeed);
			break;
		}

		case ATTR_DEFENSE: {
			auto defense = OTB::read<int32_t>(first, last);
			setIntAttr(ITEM_ATTRIBUTE_DEFENSE, defense);
			break;
		}

		case ATTR_EXTRADEFENSE: {
			auto extraDefense = OTB::read<int32_t>(first, last);
			setIntAttr(ITEM_ATTRIBUTE_EXTRADEFENSE, extraDefense);
			break;
		}

		case ATTR_ARMOR: {
			auto armor = OTB::read<int32_t>(first, last);
			setIntAttr(ITEM_ATTRIBUTE_ARMOR, armor);
			break;
		}

		case ATTR_HITCHANCE: {
			auto hitChance = OTB::read<int8_t>(first, last);
			setIntAttr(ITEM_ATTRIBUTE_HITCHANCE, hitChance);
			break;
		}

		case ATTR_SHOOTRANGE: {
			auto shootRange = OTB::read<uint8_t>(first, last);
			setIntAttr(ITEM_ATTRIBUTE_SHOOTRANGE, shootRange);
			break;
		}

		case ATTR_DECAYTO: {
			auto decayTo = OTB::read<int32_t>(first, last);
			setIntAttr(ITEM_ATTRIBUTE_DECAYTO, decayTo);
			break;
		}

		case ATTR_WRAPID: {
			auto wrapId = OTB::read<uint16_t>(first, last);
			setIntAttr(ITEM_ATTRIBUTE_WRAPID, wrapId);
			break;
		}

		case ATTR_STOREITEM: {
			auto storeItem = OTB::read<uint8_t>(first, last);
			setIntAttr(ITEM_ATTRIBUTE_STOREITEM, storeItem);
			break;
		}

		case ATTR_OPENCONTAINER: {
			auto openContainer = OTB::read<uint8_t>(first, last);
			setIntAttr(ITEM_ATTRIBUTE_OPENCONTAINER, openContainer);
			break;
		}

		case ATTR_REFLECT: {
			auto size = OTB::read<uint16_t>(first, last);

			for (uint16_t i = 0; i < size; ++i) {
				auto combatType = OTB::read<CombatType_t>(first, last);
				Reflect reflect(OTB::read<uint16_t>(first, last), OTB::read<uint16_t>(first, last));
				getAttributes()->reflect[combatType] = reflect;
			}
			break;
		}

		case ATTR_BOOST: {
			auto size = OTB::read<uint16_t>(first, last);

			for (uint16_t i = 0; i < size; ++i) {
				auto combatType = OTB::read<CombatType_t>(first, last);
				auto percent = OTB::read<uint16_t>(first, last);
				getAttributes()->boostPercent[combatType] = percent;
			}
			break;
		}

		case ATTR_CUSTOM_ATTRIBUTES: {
			auto size = OTB::read<uint64_t>(first, last);

			for (uint64_t i = 0; i < size; i++) {
				// Unserialize key type and value
				auto key = OTB::readString(first, last);

				// Unserialize value type and value
				ItemAttributes::CustomAttribute val;
				val.unserialize(first, last);
				setCustomAttribute(key, val);
			}
			break;
		}

		default:
			// fmt::print("Invalid attribute type: {:d}\n", tfs::to_underlying(attr));
			break;
	}
}

void Item::unserializeAttr(OTB::iterator& first, const OTB::iterator& last)
{
	while (first != last) {
		auto attr = OTB::read<uint8_t>(first, last);
		if (attr == ATTR_END) {
			break;
		}

		readAttr(static_cast<AttrTypes_t>(attr), first, last);
	}
}

void Item::unserializeItemNode(OTB::iterator& first, const OTB::iterator& last, const OTB::Node&)
{
	return unserializeAttr(first, last);
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

	if (hasAttribute(ITEM_ATTRIBUTE_ATTACK_SPEED)) {
		propWriteStream.write<uint8_t>(ATTR_ATTACK_SPEED);
		propWriteStream.write<uint32_t>(getIntAttr(ITEM_ATTRIBUTE_ATTACK_SPEED));
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

	if (hasAttribute(ITEM_ATTRIBUTE_DECAYTO)) {
		propWriteStream.write<uint8_t>(ATTR_DECAYTO);
		propWriteStream.write<int32_t>(getIntAttr(ITEM_ATTRIBUTE_DECAYTO));
	}

	if (hasAttribute(ITEM_ATTRIBUTE_WRAPID)) {
		propWriteStream.write<uint8_t>(ATTR_WRAPID);
		propWriteStream.write<uint16_t>(getIntAttr(ITEM_ATTRIBUTE_WRAPID));
	}

	if (hasAttribute(ITEM_ATTRIBUTE_STOREITEM)) {
		propWriteStream.write<uint8_t>(ATTR_STOREITEM);
		propWriteStream.write<uint8_t>(getIntAttr(ITEM_ATTRIBUTE_STOREITEM));
	}

	if (hasAttribute(ITEM_ATTRIBUTE_OPENCONTAINER)) {
		propWriteStream.write<uint8_t>(ATTR_OPENCONTAINER);
		propWriteStream.write<uint8_t>(getIntAttr(ITEM_ATTRIBUTE_OPENCONTAINER));
	}

	if (hasAttribute(ITEM_ATTRIBUTE_CUSTOM)) {
		const ItemAttributes::CustomAttributeMap* customAttrMap = attributes->getCustomAttributeMap();
		propWriteStream.write<uint8_t>(ATTR_CUSTOM_ATTRIBUTES);
		propWriteStream.write<uint64_t>(static_cast<uint64_t>(customAttrMap->size()));
		for (const auto& entry : *customAttrMap) {
			// Serializing key type and value
			propWriteStream.writeString(entry.first);

			// Serializing value type and value
			entry.second.serialize(propWriteStream);
		}
	}

	if (attributes) {
		const auto& reflects = attributes->reflect;
		if (!reflects.empty()) {
			propWriteStream.write<uint8_t>(ATTR_REFLECT);
			propWriteStream.write<uint16_t>(reflects.size());

			for (const auto& reflect : reflects) {
				propWriteStream.write<CombatType_t>(reflect.first);
				propWriteStream.write<uint16_t>(reflect.second.percent);
				propWriteStream.write<uint16_t>(reflect.second.chance);
			}
		}

		const auto& boosts = attributes->boostPercent;
		if (!boosts.empty()) {
			propWriteStream.write<uint8_t>(ATTR_BOOST);
			propWriteStream.write<uint16_t>(boosts.size());

			for (const auto& boost : boosts) {
				propWriteStream.write<CombatType_t>(boost.first);
				propWriteStream.write<uint16_t>(boost.second);
			}
		}
	}
}

bool Item::hasProperty(ITEMPROPERTY prop) const
{
	const ItemType& it = items[id];
	switch (prop) {
		case CONST_PROP_BLOCKSOLID:
			return it.blockSolid;
		case CONST_PROP_MOVEABLE:
			return it.moveable && !hasAttribute(ITEM_ATTRIBUTE_UNIQUEID);
		case CONST_PROP_HASHEIGHT:
			return it.hasHeight;
		case CONST_PROP_BLOCKPROJECTILE:
			return it.blockProjectile;
		case CONST_PROP_BLOCKPATH:
			return it.blockPathFind;
		case CONST_PROP_ISVERTICAL:
			return it.isVertical;
		case CONST_PROP_ISHORIZONTAL:
			return it.isHorizontal;
		case CONST_PROP_IMMOVABLEBLOCKSOLID:
			return it.blockSolid && (!it.moveable || hasAttribute(ITEM_ATTRIBUTE_UNIQUEID));
		case CONST_PROP_IMMOVABLEBLOCKPATH:
			return it.blockPathFind && (!it.moveable || hasAttribute(ITEM_ATTRIBUTE_UNIQUEID));
		case CONST_PROP_IMMOVABLENOFIELDBLOCKPATH:
			return !it.isMagicField() && it.blockPathFind && (!it.moveable || hasAttribute(ITEM_ATTRIBUTE_UNIQUEID));
		case CONST_PROP_NOFIELDBLOCKPATH:
			return !it.isMagicField() && it.blockPathFind;
		case CONST_PROP_SUPPORTHANGABLE:
			return it.isHorizontal || it.isVertical;
		default:
			return false;
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

std::string Item::getNameDescription(const ItemType& it, const Item* item /*= nullptr*/, int32_t subType /*= -1*/,
                                     bool addArticle /*= true*/)
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
		if (addArticle) {
			s << "an ";
		}
		s << "item of type " << it.id;
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

void Item::setDefaultDuration()
{
	uint32_t duration = getDefaultDurationMin();
	if (uint32_t durationMax = getDefaultDurationMax()) {
		duration = normal_random(duration, durationMax);
	}

	if (duration != 0) {
		setDuration(duration);
	}
}

bool Item::canDecay() const
{
	if (isRemoved()) {
		return false;
	}

	if (getDecayTo() < 0 || (getDecayTimeMin() == 0 && getDecayTimeMax() == 0)) {
		return false;
	}

	if (hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
		return false;
	}

	return true;
}

uint32_t Item::getWorth() const { return items[id].worth * count; }

LightInfo Item::getLightInfo() const
{
	const ItemType& it = items[id];
	return {it.lightLevel, it.lightColor};
}

Reflect Item::getReflect(CombatType_t combatType, bool total /* = true */) const
{
	const ItemType& it = Item::items[id];

	Reflect reflect;
	if (attributes) {
		reflect += attributes->getReflect(combatType);
	}

	if (total && it.abilities) {
		reflect += it.abilities->reflect[combatTypeToIndex(combatType)];
	}

	return reflect;
}

uint16_t Item::getBoostPercent(CombatType_t combatType, bool total /* = true */) const
{
	const ItemType& it = Item::items[id];

	uint16_t boostPercent = 0;
	if (attributes) {
		boostPercent += attributes->getBoostPercent(combatType);
	}

	if (total && it.abilities) {
		boostPercent += it.abilities->boostPercent[combatTypeToIndex(combatType)];
	}

	return boostPercent;
}

std::string ItemAttributes::emptyString;
int64_t ItemAttributes::emptyInt;
double ItemAttributes::emptyDouble;
bool ItemAttributes::emptyBool;
Reflect ItemAttributes::emptyReflect;

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

void ItemAttributes::setStrAttr(itemAttrTypes type, std::string_view value)
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

	auto prev_it = attributes.rbegin();
	if ((*prev_it).type == type) {
		attributes.pop_back();
	} else {
		auto it = prev_it, end = attributes.rend();
		while (++it != end) {
			if ((*it).type == type) {
				(*it) = attributes.back();
				attributes.pop_back();
				break;
			}
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

	if (type == ITEM_ATTRIBUTE_ATTACK_SPEED && value < 100) {
		value = 100;
	}

	getAttr(type).value.integer = value;
}

void ItemAttributes::increaseIntAttr(itemAttrTypes type, int64_t value) { setIntAttr(type, getIntAttr(type) + value); }

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
	attributes.emplace_back(type);
	return attributes.back();
}

void Item::startDecaying() { g_game.startDecay(this); }

bool Item::hasMarketAttributes() const
{
	if (!attributes) {
		return true;
	}

	// discard items with custom boost and reflect
	for (uint16_t i = 0; i < COMBAT_COUNT; ++i) {
		if (getBoostPercent(indexToCombatType(i), false) > 0) {
			return false;
		}

		Reflect tmpReflect = getReflect(indexToCombatType(i), false);
		if (tmpReflect.chance != 0 || tmpReflect.percent != 0) {
			return false;
		}
	}

	// discard items with other modified attributes
	for (const auto& attr : attributes->getList()) {
		if (attr.type == ITEM_ATTRIBUTE_CHARGES) {
			uint16_t charges = static_cast<uint16_t>(attr.value.integer);
			if (charges != items[id].charges) {
				return false;
			}
		} else if (attr.type == ITEM_ATTRIBUTE_DURATION) {
			uint32_t duration = static_cast<uint32_t>(attr.value.integer);
			if (duration <= getDefaultDurationMin()) {
				return false;
			}
		} else {
			return false;
		}
	}
	return true;
}
