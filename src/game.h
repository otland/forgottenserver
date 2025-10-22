// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_GAME_H
#define FS_GAME_H

#include "groups.h"
#include "map.h"
#include "mounts.h"
#include "player.h"
#include "position.h"
#include "wildcardtree.h"

class Monster;
class Npc;
class ServiceManager;

enum stackPosType_t
{
	STACKPOS_MOVE,
	STACKPOS_LOOK,
	STACKPOS_TOPDOWN_ITEM,
	STACKPOS_USEITEM,
	STACKPOS_USETARGET,
};

enum WorldType_t
{
	WORLD_TYPE_NO_PVP = 1,
	WORLD_TYPE_PVP = 2,
	WORLD_TYPE_PVP_ENFORCED = 3,
};

enum GameState_t
{
	GAME_STATE_STARTUP,
	GAME_STATE_INIT,
	GAME_STATE_NORMAL,
	GAME_STATE_CLOSED,
	GAME_STATE_SHUTDOWN,
	GAME_STATE_CLOSING,
	GAME_STATE_MAINTAIN,
};

static constexpr int32_t PLAYER_NAME_LENGTH = 25;

static constexpr int32_t EVENT_DECAYINTERVAL = 250;
static constexpr int32_t EVENT_DECAY_BUCKETS = 4;

static constexpr int32_t MOVE_CREATURE_INTERVAL = 1000;
static constexpr int32_t RANGE_MOVE_CREATURE_INTERVAL = 1500;
static constexpr int32_t RANGE_MOVE_ITEM_INTERVAL = 400;
static constexpr int32_t RANGE_USE_ITEM_INTERVAL = 400;
static constexpr int32_t RANGE_USE_ITEM_EX_INTERVAL = 400;
static constexpr int32_t RANGE_USE_WITH_CREATURE_INTERVAL = 400;
static constexpr int32_t RANGE_ROTATE_ITEM_INTERVAL = 400;
static constexpr int32_t RANGE_BROWSE_FIELD_INTERVAL = 400;
static constexpr int32_t RANGE_WRAP_ITEM_INTERVAL = 400;
static constexpr int32_t RANGE_REQUEST_TRADE_INTERVAL = 400;

static constexpr int32_t MAX_STACKPOS = 10;

static constexpr uint8_t ITEM_STACK_SIZE = 100;

/**
 * Main Game class.
 * This class is responsible to control everything that happens
 */

class Game
{
public:
	Game();

	// non-copyable
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	void start(ServiceManager* manager);

	void forceAddCondition(uint32_t creatureId, Condition* condition);
	void forceRemoveCondition(uint32_t creatureId, ConditionType_t type);

	bool loadMainMap(const std::string& filename);
	void loadMap(const std::string& path, bool isCalledByLua = false);

	/**
	 * Get the map size - info purpose only
	 * \param width width of the map
	 * \param height height of the map
	 */
	void getMapDimensions(uint32_t& width, uint32_t& height) const
	{
		width = map.width;
		height = map.height;
	}

	void setWorldType(WorldType_t type);
	WorldType_t getWorldType() const { return worldType; }

	std::shared_ptr<Cylinder> internalGetCylinder(std::shared_ptr<Player> player, const Position& pos) const;
	std::shared_ptr<Thing> internalGetThing(std::shared_ptr<Player> player, const Position& pos, int32_t index,
	                                        uint32_t spriteId, stackPosType_t type) const;
	static void internalGetPosition(std::shared_ptr<Item> item, Position& pos, uint8_t& stackpos);

	static std::string getTradeErrorDescription(ReturnValue ret, std::shared_ptr<Item> item);

	/**
	 * Returns a creature based on the unique creature identifier
	 * \param id is the unique creature id to get a creature pointer to
	 * \returns A Creature pointer to the creature
	 */
	std::shared_ptr<Creature> getCreatureByID(uint32_t id);

