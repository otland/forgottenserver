/**
* The Forgotten Server - a free and open-source MMORPG server emulator
* Copyright (C) 2017  Mark Samman <mark.samman@gmail.com>
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

#include "game.h"
#include "ban.h"
#include "iologindata.h"

extern Chat* g_chat;

void ProtocolSpectator::disconnectClient(const std::string& message) const
{
	auto output = OutputMessagePool::getOutputMessage();
	output->addByte(0x14);
	output->addString(message);
	send(output);
	disconnect();
}

void ProtocolSpectator::writeToOutputBuffer(const NetworkMessage& msg, bool)
{
	auto out = getOutputBuffer(msg.getLength());
	out->append(msg);
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

	std::string password = msg.getString();
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

	g_dispatcher.addTask(createTask(std::bind(&ProtocolSpectator::login, getThis(), characterName, password)));
}

void ProtocolSpectator::login(const std::string& character, const std::string& password)
{
	Player* caster = g_game.getPlayerByName(character); // it has a job later in the dispatcher thread.
	if (!caster || caster->isRemoved()) {
		disconnectClient("This player has stopped casting, please relog to renew casting list.");
		return;
	}

	casterProtocol = ProtocolGame::getLiveCast(caster);
	if (!casterProtocol || !casterProtocol->isLiveCasting()) {
		disconnectClient("This player has stopped casting, please relog to renew casting list.");
		return;
	}

	if (casterProtocol->castinfo.paused) {
		if (casterProtocol->castinfo.pauseReason.empty()) {
			disconnectClient("This cast is currently paused (by caster), please try again later.");
		}
		else {
			disconnectClient("This cast is currently paused (by caster) for:\n" + casterProtocol->castinfo.pauseReason);
		}

		return;
	}

	if (!casterProtocol->canJoinCast(password)) {
		disconnectClient("You have entered an incorrect password.");
		return;
	}

	if (isBanned()) {
		disconnectClient("You are banned from this cast.");
		return;
	}

	player = caster;
	casterProtocol->castinfo.spectators.push_back(getThis());
	IOLoginData::updateCast(casterProtocol->player->getGUID(), casterProtocol->castinfo.spectators.size());

	name = "Spectator [" + std::to_string(++casterProtocol->castinfo.lastSpectatorNum) + "]";

	eventConnect = 0;
	acceptPackets = true;

	sendAddCreature(player, player->getPosition(), 0, false);
	syncKnownCreatures();
	syncChannels();
	syncContainers();

	OutputMessagePool::getInstance().addProtocolToAutosend(shared_from_this());
}

void ProtocolSpectator::release()
{
	// dispatcher thread
	if (player && casterProtocol && player->client == casterProtocol->shared_from_this()) {
		casterProtocol->castinfo.spectators.erase(
			std::find(casterProtocol->castinfo.spectators.begin(),
				casterProtocol->castinfo.spectators.end(), getThis())
		);

		casterProtocol->sendChannelEvent(CHANNEL_CAST, name, CHANNELEVENT_EXCLUDE);

		TextMessage message = TextMessage(MESSAGE_GUILD, name + " has left the channel.");
		message.channelId = CHANNEL_CAST;
		casterProtocol->sendTextMessage(message);

		IOLoginData::updateCast(player->getGUID(), casterProtocol->castinfo.spectators.size());
	}

	OutputMessagePool::getInstance().removeProtocolFromAutosend(shared_from_this());
}

bool ProtocolSpectator::isMuted()
{
	for (auto it : casterProtocol->castinfo.muteList) {
		if (getIP() == it.second) {
			return true;
		}
	}
	return false;
}

bool ProtocolSpectator::isBanned()
{
	for (auto it : casterProtocol->castinfo.banList) {
		if (getIP() == it.second) {
			return true;
		}
	}
	return false;
}

// Parse methods
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
		case 0x14: g_dispatcher.addTask(createTask(std::bind(&ProtocolSpectator::disconnect, getThis()))); break;
		case 0x1D: addGameTask(&Game::playerReceivePingBack, player->getID()); break;
		case 0x1E: addGameTask(&Game::playerReceivePing, player->getID()); break;
		case 0x64: case 0x65: case 0x66: case 0x67: case 0x68: case 0x6A: case 0x6B: case 0x6C: case 0x6D: sendCancelWalk(); break;
		case 0x96: parseSay(msg); break;
		case 0x99: parseCloseChannel(msg); break;

		// directly send the change;
		case 0xA0: sendFightModes(); break;
		default: break;
	}
}

void ProtocolSpectator::parseSay(NetworkMessage& msg)
{
	if (isMuted()) {
		sendChannelMessage("[Cast System]", "You are muted.", TALKTYPE_CHANNEL_R1, CHANNEL_CAST);
		return;
	} else if (OTSYS_TIME() - lastSpeak <= 1000) {
		sendChannelMessage("[Cast System]", "You may not spam the channel.", TALKTYPE_CHANNEL_R1, CHANNEL_CAST);
		return;
	}

	/*SpeakClasses type = */msg.getByte();
	uint16_t channelId = msg.get<uint16_t>();
	if (channelId != CHANNEL_CAST) {
		return;
	}

	const std::string text = msg.getString();
	if (text.length() > 255) {
		return;
	}

	// TODO: avoid saying spells/allow spectator to do some commands.

	casterProtocol->sendChannelMessage(name, text, TALKTYPE_CHANNEL_Y, CHANNEL_CAST);
	lastSpeak = OTSYS_TIME();
}

