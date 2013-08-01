/**
 * The Forgotten Server - a server application for the MMORPG Tibia
 * Copyright (C) 2013  Mark Samman <mark.samman@gmail.com>
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

#include "chat.h"
#include "configmanager.h"
#include "player.h"
#include "game.h"
#include "iologindata.h"

extern ConfigManager g_config;
extern Game g_game;
extern Chat g_chat;

PrivateChatChannel::PrivateChatChannel(uint16_t channelId, const std::string& channelName) :
	ChatChannel(channelId, channelName)
{
	m_owner = 0;
}

bool PrivateChatChannel::isInvited(const Player* player)
{
	if (!player) {
		return false;
	}

	if (player->getGUID() == getOwner()) {
		return true;
	}

	InvitedMap::const_iterator it = m_invites.find(player->getGUID());
	return it != m_invites.end();
}

bool PrivateChatChannel::addInvited(Player* player)
{
	InvitedMap::const_iterator it = m_invites.find(player->getGUID());

	if (it != m_invites.end()) {
		return false;
	}

	m_invites[player->getGUID()] = player;
	return true;
}

bool PrivateChatChannel::removeInvited(Player* player)
{
	InvitedMap::iterator it = m_invites.find(player->getGUID());

	if (it == m_invites.end()) {
		return false;
	}

	m_invites.erase(it);
	return true;
}

void PrivateChatChannel::invitePlayer(Player* player, Player* invitePlayer)
{
	if (player != invitePlayer && addInvited(invitePlayer)) {
		std::ostringstream ss;
		ss << player->getName() << " invites you to " << (player->getSex() == PLAYERSEX_FEMALE ? "her" : "his") << " private chat channel.";
		invitePlayer->sendTextMessage(MSG_INFO_DESCR, ss.str());

		ss.str("");
		ss << invitePlayer->getName() << " has been invited.";
		player->sendTextMessage(MSG_INFO_DESCR, ss.str());

		for (const auto& it : m_users) {
			it.second->sendChannelEvent(m_id, invitePlayer->getName(), CHANNELEVENT_INVITE);
		}
	}
}

void PrivateChatChannel::excludePlayer(Player* player, Player* excludePlayer)
{
	if (player != excludePlayer && removeInvited(excludePlayer)) {
		removeUser(excludePlayer);

		std::ostringstream ss;
		ss << excludePlayer->getName() << " has been excluded.";
		player->sendTextMessage(MSG_INFO_DESCR, ss.str());

		excludePlayer->sendClosePrivate(getId());

		for (const auto& it : m_users) {
			it.second->sendChannelEvent(m_id, excludePlayer->getName(), CHANNELEVENT_EXCLUDE);
		}
	}
}

void PrivateChatChannel::closeChannel()
{
	for (const auto& it : m_users) {
		it.second->sendClosePrivate(getId());
	}
}

ChatChannel::ChatChannel(uint16_t channelId, const std::string& channelName)
{
	m_id = channelId;
	m_name = channelName;
}

bool ChatChannel::addUser(Player* player)
{
	if (m_users.find(player->getID()) != m_users.end()) {
		return false;
	}

	if (m_id == CHANNEL_GUILD) {
		Guild* guild = player->getGuild();
		if (guild && !guild->getMotd().empty()) {
			g_scheduler.addEvent(createSchedulerTask(150, boost::bind(&Game::sendGuildMotd, &g_game, player->getID(), guild->getId())));
		}
	}

	if (m_id == CHANNEL_GUILD || m_id == CHANNEL_PARTY || m_id == CHANNEL_PRIVATE) {
		for (const auto& it : m_users) {
			it.second->sendChannelEvent(m_id, player->getName(), CHANNELEVENT_JOIN);
		}
	}

	m_users[player->getID()] = player;
	return true;
}

bool ChatChannel::removeUser(Player* player)
{
	auto iter = m_users.find(player->getID());
	if (iter == m_users.end()) {
		return false;
	}

	m_users.erase(iter);

	if (m_id == CHANNEL_GUILD || m_id == CHANNEL_PARTY || m_id == CHANNEL_PRIVATE) {
		for (const auto& it : m_users) {
			it.second->sendChannelEvent(m_id, player->getName(), CHANNELEVENT_LEAVE);
		}
	}
	return true;
}

void ChatChannel::sendToAll(const std::string& message, SpeakClasses type)
{
	for (const auto& it : m_users) {
		it.second->sendChannelMessage("", message, type, m_id);
	}
}

bool ChatChannel::talk(Player* fromPlayer, SpeakClasses type, const std::string& text)
{
	if (m_users.find(fromPlayer->getID()) == m_users.end()) {
		return false;
	}

	if (!fromPlayer->hasFlag(PlayerFlag_CannotBeMuted) && (m_id == CHANNEL_ADVERTISING || m_id == CHANNEL_ADVERTISINGROOKGAARD)) {
		Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_CHANNELMUTEDTICKS, 120000, 0, false, m_id);
		fromPlayer->addCondition(condition);
	}

	for (const auto& it : m_users) {
		it.second->sendToChannel(fromPlayer, type, text, getId());
	}
	return true;
}

Chat::Chat()
{
	// Create the default channels
	m_normalChannels[CHANNEL_GAMEMASTER] = new ChatChannel(CHANNEL_GAMEMASTER, "Gamemaster");
	m_normalChannels[CHANNEL_TUTOR] = new ChatChannel(CHANNEL_TUTOR, "Tutor");
	m_normalChannels[CHANNEL_WORLDCHAT] = new ChatChannel(CHANNEL_WORLDCHAT, "World Chat");
	m_normalChannels[CHANNEL_ENGLISHCHAT] = new ChatChannel(CHANNEL_ENGLISHCHAT, "English Chat");
	m_normalChannels[CHANNEL_ADVERTISING] = new ChatChannel(CHANNEL_ADVERTISING, "Advertising");
	m_normalChannels[CHANNEL_ADVERTISINGROOKGAARD] = new ChatChannel(CHANNEL_ADVERTISINGROOKGAARD, "Advertising-Rookgaard");
	m_normalChannels[CHANNEL_HELP] = new ChatChannel(CHANNEL_HELP, "Help");

	dummyPrivate = new PrivateChatChannel(CHANNEL_PRIVATE, "Private Chat Channel");
}

Chat::~Chat()
{
	for (NormalChannelMap::iterator it = m_normalChannels.begin(); it != m_normalChannels.end(); ++it) {
		delete it->second;
	}

	m_normalChannels.clear();

	for (GuildChannelMap::iterator it = m_guildChannels.begin(); it != m_guildChannels.end(); ++it) {
		delete it->second;
	}

	m_guildChannels.clear();

	for (PartyChannelMap::iterator it = m_partyChannels.begin(); it != m_partyChannels.end(); ++it) {
		delete it->second;
	}

	m_partyChannels.clear();

	for (PrivateChannelMap::iterator it = m_privateChannels.begin(); it != m_privateChannels.end(); ++it) {
		delete it->second;
	}

	m_privateChannels.clear();
	delete dummyPrivate;
}

ChatChannel* Chat::createChannel(Player* player, uint16_t channelId)
{
	if (getChannel(player, channelId)) {
		return NULL;
	}

	switch (channelId) {
		case CHANNEL_GUILD: {
			Guild* guild = player->getGuild();
			if (guild) {
				ChatChannel* newChannel = new ChatChannel(channelId, guild->getName());
				m_guildChannels[guild->getId()] = newChannel;
				return newChannel;
			}
			break;
		}

		case CHANNEL_PARTY: {
			Party* party = player->getParty();
			if (party) {
				ChatChannel* newChannel = new ChatChannel(channelId, "Party");
				m_partyChannels[party] = newChannel;
				return newChannel;
			}
			break;
		}

		case CHANNEL_PRIVATE: {
			//only 1 private channel for each premium player
			if (!player->isPremium() || getPrivateChannel(player)) {
				return NULL;
			}

			//find a free private channel slot
			for (uint16_t i = 100; i < 10000; ++i) {
				if (m_privateChannels.find(i) == m_privateChannels.end()) {
					PrivateChatChannel* newChannel = new PrivateChatChannel(i, player->getName() + "'s Channel");
					newChannel->setOwner(player->getGUID());
					m_privateChannels[i] = newChannel;
					return newChannel;
				}
			}
			break;
		}

		default:
			break;
	}
	return NULL;
}

bool Chat::deleteChannel(Player* player, uint16_t channelId)
{
	switch (channelId) {
		case CHANNEL_GUILD: {
			Guild* guild = player->getGuild();

			if (!guild) {
				return false;
			}

			GuildChannelMap::iterator it = m_guildChannels.find(guild->getId());

			if (it == m_guildChannels.end()) {
				return false;
			}

			delete it->second;
			m_guildChannels.erase(it);
			return true;
		}

		case CHANNEL_PARTY: {
			PartyChannelMap::iterator it = m_partyChannels.find(player->getParty());

			if (it == m_partyChannels.end()) {
				return false;
			}

			delete it->second;
			m_partyChannels.erase(it);
			return true;
		}

		default: {
			PrivateChannelMap::iterator it = m_privateChannels.find(channelId);

			if (it == m_privateChannels.end()) {
				return false;
			}

			it->second->closeChannel();

			delete it->second;
			m_privateChannels.erase(it);
			return true;
		}
	}

	return false;
}

ChatChannel* Chat::addUserToChannel(Player* player, uint16_t channelId)
{
	ChatChannel* channel = getChannel(player, channelId);

	if (channel && channel->addUser(player)) {
		return channel;
	}

	return NULL;
}

bool Chat::removeUserFromChannel(Player* player, uint16_t channelId)
{
	ChatChannel* channel = getChannel(player, channelId);
	if (channel && channel->removeUser(player)) {
		if (channel->getOwner() == player->getGUID()) {
			deleteChannel(player, channelId);
		}

		return true;
	}
	return false;
}

void Chat::removeUserFromAllChannels(Player* player)
{
	for (NormalChannelMap::iterator it = m_normalChannels.begin(), end = m_normalChannels.end(); it != end; ++it) {
		it->second->removeUser(player);
	}

	for (PartyChannelMap::iterator it = m_partyChannels.begin(), end = m_partyChannels.end(); it != end; ++it) {
		it->second->removeUser(player);
	}

	for (GuildChannelMap::iterator it = m_guildChannels.begin(), end = m_guildChannels.end(); it != end; ++it) {
		it->second->removeUser(player);
	}

	for (PrivateChannelMap::iterator it = m_privateChannels.begin(), end = m_privateChannels.end(); it != end; ++it) {
		PrivateChatChannel* channel = it->second;
		channel->removeUser(player);

		if (channel->getOwner() == player->getGUID()) {
			deleteChannel(player, channel->getId());
		}
	}
}

bool Chat::talkToChannel(Player* player, SpeakClasses type, const std::string& text, uint16_t channelId)
{
	ChatChannel* channel = getChannel(player, channelId);

	if (!channel || !player) {
		return false;
	}

	if (player->getAccountType() < ACCOUNT_TYPE_GAMEMASTER) {
		if (player->getLevel() < 2 && channelId < CHANNEL_PARTY && channelId != CHANNEL_ADVERTISINGROOKGAARD) {
			player->sendCancel("You may not speak into channels as long as you are on level 1.");
			return false;
		} else if ((channelId == CHANNEL_ADVERTISING || channelId == CHANNEL_ADVERTISINGROOKGAARD) && player->hasCondition(CONDITION_CHANNELMUTEDTICKS, channelId)) {
			player->sendCancel("You may only place one offer in two minutes.");
			return false;
		} else if (channelId == CHANNEL_HELP && player->hasCondition(CONDITION_CHANNELMUTEDTICKS, CHANNEL_HELP)) {
			player->sendCancel("You are muted from the Help channel for using it inappropriately.");
			return false;
		}
	}

	if (channelId == CHANNEL_HELP && player->getAccountType() >= ACCOUNT_TYPE_TUTOR && text.length() > 6) {
		if (text.length() > 6 && text.substr(0, 6) == "!mute ") {
			std::string param = text.substr(6);
			trimString(param);
			Player* paramPlayer = g_game.getPlayerByName(param);

			if (paramPlayer && paramPlayer->getAccountType() < player->getAccountType()) {
				if (!paramPlayer->hasCondition(CONDITION_CHANNELMUTEDTICKS, CHANNEL_HELP)) {
					Condition* condition = Condition::createCondition(CONDITIONID_DEFAULT, CONDITION_CHANNELMUTEDTICKS, 3600000, 0, false, CHANNEL_HELP);
					paramPlayer->addCondition(condition);

					std::ostringstream ss;
					ss << paramPlayer->getName() << " has been muted by " << player->getName() << " for using Help Channel inappropriately.";
					channel->sendToAll(ss.str(), SPEAK_CHANNEL_R1);
				} else {
					player->sendCancel("That player is already muted.");
				}
			} else {
				player->sendCancel("A player with that name is not online.");
			}

			return true;
		} else if (text.length() > 8 && text.substr(0, 8) == "!unmute ") {
			std::string param = text.substr(8);
			trimString(param);
			Player* paramPlayer = g_game.getPlayerByName(param);

			if (paramPlayer && paramPlayer->getAccountType() < player->getAccountType()) {
				Condition* condition = paramPlayer->getCondition(CONDITION_CHANNELMUTEDTICKS, CONDITIONID_DEFAULT, CHANNEL_HELP);

				if (condition && condition->getTicks() > 0) {
					paramPlayer->removeCondition(condition);

					std::ostringstream ss;
					ss << paramPlayer->getName() << " has been unmuted by " << player->getName() << ".";
					channel->sendToAll(ss.str(), SPEAK_CHANNEL_R1);
				} else {
					player->sendCancel("That player is not muted.");
				}
			} else {
				player->sendCancel("A player with that name is not online.");
			}

			return true;
		}
	}

	if (channelId == CHANNEL_GUILD && player->getGuildLevel() > 1) {
		type = SPEAK_CHANNEL_O;
	}

	return channel->talk(player, type, text);
}

std::string Chat::getChannelName(Player* player, uint16_t channelId)
{
	ChatChannel* channel = getChannel(player, channelId);

	if (!channel) {
		return "";
	}

	return channel->getName();
}

ChannelList Chat::getChannelList(Player* player)
{
	ChannelList list;

	if (player->getGuild()) {
		ChatChannel* channel = getChannel(player, CHANNEL_GUILD);
		if (channel) {
			list.push_back(channel);
		} else {
			channel = createChannel(player, CHANNEL_GUILD);
			if (channel) {
				list.push_back(channel);
			}
		}
	}

	if (player->getParty()) {
		ChatChannel* channel = getChannel(player, CHANNEL_PARTY);
		if (channel) {
			list.push_back(channel);
		} else {
			channel = createChannel(player, CHANNEL_PARTY);
			if (channel) {
				list.push_back(channel);
			}
		}
	}

	for (NormalChannelMap::iterator it = m_normalChannels.begin(); it != m_normalChannels.end(); ++it) {
		ChatChannel* channel = getChannel(player, it->first);
		if (channel) {
			list.push_back(it->second);
		}
	}

	bool hasPrivate = false;
	for (PrivateChannelMap::iterator pit = m_privateChannels.begin(); pit != m_privateChannels.end(); ++pit) {
		if (PrivateChatChannel* channel = pit->second) {
			if (channel->isInvited(player)) {
				list.push_back(channel);
			}

			if (channel->getOwner() == player->getGUID()) {
				hasPrivate = true;
			}
		}
	}

	if (!hasPrivate && player->isPremium()) {
		list.push_front(dummyPrivate);
	}
	return list;
}

ChatChannel* Chat::getChannel(Player* player, uint16_t channelId)
{
	if (channelId == CHANNEL_GUILD) {
		Guild* guild = player->getGuild();

		if (guild) {
			GuildChannelMap::iterator git = m_guildChannels.find(guild->getId());

			if (git != m_guildChannels.end()) {
				return git->second;
			}
		}

		return NULL;
	}

	if (channelId == CHANNEL_PARTY) {
		if (!player->getParty()) {
			return NULL;
		}

		PartyChannelMap::iterator it = m_partyChannels.find(player->getParty());

		if (it != m_partyChannels.end()) {
			return it->second;
		}

		return NULL;
	}

	NormalChannelMap::iterator nit = m_normalChannels.find(channelId);

	if (nit != m_normalChannels.end()) {
		switch (channelId) {
			case CHANNEL_GAMEMASTER: {
				if (player->getAccountType() < ACCOUNT_TYPE_GAMEMASTER) {
					return NULL;
				}

				break;
			}

			case CHANNEL_TUTOR: {
				if (player->getAccountType() < ACCOUNT_TYPE_TUTOR) {
					return NULL;
				}

				break;
			}

			case CHANNEL_ADVERTISING: {
				if (player->getAccountType() < ACCOUNT_TYPE_SENIORTUTOR && player->getVocationId() == VOCATION_NONE) {
					return NULL;
				}

				break;
			}

			case CHANNEL_ADVERTISINGROOKGAARD: {
				if (player->getAccountType() < ACCOUNT_TYPE_SENIORTUTOR && player->getVocationId() != VOCATION_NONE) {
					return NULL;
				}

				break;
			}

			default:
				break;
		}

		return nit->second;
	}

	PrivateChannelMap::iterator pit = m_privateChannels.find(channelId);

	if (pit != m_privateChannels.end() && pit->second->isInvited(player)) {
		return pit->second;
	}

	return NULL;
}

ChatChannel* Chat::getGuildChannelById(uint32_t guildId)
{
	GuildChannelMap::iterator it = m_guildChannels.find(guildId);

	if (it == m_guildChannels.end()) {
		return NULL;
	}

	return it->second;
}

ChatChannel* Chat::getChannelById(uint16_t channelId)
{
	NormalChannelMap::iterator it = m_normalChannels.find(channelId);

	if (it == m_normalChannels.end()) {
		return NULL;
	}

	return it->second;
}

PrivateChatChannel* Chat::getPrivateChannel(Player* player)
{
	for (PrivateChannelMap::iterator it = m_privateChannels.begin(); it != m_privateChannels.end(); ++it) {
		PrivateChatChannel* channel = it->second;
		if (channel->getOwner() == player->getGUID()) {
			return channel;
		}
	}
	return NULL;
}