	/**
	 * Returns a monster based on the unique creature identifier
	 * \param id is the unique monster id to get a monster pointer to
	 * \returns A Monster pointer to the monster
	 */
	std::shared_ptr<Monster> getMonsterByID(uint32_t id);

	/**
	 * Returns an npc based on the unique creature identifier
	 * \param id is the unique npc id to get a npc pointer to
	 * \returns A NPC pointer to the npc
	 */
	std::shared_ptr<Npc> getNpcByID(uint32_t id);

	/**
	 * Returns a player based on the unique creature identifier
	 * \param id is the unique player id to get a player pointer to
	 * \returns A Pointer to the player
	 */
	std::shared_ptr<Player> getPlayerByID(uint32_t id);

	/**
	 * Returns a creature based on a string name identifier
	 * \param s is the name identifier
	 * \returns A Pointer to the creature
	 */
	std::shared_ptr<Creature> getCreatureByName(const std::string& s);

	/**
	 * Returns a npc based on a string name identifier
	 * \param s is the name identifier
	 * \returns A Pointer to the npc
	 */
	std::shared_ptr<Npc> getNpcByName(const std::string& s);

	/**
	 * Returns a player based on a string name identifier
	 * \param s is the name identifier
	 * \returns A Pointer to the player
	 */
	std::shared_ptr<Player> getPlayerByName(const std::string& s);

	/**
	 * Returns a player based on guid
	 * \returns A Pointer to the player
	 */
	std::shared_ptr<Player> getPlayerByGUID(const uint32_t& guid);

	/**
	 * Returns a player based on a string name identifier, with support for the "~" wildcard.
	 * \param s is the name identifier, with or without wildcard
	 * \param player will point to the found player (if any)
	 * \return "RETURNVALUE_PLAYERWITHTHISNAMEISNOTONLINE" or "RETURNVALUE_NAMEISTOOAMBIGIOUS"
	 */
	ReturnValue getPlayerByNameWildcard(const std::string& s, std::shared_ptr<Player>& player);

	/**
	 * Returns a player based on an account number identifier
	 * \param acc is the account identifier
	 * \returns A Pointer to the player
	 */
	std::shared_ptr<Player> getPlayerByAccount(uint32_t acc);

	/**
	 * Place Creature on the map without sending out events to the surrounding.
	 * \param creature Creature to place on the map
	 * \param pos The position to place the creature
	 * \param extendedPos If true, the creature will in first-hand be placed 2 tiles away
	 * \param forced If true, placing the creature will not fail because of obstacles (creatures/items)
	 */
	bool internalPlaceCreature(std::shared_ptr<Creature> creature, const Position& pos, bool extendedPos = false,
	                           bool forced = false);

	/**
	 * Place Creature on the map.
	 * \param creature Creature to place on the map
	 * \param pos The position to place the creature
	 * \param extendedPos If true, the creature will in first-hand be placed 2 tiles away
	 * \param force If true, placing the creature will not fail because of obstacles (creatures/items)
	 */
	bool placeCreature(std::shared_ptr<Creature> creature, const Position& pos, bool extendedPos = false,
	                   bool forced = false, MagicEffectClasses magicEffect = CONST_ME_TELEPORT);

	/**
	 * Remove Creature from the map.
	 * Removes the Creature from the map
	 * \param c Creature to remove
	 */
	bool removeCreature(std::shared_ptr<Creature> creature, bool isLogout = true);
	void executeDeath(uint32_t creatureId);

	void addCreatureCheck(std::shared_ptr<Creature> creature);
	static void removeCreatureCheck(std::shared_ptr<Creature> creature);

	size_t getPlayersOnline() const { return players.size(); }
	size_t getMonstersOnline() const { return monsters.size(); }
	size_t getNpcsOnline() const { return npcs.size(); }
	uint32_t getPlayersRecord() const { return playersRecord; }

	ReturnValue internalMoveCreature(std::shared_ptr<Creature> creature, Direction direction, uint32_t flags = 0);
	ReturnValue internalMoveCreature(std::shared_ptr<Creature> creature, std::shared_ptr<Tile> toTile,
	                                 uint32_t flags = 0);

