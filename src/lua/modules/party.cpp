#include "../../otpch.h"

#include "../../party.h"

#include "../../game.h"
#include "../../player.h"
#include "../api.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

extern Game g_game;

namespace {

int luaPartyCreate(lua_State* L)
{
	// Party(userdata)
	const auto& player = tfs::lua::getSharedPtr<Player>(L, 2);
	if (!player) {
		lua_pushnil(L);
		return 1;
	}

	auto party = player->getParty();
	if (!party) {
		party = std::make_shared<Party>();
		party->setLeader(player);

		g_game.updatePlayerShield(player);
		player->sendCreatureSkull(player);
		tfs::lua::pushSharedPtr(L, party);
		tfs::lua::setMetatable(L, -1, "Party");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPartyDisband(lua_State* L)
{
	// party:disband()
	if (const auto& party = tfs::lua::getSharedPtr<Party>(L, 1)) {
		party->disband();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPartyGetLeader(lua_State* L)
{
	// party:getLeader()
	const auto& party = tfs::lua::getSharedPtr<Party>(L, 1);
	if (!party) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& leader = party->getLeader()) {
		tfs::lua::pushSharedPtr(L, leader);
		tfs::lua::setMetatable(L, -1, "Player");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPartySetLeader(lua_State* L)
{
	// party:setLeader(player)
	const auto& player = tfs::lua::getPlayer(L, 2);
	const auto& party = tfs::lua::getSharedPtr<Party>(L, 1);
	if (party && player) {
		tfs::lua::pushBoolean(L, party->passPartyLeadership(player, true));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPartyGetMembers(lua_State* L)
{
	// party:getMembers()
	const auto& party = tfs::lua::getSharedPtr<Party>(L, 1);
	if (!party) {
		lua_pushnil(L);
		return 1;
	}

	const auto& members = party->getMembers() | tfs::views::lock_weak_ptrs | std::ranges::to<std::vector>();
	lua_createtable(L, members.size(), 0);

	int index = 0;
	for (const auto& player : members) {
		tfs::lua::pushSharedPtr(L, player);
		tfs::lua::setMetatable(L, -1, "Player");
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int luaPartyGetMemberCount(lua_State* L)
{
	// party:getMemberCount()
	if (const auto& party = tfs::lua::getSharedPtr<Party>(L, 1)) {
		tfs::lua::pushNumber(
		    L, std::ranges::count_if(party->getMembers(), [](const auto& member) { return !member.expired(); }));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPartyGetInvitees(lua_State* L)
{
	// party:getInvitees()
	const auto& party = tfs::lua::getSharedPtr<Party>(L, 1);
	if (!party) {
		lua_pushnil(L);
		return 1;
	}

	const auto& invitees = party->getInvitees() | tfs::views::lock_weak_ptrs | std::ranges::to<std::vector>();
	lua_createtable(L, invitees.size(), 0);

	int index = 0;
	for (const auto& player : invitees) {
		tfs::lua::pushSharedPtr(L, player);
		tfs::lua::setMetatable(L, -1, "Player");
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

int luaPartyGetInviteeCount(lua_State* L)
{
	// party:getInviteeCount()
	if (const auto& party = tfs::lua::getSharedPtr<Party>(L, 1)) {
		tfs::lua::pushNumber(
		    L, std::ranges::count_if(party->getInvitees(), [](const auto& invitee) { return !invitee.expired(); }));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPartyAddInvite(lua_State* L)
{
	// party:addInvite(player)
	const auto& player = tfs::lua::getPlayer(L, 2);
	const auto& party = tfs::lua::getSharedPtr<Party>(L, 1);
	if (party && player) {
		tfs::lua::pushBoolean(L, party->invitePlayer(player));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPartyRemoveInvite(lua_State* L)
{
	// party:removeInvite(player)
	const auto& player = tfs::lua::getPlayer(L, 2);
	const auto& party = tfs::lua::getSharedPtr<Party>(L, 1);
	if (party && player) {
		tfs::lua::pushBoolean(L, party->removeInvite(player));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPartyAddMember(lua_State* L)
{
	// party:addMember(player)
	const auto& player = tfs::lua::getPlayer(L, 2);
	const auto& party = tfs::lua::getSharedPtr<Party>(L, 1);
	if (party && player) {
		tfs::lua::pushBoolean(L, party->joinParty(player));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPartyRemoveMember(lua_State* L)
{
	// party:removeMember(player)
	const auto& player = tfs::lua::getPlayer(L, 2);
	const auto& party = tfs::lua::getSharedPtr<Party>(L, 1);
	if (party && player) {
		tfs::lua::pushBoolean(L, party->leaveParty(player));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPartyIsSharedExperienceActive(lua_State* L)
{
	// party:isSharedExperienceActive()
	if (const auto& party = tfs::lua::getSharedPtr<Party>(L, 1)) {
		tfs::lua::pushBoolean(L, party->isSharedExperienceActive());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPartyIsSharedExperienceEnabled(lua_State* L)
{
	// party:isSharedExperienceEnabled()
	if (const auto& party = tfs::lua::getSharedPtr<Party>(L, 1)) {
		tfs::lua::pushBoolean(L, party->isSharedExperienceEnabled());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPartyIsMemberSharingExp(lua_State* L)
{
	// party:isMemberSharingExp(player)
	const auto& player = tfs::lua::getSharedPtr<const Player>(L, 2);
	const auto& party = tfs::lua::getSharedPtr<Party>(L, 1);
	if (party && player) {
		tfs::lua::pushBoolean(L, party->getMemberSharedExperienceStatus(player) == SHAREDEXP_OK);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPartyShareExperience(lua_State* L)
{
	// party:shareExperience(experience)
	uint64_t experience = tfs::lua::getNumber<uint64_t>(L, 2);
	if (const auto& party = tfs::lua::getSharedPtr<Party>(L, 1)) {
		party->shareExperience(experience);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaPartySetSharedExperience(lua_State* L)
{
	// party:setSharedExperience(active)
	bool active = tfs::lua::getBoolean(L, 2);
	if (const auto& party = tfs::lua::getSharedPtr<Party>(L, 1)) {
		tfs::lua::pushBoolean(L, party->setSharedExperience(party->getLeader(), active));
	} else {
		lua_pushnil(L);
	}
	return 1;
}

} // namespace

void tfs::lua::registerParty(LuaScriptInterface& lsi)
{
	lsi.registerClass("Party", "", luaPartyCreate);
	lsi.registerMetaMethod("Party", "__eq", tfs::lua::luaUserdataCompare);
	lsi.registerMetaMethod("Party", "__gc", tfs::lua::luaSharedPtrDelete<Party>);

	lsi.registerMethod("Party", "disband", luaPartyDisband);

	lsi.registerMethod("Party", "getLeader", luaPartyGetLeader);
	lsi.registerMethod("Party", "setLeader", luaPartySetLeader);

	lsi.registerMethod("Party", "getMembers", luaPartyGetMembers);
	lsi.registerMethod("Party", "getMemberCount", luaPartyGetMemberCount);

	lsi.registerMethod("Party", "getInvitees", luaPartyGetInvitees);
	lsi.registerMethod("Party", "getInviteeCount", luaPartyGetInviteeCount);

	lsi.registerMethod("Party", "addInvite", luaPartyAddInvite);
	lsi.registerMethod("Party", "removeInvite", luaPartyRemoveInvite);

	lsi.registerMethod("Party", "addMember", luaPartyAddMember);
	lsi.registerMethod("Party", "removeMember", luaPartyRemoveMember);

	lsi.registerMethod("Party", "isSharedExperienceActive", luaPartyIsSharedExperienceActive);
	lsi.registerMethod("Party", "isSharedExperienceEnabled", luaPartyIsSharedExperienceEnabled);
	lsi.registerMethod("Party", "isMemberSharingExp", luaPartyIsMemberSharingExp);
	lsi.registerMethod("Party", "shareExperience", luaPartyShareExperience);
	lsi.registerMethod("Party", "setSharedExperience", luaPartySetSharedExperience);
}
