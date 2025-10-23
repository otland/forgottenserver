// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_EVENTS_H
#define FS_EVENTS_H

#include "const.h"
#include "creature.h"
#include "luascript.h"
#include "networkmessage.h"

class ItemType;
class Party;
class Spell;
class Tile;

enum class EventInfoId
{
	// Creature
	CREATURE_ONHEAR,

	// Monster
	MONSTER_ONSPAWN
};

namespace tfs::events {

bool load();
bool reload();
int32_t getScriptId(EventInfoId eventInfoId);

} // namespace tfs::events

namespace tfs::events::creature {

bool onChangeOutfit(std::shared_ptr<Creature> creature, const Outfit_t& outfit);
ReturnValue onAreaCombat(std::shared_ptr<Creature> creature, std::shared_ptr<Tile> tile, bool aggressive);
ReturnValue onTargetCombat(std::shared_ptr<Creature> creature, std::shared_ptr<Creature> target);
void onHear(std::shared_ptr<Creature> creature, std::shared_ptr<Creature> speaker, const std::string& words,
            SpeakClasses type);
void onChangeZone(std::shared_ptr<Creature> creature, ZoneType_t fromZone, ZoneType_t toZone);
void onUpdateStorage(std::shared_ptr<Creature> creature, uint32_t key, std::optional<int32_t> value,
                     std::optional<int32_t> oldValue, bool isSpawn);

} // namespace tfs::events::creature

namespace tfs::events::party {

bool onJoin(Party* party, std::shared_ptr<Player> player);
bool onLeave(Party* party, std::shared_ptr<Player> player);
bool onDisband(Party* party);
void onShareExperience(Party* party, uint64_t& exp);
bool onInvite(Party* party, std::shared_ptr<Player> player);
bool onRevokeInvitation(Party* party, std::shared_ptr<Player> player);
bool onPassLeadership(Party* party, std::shared_ptr<Player> player);

} // namespace tfs::events::party

namespace tfs::events::player {

bool onBrowseField(std::shared_ptr<Player> player, const Position& position);
void onLook(std::shared_ptr<Player> player, const Position& position, std::shared_ptr<Thing> thing, uint8_t stackpos,
            int32_t lookDistance);
void onLookInBattleList(std::shared_ptr<Player> player, std::shared_ptr<Creature> creature, int32_t lookDistance);
void onLookInTrade(std::shared_ptr<Player> player, std::shared_ptr<Player> partner, std::shared_ptr<Item> item,
                   int32_t lookDistance);
bool onLookInShop(std::shared_ptr<Player> player, const ItemType* itemType, uint8_t count);
bool onLookInMarket(std::shared_ptr<Player> player, const ItemType* itemType);
ReturnValue onMoveItem(std::shared_ptr<Player> player, std::shared_ptr<Item> item, uint16_t count,
                       const Position& fromPosition, const Position& toPosition, std::shared_ptr<Thing> fromCylinder,
                       std::shared_ptr<Thing> toCylinder);
void onItemMoved(std::shared_ptr<Player> player, std::shared_ptr<Item> item, uint16_t count,
                 const Position& fromPosition, const Position& toPosition, std::shared_ptr<Thing> fromCylinder,
                 std::shared_ptr<Thing> toCylinder);
bool onMoveCreature(std::shared_ptr<Player> player, std::shared_ptr<Creature> creature, const Position& fromPosition,
                    const Position& toPosition);
void onReportRuleViolation(std::shared_ptr<Player> player, const std::string& targetName, uint8_t reportType,
                           uint8_t reportReason, const std::string& comment, const std::string& translation);
bool onReportBug(std::shared_ptr<Player> player, const std::string& message, const Position& position,
                 uint8_t category);
void onRotateItem(std::shared_ptr<Player> player, std::shared_ptr<Item> item);
bool onTurn(std::shared_ptr<Player> player, Direction direction);
bool onTradeRequest(std::shared_ptr<Player> player, std::shared_ptr<Player> target, std::shared_ptr<Item> item);
bool onTradeAccept(std::shared_ptr<Player> player, std::shared_ptr<Player> target, std::shared_ptr<Item> item,
                   std::shared_ptr<Item> targetItem);
void onTradeCompleted(std::shared_ptr<Player> player, std::shared_ptr<Player> target, std::shared_ptr<Item> item,
                      std::shared_ptr<Item> targetItem, bool isSuccess);
void onPodiumRequest(std::shared_ptr<Player> player, std::shared_ptr<Item> item);
void onPodiumEdit(std::shared_ptr<Player> player, std::shared_ptr<Item> item, const Outfit_t& outfit,
                  bool podiumVisible, Direction direction);
void onGainExperience(std::shared_ptr<Player> player, std::shared_ptr<Creature> source, uint64_t& exp, uint64_t rawExp,
                      bool sendText);
void onLoseExperience(std::shared_ptr<Player> player, uint64_t& exp);
void onGainSkillTries(std::shared_ptr<Player> player, skills_t skill, uint64_t& tries);
void onWrapItem(std::shared_ptr<Player> player, std::shared_ptr<Item> item);
void onInventoryUpdate(std::shared_ptr<Player> player, std::shared_ptr<Item> item, slots_t slot, bool equip);
void onNetworkMessage(std::shared_ptr<Player> player, uint8_t recvByte, NetworkMessage_ptr& msg);
bool onSpellCheck(std::shared_ptr<Player> player, const Spell* spell);

} // namespace tfs::events::player

namespace tfs::events::monster {

void onDropLoot(std::shared_ptr<Monster> monster, std::shared_ptr<Container> corpse);
bool onSpawn(std::shared_ptr<Monster> monster, const Position& position, bool startup, bool artificial);

} // namespace tfs::events::monster

#endif // FS_EVENTS_H
