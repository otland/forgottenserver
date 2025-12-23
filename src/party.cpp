// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "party.h"

#include "configmanager.h"
#include "events.h"
#include "game.h"

extern Game g_game;

void Party::setLeader(const std::shared_ptr<Player>& leader)
{
	this->leader = leader;
	leader->setParty(shared_from_this());
}

void Party::disband()
{
	if (!tfs::events::party::onDisband(shared_from_this())) {
		return;
	}

	const auto currentLeader = leader.lock();
	leader.reset();

	currentLeader->setParty(nullptr);
	currentLeader->sendClosePrivate(CHANNEL_PARTY);
	g_game.updatePlayerShield(currentLeader);
	currentLeader->sendCreatureSkull(currentLeader);
	currentLeader->sendTextMessage(MESSAGE_INFO_DESCR, "Your party has been disbanded.");

	for (const auto& invitee : inviteList | tfs::views::lock_weak_ptrs) {
		invitee->removePartyInvitation(shared_from_this());
		currentLeader->sendCreatureShield(invitee);
	}
	inviteList.clear();

	for (const auto& member : memberList | tfs::views::lock_weak_ptrs) {
		member->setParty(nullptr);
		member->sendClosePrivate(CHANNEL_PARTY);
		member->sendTextMessage(MESSAGE_INFO_DESCR, "Your party has been disbanded.");
	}

	for (const auto& member : memberList | tfs::views::lock_weak_ptrs) {
		g_game.updatePlayerShield(member);

		for (const auto& otherMember : memberList | tfs::views::lock_weak_ptrs) {
			otherMember->sendCreatureSkull(member);
		}

		member->sendCreatureSkull(currentLeader);
		currentLeader->sendCreatureSkull(member);
	}
	memberList.clear();
}

bool Party::leaveParty(const std::shared_ptr<Player>& player, bool forceRemove /* = false */)
{
	if (!player) {
		return false;
	}

	if (player->getParty().get() != this && !tfs::owner_equal(leader, player)) {
		return false;
	}

	bool canRemove = tfs::events::party::onLeave(shared_from_this(), player);
	if (!forceRemove && !canRemove) {
		return false;
	}

	bool missingLeader = false;
	if (tfs::owner_equal(leader, player)) {
		if (!memberList.empty()) {
			if (memberList.size() == 1 && inviteList.empty()) {
				missingLeader = true;
			} else {
				passPartyLeadership(memberList.begin()->lock(), true);
			}
		} else {
			missingLeader = true;
		}
	}

	// since we already passed the leadership, we remove the player from the list
	memberList.erase(player);

	player->setParty(nullptr);
	player->sendClosePrivate(CHANNEL_PARTY);
	g_game.updatePlayerShield(player);

	for (const auto& member : memberList | tfs::views::lock_weak_ptrs) {
		member->sendCreatureSkull(player);
		player->sendPlayerPartyIcons(member);
	}

	const auto& leader = getLeader();
	leader->sendCreatureSkull(player);
	player->sendCreatureSkull(player);
	player->sendPlayerPartyIcons(leader);

	// remove pending invitation icons from the screen
	for (const auto& invitee : inviteList | tfs::views::lock_weak_ptrs) {
		player->sendCreatureShield(invitee);
	}

	player->sendTextMessage(MESSAGE_INFO_DESCR, "You have left the party.");

	updateSharedExperience();

	clearPlayerPoints(player);

	broadcastPartyMessage(MESSAGE_INFO_DESCR, std::format("{:s} has left the party.", player->getName()));

	if (missingLeader || empty()) {
		disband();
	}

	return true;
}

