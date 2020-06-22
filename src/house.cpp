/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019  Mark Samman <mark.samman@gmail.com>
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
#include "configmanager.h"
#include "bed.h"

extern ConfigManager g_config;
extern Game g_game;

House::House(uint32_t houseId) : id(houseId) {}

void House::addTile(HouseTile* tile)
{
	tile->setFlag(TILESTATE_PROTECTIONZONE);
	houseTiles.push_back(tile);
}

std::tuple<uint32_t, uint32_t, std::string, uint32_t, std::string> House::initializeOwnerDataFromDatabase(uint32_t guid_guild, HouseType_t type) {
	if (guid_guild == 0) {
		return std::make_tuple(0, 0, "", 0, "");
	}

	Database& db = Database::getInstance();

	if (type == HOUSE_TYPE_NORMAL) {
		std::ostringstream query;
		query << "SELECT `id`, `name`, `account_id` FROM `players` WHERE `id`=" << guid_guild;
		if (DBResult_ptr result = db.storeQuery(query.str())) {
			return std::make_tuple(
				result->getNumber<uint32_t>("id"), // sqlPlayerGuid
				result->getNumber<uint32_t>("account_id"), // sqlAccountId
				result->getString("name"), // sqlPlayerName
				0, // sqlGuildId
				"" // sqlGuildName
			);
		}
		throw std::runtime_error("Error in House::setOwner - Failed to find player GUID");
	}

	// HOUSE_TYPE_GUILDHALL
	std::ostringstream query;
	query << "SELECT `g`.`id`, `g`.`name` as `guild_name`, `g`.`ownerid`, `p`.`name`, `p`.`account_id` ";
	query << "FROM `guilds` as `g` INNER JOIN `players` AS `p` ON `g`.`ownerid` = `p`.`id` ";
	query << "WHERE `g`.`id`=" << guid_guild;
	if (DBResult_ptr result = db.storeQuery(query.str())) {
		return std::make_tuple(
			result->getNumber<uint32_t>("ownerid"), // sqlPlayerGuid
			result->getNumber<uint32_t>("account_id"), // sqlAccountId
			result->getString("name"), // sqlPlayerName
			result->getNumber<uint32_t>("id"), // sqlGuildId
			result->getString("guild_name") // sqlGuildName
		);
	}
	throw std::runtime_error("Error in House::setOwner - Failed to find guild ID");
}

