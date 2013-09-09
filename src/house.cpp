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

#include <algorithm>

#include "house.h"
#include "iologindata.h"
#include "game.h"
#include "town.h"
#include "configmanager.h"
#include "tools.h"
#include "beds.h"
#include "commands.h"

extern Commands g_commands;
extern ConfigManager g_config;
extern Game g_game;

House::House(uint32_t _houseid) :
	transfer_container(ITEM_LOCKER1)
{
	isLoaded = false;
	houseName = "Forgotten headquarter (Flat 1, Area 42)";
	houseOwner = 0;
	posEntry.x = 0;
	posEntry.y = 0;
	posEntry.z = 0;
	paidUntil = 0;
	houseid = _houseid;
	rentWarnings = 0;
	rent = 0;
	townid = 0;
	transferItem = NULL;
}

void House::addTile(HouseTile* tile)
{
	tile->setFlag(TILESTATE_PROTECTIONZONE);
	houseTiles.push_back(tile);
}

void House::setHouseOwner(uint32_t guid, bool updateDatabase/* = true*/, Player* player/* = NULL*/)
{
	if (updateDatabase) {
		Database* db = Database::getInstance();

		std::ostringstream query;
		query << "UPDATE `houses` SET `owner` = " << guid << " WHERE `id` = " << houseid;
		db->executeQuery(query.str());
	}

	if (isLoaded && houseOwner == guid) {
		return;
	}

	isLoaded = true;
	
	if (houseOwner) {
		//send items to depot
		if (player) {
			transferToDepot(player);
		} else {
			transferToDepot();
		}

		for (HouseTile* tile : houseTiles) {
			if (const CreatureVector* creatures = tile->getCreatures()) {
				for (Creature* creature : *creatures) {
					kickPlayer(NULL, creature->getPlayer());
				}
			}
		}

		// Remove players from beds
		for (BedItem* bed : bedsList) {
			if (bed->getSleeper() != 0) {
				bed->wakeUp(NULL);
			}
		}

		//clean access lists
		houseOwner = 0;
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

	if (guid != 0 && IOLoginData::getInstance()->getNameByGuid(guid, name)) {
		houseOwner = guid;
		houseOwnerName = name;
	}

	updateDoorDescription();
}

void House::updateDoorDescription()
{
	std::ostringstream ss;
	if (houseOwner != 0) {
		ss << "It belongs to house '" << houseName << "'. " << houseOwnerName << " owns this house.";
	} else {
		ss << "It belongs to house '" << houseName << "'. Nobody owns this house.";

		Command* command = g_commands.getCommand("!buyhouse");
		if (command && command->accountType == ACCOUNT_TYPE_NORMAL) {
			ss << " It costs " << (houseTiles.size() * g_config.getNumber(ConfigManager::HOUSE_PRICE)) << " gold coins.";
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

	if (player->getGUID() == houseOwner) {
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
		g_game.addMagicEffect(oldPosition, NM_ME_POFF);
		g_game.addMagicEffect(getEntryPosition(), NM_ME_TELEPORT);
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
			for (Creature* creature : *creatures) {
				Player* player = creature->getPlayer();
				if (player && !isInvited(player)) {
					kickPlayer(NULL, player);
				}
			}
		}
	}
}

bool House::transferToDepot()
{
	if (townid == 0 || houseOwner == 0) {
		return false;
	}

	Player* player = g_game.getPlayerByGUID(houseOwner);
	if (!player) {
		player = new Player(NULL);
		if (!IOLoginData::getInstance()->loadPlayerById(player, houseOwner)) {
			delete player;
			return false;
		}
	}
	
	transferToDepot(player);

	if (player->isOffline()) {
		IOLoginData::getInstance()->savePlayer(player);
		delete player;
	}
	return true;
}

bool House::transferToDepot(Player* player)
{
	if (townid == 0 || houseOwner == 0) {
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
						for (ItemDeque::const_iterator cit = container->getItems(); cit != container->getEnd(); ++cit) {
							moveItemList.push_back(*cit);
						}
					}
				}
			}
		}
	}

	for (ItemList::iterator it = moveItemList.begin(); it != moveItemList.end(); ++it) {
		Item* item = *it;
		g_game.internalMoveItem(item->getParent(), player->getInbox(), INDEX_WHEREEVER,
		                        item, item->getItemCount(), NULL, FLAG_NOLIMIT);
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
	HouseDoorList::iterator it = std::find(doorList.begin(), doorList.end(), door);

	if (it != doorList.end()) {
		(*it)->releaseThing2();
		doorList.erase(it);
	}
}

void House::addBed(BedItem* bed)
{
	bedsList.push_back(bed);
	bed->setHouse(this);
}

