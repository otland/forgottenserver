/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2014  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_GAME_H_3EC96D67DD024E6093B3BAC29B7A6D7F
#define FS_GAME_H_3EC96D67DD024E6093B3BAC29B7A6D7F

#include "account.h"
#include "combat.h"
#include "commands.h"
#include "groups.h"
#include "map.h"
#include "position.h"
#include "item.h"
#include "container.h"
#include "player.h"
#include "npc.h"
#include "wildcardtree.h"

class ServiceManager;
class Creature;
class Monster;
class Npc;
class CombatInfo;

enum stackPosType_t {
	STACKPOS_NORMAL,
	STACKPOS_MOVE,
	STACKPOS_LOOK,
	STACKPOS_USE,
	STACKPOS_USEITEM
};

enum WorldType_t {
	WORLD_TYPE_NO_PVP = 1,
	WORLD_TYPE_PVP = 2,
	WORLD_TYPE_PVP_ENFORCED = 3
};

enum GameState_t {
	GAME_STATE_STARTUP,
	GAME_STATE_INIT,
	GAME_STATE_NORMAL,
	GAME_STATE_CLOSED,
	GAME_STATE_SHUTDOWN,
	GAME_STATE_CLOSING,
	GAME_STATE_MAINTAIN
};

enum LightState_t {
	LIGHT_STATE_DAY,
	LIGHT_STATE_NIGHT,
	LIGHT_STATE_SUNSET,
	LIGHT_STATE_SUNRISE
};

#define EVENT_LIGHTINTERVAL 10000
#define EVENT_DECAYINTERVAL 250
#define EVENT_DECAY_BUCKETS 4

/**
  * Main Game class.
  * This class is responsible to control everything that happens
  */

class Game
{
	public:
		Game();
		~Game();

		void start(ServiceManager* servicer);

		void forceAddCondition(uint32_t creatureId, Condition* condition);
		void forceRemoveCondition(uint32_t creatureId, ConditionType_t type);

		/**
		  * Load a map.
		  * \param filename Mapfile to load
		  * \returns int32_t 0 built-in spawns, 1 needs xml spawns, 2 needs sql spawns, -1 if got error
		  */
		int32_t loadMainMap(const std::string& filename);
		void loadMap(const std::string& path);

		/**
		  * Get the map size - info purpose only
		  * \param width width of the map
		  * \param height height of the map
		  */
		void getMapDimensions(uint32_t& width, uint32_t& height) const {
			width = map.mapWidth;
			height = map.mapHeight;
		}

		void setWorldType(WorldType_t type);
		WorldType_t getWorldType() const {
			return worldType;
		}

		Cylinder* internalGetCylinder(Player* player, const Position& pos);
		Thing* internalGetThing(Player* player, const Position& pos, int32_t index,
		                        uint32_t spriteId = 0, stackPosType_t type = STACKPOS_NORMAL);
		static void internalGetPosition(Item* item, Position& pos, uint8_t& stackpos);

		static std::string getTradeErrorDescription(ReturnValue ret, Item* item);

		/**
		  * Get a single tile of the map.
		  * \returns A pointer to the tile
		*/
		Tile* getTile(int32_t x, int32_t y, int32_t z);
		Tile* getTile(const Position& pos);

		/**
		  * Set a single tile of the map, position is read from this tile
		*/
		void setTile(Tile* newTile);

		/**
		  * Get a leaf of the map.
		  * \returns A pointer to a leaf
		*/
		QTreeLeafNode* getLeaf(uint32_t x, uint32_t y);

		/**
		  * Returns a creature based on the unique creature identifier
		  * \param id is the unique creature id to get a creature pointer to
		  * \returns A Creature pointer to the creature
		  */
		Creature* getCreatureByID(uint32_t id);

		/**
		  * Returns a monster based on the unique creature identifier
		  * \param id is the unique monster id to get a monster pointer to
		  * \returns A Monster pointer to the monster
		  */
		Monster* getMonsterByID(uint32_t id);

