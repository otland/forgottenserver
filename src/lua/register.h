#pragma once

class LuaScriptInterface;

namespace tfs::lua {

void registerAction(LuaScriptInterface& i);
void registerCombat(LuaScriptInterface& i);
void registerCondition(LuaScriptInterface& i);
void registerConfigManager(LuaScriptInterface& i);
void registerContainer(LuaScriptInterface& i);
void registerCreature(LuaScriptInterface& i);
void registerCreatureEvent(LuaScriptInterface& i);
void registerDatabase(LuaScriptInterface& i);
void registerGame(LuaScriptInterface& i);
void registerGlobalEvent(LuaScriptInterface& i);
void registerGlobals(LuaScriptInterface& i);
void registerGroup(LuaScriptInterface& i);
void registerGuild(LuaScriptInterface& i);
void registerHouse(LuaScriptInterface& i);
void registerItem(LuaScriptInterface& i);
void registerItemType(LuaScriptInterface& i);
void registerModalWindow(LuaScriptInterface& i);
void registerMonster(LuaScriptInterface& i);
void registerMonsters(LuaScriptInterface& i);
void registerMoveEvent(LuaScriptInterface& i);
void registerNetworkMessage(LuaScriptInterface& i);
void registerNpc(LuaScriptInterface& i);
void registerOutfit(LuaScriptInterface& i);
void registerParty(LuaScriptInterface& i);
void registerPlayer(LuaScriptInterface& i);
void registerPodium(LuaScriptInterface& i);
void registerPosition(LuaScriptInterface& i);
void registerSpell(LuaScriptInterface& i);
void registerStdLib(LuaScriptInterface& i);
void registerTalkAction(LuaScriptInterface& i);
void registerTeleport(LuaScriptInterface& i);
void registerThing(LuaScriptInterface& i);
void registerTile(LuaScriptInterface& i);
void registerVariant(LuaScriptInterface& i);
void registerVocation(LuaScriptInterface& i);
void registerWeapon(LuaScriptInterface& i);
void registerXml(LuaScriptInterface& i);

} // namespace tfs::lua

#define registerEnum(lsi, value) \
	{ \
		std::string_view enumName = #value; \
		lsi.registerGlobalVariable(enumName.substr(enumName.find_last_of(':') + 1), value); \
	}
