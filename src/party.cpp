// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "party.h"

#include "configmanager.h"
#include "events.h"
#include "game.h"

extern Game g_game;

Party::Party(std::shared_ptr<Player> leader) : leader{std::move(leader)} { this->leader->setParty(this); }

void Party::disband()
{
	if (!tfs::events::party::onDisband(this)) {
		return;
	}

	const auto currentLeader = leader;
	leader = nullptr;

	currentLeader->setParty(nullptr);
	currentLeader->sendClosePrivate(CHANNEL_PARTY);
	g_game.updatePlayerShield(currentLeader);
	currentLeader->sendCreatureSkull(currentLeader);
	currentLeader->sendTextMessage(MESSAGE_INFO_DESCR, "Your party has been disbanded.");

	for (const auto& invitee : inviteList) {
		invitee->removePartyInvitation(this);
		currentLeader->sendCreatureShield(invitee);
	}
	inviteList.clear();

	for (const auto& member : memberList) {
		member->setParty(nullptr);
		member->sendClosePrivate(CHANNEL_PARTY);
		member->sendTextMessage(MESSAGE_INFO_DESCR, "Your party has been disbanded.");
	}

	for (const auto& member : memberList) {
		g_game.updatePlayerShield(member);

		for (const auto& otherMember : memberList) {
			otherMember->sendCreatureSkull(member);
		}

		member->sendCreatureSkull(currentLeader);
		currentLeader->sendCreatureSkull(member);
	}
	memberList.clear();
	delete this;
}

bool Party::leaveParty(const std::shared_ptr<Player>& player, bool forceRemove /* = false */)
{
	if (!player) {
		return false;
	}

	if (player->getParty() != this && leader != player) {
		return false;
	}

	bool canRemove = tfs::events::party::onLeave(this, player);
	if (!forceRemove && !canRemove) {
		return false;
	}

	bool missingLeader = false;
	if (leader == player) {
		if (!memberList.empty()) {
			if (memberList.size() == 1 && inviteList.empty()) {
				missingLeader = true;
			} else {
				passPartyLeadership(memberList.front(), true);
			}
		} else {
			missingLeader = true;
		}
	}

	// since we already passed the leadership, we remove the player from the list
	auto it = std::find(memberList.begin(), memberList.end(), player);
	if (it != memberList.end()) {
		memberList.erase(it);
	}

	player->setParty(nullptr);
	player->sendClosePrivate(CHANNEL_PARTY);
	g_game.updatePlayerShield(player);

	for (const auto& member : memberList) {
		member->sendCreatureSkull(player);
		player->sendPlayerPartyIcons(member);
	}

	leader->sendCreatureSkull(player);
	player->sendCreatureSkull(player);
	player->sendPlayerPartyIcons(leader);

	// remove pending invitation icons from the screen
	for (const auto& invitee : inviteList) {
		player->sendCreatureShield(invitee);
	}

	player->sendTextMessage(MESSAGE_INFO_DESCR, "You have left the party.");

	updateSharedExperience();

	clearPlayerPoints(player);

	broadcastPartyMessage(MESSAGE_INFO_DESCR, fmt::format("{:s} has left the party.", player->getName()));

	if (missingLeader || empty()) {
		disband();
	}

	return true;
}

bool Party::passPartyLeadership(const std::shared_ptr<Player>& player, bool forceRemove /* = false*/)
{
	if (!player || leader == player || player->getParty() != this) {
		return false;
	}

	if (!tfs::events::party::onPassLeadership(this, player) && !forceRemove) {
		return false;
	}

	// Remove it before to broadcast the message correctly
	auto it = std::find(memberList.begin(), memberList.end(), player);
	if (it != memberList.end()) {
		memberList.erase(it);
	}

	broadcastPartyMessage(MESSAGE_INFO_DESCR, fmt::format("{:s} is now the leader of the party.", player->getName()),
	                      true);

	const auto oldLeader = leader;
	leader = player;

	memberList.insert(memberList.begin(), oldLeader);

	updateSharedExperience();

	for (const auto& member : memberList) {
		member->sendCreatureShield(oldLeader);
		member->sendCreatureShield(leader);
	}

	for (const auto& invitee : inviteList) {
		invitee->sendCreatureShield(oldLeader);
		invitee->sendCreatureShield(leader);
	}

	leader->sendCreatureShield(oldLeader);
	leader->sendCreatureShield(leader);

	player->sendTextMessage(MESSAGE_INFO_DESCR, "You are now the leader of the party.");
	return true;
}