Door* House::getDoorByNumber(uint32_t doorId)
{
	HouseDoorList::iterator it;

	for (it = doorList.begin(); it != doorList.end(); ++it) {
		if ((*it)->getDoorId() == doorId) {
			return *it;
		}
	}

	return NULL;
}

Door* House::getDoorByNumber(uint32_t doorId) const
{
	HouseDoorList::const_iterator it;

	for (it = doorList.begin(); it != doorList.end(); ++it) {
		if ((*it)->getDoorId() == doorId) {
			return *it;
		}
	}

	return NULL;
}

Door* House::getDoorByPosition(const Position& pos)
{
	for (HouseDoorList::iterator it = doorList.begin(); it != doorList.end(); ++it) {
		if ((*it)->getPosition() == pos) {
			return *it;
		}
	}

	return NULL;
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
	if (transferItem != NULL) {
		return NULL;
	}

	transfer_container.setParent(NULL);
	transferItem = HouseTransferItem::createHouseTransferItem(this);
	transfer_container.__addThing(transferItem);
	return transferItem;
}

void House::resetTransferItem()
{
	if (transferItem) {
		Item* tmpItem = transferItem;
		transferItem = NULL;
		transfer_container.setParent(NULL);

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
	House* house;

	switch (event) {
		case ON_TRADE_TRANSFER: {
			house = getHouse();

			if (house) {
				house->executeTransfer(this, owner);
			}

			g_game.internalRemoveItem(this, 1);
			break;
		}

		case ON_TRADE_CANCEL: {
			house = getHouse();

			if (house) {
				house->resetTransferItem();
			}

			break;
		}

		default:
			break;
	}

	return true;
}

bool House::executeTransfer(HouseTransferItem* item, Player* newOwner)
{
	if (transferItem != item) {
		return false;
	}

	setHouseOwner(newOwner->getGUID());
	transferItem = NULL;
	return true;
}

AccessList::AccessList()
{
	//
}

AccessList::~AccessList()
{
	//
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
		trim_left(line, "\t");
		trim_right(line, "\t");
		trimString(line);

		std::transform(line.begin(), line.end(), line.begin(), tolower);

		if (line.substr(0, 1) == "#" || line.length() > 100) {
			continue;
		}

		if (line.find("@") != std::string::npos) {
			std::string::size_type pos = line.find("@");
			addGuild(line.substr(pos + 1), "");
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

	if (IOLoginData::getInstance()->getGuidByName(guid, dbName)) {
		if (playerList.find(guid) == playerList.end()) {
			playerList.insert(guid);
			return true;
		}
	}

	return false;
}

bool AccessList::addGuild(const std::string& guildName, const std::string& rank)
{
	uint32_t guildId;

	if (IOGuild::getInstance()->getGuildIdByName(guildId, guildName)) {
		if (guildId != 0 && guildList.find(guildId) == guildList.end()) {
			guildList.insert(guildId);
			return true;
		}
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
	for (std::string::const_iterator it = expression.begin(); it != expression.end(); ++it) {
		if (metachars.find(*it) != std::string::npos) {
			outExp += "\\";
		}
		outExp += (*it);
	}

	replaceString(outExp, "*", ".*");
	replaceString(outExp, "?", ".?");

	try {
		if (!outExp.empty()) {
			expressionList.push_back(outExp);

			if (outExp.substr(0, 1) == "!") {
				if (outExp.length() > 1) {
					regExList.push_front(std::make_pair(std::regex(outExp.substr(1)), false));
				}
			} else {
				regExList.push_back(std::make_pair(std::regex(outExp), true));
			}
		}
	} catch (...) {}

	return true;
}

bool AccessList::isInList(const Player* player)
{
	RegExList::iterator it;
	std::string name = player->getName();
	std::cmatch what;

	std::transform(name.begin(), name.end(), name.begin(), tolower);

	for (it = regExList.begin(); it != regExList.end(); ++it) {
		if (std::regex_match(name.c_str(), what, it->first)) {
			return it->second;
		}
	}

	PlayerList::iterator playerIt = playerList.find(player->getGUID());

	if (playerIt != playerList.end()) {
		return true;
	}

	Guild* guild = player->getGuild();

	if (guild) {
		GuildList::iterator guildIt = guildList.find(guild->getId());

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
	house = NULL;
	accessList = NULL;
}

Door::~Door()
{
	delete accessList;
}

Attr_ReadValue Door::readAttr(AttrTypes_t attr, PropStream& propStream)
{
	if (ATTR_HOUSEDOORID == attr) {
		unsigned char _doorId = 0;

		if (!propStream.GET_UCHAR(_doorId)) {
			return ATTR_READ_ERROR;
		}

		setDoorId(_doorId);
		return ATTR_READ_CONTINUE;
	} else {
		return Item::readAttr(attr, propStream);
	}
}

bool Door::serializeAttr(PropWriteStream& propWriteStream) const
{
	return true;
}

void Door::setHouse(House* _house)
{
	if (house != NULL) {
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

void Door::copyAttributes(Item* item)
{
	Item::copyAttributes(item);

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
	rentPeriod = RENTPERIOD_NEVER;
	std::string strRentPeriod = asLowerCaseString(g_config.getString(ConfigManager::HOUSE_RENT_PERIOD));

	if (strRentPeriod == "yearly") {
		rentPeriod = RENTPERIOD_YEARLY;
	} else if (strRentPeriod == "weekly") {
		rentPeriod = RENTPERIOD_WEEKLY;
	} else if (strRentPeriod == "monthly") {
		rentPeriod = RENTPERIOD_MONTHLY;
	} else if (strRentPeriod == "daily") {
		rentPeriod = RENTPERIOD_DAILY;
	}
}

House* Houses::getHouseByPlayerId(uint32_t playerId)
{
	for (HouseMap::iterator it = houseMap.begin(); it != houseMap.end(); ++it) {
		House* house = it->second;

		if (house->getHouseOwner() == playerId) {
			return house;
		}
	}

	return NULL;
}

bool Houses::loadHousesXML(const std::string& filename)
{
	xmlDocPtr doc = xmlParseFile(filename.c_str());

	if (doc) {
		xmlNodePtr root, houseNode;
		root = xmlDocGetRootElement(doc);

		if (xmlStrcmp(root->name, (const xmlChar*)"houses") != 0) {
			xmlFreeDoc(doc);
			return false;
		}

		int32_t intValue;
		std::string strValue;

		houseNode = root->children;

		while (houseNode) {
			if (xmlStrcmp(houseNode->name, (const xmlChar*)"house") == 0) {
				int32_t _houseid = 0;
				Position entryPos(0, 0, 0);

				if (!readXMLInteger(houseNode, "houseid", _houseid)) {
					xmlFreeDoc(doc);
					return false;
				}

				House* house = Houses::getInstance().getHouse(_houseid);

				if (!house) {
					std::cout << "Error: [Houses::loadHousesXML] Unknown house, id = " << _houseid << std::endl;
					xmlFreeDoc(doc);
					return false;
				}

				if (readXMLString(houseNode, "name", strValue)) {
					house->setName(strValue);
				}

				if (readXMLInteger(houseNode, "entryx", intValue)) {
					entryPos.x = intValue;
				}

				if (readXMLInteger(houseNode, "entryy", intValue)) {
					entryPos.y = intValue;
				}

				if (readXMLInteger(houseNode, "entryz", intValue)) {
					entryPos.z = intValue;
				}

				if (entryPos.x == 0 && entryPos.y == 0 && entryPos.z == 0) {
					std::cout << "Warning: [Houses::loadHousesXML] House entry not set"
					          << " - Name: " << house->getName()
					          << " - House id: " << _houseid << std::endl;
				}

				house->setEntryPos(entryPos);

				if (readXMLInteger(houseNode, "rent", intValue)) {
					house->setRent(intValue);
				}

				if (readXMLInteger(houseNode, "townid", intValue)) {
					house->setTownId(intValue);
				}

				house->setHouseOwner(0, false);
			}

			houseNode = houseNode->next;
		}

		xmlFreeDoc(doc);
		return true;
	}

	return false;
}

bool Houses::payHouses()
{
	if (rentPeriod == RENTPERIOD_NEVER) {
		return true;
	}

	time_t currentTime = time(NULL);

	for (HouseMap::iterator it = houseMap.begin(); it != houseMap.end(); ++it) {
		House* house = it->second;

		if (house->getHouseOwner() != 0) {
			uint32_t ownerid = house->getHouseOwner();
			Town* town = Towns::getInstance().getTown(house->getTownId());

			if (!town) {
#ifdef __DEBUG_HOUSES__
				std::cout << "Warning: [Houses::payHouses] town = NULL, townid = " <<
				          house->getTownId() << ", houseid = " << house->getHouseId() << std::endl;
#endif
				continue;
			}

			Player* player = g_game.getPlayerByGUID(ownerid);
			if (!player) {
				player = new Player(NULL);
				if (!IOLoginData::getInstance()->loadPlayerById(player, ownerid)) {
					//player doesnt exist, reset house owner
					house->setHouseOwner(0);
					delete player;
					continue;
				}
			}

			if (player->getBankBalance() >= house->getRent()) {
				player->setBankBalance(player->getBankBalance() - house->getRent());

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
					g_game.internalAddItem(player->getInbox(), letter, INDEX_WHEREEVER, FLAG_NOLIMIT);
					house->setPayRentWarnings(house->getPayRentWarnings() + 1);
				} else {
					house->setHouseOwner(0, true, player);
				}
			}

			if (player->isOffline()) {
				IOLoginData::getInstance()->savePlayer(player);
				delete player;
			}
		}
	}
	return true;
}
