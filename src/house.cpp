// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "house.h"

#include "bed.h"
#include "configmanager.h"
#include "game.h"
#include "housetile.h"
#include "inbox.h"
#include "iologindata.h"
#include "pugicast.h"

extern Game g_game;

House::House(uint32_t houseId) : id(houseId) {}

void House::addTile(std::shared_ptr<HouseTile> tile)
{
	tile->setFlag(TILESTATE_PROTECTIONZONE);
	houseTiles.push_back(std::move(tile));
}

void House::setOwner(uint32_t guid, bool updateDatabase /* = true*/,
                     const std::shared_ptr<Player>& player /* = nullptr*/)
{
	if (updateDatabase && owner != guid) {
		Database& db = Database::getInstance();
		db.executeQuery(fmt::format(
		    "UPDATE `houses` SET `owner` = {:d}, `bid` = 0, `bid_end` = 0, `last_bid` = 0, `highest_bidder` = 0  WHERE `id` = {:d}",
		    guid, id));
	}

	if (isLoaded && owner == guid) {
		return;
	}

	isLoaded = true;

	if (owner != 0) {
		// send items to depot
		if (player) {
			transferToDepot(player);
		} else {
			transferToDepot();
		}

		for (const auto& tile : houseTiles) {
			if (const CreatureVector* creatures = tile->getCreatures()) {
				for (int32_t i = creatures->size(); --i >= 0;) {
					kickPlayer(nullptr, (*creatures)[i]->getPlayer());
				}
			}
		}

		// Remove players from beds
		for (const auto& bed : bedsList) {
			if (bed->getSleeper() != 0) {
				bed->wakeUp(nullptr);
			}
		}

		// clean access lists
		owner = 0;
		ownerAccountId = 0;
		ownerName.clear();
		setAccessList(SUBOWNER_LIST, "");
		setAccessList(GUEST_LIST, "");

		for (const auto& door : doorSet) {
			door->setAccessList("");
		}
	} else {
		std::string strRentPeriod = boost::algorithm::to_lower_copy(getString(ConfigManager::HOUSE_RENT_PERIOD));
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

	if (guid != 0) {
		std::string name = IOLoginData::getNameByGuid(guid);
		if (!name.empty()) {
			owner = guid;
			ownerName = name;
			ownerAccountId = IOLoginData::getAccountIdByPlayerName(name);
		}
	}
}

AccessHouseLevel_t House::getHouseAccessLevel(const std::shared_ptr<const Player>& player) const
{
	if (!player) {
		return HOUSE_OWNER;
	}

	if (getBoolean(ConfigManager::HOUSE_OWNED_BY_ACCOUNT)) {
		if (ownerAccountId == player->getAccount()) {
			return HOUSE_OWNER;
		}
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

	return HOUSE_NOT_INVITED;
}

bool House::kickPlayer(const std::shared_ptr<Player>& player, const std::shared_ptr<Player>& target)
{
	if (!target) {
		return false;
	}

	const auto& tile = target->getTile();
	if (!tile) {
		return false;
	}

	if (const auto& houseTile = tile->getHouseTile(); !houseTile || houseTile->getHouse() != this) {
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

void House::setAccessList(uint32_t listId, std::string_view textlist)
{
	if (listId == GUEST_LIST) {
		guestList.parseList(textlist);
	} else if (listId == SUBOWNER_LIST) {
		subOwnerList.parseList(textlist);
	} else {
		if (const auto& door = getDoorByNumber(listId)) {
			door->setAccessList(textlist);
		}

		// We do not have to kick anyone
		return;
	}

	// kick uninvited players
	for (const auto& tile : houseTiles) {
		if (CreatureVector* creatures = tile->getCreatures()) {
			for (int32_t i = creatures->size(); --i >= 0;) {
				const auto& player = (*creatures)[i]->getPlayer();
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

	if (const auto& player = g_game.getPlayerByGUID(owner)) {
		transferToDepot(player);
	} else {
		auto tmpPlayer = std::make_shared<Player>(nullptr);
		if (!IOLoginData::loadPlayerById(tmpPlayer, owner)) {
			return false;
		}

		transferToDepot(tmpPlayer);
		IOLoginData::savePlayer(tmpPlayer);
	}
	return true;
}

bool House::transferToDepot(const std::shared_ptr<Player>& player) const
{
	if (townId == 0 || owner == 0) {
		return false;
	}

	ItemList moveItemList;
	for (const auto& tile : houseTiles) {
		if (const TileItemVector* items = tile->getItemList()) {
			for (const auto& item : *items) {
				if (item->isPickupable()) {
					moveItemList.push_back(item);
				} else if (const auto& container = item->getContainer()) {
					for (const auto& containerItem : container->getItemList()) {
						moveItemList.push_back(containerItem);
					}
				}
			}
		}
	}

	for (const auto& item : moveItemList) {
		g_game.internalMoveItem(item->getParent(), player->getInbox(), INDEX_WHEREEVER, item, item->getItemCount(),
		                        nullptr, FLAG_NOLIMIT);
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

	const auto& door = getDoorByNumber(listId);
	if (!door) {
		return false;
	}

	return door->getAccessList(list);
}

bool House::isInvited(const std::shared_ptr<const Player>& player) const
{
	return getHouseAccessLevel(player) != HOUSE_NOT_INVITED;
}

void House::addDoor(const std::shared_ptr<Door>& door)
{
	doorSet.insert(door);
	door->setHouse(this);
}

void House::removeDoor(const std::shared_ptr<Door>& door)
{
	auto it = doorSet.find(door);
	if (it != doorSet.end()) {
		doorSet.erase(it);
	}
}

void House::addBed(const std::shared_ptr<BedItem>& bed)
{
	bedsList.push_back(bed);
	bed->setHouse(this);
}

std::shared_ptr<Door> House::getDoorByNumber(uint32_t doorId) const
{
	for (const auto& door : doorSet) {
		if (door->getDoorId() == doorId) {
			return door;
		}
	}
	return nullptr;
}

std::shared_ptr<Door> House::getDoorByPosition(const Position& pos)
{
	for (const auto& door : doorSet) {
		if (door->getPosition() == pos) {
			return door;
		}
	}
	return nullptr;
}

bool House::canEditAccessList(uint32_t listId, const std::shared_ptr<const Player>& player)
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

std::shared_ptr<HouseTransferItem> House::getTransferItem()
{
	if (transferItem) {
		return nullptr;
	}

	transferContainer.setParent(nullptr);
	auto transferItem = HouseTransferItem::createHouseTransferItem(this);
	transferContainer.addThing(transferItem);
	return transferItem;
}

void House::resetTransferItem()
{
	if (transferItem) {
		auto tmpItem = transferItem;
		transferItem = nullptr;
		transferContainer.setParent(nullptr);

		transferContainer.removeThing(tmpItem, tmpItem->getItemCount());
	}
}

std::shared_ptr<HouseTransferItem> HouseTransferItem::createHouseTransferItem(House* house)
{
	const auto transferItem = std::make_shared<HouseTransferItem>(house);
	transferItem->setID(ITEM_DOCUMENT_RO);
	transferItem->setSubType(1);
	transferItem->setSpecialDescription(fmt::format("It is a house transfer document for '{:s}'.", house->getName()));
	return transferItem;
}

void HouseTransferItem::onTradeEvent(TradeEvents_t event, const std::shared_ptr<Player>& owner)
{
	if (event == ON_TRADE_TRANSFER) {
		if (house) {
			house->executeTransfer(std::static_pointer_cast<HouseTransferItem>(getItem()), owner);
		}

		g_game.internalRemoveItem(getItem(), 1);
	} else if (event == ON_TRADE_CANCEL) {
		if (house) {
			house->resetTransferItem();
		}
	}
}

bool House::executeTransfer(const std::shared_ptr<HouseTransferItem>& item, const std::shared_ptr<Player>& newOwner)
{
	if (transferItem != item) {
		return false;
	}

	setOwner(newOwner->getGUID());
	transferItem = nullptr;
	return true;
}

void AccessList::parseList(std::string_view list)
{
	playerList.clear();
	guildRankList.clear();
	allowEveryone = false;
	this->list = list;
	if (list.empty()) {
		return;
	}

	std::istringstream listStream{this->list};
	std::string line;

	uint16_t lineNo = 1;
	while (getline(listStream, line)) {
		if (++lineNo > 100) {
			break;
		}

		boost::algorithm::trim(line);
		if (line.empty() || line.front() == '#' || line.length() > 100) {
			continue;
		}

		std::string::size_type at_pos = line.find("@");
		if (at_pos != std::string::npos) {
			if (at_pos == 0) {
				addGuild(line.substr(1));
			} else {
				addGuildRank(line.substr(0, at_pos), line.substr(at_pos + 1));
			}
		} else if (line == "*") {
			allowEveryone = true;
		} else if (line.find("!") != std::string::npos || line.find("*") != std::string::npos ||
		           line.find("?") != std::string::npos) {
			continue; // regexp no longer supported
		} else {
			addPlayer(line);
		}
	}
}

void AccessList::addPlayer(const std::string& name)
{
	if (const auto& player = g_game.getPlayerByName(name)) {
		playerList.insert(player->getGUID());
	} else if (uint32_t guid = IOLoginData::getGuidByName(name)) {
		playerList.insert(guid);
	}
}

namespace {

const Guild_ptr getGuildByName(const std::string& name)
{
	uint32_t guildId = IOGuild::getGuildIdByName(name);
	if (guildId == 0) {
		return nullptr;
	}

	if (const auto& guild = g_game.getGuild(guildId)) {
		return guild;
	}

	return IOGuild::loadGuild(guildId);
}

} // namespace

void AccessList::addGuild(const std::string& name)
{
	if (const auto& guild = getGuildByName(name)) {
		for (const auto& rank : guild->getRanks()) {
			guildRankList.insert(rank->id);
		}
	}
}

void AccessList::addGuildRank(const std::string& name, const std::string& rankName)
{
	if (const auto& guild = getGuildByName(name)) {
		if (const auto& rank = guild->getRankByName(rankName)) {
			guildRankList.insert(rank->id);
		}
	}
}

bool AccessList::isInList(const std::shared_ptr<const Player>& player) const
{
	if (allowEveryone) {
		return true;
	}

	const auto& playerIt = playerList.find(player->getGUID());
	if (playerIt != playerList.end()) {
		return true;
	}

	const auto& rank = player->getGuildRank();
	return rank && guildRankList.find(rank->id) != guildRankList.end();
}

void AccessList::getList(std::string& list) const { list = this->list; }

Door::Door(uint16_t type) : Item(type) {}

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
	if (this->house) {
		return;
	}

	this->house = house;

	if (!accessList) {
		accessList.reset(new AccessList());
	}
}

bool Door::canUse(const std::shared_ptr<const Player>& player)
{
	if (!house) {
		return true;
	}

	if (house->getHouseAccessLevel(player) >= HOUSE_SUBOWNER) {
		return true;
	}

	return accessList->isInList(player);
}

void Door::setAccessList(std::string_view textlist)
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
		house->removeDoor(getDoor());
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

		Position entryPos(pugi::cast<uint16_t>(houseNode.attribute("entryx").value()),
		                  pugi::cast<uint16_t>(houseNode.attribute("entryy").value()),
		                  pugi::cast<uint16_t>(houseNode.attribute("entryz").value()));
		if (entryPos.x == 0 && entryPos.y == 0 && entryPos.z == 0) {
			std::cout << "[Warning - Houses::loadHousesXML] House entry not set - Name: " << house->getName()
			          << " - House id: " << houseId << std::endl;
		}
		house->setEntryPos(entryPos);

		house->setRent(pugi::cast<uint32_t>(houseNode.attribute("rent").value()));
		house->setTownId(pugi::cast<uint32_t>(houseNode.attribute("townid").value()));

		house->setOwner(0, false);
	}
	return true;
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
		const Town* town = g_game.map.towns.getTown(house->getTownId());
		if (!town) {
			continue;
		}

		auto player = std::make_shared<Player>(nullptr);
		if (!IOLoginData::loadPlayerById(player, ownerId)) {
			// Player doesn't exist, reset house owner
			house->setOwner(0);
			continue;
		}

		if (player->getBankBalance() >= rent) {
			player->setBankBalance(player->getBankBalance() - rent);

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
			house->setPayRentWarnings(0);
		} else {
			if (house->getPayRentWarnings() < 7) {
				int32_t daysLeft = 7 - house->getPayRentWarnings();

				const auto& letter = Item::CreateItem(ITEM_LETTER_STAMPED);
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

				letter->setText(fmt::format(
				    "Warning! \nThe {:s} rent of {:d} gold for your house \"{:s}\" is payable. Have it within {:d} days or you will lose this house.",
				    period, house->getRent(), house->getName(), daysLeft));
				g_game.internalAddItem(player->getInbox(), letter, INDEX_WHEREEVER, FLAG_NOLIMIT);
				house->setPayRentWarnings(house->getPayRentWarnings() + 1);
			} else {
				house->setOwner(0, true, player);
			}
		}

		IOLoginData::savePlayer(player);
	}
}