	ReturnValue internalMoveItem(std::shared_ptr<Cylinder> fromCylinder, std::shared_ptr<Cylinder> toCylinder,
	                             int32_t index, std::shared_ptr<Item> item, uint32_t count,
	                             std::shared_ptr<Item>* _moveItem, uint32_t flags = 0,
	                             std::shared_ptr<Creature> actor = nullptr, std::shared_ptr<Item> tradeItem = nullptr,
	                             const Position* fromPos = nullptr, const Position* toPos = nullptr);

	ReturnValue internalAddItem(std::shared_ptr<Cylinder> toCylinder, std::shared_ptr<Item> item,
	                            int32_t index = INDEX_WHEREEVER, uint32_t flags = 0, bool test = false);
	ReturnValue internalAddItem(std::shared_ptr<Cylinder> toCylinder, std::shared_ptr<Item> item, int32_t index,
	                            uint32_t flags, bool test, uint32_t& remainderCount);
	ReturnValue internalRemoveItem(std::shared_ptr<Item> item, int32_t count = -1, bool test = false,
	                               uint32_t flags = 0);

	ReturnValue internalPlayerAddItem(std::shared_ptr<Player> player, std::shared_ptr<Item> item, bool dropOnMap = true,
	                                  slots_t slot = CONST_SLOT_WHEREEVER);

	/**
	 * Find an item of a certain type
	 * \param cylinder to search the item
	 * \param itemId is the item to remove
	 * \param subType is the extra type an item can have such as charges/fluidtype, default is -1
	 * meaning it's not used
	 * \param depthSearch if true it will check child containers aswell
	 * \returns A pointer to the item to an item and nullptr if not found
	 */
	std::shared_ptr<Item> findItemOfType(std::shared_ptr<Cylinder> cylinder, uint16_t itemId, bool depthSearch = true,
	                                     int32_t subType = -1) const;

	/**
	 * Remove/Add item(s) with a monetary value
	 * \param cylinder to remove the money from
	 * \param money is the amount to remove
	 * \param flags optional flags to modify the default behavior
	 * \returns true if the removal was successful
	 */
	bool removeMoney(std::shared_ptr<Cylinder> cylinder, uint64_t money, uint32_t flags = 0);

	/**
	 * Add item(s) with monetary value
	 * \param cylinder which will receive money
	 * \param money the amount to give
	 * \param flags optional flags to modify default behavior
	 */
	void addMoney(std::shared_ptr<Cylinder> cylinder, uint64_t money, uint32_t flags = 0);

	/**
	 * Transform one item to another type/count
	 * \param item is the item to transform
	 * \param newId is the new itemid
	 * \param newCount is the new count value, use default value (-1) to not change it
	 * \returns true if the transformation was successful
	 */
	std::shared_ptr<Item> transformItem(std::shared_ptr<Item> item, uint16_t newId, int32_t newCount = -1);

	/**
	 * Teleports an object to another position
	 * \param thing is the object to teleport
	 * \param newPos is the new position
	 * \param pushMove force teleport if false
	 * \param flags optional flags to modify default behavior
	 * \returns true if the teleportation was successful
	 */
	ReturnValue internalTeleport(std::shared_ptr<Thing> thing, const Position& newPos, bool pushMove = true,
	                             uint32_t flags = 0);

	/**
	 * Turn a creature to a different direction.
	 * \param creature Creature to change the direction
	 * \param dir Direction to turn to
	 */
	bool internalCreatureTurn(std::shared_ptr<Creature> creature, Direction dir);

	/**
	 * Creature wants to say something.
	 * \param creature Creature pointer
	 * \param type Type of message
	 * \param text The text to say
	 */
	bool internalCreatureSay(std::shared_ptr<Creature> creature, SpeakClasses type, const std::string& text,
	                         bool ghostMode, SpectatorVec* spectatorsPtr = nullptr, const Position* pos = nullptr,
	                         bool echo = false);

