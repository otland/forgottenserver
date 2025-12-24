#include "../otpch.h"

#include "env.h"

#include "../game.h"
#include "../item.h"

extern Game g_game;

namespace tfs::lua {

static uint32_t lastResultId = 0;
static std::map<uint32_t, std::shared_ptr<DBResult>> tempResults = {};

static std::array<ScriptEnvironment, 16> scriptEnv = {};
static int32_t scriptEnvIndex = -1;

uint32_t addResult(std::shared_ptr<DBResult> result)
{
	tempResults[++lastResultId] = std::move(result);
	return lastResultId;
}

bool removeResult(uint32_t id)
{
	auto it = tempResults.find(id);
	if (it == tempResults.end()) {
		return false;
	}

	tempResults.erase(it);
	return true;
}

std::shared_ptr<DBResult> getResultByID(uint32_t id)
{
	auto it = tempResults.find(id);
	if (it == tempResults.end()) {
		return nullptr;
	}
	return it->second;
}

ScriptEnvironment::ScriptEnvironment() { resetEnv(); }

ScriptEnvironment::~ScriptEnvironment() { resetEnv(); }

void ScriptEnvironment::resetEnv()
{
	scriptId = 0;
	callbackId = 0;
	timerEvent = false;
	interface = nullptr;
	localMap.clear();
	tempResults.clear();
}

bool ScriptEnvironment::setCallbackId(int32_t callbackId, LuaScriptInterface* scriptInterface)
{
	if (this->callbackId != 0) {
		// nested callbacks are not allowed
		if (interface) {
			reportError(interface->getLuaState(), "Nested callbacks!");
		}
		return false;
	}

	this->callbackId = callbackId;
	interface = scriptInterface;
	return true;
}

uint32_t ScriptEnvironment::addThing(const std::shared_ptr<Thing>& thing)
{
	if (!thing || thing->isRemoved()) {
		return 0;
	}

	if (const auto& creature = thing->asCreature()) {
		return creature->getID();
	}

	const auto& item = thing->asItem();
	if (item && item->hasAttribute(ITEM_ATTRIBUTE_UNIQUEID)) {
		return item->getUniqueId();
	}

	for (auto&& [uid, localItem] : localMap | std::views::as_const) {
		if (localItem == item) {
			return uid;
		}
	}

	localMap[++lastUID] = item;
	return lastUID;
}

void ScriptEnvironment::insertItem(uint32_t uid, const std::shared_ptr<Item>& item)
{
	auto result = localMap.emplace(uid, item);
	if (!result.second) {
		std::cout << "\nLua Script Error: Thing uid already taken.";
	}
}

std::shared_ptr<Thing> ScriptEnvironment::getThingByUID(uint32_t uid)
{
	if (uid >= CREATURE_ID_MIN) {
		return g_game.getCreatureByID(uid);
	}

	if (uid <= std::numeric_limits<uint16_t>::max()) {
		if (const auto& item = g_game.getUniqueItem(uid)) {
			if (!item->isRemoved()) {
				return item;
			}
		}
		return nullptr;
	}

	auto it = localMap.find(uid);
	if (it != localMap.end()) {
		const auto& item = it->second;
		if (!item->isRemoved()) {
			return item;
		}
	}
	return nullptr;
}

std::shared_ptr<Item> ScriptEnvironment::getItemByUID(uint32_t uid)
{
	const auto& thing = getThingByUID(uid);
	if (!thing) {
		return nullptr;
	}
	return thing->asItem();
}

std::shared_ptr<Container> ScriptEnvironment::getContainerByUID(uint32_t uid)
{
	const auto& item = getItemByUID(uid);
	if (!item) {
		return nullptr;
	}
	return item->getContainer();
}

void ScriptEnvironment::removeItemByUID(uint32_t uid)
{
	if (uid <= std::numeric_limits<uint16_t>::max()) {
		g_game.removeUniqueItem(uid);
		return;
	}

	localMap.erase(uid);
}

/// Same as lua_pcall, but adds stack trace to error strings in called function.
int protectedCall(lua_State* L, int nargs, int nresults)
{
	int error_index = lua_gettop(L) - nargs;
	lua_pushcfunction(L, luaErrorHandler);
	lua_insert(L, error_index);

	int ret = lua_pcall(L, nargs, nresults, error_index);
	lua_remove(L, error_index);
	return ret;
}

ScriptEnvironment* getScriptEnv()
{
	assert(scriptEnvIndex >= 0 && scriptEnvIndex < static_cast<int32_t>(scriptEnv.size()));
	return &scriptEnv[scriptEnvIndex];
}

bool reserveScriptEnv() { return ++scriptEnvIndex < static_cast<int32_t>(scriptEnv.size()); }

void resetScriptEnv()
{
	assert(scriptEnvIndex >= 0);
	scriptEnv[scriptEnvIndex--].resetEnv();
}

} // namespace tfs::lua
