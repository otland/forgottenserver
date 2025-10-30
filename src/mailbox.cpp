// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "mailbox.h"

#include "game.h"
#include "inbox.h"
#include "iologindata.h"

extern Game g_game;

namespace {

bool canSend(const std::shared_ptr<const Item>& item)
{
	return item->getID() == ITEM_PARCEL || item->getID() == ITEM_LETTER;
}

std::string getReceiver(const std::shared_ptr<Item>& item)
{
	const auto& container = item->getContainer();
	if (container) {
		for (const auto& containerItem : container->getItemList()) {
			if (containerItem->getID() == ITEM_LABEL) {
				return getReceiver(containerItem);
			}
		}
		return "";
	}

	const std::string& text = item->getText();
	if (text.empty()) {
		return "";
	}

	std::string name;
	std::getline(std::istringstream{text}, name);
	boost::algorithm::trim(name);
	return name;
}

} // namespace

ReturnValue Mailbox::queryAdd(int32_t, const std::shared_ptr<const Thing>& thing, uint32_t, uint32_t,
                              const std::shared_ptr<Creature>&) const
{
	const auto& item = thing->getItem();
	if (item && canSend(item)) {
		return RETURNVALUE_NOERROR;
	}
	return RETURNVALUE_NOTPOSSIBLE;
}

ReturnValue Mailbox::queryMaxCount(int32_t, const std::shared_ptr<const Thing>&, uint32_t count,
                                   uint32_t& maxQueryCount, uint32_t) const
{
	maxQueryCount = std::max<uint32_t>(1, count);
	return RETURNVALUE_NOERROR;
}

void Mailbox::addThing(int32_t, const std::shared_ptr<Thing>& thing)
{
	const auto& item = thing->getItem();
	if (item && canSend(item)) {
		sendItem(item);
	}
}

void Mailbox::postAddNotification(const std::shared_ptr<Thing>& thing, const std::shared_ptr<const Thing>& oldParent,
                                  int32_t index, cylinderlink_t)
{
	getParent()->postAddNotification(thing, oldParent, index, LINK_PARENT);
}

void Mailbox::postRemoveNotification(const std::shared_ptr<Thing>& thing, const std::shared_ptr<const Thing>& newParent,
                                     int32_t index, cylinderlink_t)
{
	getParent()->postRemoveNotification(thing, newParent, index, LINK_PARENT);
}

bool Mailbox::sendItem(const std::shared_ptr<Item>& item) const
{
	const auto& receiver = getReceiver(item);
	/**No need to continue if its still empty**/
	if (receiver.empty()) {
		return false;
	}

	const auto& player = g_game.getPlayerByName(receiver);
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
