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

bool onChangeOutfit(const std::shared_ptr<Creature>& creature, const Outfit_t& outfit);
ReturnValue onAreaCombat(const std::shared_ptr<Creature>& creature, const std::shared_ptr<Tile>& tile, bool aggressive);
ReturnValue onTargetCombat(const std::shared_ptr<Creature>& creature, const std::shared_ptr<Creature>& target);
void onHear(const std::shared_ptr<Creature>& creature, const std::shared_ptr<Creature>& speaker,
            const std::string& words, SpeakClasses type);
void onChangeZone(const std::shared_ptr<Creature>& creature, ZoneType_t fromZone, ZoneType_t toZone);
void onUpdateStorage(const std::shared_ptr<Creature>& creature, uint32_t key, std::optional<int32_t> value,
                     std::optional<int32_t> oldValue, bool isSpawn);

} // namespace tfs::events::creature

namespace tfs::events::party {

bool onJoin(Party* party, const std::shared_ptr<Player>& player);
bool onLeave(Party* party, const std::shared_ptr<Player>& player);
bool onDisband(Party* party);
void onShareExperience(Party* party, uint64_t& exp);
bool onInvite(Party* party, const std::shared_ptr<Player>& player);
bool onRevokeInvitation(Party* party, const std::shared_ptr<Player>& player);
bool onPassLeadership(Party* party, const std::shared_ptr<Player>& player);

} // namespace tfs::events::party

namespace tfs::events::player {

bool onBrowseField(const std::shared_ptr<Player>& player, const Position& position);
void onLook(const std::shared_ptr<Player>& player, const Position& position, const std::shared_ptr<Thing>& thing,
            uint8_t stackpos, int32_t lookDistance);
void onLookInBattleList(const std::shared_ptr<Player>& player, const std::shared_ptr<Creature>& creature,
                        int32_t lookDistance);
void onLookInTrade(const std::shared_ptr<Player>& player, const std::shared_ptr<Player>& partner,
                   const std::shared_ptr<Item>& item, int32_t lookDistance);
bool onLookInShop(const std::shared_ptr<Player>& player, const ItemType* itemType, uint8_t count);
bool onLookInMarket(const std::shared_ptr<Player>& player, const ItemType* itemType);
ReturnValue onMoveItem(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item, uint16_t count,
                       const Position& fromPosition, const Position& toPosition,
                       const std::shared_ptr<Thing>& fromThing, const std::shared_ptr<Thing>& toThing);
void onItemMoved(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item, uint16_t count,
                 const Position& fromPosition, const Position& toPosition, const std::shared_ptr<Thing>& fromThing,
                 const std::shared_ptr<Thing>& toThing);
bool onMoveCreature(const std::shared_ptr<Player>& player, const std::shared_ptr<Creature>& creature,
                    const Position& fromPosition, const Position& toPosition);
void onReportRuleViolation(const std::shared_ptr<Player>& player, const std::string& targetName, uint8_t reportType,
                           uint8_t reportReason, const std::string& comment, const std::string& translation);
bool onReportBug(const std::shared_ptr<Player>& player, const std::string& message, const Position& position,
                 uint8_t category);
void onRotateItem(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item);
bool onTurn(const std::shared_ptr<Player>& player, Direction direction);
bool onTradeRequest(const std::shared_ptr<Player>& player, const std::shared_ptr<Player>& target,
                    const std::shared_ptr<Item>& item);
bool onTradeAccept(const std::shared_ptr<Player>& player, const std::shared_ptr<Player>& target,
                   const std::shared_ptr<Item>& item, const std::shared_ptr<Item>& targetItem);
void onTradeCompleted(const std::shared_ptr<Player>& player, const std::shared_ptr<Player>& target,
                      const std::shared_ptr<Item>& item, const std::shared_ptr<Item>& targetItem, bool isSuccess);
void onPodiumRequest(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item);
void onPodiumEdit(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item, const Outfit_t& outfit,
                  bool podiumVisible, Direction direction);
void onGainExperience(const std::shared_ptr<Player>& player, const std::shared_ptr<Creature>& source, uint64_t& exp,
                      uint64_t rawExp, bool sendText);
void onLoseExperience(const std::shared_ptr<Player>& player, uint64_t& exp);
void onGainSkillTries(const std::shared_ptr<Player>& player, skills_t skill, uint64_t& tries);
void onWrapItem(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item);
void onInventoryUpdate(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item, slots_t slot,
                       bool equip);
void onNetworkMessage(const std::shared_ptr<Player>& player, uint8_t recvByte, NetworkMessage_ptr& msg);
bool onSpellCheck(const std::shared_ptr<Player>& player, const Spell* spell);

} // namespace tfs::events::player

namespace tfs::events::monster {

void onDropLoot(const std::shared_ptr<Monster>& monster, const std::shared_ptr<Container>& corpse);
bool onSpawn(const std::shared_ptr<Monster>& monster, const Position& position, bool startup, bool artificial);

} // namespace tfs::events::monster

#endif // FS_EVENTS_H
