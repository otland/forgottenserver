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

#include "mailbox.h"
#include "game.h"
#include "player.h"
#include "iologindata.h"
#include "town.h"

extern Game g_game;

Mailbox::Mailbox(uint16_t _type) : Item(_type)
{
	//
}

Mailbox::~Mailbox()
{
	//
}

ReturnValue Mailbox::__queryAdd(int32_t index, const Thing* thing, uint32_t count,
                                uint32_t flags, Creature* actor/* = NULL*/) const
{
	if (const Item* item = thing->getItem()) {
		if (canSend(item)) {
			return RET_NOERROR;
		}
	}

	return RET_NOTPOSSIBLE;
}

ReturnValue Mailbox::__queryMaxCount(int32_t index, const Thing* thing, uint32_t count, uint32_t& maxQueryCount,
                                     uint32_t flags) const
{
	maxQueryCount = std::max<uint32_t>(1, count);
	return RET_NOERROR;
}

ReturnValue Mailbox::__queryRemove(const Thing* thing, uint32_t count, uint32_t flags) const
{
	return RET_NOTPOSSIBLE;
}

Cylinder* Mailbox::__queryDestination(int32_t& index, const Thing* thing, Item** destItem,
                                      uint32_t& flags)
{
	return this;
}

void Mailbox::__addThing(Thing* thing)
{
	return __addThing(0, thing);
}

void Mailbox::__addThing(int32_t index, Thing* thing)
{
	if (Item* item = thing->getItem()) {
		if (canSend(item)) {
			sendItem(item);
		}
	}
}

void Mailbox::__updateThing(Thing* thing, uint16_t itemId, uint32_t count)
{
	//
}

void Mailbox::__replaceThing(uint32_t index, Thing* thing)
{
	//
}

void Mailbox::__removeThing(Thing* thing, uint32_t count)
{
	//
}

void Mailbox::postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t link /*= LINK_OWNER*/)
{
	getParent()->postAddNotification(thing, oldParent, index, LINK_PARENT);
}

void Mailbox::postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, bool isCompleteRemoval, cylinderlink_t link /*= LINK_OWNER*/)
{
	getParent()->postRemoveNotification(thing, newParent, index, isCompleteRemoval, LINK_PARENT);
}

bool Mailbox::sendItem(Item* item)
{
	std::string receiver = std::string("");

	if (!getReceiver(item, receiver)) {
		return false;
	}

	/**No need to continue if its still empty**/
	if (receiver == "") {
		return false;
	}

	uint32_t guid;

	if (!IOLoginData::getInstance()->getGuidByName(guid, receiver)) {
		return false;
	}

	Player* player = g_game.getPlayerByName(receiver);

	if (player) {
		if (g_game.internalMoveItem(item->getParent(), player->getInbox(), INDEX_WHEREEVER,
		                            item, item->getItemCount(), NULL, FLAG_NOLIMIT) == RET_NOERROR) {
			g_game.transformItem(item, item->getID() + 1);
			player->onReceiveMail();
			return true;
		}
	} else {
		player = new Player(receiver, NULL);

		if (!IOLoginData::getInstance()->loadPlayer(player, receiver)) {
			delete player;
			return false;
		}

		if (g_game.internalMoveItem(item->getParent(), player->getInbox(), INDEX_WHEREEVER,
		                            item, item->getItemCount(), NULL, FLAG_NOLIMIT) == RET_NOERROR) {
			g_game.transformItem(item, item->getID() + 1);
			IOLoginData::getInstance()->savePlayer(player);
			delete player;
			return true;
		}

		delete player;
	}

	return false;
}

bool Mailbox::getReceiver(Item* item, std::string& name)
{
	if (!item) {
		return false;
	}

	if (item->getID() == ITEM_PARCEL) { /**We need to get the text from the label incase its a parcel**/
		Container* parcel = item->getContainer();

		if (parcel) {
			for (ItemDeque::const_iterator cit = parcel->getItems(), end = parcel->getEnd(); cit != end; ++cit) {
				if ((*cit)->getID() == ITEM_LABEL) {
					item = (*cit);

					if (item->getText() != "") {
						break;
					}
				}
			}
		}
	} else if (item->getID() != ITEM_LETTER) { /**The item is somehow not a parcel or letter**/
		std::cout << "Mailbox::getReciver error, trying to get reciecer from unkown item! ID: " << item->getID() << "." << std::endl;
		return false;
	}

	if (!item || item->getText() == "") { /**No label/letter found or its empty.**/
		return false;
	}

	name = getFirstLine(item->getText());
	trimString(name);
	return true;
}

bool Mailbox::canSend(const Item* item) const
{
	return item->getID() == ITEM_PARCEL || item->getID() == ITEM_LETTER;
}
