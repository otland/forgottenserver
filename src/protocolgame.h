// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_PROTOCOLGAME_H
#define FS_PROTOCOLGAME_H

#include "chat.h"
#include "creature.h"
#include "protocol.h"
#include "tasks.h"

class Container;
class Game;
class NetworkMessage;
class Player;
class ProtocolGame;
class Tile;

enum SessionEndTypes_t : uint8_t
{
	SESSION_END_LOGOUT = 0,
	SESSION_END_UNKNOWN = 1, // unknown, no difference from logout
	SESSION_END_FORCECLOSE = 2,
	SESSION_END_UNKNOWN2 = 3, // unknown, no difference from logout
};

using ProtocolGame_ptr = std::shared_ptr<ProtocolGame>;

extern Game g_game;

struct TextMessage
{
	MessageClasses type = MESSAGE_STATUS_DEFAULT;
	std::string text;
	Position position;
	uint16_t channelId;
	struct
	{
		int32_t value = 0;
		TextColor_t color;
	} primary, secondary;

	TextMessage() = default;
	TextMessage(MessageClasses type, std::string text) : type(type), text(std::move(text)) {}
};

class ProtocolGame final : public Protocol
{
public:
	// static protocol information
	enum
	{
		server_sends_first = true
	};
	enum
	{
		protocol_identifier = 0
	}; // Not required as we send first
	enum
	{
		use_checksum = true
	};
	static const char* protocol_name() { return "gameworld protocol"; }

	explicit ProtocolGame(Connection_ptr connection) : Protocol(connection) {}

	void login(uint32_t characterId, uint32_t accountId, OperatingSystem_t operatingSystem);
	void logout(bool displayEffect, bool forced);

	uint16_t getVersion() const { return version; }

private:
	ProtocolGame_ptr getThis() { return std::static_pointer_cast<ProtocolGame>(shared_from_this()); }
	void connect(uint32_t playerId, OperatingSystem_t operatingSystem);
	void disconnectClient(const std::string& message) const;
	void writeToOutputBuffer(const NetworkMessage& msg);

	void release() override;

	void checkCreatureAsKnown(uint32_t id, bool& known, uint32_t& removedKnown);

	bool canSee(int32_t x, int32_t y, int32_t z) const;
	bool canSee(const std::shared_ptr<const Creature>& creature) const;
	bool canSee(const Position& pos) const;

	// we have all the parse methods
	void parsePacket(NetworkMessage& msg) override;
	void onRecvFirstMessage(NetworkMessage& msg) override;
	void onConnect() override;

	// Parse methods
	void parseAutoWalk(NetworkMessage& msg);
	void parseSetOutfit(NetworkMessage& msg);
	void parseEditPodiumRequest(NetworkMessage& msg);
	void parseSay(NetworkMessage& msg);
	void parseLookAt(NetworkMessage& msg);
	void parseLookInBattleList(NetworkMessage& msg);
	void parseFightModes(NetworkMessage& msg);
	void parseAttack(NetworkMessage& msg);
	void parseFollow(NetworkMessage& msg);
	void parseEquipObject(NetworkMessage& msg);

	void parseDebugAssert(NetworkMessage& msg);
	void parseRuleViolationReport(NetworkMessage& msg);

	void parseThrow(NetworkMessage& msg);
	void parseUseItemEx(NetworkMessage& msg);
	void parseUseWithCreature(NetworkMessage& msg);
	void parseUseItem(NetworkMessage& msg);
	void parseCloseContainer(NetworkMessage& msg);
	void parseUpArrowContainer(NetworkMessage& msg);
	void parseUpdateContainer(NetworkMessage& msg);
	void parseTextWindow(NetworkMessage& msg);
	void parseHouseWindow(NetworkMessage& msg);
	void parseWrapItem(NetworkMessage& msg);

	void parseLookInShop(NetworkMessage& msg);
	void parsePlayerPurchase(NetworkMessage& msg);
	void parsePlayerSale(NetworkMessage& msg);

	void parseInviteToParty(NetworkMessage& msg);
	void parseJoinParty(NetworkMessage& msg);
	void parseRevokePartyInvite(NetworkMessage& msg);
	void parsePassPartyLeadership(NetworkMessage& msg);
	void parseEnableSharedPartyExperience(NetworkMessage& msg);

	void parseModalWindowAnswer(NetworkMessage& msg);

	void parseBrowseField(NetworkMessage& msg);
	void parseSeekInContainer(NetworkMessage& msg);

	// trade methods
	void parseRequestTrade(NetworkMessage& msg);
	void parseLookInTrade(NetworkMessage& msg);

	// market methods
	void parseMarketLeave();
	void parseMarketBrowse(NetworkMessage& msg);
	void parseMarketCreateOffer(NetworkMessage& msg);
	void parseMarketCancelOffer(NetworkMessage& msg);
	void parseMarketAcceptOffer(NetworkMessage& msg);

