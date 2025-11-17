// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "protocolgame.h"

#include "ban.h"
#include "base64.h"
#include "condition.h"
#include "configmanager.h"
#include "depotchest.h"
#include "game.h"
#include "inbox.h"
#include "iologindata.h"
#include "iomarket.h"
#include "monster.h"
#include "npc.h"
#include "outfit.h"
#include "outputmessage.h"
#include "player.h"
#include "podium.h"
#include "scheduler.h"
#include "storeinbox.h"

extern CreatureEvents* g_creatureEvents;
extern Chat* g_chat;

namespace {

std::deque<std::pair<int64_t, uint32_t>> waitList; // (timeout, player guid)
auto priorityEnd = waitList.end();

auto findClient(uint32_t guid)
{
	std::size_t slot = 1;
	for (auto it = waitList.begin(), end = waitList.end(); it != end; ++it, ++slot) {
		if (it->second == guid) {
			return std::make_pair(it, slot);
		}
	}
	return std::make_pair(waitList.end(), slot);
}

constexpr int64_t getWaitTime(std::size_t slot)
{
	if (slot < 5) {
		return 5;
	} else if (slot < 10) {
		return 10;
	} else if (slot < 20) {
		return 20;
	} else if (slot < 50) {
		return 60;
	}
	return 120;
}

constexpr int64_t getTimeout(std::size_t slot)
{
	// timeout is set to 15 seconds longer than expected retry attempt
	return getWaitTime(slot) + 15;
}

std::size_t clientLogin(const Player& player)
{
	if (player.hasFlag(PlayerFlag_CanAlwaysLogin) || player.getAccountType() >= ACCOUNT_TYPE_GAMEMASTER) {
		return 0;
	}

	uint32_t maxPlayers = static_cast<uint32_t>(getNumber(ConfigManager::MAX_PLAYERS));
	if (maxPlayers == 0 || (waitList.empty() && g_game.getPlayersOnline() < maxPlayers)) {
		return 0;
	}

	int64_t time = OTSYS_TIME();

	auto it = waitList.begin();
	while (it != waitList.end()) {
		if ((it->first - time) <= 0) {
			it = waitList.erase(it);
		} else {
			++it;
		}
	}

	std::size_t slot;
	std::tie(it, slot) = findClient(player.getGUID());
	if (it != waitList.end()) {
		// If server has capacity for this client, let him in even though his current slot might be higher than 0.
		if ((g_game.getPlayersOnline() + slot) <= maxPlayers) {
			waitList.erase(it);
			return 0;
		}

		// let them wait a bit longer
		it->first = time + (getTimeout(slot) * 1000);
		return slot;
	}

	if (player.isPremium()) {
		priorityEnd = waitList.emplace(priorityEnd, time + (getTimeout(slot + 1) * 1000), player.getGUID());
		return std::distance(waitList.begin(), priorityEnd);
	}

	waitList.emplace_back(time + (getTimeout(waitList.size() + 1) * 1000), player.getGUID());
	return waitList.size();
}

ClientDamageType getClientDamageType(CombatType_t combatType)
{
	switch (combatType) {
		case COMBAT_PHYSICALDAMAGE:
			return CLIENT_DAMAGETYPE_PHYSICAL;
		case COMBAT_ENERGYDAMAGE:
			return CLIENT_DAMAGETYPE_ENERGY;
		case COMBAT_EARTHDAMAGE:
			return CLIENT_DAMAGETYPE_EARTH;
		case COMBAT_FIREDAMAGE:
			return CLIENT_DAMAGETYPE_FIRE;
		case COMBAT_LIFEDRAIN:
			return CLIENT_DAMAGETYPE_LIFEDRAIN;
		case COMBAT_HEALING:
			return CLIENT_DAMAGETYPE_HEALING;
		case COMBAT_DROWNDAMAGE:
			return CLIENT_DAMAGETYPE_DROWN;
		case COMBAT_ICEDAMAGE:
			return CLIENT_DAMAGETYPE_ICE;
		case COMBAT_HOLYDAMAGE:
			return CLIENT_DAMAGETYPE_HOLY;
		case COMBAT_DEATHDAMAGE:
			return CLIENT_DAMAGETYPE_DEATH;
		default:
			return CLIENT_DAMAGETYPE_UNDEFINED;
	}
}

} // namespace

void ProtocolGame::release()
{
	// dispatcher thread
	if (player && player->client == shared_from_this()) {
		player->client.reset();
		player->decrementReferenceCounter();
		player = nullptr;
	}

	tfs::net::remove_protocol_from_autosend(shared_from_this());

	Protocol::release();
}

void ProtocolGame::login(uint32_t characterId, uint32_t accountId, OperatingSystem_t operatingSystem)
{
	// dispatcher thread
	Player* foundPlayer = g_game.getPlayerByGUID(characterId);
	if (!foundPlayer || getBoolean(ConfigManager::ALLOW_CLONES)) {
		player = new Player(getThis());

		player->incrementReferenceCounter();
		player->setID();
		player->setGUID(characterId);

		if (!IOLoginData::preloadPlayer(player)) {
			disconnectClient("Your character could not be loaded.");
			return;
		}

		if (IOBan::isPlayerNamelocked(player->getGUID())) {
			disconnectClient("Your character has been namelocked.");
			return;
		}

		if (g_game.getGameState() == GAME_STATE_CLOSING && !player->hasFlag(PlayerFlag_CanAlwaysLogin)) {
			disconnectClient("The game is just going down.\nPlease try again later.");
			return;
		}

		if (g_game.getGameState() == GAME_STATE_CLOSED && !player->hasFlag(PlayerFlag_CanAlwaysLogin)) {
			disconnectClient("Server is currently closed.\nPlease try again later.");
			return;
		}

		if (getBoolean(ConfigManager::ONE_PLAYER_ON_ACCOUNT) && player->getAccountType() < ACCOUNT_TYPE_GAMEMASTER &&
		    g_game.getPlayerByAccount(player->getAccount())) {
			disconnectClient("You may only login with one character\nof your account at the same time.");
			return;
		}

		if (!player->hasFlag(PlayerFlag_CannotBeBanned)) {
			if (const auto& banInfo = IOBan::getAccountBanInfo(accountId)) {
				if (banInfo->expiresAt > 0) {
					disconnectClient(
					    std::format("Your account has been banned until {:s} by {:s}.\n\nReason specified:\n{:s}",
					                formatDateShort(banInfo->expiresAt), banInfo->bannedBy, banInfo->reason));
				} else {
					disconnectClient(
					    std::format("Your account has been permanently banned by {:s}.\n\nReason specified:\n{:s}",
					                banInfo->bannedBy, banInfo->reason));
				}
				return;
			}
		}

		if (std::size_t currentSlot = clientLogin(*player)) {
			uint8_t retryTime = getWaitTime(currentSlot);
			auto output = tfs::net::make_output_message();
			output->addByte(0x16);
			output->addString(
			    std::format("Too many players online.\nYou are at place {:d} on the waiting list.", currentSlot));
			output->addByte(retryTime);
			send(output);

			disconnect();
			return;
		}

		if (!IOLoginData::loadPlayerById(player, player->getGUID())) {
			disconnectClient("Your character could not be loaded.");
			return;
		}

		player->setOperatingSystem(operatingSystem);

		if (!g_game.placeCreature(player, player->getLoginPosition())) {
			if (!g_game.placeCreature(player, player->getTemplePosition(), false, true)) {
				disconnectClient("Temple position is wrong. Contact the administrator.");
				return;
			}
		}

		if (operatingSystem >= CLIENTOS_OTCLIENT_LINUX) {
			player->registerCreatureEvent("ExtendedOpcode");
		}

		player->lastIP = player->getIP();
		player->lastLoginSaved = std::max<time_t>(time(nullptr), player->lastLoginSaved + 1);
		acceptPackets = true;
	} else {
		if (eventConnect != 0 || !getBoolean(ConfigManager::REPLACE_KICK_ON_LOGIN)) {
			// Already trying to connect
			disconnectClient("You are already logged in.");
			return;
		}

		if (foundPlayer->client) {
			foundPlayer->disconnect();
			foundPlayer->isConnecting = true;

			eventConnect = g_scheduler.addEvent(
			    createSchedulerTask(1000, [=, thisPtr = getThis(), playerID = foundPlayer->getID()]() {
				    thisPtr->connect(playerID, operatingSystem);
			    }));
		} else {
			connect(foundPlayer->getID(), operatingSystem);
		}
	}

	tfs::net::insert_protocol_to_autosend(shared_from_this());
}

void ProtocolGame::connect(uint32_t playerId, OperatingSystem_t operatingSystem)
{
	// dispatcher thread
	eventConnect = 0;

	Player* foundPlayer = g_game.getPlayerByID(playerId);
	if (!foundPlayer || foundPlayer->client) {
		disconnectClient("You are already logged in.");
		return;
	}

	if (isConnectionExpired()) {
		// ProtocolGame::release() has been called at this point and the Connection object no longer exists, so we
		// return to prevent leakage of the Player.
		return;
	}

	player = foundPlayer;
	player->incrementReferenceCounter();

	g_chat->removeUserFromAllChannels(*player);
	player->clearModalWindows();
	player->setOperatingSystem(operatingSystem);
	player->isConnecting = false;

	player->client = getThis();
	sendAddCreature(player, player->getPosition(), 0);
	player->lastIP = player->getIP();
	player->lastLoginSaved = std::max<time_t>(time(nullptr), player->lastLoginSaved + 1);
	player->resetIdleTime();
	acceptPackets = true;

	g_creatureEvents->playerReconnect(player);
}

void ProtocolGame::logout(bool displayEffect, bool forced)
{
	// dispatcher thread
	if (!player) {
		return;
	}

	if (!player->isRemoved()) {
		if (!forced) {
			if (!player->isAccessPlayer()) {
				if (player->getTile()->hasFlag(TILESTATE_NOLOGOUT)) {
					player->sendCancelMessage(RETURNVALUE_YOUCANNOTLOGOUTHERE);
					return;
				}

				if (!player->getTile()->hasFlag(TILESTATE_PROTECTIONZONE) && player->hasCondition(CONDITION_INFIGHT)) {
					player->sendCancelMessage(RETURNVALUE_YOUMAYNOTLOGOUTDURINGAFIGHT);
					return;
				}
			}

			// scripting event - onLogout
			if (!g_creatureEvents->playerLogout(player)) {
				// Let the script handle the error message
				return;
			}
		}

		if (displayEffect && !player->isDead() && !player->isInGhostMode()) {
			g_game.addMagicEffect(player->getPosition(), CONST_ME_POFF);
		}
	}

	sendSessionEnd(forced ? SESSION_END_FORCECLOSE : SESSION_END_LOGOUT);
	disconnect();

	g_game.removeCreature(player);
}

// Login to the game world request
void ProtocolGame::onRecvFirstMessage(NetworkMessage& msg)
{
	// Server is shutting down
	if (g_game.getGameState() == GAME_STATE_SHUTDOWN) {
		disconnect();
		return;
	}

	// Client type and OS used
	OperatingSystem_t operatingSystem = static_cast<OperatingSystem_t>(msg.get<uint16_t>());

	version = msg.get<uint16_t>(); // U16 client version
	msg.skipBytes(4);              // U32 client version

	// String client version
	if (version >= 1240) {
		if (msg.getRemainingBufferLength() > 132) {
			msg.getString();
		}
	}

	msg.skipBytes(3); // U16 dat revision, U8 preview state

	// Disconnect if RSA decrypt fails
	if (!Protocol::RSA_decrypt(msg)) {
		disconnect();
		return;
	}

	// Get XTEA key
	xtea::key key;
	key[0] = msg.get<uint32_t>();
	key[1] = msg.get<uint32_t>();
	key[2] = msg.get<uint32_t>();
	key[3] = msg.get<uint32_t>();
	enableXTEAEncryption();
	setXTEAKey(std::move(key));

	// Enable extended opcode feature for otclient
	if (operatingSystem >= CLIENTOS_OTCLIENT_LINUX) {
		NetworkMessage opcodeMessage;
		opcodeMessage.addByte(0x32);
		opcodeMessage.addByte(0x00);
		opcodeMessage.add<uint16_t>(0x00);
		writeToOutputBuffer(opcodeMessage);
	}

	// Change packet verifying mode for QT clients
	if (version >= 1111 && operatingSystem >= CLIENTOS_QT_LINUX && operatingSystem <= CLIENTOS_OTCLIENT_MAC) {
		setChecksumMode(CHECKSUM_SEQUENCE);
	}

	// Web login skips the character list request so we need to check the client version again
	if (version < CLIENT_VERSION_MIN || version > CLIENT_VERSION_MAX) {
		disconnectClient(std::format("Only clients with protocol {:s} allowed!", CLIENT_VERSION_STR));
		return;
	}

	msg.skipBytes(1); // Gamemaster flag

	auto sessionToken = tfs::base64::decode(msg.getString());
	if (sessionToken.empty()) {
		disconnectClient("Malformed session key.");
		return;
	}

	if (operatingSystem == CLIENTOS_QT_LINUX) {
		msg.getString(); // OS name (?)
		msg.getString(); // OS version (?)
	}

	auto characterName = msg.getString();
	uint32_t timeStamp = msg.get<uint32_t>();
	uint8_t randNumber = msg.getByte();
	if (challengeTimestamp != timeStamp || challengeRandom != randNumber) {
		disconnect();
		return;
	}

	if (g_game.getGameState() == GAME_STATE_STARTUP) {
		disconnectClient("Gameworld is starting up. Please wait.");
		return;
	}

	if (g_game.getGameState() == GAME_STATE_MAINTAIN) {
		disconnectClient("Gameworld is under maintenance. Please re-connect in a while.");
		return;
	}

	auto ip = getIP();
	if (const auto& banInfo = IOBan::getIpBanInfo(ip)) {
		disconnectClient(std::format("Your IP has been banned until {:s} by {:s}.\n\nReason specified:\n{:s}",
		                             formatDateShort(banInfo->expiresAt), banInfo->bannedBy, banInfo->reason));
		return;
	}

	Database& db = Database::getInstance();
	auto result = db.storeQuery(std::format(
	    "SELECT `a`.`id` AS `account_id`, INET6_NTOA(`s`.`ip`) AS `session_ip`, `p`.`id` AS `character_id` FROM `accounts` `a` JOIN `sessions` `s` ON `a`.`id` = `s`.`account_id` JOIN `players` `p` ON `a`.`id` = `p`.`account_id` WHERE `s`.`token` = {:s} AND `s`.`expired_at` IS NULL AND `p`.`name` = {:s} AND `p`.`deletion` = 0",
	    db.escapeString(sessionToken), db.escapeString(characterName)));
	if (!result) {
		disconnectClient("Account name or password is not correct.");
		return;
	}

	uint32_t accountId = result->getNumber<uint32_t>("account_id");
	if (accountId == 0) {
		disconnectClient("Account name or password is not correct.");
		return;
	}

	Connection::Address sessionIP = boost::asio::ip::make_address(result->getString("session_ip"));
	if (!sessionIP.is_loopback() && ip != sessionIP) {
		disconnectClient("Your game session is already locked to a different IP. Please log in again.");
	}

	g_dispatcher.addTask([=, thisPtr = getThis(), characterId = result->getNumber<uint32_t>("character_id")]() {
		thisPtr->login(characterId, accountId, operatingSystem);
	});
}

