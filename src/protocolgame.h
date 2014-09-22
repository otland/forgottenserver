/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2016  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_PROTOCOLGAME_H_FACA2A2D1A9348B78E8FD7E8003EBB87
#define FS_PROTOCOLGAME_H_FACA2A2D1A9348B78E8FD7E8003EBB87

#include "protocol.h"
#include "chat.h"
#include "creature.h"
#include "tasks.h"
#include "protocolgamebase.h"
#include "protocolspectator.h"

class NetworkMessage;
class Player;
class Game;
class House;
class Container;
class Tile;
class Connection;
class Quest;
class ProtocolGame;
class ProtocolSpectator;
typedef std::shared_ptr<ProtocolGame> ProtocolGame_ptr;

extern Game g_game;

struct TextMessage
{
	MessageClasses type;
	std::string text;
	Position position;
	struct {
		int32_t value;
		TextColor_t color;
	} primary, secondary;

	TextMessage() {
		type = MESSAGE_STATUS_DEFAULT;
		primary.value = 0;
		secondary.value = 0;
	}
	TextMessage(MessageClasses type, std::string text) : type(type), text(text) {
		primary.value = 0;
		secondary.value = 0;
	}
};

class ProtocolGame final : public ProtocolGameBase
{
	public:
		static const char* protocol_name() {
			return "gameworld protocol";
		}
		explicit ProtocolGame(Connection_ptr connection):
			ProtocolGameBase(connection) {}

		void login(const std::string& name, uint32_t accnumber, OperatingSystem_t operatingSystem);
		void logout(bool displayEffect, bool forced);

		uint16_t getVersion() const {
			return version;
		}

		const std::unordered_set<uint32_t>& getKnownCreatures() const {
			return knownCreatureSet;
		}

		typedef std::unordered_map<Player*, ProtocolGame_ptr> LiveCastsMap;
		typedef std::vector<ProtocolSpectator_ptr> CastSpectatorVec;

		/** \brief Adds a spectator from the spectators vector.
		 *  \param spectatorClient pointer to the \ref ProtocolSpectator object representing the spectator
		 */
		void addSpectator(ProtocolSpectator_ptr spectatorClient);

		/** \brief Removes a spectator from the spectators vector.
		 *  \param spectatorClient pointer to the \ref ProtocolSpectator object representing the spectator
		 */
		void removeSpectator(ProtocolSpectator_ptr spectatorClient);

		/** \brief Starts the live cast.
		 *  \param password live cast password(optional)
		 *  \returns bool type indicating whether starting the cast was successful
		*/
		bool startLiveCast(const std::string& password = "");

		/** \brief Stops the live cast and disconnects all spectators.
		 *  \returns bool type indicating whether stopping the cast was successful
		*/
		bool stopLiveCast();

		const CastSpectatorVec& getLiveCastSpectators() const {
			return spectators;
		}

		size_t getSpectatorCount() const {
			return spectators.size();
		}

		bool isLiveCaster() const {
			return isCaster.load(std::memory_order_relaxed);
		}

		std::mutex liveCastLock;

		/** \brief Adds a new live cast to the list of available casts
		 */
		void registerLiveCast();

		/** \brief Removes a live cast from the list of available casts
		 */
		void unregisterLiveCast();

		/** \brief Update live cast info in the database.
		 *  \param player pointer to the casting \ref Player object
		 *  \param client pointer to the caster's \ref ProtocolGame object
		 */
		void updateLiveCastInfo();

		/** \brief Clears all live casts. Used to make sure there aro no live cast db rows left should a crash occur.
		 *  \warning Only supposed to be called once.
		 */
		static void clearLiveCastInfo();

		/** \brief Finds the caster's \ref ProtocolGame object
		 *  \param player pointer to the casting \ref Player object
		 *  \returns A pointer to the \ref ProtocolGame of the caster
		 */
		static ProtocolGame_ptr getLiveCast(Player* player) {
			const auto it = liveCasts.find(player);
			return it != liveCasts.end() ? it->second : nullptr;
		}

		const std::string& getLiveCastName() const {
			return liveCastName;
		}

		const std::string& getLiveCastPassword() const {
			return liveCastPassword;
		}

		bool isPasswordProtected() const {
			return !liveCastPassword.empty();
		}

		static const LiveCastsMap& getLiveCasts() {
			return liveCasts;
		}

		/** \brief Allows spectators to send text messages to the caster
		 *   and then get broadcast to the rest of the spectators
		 *  \param text string containing the text message
		 */
		void broadcastSpectatorMessage(const std::string& text) {
			if (player) {
				sendChannelMessage("Spectator", text, TALKTYPE_CHANNEL_Y, CHANNEL_CAST);
			}
		}

		static uint8_t getMaxLiveCastCount() {
			return std::numeric_limits<int8_t>::max();
		}

