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

#include "otpch.h"

#include <boost/range/adaptor/reversed.hpp>

#include "protocolspectator.h"

#include "outputmessage.h"

#include "player.h"

#include "configmanager.h"
#include "actions.h"
#include "game.h"
#include "iologindata.h"
#include "iomarket.h"
#include "waitlist.h"
#include "ban.h"
#include "scheduler.h"
#include "spells.h"

extern ConfigManager g_config;
extern Actions actions;
extern CreatureEvents* g_creatureEvents;
extern Chat* g_chat;
extern Spells* g_spells;

void ProtocolSpectator::release()
{
	if (spectator && spectator->client == shared_from_this()) {
		if(player->isLiveCasting()) {
			std::stringstream ss;
			ss << spectator->getName() << " has left the cast.";
			player->sendChannelMessage("", ss.str(), TALKTYPE_CHANNEL_O, CHANNEL_CAST);
		}
		spectator->client.reset();
		spectator = nullptr;
	}
	OutputMessagePool::getInstance().removeProtocolFromAutosend(shared_from_this());
	Protocol::release();
}

void ProtocolSpectator::login(const std::string& name, const std::string& password, OperatingSystem_t operatingSystem)
{
	//dispatcher thread
	Player* foundPlayer = g_game.getPlayerByName(name);
	if (foundPlayer && foundPlayer->isLiveCasting()) {

		if(foundPlayer->castPassword != password) {
			disconnectClient("The password you have entered is incorrect.");
			return;
		}

		for(auto it : foundPlayer->spectatorBans) {
			if(it.second == getIP()) {
				disconnectClient("You have been banned from this live cast.");
				return;
			}
		}

		if ((int32_t)foundPlayer->spectatorCount >= g_config.getNumber(ConfigManager::LIVE_CAST_MAX) && g_config.getNumber(ConfigManager::LIVE_CAST_MAX) != 0) {
			disconnectClient("This live cast is full, please try again later.");
			return;
		}

		spectator = new Player(getThis());
		uint16_t spectatorCount = foundPlayer->spectatorCount;
		std::stringstream ss;
		ss << "Spectator " << (spectatorCount + 1);
		while(g_game.getPlayerByName(ss.str()) != nullptr || foundPlayer->spectatorBans.find(name) != foundPlayer->spectatorBans.end()) {
			ss.str(std::string());
			ss << "Spectator " << (++spectatorCount);
		}
		spectator->setName(ss.str());

		spectator->isSpectator = true;

		if (g_game.getGameState() == GAME_STATE_CLOSING) {
			disconnectClient("The game is just going down.\nPlease try again later.");
			return;
		}

		if (g_game.getGameState() == GAME_STATE_CLOSED) {
			disconnectClient("Server is currently closed.\nPlease try again later.");
			return;
		}

		spectator->setOperatingSystem(operatingSystem);
		acceptPackets = true;
	} else {
		disconnectClient("Live cast could not be found.");
		return;
	}

	connect(foundPlayer->getID(), operatingSystem);

	OutputMessagePool::getInstance().addProtocolToAutosend(shared_from_this());
}

void ProtocolSpectator::syncOpenContainers() {
	for (const auto& it : player->openContainers) {
			Container* container = it.second.container;
			if(container) {
				sendContainer(it.first, container, container->hasParent(), player->getContainerIndex(it.first));
			}
	}
}