	void loadPlayersRecord();
	void checkPlayersRecord();

	void sendGuildMotd(uint32_t playerId);
	void kickPlayer(uint32_t playerId, bool displayEffect);
	void playerDebugAssert(uint32_t playerId, const std::string& assertLine, const std::string& date,
	                       const std::string& description, const std::string& comment);
	void playerAnswerModalWindow(uint32_t playerId, uint32_t modalWindowId, uint8_t button, uint8_t choice);
	void playerReportRuleViolation(uint32_t playerId, const std::string& targetName, uint8_t reportType,
	                               uint8_t reportReason, const std::string& comment, const std::string& translation);

	bool internalStartTrade(std::shared_ptr<Player> player, std::shared_ptr<Player> tradePartner,
	                        std::shared_ptr<Item> tradeItem);
	void internalCloseTrade(std::shared_ptr<Player> player, bool sendCancel = true);
	bool playerBroadcastMessage(std::shared_ptr<Player> player, const std::string& text) const;
	void broadcastMessage(const std::string& text, MessageClasses type) const;

	// Implementation of player invoked events
	void playerMoveThing(uint32_t playerId, const Position& fromPos, uint16_t spriteId, uint8_t fromStackPos,
	                     const Position& toPos, uint8_t count);
	void playerMoveCreatureByID(uint32_t playerId, uint32_t movingCreatureId, const Position& movingCreatureOrigPos,
	                            const Position& toPos);
	void playerMoveCreature(std::shared_ptr<Player> player, std::shared_ptr<Creature> movingCreature,
	                        const Position& movingCreatureOrigPos, std::shared_ptr<Tile> toTile);
	void playerMoveItemByPlayerID(uint32_t playerId, const Position& fromPos, uint16_t spriteId, uint8_t fromStackPos,
	                              const Position& toPos, uint8_t count);
	void playerMoveItem(std::shared_ptr<Player> player, const Position& fromPos, uint16_t spriteId,
	                    uint8_t fromStackPos, const Position& toPos, uint8_t count, std::shared_ptr<Item> item,
	                    std::shared_ptr<Cylinder> toCylinder);
	void playerEquipItem(uint32_t playerId, uint16_t spriteId);
	void playerMove(uint32_t playerId, Direction direction);
	void playerCreatePrivateChannel(uint32_t playerId);
	void playerChannelInvite(uint32_t playerId, const std::string& name);
	void playerChannelExclude(uint32_t playerId, const std::string& name);
	void playerRequestChannels(uint32_t playerId);
	void playerOpenChannel(uint32_t playerId, uint16_t channelId);
	void playerCloseChannel(uint32_t playerId, uint16_t channelId);
	void playerOpenPrivateChannel(uint32_t playerId, std::string receiver);
	void playerCloseNpcChannel(uint32_t playerId);
	void playerReceivePing(uint32_t playerId);
	void playerReceivePingBack(uint32_t playerId);
	void playerAutoWalk(uint32_t playerId, const std::vector<Direction>& listDir);
	void playerStopAutoWalk(uint32_t playerId);
	void playerUseItemEx(uint32_t playerId, const Position& fromPos, uint8_t fromStackPos, uint16_t fromSpriteId,
	                     const Position& toPos, uint8_t toStackPos, uint16_t toSpriteId);
	void playerUseItem(uint32_t playerId, const Position& pos, uint8_t stackPos, uint8_t index, uint16_t spriteId);
	void playerUseWithCreature(uint32_t playerId, const Position& fromPos, uint8_t fromStackPos, uint32_t creatureId,
	                           uint16_t spriteId);
	void playerCloseContainer(uint32_t playerId, uint8_t cid);
	void playerMoveUpContainer(uint32_t playerId, uint8_t cid);
	void playerUpdateContainer(uint32_t playerId, uint8_t cid);
	void playerRotateItem(uint32_t playerId, const Position& pos, uint8_t stackPos, const uint16_t spriteId);
	void playerWriteItem(uint32_t playerId, uint32_t windowTextId, std::string_view text);
	void playerBrowseField(uint32_t playerId, const Position& pos);
	void playerSeekInContainer(uint32_t playerId, uint8_t containerId, uint16_t index);
	void playerUpdateHouseWindow(uint32_t playerId, uint8_t listId, uint32_t windowTextId, const std::string& text);
	void playerWrapItem(uint32_t playerId, const Position& position, uint8_t stackPos, const uint16_t spriteId);
	void playerRequestTrade(uint32_t playerId, const Position& pos, uint8_t stackPos, uint32_t tradePlayerId,
	                        uint16_t spriteId);
	void playerAcceptTrade(uint32_t playerId);
	void playerLookInTrade(uint32_t playerId, bool lookAtCounterOffer, uint8_t index);
	void playerPurchaseItem(uint32_t playerId, uint16_t spriteId, uint8_t count, uint16_t amount,
	                        bool ignoreCap = false, bool inBackpacks = false);
	void playerSellItem(uint32_t playerId, uint16_t spriteId, uint8_t count, uint16_t amount,
	                    bool ignoreEquipped = false);
	void playerCloseShop(uint32_t playerId);
	void playerLookInShop(uint32_t playerId, uint16_t spriteId, uint8_t count);
	void playerCloseTrade(uint32_t playerId);
	void playerSetAttackedCreature(uint32_t playerId, uint32_t creatureId);
	void playerFollowCreature(uint32_t playerId, uint32_t creatureId);
	void playerCancelAttackAndFollow(uint32_t playerId);
	void playerSetFightModes(uint32_t playerId, fightMode_t fightMode, bool chaseMode, bool secureMode);
	void playerLookAt(uint32_t playerId, const Position& pos, uint8_t stackPos);
	void playerLookInBattleList(uint32_t playerId, uint32_t creatureId);
	void playerRequestAddVip(uint32_t playerId, const std::string& name);
	void playerRequestRemoveVip(uint32_t playerId, uint32_t guid);
	void playerRequestEditVip(uint32_t playerId, uint32_t guid, const std::string& description, uint32_t icon,
	                          bool notify);
	void playerTurn(uint32_t playerId, Direction dir);
	void playerRequestOutfit(uint32_t playerId);
	void playerRequestEditPodium(uint32_t playerId, const Position& position, uint8_t stackPos,
	                             const uint16_t spriteId);
	void playerEditPodium(uint32_t playerId, Outfit_t outfit, const Position& position, uint8_t stackPos,
	                      const uint16_t spriteId, bool podiumVisible, Direction direction);
	void playerSay(uint32_t playerId, uint16_t channelId, SpeakClasses type, const std::string& receiver,
	               const std::string& text);
	void playerChangeOutfit(uint32_t playerId, Outfit_t outfit, bool randomizeMount = false);
	void playerInviteToParty(uint32_t playerId, uint32_t invitedId);
	void playerJoinParty(uint32_t playerId, uint32_t leaderId);
	void playerRevokePartyInvitation(uint32_t playerId, uint32_t invitedId);
	void playerPassPartyLeadership(uint32_t playerId, uint32_t newLeaderId);
	void playerLeaveParty(uint32_t playerId);
	void playerEnableSharedPartyExperience(uint32_t playerId, bool sharedExpActive);
	void playerToggleMount(uint32_t playerId, bool mount);
	void playerLeaveMarket(uint32_t playerId);
	void playerBrowseMarket(uint32_t playerId, uint16_t spriteId);
	void playerBrowseMarketOwnOffers(uint32_t playerId);
	void playerBrowseMarketOwnHistory(uint32_t playerId);
	void playerCreateMarketOffer(uint32_t playerId, uint8_t type, uint16_t spriteId, uint16_t amount, uint64_t price,
	                             bool anonymous);
	void playerCancelMarketOffer(uint32_t playerId, uint32_t timestamp, uint16_t counter);
	void playerAcceptMarketOffer(uint32_t playerId, uint32_t timestamp, uint16_t counter, uint16_t amount);

