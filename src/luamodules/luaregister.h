#ifndef TFS_LUAREGISTER
#define TFS_LUAREGISTER

class LuaScriptInterface;

namespace tfs::lua {

void importModules(LuaScriptInterface& lsi);

void registerGlobals(LuaScriptInterface& lsi);
void registerAction(LuaScriptInterface& lsi);
void registerCombat(LuaScriptInterface& lsi);
void registerCondition(LuaScriptInterface& lsi);
void registerConfigManager(LuaScriptInterface& lsi);
void registerContainer(LuaScriptInterface& lsi);
void registerCreature(LuaScriptInterface& lsi);
void registerCreatureEvent(LuaScriptInterface& lsi);
void registerGame(LuaScriptInterface& lsi);
void registerGlobalEvent(LuaScriptInterface& lsi);
void registerGroup(LuaScriptInterface& lsi);
void registerGuild(LuaScriptInterface& lsi);
void registerHouse(LuaScriptInterface& lsi);
void registerItem(LuaScriptInterface& lsi);
void registerItems(LuaScriptInterface& lsi);
void registerModalWindow(LuaScriptInterface& lsi);
void registerMonster(LuaScriptInterface& lsi);
void registerMonsters(LuaScriptInterface& lsi);
void registerMoveEvent(LuaScriptInterface& lsi);
void registerNetworkMessage(LuaScriptInterface& lsi);
void registerNpc(LuaScriptInterface& lsi);
void registerOutfit(LuaScriptInterface& lsi);
void registerParty(LuaScriptInterface& lsi);
void registerPlayer(LuaScriptInterface& lsi);
void registerPodium(LuaScriptInterface& lsi);
void registerPosition(LuaScriptInterface& lsi);
void registerSpells(LuaScriptInterface& lsi);
void registerStdlib(LuaScriptInterface& lsi);
void registerTalkAction(LuaScriptInterface& lsi);
void registerTeleport(LuaScriptInterface& lsi);
void registerTile(LuaScriptInterface& lsi);
void registerTown(LuaScriptInterface& lsi);
void registerVariant(LuaScriptInterface& lsi);
void registerVocation(LuaScriptInterface& lsi);
void registerWeapon(LuaScriptInterface& lsi);
void registerXml(LuaScriptInterface& lsi);

} // namespace tfs::lua

#define registerEnum(lsi, value) \
	{ \
		std::string_view enumName = #value; \
		lsi.registerGlobalVariable(enumName.substr(enumName.find_last_of(':') + 1), value); \
	}

#endif