void ProtocolSpectator::connect(uint32_t playerId, OperatingSystem_t operatingSystem)
{
	eventConnect = 0;

	Player* foundPlayer = g_game.getPlayerByID(playerId);
	if (!foundPlayer) {
		disconnectClient("Caster could not be found.");
		return;
	}

	if (isConnectionExpired()) {
		//ProtocolSpectator::release() has been called at this point and the Connection object
		//no longer exists, so we return to prevent leakage of the Player.
		return;
	}

	player = foundPlayer;

	g_chat->removeUserFromAllChannels(*spectator);
	spectator->clearModalWindows();
	spectator->setOperatingSystem(operatingSystem);
	spectator->isConnecting = false;
	player->addSpectator(this);

	spectator->client = getThis();
	sendAddCreature(player, player->getPosition(), 0, false);
	spectator->lastIP = spectator->getIP();
	acceptPackets = true;

	sendChannel(CHANNEL_CAST, "Live Cast", nullptr, nullptr);
	std::stringstream ss;
	ss << spectator->getName() << " has joined the cast.";
	player->sendChannelMessage("", ss.str(), TALKTYPE_CHANNEL_O, CHANNEL_CAST);
	syncOpenContainers();
}

void ProtocolSpectator::logout(bool displayEffect, bool forced)
{
	player->removeSpectator(this);
	disconnect();
}

void ProtocolSpectator::onRecvFirstMessage(NetworkMessage& msg)
{
	if (g_game.getGameState() == GAME_STATE_SHUTDOWN) {
		disconnect();
		return;
	}

	OperatingSystem_t operatingSystem = static_cast<OperatingSystem_t>(msg.get<uint16_t>());
	version = msg.get<uint16_t>();

	msg.skipBytes(7); // U32 client version, U8 client type, U16 dat revision

	if (!Protocol::RSA_decrypt(msg)) {
		disconnect();
		return;
	}

	uint32_t key[4];
	key[0] = msg.get<uint32_t>();
	key[1] = msg.get<uint32_t>();
	key[2] = msg.get<uint32_t>();
	key[3] = msg.get<uint32_t>();
	enableXTEAEncryption();
	setXTEAKey(key);

	if (operatingSystem >= CLIENTOS_OTCLIENT_LINUX) {
		NetworkMessage opcodeMessage;
		opcodeMessage.addByte(0x32);
		opcodeMessage.addByte(0x00);
		opcodeMessage.add<uint16_t>(0x00);
		writeToOutputBuffer(opcodeMessage);
	}

	msg.skipBytes(1); // gamemaster flag

	std::string sessionKey = msg.getString();

	auto sessionArgs = explodeString(sessionKey, "\n", 4);
	std::string& accountName = sessionArgs[0];
	std::string& password = sessionArgs[1];

	if (!accountName.empty()) {
		disconnectClient("An error has occured.");
		return;
	}

	std::string characterName = msg.getString();

	uint32_t timeStamp = msg.get<uint32_t>();
	uint8_t randNumber = msg.getByte();
	if (challengeTimestamp != timeStamp || challengeRandom != randNumber) {
		disconnect();
		return;
	}

	if (version < CLIENT_VERSION_MIN || version > CLIENT_VERSION_MAX) {
		std::ostringstream ss;
		ss << "Only clients with protocol " << CLIENT_VERSION_STR << " allowed!";
		disconnectClient(ss.str());
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

	BanInfo banInfo;
	if (IOBan::isIpBanned(getIP(), banInfo)) {
		if (banInfo.reason.empty()) {
			banInfo.reason = "(none)";
		}

		std::ostringstream ss;
		ss << "Your IP has been banned until " << formatDateShort(banInfo.expiresAt) << " by " << banInfo.bannedBy << ".\n\nReason specified:\n" << banInfo.reason;
		disconnectClient(ss.str());
		return;
	}

	g_dispatcher.addTask(createTask(std::bind(&ProtocolSpectator::login, getThis(), characterName, password, operatingSystem)));
}

void ProtocolSpectator::onConnect()
{
	auto output = OutputMessagePool::getOutputMessage();
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

void ProtocolSpectator::disconnectClient(const std::string& message) const
{
	auto output = OutputMessagePool::getOutputMessage();
	output->addByte(0x14);
	output->addString(message);
	send(output);
	disconnect();
}

void ProtocolSpectator::writeToOutputBuffer(const NetworkMessage& msg) {
	auto out = getOutputBuffer(msg.getLength());
	out->append(msg);
}

void ProtocolSpectator::sendPingBack()
{
	NetworkMessage msg;
	msg.addByte(0x1E);
	writeToOutputBuffer(msg);
}

void ProtocolSpectator::parsePacket(NetworkMessage& msg)
{
	if (!acceptPackets || g_game.getGameState() == GAME_STATE_SHUTDOWN || msg.getLength() <= 0) {
		return;
	}

	uint8_t recvbyte = msg.getByte();

	if (!player) {
		if (recvbyte == 0x0F) {
			disconnect();
		}

		return;
	}

	//a dead player can not performs actions
	if (player->isRemoved() || player->getHealth() <= 0) {
		if (recvbyte == 0x0F) {
			disconnect();
			return;
		}

		if (recvbyte != 0x14) {
			return;
		}
	}

	switch (recvbyte) {
		case 0x14: g_dispatcher.addTask(createTask(std::bind(&ProtocolSpectator::logout, getThis(), true, false))); break;
		case 0x1D: sendPingBack(); break;
		case 0x64:
		case 0x65:
		case 0x66:
		case 0x67:
		case 0x68:
		case 0x6A:
		case 0x6B:
		case 0x6C:
		case 0x6D: 
			if(recvbyte == 0x64) {
				sendCancelWalk();
			}
			sendMoveCreature(player, player->getPosition(), player->getTile()->getStackposOfCreature(spectator, player->getCreature()), player->getPosition(), player->getTile()->getStackposOfCreature(spectator, player->getCreature()), false);
			sendCreatureTurn(player, player->getTile()->getStackposOfCreature(spectator, player->getCreature()));
		break;
		case 0x96: parseSay(msg); break;
		case 0x99: parseCloseChannel(msg); break;

		default:
			// std::cout << "Player(spectator): " << player->getName() << " sent an unknown packet header: 0x" << std::hex << static_cast<uint16_t>(recvbyte) << std::dec << "!" << std::endl;
			break;
	}

	if (msg.isOverrun()) {
		disconnect();
	}
}

void ProtocolSpectator::GetTileDescription(const Tile* tile, NetworkMessage& msg)
{
	msg.add<uint16_t>(0x00); //environmental effects

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

			if (++count == 10) {
				return;
			}
		}
	}

	const CreatureVector* creatures = tile->getCreatures();
	if (creatures) {
		for (const Creature* creature : boost::adaptors::reverse(*creatures)) {
			if (!player->canSeeCreature(creature)) {
				continue;
			}

			bool known;
			uint32_t removedKnown;
			checkCreatureAsKnown(creature->getID(), known, removedKnown);
			AddCreature(msg, creature, known, removedKnown);

			if (++count == 10) {
				return;
			}
		}
	}

	if (items) {
		for (auto it = items->getBeginDownItem(), end = items->getEndDownItem(); it != end; ++it) {
			msg.addItem(*it);

			if (++count == 10) {
				return;
			}
		}
	}
}

