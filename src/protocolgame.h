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

#ifndef FS_PROTOCOLGAME_H_FACA2A2D1A9348B78E8FD7E8003EBB87
#define FS_PROTOCOLGAME_H_FACA2A2D1A9348B78E8FD7E8003EBB87

#include "protocolgamebase.h"
#include "protocolspectator.h"
#include "chat.h"
#include "creature.h"
#include "tasks.h"

typedef std::unordered_map<Player*, ProtocolGame*> LiveCastsMap;

extern Game g_game;

class ProtocolGame final : public ProtocolGameBase
{
	public:
		// static protocol information
		enum {server_sends_first = true};
		enum {protocol_identifier = 0}; // Not required as we send first
		enum {use_checksum = true};
		static const char* protocol_name() {
			return "gameworld protocol";
		}

		explicit ProtocolGame(Connection_ptr connection) : ProtocolGameBase(connection) {}

		void login(const std::string& name, uint32_t accnumber, OperatingSystem_t operatingSystem);
		void logout(bool displayEffect, bool forced);

		uint16_t getVersion() const {
			return version;
		}

		bool canJoinCast(const std::string& password) {
			return castinfo.enabled && castinfo.password == password;
		}

		uint32_t getSpectatorsCount() {
			return castinfo.spectators.size();
		}

		static ProtocolGame* getLiveCast(Player* player) {
			const auto& it = liveCasts.find(player);
			return it != liveCasts.end() ? it->second : nullptr;
		}

		static LiveCastsMap liveCasts;
	private:
		ProtocolGame_ptr getThis() {
			return std::static_pointer_cast<ProtocolGame>(shared_from_this());
		}

		bool startLiveCasting(const std::string& password);
		void stopLiveCasting();
		void pauseLiveCasting(const std::string& reason);
		bool isLiveCasting() {
			return castinfo.enabled;
		}
		bool kickCastSpectator(std::string name);
		bool banCastSpectator(std::string name);
		bool unBanCastSpectator(std::string name);
		bool muteCastSpectator(std::string name);
		bool unMuteCastSpectator(std::string name);

		void connect(uint32_t playerId, OperatingSystem_t operatingSystem);
		void writeToSpectatorsOutputBuffer(const NetworkMessage& msg);
		void writeToOutputBuffer(const NetworkMessage& msg, bool broadcast = true) final;

		void release() final;

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

		//otclient
		void parseExtendedOpcode(NetworkMessage& msg);

		// send functions
		void sendCastChannel();

		friend class ProtocolSpectator;
		friend class Player;

		// casting info
		struct {
			uint16_t lastSpectatorNum = 0; // identifying a spectator number

			bool enabled = false;
			bool paused = false;

			std::vector<ProtocolSpectator_ptr> spectators = {};
			std::unordered_map<std::string, uint32_t> muteList = {};
			std::unordered_map<std::string, uint32_t> banList = {};
			std::unordered_map<uint32_t, std::string> spectatorsNames = {}; // names based on ip

			std::string password;
			std::string pauseReason;

			void pause(const std::string& reason) {
				paused = true;
				pauseReason = reason;
			}

			void operator()(std::string newPassword) {
				if (!newPassword.empty()) {
					password = newPassword;
				}
				paused = false;
			}
		} castinfo;
};

#endif
