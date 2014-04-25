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

#include "pugicast.h"

#include "house.h"
#include "iologindata.h"
#include "game.h"
#include "town.h"
#include "configmanager.h"
#include "tools.h"
#include "beds.h"

extern ConfigManager g_config;
extern Game g_game;

House::House(uint32_t _houseid) :
	transfer_container(ITEM_LOCKER1)
{
	isLoaded = false;
	owner = 0;
	posEntry.x = 0;
	posEntry.y = 0;
	posEntry.z = 0;
	paidUntil = 0;
	id = _houseid;
	rentWarnings = 0;
	rent = 0;
	townid = 0;
	transferItem = nullptr;
}

void House::addTile(HouseTile* tile)
{
	tile->setFlag(TILESTATE_PROTECTIONZONE);
	houseTiles.push_back(tile);
}

void House::setOwner(uint32_t guid, bool updateDatabase/* = true*/, Player* player/* = nullptr*/)
{
	if (updateDatabase && owner != guid) {
		Database* db = Database::getInstance();

		std::ostringstream query;
		query << "UPDATE `houses` SET `owner` = " << guid << ", `bid` = 0, `bid_end` = 0, `last_bid` = 0, `highest_bidder` = 0  WHERE `id` = " << id;
		db->executeQuery(query.str());
	}

	if (isLoaded && owner == guid) {
		return;
	}

	isLoaded = true;

	if (owner != 0) {
		//send items to depot
		if (player) {
			transferToDepot(player);
		} else {
			transferToDepot();
		}

		for (HouseTile* tile : houseTiles) {
			if (const CreatureVector* creatures = tile->getCreatures()) {
				for (int32_t i = creatures->size(); --i >= 0;) {
					kickPlayer(nullptr, (*creatures)[i]->getPlayer());
				}
			}
		}

		// Remove players from beds
		for (BedItem* bed : bedsList) {
			if (bed->getSleeper() != 0) {
				bed->wakeUp(nullptr);
			}
		}

		//clean access lists
		owner = 0;
		setAccessList(SUBOWNER_LIST, "");
		setAccessList(GUEST_LIST, "");

		for (Door* door : doorList) {
			door->setAccessList("");
		}

		//reset paid date
		paidUntil = 0;
		rentWarnings = 0;
	}

	std::string name;
	if (guid != 0 && IOLoginData::getNameByGuid(guid, name)) {
		owner = guid;
		ownerName = name;
	}

	updateDoorDescription();
}

void House::updateDoorDescription() const
{
	std::ostringstream ss;
	if (owner != 0) {
		ss << "It belongs to house '" << houseName << "'. " << ownerName << " owns this house.";
	} else {
		ss << "It belongs to house '" << houseName << "'. Nobody owns this house.";

		const int32_t housePrice = g_config.getNumber(ConfigManager::HOUSE_PRICE);
		if (housePrice != -1) {
			ss << " It costs " << (houseTiles.size() * housePrice) << " gold coins.";
		}
	}

	for (const auto& it : doorList) {
		it->setSpecialDescription(ss.str());
	}
}

AccessHouseLevel_t House::getHouseAccessLevel(const Player* player)
{
	if (!player) {
		return HOUSE_OWNER;
	}

	if (player->hasFlag(PlayerFlag_CanEditHouses)) {
		return HOUSE_OWNER;
	}

	if (player->getGUID() == owner) {
		return HOUSE_OWNER;
	}

	if (subOwnerList.isInList(player)) {
		return HOUSE_SUBOWNER;
	}

	if (guestList.isInList(player)) {
		return HOUSE_GUEST;
	}

	return HOUSE_NO_INVITED;
}