void ProtocolSpectator::GetMapDescription(int32_t x, int32_t y, int32_t z, int32_t width, int32_t height, NetworkMessage& msg)
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

void ProtocolSpectator::GetFloorDescription(NetworkMessage& msg, int32_t x, int32_t y, int32_t z, int32_t width, int32_t height, int32_t offset, int32_t& skip)
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

void ProtocolSpectator::checkCreatureAsKnown(uint32_t id, bool& known, uint32_t& removedKnown)
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

bool ProtocolSpectator::canSee(const Creature* c) const
{
	if (!c || !player || c->isRemoved()) {
		return false;
	}

	if (!player->canSeeCreature(c)) {
		return false;
	}

	return canSee(c->getPosition());
}

bool ProtocolSpectator::canSee(const Position& pos) const
{
	return canSee(pos.x, pos.y, pos.z);
}

bool ProtocolSpectator::canSee(int32_t x, int32_t y, int32_t z) const
{
	if (!player) {
		return false;
	}

	const Position& myPos = player->getPosition();
	if (myPos.z <= 7) {
		//we are on ground level or above (7 -> 0)
		//view is from 7 -> 0
		if (z > 7) {
			return false;
		}
	} else if (myPos.z >= 8) {
		//we are underground (8 -> 15)
		//view is +/- 2 from the floor we stand on
		if (std::abs(myPos.getZ() - z) > 2) {
			return false;
		}
	}

	//negative offset means that the action taken place is on a lower floor than ourself
	int32_t offsetz = myPos.getZ() - z;
	if ((x >= myPos.getX() - 8 + offsetz) && (x <= myPos.getX() + 9 + offsetz) &&
	        (y >= myPos.getY() - 6 + offsetz) && (y <= myPos.getY() + 7 + offsetz)) {
		return true;
	}
	return false;
}