		/**
		  * Returns a npc based on the unique creature identifier
		  * \param id is the unique npc id to get a npc pointer to
		  * \returns A NPC pointer to the npc
		  */
		Npc* getNpcByID(uint32_t id);

		/**
		  * Returns a player based on the unique creature identifier
		  * \param id is the unique player id to get a player pointer to
		  * \returns A Pointer to the player
		  */
		Player* getPlayerByID(uint32_t id);

		/**
		  * Returns a creature based on a string name identifier
		  * \param s is the name identifier
		  * \returns A Pointer to the creature
		  */
		Creature* getCreatureByName(const std::string& s);

		/**
		  * Returns a npc based on a string name identifier
		  * \param s is the name identifier
		  * \returns A Pointer to the npc
		  */
		Npc* getNpcByName(const std::string& s);

		/**
		  * Returns a player based on a string name identifier
		  * \param s is the name identifier
		  * \returns A Pointer to the player
		  */
		Player* getPlayerByName(const std::string& s);

		/**
		  * Returns a player based on guid
		  * \param guid
		  * \returns A Pointer to the player
		  */
		Player* getPlayerByGUID(const uint32_t& guid);

		/**
		  * Returns a player based on a string name identifier, with support for the "~" wildcard.
		  * \param s is the name identifier, with or without wildcard
		  * \param player will point to the found player (if any)
		  * \return "RET_PLAYERWITHTHISNAMEISNOTONLINE" or "RET_NAMEISTOOAMBIGIOUS"
		  */
		ReturnValue getPlayerByNameWildcard(const std::string& s, Player*& player);

		/**
		  * Returns a player based on an account number identifier
		  * \param acc is the account identifier
		  * \returns A Pointer to the player
		  */
		Player* getPlayerByAccount(uint32_t acc);

		/* Place Creature on the map without sending out events to the surrounding.
		  * \param creature Creature to place on the map
		  * \param pos The position to place the creature
		  * \param extendedPos If true, the creature will in first-hand be placed 2 tiles away
		  * \param forced If true, placing the creature will not fail because of obstacles (creatures/items)
		  */
		bool internalPlaceCreature(Creature* creature, const Position& pos, bool extendedPos = false, bool forced = false);

		/**
		  * Place Creature on the map.
		  * \param creature Creature to place on the map
		  * \param pos The position to place the creature
		  * \param extendedPos If true, the creature will in first-hand be placed 2 tiles away
		  * \param forced If true, placing the creature will not fail because of obstacles (creatures/items)
		  */
		bool placeCreature(Creature* creature, const Position& pos, bool extendedPos = false, bool force = false);

		/**
		  * Remove Creature from the map.
		  * Removes the Creature the map
		  * \param c Creature to remove
		  */
		bool removeCreature(Creature* creature, bool isLogout = true);

		void addCreatureCheck(Creature* creature);
		static void removeCreatureCheck(Creature* creature);

		size_t getPlayersOnline() const {
			return players.size();
		}
		size_t getMonstersOnline() const {
			return monsters.size();
		}
		size_t getNpcsOnline() const {
			return npcs.size();
		}
		uint32_t getPlayersRecord() const {
			return playersRecord;
		}

		void getWorldLightInfo(LightInfo& lightInfo) const;

		void getSpectators(SpectatorVec& list, const Position& centerPos, bool multifloor = false, bool onlyPlayers = false,
		                   int32_t minRangeX = 0, int32_t maxRangeX = 0,
		                   int32_t minRangeY = 0, int32_t maxRangeY = 0) {
			map.getSpectators(list, centerPos, multifloor, onlyPlayers, minRangeX, maxRangeX, minRangeY, maxRangeY);
		}

		const SpectatorVec& getSpectators(const Position& centerPos) {
			return map.getSpectators(centerPos);
		}

		void clearSpectatorCache() {
			map.clearSpectatorCache();
		}