bool House::kickPlayer(Player* player, Player* target)
{
	if (!target) {
		return false;
	}

	HouseTile* houseTile = dynamic_cast<HouseTile*>(target->getTile());
	if (!houseTile || houseTile->getHouse() != this) {
		return false;
	}

	if (getHouseAccessLevel(player) < getHouseAccessLevel(target) || target->hasFlag(PlayerFlag_CanEditHouses)) {
		return false;
	}

	Position oldPosition = target->getPosition();
	if (g_game.internalTeleport(target, getEntryPosition()) == RET_NOERROR) {
		g_game.addMagicEffect(oldPosition, CONST_ME_POFF);
		g_game.addMagicEffect(getEntryPosition(), CONST_ME_TELEPORT);
	}
	return true;
}

void House::setAccessList(uint32_t listId, const std::string& textlist)
{
	if (listId == GUEST_LIST) {
		guestList.parseList(textlist);
	} else if (listId == SUBOWNER_LIST) {
		subOwnerList.parseList(textlist);
	} else {
		Door* door = getDoorByNumber(listId);
		if (door) {
			door->setAccessList(textlist);
		}

		// We dont have kick anyone
		return;
	}

	//kick uninvited players
	for (HouseTile* tile : houseTiles) {
		if (CreatureVector* creatures = tile->getCreatures()) {
			for (int32_t i = creatures->size(); --i >= 0;) {
				Player* player = (*creatures)[i]->getPlayer();
				if (player && !isInvited(player)) {
					kickPlayer(nullptr, player);
				}
			}
		}
	}
}

bool House::transferToDepot() const
{
	if (townid == 0 || owner == 0) {
		return false;
	}

	Player* player = g_game.getPlayerByGUID(owner);
	if (player) {
		transferToDepot(player);
	} else {
		Player tmpPlayer(nullptr);
		if (!IOLoginData::loadPlayerById(&tmpPlayer, owner)) {
			return false;
		}

		transferToDepot(&tmpPlayer);
		IOLoginData::savePlayer(&tmpPlayer);
	}
	return true;
}

bool House::transferToDepot(Player* player) const
{
	if (townid == 0 || owner == 0) {
		return false;
	}

	ItemList moveItemList;
	for (HouseTile* tile : houseTiles) {
		if (const TileItemVector* items = tile->getItemList()) {
			for (Item* item : *items) {
				if (item->isPickupable()) {
					moveItemList.push_back(item);
				} else {
					Container* container = item->getContainer();
					if (container) {
						for (Item* containerItem : container->getItemList()) {
							moveItemList.push_back(containerItem);
						}
					}
				}
			}
		}
	}

	for (Item* item : moveItemList) {
		g_game.internalMoveItem(item->getParent(), player->getInbox(), INDEX_WHEREEVER, item, item->getItemCount(), nullptr, FLAG_NOLIMIT);
	}
	return true;
}

bool House::getAccessList(uint32_t listId, std::string& list) const
{
	if (listId == GUEST_LIST) {
		guestList.getList(list);
		return true;
	} else if (listId == SUBOWNER_LIST) {
		subOwnerList.getList(list);
		return true;
	}

	Door* door = getDoorByNumber(listId);
	if (!door) {
		return false;
	}

	return door->getAccessList(list);
}

bool House::isInvited(const Player* player)
{
	return getHouseAccessLevel(player) != HOUSE_NO_INVITED;
}

void House::addDoor(Door* door)
{
	door->useThing2();
	doorList.push_back(door);
	door->setHouse(this);
	updateDoorDescription();
}

void House::removeDoor(Door* door)
{
	auto it = std::find(doorList.begin(), doorList.end(), door);
	if (it != doorList.end()) {
		door->releaseThing2();
		doorList.erase(it);
	}
}

void House::addBed(BedItem* bed)
{
	bedsList.push_back(bed);
	bed->setHouse(this);
}

Door* House::getDoorByNumber(uint32_t doorId) const
{
	for (Door* door : doorList) {
		if (door->getDoorId() == doorId) {
			return door;
		}
	}
	return nullptr;
}

Door* House::getDoorByPosition(const Position& pos)
{
	for (Door* door : doorList) {
		if (door->getPosition() == pos) {
			return door;
		}
	}
	return nullptr;
}