void ProtocolSpectator::parseCloseChannel(NetworkMessage& msg)
{
	uint16_t channelId = msg.get<uint16_t>();
	addGameTask(&Game::playerCloseChannel, player->getID(), channelId);
}

void ProtocolSpectator::parseExecuteCommand(const std::string& text) {
	size_t pos = text.find(' ');
	std::string command = text.substr(1, pos - 1);
	if (command.empty()) {
		return;
	}

	if(command == "commands") {
		sendChannelMessage("", "Available commands:", TALKTYPE_CHANNEL_O, CHANNEL_CAST);
		sendChannelMessage("", "/name - change your nickname in this live cast.", TALKTYPE_CHANNEL_O, CHANNEL_CAST);
		sendChannelMessage("", "/spectators - view current spectators in this live cast.", TALKTYPE_CHANNEL_O, CHANNEL_CAST);
		sendChannelMessage("", "/commands - view available live cast commands.", TALKTYPE_CHANNEL_O, CHANNEL_CAST);
	} else if(command == "name") {
		std::string name = text.substr(pos + 1);
		int i = std::count_if(name.begin(),name.end(),[](char c){ return (!(std::isalpha(c)) && c !=  ' '); });
		if(name.empty() || name.length() < 2 || name.length() > 30 || name == "/name" || i > 0) {
			sendChannelMessage("", "Invalid name, you may not have a name that is longer than 30 characters or shorter than 2 characters.", TALKTYPE_CHANNEL_R1, CHANNEL_CAST);
			sendChannelMessage("", "Example of a valid name: Marksman Jack", TALKTYPE_CHANNEL_R1, CHANNEL_CAST);
			return;
		}
		if (name == spectator->getName() || name == player->getName() || player->spectatorBans.find(name) != player->spectatorBans.end()) {
			sendChannelMessage("", "There is already someone with that name.", TALKTYPE_CHANNEL_R1, CHANNEL_CAST);
			return;
		}
		for(ProtocolSpectator* Spectator : player->getSpectators()) {
			if(Spectator->spectator->getName() == name) {
				sendChannelMessage("", "There is already someone with that name.", TALKTYPE_CHANNEL_R1, CHANNEL_CAST);
				return;
			}
		}

		player->sendChannelMessage("", spectator->getName() + " has changed name to " + name + ".", TALKTYPE_CHANNEL_O, CHANNEL_CAST);
		spectator->setName(name);

	} else if(command == "spectators") {
		std::stringstream ss;
		ss << "Currently spectating this live cast (" << player->getSpectatorCount() << "):";
		sendChannelMessage("", ss.str(), TALKTYPE_CHANNEL_O, CHANNEL_CAST);
		for(ProtocolSpectator* Spectator : player->getSpectators()) {
			sendChannelMessage("", Spectator->spectator->getName(), TALKTYPE_CHANNEL_O, CHANNEL_CAST);
		}
	}
}