	void parsePlayerExtendedOpcode(uint32_t playerId, uint8_t opcode, const std::string& buffer);
	void parsePlayerNetworkMessage(uint32_t playerId, uint8_t recvByte, NetworkMessage_ptr msg);

	std::vector<std::shared_ptr<Item>> getMarketItemList(uint16_t wareId, uint16_t sufficientCount, Player& player);

	void cleanup();
	void shutdown();

	bool canThrowObjectTo(const Position& fromPos, const Position& toPos, bool checkLineOfSight = true,
	                      bool sameFloor = false, int32_t rangex = Map::maxClientViewportX,
	                      int32_t rangey = Map::maxClientViewportY) const;
	bool isSightClear(const Position& fromPos, const Position& toPos, bool sameFloor = false) const;

	void changeSpeed(std::shared_ptr<Creature> creature, int32_t varSpeedDelta);
	void internalCreatureChangeOutfit(std::shared_ptr<Creature> creature, const Outfit_t& outfit);
	void internalCreatureChangeVisible(std::shared_ptr<Creature> creature, bool visible);
	void changeLight(std::shared_ptr<const Creature> creature);
	void updateCreatureSkull(std::shared_ptr<const Creature> creature);
	void updatePlayerShield(std::shared_ptr<Player> player);
	void updateCreatureWalkthrough(std::shared_ptr<const Creature> creature);
	void updateKnownCreature(std::shared_ptr<const Creature> creature);