bool House::canEditAccessList(uint32_t listId, const Player* player)
{
	switch (getHouseAccessLevel(player)) {
		case HOUSE_OWNER:
			return true;

		case HOUSE_SUBOWNER:
			return listId == GUEST_LIST;

		default:
			return false;
	}
}

HouseTransferItem* House::getTransferItem()
{
	if (transferItem != nullptr) {
		return nullptr;
	}

	transfer_container.setParent(nullptr);
	transferItem = HouseTransferItem::createHouseTransferItem(this);
	transfer_container.__addThing(transferItem);
	return transferItem;
}

void House::resetTransferItem()
{
	if (transferItem) {
		Item* tmpItem = transferItem;
		transferItem = nullptr;
		transfer_container.setParent(nullptr);

		transfer_container.__removeThing(tmpItem, tmpItem->getItemCount());
		g_game.ReleaseItem(tmpItem);
	}
}

HouseTransferItem* HouseTransferItem::createHouseTransferItem(House* house)
{
	HouseTransferItem* transferItem = new HouseTransferItem(house);
	transferItem->useThing2();
	transferItem->setID(ITEM_DOCUMENT_RO);
	transferItem->setSubType(1);
	std::ostringstream ss;
	ss << "It is a house transfer document for '" << house->getName() << "'.";
	transferItem->setSpecialDescription(ss.str());
	return transferItem;
}

bool HouseTransferItem::onTradeEvent(TradeEvents_t event, Player* owner)
{
	switch (event) {
		case ON_TRADE_TRANSFER: {
			House* house = getHouse();
			if (house) {
				house->executeTransfer(this, owner);
			}

			g_game.internalRemoveItem(this, 1);
			break;
		}

		case ON_TRADE_CANCEL: {
			House* house = getHouse();
			if (house) {
				house->resetTransferItem();
			}
			break;
		}
	}
	return true;
}

bool House::executeTransfer(HouseTransferItem* item, Player* newOwner)
{
	if (transferItem != item) {
		return false;
	}

	setOwner(newOwner->getGUID());
	transferItem = nullptr;
	return true;
}

bool AccessList::parseList(const std::string& _list)
{
	playerList.clear();
	guildList.clear();
	expressionList.clear();
	regExList.clear();
	list = _list;

	if (_list.empty()) {
		return true;
	}

	std::istringstream listStream(_list);
	std::string line;

	while (getline(listStream, line)) {
		trimString(line);
		trim_left(line, '\t');
		trim_right(line, '\t');
		trimString(line);

		if (line.empty() || line.front() == '#' || line.length() > 100) {
			continue;
		}

		toLowerCaseString(line);

		std::string::size_type at_pos = line.find("@");
		if (at_pos != std::string::npos) {
			addGuild(line.substr(at_pos + 1));
		} else if (line.find("!") != std::string::npos || line.find("*") != std::string::npos || line.find("?") != std::string::npos) {
			addExpression(line);
		} else {
			addPlayer(line);
		}
	}
	return true;
}

bool AccessList::addPlayer(const std::string& name)
{
	uint32_t guid;
	std::string dbName = name;

	if (IOLoginData::getGuidByName(guid, dbName)) {
		if (playerList.find(guid) == playerList.end()) {
			playerList.insert(guid);
			return true;
		}
	}

	return false;
}

bool AccessList::addGuild(const std::string& guildName)
{
	uint32_t guildId;
	if (IOGuild::getGuildIdByName(guildId, guildName) && guildList.find(guildId) == guildList.end()) {
		guildList.insert(guildId);
		return true;
	}
	return false;
}

bool AccessList::addExpression(const std::string& expression)
{
	for (const std::string& expr : expressionList) {
		if (expr == expression) {
			return false;
		}
	}

	std::string outExp;
	outExp.reserve(expression.length());

	std::string metachars = ".[{}()\\+|^$";
	for (const char c : expression) {
		if (metachars.find(c) != std::string::npos) {
			outExp.push_back('\\');
		}
		outExp.push_back(c);
	}

	replaceString(outExp, "*", ".*");
	replaceString(outExp, "?", ".?");

	try {
		if (!outExp.empty()) {
			expressionList.push_back(outExp);

			if (outExp.front() == '!') {
				if (outExp.length() > 1) {
					regExList.emplace_front(std::regex(outExp.substr(1)), false);
				}
			} else {
				regExList.emplace_back(std::regex(outExp), true);
			}
		}
	} catch (...) {}
	return true;
}