		ReturnValue internalMoveCreature(Creature* creature, Direction direction, uint32_t flags = 0);
		ReturnValue internalMoveCreature(Creature* creature, Cylinder* fromCylinder, Cylinder* toCylinder, uint32_t flags = 0);

		ReturnValue internalMoveItem(Cylinder* fromCylinder, Cylinder* toCylinder, int32_t index,
		                             Item* item, uint32_t count, Item** _moveItem, uint32_t flags = 0, Creature* actor = nullptr, Item* tradeItem = nullptr);

		ReturnValue internalAddItem(Cylinder* toCylinder, Item* item, int32_t index = INDEX_WHEREEVER,
		                            uint32_t flags = 0, bool test = false);
		ReturnValue internalAddItem(Cylinder* toCylinder, Item* item, int32_t index,
		                            uint32_t flags, bool test, uint32_t& remainderCount);
		ReturnValue internalRemoveItem(Item* item, int32_t count = -1, bool test = false, uint32_t flags = 0);

		ReturnValue internalPlayerAddItem(Player* player, Item* item, bool dropOnMap = true, slots_t slot = CONST_SLOT_WHEREEVER);

		/**
		  * Find an item of a certain type
		  * \param cylinder to search the item
		  * \param itemId is the item to remove
		  * \param subType is the extra type an item can have such as charges/fluidtype, default is -1
			* meaning it's not used
		  * \param depthSearch if true it will check child containers aswell
		  * \returns A pointer to the item to an item and nullptr if not found
		  */
		Item* findItemOfType(Cylinder* cylinder, uint16_t itemId,
		                     bool depthSearch = true, int32_t subType = -1);

		/**
		  * Remove/Add item(s) with a monetary value
		  * \param cylinder to remove the money from
		  * \param money is the amount to remove
		  * \param flags optional flags to modifiy the default behaviour
		  * \returns true if the removal was successful
		  */
		bool removeMoney(Cylinder* cylinder, uint64_t money, uint32_t flags = 0);

		/**
		  * Add item(s) with monetary value
		  * \param cylinder which will receive money
		  * \param money the amount to give
		  * \param flags optional flags to modify default behavior
		  */
		void addMoney(Cylinder* cylinder, uint64_t money, uint32_t flags = 0);

		/**
		  * Transform one item to another type/count
		  * \param item is the item to transform
		  * \param newId is the new itemid
		  * \param newCount is the new count value, use default value (-1) to not change it
		  * \returns true if the tranformation was successful
		  */
		Item* transformItem(Item* item, uint16_t newId, int32_t newCount = -1);

		/**
		  * Teleports an object to another position
		  * \param thing is the object to teleport
		  * \param newPos is the new position
		  * \param pushMove force teleport if false
		  * \param flags optional flags to modify default behavior
		  * \returns true if the teleportation was successful
		  */
		ReturnValue internalTeleport(Thing* thing, const Position& newPos, bool pushMove = true, uint32_t flags = 0);

		/**
		  * Turn a creature to a different direction.
		  * \param creature Creature to change the direction
		  * \param dir Direction to turn to
		  */
		bool internalCreatureTurn(Creature* creature, Direction dir);

		/**
		  * Creature wants to say something.
		  * \param creature Creature pointer
		  * \param type Type of message
		  * \param text The text to say
		  */
		bool internalCreatureSay(Creature* creature, SpeakClasses type, const std::string& text,
		                         bool ghostMode, SpectatorVec* listPtr = nullptr, const Position* pos = nullptr);

		void loadPlayersRecord();
		void checkPlayersRecord();

		void sendGuildMotd(uint32_t playerId);
		void kickPlayer(uint32_t playerId, bool displayEffect);
		void playerReportBug(uint32_t playerId, const std::string& bug);
		void playerDebugAssert(uint32_t playerId, const std::string& assertLine, const std::string& date, const std::string& description, const std::string& comment);
		void playerAnswerModalWindow(uint32_t playerId, uint32_t modalWindowId, uint8_t button, uint8_t choice);