	GameState_t getGameState() const;
	void setGameState(GameState_t newState);
	void saveGameState();

	// Events
	void checkCreatureWalk(uint32_t creatureId);
	void updateCreatureWalk(uint32_t creatureId);
	void checkCreatureAttack(uint32_t creatureId);
	void checkCreatures(size_t index);
	void updateCreaturesPath(size_t index);

	bool combatBlockHit(CombatDamage& damage, std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> target,
	                    bool checkDefense, bool checkArmor, bool field, bool ignoreResistances = false);

	void combatGetTypeInfo(CombatType_t combatType, std::shared_ptr<Creature> target, TextColor_t& color,
	                       uint8_t& effect);

	bool combatChangeHealth(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> target, CombatDamage& damage);
	bool combatChangeMana(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> target, CombatDamage& damage);

	// animation help functions
	void addCreatureHealth(std::shared_ptr<const Creature> target);
	static void addCreatureHealth(const SpectatorVec& spectators, std::shared_ptr<const Creature> target);
	void addMagicEffect(const Position& pos, uint8_t effect);
	static void addMagicEffect(const SpectatorVec& spectators, const Position& pos, uint8_t effect);
	void addDistanceEffect(const Position& fromPos, const Position& toPos, uint8_t effect);
	static void addDistanceEffect(const SpectatorVec& spectators, const Position& fromPos, const Position& toPos,
	                              uint8_t effect);

	void startDecay(std::shared_ptr<Item> item);

	void sendOfflineTrainingDialog(std::shared_ptr<Player> player);

	const std::unordered_map<uint32_t, std::shared_ptr<Player>>& getPlayers() const { return players; }
	const std::map<uint32_t, std::shared_ptr<Npc>>& getNpcs() const { return npcs; }
	const std::map<uint32_t, std::shared_ptr<Monster>>& getMonsters() const { return monsters; }

	void addPlayer(std::shared_ptr<Player> player);
	void removePlayer(std::shared_ptr<Player> player);

	void addNpc(std::shared_ptr<Npc> npc);
	void removeNpc(std::shared_ptr<Npc> npc);

	void addMonster(std::shared_ptr<Monster> monster);
	void removeMonster(std::shared_ptr<Monster> monster);

