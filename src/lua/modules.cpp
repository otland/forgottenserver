#include "../otpch.h"

#include "modules.h"

#include "register.h"

namespace tfs::lua {

void importModules(LuaScriptInterface& lsi)
{
	std::cout << "Importing Lua modules..." << std::endl;

	registerStdLib(lsi);
	registerGlobals(lsi);
	registerConfigManager(lsi);
	registerDatabase(lsi);
	registerAction(lsi);
	registerCombat(lsi);
	registerCondition(lsi);
	registerThing(lsi);

	registerCreature(lsi);
	registerMonster(lsi); // requires creature
	registerNpc(lsi);     // requires creature
	registerPlayer(lsi);  // requires creature

	registerGame(lsi);
	registerGlobalEvent(lsi);
	registerGroup(lsi);
	registerGuild(lsi);
	registerHouse(lsi);

	registerItem(lsi);
	registerContainer(lsi); // requires item
	registerPodium(lsi);    // requires item
	registerTeleport(lsi);  // requires item

	registerItemType(lsi);
	registerModalWindow(lsi);
	registerMonsters(lsi);
	registerMoveEvent(lsi);
	registerNetworkMessage(lsi);
	registerOutfit(lsi);
	registerParty(lsi);
	registerPosition(lsi);
	registerSpell(lsi);
	registerTalkAction(lsi);
	registerTile(lsi);
	registerVariant(lsi);
	registerVocation(lsi);
	registerWeapon(lsi);
	registerXml(lsi);
}

} // namespace tfs::lua