		bool internalStartTrade(Player* player, Player* partner, Item* tradeItem);
		bool internalCloseTrade(Player* player);
		bool playerBroadcastMessage(Player* player, const std::string& text) const;
		void broadcastMessage(const std::string& text, MessageClasses type) const;

		//Implementation of player invoked events
		void playerMoveThing(uint32_t playerId, const Position& fromPos, uint16_t spriteId, uint8_t fromStackPos,
		                     const Position& toPos, uint8_t count);
		void playerMoveCreature(uint32_t playerId, uint32_t movingCreatureId,
		                        const Position& movingCreatureOrigPos, const Position& toPos);
		void playerMoveItem(uint32_t playerId, const Position& fromPos,
		                    uint16_t spriteId, uint8_t fromStackPos, const Position& toPos, uint8_t count);
		void playerMove(uint32_t playerId, Direction direction);
		void playerCreatePrivateChannel(uint32_t playerId);
		void playerChannelInvite(uint32_t playerId, const std::string& name);
		void playerChannelExclude(uint32_t playerId, const std::string& name);
		void playerRequestChannels(uint32_t playerId);
		void playerOpenChannel(uint32_t playerId, uint16_t channelId);
		void playerCloseChannel(uint32_t playerId, uint16_t channelId);
		void playerOpenPrivateChannel(uint32_t playerId, std::string& receiver);
		void playerCloseNpcChannel(uint32_t playerId);
		void playerReceivePing(uint32_t playerId);
		void playerReceivePingBack(uint32_t playerId);
		void playerAutoWalk(uint32_t playerId, const std::list<Direction>& listDir);
		void playerStopAutoWalk(uint32_t playerId);
		void playerUseItemEx(uint32_t playerId, const Position& fromPos, uint8_t fromStackPos,
		                     uint16_t fromSpriteId, const Position& toPos, uint8_t toStackPos, uint16_t toSpriteId, bool isHotkey);
		void playerUseItem(uint32_t playerId, const Position& pos, uint8_t stackPos,
		                   uint8_t index, uint16_t spriteId, bool isHotkey);
		void playerUseWithCreature(uint32_t playerId, const Position& fromPos,
		                           uint8_t fromStackPos, uint32_t creatureId, uint16_t spriteId, bool isHotkey);
		void playerCloseContainer(uint32_t playerId, uint8_t cid);
		void playerMoveUpContainer(uint32_t playerId, uint8_t cid);
		void playerUpdateContainer(uint32_t playerId, uint8_t cid);
		void playerRotateItem(uint32_t playerId, const Position& pos, uint8_t stackPos, const uint16_t spriteId);
		void playerWriteItem(uint32_t playerId, uint32_t windowTextId, const std::string& text);
		void playerBrowseField(uint32_t playerId, const Position& pos);
		void playerSeekInContainer(uint32_t playerId, uint8_t containerId, uint16_t index);
		void playerUpdateHouseWindow(uint32_t playerId, uint8_t listId, uint32_t windowTextId, const std::string& text);
		void playerRequestTrade(uint32_t playerId, const Position& pos, uint8_t stackPos,
		                        uint32_t tradePlayerId, uint16_t spriteId);
		void playerAcceptTrade(uint32_t playerId);
		void playerLookInTrade(uint32_t playerId, bool lookAtCounterOffer, uint8_t index);
		void playerPurchaseItem(uint32_t playerId, uint16_t spriteId, uint8_t count, uint8_t amount,
		                        bool ignoreCap = false, bool inBackpacks = false);
		void playerSellItem(uint32_t playerId, uint16_t spriteId, uint8_t count,
		                    uint8_t amount, bool ignoreEquipped = false);
		void playerCloseShop(uint32_t playerId);
		void playerLookInShop(uint32_t playerId, uint16_t spriteId, uint8_t count);
		void playerCloseTrade(uint32_t playerId);
		void playerSetAttackedCreature(uint32_t playerId, uint32_t creatureId);
		void playerFollowCreature(uint32_t playerId, uint32_t creatureId);
		void playerCancelAttackAndFollow(uint32_t playerId);
		void playerSetFightModes(uint32_t playerId, fightMode_t fightMode, chaseMode_t chaseMode, secureMode_t secureMode);
		void playerLookAt(uint32_t playerId, const Position& pos, uint16_t spriteId, uint8_t stackPos);
		void playerLookInBattleList(uint32_t playerId, uint32_t creatureId);
		void playerRequestAddVip(uint32_t playerId, const std::string& name);
		void playerRequestRemoveVip(uint32_t playerId, uint32_t guid);
		void playerRequestEditVip(uint32_t playerId, uint32_t guid, const std::string& description, uint32_t icon, bool notify);
		void playerTurn(uint32_t playerId, Direction dir);
		void playerRequestOutfit(uint32_t playerId);
		void playerShowQuestLog(uint32_t playerId);
		void playerShowQuestLine(uint32_t playerId, uint16_t questId);
		void playerSay(uint32_t playerId, uint16_t channelId, SpeakClasses type,
		               const std::string& receiver, const std::string& text);
		void playerChangeOutfit(uint32_t playerId, Outfit_t outfit);
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
		void playerCreateMarketOffer(uint32_t playerId, uint8_t type, uint16_t spriteId, uint16_t amount, uint32_t price, bool anonymous);
		void playerCancelMarketOffer(uint32_t playerId, uint32_t timestamp, uint16_t counter);
		void playerAcceptMarketOffer(uint32_t playerId, uint32_t timestamp, uint16_t counter, uint16_t amount);
		void checkExpiredMarketOffers();

