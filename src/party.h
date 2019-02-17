/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2018  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_PARTY_H_41D4D7CF417C4CC99FAE94D552255044
#define FS_PARTY_H_41D4D7CF417C4CC99FAE94D552255044

#include "player.h"
#include "monsters.h"

class Player;
class Party;

using PlayerVector = std::vector<Player*>;

class Party
{
	public:
		explicit Party(Player* leader);

		Player* getLeader() const {
			return leader;
		}
		PlayerVector& getMembers() {
			return memberList;
		}
		const PlayerVector& getInvitees() const {
			return inviteList;
		}
		size_t getMemberCount() const {
			return memberList.size();
		}
		size_t getInvitationCount() const {
			return inviteList.size();
		}

		void disband();
		bool invitePlayer(Player& player);
		bool joinParty(Player& player);
		void revokeInvitation(Player& player);
		bool passPartyLeadership(Player* player);
		bool leaveParty(Player* player);

		bool removeInvite(Player& player, bool removeFromPlayer = true);

		bool isPlayerInvited(const Player* player) const;
		void updateAllPartyIcons();
		void broadcastPartyMessage(MessageClasses msgClass, const std::string& msg, bool sendToInvitations = false);
		void broadcastPartyLoot(const std::string& loot);
		bool empty() const {
			return memberList.empty() && inviteList.empty();
		}
		bool canOpenCorpse(uint32_t ownerId) const;

		void shareExperience(uint64_t experience, Creature* source = nullptr);
		bool setSharedExperience(Player* player, bool sharedExpActive);
		bool isSharedExperienceActive() const {
			return sharedExpActive;
		}
		bool isSharedExperienceEnabled() const {
			return sharedExpEnabled;
		}
		bool canUseSharedExperience(const Player* player) const;
		void updateSharedExperience();

		void updatePlayerTicks(Player* player, uint32_t points);
		void clearPlayerPoints(Player* player);

	private:
		bool canEnableSharedExperience();

		std::map<uint32_t, int64_t> ticksMap;

		PlayerVector memberList;
		PlayerVector inviteList;

		Player* leader;

		bool sharedExpActive = false;
		bool sharedExpEnabled = false;
};

#endif