void ProtocolGame::onConnect()
{
	auto output = tfs::net::make_output_message();
	static std::random_device rd;
	static std::ranlux24 generator(rd());
	static std::uniform_int_distribution<uint16_t> randNumber(0x00, 0xFF);

	// Skip checksum
	output->skipBytes(sizeof(uint32_t));

	// Packet length & type
	output->add<uint16_t>(0x0006);
	output->addByte(0x1F);

	// Add timestamp & random number
	challengeTimestamp = static_cast<uint32_t>(time(nullptr));
	output->add<uint32_t>(challengeTimestamp);

	challengeRandom = randNumber(generator);
	output->addByte(challengeRandom);

	// Go back and write checksum
	output->skipBytes(-12);
	output->add<uint32_t>(adlerChecksum(output->getOutputBuffer() + sizeof(uint32_t), 8));

	send(output);
}

void ProtocolGame::disconnectClient(const std::string& message) const
{
	auto output = tfs::net::make_output_message();
	output->addByte(0x14);
	output->addString(message);
	send(output);

	disconnect();
}

void ProtocolGame::writeToOutputBuffer(const NetworkMessage& msg)
{
	auto out = getOutputBuffer(msg.getLength());
	out->append(msg);
}

void ProtocolGame::parsePacket(NetworkMessage& msg)
{
	if (!acceptPackets || g_game.getGameState() == GAME_STATE_SHUTDOWN || msg.isEmpty()) {
		return;
	}

	uint8_t recvbyte = msg.getByte();

	if (!player) {
		if (recvbyte == 0x0F) {
			disconnect();
		}

		return;
	}

	// a dead player can not performs actions
	if (player->isRemoved() || player->isDead()) {
		if (recvbyte == 0x0F) {
			disconnect();
			return;
		}

		if (recvbyte != 0x14) {
			return;
		}
	}

	switch (recvbyte) {
		case 0x14:
			g_dispatcher.addTask([thisPtr = getThis()]() { thisPtr->logout(true, false); });
			break;
		case 0x1D:
			g_dispatcher.addTask([playerID = player->getID()]() { g_game.playerReceivePingBack(playerID); });
			break;
		case 0x1E:
			g_dispatcher.addTask([playerID = player->getID()]() { g_game.playerReceivePing(playerID); });
			break;
		// case 0x2A: break; // bestiary tracker
		// case 0x2C: break; // team finder (leader)
		// case 0x2D: break; // team finder (member)
		// case 0x28: break; // stash withdraw
		case 0x32:
			parseExtendedOpcode(msg);
			break; // otclient extended opcode
		case 0x64:
			parseAutoWalk(msg);
			break;
		case 0x65:
			g_dispatcher.addTask([playerID = player->getID()]() { g_game.playerMove(playerID, DIRECTION_NORTH); });
			break;
		case 0x66:
			g_dispatcher.addTask([playerID = player->getID()]() { g_game.playerMove(playerID, DIRECTION_EAST); });
			break;
		case 0x67:
			g_dispatcher.addTask([playerID = player->getID()]() { g_game.playerMove(playerID, DIRECTION_SOUTH); });
			break;
		case 0x68:
			g_dispatcher.addTask([playerID = player->getID()]() { g_game.playerMove(playerID, DIRECTION_WEST); });
			break;
		case 0x69:
			g_dispatcher.addTask([playerID = player->getID()]() { g_game.playerStopAutoWalk(playerID); });
			break;
		case 0x6A:
			g_dispatcher.addTask([playerID = player->getID()]() { g_game.playerMove(playerID, DIRECTION_NORTHEAST); });
			break;
		case 0x6B:
			g_dispatcher.addTask([playerID = player->getID()]() { g_game.playerMove(playerID, DIRECTION_SOUTHEAST); });
			break;
		case 0x6C:
			g_dispatcher.addTask([playerID = player->getID()]() { g_game.playerMove(playerID, DIRECTION_SOUTHWEST); });
			break;
		case 0x6D:
			g_dispatcher.addTask([playerID = player->getID()]() { g_game.playerMove(playerID, DIRECTION_NORTHWEST); });
			break;
		case 0x6F:
			g_dispatcher.addTask(DISPATCHER_TASK_EXPIRATION,
			                     [playerID = player->getID()]() { g_game.playerTurn(playerID, DIRECTION_NORTH); });
			break;
		case 0x70:
			g_dispatcher.addTask(DISPATCHER_TASK_EXPIRATION,
			                     [playerID = player->getID()]() { g_game.playerTurn(playerID, DIRECTION_EAST); });
			break;
		case 0x71:
			g_dispatcher.addTask(DISPATCHER_TASK_EXPIRATION,
			                     [playerID = player->getID()]() { g_game.playerTurn(playerID, DIRECTION_SOUTH); });
			break;
		case 0x72:
			g_dispatcher.addTask(DISPATCHER_TASK_EXPIRATION,
			                     [playerID = player->getID()]() { g_game.playerTurn(playerID, DIRECTION_WEST); });
			break;
		case 0x77:
			parseEquipObject(msg);
			break;
		case 0x78:
			parseThrow(msg);
			break;
		case 0x79:
			parseLookInShop(msg);
			break;
		case 0x7A:
			parsePlayerPurchase(msg);
			break;
		case 0x7B:
			parsePlayerSale(msg);
			break;
		case 0x7C:
			g_dispatcher.addTask([playerID = player->getID()]() { g_game.playerCloseShop(playerID); });
			break;
		case 0x7D:
			parseRequestTrade(msg);
			break;
		case 0x7E:
			parseLookInTrade(msg);
			break;
		case 0x7F:
			g_dispatcher.addTask([playerID = player->getID()]() { g_game.playerAcceptTrade(playerID); });
			break;
		case 0x80:
			g_dispatcher.addTask([playerID = player->getID()]() { g_game.playerCloseTrade(playerID); });
			break;
		case 0x82:
			parseUseItem(msg);
			break;
		case 0x83:
			parseUseItemEx(msg);
			break;
		case 0x84:
			parseUseWithCreature(msg);
			break;
		case 0x85:
			parseRotateItem(msg);
			break;
		case 0x86:
			parseEditPodiumRequest(msg);
			break;
		case 0x87:
			parseCloseContainer(msg);
			break;
		case 0x88:
			parseUpArrowContainer(msg);
			break;
		case 0x89:
			parseTextWindow(msg);
			break;
		case 0x8A:
			parseHouseWindow(msg);
			break;
		case 0x8B:
			parseWrapItem(msg);
			break;
		case 0x8C:
			parseLookAt(msg);
			break;
		case 0x8D:
			parseLookInBattleList(msg);
			break;
		case 0x8E: /* join aggression */
			break;
		// case 0x8F: break; // quick loot
		// case 0x90: break; // loot container
		// case 0x91: break; // update loot whitelist
		// case 0x92: break; // request locker items
		case 0x96:
			parseSay(msg);
			break;
		case 0x97:
			g_dispatcher.addTask([playerID = player->getID()]() { g_game.playerRequestChannels(playerID); });
			break;
		case 0x98:
			parseOpenChannel(msg);
			break;
		case 0x99:
			parseCloseChannel(msg);
			break;
		case 0x9A:
			parseOpenPrivateChannel(msg);
			break;
		case 0x9E:
			g_dispatcher.addTask([playerID = player->getID()]() { g_game.playerCloseNpcChannel(playerID); });
			break;
		case 0xA0:
			parseFightModes(msg);
			break;
		case 0xA1:
			parseAttack(msg);
			break;
		case 0xA2:
			parseFollow(msg);
			break;
		case 0xA3:
			parseInviteToParty(msg);
			break;
		case 0xA4:
			parseJoinParty(msg);
			break;
		case 0xA5:
			parseRevokePartyInvite(msg);
			break;
		case 0xA6:
			parsePassPartyLeadership(msg);
			break;
		case 0xA7:
			g_dispatcher.addTask([playerID = player->getID()]() { g_game.playerLeaveParty(playerID); });
			break;
		case 0xA8:
			parseEnableSharedPartyExperience(msg);
			break;
		case 0xAA:
			g_dispatcher.addTask([playerID = player->getID()]() { g_game.playerCreatePrivateChannel(playerID); });
			break;
		case 0xAB:
			parseChannelInvite(msg);
			break;
		case 0xAC:
			parseChannelExclude(msg);
			break;
		// case 0xB1: break; // request highscores
		case 0xBE:
			g_dispatcher.addTask([playerID = player->getID()]() { g_game.playerCancelAttackAndFollow(playerID); });
			break;
		// case 0xC7: break; // request tournament leaderboard
		case 0xC9: /* update tile */
			break;
		case 0xCA:
			parseUpdateContainer(msg);
			break;
		case 0xCB:
			parseBrowseField(msg);
			break;
		case 0xCC:
			parseSeekInContainer(msg);
			break;
		// case 0xCD: break; // request inspect window
		case 0xD2:
			g_dispatcher.addTask([playerID = player->getID()]() { g_game.playerRequestOutfit(playerID); });
			break;
		case 0xD3:
			parseSetOutfit(msg);
			break;
		// case 0xD5: break; // apply imbuement
		// case 0xD6: break; // clear imbuement
		// case 0xD7: break; // close imbuing window
		case 0xDC:
			parseAddVip(msg);
			break;
		case 0xDD:
			parseRemoveVip(msg);
			break;
		case 0xDE:
			parseEditVip(msg);
			break;
		// case 0xDF: break; // premium shop (?)
		// case 0xE0: break; // premium shop (?)
		// case 0xE4: break; // buy charm rune
		// case 0xE5: break; // request character info (cyclopedia)
		// case 0xE6: break // parse bug report
		case 0xE7: /* thank you */
			break;
		case 0xE8:
			parseDebugAssert(msg);
			break;
		// case 0xEF: break; // request store coins transfer
		case 0xF2:
			parseRuleViolationReport(msg);
			break;
		case 0xF3: /* get object info */
			break;
		case 0xF4:
			parseMarketLeave();
			break;
		case 0xF5:
			parseMarketBrowse(msg);
			break;
		case 0xF6:
			parseMarketCreateOffer(msg);
			break;
		case 0xF7:
			parseMarketCancelOffer(msg);
			break;
		case 0xF8:
			parseMarketAcceptOffer(msg);
			break;
		case 0xF9:
			parseModalWindowAnswer(msg);
			break;
			// case 0xFA: break; // store window open
			// case 0xFB: break; // store window click
			// case 0xFC: break; // store window buy
			// case 0xFD: break; // store window history 1
			// case 0xFE: break; // store window history 2

		default:
			// we cannot pass an unique_ptr as capture here because
			// std::function requires the callable object to be *copyable*
			g_dispatcher.addTask([=, playerID = player->getID(), msg = new NetworkMessage(msg)]() {
				g_game.parsePlayerNetworkMessage(playerID, recvbyte, NetworkMessage_ptr(msg));
			});
			break;
	}

	if (msg.isOverrun()) {
		disconnect();
	}
}

void ProtocolGame::GetTileDescription(const Tile* tile, NetworkMessage& msg)
{
	int32_t count;
	Item* ground = tile->getGround();
	if (ground) {
		msg.addItem(ground);
		count = 1;
	} else {
		count = 0;
	}

	const TileItemVector* items = tile->getItemList();
	if (items) {
		for (auto it = items->getBeginTopItem(), end = items->getEndTopItem(); it != end; ++it) {
			msg.addItem(*it);

			if (++count == MAX_STACKPOS) {
				break;
			}
		}
	}

	const CreatureVector* creatures = tile->getCreatures();
	if (creatures) {
		for (auto it = creatures->rbegin(), end = creatures->rend(); it != end; ++it) {
			const Creature* creature = (*it);
			if (!player->canSeeCreature(creature)) {
				continue;
			}

			bool known;
			uint32_t removedKnown;
			checkCreatureAsKnown(creature->getID(), known, removedKnown);
			AddCreature(msg, creature, known, removedKnown);
			++count;
		}
	}

	if (items && count < MAX_STACKPOS) {
		for (auto it = items->getBeginDownItem(), end = items->getEndDownItem(); it != end; ++it) {
			msg.addItem(*it);

			if (++count == MAX_STACKPOS) {
				return;
			}
		}
	}
}

void ProtocolGame::GetMapDescription(int32_t x, int32_t y, int32_t z, int32_t width, int32_t height,
                                     NetworkMessage& msg)
{
	int32_t skip = -1;
	int32_t startz, endz, zstep;

	if (z > 7) {
		startz = z - 2;
		endz = std::min<int32_t>(MAP_MAX_LAYERS - 1, z + 2);
		zstep = 1;
	} else {
		startz = 7;
		endz = 0;
		zstep = -1;
	}

	for (int32_t nz = startz; nz != endz + zstep; nz += zstep) {
		GetFloorDescription(msg, x, y, nz, width, height, z - nz, skip);
	}

	if (skip >= 0) {
		msg.addByte(skip);
		msg.addByte(0xFF);
	}
}

void ProtocolGame::GetFloorDescription(NetworkMessage& msg, int32_t x, int32_t y, int32_t z, int32_t width,
                                       int32_t height, int32_t offset, int32_t& skip)
{
	for (int32_t nx = 0; nx < width; nx++) {
		for (int32_t ny = 0; ny < height; ny++) {
			Tile* tile = g_game.map.getTile(x + nx + offset, y + ny + offset, z);
			if (tile) {
				if (skip >= 0) {
					msg.addByte(skip);
					msg.addByte(0xFF);
				}

				skip = 0;
				GetTileDescription(tile, msg);
			} else if (skip == 0xFE) {
				msg.addByte(0xFF);
				msg.addByte(0xFF);
				skip = -1;
			} else {
				++skip;
			}
		}
	}
}

void ProtocolGame::checkCreatureAsKnown(uint32_t id, bool& known, uint32_t& removedKnown)
{
	auto result = knownCreatureSet.insert(id);
	if (!result.second) {
		known = true;
		return;
	}

	known = false;

	if (knownCreatureSet.size() > 1300) {
		// Look for a creature to remove
		for (auto it = knownCreatureSet.begin(), end = knownCreatureSet.end(); it != end; ++it) {
			Creature* creature = g_game.getCreatureByID(*it);
			if (!canSee(creature)) {
				removedKnown = *it;
				knownCreatureSet.erase(it);
				return;
			}
		}

		// Bad situation. Let's just remove anyone.
		auto it = knownCreatureSet.begin();
		if (*it == id) {
			++it;
		}

		removedKnown = *it;
		knownCreatureSet.erase(it);
	} else {
		removedKnown = 0;
	}
}

bool ProtocolGame::canSee(const Creature* c) const
{
	if (!c || !player || c->isRemoved()) {
		return false;
	}

	if (!player->canSeeCreature(c)) {
		return false;
	}

	return canSee(c->getPosition());
}

bool ProtocolGame::canSee(const Position& pos) const { return canSee(pos.x, pos.y, pos.z); }

bool ProtocolGame::canSee(int32_t x, int32_t y, int32_t z) const
{
	if (!player) {
		return false;
	}

	const Position& myPos = player->getPosition();
	if (myPos.z <= 7) {
		// we are on ground level or above (7 -> 0) view is from 7 -> 0
		if (z > 7) {
			return false;
		}
	} else { // if (myPos.z >= 8) { we are underground (8 -> 15) view is +/- 2 from the floor we stand on
		if (std::abs(myPos.getZ() - z) > 2) {
			return false;
		}
	}

	// negative offset means that the action taken place is on a lower floor than ourself
	int32_t offsetz = myPos.getZ() - z;
	if ((x >= myPos.getX() - Map::maxClientViewportX + offsetz) &&
	    (x <= myPos.getX() + (Map::maxClientViewportX + 1) + offsetz) &&
	    (y >= myPos.getY() - Map::maxClientViewportY + offsetz) &&
	    (y <= myPos.getY() + (Map::maxClientViewportY + 1) + offsetz)) {
		return true;
	}
	return false;
}