void ProtocolSpectator::parseSay(NetworkMessage& msg)
{
	if(std::find(player->spectatorMutes.begin(), player->spectatorMutes.end(), spectator) != player->spectatorMutes.end()) {
		sendChannelMessage("", "You are muted.", TALKTYPE_CHANNEL_R1, CHANNEL_CAST);
		return;
	}

	if(player->liveChatDisabled) {
		sendChannelMessage("", "The live cast chat is currently disabled.", TALKTYPE_CHANNEL_R1, CHANNEL_CAST);
		return;
	}

	SpeakClasses type = static_cast<SpeakClasses>(msg.getByte());
	switch (type) {
		case TALKTYPE_PRIVATE_TO:
		case TALKTYPE_PRIVATE_RED_TO:
			msg.getString();//reciever
			break;

		case TALKTYPE_CHANNEL_Y:
		case TALKTYPE_CHANNEL_R1:
			msg.get<uint16_t>();
			break;

		default:
			break;
	}

	const std::string text = msg.getString();
	if (text.length() > 255) {
		return;
	}

	InstantSpell* instantSpell = g_spells->getInstantSpell(text);
	if (instantSpell) {
		return;
	}

	if (!text.empty() && text.front() == '/') {
		parseExecuteCommand(text);
	} else {
		player->sendChannelMessage(spectator->getName(), text, TALKTYPE_CHANNEL_Y, CHANNEL_CAST);
	}
}

void ProtocolSpectator::sendCreatureLight(const Creature* creature)
{
	NetworkMessage msg;
	AddCreatureLight(msg, creature);
	writeToOutputBuffer(msg);
}

void ProtocolSpectator::sendWorldLight(const LightInfo& lightInfo)
{
	NetworkMessage msg;
	AddWorldLight(msg, lightInfo);
	writeToOutputBuffer(msg);
}

void ProtocolSpectator::sendStats()
{
	NetworkMessage msg;
	AddPlayerStats(msg);
	writeToOutputBuffer(msg);
}

void ProtocolSpectator::sendBasicData()
{
	NetworkMessage msg;
	msg.addByte(0x9F);
	if (player->isPremium()) {
		msg.addByte(1);
		msg.add<uint32_t>(time(nullptr) + (player->premiumDays * 86400));
	} else {
		msg.addByte(0);
		msg.add<uint32_t>(0);
	}
	msg.addByte(player->getVocation()->getClientId());
	msg.add<uint16_t>(0xFF); // number of known spells
	for (uint8_t spellId = 0x00; spellId < 0xFF; spellId++) {
		msg.addByte(spellId);
	}
	writeToOutputBuffer(msg);
}

void ProtocolSpectator::sendChannel(uint16_t channelId, const std::string& channelName, const UsersMap* channelUsers, const InvitedMap* invitedUsers)
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

void ProtocolSpectator::sendChannelMessage(const std::string& author, const std::string& text, SpeakClasses type, uint16_t channel)
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

void ProtocolSpectator::sendContainer(uint8_t cid, const Container* container, bool hasParent, uint16_t firstIndex)
{
	NetworkMessage msg;
	msg.addByte(0x6E);

	msg.addByte(cid);

	if (container->getID() == ITEM_BROWSEFIELD) {
		msg.addItem(1987, 1);
		msg.addString("Browse Field");
	} else {
		msg.addItem(container);
		msg.addString(container->getName());
	}

	msg.addByte(container->capacity());

	msg.addByte(hasParent ? 0x01 : 0x00);

	msg.addByte(container->isUnlocked() ? 0x01 : 0x00); // Drag and drop
	msg.addByte(container->hasPagination() ? 0x01 : 0x00); // Pagination

	uint32_t containerSize = container->size();
	msg.add<uint16_t>(containerSize);
	msg.add<uint16_t>(firstIndex);
	if (firstIndex < containerSize) {
		uint8_t itemsToSend = std::min<uint32_t>(std::min<uint32_t>(container->capacity(), containerSize - firstIndex), std::numeric_limits<uint8_t>::max());

		msg.addByte(itemsToSend);
		for (auto it = container->getItemList().begin() + firstIndex, end = it + itemsToSend; it != end; ++it) {
			msg.addItem(*it);
		}
	} else {
		msg.addByte(0x00);
	}
	writeToOutputBuffer(msg);
}

