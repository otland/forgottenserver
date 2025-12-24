#include "../../otpch.h"

#include "../../npc.h"

#include "../../game.h"
#include "../api.h"
#include "../env.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

extern Game g_game;

namespace {

int luaNpcCreate(lua_State* L)
{
	// Npc([id or name or userdata])
	std::shared_ptr<Npc> npc = nullptr;
	if (lua_gettop(L) >= 2) {
		if (tfs::lua::isNumber(L, 2)) {
			npc = g_game.getNpcByID(tfs::lua::getNumber<uint32_t>(L, 2));
		} else if (lua_isstring(L, 2)) {
			npc = g_game.getNpcByName(tfs::lua::getString(L, 2));
		} else if (lua_isuserdata(L, 2)) {
			if (tfs::lua::getUserdataType(L, 2) != tfs::lua::LuaData_Npc) {
				lua_pushnil(L);
				return 1;
			}
			npc = tfs::lua::getSharedPtr<Npc>(L, 2);
		}
	} else {
		npc = tfs::lua::getScriptEnv()->getNpc();
	}

	if (npc) {
		tfs::lua::pushSharedPtr(L, npc);
		tfs::lua::setMetatable(L, -1, "Npc");
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaNpcIsNpc(lua_State* L)
{
	// npc:isNpc()
	if (const auto& creature = tfs::lua::getCreature(L, 1)) {
		tfs::lua::pushBoolean(L, creature->asNpc() != nullptr);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaNpcSetMasterPos(lua_State* L)
{
	// npc:setMasterPos(pos[, radius])
	const auto& npc = tfs::lua::getSharedPtr<Npc>(L, 1);
	if (!npc) {
		lua_pushnil(L);
		return 1;
	}

	const Position& pos = tfs::lua::getPosition(L, 2);
	int32_t radius = tfs::lua::getNumber<int32_t>(L, 3, 1);
	npc->setMasterPos(pos, radius);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaNpcGetSpeechBubble(lua_State* L)
{
	// npc:getSpeechBubble()
	if (const auto& npc = tfs::lua::getSharedPtr<Npc>(L, 1)) {
		tfs::lua::pushNumber(L, npc->getSpeechBubble());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaNpcSetSpeechBubble(lua_State* L)
{
	// npc:setSpeechBubble(speechBubble)
	const auto& npc = tfs::lua::getSharedPtr<Npc>(L, 1);
	if (!npc) {
		lua_pushnil(L);
		return 1;
	}

	if (!tfs::lua::isNumber(L, 2)) {
		lua_pushnil(L);
		return 1;
	}

	uint8_t speechBubble = tfs::lua::getNumber<uint8_t>(L, 2);
	if (speechBubble > SPEECHBUBBLE_LAST) {
		lua_pushnil(L);
	} else {
		npc->setSpeechBubble(speechBubble);

		// update creature speech bubble
		g_game.updateKnownCreature(npc);
		tfs::lua::pushBoolean(L, true);
	}
	return 1;
}

int luaNpcGetSpectators(lua_State* L)
{
	// npc:getSpectators()
	const auto& npc = tfs::lua::getSharedPtr<Npc>(L, 1);
	if (!npc) {
		lua_pushnil(L);
		return 1;
	}

	const auto& spectators = npc->getSpectators();
	lua_createtable(L, spectators.size(), 0);

	int index = 0;
	for (const auto& spectatorPlayer : npc->getSpectators()) {
		tfs::lua::pushSharedPtr(L, spectatorPlayer);
		tfs::lua::setMetatable(L, -1, "Player");
		lua_rawseti(L, -2, ++index);
	}
	return 1;
}

} // namespace

void tfs::lua::registerNpc(LuaScriptInterface& lsi)
{
	registerEnum(lsi, SPEECHBUBBLE_NONE);
	registerEnum(lsi, SPEECHBUBBLE_NORMAL);
	registerEnum(lsi, SPEECHBUBBLE_TRADE);
	registerEnum(lsi, SPEECHBUBBLE_QUEST);
	registerEnum(lsi, SPEECHBUBBLE_COMPASS);
	registerEnum(lsi, SPEECHBUBBLE_NORMAL2);
	registerEnum(lsi, SPEECHBUBBLE_NORMAL3);
	registerEnum(lsi, SPEECHBUBBLE_HIRELING);

	lsi.registerClass("Npc", "Creature", luaNpcCreate);
	lsi.registerMetaMethod("Npc", "__eq", tfs::lua::luaUserdataCompare);
	lsi.registerMetaMethod("Npc", "__gc", tfs::lua::luaSharedPtrDelete<Npc>);

	lsi.registerMethod("Npc", "isNpc", luaNpcIsNpc);

	lsi.registerMethod("Npc", "setMasterPos", luaNpcSetMasterPos);

	lsi.registerMethod("Npc", "getSpeechBubble", luaNpcGetSpeechBubble);
	lsi.registerMethod("Npc", "setSpeechBubble", luaNpcSetSpeechBubble);

	lsi.registerMethod("Npc", "getSpectators", luaNpcGetSpectators);
}