// Parse methods
void ProtocolGame::parseChannelInvite(NetworkMessage& msg)
{
	auto name = msg.getString();
	g_dispatcher.addTask(
	    [playerID = player->getID(), name = std::string{name}]() { g_game.playerChannelInvite(playerID, name); });
}

void ProtocolGame::parseChannelExclude(NetworkMessage& msg)
{
	auto name = msg.getString();
	g_dispatcher.addTask(
	    [=, playerID = player->getID(), name = std::string{name}]() { g_game.playerChannelExclude(playerID, name); });
}

void ProtocolGame::parseOpenChannel(NetworkMessage& msg)
{
	uint16_t channelID = msg.get<uint16_t>();
	g_dispatcher.addTask([=, playerID = player->getID()]() { g_game.playerOpenChannel(playerID, channelID); });
}

void ProtocolGame::parseCloseChannel(NetworkMessage& msg)
{
	uint16_t channelID = msg.get<uint16_t>();
	g_dispatcher.addTask([=, playerID = player->getID()]() { g_game.playerCloseChannel(playerID, channelID); });
}

void ProtocolGame::parseOpenPrivateChannel(NetworkMessage& msg)
{
	auto receiver = msg.getString();
	g_dispatcher.addTask([playerID = player->getID(), receiver = std::string{receiver}]() {
		g_game.playerOpenPrivateChannel(playerID, receiver);
	});
}

void ProtocolGame::parseAutoWalk(NetworkMessage& msg)
{
	uint8_t numdirs = msg.getByte();
	if (numdirs == 0 || (msg.getBufferPosition() + numdirs) != (msg.getLength() + 8)) {
		return;
	}

	msg.skipBytes(numdirs);

	std::vector<Direction> path;
	path.reserve(numdirs);

	for (uint8_t i = 0; i < numdirs; ++i) {
		uint8_t rawdir = msg.getPreviousByte();
		switch (rawdir) {
			case 1:
				path.push_back(DIRECTION_EAST);
				break;
			case 2:
				path.push_back(DIRECTION_NORTHEAST);
				break;
			case 3:
				path.push_back(DIRECTION_NORTH);
				break;
			case 4:
				path.push_back(DIRECTION_NORTHWEST);
				break;
			case 5:
				path.push_back(DIRECTION_WEST);
				break;
			case 6:
				path.push_back(DIRECTION_SOUTHWEST);
				break;
			case 7:
				path.push_back(DIRECTION_SOUTH);
				break;
			case 8:
				path.push_back(DIRECTION_SOUTHEAST);
				break;
			default:
				break;
		}
	}

	if (path.empty()) {
		return;
	}

	g_dispatcher.addTask(
	    [playerID = player->getID(), path = std::move(path)]() { g_game.playerAutoWalk(playerID, path); });
}

void ProtocolGame::parseSetOutfit(NetworkMessage& msg)
{
	uint8_t outfitType = msg.getByte();

	Outfit_t newOutfit;
	newOutfit.lookType = msg.get<uint16_t>();
	newOutfit.lookHead = msg.getByte();
	newOutfit.lookBody = msg.getByte();
	newOutfit.lookLegs = msg.getByte();
	newOutfit.lookFeet = msg.getByte();
	newOutfit.lookAddons = msg.getByte();

	// Set outfit window
	if (outfitType == 0) {
		newOutfit.lookMount = msg.get<uint16_t>();
		if (newOutfit.lookMount != 0) {
			newOutfit.lookMountHead = msg.getByte();
			newOutfit.lookMountBody = msg.getByte();
			newOutfit.lookMountLegs = msg.getByte();
			newOutfit.lookMountFeet = msg.getByte();
		} else {
			msg.skipBytes(4);

			// prevent mount color settings from resetting
			const Outfit_t& currentOutfit = player->getCurrentOutfit();
			newOutfit.lookMountHead = currentOutfit.lookMountHead;
			newOutfit.lookMountBody = currentOutfit.lookMountBody;
			newOutfit.lookMountLegs = currentOutfit.lookMountLegs;
			newOutfit.lookMountFeet = currentOutfit.lookMountFeet;
		}

		msg.get<uint16_t>(); // familiar looktype
		bool randomizeMount = msg.getByte() == 0x01;
		g_dispatcher.addTask(
		    [=, playerID = player->getID()]() { g_game.playerChangeOutfit(playerID, newOutfit, randomizeMount); });

		// Store "try outfit" window
	} else if (outfitType == 1) {
		newOutfit.lookMount = 0;
		// mount colors or store offerId (needs testing)
		newOutfit.lookMountHead = msg.getByte();
		newOutfit.lookMountBody = msg.getByte();
		newOutfit.lookMountLegs = msg.getByte();
		newOutfit.lookMountFeet = msg.getByte();
		// player->? (open store?)

		// Podium interaction
	} else if (outfitType == 2) {
		Position pos = msg.getPosition();
		uint16_t spriteId = msg.get<uint16_t>();
		uint8_t stackpos = msg.getByte();
		newOutfit.lookMount = msg.get<uint16_t>();
		newOutfit.lookMountHead = msg.getByte();
		newOutfit.lookMountBody = msg.getByte();
		newOutfit.lookMountLegs = msg.getByte();
		newOutfit.lookMountFeet = msg.getByte();
		Direction direction = static_cast<Direction>(msg.getByte());
		bool podiumVisible = msg.getByte() == 1;

		// apply to podium
		g_dispatcher.addTask(DISPATCHER_TASK_EXPIRATION, [=, playerID = player->getID()]() {
			g_game.playerEditPodium(playerID, newOutfit, pos, stackpos, spriteId, podiumVisible, direction);
		});
	}
}

void ProtocolGame::parseEditPodiumRequest(NetworkMessage& msg)
{
	Position pos = msg.getPosition();
	uint16_t spriteId = msg.get<uint16_t>();
	uint8_t stackpos = msg.getByte();
	g_dispatcher.addTask(DISPATCHER_TASK_EXPIRATION, [=, playerID = player->getID()]() {
		g_game.playerRequestEditPodium(playerID, pos, stackpos, spriteId);
	});
}

void ProtocolGame::parseUseItem(NetworkMessage& msg)
{
	Position pos = msg.getPosition();
	uint16_t spriteId = msg.get<uint16_t>();
	uint8_t stackpos = msg.getByte();
	uint8_t index = msg.getByte();
	g_dispatcher.addTask(DISPATCHER_TASK_EXPIRATION, [=, playerID = player->getID()]() {
		g_game.playerUseItem(playerID, pos, stackpos, index, spriteId);
	});
}

void ProtocolGame::parseUseItemEx(NetworkMessage& msg)
{
	Position fromPos = msg.getPosition();
	uint16_t fromSpriteId = msg.get<uint16_t>();
	uint8_t fromStackPos = msg.getByte();
	Position toPos = msg.getPosition();
	uint16_t toSpriteId = msg.get<uint16_t>();
	uint8_t toStackPos = msg.getByte();
	g_dispatcher.addTask(DISPATCHER_TASK_EXPIRATION, [=, playerID = player->getID()]() {
		g_game.playerUseItemEx(playerID, fromPos, fromStackPos, fromSpriteId, toPos, toStackPos, toSpriteId);
	});
}

void ProtocolGame::parseUseWithCreature(NetworkMessage& msg)
{
	Position fromPos = msg.getPosition();
	uint16_t spriteId = msg.get<uint16_t>();
	uint8_t fromStackPos = msg.getByte();
	uint32_t creatureId = msg.get<uint32_t>();
	g_dispatcher.addTask(DISPATCHER_TASK_EXPIRATION, [=, playerID = player->getID()]() {
		g_game.playerUseWithCreature(playerID, fromPos, fromStackPos, creatureId, spriteId);
	});
}

void ProtocolGame::parseCloseContainer(NetworkMessage& msg)
{
	uint8_t cid = msg.getByte();
	g_dispatcher.addTask([=, playerID = player->getID()]() { g_game.playerCloseContainer(playerID, cid); });
}

void ProtocolGame::parseUpArrowContainer(NetworkMessage& msg)
{
	uint8_t cid = msg.getByte();
	g_dispatcher.addTask([=, playerID = player->getID()]() { g_game.playerMoveUpContainer(playerID, cid); });
}

void ProtocolGame::parseUpdateContainer(NetworkMessage& msg)
{
	uint8_t cid = msg.getByte();
	g_dispatcher.addTask([=, playerID = player->getID()]() { g_game.playerUpdateContainer(playerID, cid); });
}

void ProtocolGame::parseThrow(NetworkMessage& msg)
{
	Position fromPos = msg.getPosition();
	uint16_t spriteId = msg.get<uint16_t>();
	uint8_t fromStackpos = msg.getByte();
	Position toPos = msg.getPosition();
	uint8_t count = msg.getByte();

	if (toPos != fromPos) {
		g_dispatcher.addTask(DISPATCHER_TASK_EXPIRATION, [=, playerID = player->getID()]() {
			g_game.playerMoveThing(playerID, fromPos, spriteId, fromStackpos, toPos, count);
		});
	}
}

void ProtocolGame::parseLookAt(NetworkMessage& msg)
{
	Position pos = msg.getPosition();
	msg.skipBytes(2); // spriteId
	uint8_t stackpos = msg.getByte();
	g_dispatcher.addTask(DISPATCHER_TASK_EXPIRATION,
	                     [=, playerID = player->getID()]() { g_game.playerLookAt(playerID, pos, stackpos); });
}

void ProtocolGame::parseLookInBattleList(NetworkMessage& msg)
{
	uint32_t creatureID = msg.get<uint32_t>();
	g_dispatcher.addTask(DISPATCHER_TASK_EXPIRATION,
	                     [=, playerID = player->getID()]() { g_game.playerLookInBattleList(playerID, creatureID); });
}

void ProtocolGame::parseSay(NetworkMessage& msg)
{
	std::string receiver;
	uint16_t channelId;

	SpeakClasses type = static_cast<SpeakClasses>(msg.getByte());
	switch (type) {
		case TALKTYPE_PRIVATE_TO:
		case TALKTYPE_PRIVATE_RED_TO:
			receiver = msg.getString();
			channelId = 0;
			break;

		case TALKTYPE_CHANNEL_Y:
		case TALKTYPE_CHANNEL_R1:
			channelId = msg.get<uint16_t>();
			break;

		default:
			channelId = 0;
			break;
	}

	auto text = msg.getString();
	if (text.length() > 255) {
		return;
	}

	g_dispatcher.addTask([=, playerID = player->getID(), receiver = std::string{receiver}, text = std::string{text}]() {
		g_game.playerSay(playerID, channelId, type, receiver, text);
	});
}

void ProtocolGame::parseFightModes(NetworkMessage& msg)
{
	uint8_t rawFightMode = msg.getByte();  // 1 - offensive, 2 - balanced, 3 - defensive
	uint8_t rawChaseMode = msg.getByte();  // 0 - stand while fighting, 1 - chase opponent
	uint8_t rawSecureMode = msg.getByte(); // 0 - can't attack unmarked, 1 - can attack unmarked
	// uint8_t rawPvpMode = msg.getByte(); // pvp mode introduced in 10.0

	fightMode_t fightMode;
	if (rawFightMode == 1) {
		fightMode = FIGHTMODE_ATTACK;
	} else if (rawFightMode == 2) {
		fightMode = FIGHTMODE_BALANCED;
	} else {
		fightMode = FIGHTMODE_DEFENSE;
	}

	g_dispatcher.addTask([=, playerID = player->getID()]() {
		g_game.playerSetFightModes(playerID, fightMode, rawChaseMode != 0, rawSecureMode != 0);
	});
}

void ProtocolGame::parseAttack(NetworkMessage& msg)
{
	uint32_t creatureID = msg.get<uint32_t>();
	// msg.get<uint32_t>(); creatureID (same as above)
	g_dispatcher.addTask([=, playerID = player->getID()]() { g_game.playerSetAttackedCreature(playerID, creatureID); });
}

void ProtocolGame::parseFollow(NetworkMessage& msg)
{
	uint32_t creatureID = msg.get<uint32_t>();
	// msg.get<uint32_t>(); creatureID (same as above)
	g_dispatcher.addTask([=, playerID = player->getID()]() { g_game.playerFollowCreature(playerID, creatureID); });
}

void ProtocolGame::parseEquipObject(NetworkMessage& msg)
{
	// hotkey equip (?)
	uint16_t spriteID = msg.get<uint16_t>();
	// msg.get<uint8_t>(); // bool smartMode (?)

	g_dispatcher.addTask(DISPATCHER_TASK_EXPIRATION,
	                     [=, playerID = player->getID()]() { g_game.playerEquipItem(playerID, spriteID); });
}

void ProtocolGame::parseTextWindow(NetworkMessage& msg)
{
	uint32_t windowTextID = msg.get<uint32_t>();
	auto newText = msg.getString();
	g_dispatcher.addTask([playerID = player->getID(), windowTextID, newText]() {
		g_game.playerWriteItem(playerID, windowTextID, newText);
	});
}

void ProtocolGame::parseHouseWindow(NetworkMessage& msg)
{
	uint8_t doorId = msg.getByte();
	uint32_t id = msg.get<uint32_t>();
	auto text = msg.getString();
	g_dispatcher.addTask([=, playerID = player->getID(), text = std::string{text}]() {
		g_game.playerUpdateHouseWindow(playerID, doorId, id, text);
	});
}

void ProtocolGame::parseWrapItem(NetworkMessage& msg)
{
	Position pos = msg.getPosition();
	uint16_t spriteId = msg.get<uint16_t>();
	uint8_t stackpos = msg.getByte();
	g_dispatcher.addTask(DISPATCHER_TASK_EXPIRATION, [=, playerID = player->getID()]() {
		g_game.playerWrapItem(playerID, pos, stackpos, spriteId);
	});
}

void ProtocolGame::parseLookInShop(NetworkMessage& msg)
{
	uint16_t id = msg.get<uint16_t>();
	uint8_t count = msg.getByte();
	g_dispatcher.addTask(DISPATCHER_TASK_EXPIRATION,
	                     [=, playerID = player->getID()]() { g_game.playerLookInShop(playerID, id, count); });
}

void ProtocolGame::parsePlayerPurchase(NetworkMessage& msg)
{
	uint16_t id = msg.get<uint16_t>();
	uint8_t count = msg.getByte();
	uint16_t amount = msg.get<uint16_t>();
	bool ignoreCap = msg.getByte() != 0;
	bool inBackpacks = msg.getByte() != 0;
	g_dispatcher.addTask(DISPATCHER_TASK_EXPIRATION, [=, playerID = player->getID()]() {
		g_game.playerPurchaseItem(playerID, id, count, amount, ignoreCap, inBackpacks);
	});
}

void ProtocolGame::parsePlayerSale(NetworkMessage& msg)
{
	uint16_t id = msg.get<uint16_t>();
	uint8_t count = msg.getByte();
	uint16_t amount = msg.get<uint16_t>();
	bool ignoreEquipped = msg.getByte() != 0;
	g_dispatcher.addTask(DISPATCHER_TASK_EXPIRATION, [=, playerID = player->getID()]() {
		g_game.playerSellItem(playerID, id, count, amount, ignoreEquipped);
	});
}

