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

#ifndef __PARTY_H__
#define __PARTY_H__

#include "player.h"
#include "monsters.h"

class Player;
class Party;

typedef std::vector<Player*> PlayerVector;

class Party
{
	public:
		Party(Player* _leader);
		virtual ~Party();

		Player* getLeader() const {
			return leader;
		}
		void setLeader(Player* _leader) {
			leader = _leader;
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
		bool invitePlayer(Player* player);
		bool joinParty(Player* player);
		void revokeInvitation(Player* player);
		bool passPartyLeadership(Player* player);
		bool leaveParty(Player* player);

		bool removeInvite(Player* player);

		bool isPlayerInvited(const Player* player) const;
		void updateAllPartyIcons();
		void broadcastPartyMessage(MessageClasses msgClass, const std::string& msg, bool sendToInvitations = false);
		void broadcastPartyLoot(const std::string& loot);
		bool disbandParty() {
			return (memberList.empty() && inviteList.empty());
		}
		bool canOpenCorpse(uint32_t ownerId);

		void shareExperience(uint64_t experience);
		bool setSharedExperience(Player* player, bool _sharedExpActive);
		bool isSharedExperienceActive() const {
			return sharedExpActive;
		}
		bool isSharedExperienceEnabled() const {
			return sharedExpEnabled;
		}
		bool canUseSharedExperience(const Player* player) const;
		void updateSharedExperience();

		void addPlayerHealedMember(Player* player, uint32_t points);
		void addPlayerDamageMonster(Player* player, uint32_t points);
		void clearPlayerPoints(Player* player);

	protected:
		bool sharedExpActive;
		bool sharedExpEnabled;

		Player* leader;
		PlayerVector memberList;
		PlayerVector inviteList;

		struct CountBlock_t {
			int32_t totalHeal;
			int32_t totalDamage;
			int64_t ticks;
		};
		typedef std::map<uint32_t, CountBlock_t> CountMap;
		CountMap pointMap;

		bool canEnableSharedExperience();
};

#endif