void ProtocolSpectator::sendCreatureTurn(const Creature* creature, uint32_t stackPos)
{
	NetworkMessage msg;
	msg.addByte(0x6B);
	msg.addPosition(creature->getPosition());
	msg.addByte(stackPos);
	msg.add<uint16_t>(0x63);
	msg.add<uint32_t>(creature->getID());
	msg.addByte(creature->getDirection());
	msg.addByte(player->canWalkthroughEx(creature) ? 0x00 : 0x01);
	writeToOutputBuffer(msg);
}

void ProtocolSpectator::sendCancelWalk()
{
	NetworkMessage msg;
	msg.addByte(0xB5);
	msg.addByte(player->getDirection());
	writeToOutputBuffer(msg);
}

void ProtocolSpectator::sendSkills()
{
	NetworkMessage msg;
	AddPlayerSkills(msg);
	writeToOutputBuffer(msg);
}

void ProtocolSpectator::sendMapDescription(const Position& pos)
{
	NetworkMessage msg;
	msg.addByte(0x64);
	msg.addPosition(player->getPosition());
	GetMapDescription(pos.x - 8, pos.y - 6, pos.z, 18, 14, msg);
	writeToOutputBuffer(msg);
}

void ProtocolSpectator::sendPendingStateEntered()
{
	NetworkMessage msg;
	msg.addByte(0x0A);
	writeToOutputBuffer(msg);
}

void ProtocolSpectator::sendEnterWorld()
{
	NetworkMessage msg;
	msg.addByte(0x0F);
	writeToOutputBuffer(msg);
}

void ProtocolSpectator::sendAddCreature(const Creature* creature, const Position& pos, int32_t stackpos, bool isLogin)
{
	NetworkMessage msg;
	msg.addByte(0x17);

	msg.add<uint32_t>(player->getID());
	msg.add<uint16_t>(0x32); // beat duration (50)

	msg.addDouble(Creature::speedA, 3);
	msg.addDouble(Creature::speedB, 3);
	msg.addDouble(Creature::speedC, 3);

	msg.addByte(0x00);

	msg.addByte(0x00); // can change pvp framing option
	msg.addByte(0x00); // expert mode button enabled

	msg.add<uint16_t>(0x00); // URL (string) to ingame store images
	msg.add<uint16_t>(25); // premium coin package size

	writeToOutputBuffer(msg);

	sendPendingStateEntered();
	sendEnterWorld();
	sendMapDescription(pos);

	sendInventoryItem(CONST_SLOT_HEAD, player->getInventoryItem(CONST_SLOT_HEAD));
	sendInventoryItem(CONST_SLOT_NECKLACE, player->getInventoryItem(CONST_SLOT_NECKLACE));
	sendInventoryItem(CONST_SLOT_BACKPACK, player->getInventoryItem(CONST_SLOT_BACKPACK));
	sendInventoryItem(CONST_SLOT_ARMOR, player->getInventoryItem(CONST_SLOT_ARMOR));
	sendInventoryItem(CONST_SLOT_RIGHT, player->getInventoryItem(CONST_SLOT_RIGHT));
	sendInventoryItem(CONST_SLOT_LEFT, player->getInventoryItem(CONST_SLOT_LEFT));
	sendInventoryItem(CONST_SLOT_LEGS, player->getInventoryItem(CONST_SLOT_LEGS));
	sendInventoryItem(CONST_SLOT_FEET, player->getInventoryItem(CONST_SLOT_FEET));
	sendInventoryItem(CONST_SLOT_RING, player->getInventoryItem(CONST_SLOT_RING));
	sendInventoryItem(CONST_SLOT_AMMO, player->getInventoryItem(CONST_SLOT_AMMO));

	sendStats();
	sendSkills();

	//gameworld light-settings
	LightInfo lightInfo;
	g_game.getWorldLightInfo(lightInfo);
	sendWorldLight(lightInfo);

	//player light level
	sendCreatureLight(creature);

	sendBasicData();
}

