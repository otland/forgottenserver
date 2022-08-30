#include "../otpch.h"

#include "luaregister.h"

namespace tfs::lua {

using ModuleInit = std::function<void(LuaScriptInterface&)>;

// static auto& getModules()
// {
// 	static std::vector<std::pair<std::string_view, ModuleInit>> modules;
// 	return modules;
// };

// void registerModule(std::string_view moduleName, ModuleInit init, const std::vector<std::string_view>& dependencies)
// {
// 	auto& modules = getModules();

// 	auto hint = modules.begin();
// 	for (auto depName : dependencies) {
// 		if (auto it = find_if(hint, modules.end(), [=](auto module) { return module.first == depName; });
// 		    it != modules.end()) {
// 			hint = next(it);
// 		}
// 	}

// 	std::cout << "Registering Lua module " << moduleName << "..." << std::endl;
// 	modules.emplace(hint, moduleName, init);
// }

void importModules(LuaScriptInterface& lsi)
{
	std::cout << "Importing Lua modules..." << std::endl;

	registerStdlib(lsi);
	registerGlobals(lsi);
	registerConfigManager(lsi);

	registerAction(lsi);
	registerCombat(lsi);
	registerCondition(lsi);

	registerCreature(lsi);
	registerMonster(lsi); // requires creature
	registerNpc(lsi);     // requires creature
	registerPlayer(lsi);  // requires creature

	registerCreatureEvent(lsi);
	registerGame(lsi);
	registerGlobalEvent(lsi);
	registerGroup(lsi);
	registerGuild(lsi);
	registerHouse(lsi);

	registerItem(lsi);
	registerContainer(lsi); // requires item
	registerPodium(lsi);    // requires item
	registerTeleport(lsi);  // requires item

	registerItems(lsi);
	registerModalWindow(lsi);
	registerMonsters(lsi);
	registerMoveEvent(lsi);
	registerNetworkMessage(lsi);
	registerOutfit(lsi);
	registerParty(lsi);
	registerPosition(lsi);
	registerSpells(lsi);
	registerTalkAction(lsi);
	registerTeleport(lsi);
	registerTile(lsi);
	registerTown(lsi);
	registerVariant(lsi);
	registerVocation(lsi);
	registerWeapon(lsi);
	registerXml(lsi);
}

} // namespace tfs::lua