	// VIP methods
	void parseAddVip(NetworkMessage& msg);
	void parseRemoveVip(NetworkMessage& msg);
	void parseEditVip(NetworkMessage& msg);

	void parseRotateItem(NetworkMessage& msg);

	// Channel tabs
	void parseChannelInvite(NetworkMessage& msg);
	void parseChannelExclude(NetworkMessage& msg);
	void parseOpenChannel(NetworkMessage& msg);
	void parseOpenPrivateChannel(NetworkMessage& msg);
	void parseCloseChannel(NetworkMessage& msg);

	// Send functions
	void sendChannelMessage(const std::string& author, const std::string& text, SpeakClasses type, uint16_t channel);
	void sendChannelEvent(uint16_t channelId, const std::string& playerName, ChannelEvent_t channelEvent);
	void sendClosePrivate(uint16_t channelId);
	void sendCreatePrivateChannel(uint16_t channelId, const std::string& channelName);
	void sendChannelsDialog();
	void sendChannel(uint16_t channelId, const std::string& channelName, const UsersMap* channelUsers,
	                 const InvitedMap* invitedUsers);
	void sendOpenPrivateChannel(const std::string& receiver);
	void sendToChannel(const std::shared_ptr<const Creature>& creature, SpeakClasses type, const std::string& text,
	                   uint16_t channelId);
	void sendPrivateMessage(const std::shared_ptr<const Player>& speaker, SpeakClasses type, const std::string& text);
	void sendIcons(uint32_t icons);
	void sendFYIBox(const std::string& message);

	void sendDistanceShoot(const Position& from, const Position& to, uint8_t type);
	void sendMagicEffect(const Position& pos, uint8_t type);
	void sendCreatureHealth(const std::shared_ptr<const Creature>& creature);
	void sendSkills();
	void sendPing();
	void sendPingBack();
	void sendCreatureTurn(const std::shared_ptr<const Creature>& creature, uint32_t stackpos);
	void sendCreatureSay(const std::shared_ptr<const Creature>& creature, SpeakClasses type, const std::string& text,
	                     const Position* pos = nullptr);

	void sendCancelWalk();
	void sendChangeSpeed(const std::shared_ptr<const Creature>& creature, uint32_t speed);
	void sendCancelTarget();
	void sendCreatureOutfit(const std::shared_ptr<const Creature>& creature, const Outfit_t& outfit);
	void sendStats();
	void sendExperienceTracker(int64_t rawExp, int64_t finalExp);
	void sendClientFeatures();
	void sendBasicData();
	void sendTextMessage(const TextMessage& message);
	void sendReLoginWindow(uint8_t unfairFightReduction);

	void sendTutorial(uint8_t tutorialId);
	void sendAddMarker(const Position& pos, uint8_t markType, const std::string& desc);

	void sendCreatureWalkthrough(const std::shared_ptr<const Creature>& creature, bool walkthrough);
	void sendCreatureShield(const std::shared_ptr<const Creature>& creature);
	void sendCreatureSkull(const std::shared_ptr<const Creature>& creature);

	void sendShop(const std::shared_ptr<Npc>& npc, const ShopInfoList& itemList);
	void sendCloseShop();
	void sendSaleItemList(const std::list<ShopInfo>& shop);
	void sendResourceBalance(const ResourceTypes_t resourceType, uint64_t amount);
	void sendStoreBalance();
	void sendMarketEnter();
	void sendMarketLeave();
	void sendMarketBrowseItem(uint16_t itemId, const MarketOfferList& buyOffers, const MarketOfferList& sellOffers);
	void sendMarketAcceptOffer(const MarketOfferEx& offer);
	void sendMarketBrowseOwnOffers(const MarketOfferList& buyOffers, const MarketOfferList& sellOffers);
	void sendMarketCancelOffer(const MarketOfferEx& offer);
	void sendMarketBrowseOwnHistory(const HistoryMarketOfferList& buyOffers, const HistoryMarketOfferList& sellOffers);
	void sendTradeItemRequest(const std::string& traderName, const std::shared_ptr<const Item>& item, bool ack);
	void sendCloseTrade();

	void sendTextWindow(uint32_t windowTextId, const std::shared_ptr<const Item>& item, uint16_t maxlen, bool canWrite);
	void sendTextWindow(uint32_t windowTextId, uint32_t itemId, const std::string& text);
	void sendHouseWindow(uint32_t windowTextId, const std::string& text);
	void sendCombatAnalyzer(CombatType_t type, int32_t amount, DamageAnalyzerImpactType impactType,
	                        const std::string& target);
	void sendOutfitWindow();

	void sendPodiumWindow(const std::shared_ptr<const Item>& item);

	void sendUpdatedVIPStatus(uint32_t guid, VipStatus_t newStatus);
	void sendVIP(uint32_t guid, const std::string& name, const std::string& description, uint32_t icon, bool notify,
	             VipStatus_t status);
	void sendVIPEntries();