void ProtocolSpectator::sendMoveCreature(const Creature* creature, const Position& newPos, int32_t newStackPos, const Position& oldPos, int32_t oldStackPos, bool teleport)
{
	NetworkMessage msg;

	msg.addByte(0x6D);
	msg.addPosition(oldPos);
	msg.addByte(oldStackPos);
	msg.addPosition(newPos);

	if (oldPos.y > newPos.y) { // north, for old x
		msg.addByte(0x65);
	} else if (oldPos.y < newPos.y) { // south, for old x
		msg.addByte(0x67);
	}

	if (oldPos.x < newPos.x) { // east, [with new y]
		msg.addByte(0x66);
	} else if (oldPos.x > newPos.x) { // west, [with new y]
		msg.addByte(0x68);
	}
	writeToOutputBuffer(msg);
}

void ProtocolSpectator::sendInventoryItem(slots_t slot, const Item* item)
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

void ProtocolSpectator::AddCreature(NetworkMessage& msg, const Creature* creature, bool known, uint32_t remove)
{
	CreatureType_t creatureType = creature->getType();

	const Player* otherPlayer = creature->getPlayer();

	if (known) {
		msg.add<uint16_t>(0x62);
		msg.add<uint32_t>(creature->getID());
	} else {
		msg.add<uint16_t>(0x61);
		msg.add<uint32_t>(remove);
		msg.add<uint32_t>(creature->getID());
		msg.addByte(creatureType);
		msg.addString(creature->getName());
	}

	if (creature->isHealthHidden()) {
		msg.addByte(0x00);
	} else {
		msg.addByte(std::ceil((static_cast<double>(creature->getHealth()) / std::max<int32_t>(creature->getMaxHealth(), 1)) * 100));
	}

	msg.addByte(creature->getDirection());

	if (!creature->isInGhostMode() && !creature->isInvisible()) {
		AddOutfit(msg, creature->getCurrentOutfit());
	} else {
		static Outfit_t outfit;
		AddOutfit(msg, outfit);
	}

	LightInfo lightInfo;
	creature->getCreatureLight(lightInfo);
	msg.addByte(lightInfo.level);
	msg.addByte(lightInfo.color);

	msg.add<uint16_t>(creature->getStepSpeed() / 2);

	msg.addByte(player->getSkullClient(creature));
	msg.addByte(player->getPartyShield(otherPlayer));

	if (!known) {
		msg.addByte(player->getGuildEmblem(otherPlayer));
	}

	if (creatureType == CREATURETYPE_MONSTER) {
		const Creature* master = creature->getMaster();
		if (master) {
			const Player* masterPlayer = master->getPlayer();
			if (masterPlayer) {
				if (masterPlayer == player) {
					creatureType = CREATURETYPE_SUMMON_OWN;
				} else {
					creatureType = CREATURETYPE_SUMMON_OTHERS;
				}
			}
		}
	}

	msg.addByte(creatureType); // Type (for summons)
	msg.addByte(creature->getSpeechBubble());
	msg.addByte(0xFF); // MARK_UNMARKED

	if (otherPlayer) {
		msg.add<uint16_t>(otherPlayer->getHelpers());
	} else {
		msg.add<uint16_t>(0x00);
	}

	msg.addByte(player->canWalkthroughEx(creature) ? 0x00 : 0x01);
}