void ProtocolGame::parseRequestTrade(NetworkMessage& msg)
{
	Position pos = msg.getPosition();
	uint16_t spriteId = msg.get<uint16_t>();
	uint8_t stackpos = msg.getByte();
	uint32_t playerId = msg.get<uint32_t>();
	g_dispatcher.addTask(
	    [=, playerID = player->getID()]() { g_game.playerRequestTrade(playerID, pos, stackpos, playerId, spriteId); });
}

void ProtocolGame::parseLookInTrade(NetworkMessage& msg)
{
	bool counterOffer = (msg.getByte() == 0x01);
	uint8_t index = msg.getByte();
	g_dispatcher.addTask(DISPATCHER_TASK_EXPIRATION, [=, playerID = player->getID()]() {
		g_game.playerLookInTrade(playerID, counterOffer, index);
	});
}

void ProtocolGame::parseAddVip(NetworkMessage& msg)
{
	auto name = msg.getString();
	g_dispatcher.addTask(
	    [playerID = player->getID(), name = std::string{name}]() { g_game.playerRequestAddVip(playerID, name); });
}

void ProtocolGame::parseRemoveVip(NetworkMessage& msg)
{
	uint32_t guid = msg.get<uint32_t>();
	g_dispatcher.addTask([=, playerID = player->getID()]() { g_game.playerRequestRemoveVip(playerID, guid); });
}

void ProtocolGame::parseEditVip(NetworkMessage& msg)
{
	uint32_t guid = msg.get<uint32_t>();
	auto description = msg.getString();
	uint32_t icon = std::min<uint32_t>(10, msg.get<uint32_t>()); // 10 is max icon in 9.63
	bool notify = msg.getByte() != 0;
	g_dispatcher.addTask([=, playerID = player->getID(), description = std::string{description}]() {
		g_game.playerRequestEditVip(playerID, guid, description, icon, notify);
	});
}

void ProtocolGame::parseRotateItem(NetworkMessage& msg)
{
	Position pos = msg.getPosition();
	uint16_t spriteId = msg.get<uint16_t>();
	uint8_t stackpos = msg.getByte();
	g_dispatcher.addTask(DISPATCHER_TASK_EXPIRATION, [=, playerID = player->getID()]() {
		g_game.playerRotateItem(playerID, pos, stackpos, spriteId);
	});
}

void ProtocolGame::parseRuleViolationReport(NetworkMessage& msg)
{
	uint8_t reportType = msg.getByte();
	uint8_t reportReason = msg.getByte();
	auto targetName = msg.getString();
	auto comment = msg.getString();
	std::string translation;
	if (reportType == REPORT_TYPE_NAME) {
		translation = msg.getString();
	} else if (reportType == REPORT_TYPE_STATEMENT) {
		translation = msg.getString();
		msg.get<uint32_t>(); // statement id, used to get whatever player have said, we don't log that.
	}

	g_dispatcher.addTask([=, playerID = player->getID(), targetName = std::string{targetName},
	                      comment = std::string{comment}, translation = std::string{translation}]() {
		g_game.playerReportRuleViolation(playerID, targetName, reportType, reportReason, comment, translation);
	});
}

void ProtocolGame::parseDebugAssert(NetworkMessage& msg)
{
	if (debugAssertSent) {
		return;
	}

	debugAssertSent = true;

	auto assertLine = msg.getString();
	auto date = msg.getString();
	auto description = msg.getString();
	auto comment = msg.getString();
	g_dispatcher.addTask([playerID = player->getID(), assertLine = std::string{assertLine}, date = std::string{date},
	                      description = std::string{description}, comment = std::string{comment}]() {
		g_game.playerDebugAssert(playerID, assertLine, date, description, comment);
	});
}

void ProtocolGame::parseInviteToParty(NetworkMessage& msg)
{
	uint32_t targetID = msg.get<uint32_t>();
	g_dispatcher.addTask([=, playerID = player->getID()]() { g_game.playerInviteToParty(playerID, targetID); });
}

void ProtocolGame::parseJoinParty(NetworkMessage& msg)
{
	uint32_t targetID = msg.get<uint32_t>();
	g_dispatcher.addTask([=, playerID = player->getID()]() { g_game.playerJoinParty(playerID, targetID); });
}

void ProtocolGame::parseRevokePartyInvite(NetworkMessage& msg)
{
	uint32_t targetID = msg.get<uint32_t>();
	g_dispatcher.addTask([=, playerID = player->getID()]() { g_game.playerRevokePartyInvitation(playerID, targetID); });
}

void ProtocolGame::parsePassPartyLeadership(NetworkMessage& msg)
{
	uint32_t targetID = msg.get<uint32_t>();
	g_dispatcher.addTask([=, playerID = player->getID()]() { g_game.playerPassPartyLeadership(playerID, targetID); });
}

void ProtocolGame::parseEnableSharedPartyExperience(NetworkMessage& msg)
{
	bool sharedExpActive = msg.getByte() == 1;
	g_dispatcher.addTask(
	    [=, playerID = player->getID()]() { g_game.playerEnableSharedPartyExperience(playerID, sharedExpActive); });
}

void ProtocolGame::parseMarketLeave()
{
	g_dispatcher.addTask([playerID = player->getID()]() { g_game.playerLeaveMarket(playerID); });
}

void ProtocolGame::parseMarketBrowse(NetworkMessage& msg)
{
	uint8_t browseId = msg.get<uint8_t>();
	if (browseId == MARKETREQUEST_OWN_OFFERS) {
		g_dispatcher.addTask([playerID = player->getID()]() { g_game.playerBrowseMarketOwnOffers(playerID); });
	} else if (browseId == MARKETREQUEST_OWN_HISTORY) {
		g_dispatcher.addTask([playerID = player->getID()]() { g_game.playerBrowseMarketOwnHistory(playerID); });
	} else {
		uint16_t spriteID = msg.get<uint16_t>();
		g_dispatcher.addTask([=, playerID = player->getID()]() { g_game.playerBrowseMarket(playerID, spriteID); });
	}
}

void ProtocolGame::parseMarketCreateOffer(NetworkMessage& msg)
{
	uint8_t type = msg.getByte();
	uint16_t spriteId = msg.get<uint16_t>();

	const ItemType& it = Item::items.getItemIdByClientId(spriteId);
	if (it.id == 0 || it.wareId == 0) {
		return;
	} else if (it.classification > 0) {
		msg.getByte(); // item tier
	}

	uint16_t amount = msg.get<uint16_t>();
	uint64_t price = msg.get<uint64_t>();
	bool anonymous = (msg.getByte() != 0);
	g_dispatcher.addTask([=, playerID = player->getID()]() {
		g_game.playerCreateMarketOffer(playerID, type, spriteId, amount, price, anonymous);
	});
	sendStoreBalance();
}

void ProtocolGame::parseMarketCancelOffer(NetworkMessage& msg)
{
	uint32_t timestamp = msg.get<uint32_t>();
	uint16_t counter = msg.get<uint16_t>();
	g_dispatcher.addTask(
	    [=, playerID = player->getID()]() { g_game.playerCancelMarketOffer(playerID, timestamp, counter); });
	sendStoreBalance();
}

void ProtocolGame::parseMarketAcceptOffer(NetworkMessage& msg)
{
	uint32_t timestamp = msg.get<uint32_t>();
	uint16_t counter = msg.get<uint16_t>();
	uint16_t amount = msg.get<uint16_t>();
	g_dispatcher.addTask(
	    [=, playerID = player->getID()]() { g_game.playerAcceptMarketOffer(playerID, timestamp, counter, amount); });
}

void ProtocolGame::parseModalWindowAnswer(NetworkMessage& msg)
{
	uint32_t id = msg.get<uint32_t>();
	uint8_t button = msg.getByte();
	uint8_t choice = msg.getByte();
	g_dispatcher.addTask(
	    [=, playerID = player->getID()]() { g_game.playerAnswerModalWindow(playerID, id, button, choice); });
}

void ProtocolGame::parseBrowseField(NetworkMessage& msg)
{
	Position pos = msg.getPosition();
	g_dispatcher.addTask([=, playerID = player->getID()]() { g_game.playerBrowseField(playerID, pos); });
}

void ProtocolGame::parseSeekInContainer(NetworkMessage& msg)
{
	uint8_t containerId = msg.getByte();
	uint16_t index = msg.get<uint16_t>();
	g_dispatcher.addTask(
	    [=, playerID = player->getID()]() { g_game.playerSeekInContainer(playerID, containerId, index); });
}