bool AccessList::isInList(const Player* player)
{
	std::string name = asLowerCaseString(player->getName());
	std::cmatch what;

	for (const auto& it : regExList) {
		if (std::regex_match(name.c_str(), what, it.first)) {
			return it.second;
		}
	}

	auto playerIt = playerList.find(player->getGUID());
	if (playerIt != playerList.end()) {
		return true;
	}

	Guild* guild = player->getGuild();
	if (guild) {
		auto guildIt = guildList.find(guild->getId());
		if (guildIt != guildList.end()) {
			return true;
		}
	}
	return false;
}

void AccessList::getList(std::string& _list) const
{
	_list = list;
}

Door::Door(uint16_t _type)
	: Item(_type)
{
	house = nullptr;
	accessList = nullptr;
}

Door::~Door()
{
	delete accessList;
}

Attr_ReadValue Door::readAttr(AttrTypes_t attr, PropStream& propStream)
{
	if (ATTR_HOUSEDOORID == attr) {
		uint8_t _doorId;
		if (!propStream.GET_UCHAR(_doorId)) {
			return ATTR_READ_ERROR;
		}

		setDoorId(_doorId);
		return ATTR_READ_CONTINUE;
	} else {
		return Item::readAttr(attr, propStream);
	}
}

bool Door::serializeAttr(PropWriteStream&) const
{
	return true;
}

void Door::setHouse(House* _house)
{
	if (house != nullptr) {
		return;
	}

	house = _house;

	if (!accessList) {
		accessList = new AccessList();
	}
}

bool Door::canUse(const Player* player)
{
	if (!house) {
		return true;
	}

	if (house->getHouseAccessLevel(player) >= HOUSE_SUBOWNER) {
		return true;
	}

	return accessList->isInList(player);
}

void Door::setAccessList(const std::string& textlist)
{
	if (!accessList) {
		accessList = new AccessList();
	}

	accessList->parseList(textlist);
}

bool Door::getAccessList(std::string& list) const
{
	if (!house) {
		return false;
	}

	accessList->getList(list);
	return true;
}

void Door::stealAttributes(Item* item)
{
	Item::stealAttributes(item);

	if (Door* door = item->getDoor()) {
		std::string list;
		if (door->getAccessList(list)) {
			setAccessList(list);
		}
	}
}

void Door::onRemoved()
{
	Item::onRemoved();

	if (house) {
		house->removeDoor(this);
	}
}

Houses::Houses()
{
	std::string strRentPeriod = asLowerCaseString(g_config.getString(ConfigManager::HOUSE_RENT_PERIOD));
	if (strRentPeriod == "yearly") {
		rentPeriod = RENTPERIOD_YEARLY;
	} else if (strRentPeriod == "weekly") {
		rentPeriod = RENTPERIOD_WEEKLY;
	} else if (strRentPeriod == "monthly") {
		rentPeriod = RENTPERIOD_MONTHLY;
	} else if (strRentPeriod == "daily") {
		rentPeriod = RENTPERIOD_DAILY;
	} else {
		rentPeriod = RENTPERIOD_NEVER;
	}
}

House* Houses::getHouseByPlayerId(uint32_t playerId)
{
	for (const auto& it : houseMap) {
		if (it.second->getOwner() == playerId) {
			return it.second;
		}
	}
	return nullptr;
}