		void parsePlayerExtendedOpcode(uint32_t playerId, uint8_t opcode, const std::string& buffer);

		static void updatePremium(Account& account);

		void cleanup();
		void shutdown();
		void ReleaseCreature(Creature* creature);
		void ReleaseItem(Item* item);

		bool canThrowObjectTo(const Position& fromPos, const Position& toPos, bool checkLineOfSight = true,
		                      int32_t rangex = Map::maxClientViewportX, int32_t rangey = Map::maxClientViewportY) const;
		bool isSightClear(const Position& fromPos, const Position& toPos, bool sameFloor) const;

		void changeSpeed(Creature* creature, int32_t varSpeedDelta);
		void internalCreatureChangeOutfit(Creature* creature, const Outfit_t& oufit);
		void internalCreatureChangeVisible(Creature* creature, bool visible);
		void changeLight(const Creature* creature);
		void updatePlayerSkull(Player* player);
		void updatePlayerShield(Player* player);
		void updatePlayerHelpers(const Player& player);
		void updateCreatureType(Creature* creature);
		void updateCreatureWalkthrough(const Creature* creature);

		void sendPublicSquare(Player* sender, SquareColor_t color);

		GameState_t getGameState() const;
		void setGameState(GameState_t newState);
		void saveGameState();

		//Events
		void checkCreatureWalk(uint32_t creatureId);
		void updateCreatureWalk(uint32_t creatureId);
		void checkCreatureAttack(uint32_t creatureId);
		void checkCreatures(size_t index);
		void checkLight();

		bool combatBlockHit(CombatType_t combatType, Creature* attacker, Creature* target,
		                    int32_t& healthChange, bool checkDefense, bool checkArmor, bool field);

		void combatGetTypeInfo(CombatType_t combatType, Creature* target, TextColor_t& color, uint8_t& effect);

		bool combatChangeHealth(CombatType_t combatType, Creature* attacker, Creature* target, int32_t healthChange);
		bool combatChangeHealth(Creature* attacker, Creature* target, CombatDamage& damage);
		bool combatChangeMana(Creature* attacker, Creature* target, int32_t manaChange);