// Send methods
void ProtocolGame::sendOpenPrivateChannel(const std::string& receiver)
{
	NetworkMessage msg;
	msg.addByte(0xAD);
	msg.addString(receiver);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendChannelEvent(uint16_t channelId, const std::string& playerName, ChannelEvent_t channelEvent)
{
	NetworkMessage msg;
	msg.addByte(0xF3);
	msg.add<uint16_t>(channelId);
	msg.addString(playerName);
	msg.addByte(channelEvent);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendCreatureOutfit(const Creature* creature, const Outfit_t& outfit)
{
	if (!canSee(creature)) {
		return;
	}

	NetworkMessage msg;
	msg.addByte(0x8E);
	msg.add<uint32_t>(creature->getID());
	AddOutfit(msg, outfit);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendCreatureLight(const Creature* creature)
{
	if (!canSee(creature)) {
		return;
	}

	NetworkMessage msg;
	msg.addByte(0x8D);
	msg.add<uint32_t>(creature->getID());

	auto&& [level, color] = creature->getCreatureLight();
	msg.addByte((player->isAccessPlayer() ? 0xFF : level));
	msg.addByte(color);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendCreatureWalkthrough(const Creature* creature, bool walkthrough)
{
	if (!canSee(creature)) {
		return;
	}

	NetworkMessage msg;
	msg.addByte(0x92);
	msg.add<uint32_t>(creature->getID());
	msg.addByte(walkthrough ? 0x00 : 0x01);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendCreatureShield(const Creature* creature)
{
	if (!canSee(creature)) {
		return;
	}

	NetworkMessage msg;
	msg.addByte(0x91);
	msg.add<uint32_t>(creature->getID());
	msg.addByte(player->getPartyShield(creature->getPlayer()));
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendCreatureSkull(const Creature* creature)
{
	if (g_game.getWorldType() != WORLD_TYPE_PVP) {
		return;
	}

	if (!canSee(creature)) {
		return;
	}

	NetworkMessage msg;
	msg.addByte(0x90);
	msg.add<uint32_t>(creature->getID());
	msg.addByte(player->getSkullClient(creature));
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendCreatureSquare(const Creature* creature, SquareColor_t color)
{
	if (!canSee(creature)) {
		return;
	}

	NetworkMessage msg;
	msg.addByte(0x93);
	msg.add<uint32_t>(creature->getID());
	msg.addByte(0x01);
	msg.addByte(color);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendTutorial(uint8_t tutorialId)
{
	NetworkMessage msg;
	msg.addByte(0xDC);
	msg.addByte(tutorialId);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendAddMarker(const Position& pos, uint8_t markType, const std::string& desc)
{
	NetworkMessage msg;
	msg.addByte(0xDD);
	msg.addByte(0x00); // unknown
	msg.addPosition(pos);
	msg.addByte(markType);
	msg.addString(desc);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendReLoginWindow(uint8_t unfairFightReduction)
{
	NetworkMessage msg;
	msg.addByte(0x28);
	msg.addByte(0x00);
	msg.addByte(unfairFightReduction);
	msg.addByte(0x00); // can use death redemption (bool)
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendStats()
{
	NetworkMessage msg;
	AddPlayerStats(msg);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendExperienceTracker(int64_t rawExp, int64_t finalExp)
{
	NetworkMessage msg;
	msg.addByte(0xAF);
	msg.add<int64_t>(rawExp);
	msg.add<int64_t>(finalExp);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendClientFeatures()
{
	NetworkMessage msg;
	msg.addByte(0x17);

	msg.add<uint32_t>(player->getID());
	msg.add<uint16_t>(50); // beat duration

	msg.addDouble(Creature::speedA, 3);
	msg.addDouble(Creature::speedB, 3);
	msg.addDouble(Creature::speedC, 3);

	// can report bugs?
	msg.addByte(player->getAccountType() >= ACCOUNT_TYPE_TUTOR ? 0x01 : 0x00);

	msg.addByte(0x00); // can change pvp framing option
	msg.addByte(0x00); // expert mode button enabled

	msg.add<uint16_t>(0x00); // store images url (string or u16 0x00)
	msg.add<uint16_t>(25);   // premium coin package size

	msg.addByte(0x00); // exiva button enabled (bool)
	msg.addByte(0x00); // Tournament button (bool)

	writeToOutputBuffer(msg);
}

void ProtocolGame::sendBasicData()
{
	NetworkMessage msg;
	msg.addByte(0x9F);
	if (player->isPremium()) {
		msg.addByte(1);
		msg.add<uint32_t>(getBoolean(ConfigManager::FREE_PREMIUM) ? 0 : player->premiumEndsAt);
	} else {
		msg.addByte(0);
		msg.add<uint32_t>(0);
	}

	msg.addByte(player->getVocation()->getClientId());
	msg.addByte(0x00); // is prey system enabled (bool)

	// unlock spells on action bar
	msg.add<uint16_t>(0xFF);
	for (uint8_t spellId = 0x00; spellId < 0xFF; spellId++) {
		msg.add<uint16_t>(spellId);
	}

	msg.addByte(player->getVocation()->getMagicShield()); // is magic shield active (bool)
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendTextMessage(const TextMessage& message)
{
	NetworkMessage msg;
	msg.addByte(0xB4);
	msg.addByte(message.type);
	switch (message.type) {
		case MESSAGE_DAMAGE_DEALT:
		case MESSAGE_DAMAGE_RECEIVED:
		case MESSAGE_DAMAGE_OTHERS: {
			msg.addPosition(message.position);
			msg.add<uint32_t>(message.primary.value);
			msg.addByte(message.primary.color);
			msg.add<uint32_t>(message.secondary.value);
			msg.addByte(message.secondary.color);
			break;
		}
		case MESSAGE_HEALED:
		case MESSAGE_HEALED_OTHERS:
		case MESSAGE_EXPERIENCE:
		case MESSAGE_EXPERIENCE_OTHERS: {
			msg.addPosition(message.position);
			msg.add<uint32_t>(message.primary.value);
			msg.addByte(message.primary.color);
			break;
		}
		case MESSAGE_GUILD:
		case MESSAGE_PARTY_MANAGEMENT:
		case MESSAGE_PARTY:
			msg.add<uint16_t>(message.channelId);
			break;
		default: {
			break;
		}
	}
	msg.addString(message.text);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendClosePrivate(uint16_t channelId)
{
	NetworkMessage msg;
	msg.addByte(0xB3);
	msg.add<uint16_t>(channelId);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendCreatePrivateChannel(uint16_t channelId, const std::string& channelName)
{
	NetworkMessage msg;
	msg.addByte(0xB2);
	msg.add<uint16_t>(channelId);
	msg.addString(channelName);
	msg.add<uint16_t>(0x01);
	msg.addString(player->getName());
	msg.add<uint16_t>(0x00);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendChannelsDialog()
{
	NetworkMessage msg;
	msg.addByte(0xAB);

	const ChannelList& list = g_chat->getChannelList(*player);
	msg.addByte(list.size());
	for (ChatChannel* channel : list) {
		msg.add<uint16_t>(channel->getId());
		msg.addString(channel->getName());
	}

	writeToOutputBuffer(msg);
}

void ProtocolGame::sendChannel(uint16_t channelId, const std::string& channelName, const UsersMap* channelUsers,
                               const InvitedMap* invitedUsers)
{
	NetworkMessage msg;
	msg.addByte(0xAC);

	msg.add<uint16_t>(channelId);
	msg.addString(channelName);

	if (channelUsers) {
		msg.add<uint16_t>(channelUsers->size());
		for (const auto& it : *channelUsers) {
			msg.addString(it.second->getName());
		}
	} else {
		msg.add<uint16_t>(0x00);
	}

	if (invitedUsers) {
		msg.add<uint16_t>(invitedUsers->size());
		for (const auto& it : *invitedUsers) {
			msg.addString(it.second->getName());
		}
	} else {
		msg.add<uint16_t>(0x00);
	}
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendChannelMessage(const std::string& author, const std::string& text, SpeakClasses type,
                                      uint16_t channel)
{
	NetworkMessage msg;
	msg.addByte(0xAA);
	msg.add<uint32_t>(0x00);
	msg.addString(author);
	msg.add<uint16_t>(0x00);
	msg.addByte(type);
	msg.add<uint16_t>(channel);
	msg.addString(text);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendIcons(uint32_t icons)
{
	NetworkMessage msg;
	msg.addByte(0xA2);
	msg.add<uint32_t>(icons);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendContainer(uint8_t cid, const Container* container, uint16_t firstIndex)
{
	NetworkMessage msg;
	msg.addByte(0x6E);

	msg.addByte(cid);

	if (container->getID() == ITEM_BROWSEFIELD) {
		msg.addItem(ITEM_BAG, 1);
		msg.addString("Browse Field");
	} else {
		msg.addItem(container);
		msg.addString(container->getName());
	}

	msg.addByte(container->capacity());
	msg.addByte(container->hasContainerParent() ? 0x01 : 0x00);
	msg.addByte(0x00);                                     // show search icon (boolean)
	msg.addByte(container->isUnlocked() ? 0x01 : 0x00);    // Drag and drop
	msg.addByte(container->hasPagination() ? 0x01 : 0x00); // Pagination

	uint32_t containerSize = container->size();
	msg.add<uint16_t>(containerSize);
	msg.add<uint16_t>(firstIndex);
	if (firstIndex < containerSize) {
		uint8_t itemsToSend = std::min<uint32_t>(std::min<uint32_t>(container->capacity(), containerSize - firstIndex),
		                                         std::numeric_limits<uint8_t>::max());

		msg.addByte(itemsToSend);
		for (auto it = container->getItemList().begin() + firstIndex, end = it + itemsToSend; it != end; ++it) {
			msg.addItem(*it);
		}
	} else {
		msg.addByte(0x00);
	}
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendEmptyContainer(uint8_t cid)
{
	NetworkMessage msg;
	msg.addByte(0x6E);

	msg.addByte(cid);

	msg.addItem(ITEM_BAG, 1);
	msg.addString("Placeholder");

	msg.addByte(8);
	msg.addByte(0x00);
	msg.addByte(0x00);
	msg.addByte(0x01);
	msg.addByte(0x00);
	msg.add<uint16_t>(0);
	msg.add<uint16_t>(0);
	msg.addByte(0x00);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendShop(Npc* npc, const ShopInfoList& itemList)
{
	NetworkMessage msg;
	msg.addByte(0x7A);
	msg.addString(npc->getName());

	// currency displayed in trade window (currently only gold supported) if item other than gold coin is sent, the shop
	// window takes information about currency amount from player items packet (the one that updates action bars)
	msg.add<uint16_t>(Item::items[ITEM_GOLD_COIN].clientId);
	msg.addString(""); // doesn't show anywhere, could be used in otclient for currency name

	uint16_t itemsToSend = std::min<size_t>(itemList.size(), std::numeric_limits<uint16_t>::max());
	msg.add<uint16_t>(itemsToSend);

	uint16_t i = 0;
	for (auto it = itemList.begin(); i < itemsToSend; ++it, ++i) {
		AddShopItem(msg, *it);
	}

	writeToOutputBuffer(msg);
}

void ProtocolGame::sendCloseShop()
{
	NetworkMessage msg;
	msg.addByte(0x7C);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendSaleItemList(const std::list<ShopInfo>& shop)
{
	uint64_t playerBank = player->getBankBalance();
	uint64_t playerMoney = player->getMoney();
	sendResourceBalance(RESOURCE_BANK_BALANCE, playerBank);
	sendResourceBalance(RESOURCE_GOLD_EQUIPPED, playerMoney);

	NetworkMessage msg;
	msg.addByte(0x7B);

	std::map<uint16_t, uint32_t> saleMap;

	if (shop.size() <= 5) {
		// For very small shops it's not worth it to create the complete map
		for (const ShopInfo& shopInfo : shop) {
			if (shopInfo.sellPrice == 0) {
				continue;
			}

			int8_t subtype = -1;

			const ItemType& itemType = Item::items[shopInfo.itemId];
			if (itemType.hasSubType() && !itemType.stackable) {
				subtype = (shopInfo.subType == 0 ? -1 : shopInfo.subType);
			}

			uint32_t count = player->getItemTypeCount(shopInfo.itemId, subtype);
			if (count > 0) {
				saleMap[shopInfo.itemId] = count;
			}
		}
	} else {
		// Large shop, it's better to get a cached map of all item counts and use it We need a temporary map since the
		// finished map should only contain items available in the shop
		std::map<uint32_t, uint32_t> tempSaleMap;
		player->getAllItemTypeCount(tempSaleMap);

		// We must still check manually for the special items that require subtype matches (That is, fluids such as
		// potions etc., actually these items are very few since health potions now use their own ID)
		for (const ShopInfo& shopInfo : shop) {
			if (shopInfo.sellPrice == 0) {
				continue;
			}

			int8_t subtype = -1;

			const ItemType& itemType = Item::items[shopInfo.itemId];
			if (itemType.hasSubType() && !itemType.stackable) {
				subtype = (shopInfo.subType == 0 ? -1 : shopInfo.subType);
			}

			if (subtype != -1) {
				uint32_t count;
				if (itemType.isFluidContainer() || itemType.isSplash()) {
					count = player->getItemTypeCount(shopInfo.itemId,
					                                 subtype); // This shop item requires extra checks
				} else {
					count = subtype;
				}

				if (count > 0) {
					saleMap[shopInfo.itemId] = count;
				}
			} else {
				std::map<uint32_t, uint32_t>::const_iterator findIt = tempSaleMap.find(shopInfo.itemId);
				if (findIt != tempSaleMap.end() && findIt->second > 0) {
					saleMap[shopInfo.itemId] = findIt->second;
				}
			}
		}
	}

	uint8_t itemsToSend = std::min<size_t>(saleMap.size(), std::numeric_limits<uint8_t>::max());
	msg.addByte(itemsToSend);

	uint8_t i = 0;
	for (std::map<uint16_t, uint32_t>::const_iterator it = saleMap.begin(); i < itemsToSend; ++it, ++i) {
		msg.addItemId(it->first);
		msg.add<uint16_t>(std::min<uint16_t>(it->second, std::numeric_limits<uint16_t>::max()));
	}

	writeToOutputBuffer(msg);
}

void ProtocolGame::sendResourceBalance(const ResourceTypes_t resourceType, uint64_t amount)
{
	NetworkMessage msg;
	msg.addByte(0xEE);
	msg.addByte(resourceType);
	msg.add<uint64_t>(amount);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendStoreBalance()
{
	NetworkMessage msg;
	msg.addByte(0xDF);
	msg.addByte(0x01);

	// placeholder packet / to do
	msg.add<uint32_t>(0); // total store coins (transferable + non-t)
	msg.add<uint32_t>(0); // transferable store coins
	msg.add<uint32_t>(0); // reserved auction coins
	msg.add<uint32_t>(0); // tournament coins
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendMarketEnter()
{
	NetworkMessage msg;
	msg.addByte(0xF6);
	msg.addByte(
	    std::min<uint32_t>(tfs::iomarket::getPlayerOfferCount(player->getGUID()), std::numeric_limits<uint8_t>::max()));

	player->setInMarket(true);

	std::map<uint16_t, uint32_t> depotItems;
	std::forward_list<Container*> containerList{player->getInbox().get()};

	for (const auto& chest : player->depotChests) {
		if (!chest.second->empty()) {
			containerList.push_front(chest.second.get());
		}
	}

	do {
		Container* container = containerList.front();
		containerList.pop_front();

		for (Item* item : container->getItemList()) {
			Container* c = item->getContainer();
			if (c && !c->empty()) {
				containerList.push_front(c);
				continue;
			}

			const ItemType& itemType = Item::items[item->getID()];
			if (itemType.wareId == 0) {
				continue;
			}

			if (c && (!itemType.isContainer() || c->capacity() != itemType.maxItems)) {
				continue;
			}

			if (!item->hasMarketAttributes()) {
				continue;
			}

			depotItems[itemType.id] += Item::countByType(item, -1);
		}
	} while (!containerList.empty());

	uint16_t itemsToSend = std::min<size_t>(depotItems.size(), std::numeric_limits<uint16_t>::max());
	uint16_t i = 0;

	msg.add<uint16_t>(itemsToSend);
	for (std::map<uint16_t, uint32_t>::const_iterator it = depotItems.begin(); i < itemsToSend; ++it, ++i) {
		const ItemType& itemType = Item::items[it->first];
		msg.add<uint16_t>(itemType.wareId);
		if (itemType.classification > 0) {
			msg.addByte(0);
		}
		msg.add<uint16_t>(std::min<uint32_t>(0xFFFF, it->second));
	}
	writeToOutputBuffer(msg);

	sendResourceBalance(RESOURCE_BANK_BALANCE, player->getBankBalance());
	sendResourceBalance(RESOURCE_GOLD_EQUIPPED, player->getMoney());
	sendStoreBalance();
}

void ProtocolGame::sendMarketLeave()
{
	NetworkMessage msg;
	msg.addByte(0xF7);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendMarketBrowseItem(uint16_t itemId, const MarketOfferList& buyOffers,
                                        const MarketOfferList& sellOffers)
{
	sendStoreBalance();

	NetworkMessage msg;
	msg.addByte(0xF9);
	msg.addByte(MARKETREQUEST_ITEM);
	msg.addItemId(itemId);

	if (Item::items[itemId].classification > 0) {
		msg.addByte(0); // item tier
	}

	msg.add<uint32_t>(buyOffers.size());
	for (const MarketOffer& offer : buyOffers) {
		msg.add<uint32_t>(offer.timestamp);
		msg.add<uint16_t>(offer.counter);
		msg.add<uint16_t>(offer.amount);
		msg.add<uint64_t>(offer.price);
		msg.addString(offer.playerName);
	}

	msg.add<uint32_t>(sellOffers.size());
	for (const MarketOffer& offer : sellOffers) {
		msg.add<uint32_t>(offer.timestamp);
		msg.add<uint16_t>(offer.counter);
		msg.add<uint16_t>(offer.amount);
		msg.add<uint64_t>(offer.price);
		msg.addString(offer.playerName);
	}

	writeToOutputBuffer(msg);
}

void ProtocolGame::sendMarketAcceptOffer(const MarketOfferEx& offer)
{
	NetworkMessage msg;
	msg.addByte(0xF9);
	msg.addByte(MARKETREQUEST_ITEM);
	msg.addItemId(offer.itemId);
	if (Item::items[offer.itemId].classification > 0) {
		msg.addByte(0);
	}

	if (offer.type == MARKETACTION_BUY) {
		msg.add<uint32_t>(0x01);
		msg.add<uint32_t>(offer.timestamp);
		msg.add<uint16_t>(offer.counter);
		msg.add<uint16_t>(offer.amount);
		msg.add<uint64_t>(offer.price);
		msg.addString(offer.playerName);
		msg.add<uint32_t>(0x00);
	} else {
		msg.add<uint32_t>(0x00);
		msg.add<uint32_t>(0x01);
		msg.add<uint32_t>(offer.timestamp);
		msg.add<uint16_t>(offer.counter);
		msg.add<uint16_t>(offer.amount);
		msg.add<uint64_t>(offer.price);
		msg.addString(offer.playerName);
	}

	writeToOutputBuffer(msg);
}

void ProtocolGame::sendMarketBrowseOwnOffers(const MarketOfferList& buyOffers, const MarketOfferList& sellOffers)
{
	NetworkMessage msg;
	msg.addByte(0xF9);
	msg.addByte(MARKETREQUEST_OWN_OFFERS);

	msg.add<uint32_t>(buyOffers.size());
	for (const MarketOffer& offer : buyOffers) {
		msg.add<uint32_t>(offer.timestamp);
		msg.add<uint16_t>(offer.counter);
		msg.addItemId(offer.itemId);
		if (Item::items[offer.itemId].classification > 0) {
			msg.addByte(0);
		}
		msg.add<uint16_t>(offer.amount);
		msg.add<uint64_t>(offer.price);
	}

	msg.add<uint32_t>(sellOffers.size());
	for (const MarketOffer& offer : sellOffers) {
		msg.add<uint32_t>(offer.timestamp);
		msg.add<uint16_t>(offer.counter);
		msg.addItemId(offer.itemId);
		if (Item::items[offer.itemId].classification > 0) {
			msg.addByte(0);
		}
		msg.add<uint16_t>(offer.amount);
		msg.add<uint64_t>(offer.price);
	}

	writeToOutputBuffer(msg);
}

void ProtocolGame::sendMarketCancelOffer(const MarketOfferEx& offer)
{
	NetworkMessage msg;
	msg.addByte(0xF9);
	msg.addByte(MARKETREQUEST_OWN_OFFERS);

	if (offer.type == MARKETACTION_BUY) {
		msg.add<uint32_t>(0x01);
		msg.add<uint32_t>(offer.timestamp);
		msg.add<uint16_t>(offer.counter);
		msg.addItemId(offer.itemId);
		if (Item::items[offer.itemId].classification > 0) {
			msg.addByte(0);
		}
		msg.add<uint16_t>(offer.amount);
		msg.add<uint64_t>(offer.price);
		msg.add<uint32_t>(0x00);
	} else {
		msg.add<uint32_t>(0x00);
		msg.add<uint32_t>(0x01);
		msg.add<uint32_t>(offer.timestamp);
		msg.add<uint16_t>(offer.counter);
		msg.addItemId(offer.itemId);
		if (Item::items[offer.itemId].classification > 0) {
			msg.addByte(0);
		}
		msg.add<uint16_t>(offer.amount);
		msg.add<uint64_t>(offer.price);
	}

	writeToOutputBuffer(msg);
}

void ProtocolGame::sendMarketBrowseOwnHistory(const HistoryMarketOfferList& buyOffers,
                                              const HistoryMarketOfferList& sellOffers)
{
	uint32_t i = 0;
	std::map<uint32_t, uint16_t> counterMap;
	uint32_t buyOffersToSend =
	    std::min<uint32_t>(buyOffers.size(), 810 + std::max<int32_t>(0, 810 - sellOffers.size()));
	uint32_t sellOffersToSend =
	    std::min<uint32_t>(sellOffers.size(), 810 + std::max<int32_t>(0, 810 - buyOffers.size()));

	NetworkMessage msg;
	msg.addByte(0xF9);
	msg.addByte(MARKETREQUEST_OWN_HISTORY);

	msg.add<uint32_t>(buyOffersToSend);
	for (auto it = buyOffers.begin(); i < buyOffersToSend; ++it, ++i) {
		msg.add<uint32_t>(it->timestamp);
		msg.add<uint16_t>(counterMap[it->timestamp]++);
		msg.addItemId(it->itemId);
		if (Item::items[it->itemId].classification > 0) {
			msg.addByte(0);
		}
		msg.add<uint16_t>(it->amount);
		msg.add<uint64_t>(it->price);
		msg.addByte(it->state);
	}

	counterMap.clear();
	i = 0;

	msg.add<uint32_t>(sellOffersToSend);
	for (auto it = sellOffers.begin(); i < sellOffersToSend; ++it, ++i) {
		msg.add<uint32_t>(it->timestamp);
		msg.add<uint16_t>(counterMap[it->timestamp]++);
		msg.addItemId(it->itemId);
		if (Item::items[it->itemId].classification > 0) {
			msg.addByte(0);
		}
		msg.add<uint16_t>(it->amount);
		msg.add<uint64_t>(it->price);
		msg.addByte(it->state);
	}

	writeToOutputBuffer(msg);
}

void ProtocolGame::sendTradeItemRequest(const std::string& traderName, const Item* item, bool ack)
{
	NetworkMessage msg;

	if (ack) {
		msg.addByte(0x7D);
	} else {
		msg.addByte(0x7E);
	}

	msg.addString(traderName);

	if (const Container* tradeContainer = item->getContainer()) {
		std::list<const Container*> listContainer{tradeContainer};
		std::list<const Item*> itemList{tradeContainer};
		while (!listContainer.empty()) {
			const Container* container = listContainer.front();
			listContainer.pop_front();

			for (Item* containerItem : container->getItemList()) {
				Container* tmpContainer = containerItem->getContainer();
				if (tmpContainer) {
					listContainer.push_back(tmpContainer);
				}
				itemList.push_back(containerItem);
			}
		}

		msg.addByte(itemList.size());
		for (const Item* listItem : itemList) {
			msg.addItem(listItem);
		}
	} else {
		msg.addByte(0x01);
		msg.addItem(item);
	}
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendCloseTrade()
{
	NetworkMessage msg;
	msg.addByte(0x7F);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendCloseContainer(uint8_t cid)
{
	NetworkMessage msg;
	msg.addByte(0x6F);
	msg.addByte(cid);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendCreatureTurn(const Creature* creature, uint32_t stackpos)
{
	if (!canSee(creature)) {
		return;
	}

	NetworkMessage msg;
	msg.addByte(0x6B);
	if (stackpos >= MAX_STACKPOS) {
		msg.add<uint16_t>(0xFFFF);
		msg.add<uint32_t>(creature->getID());
	} else {
		msg.addPosition(creature->getPosition());
		msg.addByte(stackpos);
	}

	msg.add<uint16_t>(0x63);
	msg.add<uint32_t>(creature->getID());
	msg.addByte(creature->getDirection());
	msg.addByte(player->canWalkthroughEx(creature) ? 0x00 : 0x01);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendCreatureSay(const Creature* creature, SpeakClasses type, const std::string& text,
                                   const Position* pos /* = nullptr*/)
{
	NetworkMessage msg;
	msg.addByte(0xAA);

	static uint32_t statementId = 0;
	msg.add<uint32_t>(++statementId);

	msg.addString(creature->getName());
	msg.addByte(0x00); // "(Traded)" suffix after player name

	// Add level only for players
	if (const Player* speaker = creature->getPlayer()) {
		msg.add<uint16_t>(speaker->getLevel());
	} else {
		msg.add<uint16_t>(0x00);
	}

	msg.addByte(type);
	if (pos) {
		msg.addPosition(*pos);
	} else {
		msg.addPosition(creature->getPosition());
	}

	msg.addString(text);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendToChannel(const Creature* creature, SpeakClasses type, const std::string& text,
                                 uint16_t channelId)
{
	NetworkMessage msg;
	msg.addByte(0xAA);

	static uint32_t statementId = 0;
	msg.add<uint32_t>(++statementId);
	if (!creature) {
		msg.add<uint32_t>(0x00);
		msg.addByte(0x00); // "(Traded)" suffix after player name
	} else {
		msg.addString(creature->getName());
		msg.addByte(0x00); // "(Traded)" suffix after player name

		// Add level only for players
		if (const Player* speaker = creature->getPlayer()) {
			msg.add<uint16_t>(speaker->getLevel());
		} else {
			msg.add<uint16_t>(0x00);
		}
	}

	msg.addByte(type);
	msg.add<uint16_t>(channelId);
	msg.addString(text);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendPrivateMessage(const Player* speaker, SpeakClasses type, const std::string& text)
{
	NetworkMessage msg;
	msg.addByte(0xAA);
	static uint32_t statementId = 0;
	msg.add<uint32_t>(++statementId);
	if (speaker) {
		msg.addString(speaker->getName());
		msg.addByte(0x00); // "(Traded)" suffix after player name
		msg.add<uint16_t>(speaker->getLevel());
	} else {
		msg.add<uint32_t>(0x00);
		msg.addByte(0x00); // "(Traded)" suffix after player name
	}
	msg.addByte(type);
	msg.addString(text);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendCancelTarget()
{
	NetworkMessage msg;
	msg.addByte(0xA3);
	msg.add<uint32_t>(0x00);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendChangeSpeed(const Creature* creature, uint32_t speed)
{
	NetworkMessage msg;
	msg.addByte(0x8F);
	msg.add<uint32_t>(creature->getID());
	msg.add<uint16_t>(creature->getBaseSpeed() / 2);
	msg.add<uint16_t>(speed / 2);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendCancelWalk()
{
	NetworkMessage msg;
	msg.addByte(0xB5);
	msg.addByte(player->getDirection());
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendSkills()
{
	NetworkMessage msg;
	AddPlayerSkills(msg);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendPing()
{
	NetworkMessage msg;
	msg.addByte(0x1D);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendPingBack()
{
	NetworkMessage msg;
	msg.addByte(0x1E);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendDistanceShoot(const Position& from, const Position& to, uint8_t type)
{
	NetworkMessage msg;
	msg.addByte(0x83);
	msg.addPosition(from);
	msg.addByte(MAGIC_EFFECTS_CREATE_DISTANCEEFFECT);
	msg.addByte(type);
	msg.addByte(static_cast<uint8_t>(static_cast<int8_t>(static_cast<int32_t>(to.x) - static_cast<int32_t>(from.x))));
	msg.addByte(static_cast<uint8_t>(static_cast<int8_t>(static_cast<int32_t>(to.y) - static_cast<int32_t>(from.y))));
	msg.addByte(MAGIC_EFFECTS_END_LOOP);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendMagicEffect(const Position& pos, uint8_t type)
{
	if (!canSee(pos)) {
		return;
	}

	NetworkMessage msg;
	msg.addByte(0x83);
	msg.addPosition(pos);
	msg.addByte(MAGIC_EFFECTS_CREATE_EFFECT);
	msg.addByte(type);
	msg.addByte(MAGIC_EFFECTS_END_LOOP);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendCreatureHealth(const Creature* creature)
{
	NetworkMessage msg;
	msg.addByte(0x8C);
	msg.add<uint32_t>(creature->getID());

	if (creature->isHealthHidden()) {
		msg.addByte(0x00);
	} else {
		msg.addByte(std::ceil(
		    (static_cast<double>(creature->getHealth()) / std::max<int32_t>(creature->getMaxHealth(), 1)) * 100));
	}
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendFYIBox(const std::string& message)
{
	NetworkMessage msg;
	msg.addByte(0x15);
	msg.addString(message);
	writeToOutputBuffer(msg);
}

// tile
void ProtocolGame::sendMapDescription(const Position& pos)
{
	NetworkMessage msg;
	msg.addByte(0x64);
	msg.addPosition(player->getPosition());
	GetMapDescription(pos.x - Map::maxClientViewportX, pos.y - Map::maxClientViewportY, pos.z,
	                  (Map::maxClientViewportX * 2) + 2, (Map::maxClientViewportY * 2) + 2, msg);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendAddTileItem(const Position& pos, uint32_t stackpos, const Item* item)
{
	if (!canSee(pos)) {
		return;
	}

	NetworkMessage msg;
	msg.addByte(0x6A);
	msg.addPosition(pos);
	msg.addByte(stackpos);
	msg.addItem(item);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendUpdateTileItem(const Position& pos, uint32_t stackpos, const Item* item)
{
	if (!canSee(pos)) {
		return;
	}

	NetworkMessage msg;
	msg.addByte(0x6B);
	msg.addPosition(pos);
	msg.addByte(stackpos);
	msg.addItem(item);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendRemoveTileThing(const Position& pos, uint32_t stackpos)
{
	if (!canSee(pos)) {
		return;
	}

	NetworkMessage msg;
	RemoveTileThing(msg, pos, stackpos);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendUpdateTileCreature(const Position& pos, uint32_t stackpos, const Creature* creature)
{
	if (!canSee(pos)) {
		return;
	}

	NetworkMessage msg;
	msg.addByte(0x6B);
	msg.addPosition(pos);
	msg.addByte(stackpos);

	bool known;
	uint32_t removedKnown;
	checkCreatureAsKnown(creature->getID(), known, removedKnown);
	AddCreature(msg, creature, false, removedKnown);

	writeToOutputBuffer(msg);
}

void ProtocolGame::sendRemoveTileCreature(const Creature* creature, const Position& pos, uint32_t stackpos)
{
	if (stackpos < MAX_STACKPOS) {
		if (!canSee(pos)) {
			return;
		}

		NetworkMessage msg;
		RemoveTileThing(msg, pos, stackpos);
		writeToOutputBuffer(msg);
		return;
	}

	NetworkMessage msg;
	msg.addByte(0x6C);
	msg.add<uint16_t>(0xFFFF);
	msg.add<uint32_t>(creature->getID());
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendUpdateTile(const Tile* tile, const Position& pos)
{
	if (!canSee(pos)) {
		return;
	}

	NetworkMessage msg;
	msg.addByte(0x69);
	msg.addPosition(pos);

	if (tile) {
		GetTileDescription(tile, msg);
		msg.addByte(0x00);
		msg.addByte(0xFF);
	} else {
		msg.addByte(0x01);
		msg.addByte(0xFF);
	}

	writeToOutputBuffer(msg);
}

void ProtocolGame::sendUpdateCreatureIcons(const Creature* creature)
{
	if (!canSee(creature->getPosition())) {
		return;
	}

	NetworkMessage msg;
	msg.addByte(0x8B);
	msg.add<uint32_t>(creature->getID());
	msg.addByte(14); // event player icons

	AddCreatureIcons(msg, creature);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendPendingStateEntered()
{
	NetworkMessage msg;
	msg.addByte(0x0A);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendEnterWorld()
{
	NetworkMessage msg;
	msg.addByte(0x0F);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendFightModes()
{
	NetworkMessage msg;
	msg.addByte(0xA7);
	msg.addByte(player->fightMode);
	msg.addByte(player->chaseMode);
	msg.addByte(player->secureMode);
	msg.addByte(PVP_MODE_DOVE);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendAddCreature(const Creature* creature, const Position& pos, int32_t stackpos,
                                   MagicEffectClasses magicEffect /*= CONST_ME_NONE*/)
{
	if (!canSee(pos)) {
		return;
	}

	if (creature != player) {
		// stack pos is always real index now, so it can exceed the limit if stack pos exceeds the limit, we need to
		// refresh the tile instead
		// 1. this is a rare case, and is only triggered by forcing summon in a position
		// 2. since no stackpos will be send to the client about that creature, removing it must be done with its id if
		// its stackpos remains >= MAX_STACKPOS. this is done to add creatures to battle list instead of rendering on
		// screen
		if (stackpos >= MAX_STACKPOS) {
			// @todo: should we avoid this check?
			if (const Tile* tile = creature->getTile()) {
				sendUpdateTile(tile, pos);
			}
		} else {
			// if stackpos is -1, the client will automatically detect it
			NetworkMessage msg;
			msg.addByte(0x6A);
			msg.addPosition(pos);
			msg.addByte(stackpos);

			bool known;
			uint32_t removedKnown;
			checkCreatureAsKnown(creature->getID(), known, removedKnown);
			AddCreature(msg, creature, known, removedKnown);
			writeToOutputBuffer(msg);
		}

		if (magicEffect != CONST_ME_NONE) {
			sendMagicEffect(pos, magicEffect);
		}
		return;
	}

	// send player stats
	sendStats();         // hp, cap, level, xp rate, etc.
	sendSkills();        // skills and special skills
	player->sendIcons(); // active conditions

	// send client info
	sendClientFeatures(); // player speed, bug reports, store url, pvp mode, etc
	sendBasicData();      // premium account, vocation, known spells, prey system status, magic shield status
	sendItems();          // send carried items for action bars

	// enter world and send game screen
	sendPendingStateEntered();
	sendEnterWorld();
	sendMapDescription(pos);

	// send login effect
	if (magicEffect != CONST_ME_NONE) {
		sendMagicEffect(pos, magicEffect);
	}

	// send equipment
	for (int i = CONST_SLOT_FIRST; i <= CONST_SLOT_LAST; ++i) {
		sendInventoryItem(static_cast<slots_t>(i), player->getInventoryItem(static_cast<slots_t>(i)));
	}

	// send store inbox
	sendInventoryItem(CONST_SLOT_STORE_INBOX, player->getStoreInbox()->getItem());

	// player light level
	sendCreatureLight(creature);

	// player vip list
	sendVIPEntries();

	// tiers for forge and market
	sendItemClasses();

	// opened containers
	player->openSavedContainers();
}

void ProtocolGame::sendMoveCreature(const Creature* creature, const Position& newPos, int32_t newStackPos,
                                    const Position& oldPos, int32_t oldStackPos, bool teleport)
{
	if (creature == player) {
		if (teleport) {
			sendRemoveTileCreature(creature, oldPos, oldStackPos);
			sendMapDescription(newPos);
		} else {
			NetworkMessage msg;
			if (oldPos.z == 7 && newPos.z >= 8) {
				RemoveTileCreature(msg, creature, oldPos, oldStackPos);
			} else {
				msg.addByte(0x6D);
				if (oldStackPos < MAX_STACKPOS) {
					msg.addPosition(oldPos);
					msg.addByte(oldStackPos);
				} else {
					msg.add<uint16_t>(0xFFFF);
					msg.add<uint32_t>(creature->getID());
				}
				msg.addPosition(newPos);
			}

			if (newPos.z > oldPos.z) {
				MoveDownCreature(msg, creature, newPos, oldPos);
			} else if (newPos.z < oldPos.z) {
				MoveUpCreature(msg, creature, newPos, oldPos);
			}

			if (oldPos.y > newPos.y) { // north, for old x
				msg.addByte(0x65);
				GetMapDescription(oldPos.x - Map::maxClientViewportX, newPos.y - Map::maxClientViewportY, newPos.z,
				                  (Map::maxClientViewportX * 2) + 2, 1, msg);
			} else if (oldPos.y < newPos.y) { // south, for old x
				msg.addByte(0x67);
				GetMapDescription(oldPos.x - Map::maxClientViewportX, newPos.y + (Map::maxClientViewportY + 1),
				                  newPos.z, (Map::maxClientViewportX * 2) + 2, 1, msg);
			}

			if (oldPos.x < newPos.x) { // east, [with new y]
				msg.addByte(0x66);
				GetMapDescription(newPos.x + (Map::maxClientViewportX + 1), newPos.y - Map::maxClientViewportY,
				                  newPos.z, 1, (Map::maxClientViewportY * 2) + 2, msg);
			} else if (oldPos.x > newPos.x) { // west, [with new y]
				msg.addByte(0x68);
				GetMapDescription(newPos.x - Map::maxClientViewportX, newPos.y - Map::maxClientViewportY, newPos.z, 1,
				                  (Map::maxClientViewportY * 2) + 2, msg);
			}
			writeToOutputBuffer(msg);
		}
	} else if (canSee(oldPos) && canSee(creature->getPosition())) {
		if (teleport || (oldPos.z == 7 && newPos.z >= 8)) {
			sendRemoveTileCreature(creature, oldPos, oldStackPos);
			sendAddCreature(creature, newPos, newStackPos);
		} else {
			NetworkMessage msg;
			msg.addByte(0x6D);
			if (oldStackPos < MAX_STACKPOS) {
				msg.addPosition(oldPos);
				msg.addByte(oldStackPos);
			} else {
				msg.add<uint16_t>(0xFFFF);
				msg.add<uint32_t>(creature->getID());
			}
			msg.addPosition(creature->getPosition());
			writeToOutputBuffer(msg);
		}
	} else if (canSee(oldPos)) {
		sendRemoveTileCreature(creature, oldPos, oldStackPos);
	} else if (canSee(creature->getPosition())) {
		sendAddCreature(creature, newPos, newStackPos);
	}
}

void ProtocolGame::sendInventoryItem(slots_t slot, const Item* item)
{
	NetworkMessage msg;
	if (item) {
		msg.addByte(0x78);
		msg.addByte(slot);
		msg.addItem(item);
	} else {
		msg.addByte(0x79);
		msg.addByte(slot);
	}
	writeToOutputBuffer(msg);
}

// to do: make it lightweight, update each time player gets/loses an item
void ProtocolGame::sendItems()
{
	NetworkMessage msg;
	msg.addByte(0xF5);

	// find all items carried by character (itemId, amount)
	std::map<uint32_t, uint32_t> inventory;
	player->getAllItemTypeCount(inventory);

	msg.add<uint16_t>(inventory.size() + 11);
	for (uint16_t i = 1; i <= 11; i++) {
		msg.add<uint16_t>(i); // slotId
		msg.addByte(0);       // always 0
		msg.add<uint16_t>(1); // always 1
	}

	for (const auto& item : inventory) {
		msg.add<uint16_t>(Item::items[item.first].clientId); // item clientId
		msg.addByte(0);                                      // always 0
		msg.add<uint16_t>(item.second);                      // count
	}

	writeToOutputBuffer(msg);
}

void ProtocolGame::sendAddContainerItem(uint8_t cid, uint16_t slot, const Item* item)
{
	NetworkMessage msg;
	msg.addByte(0x70);
	msg.addByte(cid);
	msg.add<uint16_t>(slot);
	msg.addItem(item);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendUpdateContainerItem(uint8_t cid, uint16_t slot, const Item* item)
{
	NetworkMessage msg;
	msg.addByte(0x71);
	msg.addByte(cid);
	msg.add<uint16_t>(slot);
	msg.addItem(item);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendRemoveContainerItem(uint8_t cid, uint16_t slot, const Item* lastItem)
{
	NetworkMessage msg;
	msg.addByte(0x72);
	msg.addByte(cid);
	msg.add<uint16_t>(slot);
	if (lastItem) {
		msg.addItem(lastItem);
	} else {
		msg.add<uint16_t>(0x00);
	}
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendTextWindow(uint32_t windowTextId, Item* item, uint16_t maxlen, bool canWrite)
{
	NetworkMessage msg;
	msg.addByte(0x96);
	msg.add<uint32_t>(windowTextId);
	msg.addItem(item);

	if (canWrite) {
		msg.add<uint16_t>(maxlen);
		msg.addString(item->getText());
	} else {
		const std::string& text = item->getText();
		msg.add<uint16_t>(text.size());
		msg.addString(text);
	}

	const std::string& writer = item->getWriter();
	if (!writer.empty()) {
		msg.addString(writer);
	} else {
		msg.add<uint16_t>(0x00);
	}

	msg.addByte(0x00); // "(traded)" suffix after player name (bool)

	time_t writtenDate = item->getDate();
	if (writtenDate != 0) {
		msg.addString(formatDateShort(writtenDate));
	} else {
		msg.add<uint16_t>(0x00);
	}

	writeToOutputBuffer(msg);
}

void ProtocolGame::sendTextWindow(uint32_t windowTextId, uint32_t itemId, const std::string& text)
{
	NetworkMessage msg;
	msg.addByte(0x96);
	msg.add<uint32_t>(windowTextId);
	msg.addItem(itemId, 1);
	msg.add<uint16_t>(text.size());
	msg.addString(text);
	msg.add<uint16_t>(0x00); // writer name
	msg.addByte(0x00);       // "(traded)" byte
	msg.add<uint16_t>(0x00); // date
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendHouseWindow(uint32_t windowTextId, const std::string& text)
{
	NetworkMessage msg;
	msg.addByte(0x97);
	msg.addByte(0x00);
	msg.add<uint32_t>(windowTextId);
	msg.addString(text);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendCombatAnalyzer(CombatType_t type, int32_t amount, DamageAnalyzerImpactType impactType,
                                      const std::string& target)
{
	NetworkMessage msg;
	msg.addByte(0xCC);
	msg.addByte(impactType);
	msg.add<uint32_t>(amount);

	switch (impactType) {
		case RECEIVED:
			msg.addByte(getClientDamageType(type));
			msg.addString(target);
			break;

		case DEALT:
			msg.addByte(getClientDamageType(type));
			break;

		default:
			break;
	}
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendOutfitWindow()
{
	const auto& outfits = Outfits::getInstance().getOutfits(player->getSex());
	if (outfits.size() == 0) {
		return;
	}

	NetworkMessage msg;
	msg.addByte(0xC8);

	Outfit_t currentOutfit = player->getDefaultOutfit();

	if (currentOutfit.lookType == 0) {
		Outfit_t newOutfit;
		newOutfit.lookType = outfits.front().lookType;
		currentOutfit = newOutfit;
	}

	Mount* currentMount = g_game.mounts.getMountByID(player->getCurrentMount());
	if (currentMount) {
		currentOutfit.lookMount = currentMount->clientId;
	}

	bool mounted;
	if (player->wasMounted) {
		mounted = currentOutfit.lookMount != 0;
	} else {
		mounted = player->isMounted();
	}

	AddOutfit(msg, currentOutfit);

	// mount color bytes are required here regardless of having one
	if (currentOutfit.lookMount == 0) {
		msg.addByte(currentOutfit.lookMountHead);
		msg.addByte(currentOutfit.lookMountBody);
		msg.addByte(currentOutfit.lookMountLegs);
		msg.addByte(currentOutfit.lookMountFeet);
	}

	msg.add<uint16_t>(0); // current familiar looktype

	std::vector<ProtocolOutfit> protocolOutfits;
	if (player->isAccessPlayer()) {
		protocolOutfits.emplace_back("Gamemaster", 75, 0);
	}

	for (const Outfit& outfit : outfits) {
		uint8_t addons;
		if (!player->getOutfitAddons(outfit, addons)) {
			continue;
		}

		protocolOutfits.emplace_back(outfit.name, outfit.lookType, addons);
	}

	msg.add<uint16_t>(protocolOutfits.size());
	for (const ProtocolOutfit& outfit : protocolOutfits) {
		msg.add<uint16_t>(outfit.lookType);
		msg.addString(outfit.name);
		msg.addByte(outfit.addons);
		msg.addByte(0x00); // mode: 0x00 - available, 0x01 store (requires U32 store offerId), 0x02 golden outfit
		                   // tooltip (hardcoded)
	}

	std::vector<const Mount*> mounts;
	for (const Mount& mount : g_game.mounts.getMounts()) {
		if (player->hasMount(&mount)) {
			mounts.push_back(&mount);
		}
	}

	msg.add<uint16_t>(mounts.size());
	for (const Mount* mount : mounts) {
		msg.add<uint16_t>(mount->clientId);
		msg.addString(mount->name);
		msg.addByte(0x00); // mode: 0x00 - available, 0x01 store (requires U32 store offerId)
	}

	msg.add<uint16_t>(0x00); // familiars.size()
	// size > 0
	// U16 looktype
	// String name
	// 0x00 // mode: 0x00 - available, 0x01 store (requires U32 store offerId)

	msg.addByte(0x00); // Try outfit mode (?)
	msg.addByte(mounted ? 0x01 : 0x00);
	msg.addByte(player->randomizeMount ? 0x01 : 0x00);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendPodiumWindow(const Item* item)
{
	if (!item) {
		return;
	}

	const Podium* podium = item->getPodium();
	if (!podium) {
		return;
	}

	const Tile* tile = item->getTile();
	if (!tile) {
		return;
	}

	int32_t stackpos = tile->getThingIndex(item);

	// read podium outfit
	Outfit_t podiumOutfit = podium->getOutfit();
	Outfit_t playerOutfit = player->getDefaultOutfit();
	bool isEmpty = podiumOutfit.lookType == 0 && podiumOutfit.lookMount == 0;

	if (podiumOutfit.lookType == 0) {
		// copy player outfit
		podiumOutfit.lookType = playerOutfit.lookType;
		podiumOutfit.lookHead = playerOutfit.lookHead;
		podiumOutfit.lookBody = playerOutfit.lookBody;
		podiumOutfit.lookLegs = playerOutfit.lookLegs;
		podiumOutfit.lookFeet = playerOutfit.lookFeet;
		podiumOutfit.lookAddons = playerOutfit.lookAddons;
	}

	if (podiumOutfit.lookMount == 0) {
		// copy player mount
		podiumOutfit.lookMount = playerOutfit.lookMount;
		podiumOutfit.lookMountHead = playerOutfit.lookMountHead;
		podiumOutfit.lookMountBody = playerOutfit.lookMountBody;
		podiumOutfit.lookMountLegs = playerOutfit.lookMountLegs;
		podiumOutfit.lookMountFeet = playerOutfit.lookMountFeet;
	}

	// fetch player outfits
	const auto& outfits = Outfits::getInstance().getOutfits(player->getSex());
	if (outfits.size() == 0) {
		player->sendCancelMessage(RETURNVALUE_NOTPOSSIBLE);
		return;
	}

	// add GM outfit for staff members
	std::vector<ProtocolOutfit> protocolOutfits;
	if (player->isAccessPlayer()) {
		protocolOutfits.emplace_back("Gamemaster", 75, 0);
	}

	// fetch player addons info
	for (const Outfit& outfit : outfits) {
		uint8_t addons;
		if (!player->getOutfitAddons(outfit, addons)) {
			continue;
		}

		protocolOutfits.emplace_back(outfit.name, outfit.lookType, addons);
	}

	// select first outfit available when the one from podium is not unlocked
	if (!player->canWear(podiumOutfit.lookType, 0)) {
		podiumOutfit.lookType = outfits.front().lookType;
	}

	// fetch player mounts
	std::vector<const Mount*> mounts;
	for (const Mount& mount : g_game.mounts.getMounts()) {
		if (player->hasMount(&mount)) {
			mounts.push_back(&mount);
		}
	}

	// packet header
	NetworkMessage msg;
	msg.addByte(0xC8);

	// current outfit
	msg.add<uint16_t>(podiumOutfit.lookType);
	msg.addByte(podiumOutfit.lookHead);
	msg.addByte(podiumOutfit.lookBody);
	msg.addByte(podiumOutfit.lookLegs);
	msg.addByte(podiumOutfit.lookFeet);
	msg.addByte(podiumOutfit.lookAddons);

	// current mount
	msg.add<uint16_t>(podiumOutfit.lookMount);
	msg.addByte(podiumOutfit.lookMountHead);
	msg.addByte(podiumOutfit.lookMountBody);
	msg.addByte(podiumOutfit.lookMountLegs);
	msg.addByte(podiumOutfit.lookMountFeet);

	// current familiar (not used in podium mode)
	msg.add<uint16_t>(0);

	// available outfits
	msg.add<uint16_t>(protocolOutfits.size());
	for (const ProtocolOutfit& outfit : protocolOutfits) {
		msg.add<uint16_t>(outfit.lookType);
		msg.addString(outfit.name);
		msg.addByte(outfit.addons);
		msg.addByte(0x00); // mode: 0x00 - available, 0x01 store (requires U32 store offerId), 0x02 golden outfit
		                   // tooltip (hardcoded)
	}

	// available mounts
	msg.add<uint16_t>(mounts.size());
	for (const Mount* mount : mounts) {
		msg.add<uint16_t>(mount->clientId);
		msg.addString(mount->name);
		msg.addByte(0x00); // mode: 0x00 - available, 0x01 store (requires U32 store offerId)
	}

	// available familiars (not used in podium mode)
	msg.add<uint16_t>(0);

	msg.addByte(0x05); // "set outfit" window mode (5 = podium)
	msg.addByte((isEmpty && playerOutfit.lookMount != 0) || podium->hasFlag(PODIUM_SHOW_MOUNT)
	                ? 0x01
	                : 0x00); // "mount" checkbox
	msg.add<uint16_t>(0);    // unknown
	msg.addPosition(item->getPosition());
	msg.add<uint16_t>(item->getClientID());
	msg.addByte(stackpos);

	msg.addByte(podium->hasFlag(PODIUM_SHOW_PLATFORM) ? 0x01 : 0x00); // is platform visible
	msg.addByte(0x01);                                                // "outfit" checkbox, ignored by the client
	msg.addByte(podium->getDirection());                              // outfit direction
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendUpdatedVIPStatus(uint32_t guid, VipStatus_t newStatus)
{
	NetworkMessage msg;
	msg.addByte(0xD3);
	msg.add<uint32_t>(guid);
	msg.addByte(newStatus);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendVIP(uint32_t guid, const std::string& name, const std::string& description, uint32_t icon,
                           bool notify, VipStatus_t status)
{
	NetworkMessage msg;
	msg.addByte(0xD2);
	msg.add<uint32_t>(guid);
	msg.addString(name);
	msg.addString(description);
	msg.add<uint32_t>(std::min<uint32_t>(10, icon));
	msg.addByte(notify ? 0x01 : 0x00);
	msg.addByte(status);
	msg.addByte(0x00); // vipGroups (placeholder)
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendVIPEntries()
{
	const std::forward_list<VIPEntry>& vipEntries = IOLoginData::getVIPEntries(player->getAccount());

	for (const VIPEntry& entry : vipEntries) {
		VipStatus_t vipStatus = VIPSTATUS_ONLINE;

		Player* vipPlayer = g_game.getPlayerByGUID(entry.guid);

		if (!vipPlayer || !player->canSeeCreature(vipPlayer)) {
			vipStatus = VIPSTATUS_OFFLINE;
		}

		sendVIP(entry.guid, entry.name, entry.description, entry.icon, entry.notify, vipStatus);
	}
}

void ProtocolGame::sendItemClasses()
{
	NetworkMessage msg;
	msg.addByte(0x86);

	uint8_t classSize = 4;
	uint8_t tiersSize = 10;

	// item classes
	msg.addByte(classSize);
	for (uint8_t i = 0; i < classSize; i++) {
		msg.addByte(i + 1); // class id

		// item tiers
		msg.addByte(tiersSize); // tiers size
		for (uint8_t j = 0; j < tiersSize; j++) {
			msg.addByte(j);           // tier id
			msg.add<uint64_t>(10000); // upgrade cost
		}
	}

	// unknown
	for (uint8_t i = 0; i < tiersSize + 1; i++) {
		msg.addByte(0);
	}

	writeToOutputBuffer(msg);
}

void ProtocolGame::sendSpellCooldown(uint8_t spellId, uint32_t time)
{
	NetworkMessage msg;
	msg.addByte(0xA4);
	msg.add<uint16_t>(static_cast<uint16_t>(spellId));
	msg.add<uint32_t>(time);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendSpellGroupCooldown(SpellGroup_t groupId, uint32_t time)
{
	NetworkMessage msg;
	msg.addByte(0xA5);
	msg.addByte(groupId);
	msg.add<uint32_t>(time);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendUseItemCooldown(uint32_t time)
{
	NetworkMessage msg;
	msg.addByte(0xA6);
	msg.add<uint32_t>(time);
	writeToOutputBuffer(msg);
}

void ProtocolGame::sendSupplyUsed(const uint16_t clientId)
{
	NetworkMessage msg;
	msg.addByte(0xCE);
	msg.add<uint16_t>(clientId);

	writeToOutputBuffer(msg);
}

void ProtocolGame::sendModalWindow(const ModalWindow& modalWindow)
{
	NetworkMessage msg;
	msg.addByte(0xFA);

	msg.add<uint32_t>(modalWindow.id);
	msg.addString(modalWindow.title);
	msg.addString(modalWindow.message);

	msg.addByte(modalWindow.buttons.size());
	for (const auto& it : modalWindow.buttons) {
		msg.addString(it.first);
		msg.addByte(it.second);
	}

	msg.addByte(modalWindow.choices.size());
	for (const auto& it : modalWindow.choices) {
		msg.addString(it.first);
		msg.addByte(it.second);
	}

	msg.addByte(modalWindow.defaultEscapeButton);
	msg.addByte(modalWindow.defaultEnterButton);
	msg.addByte(modalWindow.priority ? 0x01 : 0x00);

	writeToOutputBuffer(msg);
}

void ProtocolGame::sendSessionEnd(SessionEndTypes_t reason)
{
	auto output = tfs::net::make_output_message();
	output->addByte(0x18);
	output->addByte(reason);
	send(output);
}

////////////// Add common messages
void ProtocolGame::AddCreature(NetworkMessage& msg, const Creature* creature, bool known, uint32_t remove)
{
	CreatureType_t creatureType = creature->getType();
	const Player* otherPlayer = creature->getPlayer();
	const Player* masterPlayer = nullptr;
	uint32_t masterId = 0;

	if (creatureType == CREATURETYPE_MONSTER) {
		const Creature* master = creature->getMaster();
		if (master) {
			masterPlayer = master->getPlayer();
			if (masterPlayer) {
				masterId = master->getID();
				creatureType = CREATURETYPE_SUMMON_OWN;
			}
		}
	}

	if (known) {
		msg.add<uint16_t>(0x62);
		msg.add<uint32_t>(creature->getID());
	} else {
		msg.add<uint16_t>(0x61);
		msg.add<uint32_t>(remove);
		msg.add<uint32_t>(creature->getID());
		msg.addByte(creature->isHealthHidden() ? CREATURETYPE_HIDDEN : creatureType);

		if (creatureType == CREATURETYPE_SUMMON_OWN) {
			msg.add<uint32_t>(masterId);
		}

		msg.addString(creature->isHealthHidden() ? "" : creature->getName());
	}

	if (creature->isHealthHidden()) {
		msg.addByte(0x00);
	} else {
		msg.addByte(std::ceil(
		    (static_cast<double>(creature->getHealth()) / std::max<int32_t>(creature->getMaxHealth(), 1)) * 100));
	}

	msg.addByte(creature->getDirection());

	if (!creature->isInGhostMode() && !creature->isInvisible()) {
		const Outfit_t& outfit = creature->getCurrentOutfit();
		AddOutfit(msg, outfit);
	} else {
		static Outfit_t outfit;
		AddOutfit(msg, outfit);
	}

	LightInfo lightInfo = creature->getCreatureLight();
	msg.addByte(player->isAccessPlayer() ? 0xFF : lightInfo.level);
	msg.addByte(lightInfo.color);

	msg.add<uint16_t>(creature->getStepSpeed() / 2);

	AddCreatureIcons(msg, creature);

	msg.addByte(player->getSkullClient(creature));
	msg.addByte(player->getPartyShield(otherPlayer));

	if (!known) {
		msg.addByte(player->getGuildEmblem(otherPlayer));
	}

	// Creature type and summon emblem
	msg.addByte(creature->isHealthHidden() ? CREATURETYPE_HIDDEN : creatureType);
	if (creatureType == CREATURETYPE_SUMMON_OWN) {
		msg.add<uint32_t>(masterId);
	}

	// Player vocation info
	if (creatureType == CREATURETYPE_PLAYER) {
		msg.addByte(otherPlayer ? otherPlayer->getVocation()->getClientId() : 0x00);
	}

	msg.addByte(creature->getSpeechBubble());
	msg.addByte(0xFF); // MARK_UNMARKED
	msg.addByte(0x00); // inspection type (bool?)

	msg.addByte(player->canWalkthroughEx(creature) ? 0x00 : 0x01);
}

void ProtocolGame::AddCreatureIcons(NetworkMessage& msg, const Creature* creature)
{
	const auto& creatureIcons = creature->getIcons();
	if (const Monster* monster = creature->getMonster()) {
		const auto& monsterIcons = monster->getSpecialIcons();
		msg.addByte(creatureIcons.size() + monsterIcons.size());
		for (const auto& [iconId, level] : monsterIcons) {
			msg.addByte(iconId);
			msg.addByte(1);
			msg.add<uint16_t>(level);
		}
	} else {
		msg.addByte(creatureIcons.size());
	}

	for (const auto& [iconId, level] : creatureIcons) {
		msg.addByte(iconId);
		msg.addByte(0);
		msg.add<uint16_t>(level);
	}
}

void ProtocolGame::AddPlayerStats(NetworkMessage& msg)
{
	msg.addByte(0xA0);

	msg.add<uint32_t>(static_cast<uint32_t>(player->getHealth()));
	msg.add<uint32_t>(static_cast<uint32_t>(player->getMaxHealth()));

	msg.add<uint32_t>(player->hasFlag(PlayerFlag_HasInfiniteCapacity) ? 1000000 : player->getFreeCapacity());
	msg.add<uint64_t>(player->getExperience());

	msg.add<uint16_t>(player->getLevel());
	msg.addByte(player->getLevelPercent());

	msg.add<uint16_t>(player->getClientExpDisplay());
	msg.add<uint16_t>(player->getClientLowLevelBonusDisplay());
	msg.add<uint16_t>(0); // store exp bonus
	msg.add<uint16_t>(player->getClientStaminaBonusDisplay());

	msg.add<uint32_t>(static_cast<uint32_t>(player->getMana()));
	msg.add<uint32_t>(static_cast<uint32_t>(player->getMaxMana()));

	msg.addByte(player->getSoul());
	msg.add<uint16_t>(player->getStaminaMinutes());
	msg.add<uint16_t>(player->getBaseSpeed() / 2);

	Condition* condition = player->getCondition(CONDITION_REGENERATION, CONDITIONID_DEFAULT);
	msg.add<uint16_t>(condition ? condition->getTicks() / 1000 : 0x00);

	msg.add<uint16_t>(player->getOfflineTrainingTime() / 60 / 1000);

	msg.add<uint16_t>(0); // xp boost time (seconds)
	msg.addByte(0x00);    // enables exp boost in the store

	if (ConditionManaShield* conditionManaShield =
	        dynamic_cast<ConditionManaShield*>(player->getCondition(CONDITION_MANASHIELD_BREAKABLE))) {
		msg.add<uint32_t>(conditionManaShield->getManaShield());
		msg.add<uint32_t>(conditionManaShield->getMaxManaShield());
	} else {
		msg.add<uint32_t>(0); // remaining mana shield
		msg.add<uint32_t>(0); // total mana shield
	}
}

void ProtocolGame::AddPlayerSkills(NetworkMessage& msg)
{
	msg.addByte(0xA1);
	msg.add<uint16_t>(player->getMagicLevel());
	msg.add<uint16_t>(player->getBaseMagicLevel());
	msg.add<uint16_t>(player->getBaseMagicLevel()); // base + loyalty bonus(?)
	msg.add<uint16_t>(player->getMagicLevelPercent());

	for (uint8_t i = SKILL_FIRST; i <= SKILL_LAST; ++i) {
		msg.add<uint16_t>(std::min<int32_t>(player->getSkillLevel(i), std::numeric_limits<uint16_t>::max()));
		msg.add<uint16_t>(player->getBaseSkill(i));
		msg.add<uint16_t>(player->getBaseSkill(i)); // base + loyalty bonus(?)
		msg.add<uint16_t>(player->getSkillPercent(i));
	}

	for (uint8_t i = SPECIALSKILL_FIRST; i <= SPECIALSKILL_LAST; ++i) {
		msg.add<uint16_t>(std::min<int32_t>(10000, player->varSpecialSkills[i])); // base + bonus special skill
		msg.add<uint16_t>(0);                                                     // base special skill
	}

	msg.addByte(0); // element magic level
	// structure:
	// u8 client element id
	// u16 bonus element ml

	// fatal, dodge, momentum
	for (int i = 0; i < 3; ++i) {
		msg.add<uint16_t>(0);
		msg.add<uint16_t>(0);
	}

	// to do: bonus cap
	uint32_t capacityValue = player->hasFlag(PlayerFlag_HasInfiniteCapacity) ? 1000000 : player->getCapacity();
	msg.add<uint32_t>(capacityValue); // base + bonus capacity
	msg.add<uint32_t>(capacityValue); // base capacity
}

void ProtocolGame::AddOutfit(NetworkMessage& msg, const Outfit_t& outfit)
{
	// outfit
	msg.add<uint16_t>(outfit.lookType);
	if (outfit.lookType != 0) {
		msg.addByte(outfit.lookHead);
		msg.addByte(outfit.lookBody);
		msg.addByte(outfit.lookLegs);
		msg.addByte(outfit.lookFeet);
		msg.addByte(outfit.lookAddons);
	} else {
		msg.addItemId(outfit.lookTypeEx);
	}

	// mount
	msg.add<uint16_t>(outfit.lookMount);
	if (outfit.lookMount != 0) {
		msg.addByte(outfit.lookMountHead);
		msg.addByte(outfit.lookMountBody);
		msg.addByte(outfit.lookMountLegs);
		msg.addByte(outfit.lookMountFeet);
	}
}

// tile
void ProtocolGame::RemoveTileThing(NetworkMessage& msg, const Position& pos, uint32_t stackpos)
{
	if (stackpos >= MAX_STACKPOS) {
		return;
	}

	msg.addByte(0x6C);
	msg.addPosition(pos);
	msg.addByte(stackpos);
}

void ProtocolGame::RemoveTileCreature(NetworkMessage& msg, const Creature* creature, const Position& pos,
                                      uint32_t stackpos)
{
	if (stackpos < MAX_STACKPOS) {
		RemoveTileThing(msg, pos, stackpos);
		return;
	}

	msg.addByte(0x6C);
	msg.add<uint16_t>(0xFFFF);
	msg.add<uint32_t>(creature->getID());
}

void ProtocolGame::MoveUpCreature(NetworkMessage& msg, const Creature* creature, const Position& newPos,
                                  const Position& oldPos)
{
	if (creature != player) {
		return;
	}

	// floor change up
	msg.addByte(0xBE);

	// going to surface
	if (newPos.z == 7) {
		int32_t skip = -1;

		// floor 7 and 6 already set
		for (int i = 5; i >= 0; --i) {
			GetFloorDescription(msg, oldPos.x - Map::maxClientViewportX, oldPos.y - Map::maxClientViewportY, i,
			                    (Map::maxClientViewportX * 2) + 2, (Map::maxClientViewportY * 2) + 2, 8 - i, skip);
		}
		if (skip >= 0) {
			msg.addByte(skip);
			msg.addByte(0xFF);
		}
	}
	// underground, going one floor up (still underground)
	else if (newPos.z > 7) {
		int32_t skip = -1;
		GetFloorDescription(msg, oldPos.x - Map::maxClientViewportX, oldPos.y - Map::maxClientViewportY,
		                    oldPos.getZ() - 3, (Map::maxClientViewportX * 2) + 2, (Map::maxClientViewportY * 2) + 2, 3,
		                    skip);

		if (skip >= 0) {
			msg.addByte(skip);
			msg.addByte(0xFF);
		}
	}

	// moving up a floor up makes us out of sync
	// west
	msg.addByte(0x68);
	GetMapDescription(oldPos.x - Map::maxClientViewportX, oldPos.y - (Map::maxClientViewportY - 1), newPos.z, 1,
	                  (Map::maxClientViewportY * 2) + 2, msg);

	// north
	msg.addByte(0x65);
	GetMapDescription(oldPos.x - Map::maxClientViewportX, oldPos.y - Map::maxClientViewportY, newPos.z,
	                  (Map::maxClientViewportX * 2) + 2, 1, msg);
}

void ProtocolGame::MoveDownCreature(NetworkMessage& msg, const Creature* creature, const Position& newPos,
                                    const Position& oldPos)
{
	if (creature != player) {
		return;
	}

	// floor change down
	msg.addByte(0xBF);

	// going from surface to underground
	if (newPos.z == 8) {
		int32_t skip = -1;

		for (int i = 0; i < 3; ++i) {
			GetFloorDescription(msg, oldPos.x - Map::maxClientViewportX, oldPos.y - Map::maxClientViewportY,
			                    newPos.z + i, (Map::maxClientViewportX * 2) + 2, (Map::maxClientViewportY * 2) + 2,
			                    -i - 1, skip);
		}
		if (skip >= 0) {
			msg.addByte(skip);
			msg.addByte(0xFF);
		}
	}
	// going further down
	else if (newPos.z > oldPos.z && newPos.z > 8 && newPos.z < 14) {
		int32_t skip = -1;
		GetFloorDescription(msg, oldPos.x - Map::maxClientViewportX, oldPos.y - Map::maxClientViewportY, newPos.z + 2,
		                    (Map::maxClientViewportX * 2) + 2, (Map::maxClientViewportY * 2) + 2, -3, skip);

		if (skip >= 0) {
			msg.addByte(skip);
			msg.addByte(0xFF);
		}
	}

	// moving down a floor makes us out of sync
	// east
	msg.addByte(0x66);
	GetMapDescription(oldPos.x + (Map::maxClientViewportX + 1), oldPos.y - (Map::maxClientViewportY + 1), newPos.z, 1,
	                  (Map::maxClientViewportY * 2) + 2, msg);

	// south
	msg.addByte(0x67);
	GetMapDescription(oldPos.x - Map::maxClientViewportX, oldPos.y + (Map::maxClientViewportY + 1), newPos.z,
	                  (Map::maxClientViewportX * 2) + 2, 1, msg);
}

void ProtocolGame::AddShopItem(NetworkMessage& msg, const ShopInfo& item)
{
	const ItemType& it = Item::items[item.itemId];
	msg.add<uint16_t>(it.clientId);

	if (it.isSplash() || it.isFluidContainer()) {
		msg.addByte(serverFluidToClient(item.subType));
	} else {
		msg.addByte(0x00);
	}

	msg.addString(item.realName);
	msg.add<uint32_t>(it.weight);
	msg.add<uint32_t>(std::max<uint32_t>(item.buyPrice, 0));
	msg.add<uint32_t>(std::max<uint32_t>(item.sellPrice, 0));
}

void ProtocolGame::parseExtendedOpcode(NetworkMessage& msg)
{
	uint8_t opcode = msg.getByte();
	auto buffer = msg.getString();

	// process additional opcodes via lua script event
	g_dispatcher.addTask([=, playerID = player->getID(), buffer = std::string{buffer}]() {
		g_game.parsePlayerExtendedOpcode(playerID, opcode, buffer);
	});
}