	Guild_ptr getGuild(uint32_t id) const;
	void addGuild(Guild_ptr guild);
	void removeGuild(uint32_t guildId);
	void decreaseBrowseFieldRef(const Position& pos);

	std::unordered_map<std::shared_ptr<Tile>, std::shared_ptr<Container>> browseFields;

	void internalRemoveItems(std::vector<std::shared_ptr<Item>> itemList, uint32_t amount, bool stackable);

	std::shared_ptr<BedItem> getBedBySleeper(uint32_t guid) const;
	void setBedSleeper(std::shared_ptr<BedItem> bed, uint32_t guid);
	void removeBedSleeper(uint32_t guid);

	void updatePodium(std::shared_ptr<Item> item);

	std::shared_ptr<Item> getUniqueItem(uint16_t uniqueId);
	bool addUniqueItem(uint16_t uniqueId, std::shared_ptr<Item> item);
	void removeUniqueItem(uint16_t uniqueId);

	bool reload(ReloadTypes_t reloadType);

	Groups groups;
	Map map;
	Mounts mounts;

	std::forward_list<std::shared_ptr<Item>> toDecayItems;

	std::unordered_set<std::shared_ptr<Tile>> getTilesToClean() const { return tilesToClean; }
	bool isTileInCleanList(std::shared_ptr<Tile> tile) { return tilesToClean.find(tile) != tilesToClean.end(); }
	void addTileToClean(std::shared_ptr<Tile> tile) { tilesToClean.emplace(tile); }
	void removeTileToClean(std::shared_ptr<Tile> tile) { tilesToClean.erase(tile); }
	void clearTilesToClean() { tilesToClean.clear(); }

private:
	bool playerSaySpell(std::shared_ptr<Player> player, SpeakClasses type, const std::string& text);
	void playerWhisper(std::shared_ptr<Player> player, const std::string& text);
	bool playerYell(std::shared_ptr<Player> player, const std::string& text);
	bool playerSpeakTo(std::shared_ptr<Player> player, SpeakClasses type, const std::string& receiver,
	                   const std::string& text);
	void playerSpeakToNpc(std::shared_ptr<Player> player, const std::string& text);

	void checkDecay();
	void internalDecayItem(std::shared_ptr<Item> item);

	std::unordered_map<uint32_t, std::shared_ptr<Player>> players;
	std::unordered_map<std::string, std::shared_ptr<Player>> mappedPlayerNames;
	std::unordered_map<uint32_t, std::shared_ptr<Player>> mappedPlayerGuids;
	std::unordered_map<uint32_t, Guild_ptr> guilds;
	std::unordered_map<uint16_t, std::shared_ptr<Item>> uniqueItems;

	std::list<std::shared_ptr<Item>> decayItems[EVENT_DECAY_BUCKETS];
	std::list<std::shared_ptr<Creature>> checkCreatureLists[EVENT_CREATURECOUNT];

	std::vector<std::shared_ptr<Creature>> ToReleaseCreatures;
	std::vector<std::shared_ptr<Item>> ToReleaseItems;

	size_t lastBucket = 0;

	WildcardTreeNode wildcardTree{false};

	std::map<uint32_t, std::shared_ptr<Npc>> npcs;
	std::map<uint32_t, std::shared_ptr<Monster>> monsters;

	// list of items that are in trading state, mapped to the player holding them
	std::map<std::shared_ptr<Item>, uint32_t> tradeItems;

	std::map<uint32_t, std::shared_ptr<BedItem>> bedSleepersMap;

	std::unordered_set<std::shared_ptr<Tile>> tilesToClean;

	ModalWindow offlineTrainingWindow{std::numeric_limits<uint32_t>::max(), "Choose a Skill", "Please choose a skill:"};

	GameState_t gameState = GAME_STATE_NORMAL;
	WorldType_t worldType = WORLD_TYPE_PVP;

	ServiceManager* serviceManager = nullptr;

	void updatePlayersRecord() const;
	uint32_t playersRecord = 0;
};

#endif // FS_GAME_H