// Param: guid_guild is either 0 (remove owner), player->getGUID() or guild->getId()
void House::setOwner(uint32_t guid_guild, bool updateDatabase/* = true*/, Player* previousPlayer/* = nullptr*/)
{

	uint32_t sqlAccountId, sqlPlayerGuid, sqlGuildId;
	std::string sqlPlayerName, sqlGuildName;

	try {
		std::tie(sqlPlayerGuid, sqlAccountId, sqlPlayerName, sqlGuildId, sqlGuildName) = initializeOwnerDataFromDatabase(guid_guild, type);
	} catch (const std::runtime_error& err) {
		std::cout << err.what();
		return;
	}

	// If the old owner of this house is not the new owner
	if (updateDatabase && owner != guid_guild) {
		Database& db = Database::getInstance();

		std::ostringstream query;
		query << "UPDATE `houses` SET `owner` = " << guid_guild << ", `bid` = 0, `bid_end` = 0, `last_bid` = 0, `highest_bidder` = 0  WHERE `id` = " << id;
		db.executeQuery(query.str());
	}

	if (isLoaded && owner == guid_guild) {
		return;
	}

	isLoaded = true;

	// If there was a previous owner to his house
	// clean the house and return items to owner
	if (owner != 0) {
		//send items to depot
		if (previousPlayer) {
			transferToDepot(previousPlayer);
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
		ownerAccountId = 0;
		setAccessList(SUBOWNER_LIST, "");
		setAccessList(GUEST_LIST, "");

		for (Door* door : doorSet) {
			door->setAccessList("");
		}
	} else {
		std::string strRentPeriod = asLowerCaseString(g_config.getString(ConfigManager::HOUSE_RENT_PERIOD));
		time_t currentTime = time(nullptr);
		if (strRentPeriod == "yearly") {
		    currentTime += 24 * 60 * 60 * 365;
		} else if (strRentPeriod == "monthly") {
		    currentTime += 24 * 60 * 60 * 30;
		} else if (strRentPeriod == "weekly") {
		    currentTime += 24 * 60 * 60 * 7;
		} else if (strRentPeriod == "daily") {
		    currentTime += 24 * 60 * 60;
		} else {
		    currentTime = 0;
		}

		paidUntil = currentTime;
	}

	rentWarnings = 0;

	// Save the new owner to the house object
	if (guid_guild != 0) {
		owner = guid_guild;
		ownerAccountId = sqlAccountId;
		if (type == HOUSE_TYPE_GUILDHALL) {
			std::ostringstream ss;
			ss << "The " << sqlGuildName;
			ownerName = ss.str();
		} else {
			ownerName = sqlPlayerName;
		}
	}

	updateDoorDescription();
}

void House::updateDoorDescription() const
{
	std::string houseType = "house";
	if (type == HOUSE_TYPE_GUILDHALL) {
		houseType = "guildhall";
	}

	std::ostringstream ss;
	if (owner != 0) {
		ss << "It belongs to " << houseType << " '" << houseName << "'. " << ownerName << " owns this " << houseType << ".";
	} else {
		ss << "It belongs to " << houseType << " '" << houseName << "'. Nobody owns this " << houseType << ".";

		const int32_t housePrice = g_config.getNumber(ConfigManager::HOUSE_PRICE);
		if (housePrice != -1 && g_config.getBoolean(ConfigManager::HOUSE_DOOR_SHOW_PRICE)) {
			ss << " It costs " << (houseTiles.size() * housePrice) << " gold coins.";
		}
	}

	for (const auto& it : doorSet) {
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

	uint32_t guid = player->getGUID();

	if (type == HOUSE_TYPE_NORMAL) {
		if (g_config.getBoolean(ConfigManager::HOUSE_OWNED_BY_ACCOUNT)) {
			if (ownerAccountId == player->getAccount()) {
				return HOUSE_OWNER;
			}
		}
		if (guid == owner) {
			return HOUSE_OWNER;
		}

	} else { // HOUSE_TYPE_GUILDHALL
		Guild* guild = player->getGuild();
		if (guild && guild->getId() == owner) {
			if (guild->getOwnerGUID() == guid) {
				return HOUSE_OWNER;
			}
			if (player->getGuildRank() == guild->getRankByLevel(2)) {
				return HOUSE_SUBOWNER;
			}
		}
	}

	if (subOwnerList.isInList(player)) {
		return HOUSE_SUBOWNER;
	}
	if (guestList.isInList(player)) {
		return HOUSE_GUEST;
	}

	return HOUSE_NOT_INVITED;
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
	if (g_game.internalTeleport(target, getEntryPosition()) == RETURNVALUE_NOERROR) {
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
	if (townId == 0 || owner == 0) {
		return false;
	}

	if (type == HOUSE_TYPE_NORMAL) {
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
	} else { // HOUSE_TYPE_GUILDHALL
		Guild* guild = g_game.getGuild(owner);
		if (!guild) {
			guild = IOGuild::loadGuild(owner);
			if (!guild) {
				std::cout << "Warning: [Houses::transferToDepot] Failed to find guild associated to guildhall = " << id << ". Guild = " << owner << std::endl;
				return false;
			}
		}
		Player* player = g_game.getPlayerByGUID(guild->getOwnerGUID());
		if (player) {
			transferToDepot(player);
		} else {
			Player tmpPlayer(nullptr);
			if (!IOLoginData::loadPlayerById(&tmpPlayer, guild->getOwnerGUID())) {
				return false;
			}

			transferToDepot(&tmpPlayer);
			IOLoginData::savePlayer(&tmpPlayer);
		}
	}

	return true;
}

bool House::transferToDepot(Player* player) const
{
	if (townId == 0 || owner == 0) {
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
	return getHouseAccessLevel(player) != HOUSE_NOT_INVITED;
}

void House::addDoor(Door* door)
{
	door->incrementReferenceCounter();
	doorSet.insert(door);
	door->setHouse(this);
	updateDoorDescription();
}

void House::removeDoor(Door* door)
{
	auto it = doorSet.find(door);
	if (it != doorSet.end()) {
		door->decrementReferenceCounter();
		doorSet.erase(it);
	}
}

void House::addBed(BedItem* bed)
{
	bedsList.push_back(bed);
	bed->setHouse(this);
}

Door* House::getDoorByNumber(uint32_t doorId) const
{
	for (Door* door : doorSet) {
		if (door->getDoorId() == doorId) {
			return door;
		}
	}
	return nullptr;
}

Door* House::getDoorByPosition(const Position& pos)
{
	for (Door* door : doorSet) {
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
	transfer_container.addThing(transferItem);
	return transferItem;
}

void House::resetTransferItem()
{
	if (transferItem) {
		Item* tmpItem = transferItem;
		transferItem = nullptr;
		transfer_container.setParent(nullptr);

		transfer_container.removeThing(tmpItem, tmpItem->getItemCount());
		g_game.ReleaseItem(tmpItem);
	}
}

HouseTransferItem* HouseTransferItem::createHouseTransferItem(House* house)
{
	HouseTransferItem* transferItem = new HouseTransferItem(house);
	transferItem->incrementReferenceCounter();
	transferItem->setID(ITEM_DOCUMENT_RO);
	transferItem->setSubType(1);
	std::ostringstream ss;
	ss << "It is a house transfer document for '" << house->getName() << "'.";
	transferItem->setSpecialDescription(ss.str());
	return transferItem;
}

void HouseTransferItem::onTradeEvent(TradeEvents_t event, Player* owner)
{
	if (event == ON_TRADE_TRANSFER) {
		if (house) {
			house->executeTransfer(this, owner);
		}

		g_game.internalRemoveItem(this, 1);
	} else if (event == ON_TRADE_CANCEL) {
		if (house) {
			house->resetTransferItem();
		}
	}
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

void AccessList::parseList(const std::string& list)
{
	playerList.clear();
	guildRankList.clear();
	allowEveryone = false;
	this->list = list;
	if (list.empty()) {
		return;
	}

	std::istringstream listStream(list);
	std::string line;

	int lineNo = 1;
	while (getline(listStream, line)) {
		if (++lineNo > 100) {
			break;
		}

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
			if (at_pos == 0) {
				addGuild(line.substr(1));
			} else {
				addGuildRank(line.substr(0, at_pos - 1), line.substr(at_pos + 1));
			}
		} else if (line == "*") {
			allowEveryone = true;
		} else if (line.find("!") != std::string::npos || line.find("*") != std::string::npos || line.find("?") != std::string::npos) {
			continue; // regexp no longer supported
		} else {
			addPlayer(line);
		}
	}
}

void AccessList::addPlayer(const std::string& name)
{
	Player* player = g_game.getPlayerByName(name);
	if (player) {
		playerList.insert(player->getGUID());
	} else {
		uint32_t guid = IOLoginData::getGuidByName(name);
		if (guid != 0) {
			playerList.insert(guid);
		}
	}
}

namespace {

const Guild* getGuildByName(const std::string& name)
{
	uint32_t guildId = IOGuild::getGuildIdByName(name);
	if (guildId == 0) {
		return nullptr;
	}

	const Guild* guild = g_game.getGuild(guildId);
	if (guild) {
		return guild;
	}

	return IOGuild::loadGuild(guildId);
}

}

void AccessList::addGuild(const std::string& name)
{
	const Guild* guild = getGuildByName(name);
	if (guild) {
		for (auto rank : guild->getRanks()) {
			guildRankList.insert(rank->id);
		}
	}
}

void AccessList::addGuildRank(const std::string& name, const std::string& rankName)
{
	const Guild* guild = getGuildByName(name);
	if (guild) {
		GuildRank_ptr rank = guild->getRankByName(rankName);
		if (rank) {
			guildRankList.insert(rank->id);
		}
	}
}

bool AccessList::isInList(const Player* player)
{
	if (allowEveryone) {
		return true;
	}

	auto playerIt = playerList.find(player->getGUID());
	if (playerIt != playerList.end()) {
		return true;
	}

	GuildRank_ptr rank = player->getGuildRank();
	return rank && guildRankList.find(rank->id) != guildRankList.end();
}

void AccessList::getList(std::string& list) const
{
	list = this->list;
}

Door::Door(uint16_t type) :	Item(type) {}

Attr_ReadValue Door::readAttr(AttrTypes_t attr, PropStream& propStream)
{
	if (attr == ATTR_HOUSEDOORID) {
		uint8_t doorId;
		if (!propStream.read<uint8_t>(doorId)) {
			return ATTR_READ_ERROR;
		}

		setDoorId(doorId);
		return ATTR_READ_CONTINUE;
	}
	return Item::readAttr(attr, propStream);
}

void Door::setHouse(House* house)
{
	if (this->house != nullptr) {
		return;
	}

	this->house = house;

	if (!accessList) {
		accessList.reset(new AccessList());
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
		accessList.reset(new AccessList());
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

void Door::onRemoved()
{
	Item::onRemoved();

	if (house) {
		house->removeDoor(this);
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
		printXMLError("Error - Houses::loadHousesXML", filename, result);
		return false;
	}

	for (auto houseNode : doc.child("houses").children()) {
		pugi::xml_attribute houseIdAttribute = houseNode.attribute("houseid");
		if (!houseIdAttribute) {
			return false;
		}

		int32_t houseId = pugi::cast<int32_t>(houseIdAttribute.value());

		House* house = getHouse(houseId);
		if (!house) {
			std::cout << "Error: [Houses::loadHousesXML] Unknown house, id = " << houseId << std::endl;
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
					    << " - House id: " << houseId << std::endl;
		}
		house->setEntryPos(entryPos);

		house->setRent(pugi::cast<uint32_t>(houseNode.attribute("rent").value()));
		house->setTownId(pugi::cast<uint32_t>(houseNode.attribute("townid").value()));
		if (houseNode.attribute("guildhall").as_bool()) {
			house->setType(HOUSE_TYPE_GUILDHALL);
		}

		house->setOwner(0, false);
	}
	return true;
}

time_t Houses::increasePaidUntil(RentPeriod_t rentPeriod, time_t paidUntil) const
{
	switch (rentPeriod) {
		case RENTPERIOD_DAILY:
			return paidUntil += 24 * 60 * 60;
		case RENTPERIOD_WEEKLY:
			return paidUntil += 24 * 60 * 60 * 7;
		case RENTPERIOD_MONTHLY:
			return paidUntil += 24 * 60 * 60 * 30;
		case RENTPERIOD_YEARLY:
			return paidUntil += 24 * 60 * 60 * 365;
		case RENTPERIOD_DEV:
			return paidUntil += 5 * 60;
		default:
			return paidUntil;
	}
}

std::string Houses::getRentPeriod(RentPeriod_t rentPeriod) const
{
	switch (rentPeriod) {
		case RENTPERIOD_DAILY:
			return "daily";
		case RENTPERIOD_WEEKLY:
			return "weekly";
		case RENTPERIOD_MONTHLY:
			return "monthly";
		case RENTPERIOD_YEARLY:
			return "annual";
		case RENTPERIOD_DEV:
			return "dev";
		default:
			return "never";
	}
}

void Houses::payHouses(RentPeriod_t rentPeriod) const
{
	if (rentPeriod == RENTPERIOD_NEVER) {
		return;
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

		const uint32_t ownerId = house->getOwner();
		Town* town = g_game.map.towns.getTown(house->getTownId());
		if (!town) {
			continue;
		}

		if (house->getType() == HOUSE_TYPE_NORMAL) {
			Player player(nullptr);
			if (!IOLoginData::loadPlayerById(&player, ownerId)) {
				// Player doesn't exist, reset house owner
				house->setOwner(0);
				continue;
			}

			if (player.getBankBalance() >= rent) {
				player.setBankBalance(player.getBankBalance() - rent);

				time_t paidUntil = increasePaidUntil(rentPeriod, currentTime);
				house->setPaidUntil(paidUntil);
			} else {
				if (house->getPayRentWarnings() < 7) {
					int32_t daysLeft = 7 - house->getPayRentWarnings();

					Item* letter = Item::CreateItem(ITEM_LETTER_STAMPED);
					std::string period = getRentPeriod(rentPeriod);

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

		} else { // HOUSE_TYPE_GUILDHALL
			Guild* guild = g_game.getGuild(ownerId);
			if (!guild) {
				guild = IOGuild::loadGuild(ownerId);
				if (!guild) {
					house->setOwner(0);
					continue;
				}
			}

			// If guild can afford paying rent
			if (guild->getBankBalance() >= rent) {
				std::cout << "[Info - Houses::payHouses] Paying rent info"
				    << " - Name: " << house->getName()
				    << " - House id: " << house->getId()
					<< " - Guild: " << guild->getName()
					<< " - Balance " << guild->getBankBalance()
					<< " - Rent " << rent
					<< " - New balance " << guild->getBankBalance() - rent << std::endl;
				guild->setBankBalance(guild->getBankBalance() - rent);

				time_t paidUntil = increasePaidUntil(rentPeriod, currentTime);
				house->setPaidUntil(paidUntil);
			} else { // guild cannot afford rent
				std::cout << "a guild cannot afford their rent " << house->getPayRentWarnings() << std::endl;
				Player player(nullptr);
				if (!IOLoginData::loadPlayerById(&player, guild->getOwnerGUID())) {
					// Player doesn't exist, reset house owner
					house->setOwner(0);
					std::ostringstream ss;
					ss << "Error: Guild " << guild->getName() << " has an owner that does not exist: " << guild->getOwnerGUID();
					std::cout << ss.str() << std::endl;
					continue;
				}

				if (house->getPayRentWarnings() < 7) {
					int32_t daysLeft = 7 - house->getPayRentWarnings();

					Item* letter = Item::CreateItem(ITEM_LETTER_STAMPED);
					std::string period = getRentPeriod(rentPeriod);

					std::ostringstream ss;
					ss << "Warning! \nThe " << period << " rent of " << house->getRent() << " gold for your house \"" << house->getName() << "\" is payable. Have it within " << daysLeft << " days or you will lose this house.";
					letter->setText(ss.str());
					g_game.internalAddItem(player.getInbox(), letter, INDEX_WHEREEVER, FLAG_NOLIMIT);
					house->setPayRentWarnings(house->getPayRentWarnings() + 1);
				} else {
					house->setOwner(0, true, &player);
				}
			}
		}
	}
}
