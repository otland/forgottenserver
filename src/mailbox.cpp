// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "mailbox.h"

#include "game.h"
#include "inbox.h"
#include "ioinbox.h"
#include "iologindata.h"

extern Game g_game;

ReturnValue Mailbox::queryAdd(int32_t, const Thing& thing, uint32_t, uint32_t, Creature*) const
{
	const Item* item = thing.getItem();
	if (item && Mailbox::canSend(item)) {
		return RETURNVALUE_NOERROR;
	}
	return RETURNVALUE_NOTPOSSIBLE;
}

ReturnValue Mailbox::queryMaxCount(int32_t, const Thing&, uint32_t count, uint32_t& maxQueryCount, uint32_t) const
{
	maxQueryCount = std::max<uint32_t>(1, count);
	return RETURNVALUE_NOERROR;
}

ReturnValue Mailbox::queryRemove(const Thing&, uint32_t, uint32_t, Creature* /*= nullptr */) const
{
	return RETURNVALUE_NOTPOSSIBLE;
}

Cylinder* Mailbox::queryDestination(int32_t&, const Thing&, Item**, uint32_t&) { return this; }

void Mailbox::addThing(Thing* thing) { return addThing(0, thing); }

void Mailbox::addThing(int32_t, Thing* thing)
{
	Item* item = thing->getItem();
	if (item && Mailbox::canSend(item)) {
		sendItem(item);
	}
}

void Mailbox::updateThing(Thing*, uint16_t, uint32_t)
{
	//
}

void Mailbox::replaceThing(uint32_t, Thing*)
{
	//
}

void Mailbox::removeThing(Thing*, uint32_t)
{
	//
}

void Mailbox::postAddNotification(Thing* thing, const Cylinder* oldParent, int32_t index, cylinderlink_t)
{
	getParent()->postAddNotification(thing, oldParent, index, LINK_PARENT);
}

void Mailbox::postRemoveNotification(Thing* thing, const Cylinder* newParent, int32_t index, cylinderlink_t)
{
	getParent()->postRemoveNotification(thing, newParent, index, LINK_PARENT);
}

bool Mailbox::sendItem(Item* item) const
{
	std::string receiver;
	if (!getReceiver(item, receiver)) {
		return false;
	}

	/**No need to continue if its still empty**/
	if (receiver.empty()) {
		return false;
	}

	if (Player* player = g_game.getPlayerByName(receiver)) {
		if (Inbox* inbox = player->getInbox()) {
			ReturnValue ret = g_game.internalMoveItem(item->getParent(), inbox, INDEX_WHEREEVER, item,
			                                          item->getItemCount(), nullptr, FLAG_NOLIMIT);
			if (ret == RETURNVALUE_NOERROR) {
				g_game.transformItem(item, item->getID() + 1);
				player->onReceiveMail();
				return true;
			}
		} else {
			Item* cloneItem = item->clone();

			ReturnValue ret = g_game.internalRemoveItem(item);
			if (ret == RETURNVALUE_NOERROR) {
				cloneItem->setID(cloneItem->getID() + 1);

				ItemBlockList itemList;
				itemList.emplace_back(0, cloneItem);
				IOInbox::getInstance().savePlayerItems(player, itemList);
				return true;
			} else {
				delete cloneItem;
			}
		}
	} else if (const uint32_t guid = IOLoginData::getGuidByName(receiver)) {
		Item* cloneItem = item->clone();

		ReturnValue ret = g_game.internalRemoveItem(item);
		if (ret == RETURNVALUE_NOERROR) {
			cloneItem->setID(cloneItem->getID() + 1);

			ItemBlockList itemList;
			itemList.emplace_back(0, cloneItem);
			IOInbox::getInstance().savePlayerItems(guid, itemList);
			return true;
		} else {
			delete cloneItem;
		}
	}
	return false;
}

bool Mailbox::getReceiver(Item* item, std::string& name) const
{
	const Container* container = item->getContainer();
	if (container) {
		for (Item* containerItem : container->getItemList()) {
			if (containerItem->getID() == ITEM_LABEL && getReceiver(containerItem, name)) {
				return true;
			}
		}
		return false;
	}

	const std::string& text = item->getText();
	if (text.empty()) {
		return false;
	}

	name = getFirstLine(text);
	boost::algorithm::trim(name);
	return true;
}

bool Mailbox::canSend(const Item* item) { return item->getID() == ITEM_PARCEL || item->getID() == ITEM_LETTER; }