void ProtocolSpectator::AddPlayerStats(NetworkMessage& msg)
{
	msg.addByte(0xA0);

	msg.add<uint16_t>(std::min<int32_t>(player->getHealth(), std::numeric_limits<uint16_t>::max()));
	msg.add<uint16_t>(std::min<int32_t>(player->getMaxHealth(), std::numeric_limits<uint16_t>::max()));

	msg.add<uint32_t>(player->getFreeCapacity());
	msg.add<uint32_t>(player->getCapacity());

	msg.add<uint64_t>(player->getExperience());

	msg.add<uint16_t>(player->getLevel());
	msg.addByte(player->getLevelPercent());

	msg.add<uint16_t>(100); // base xp gain rate
	msg.add<uint16_t>(0); // xp voucher
	msg.add<uint16_t>(0); // low level bonus
	msg.add<uint16_t>(0); // xp boost
	msg.add<uint16_t>(100); // stamina multiplier (100 = x1.0)

	msg.add<uint16_t>(std::min<int32_t>(player->getMana(), std::numeric_limits<uint16_t>::max()));
	msg.add<uint16_t>(std::min<int32_t>(player->getMaxMana(), std::numeric_limits<uint16_t>::max()));

	msg.addByte(std::min<uint32_t>(player->getMagicLevel(), std::numeric_limits<uint8_t>::max()));
	msg.addByte(std::min<uint32_t>(player->getBaseMagicLevel(), std::numeric_limits<uint8_t>::max()));
	msg.addByte(player->getMagicLevelPercent());

	msg.addByte(player->getSoul());

	msg.add<uint16_t>(player->getStaminaMinutes());

	msg.add<uint16_t>(player->getBaseSpeed() / 2);

	Condition* condition = player->getCondition(CONDITION_REGENERATION);
	msg.add<uint16_t>(condition ? condition->getTicks() / 1000 : 0x00);

	msg.add<uint16_t>(player->getOfflineTrainingTime() / 60 / 1000);

	msg.add<uint16_t>(0); // xp boost time (seconds)
	msg.addByte(0); // enables exp boost in the store
}

void ProtocolSpectator::AddPlayerSkills(NetworkMessage& msg)
{
	msg.addByte(0xA1);

	for (uint8_t i = SKILL_FIRST; i <= SKILL_LAST; ++i) {
		msg.add<uint16_t>(std::min<int32_t>(player->getSkillLevel(i), std::numeric_limits<uint16_t>::max()));
		msg.add<uint16_t>(player->getBaseSkill(i));
		msg.addByte(player->getSkillPercent(i));
	}

	// critical chance
	msg.add<uint16_t>(0);
	msg.add<uint16_t>(0);

	// critical damage
	msg.add<uint16_t>(0);
	msg.add<uint16_t>(0);

	// life leech chance
	msg.add<uint16_t>(0);
	msg.add<uint16_t>(0);

	// life leech
	msg.add<uint16_t>(0);
	msg.add<uint16_t>(0);

	// mana leech chance
	msg.add<uint16_t>(0);
	msg.add<uint16_t>(0);

	// mana leech
	msg.add<uint16_t>(0);
	msg.add<uint16_t>(0);
}

void ProtocolSpectator::AddOutfit(NetworkMessage& msg, const Outfit_t& outfit)
{
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

	msg.add<uint16_t>(outfit.lookMount);
}

void ProtocolSpectator::AddWorldLight(NetworkMessage& msg, const LightInfo& lightInfo)
{
	msg.addByte(0x82);
	msg.addByte(lightInfo.level);
	msg.addByte(lightInfo.color);
}

void ProtocolSpectator::AddCreatureLight(NetworkMessage& msg, const Creature* creature)
{
	LightInfo lightInfo;
	creature->getCreatureLight(lightInfo);

	msg.addByte(0x8D);
	msg.add<uint32_t>(creature->getID());
	msg.addByte(lightInfo.level);
	msg.addByte(lightInfo.color);
}