bool Party::passPartyLeadership(const std::shared_ptr<Player>& player, bool forceRemove /* = false*/)
{
	if (!player || getLeader() == player || player->getParty().get() != this) {
		return false;
	}

	if (!tfs::events::party::onPassLeadership(shared_from_this(), player) && !forceRemove) {
		return false;
	}

	// Remove it before to broadcast the message correctly
	memberList.erase(player);

	broadcastPartyMessage(MESSAGE_INFO_DESCR, std::format("{:s} is now the leader of the party.", player->getName()),
	                      true);

	const auto oldLeader = getLeader();
	leader = player;

	memberList.emplace(oldLeader);

	updateSharedExperience();

	for (const auto& member : memberList | tfs::views::lock_weak_ptrs) {
		member->sendCreatureShield(oldLeader);
		member->sendCreatureShield(player);
	}

	for (const auto& invitee : inviteList | tfs::views::lock_weak_ptrs) {
		invitee->sendCreatureShield(oldLeader);
		invitee->sendCreatureShield(player);
	}

	player->sendCreatureShield(oldLeader);
	player->sendCreatureShield(player);

	player->sendTextMessage(MESSAGE_INFO_DESCR, "You are now the leader of the party.");
	return true;
}

bool Party::joinParty(const std::shared_ptr<Player>& player)
{
	// check if lua scripts allow the player to join
	if (!tfs::events::party::onJoin(shared_from_this(), player)) {
		return false;
	}

	// first player accepted the invitation the party gets officially formed the leader can no longer take invitations
	// from others
	const auto& leader = getLeader();
	if (memberList.empty()) {
		leader->clearPartyInvitations();
	}

	// add player to the party
	memberList.emplace(player);
	player->setParty(shared_from_this());
	broadcastPartyMessage(MESSAGE_INFO_DESCR, std::format("{:s} has joined the party.", player->getName()));

	// remove player pending invitations to this and other parties
	player->clearPartyInvitations();

	// update player icon on the screen
	g_game.updatePlayerShield(player);

	// update player-member party icons
	for (const auto& member : memberList | tfs::views::lock_weak_ptrs) {
		member->sendCreatureSkull(player);
		player->sendPlayerPartyIcons(member);
	}

	// update player-leader party icons
	leader->sendCreatureSkull(player);
	player->sendPlayerPartyIcons(leader);
	// update player own skull
	player->sendCreatureSkull(player);

	// show the new member who else is invited
	for (const auto& invitee : inviteList | tfs::views::lock_weak_ptrs) {
		player->sendCreatureShield(invitee);
	}

	// check the party eligibility for shared experience
	updateSharedExperience();

	const std::string& leaderName = leader->getName();
	player->sendTextMessage(
	    MESSAGE_INFO_DESCR,
	    std::format("You have joined {:s}'{:s} party. Open the party channel to communicate with your companions.",
	                leaderName, leaderName.back() == 's' ? "" : "s"));
	return true;
}

bool Party::removeInvite(const std::shared_ptr<Player>& player, bool removeFromPlayer /* = true*/)
{
	const auto erased = inviteList.erase(player);
	if (erased == 0) {
		return false;
	}

	const auto& leader = getLeader();
	leader->sendCreatureShield(player);
	player->sendCreatureShield(leader);

	if (removeFromPlayer) {
		player->removePartyInvitation(shared_from_this());
	}

	if (empty()) {
		disband();
	}

	return true;
}

void Party::revokeInvitation(const std::shared_ptr<Player>& player)
{
	if (!tfs::events::party::onRevokeInvitation(shared_from_this(), player)) {
		return;
	}

	const auto& leader = getLeader();
	player->sendTextMessage(MESSAGE_INFO_DESCR, std::format("{:s} has revoked {:s} invitation.", leader->getName(),
	                                                        leader->getSex() == PLAYERSEX_FEMALE ? "her" : "his"));
	leader->sendTextMessage(MESSAGE_INFO_DESCR,
	                        std::format("Invitation for {:s} has been revoked.", player->getName()));
	removeInvite(player);
}