	void sendItemClasses();

	void sendPendingStateEntered();
	void sendEnterWorld();

	void sendFightModes();

	void sendCreatureLight(const std::shared_ptr<const Creature>& creature);

	void sendCreatureSquare(const std::shared_ptr<const Creature>& creature, SquareColor_t color);

	void sendSpellCooldown(uint8_t spellId, uint32_t time);
	void sendSpellGroupCooldown(SpellGroup_t groupId, uint32_t time);
	void sendUseItemCooldown(uint32_t time);
	void sendSupplyUsed(const uint16_t clientId);

	// tiles
	void sendMapDescription(const Position& pos);

	void sendAddTileItem(const Position& pos, uint32_t stackpos, const std::shared_ptr<const Item>& item);
	void sendUpdateTileItem(const Position& pos, uint32_t stackpos, const std::shared_ptr<const Item>& item);
	void sendRemoveTileThing(const Position& pos, uint32_t stackpos);
	void sendUpdateTileCreature(const Position& pos, uint32_t stackpos,
	                            const std::shared_ptr<const Creature>& creature);
	void sendRemoveTileCreature(const std::shared_ptr<const Creature>& creature, const Position& pos,
	                            uint32_t stackpos);
	void sendUpdateTile(const std::shared_ptr<const Tile>& tile, const Position& pos);

	void sendUpdateCreatureIcons(const std::shared_ptr<const Creature>& creature);

	void sendAddCreature(const std::shared_ptr<const Creature>& creature, const Position& pos, int32_t stackpos,
	                     MagicEffectClasses magicEffect = CONST_ME_NONE);
	void sendMoveCreature(const std::shared_ptr<const Creature>& creature, const Position& newPos, int32_t newStackPos,
	                      const Position& oldPos, int32_t oldStackPos, bool teleport);

	// containers
	void sendAddContainerItem(uint8_t cid, uint16_t slot, const std::shared_ptr<const Item>& item);
	void sendUpdateContainerItem(uint8_t cid, uint16_t slot, const std::shared_ptr<const Item>& item);
	void sendRemoveContainerItem(uint8_t cid, uint16_t slot, const std::shared_ptr<const Item>& lastItem);

	void sendContainer(uint8_t cid, const std::shared_ptr<const Container>& container, uint16_t firstIndex);
	void sendEmptyContainer(uint8_t cid);
	void sendCloseContainer(uint8_t cid);

	// inventory
	void sendInventoryItem(slots_t slot, const std::shared_ptr<const Item>& item);
	void sendItems();

	// messages
	void sendModalWindow(const ModalWindow& modalWindow);

	// session end
	void sendSessionEnd(SessionEndTypes_t reason);

	// Help functions

	// translate a tile to client-readable format
	void GetTileDescription(const std::shared_ptr<const Tile>& tile, NetworkMessage& msg);

	// translate a floor to client-readable format
	void GetFloorDescription(NetworkMessage& msg, int32_t x, int32_t y, int32_t z, int32_t width, int32_t height,
	                         int32_t offset, int32_t& skip);

	// translate a map area to client-readable format
	void GetMapDescription(int32_t x, int32_t y, int32_t z, int32_t width, int32_t height, NetworkMessage& msg);

	void AddCreature(NetworkMessage& msg, const std::shared_ptr<const Creature>& creature, bool known, uint32_t remove);
	void AddCreatureIcons(NetworkMessage& msg, const std::shared_ptr<const Creature>& creature);
	void AddPlayerStats(NetworkMessage& msg);
	void AddOutfit(NetworkMessage& msg, const Outfit_t& outfit);
	void AddPlayerSkills(NetworkMessage& msg);

	// tiles
	static void RemoveTileThing(NetworkMessage& msg, const Position& pos, uint32_t stackpos);
	static void RemoveTileCreature(NetworkMessage& msg, const std::shared_ptr<const Creature>& creature,
	                               const Position& pos, uint32_t stackpos);

	void MoveUpCreature(NetworkMessage& msg, const std::shared_ptr<const Creature>& creature, const Position& newPos,
	                    const Position& oldPos);
	void MoveDownCreature(NetworkMessage& msg, const std::shared_ptr<const Creature>& creature, const Position& newPos,
	                      const Position& oldPos);

	// shop
	void AddShopItem(NetworkMessage& msg, const ShopInfo& item);

	// otclient
	void parseExtendedOpcode(NetworkMessage& msg);

	friend class Player;

	std::unordered_set<uint32_t> knownCreatureSet;
	std::shared_ptr<Player> player = nullptr;

	uint32_t eventConnect = 0;
	uint32_t challengeTimestamp = 0;
	uint16_t version = CLIENT_VERSION_MIN;

	uint8_t challengeRandom = 0;

	bool debugAssertSent = false;
	bool acceptPackets = false;
};

#endif // FS_PROTOCOLGAME_H