void ProtocolSpectator::parseCloseChannel(NetworkMessage& msg)
{
	uint16_t channelId = msg.get<uint16_t>();
	if (channelId == CHANNEL_CAST) {
		sendCastChannel();
		sendChannelMessage("[Cast System]", "You can't close this channel!", TALKTYPE_CHANNEL_R1, CHANNEL_CAST);
	}
}

// Send Methods
void ProtocolSpectator::syncKnownCreatures()
{
	const auto& casterKnownCreatures = casterProtocol->knownCreatureSet;
	const auto playerPos = player->getPosition();
	const auto tile = player->getTile();

	if (!tile || !tile->getGround()) {
		disconnectClient("An error has occured.");
		return;
	}

	for (const auto creatureID : casterKnownCreatures) {
		bool known;
		uint32_t removedKnown;
		if (knownCreatureSet.find(creatureID) != knownCreatureSet.end()) {
			continue;
		}

		NetworkMessage msg;
		const auto creature = g_game.getCreatureByID(creatureID);
		if (creature && !creature->isRemoved()) {
			msg.addByte(0x6A);
			msg.addPosition(playerPos);
			msg.addByte(1); //stackpos
			checkCreatureAsKnown(creature->getID(), known, removedKnown);
			AddCreature(msg, creature, known, removedKnown);
			RemoveTileThing(msg, playerPos, 1);
		}
		writeToOutputBuffer(msg);
	}

	sendUpdateTile(tile, playerPos);
}

void ProtocolSpectator::syncChannels()
{
	const auto channels = g_chat->getChannelList(*player);
	for (const auto channel : channels) {
		const auto& channelUsers = channel->getUsers();
		if (channelUsers.find(player->getID()) != channelUsers.end()) {
			sendChannel(channel->getId(), channel->getName(), &channelUsers, channel->getInvitedUsers());
		}
	}

	sendCastChannel();

	for (ProtocolSpectator_ptr it : casterProtocol->castinfo.spectators) {
		if (it && it != getThis()) {
			it->sendChannelEvent(CHANNEL_CAST, name, CHANNELEVENT_JOIN);
		}
	}
	casterProtocol->sendChannelEvent(CHANNEL_CAST, name, CHANNELEVENT_JOIN, false);
}

void ProtocolSpectator::syncContainers()
{
	const auto openContainers = player->getOpenContainers();
	for (const auto& it : openContainers) {
		auto openContainer = it.second;
		auto container = openContainer.container;
		sendContainer(it.first, container, container->hasParent(), openContainer.index);
	}
}

void ProtocolSpectator::sendCastChannel()
{
	NetworkMessage msg;
	msg.addByte(0xAC);

	msg.add<uint16_t>(CHANNEL_CAST);
	msg.addString(CHANNEL_CAST_STR);

	msg.add<uint16_t>(casterProtocol->castinfo.spectators.size() - 1); // without me;
	for (auto it : casterProtocol->castinfo.spectators) {
		if (it != getThis()) {
			msg.addString(it->name);
		}
	}

	msg.add<uint16_t>(0x00);
	writeToOutputBuffer(msg);
}