bool Party::invitePlayer(const std::shared_ptr<Player>& player)
{
	if (isPlayerInvited(player)) {
		return false;
	}

	const auto& leader = getLeader();
	if (empty()) {
		leader->sendTextMessage(
		    MESSAGE_INFO_DESCR,
		    std::format("{:s} has been invited. Open the party channel to communicate with your members.",
		                player->getName()));
		g_game.updatePlayerShield(leader);
		leader->sendCreatureSkull(leader);
	} else {
		leader->sendTextMessage(MESSAGE_INFO_DESCR, std::format("{:s} has been invited.", player->getName()));
	}

	// add player to invite lists
	inviteList.emplace(player);
	player->addPartyInvitation(shared_from_this());

	// update leader-invitee party status
	leader->sendCreatureShield(player);
	player->sendCreatureShield(leader);

	// update the invitation status for other members
	for (const auto& member : memberList | tfs::views::lock_weak_ptrs) {
		member->sendCreatureShield(player);
	}

	player->sendTextMessage(MESSAGE_INFO_DESCR, std::format("{:s} has invited you to {:s} party.", leader->getName(),
	                                                        leader->getSex() == PLAYERSEX_FEMALE ? "her" : "his"));
	return true;
}

bool Party::isPlayerInvited(const std::shared_ptr<const Player>& player) const
{
	return std::ranges::find_if(inviteList, [&player](const auto& invitee) {
		       return tfs::owner_equal(invitee, player);
	       }) != inviteList.end();
}

void Party::updateAllPartyIcons()
{
	const auto& leader = getLeader();
	for (const auto& member : memberList | tfs::views::lock_weak_ptrs) {
		for (const auto& otherMember : memberList | tfs::views::lock_weak_ptrs) {
			member->sendCreatureShield(otherMember);
		}

		member->sendCreatureShield(leader);
		leader->sendCreatureShield(member);
	}
	leader->sendCreatureShield(leader);
}

void Party::broadcastPartyMessage(MessageClasses msgClass, const std::string& msg, bool sendToInvitations /*= false*/)
{
	for (const auto& member : memberList | tfs::views::lock_weak_ptrs) {
		member->sendTextMessage(msgClass, msg);
	}

	getLeader()->sendTextMessage(msgClass, msg);

	if (sendToInvitations) {
		for (const auto& invitee : inviteList | tfs::views::lock_weak_ptrs) {
			invitee->sendTextMessage(msgClass, msg);
		}
	}
}

void Party::updateSharedExperience()
{
	if (sharedExpActive) {
		bool result = getSharedExperienceStatus() == SHAREDEXP_OK;
		if (result != sharedExpEnabled) {
			sharedExpEnabled = result;
			updateAllPartyIcons();
		}
	}
}

namespace {

const char* getSharedExpReturnMessage(SharedExpStatus_t value)
{
	switch (value) {
		case SHAREDEXP_OK:
			return "Shared Experience is now active.";
		case SHAREDEXP_TOOFARAWAY:
			return "Shared Experience has been activated, but some members of your party are too far away.";
		case SHAREDEXP_LEVELDIFFTOOLARGE:
			return "Shared Experience has been activated, but the level spread of your party is too wide.";
		case SHAREDEXP_MEMBERINACTIVE:
			return "Shared Experience has been activated, but some members of your party are inactive.";
		case SHAREDEXP_EMPTYPARTY:
			return "Shared Experience has been activated, but you are alone in your party.";
		default:
			return "An error occured. Unable to activate shared experience.";
	}
}

} // namespace

bool Party::setSharedExperience(const std::shared_ptr<Player>& player, bool sharedExpActive)
{
	const auto& leader = getLeader();
	if (!player || leader != player) {
		return false;
	}

	if (this->sharedExpActive == sharedExpActive) {
		return true;
	}

	this->sharedExpActive = sharedExpActive;

	if (sharedExpActive) {
		SharedExpStatus_t sharedExpStatus = getSharedExperienceStatus();
		this->sharedExpEnabled = sharedExpStatus == SHAREDEXP_OK;
		leader->sendTextMessage(MESSAGE_INFO_DESCR, getSharedExpReturnMessage(sharedExpStatus));
	} else {
		leader->sendTextMessage(MESSAGE_INFO_DESCR, "Shared Experience has been deactivated.");
	}

	updateAllPartyIcons();
	return true;
}

