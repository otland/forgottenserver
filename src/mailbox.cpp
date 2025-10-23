// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "mailbox.h"

#include "game.h"
#include "inbox.h"
#include "iologindata.h"

extern Game g_game;

static bool canSend(std::shared_ptr<const Item> item)
{
	return item->getID() == ITEM_PARCEL || item->getID() == ITEM_LETTER;
}

ReturnValue Mailbox::queryAdd(int32_t, std::shared_ptr<const Thing> thing, uint32_t, uint32_t,
                              std::shared_ptr<Creature>) const
{
	auto item = thing->getItem();
	if (item && canSend(item)) {
		return RETURNVALUE_NOERROR;
	}
	return RETURNVALUE_NOTPOSSIBLE;
}

ReturnValue Mailbox::queryMaxCount(int32_t, std::shared_ptr<const Thing>, uint32_t count, uint32_t& maxQueryCount,
                                   uint32_t) const
{
	maxQueryCount = std::max<uint32_t>(1, count);
	return RETURNVALUE_NOERROR;
}

ReturnValue Mailbox::queryRemove(std::shared_ptr<const Thing>, uint32_t, uint32_t,
                                 std::shared_ptr<Creature> /*= nullptr */) const
{
	return RETURNVALUE_NOTPOSSIBLE;
}

std::shared_ptr<Thing> Mailbox::queryDestination(int32_t&, std::shared_ptr<const Thing>, std::shared_ptr<Item>&,
                                                 uint32_t&)
{
	return shared_from_this();
}

void Mailbox::addThing(std::shared_ptr<Thing> thing) { return addThing(0, thing); }

void Mailbox::addThing(int32_t, std::shared_ptr<Thing> thing)
{
	auto item = thing->getItem();
	if (item && canSend(item)) {
		sendItem(item);
	}
}

void Mailbox::updateThing(std::shared_ptr<Thing>, uint16_t, uint32_t)
{
	//
}

void Mailbox::replaceThing(uint32_t, std::shared_ptr<Thing>)
{
	//
}

void Mailbox::removeThing(std::shared_ptr<Thing>, uint32_t)
{
	//
}

void Mailbox::postAddNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Thing> oldParent, int32_t index,
                                  cylinderlink_t)
{
	Item::getParent()->postAddNotification(thing, oldParent, index, LINK_PARENT);
}

void Mailbox::postRemoveNotification(std::shared_ptr<Thing> thing, std::shared_ptr<const Thing> newParent,
                                     int32_t index, cylinderlink_t)
{
	Item::getParent()->postRemoveNotification(thing, newParent, index, LINK_PARENT);
}

bool Mailbox::sendItem(std::shared_ptr<Item> item) const
{
	std::string receiver;
	if (!getReceiver(item, receiver)) {
		return false;
	}

	/**No need to continue if its still empty**/
	if (receiver.empty()) {
		return false;
	}

	auto player = g_game.getPlayerByName(receiver);
	if (player) {
		if (g_game.internalMoveItem(item->getParent(), player->getInbox(), INDEX_WHEREEVER, item, item->getItemCount(),
		                            nullptr, FLAG_NOLIMIT) == RETURNVALUE_NOERROR) {
			g_game.transformItem(item, item->getID() + 1);
			player->onReceiveMail();
			return true;
		}
	} else {
		auto tmpPlayer = std::make_shared<Player>(nullptr);
		if (!IOLoginData::loadPlayerByName(tmpPlayer, receiver)) {
			return false;
		}

		if (g_game.internalMoveItem(item->getParent(), tmpPlayer->getInbox(), INDEX_WHEREEVER, item,
		                            item->getItemCount(), nullptr, FLAG_NOLIMIT) == RETURNVALUE_NOERROR) {
			g_game.transformItem(item, item->getID() + 1);
			IOLoginData::savePlayer(tmpPlayer);
			return true;
		}
	}
	return false;
}

bool Mailbox::getReceiver(std::shared_ptr<Item> item, std::string& name) const
{
	auto container = item->getContainer();
	if (container) {
		for (const auto& containerItem : container->getItemList()) {
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
