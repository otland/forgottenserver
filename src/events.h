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

bool onChangeOutfit(Creature* creature, const Outfit_t& outfit);
ReturnValue onAreaCombat(Creature* creature, Tile* tile, bool aggressive);
ReturnValue onTargetCombat(Creature* creature, Creature* target);
void onHear(Creature* creature, Creature* speaker, const std::string& words, SpeakClasses type);
void onChangeZone(Creature* creature, ZoneType_t fromZone, ZoneType_t toZone);
void onUpdateStorage(Creature* creature, uint32_t key, std::optional<int32_t> value, std::optional<int32_t> oldValue,
                     bool isSpawn);

} // namespace tfs::events::creature

namespace tfs::events::party {

bool onJoin(Party* party, Player* player);
bool onLeave(Party* party, Player* player);
bool onDisband(Party* party);
void onShareExperience(Party* party, uint64_t& exp);
bool onInvite(Party* party, Player* player);
bool onRevokeInvitation(Party* party, Player* player);
bool onPassLeadership(Party* party, Player* player);

} // namespace tfs::events::party

namespace tfs::events::player {

bool onBrowseField(Player* player, const Position& position);
void onLook(Player* player, const Position& position, Thing* thing, uint8_t stackpos, int32_t lookDistance);
void onLookInBattleList(Player* player, Creature* creature, int32_t lookDistance);
void onLookInTrade(Player* player, Player* partner, Item* item, int32_t lookDistance);
bool onLookInShop(Player* player, const ItemType* itemType, uint8_t count);
bool onLookInMarket(Player* player, const ItemType* itemType);
ReturnValue onMoveItem(Player* player, Item* item, uint16_t count, const Position& fromPosition,
                       const Position& toPosition, Cylinder* fromCylinder, Cylinder* toCylinder);
void onItemMoved(Player* player, Item* item, uint16_t count, const Position& fromPosition, const Position& toPosition,
                 Cylinder* fromCylinder, Cylinder* toCylinder);
bool onMoveCreature(Player* player, Creature* creature, const Position& fromPosition, const Position& toPosition);
void onReportRuleViolation(Player* player, const std::string& targetName, uint8_t reportType, uint8_t reportReason,
                           const std::string& comment, const std::string& translation);
bool onReportBug(Player* player, const std::string& message, const Position& position, uint8_t category);
void onRotateItem(Player* player, Item* item);
bool onTurn(Player* player, Direction direction);
bool onTradeRequest(Player* player, Player* target, Item* item);
bool onTradeAccept(Player* player, Player* target, Item* item, Item* targetItem);
void onTradeCompleted(Player* player, Player* target, Item* item, Item* targetItem, bool isSuccess);
void onPodiumRequest(Player* player, Item* item);
void onPodiumEdit(Player* player, Item* item, const Outfit_t& outfit, bool podiumVisible, Direction direction);
void onGainExperience(Player* player, Creature* source, uint64_t& exp, uint64_t rawExp, bool sendText);
void onLoseExperience(Player* player, uint64_t& exp);
void onGainSkillTries(Player* player, skills_t skill, uint64_t& tries);
void onWrapItem(Player* player, Item* item);
void onInventoryUpdate(Player* player, Item* item, slots_t slot, bool equip);
void onNetworkMessage(Player* player, uint8_t recvByte, NetworkMessage_ptr msg);
bool onSpellCheck(Player* player, const Spell* spell);

} // namespace tfs::events::player

namespace tfs::events::monster {

void onDropLoot(Monster* monster, Container* corpse);
bool onSpawn(Monster* monster, const Position& position, bool startup, bool artificial);

} // namespace tfs::events::monster

#endif // FS_EVENTS_H