bool Party::joinParty(const std::shared_ptr<Player>& player)
{
	// check if lua scripts allow the player to join
	if (!tfs::events::party::onJoin(this, player)) {
		return false;
	}

	// first player accepted the invitation the party gets officially formed the leader can no longer take invitations
	// from others
	if (memberList.empty()) {
		leader->clearPartyInvitations();
	}

	// add player to the party
	memberList.push_back(player);
	player->setParty(this);
	broadcastPartyMessage(MESSAGE_INFO_DESCR, fmt::format("{:s} has joined the party.", player->getName()));

	// remove player pending invitations to this and other parties
	player->clearPartyInvitations();

	// update player icon on the screen
	g_game.updatePlayerShield(player);

	// update player-member party icons
	for (const auto& member : memberList) {
		member->sendCreatureSkull(player);
		player->sendPlayerPartyIcons(member);
	}

	// update player-leader party icons
	leader->sendCreatureSkull(player);
	player->sendPlayerPartyIcons(leader);
	// update player own skull
	player->sendCreatureSkull(player);

	// show the new member who else is invited
	for (const auto& invitee : inviteList) {
		player->sendCreatureShield(invitee);
	}

	// check the party eligibility for shared experience
	updateSharedExperience();

	const std::string& leaderName = leader->getName();
	player->sendTextMessage(
	    MESSAGE_INFO_DESCR,
	    fmt::format("You have joined {:s}'{:s} party. Open the party channel to communicate with your companions.",
	                leaderName, leaderName.back() == 's' ? "" : "s"));
	return true;
}

bool Party::removeInvite(const std::shared_ptr<Player>& player, bool removeFromPlayer /* = true*/)
{
	auto it = std::find(inviteList.begin(), inviteList.end(), player);
	if (it == inviteList.end()) {
		return false;
	}

	inviteList.erase(it);

	leader->sendCreatureShield(player);
	player->sendCreatureShield(leader);

	if (removeFromPlayer) {
		player->removePartyInvitation(this);
	}

	if (empty()) {
		disband();
	}

	return true;
}

void Party::revokeInvitation(const std::shared_ptr<Player>& player)
{
	if (!tfs::events::party::onRevokeInvitation(this, player)) {
		return;
	}

	player->sendTextMessage(MESSAGE_INFO_DESCR, fmt::format("{:s} has revoked {:s} invitation.", leader->getName(),
	                                                        leader->getSex() == PLAYERSEX_FEMALE ? "her" : "his"));
	leader->sendTextMessage(MESSAGE_INFO_DESCR,
	                        fmt::format("Invitation for {:s} has been revoked.", player->getName()));
	removeInvite(player);
}

bool Party::invitePlayer(const std::shared_ptr<Player>& player)
{
	if (isPlayerInvited(player)) {
		return false;
	}

	if (empty()) {
		leader->sendTextMessage(
		    MESSAGE_INFO_DESCR,
		    fmt::format("{:s} has been invited. Open the party channel to communicate with your members.",
		                player->getName()));
		g_game.updatePlayerShield(leader);
		leader->sendCreatureSkull(leader);
	} else {
		leader->sendTextMessage(MESSAGE_INFO_DESCR, fmt::format("{:s} has been invited.", player->getName()));
	}

	// add player to invite lists
	inviteList.push_back(player);
	player->addPartyInvitation(this);

	// update leader-invitee party status
	leader->sendCreatureShield(player);
	player->sendCreatureShield(leader);

	// update the invitation status for other members
	for (const auto& member : memberList) {
		member->sendCreatureShield(player);
	}

	player->sendTextMessage(MESSAGE_INFO_DESCR, fmt::format("{:s} has invited you to {:s} party.", leader->getName(),
	                                                        leader->getSex() == PLAYERSEX_FEMALE ? "her" : "his"));
	return true;
}

bool Party::isPlayerInvited(const std::shared_ptr<const Player>& player) const
{
	return std::find(inviteList.begin(), inviteList.end(), player) != inviteList.end();
}

void Party::updateAllPartyIcons()
{
	for (const auto& member : memberList) {
		for (const auto& otherMember : memberList) {
			member->sendCreatureShield(otherMember);
		}

		member->sendCreatureShield(leader);
		leader->sendCreatureShield(member);
	}
	leader->sendCreatureShield(leader);
}

void Party::broadcastPartyMessage(MessageClasses msgClass, const std::string& msg, bool sendToInvitations /*= false*/)
{
	for (const auto& member : memberList) {
		member->sendTextMessage(msgClass, msg);
	}

	leader->sendTextMessage(msgClass, msg);

	if (sendToInvitations) {
		for (const auto& invitee : inviteList) {
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
	tfs::events::party::onShareExperience(this, shareExperience);

	for (const auto& member : memberList) {
		member->onGainSharedExperience(shareExperience, source);
	}
	leader->onGainSharedExperience(shareExperience, source);
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

	uint32_t highestLevel = leader->getLevel();
	for (const auto& member : memberList) {
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
	SharedExpStatus_t leaderStatus = getMemberSharedExperienceStatus(leader);
	if (leaderStatus != SHAREDEXP_OK) {
		return leaderStatus;
	}

	for (const auto& member : memberList) {
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
		return leader->getID() == ownerId || player->getParty() == this;
	}
	return false;
}