	private:
		ProtocolGame_ptr getThis() {
			return std::static_pointer_cast<ProtocolGame>(shared_from_this());
		}
		void connect(uint32_t playerId, OperatingSystem_t operatingSystem);
		void disconnectClient(const std::string& message) const;
		void writeToOutputBuffer(const NetworkMessage& msg, bool broadcast = true) final;

		void release() final;

		void checkCreatureAsKnown(uint32_t id, bool& known, uint32_t& removedKnown);

		// we have all the parse methods
		void parsePacket(NetworkMessage& msg) final;
		void onRecvFirstMessage(NetworkMessage& msg) final;

		//Parse methods
		void parseAutoWalk(NetworkMessage& msg);
		void parseSetOutfit(NetworkMessage& msg);
		void parseSay(NetworkMessage& msg);
		void parseLookAt(NetworkMessage& msg);
		void parseLookInBattleList(NetworkMessage& msg);
		void parseFightModes(NetworkMessage& msg);
		void parseAttack(NetworkMessage& msg);
		void parseFollow(NetworkMessage& msg);

		void parseBugReport(NetworkMessage& msg);
		void parseDebugAssert(NetworkMessage& msg);

		void parseThrow(NetworkMessage& msg);
		void parseUseItemEx(NetworkMessage& msg);
		void parseUseWithCreature(NetworkMessage& msg);
		void parseUseItem(NetworkMessage& msg);
		void parseCloseContainer(NetworkMessage& msg);
		void parseUpArrowContainer(NetworkMessage& msg);
		void parseUpdateContainer(NetworkMessage& msg);
		void parseTextWindow(NetworkMessage& msg);
		void parseHouseWindow(NetworkMessage& msg);

		void parseLookInShop(NetworkMessage& msg);
		void parsePlayerPurchase(NetworkMessage& msg);
		void parsePlayerSale(NetworkMessage& msg);

		void parseQuestLine(NetworkMessage& msg);

		void parseInviteToParty(NetworkMessage& msg);
		void parseJoinParty(NetworkMessage& msg);
		void parseRevokePartyInvite(NetworkMessage& msg);
		void parsePassPartyLeadership(NetworkMessage& msg);
		void parseEnableSharedPartyExperience(NetworkMessage& msg);

		void parseToggleMount(NetworkMessage& msg);

		void parseModalWindowAnswer(NetworkMessage& msg);

		void parseBrowseField(NetworkMessage& msg);
		void parseSeekInContainer(NetworkMessage& msg);

		//trade methods
		void parseRequestTrade(NetworkMessage& msg);
		void parseLookInTrade(NetworkMessage& msg);

		//market methods
		void parseMarketLeave();
		void parseMarketBrowse(NetworkMessage& msg);
		void parseMarketCreateOffer(NetworkMessage& msg);
		void parseMarketCancelOffer(NetworkMessage& msg);
		void parseMarketAcceptOffer(NetworkMessage& msg);

		//VIP methods
		void parseAddVip(NetworkMessage& msg);
		void parseRemoveVip(NetworkMessage& msg);
		void parseEditVip(NetworkMessage& msg);

		void parseRotateItem(NetworkMessage& msg);

		//Channel tabs
		void parseChannelInvite(NetworkMessage& msg);
		void parseChannelExclude(NetworkMessage& msg);
		void parseOpenChannel(NetworkMessage& msg);
		void parseOpenPrivateChannel(NetworkMessage& msg);
		void parseCloseChannel(NetworkMessage& msg);

		//Send functions
		void sendChannelMessage(const std::string& author, const std::string& text, SpeakClasses type, uint16_t channel);
		void sendChannelEvent(uint16_t channelId, const std::string& playerName, ChannelEvent_t channelEvent);
		void sendClosePrivate(uint16_t channelId);
		void sendCreatePrivateChannel(uint16_t channelId, const std::string& channelName);
		void sendChannelsDialog();
		void sendOpenPrivateChannel(const std::string& receiver);
		void sendToChannel(const Creature* creature, SpeakClasses type, const std::string& text, uint16_t channelId);
		void sendPrivateMessage(const Player* speaker, SpeakClasses type, const std::string& text);
		void sendIcons(uint16_t icons);
		void sendFYIBox(const std::string& message);

		void sendDistanceShoot(const Position& from, const Position& to, uint8_t type);
		void sendCreatureHealth(const Creature* creature);
		void sendCreatureTurn(const Creature* creature, uint32_t stackpos);
		void sendCreatureSay(const Creature* creature, SpeakClasses type, const std::string& text, const Position* pos = nullptr);

		void sendQuestLog();
		void sendQuestLine(const Quest* quest);

		void sendChangeSpeed(const Creature* creature, uint32_t speed);
		void sendCancelTarget();
		void sendCreatureVisible(const Creature* creature, bool visible);
		void sendCreatureOutfit(const Creature* creature, const Outfit_t& outfit);
		void sendTextMessage(const TextMessage& message);
		void sendReLoginWindow(uint8_t unfairFightReduction);