void Party::shareExperience(uint64_t experience, const std::shared_ptr<Creature>& source /* = nullptr*/)
{
	uint64_t shareExperience = experience;
	tfs::events::party::onShareExperience(shared_from_this(), shareExperience);

	for (const auto& member : memberList | tfs::views::lock_weak_ptrs) {
		member->onGainSharedExperience(shareExperience, source);
	}
	getLeader()->onGainSharedExperience(shareExperience, source);
}

bool Party::canUseSharedExperience(const std::shared_ptr<const Player>& player) const
{
	return getMemberSharedExperienceStatus(player) == SHAREDEXP_OK;
}

SharedExpStatus_t Party::getMemberSharedExperienceStatus(const std::shared_ptr<const Player>& player) const
{
	if (memberList.empty()) {
		return SHAREDEXP_EMPTYPARTY;
	}

	const auto& leader = getLeader();
	uint32_t highestLevel = leader->getLevel();
	for (const auto& member : memberList | tfs::views::lock_weak_ptrs) {
		if (member->getLevel() > highestLevel) {
			highestLevel = member->getLevel();
		}
	}

	uint32_t minLevel = static_cast<uint32_t>(std::ceil((static_cast<float>(highestLevel) * 2) / 3));
	if (player->getLevel() < minLevel) {
		return SHAREDEXP_LEVELDIFFTOOLARGE;
	}

	if (!leader->getPosition().isInRange(player->getPosition(), EXPERIENCE_SHARE_RANGE, EXPERIENCE_SHARE_RANGE,
	                                     EXPERIENCE_SHARE_FLOORS)) {
		return SHAREDEXP_TOOFARAWAY;
	}

	if (!player->hasFlag(PlayerFlag_NotGainInFight)) {
		// check if the player has healed/attacked anything recently
		auto it = ticksMap.find(player->getID());
		if (it == ticksMap.end()) {
			return SHAREDEXP_MEMBERINACTIVE;
		}

		uint64_t timeDiff = OTSYS_TIME() - it->second;
		if (timeDiff > static_cast<uint64_t>(getNumber(ConfigManager::PZ_LOCKED))) {
			return SHAREDEXP_MEMBERINACTIVE;
		}
	}
	return SHAREDEXP_OK;
}

SharedExpStatus_t Party::getSharedExperienceStatus()
{
	SharedExpStatus_t leaderStatus = getMemberSharedExperienceStatus(getLeader());
	if (leaderStatus != SHAREDEXP_OK) {
		return leaderStatus;
	}

	for (const auto& member : memberList | tfs::views::lock_weak_ptrs) {
		SharedExpStatus_t memberStatus = getMemberSharedExperienceStatus(member);
		if (memberStatus != SHAREDEXP_OK) {
			return memberStatus;
		}
	}
	return SHAREDEXP_OK;
}

void Party::updatePlayerTicks(const std::shared_ptr<Player>& player, uint32_t points)
{
	if (points != 0 && !player->hasFlag(PlayerFlag_NotGainInFight)) {
		ticksMap[player->getID()] = OTSYS_TIME();
		updateSharedExperience();
	}
}

void Party::clearPlayerPoints(const std::shared_ptr<Player>& player)
{
	auto it = ticksMap.find(player->getID());
	if (it != ticksMap.end()) {
		ticksMap.erase(it);
		updateSharedExperience();
	}
}

bool Party::canOpenCorpse(uint32_t ownerId) const
{
	if (const auto& player = g_game.getPlayerByID(ownerId)) {
		return getLeader()->getID() == ownerId || tfs::owner_equal(player->getParty(), shared_from_this());
	}
	return false;
}