		//animation help functions
		void addCreatureHealth(const Creature* target);
		static void addCreatureHealth(const SpectatorVec& list, const Creature* target);
		void addMagicEffect(const Position& pos, uint8_t effect);
		static void addMagicEffect(const SpectatorVec& list, const Position& pos, uint8_t effect);
		void addDistanceEffect(const Position& fromPos, const Position& toPos, uint8_t effect);
		static void addDistanceEffect(const SpectatorVec& list, const Position& fromPos, const Position& toPos, uint8_t effect);

		Map* getMap() {
			return &map;
		}

		void addCommandTag(char tag);
		void resetCommandTag();

		void startDecay(Item* item);
		int32_t getLightHour() const {
			return lightHour;
		}

		bool loadExperienceStages();
		uint64_t getExperienceStage(uint32_t level);

		void loadMotdNum();
		void saveMotdNum() const;
		const std::string& getMotdHash() const { return motdHash; }
		int32_t getMotdNum() const { return motdNum; }
		void incrementMotdNum() { motdNum++; }

		void sendOfflineTrainingDialog(Player* player);

		const std::unordered_map<uint32_t, Player*>& getPlayers() const { return players; }
		const std::map<uint32_t, Npc*>& getNpcs() const { return npcs; }
		const std::map<uint32_t, Monster*>& getMonsters() const { return monsters; }

		void addPlayer(Player* player);
		void removePlayer(Player* player);

		void addNpc(Npc* npc);
		void removeNpc(Npc* npc);

		void addMonster(Monster* npc);
		void removeMonster(Monster* npc);

		Guild* getGuild(uint32_t id) const;
		void addGuild(Guild* guild);
		void decreaseBrowseFieldRef(const Position& pos);

		std::unordered_map<Tile*, Container*> browseFields;

		Group* getGroup(uint32_t id);

		void internalRemoveItems(std::vector<Item*> itemList, uint32_t amount, bool stackable);

	protected:
		bool playerSayCommand(Player* player, const std::string& text);
		bool playerSaySpell(Player* player, SpeakClasses type, const std::string& text);
		bool playerWhisper(Player* player, const std::string& text);
		bool playerYell(Player* player, const std::string& text);
		bool playerSpeakTo(Player* player, SpeakClasses type, const std::string& receiver, const std::string& text);
		bool playerSpeakToNpc(Player* player, const std::string& text);

		void checkDecay();
		void internalDecayItem(Item* item);

		Map map;

		std::unordered_map<uint32_t, Player*> players;
		std::unordered_map<std::string, Player*> mappedPlayerNames;
		std::unordered_map<uint32_t, Guild*> guilds;
		std::map<uint32_t, uint32_t> stages;

		std::list<Item*> decayItems[EVENT_DECAY_BUCKETS];
		std::list<Creature*> checkCreatureLists[EVENT_CREATURECOUNT];

		std::forward_list<Item*> toDecayItems;

		std::vector<Creature*> ToReleaseCreatures;
		std::vector<Item*> ToReleaseItems;
		std::vector<char> commandTags;

		size_t lastBucket;

		WildcardTreeNode wildcardTree;

		std::map<uint32_t, Npc*> npcs;
		std::map<uint32_t, Monster*> monsters;

		//list of items that are in trading state, mapped to the player
		std::map<Item*, uint32_t> tradeItems;

		ModalWindow offlineTrainingWindow;
		Commands commands;
		Groups groups;

		struct GameEvent {
			int64_t tick;
			int type;
			void* data;
		};

		static const int32_t LIGHT_LEVEL_DAY = 250;
		static const int32_t LIGHT_LEVEL_NIGHT = 40;
		static const int32_t SUNSET = 1305;
		static const int32_t SUNRISE = 430;

		GameState_t gameState;
		WorldType_t worldType;

		LightState_t lightState;
		int32_t lightLevel;
		int32_t lightHour;
		int32_t lightHourDelta;

		ServiceManager* services;

		void updatePlayersRecord() const;
		uint32_t playersRecord;

		std::string motdHash;
		int32_t motdNum;

		uint32_t lastStageLevel;
		bool stagesEnabled;
		bool useLastStageLevel;
};

#endif