		void sendTutorial(uint8_t tutorialId);
		void sendAddMarker(const Position& pos, uint8_t markType, const std::string& desc);

		void sendCreatureWalkthrough(const Creature* creature, bool walkthrough);
		void sendCreatureShield(const Creature* creature);
		void sendCreatureSkull(const Creature* creature);
		void sendCreatureType(uint32_t creatureId, uint8_t creatureType);
		void sendCreatureHelpers(uint32_t creatureId, uint16_t helpers);

		void sendShop(Npc* npc, const ShopInfoList& itemList);
		void sendCloseShop();
		void sendSaleItemList(const std::list<ShopInfo>& shop);
		void sendMarketEnter(uint32_t depotId);
		void sendMarketLeave();
		void sendMarketBrowseItem(uint16_t itemId, const MarketOfferList& buyOffers, const MarketOfferList& sellOffers);
		void sendMarketAcceptOffer(const MarketOfferEx& offer);
		void sendMarketBrowseOwnOffers(const MarketOfferList& buyOffers, const MarketOfferList& sellOffers);
		void sendMarketCancelOffer(const MarketOfferEx& offer);
		void sendMarketBrowseOwnHistory(const HistoryMarketOfferList& buyOffers, const HistoryMarketOfferList& sellOffers);
		void sendMarketDetail(uint16_t itemId);
		void sendTradeItemRequest(const std::string& traderName, const Item* item, bool ack);
		void sendCloseTrade();

		void sendTextWindow(uint32_t windowTextId, Item* item, uint16_t maxlen, bool canWrite);
		void sendTextWindow(uint32_t windowTextId, uint32_t itemId, const std::string& text);
		void sendHouseWindow(uint32_t windowTextId, const std::string& text);
		void sendOutfitWindow();

		void sendUpdatedVIPStatus(uint32_t guid, VipStatus_t newStatus);

		void sendFightModes();

		void sendCreatureSquare(const Creature* creature, SquareColor_t color);

		void sendSpellCooldown(uint8_t spellId, uint32_t time);
		void sendSpellGroupCooldown(SpellGroup_t groupId, uint32_t time);

		//tiles

		void sendAddTileItem(const Position& pos, uint32_t stackpos, const Item* item);
		void sendUpdateTileItem(const Position& pos, uint32_t stackpos, const Item* item);
		void sendRemoveTileThing(const Position& pos, uint32_t stackpos);

		void sendMoveCreature(const Creature* creature, const Position& newPos, int32_t newStackPos,
		                      const Position& oldPos, int32_t oldStackPos, bool teleport);

		//containers
		void sendAddContainerItem(uint8_t cid, uint16_t slot, const Item* item);
		void sendUpdateContainerItem(uint8_t cid, uint16_t slot, const Item* item);
		void sendRemoveContainerItem(uint8_t cid, uint16_t slot, const Item* lastItem);

		void sendCloseContainer(uint8_t cid);

		//messages
		void sendModalWindow(const ModalWindow& modalWindow);

		//Help functions

		void MoveUpCreature(NetworkMessage& msg, const Creature* creature, const Position& newPos, const Position& oldPos);
		void MoveDownCreature(NetworkMessage& msg, const Creature* creature, const Position& newPos, const Position& oldPos);

		//container
		void AddContainerItem(NetworkMessage& msg, uint8_t cid, const Item* item);
		void UpdateContainerItem(NetworkMessage& msg, uint8_t cid, uint16_t slot, const Item* item);
		void RemoveContainerItem(NetworkMessage& msg, uint8_t cid, uint16_t slot);

		//inventory
		void SetInventoryItem(NetworkMessage& msg, slots_t slot, const Item* item);

		//shop
		void AddShopItem(NetworkMessage& msg, const ShopInfo& item);

		//otclient
		void parseExtendedOpcode(NetworkMessage& msg);

		friend class Player;

		// Helpers so we don't need to bind every time
		template <typename Callable, typename... Args>
		void addGameTask(Callable function, Args&&... args) {
			g_dispatcher.addTask(createTask(std::bind(function, &g_game, std::forward<Args>(args)...)));
		}

		template <typename Callable, typename... Args>
		void addGameTaskTimed(uint32_t delay, Callable function, Args&&... args) {
			g_dispatcher.addTask(createTask(delay, std::bind(function, &g_game, std::forward<Args>(args)...)));
		}

		static LiveCastsMap liveCasts; ///< Stores all available casts.

		std::atomic<bool> isCaster {false}; ///< Determines if this \ref ProtocolGame object is casting

		/// list of spectators \warning This variable should only be accessed after locking \ref liveCastLock
		CastSpectatorVec spectators;

		/// Live cast name that is also used as login
		std::string liveCastName;

		/// Password used to access the live cast
		std::string liveCastPassword;
};

#endif
