// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_PARTY_H
#define FS_PARTY_H

#include "const.h"

class Creature;
class Player;

static constexpr int32_t EXPERIENCE_SHARE_RANGE = 30;
static constexpr int32_t EXPERIENCE_SHARE_FLOORS = 1;

enum SharedExpStatus_t : uint8_t
{
	SHAREDEXP_OK,
	SHAREDEXP_TOOFARAWAY,
	SHAREDEXP_LEVELDIFFTOOLARGE,
	SHAREDEXP_MEMBERINACTIVE,
	SHAREDEXP_EMPTYPARTY
};

class Party
{
public:
	explicit Party(const std::shared_ptr<Player>& leader);

	auto getLeader() const { return leader.lock(); }
	const auto& getMembers() const { return memberList; }
	const auto& getInvitees() const { return inviteList; }

	void disband();
	bool invitePlayer(const std::shared_ptr<Player>& player);
	bool joinParty(const std::shared_ptr<Player>& player);
	void revokeInvitation(const std::shared_ptr<Player>& player);
	bool passPartyLeadership(const std::shared_ptr<Player>& player, bool forceRemove = false);
	bool leaveParty(const std::shared_ptr<Player>& player, bool forceRemove = false);

	bool removeInvite(const std::shared_ptr<Player>& player, bool removeFromPlayer = true);

	bool isPlayerInvited(const std::shared_ptr<const Player>& player) const;
	void updateAllPartyIcons();
	void broadcastPartyMessage(MessageClasses msgClass, const std::string& msg, bool sendToInvitations = false);
	bool empty() const { return memberList.empty() && inviteList.empty(); }
	bool canOpenCorpse(uint32_t ownerId) const;

	void shareExperience(uint64_t experience, const std::shared_ptr<Creature>& source = nullptr);
	bool setSharedExperience(const std::shared_ptr<Player>& player, bool sharedExpActive);
	bool isSharedExperienceActive() const { return sharedExpActive; }
	bool isSharedExperienceEnabled() const { return sharedExpEnabled; }
	bool canUseSharedExperience(const std::shared_ptr<const Player>& player) const;
	SharedExpStatus_t getMemberSharedExperienceStatus(const std::shared_ptr<const Player>& player) const;
	void updateSharedExperience();

	void updatePlayerTicks(const std::shared_ptr<Player>& player, uint32_t points);
	void clearPlayerPoints(const std::shared_ptr<Player>& player);

private:
	SharedExpStatus_t getSharedExperienceStatus();

	std::map<uint32_t, int64_t> ticksMap;

	boost::container::flat_set<std::weak_ptr<Player>, std::owner_less<std::weak_ptr<const Player>>> memberList;
	boost::container::flat_set<std::weak_ptr<Player>, std::owner_less<std::weak_ptr<const Player>>> inviteList;

	std::weak_ptr<Player> leader;

	bool sharedExpActive = false;
	bool sharedExpEnabled = false;
};

#endif // FS_PARTY_H