bool Houses::loadHousesXML(const std::string& filename)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename.c_str());
	if (!result) {
		std::cout << "[Error - Houses::loadHousesXML] Failed to load " << filename << ": " << result.description() << std::endl;
		return false;
	}

	for (pugi::xml_node houseNode = doc.child("houses").first_child(); houseNode; houseNode = houseNode.next_sibling()) {
		pugi::xml_attribute houseIdAttribute = houseNode.attribute("houseid");
		if (!houseIdAttribute) {
			return false;
		}

		int32_t _houseid = pugi::cast<int32_t>(houseIdAttribute.value());

		House* house = Houses::getInstance().getHouse(_houseid);
		if (!house) {
			std::cout << "Error: [Houses::loadHousesXML] Unknown house, id = " << _houseid << std::endl;
			return false;
		}

		house->setName(houseNode.attribute("name").as_string());

		Position entryPos(
			pugi::cast<uint16_t>(houseNode.attribute("entryx").value()),
			pugi::cast<uint16_t>(houseNode.attribute("entryy").value()),
			pugi::cast<uint16_t>(houseNode.attribute("entryz").value())
		);
		if (entryPos.x == 0 && entryPos.y == 0 && entryPos.z == 0) {
			std::cout << "[Warning - Houses::loadHousesXML] House entry not set"
					    << " - Name: " << house->getName()
					    << " - House id: " << _houseid << std::endl;
		}
		house->setEntryPos(entryPos);

		house->setRent(pugi::cast<uint32_t>(houseNode.attribute("rent").value()));
		house->setTownId(pugi::cast<uint32_t>(houseNode.attribute("townid").value()));

		house->setOwner(0, false);
	}
	return true;
}

bool Houses::payHouses() const
{
	if (rentPeriod == RENTPERIOD_NEVER) {
		return true;
	}

	time_t currentTime = time(nullptr);
	for (const auto& it : houseMap) {
		House* house = it.second;
		if (house->getOwner() == 0) {
			continue;
		}

		const uint32_t rent = house->getRent();
		if (rent == 0 || house->getPaidUntil() > currentTime) {
			continue;
		}

		const uint32_t ownerid = house->getOwner();
		Town* town = Towns::getInstance().getTown(house->getTownId());
		if (!town) {
			continue;
		}

		Player player(nullptr);
		if (!IOLoginData::loadPlayerById(&player, ownerid)) {
			//player doesnt exist, reset house owner
			house->setOwner(0);
			continue;
		}

		if (player.getBankBalance() >= rent) {
			player.setBankBalance(player.getBankBalance() - rent);

			time_t paidUntil = currentTime;
			switch (rentPeriod) {
				case RENTPERIOD_DAILY:
					paidUntil += 24 * 60 * 60;
					break;
				case RENTPERIOD_WEEKLY:
					paidUntil += 24 * 60 * 60 * 7;
					break;
				case RENTPERIOD_MONTHLY:
					paidUntil += 24 * 60 * 60 * 30;
					break;
				case RENTPERIOD_YEARLY:
					paidUntil += 24 * 60 * 60 * 365;
					break;
				default:
					break;
			}

			house->setPaidUntil(paidUntil);
		} else {
			if (house->getPayRentWarnings() < 7) {
				int32_t daysLeft = 7 - house->getPayRentWarnings();

				Item* letter = Item::CreateItem(ITEM_LETTER_STAMPED);
				std::string period;

				switch (rentPeriod) {
					case RENTPERIOD_DAILY:
						period = "daily";
						break;

					case RENTPERIOD_WEEKLY:
						period = "weekly";
						break;

					case RENTPERIOD_MONTHLY:
						period = "monthly";
						break;

					case RENTPERIOD_YEARLY:
						period = "annual";
						break;

					default:
						break;
				}

				std::ostringstream ss;
				ss << "Warning! \nThe " << period << " rent of " << house->getRent() << " gold for your house \"" << house->getName() << "\" is payable. Have it within " << daysLeft << " days or you will lose this house.";
				letter->setText(ss.str());
				g_game.internalAddItem(player.getInbox(), letter, INDEX_WHEREEVER, FLAG_NOLIMIT);
				house->setPayRentWarnings(house->getPayRentWarnings() + 1);
			} else {
				house->setOwner(0, true, &player);
			}
		}

		IOLoginData::savePlayer(&player);
	}
	return true;
}
