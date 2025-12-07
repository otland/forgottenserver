// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "bed.h"

#include "condition.h"
#include "game.h"
#include "iologindata.h"
#include "scheduler.h"

extern Game g_game;

BedItem::BedItem(uint16_t id) : Item(id) { internalRemoveSleeper(); }

Attr_ReadValue BedItem::readAttr(AttrTypes_t attr, PropStream& propStream)
{
	switch (attr) {
		case ATTR_SLEEPERGUID: {
			uint32_t guid;
			if (!propStream.read<uint32_t>(guid)) {
				return ATTR_READ_ERROR;
			}

			if (guid != 0) {
				std::string name = IOLoginData::getNameByGuid(guid);
				if (!name.empty()) {
					setSpecialDescription(name + " is sleeping there.");
					g_game.setBedSleeper(getBed(), guid);
					sleeperGUID = guid;
				}
			}
			return ATTR_READ_CONTINUE;
		}

		case ATTR_SLEEPSTART: {
			uint32_t sleep_start;
			if (!propStream.read<uint32_t>(sleep_start)) {
				return ATTR_READ_ERROR;
			}

			sleepStart = static_cast<uint64_t>(sleep_start);
			return ATTR_READ_CONTINUE;
		}

		default:
			break;
	}
	return Item::readAttr(attr, propStream);
}

void BedItem::serializeAttr(PropWriteStream& propWriteStream) const
{
	if (sleeperGUID != 0) {
		propWriteStream.write<uint8_t>(ATTR_SLEEPERGUID);
		propWriteStream.write<uint32_t>(sleeperGUID);
	}

	if (sleepStart != 0) {
		propWriteStream.write<uint8_t>(ATTR_SLEEPSTART);
		// FIXME: should be stored as 64-bit, but we need to retain backwards compatibility
		propWriteStream.write<uint32_t>(static_cast<uint32_t>(sleepStart));
	}
}

std::shared_ptr<BedItem> BedItem::getNextBedItem() const
{
	Direction dir = Item::items[id].bedPartnerDir;
	Position targetPos = getNextPosition(dir, getPosition());

	const auto& tile = g_game.map.getTile(targetPos);
	if (!tile) {
		return nullptr;
	}
	return tile->getBedItem();
}

bool BedItem::canUse(const std::shared_ptr<Player>& player)
{
	if (!player || !house || !player->isPremium() || player->getZone() != ZONE_PROTECTION) {
		return false;
	}

	if (sleeperGUID == 0) {
		return true;
	}

	if (house->getHouseAccessLevel(player) == HOUSE_OWNER) {
		return true;
	}

	auto sleeper = std::make_shared<Player>(nullptr);
	if (!IOLoginData::loadPlayerById(sleeper, sleeperGUID)) {
		return false;
	}

	if (house->getHouseAccessLevel(sleeper) > house->getHouseAccessLevel(player)) {
		return false;
	}
	return true;
}

bool BedItem::trySleep(const std::shared_ptr<Player>& player)
{
	if (!house || player->isRemoved()) {
		return false;
	}

	if (sleeperGUID != 0) {
		if (Item::items[id].transformToFree != 0 && house->getOwner() == player->getGUID()) {
			wakeUp(nullptr);
		}

		g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		return false;
	}
	return true;
}

bool BedItem::sleep(const std::shared_ptr<Player>& player)
{
	if (!house) {
		return false;
	}

	if (sleeperGUID != 0) {
		return false;
	}

	const auto& nextBedItem = getNextBedItem();

	internalSetSleeper(player);

	if (nextBedItem) {
		nextBedItem->internalSetSleeper(player);
	}

	// update the bedSleepersMap
	g_game.setBedSleeper(getBed(), player->getGUID());

	// make the player walk onto the bed
	g_game.map.moveCreature(player, getTile());

	// display 'Zzzz'/sleep effect
	g_game.addMagicEffect(player->getPosition(), CONST_ME_SLEEP);

	// kick player after he sees himself walk onto the bed and it change id
	g_scheduler.addEvent(createSchedulerTask(SCHEDULER_MINTICKS,
	                                         [playerID = player->getID()]() { g_game.kickPlayer(playerID, false); }));

	// change self and partner's appearance
	updateAppearance(player);

	if (nextBedItem) {
		nextBedItem->updateAppearance(player);
	}

	return true;
}

void BedItem::wakeUp(const std::shared_ptr<Player>& player)
{
	if (!house) {
		return;
	}

	if (sleeperGUID != 0) {
		if (!player) {
			auto regenPlayer = std::make_shared<Player>(nullptr);
			if (IOLoginData::loadPlayerById(regenPlayer, sleeperGUID)) {
				regeneratePlayer(regenPlayer);
				IOLoginData::savePlayer(regenPlayer);
			}
		} else {
			regeneratePlayer(player);
			g_game.addCreatureHealth(player);
		}
	}

	// update the bedSleepersMap
	g_game.removeBedSleeper(sleeperGUID);

	const auto& nextBedItem = getNextBedItem();

	// unset sleep info
	internalRemoveSleeper();

	if (nextBedItem) {
		nextBedItem->internalRemoveSleeper();
	}

	// change self and partner's appearance
	updateAppearance(nullptr);

	if (nextBedItem) {
		nextBedItem->updateAppearance(nullptr);
	}
}

void BedItem::regeneratePlayer(const std::shared_ptr<Player>& player) const
{
	const uint32_t sleptTime = time(nullptr) - sleepStart;

	Condition* condition = player->getCondition(CONDITION_REGENERATION, CONDITIONID_DEFAULT);
	if (condition) {
		uint32_t regen;
		if (condition->getTicks() != -1) {
			regen = std::min<int32_t>((condition->getTicks() / 1000), sleptTime) / 30;
			const int32_t newRegenTicks = condition->getTicks() - (regen * 30000);
			if (newRegenTicks <= 0) {
				player->removeCondition(condition);
			} else {
				condition->setTicks(newRegenTicks);
			}
		} else {
			regen = sleptTime / 30;
		}

		player->changeHealth(regen, false);
		player->changeMana(regen);
	}

	const int32_t soulRegen = sleptTime / (60 * 15);
	player->changeSoul(soulRegen);
}

void BedItem::updateAppearance(const std::shared_ptr<const Player>& player)
{
	const ItemType& it = Item::items[id];
	if (it.type == ITEM_TYPE_BED) {
		if (player && it.transformToOnUse[player->getSex()] != 0) {
			const ItemType& newType = Item::items[it.transformToOnUse[player->getSex()]];
			if (newType.type == ITEM_TYPE_BED) {
				g_game.transformItem(getBed(), it.transformToOnUse[player->getSex()]);
			}
		} else if (it.transformToFree != 0) {
			const ItemType& newType = Item::items[it.transformToFree];
			if (newType.type == ITEM_TYPE_BED) {
				g_game.transformItem(getBed(), it.transformToFree);
			}
		}
	}
}

void BedItem::internalSetSleeper(const std::shared_ptr<const Player>& player)
{
	std::string desc_str = player->getName() + " is sleeping there.";

	sleeperGUID = player->getGUID();
	sleepStart = time(nullptr);
	setSpecialDescription(desc_str);
}

void BedItem::internalRemoveSleeper()
{
	sleeperGUID = 0;
	sleepStart = 0;
	setSpecialDescription("Nobody is sleeping there.");
}
