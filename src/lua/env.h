#pragma once

class Container;
class DBResult;
class Item;
class LuaScriptInterface;
class LuaVariant;
class Npc;
class Thing;

namespace tfs::lua {

uint32_t addResult(std::shared_ptr<DBResult> result);
bool removeResult(uint32_t id);
std::shared_ptr<DBResult> getResultByID(uint32_t id);

class ScriptEnvironment
{
public:
	ScriptEnvironment();
	~ScriptEnvironment();

	// non-copyable
	ScriptEnvironment(const ScriptEnvironment&) = delete;
	ScriptEnvironment& operator=(const ScriptEnvironment&) = delete;

	void resetEnv();

	void setScriptId(int32_t scriptId, LuaScriptInterface* scriptInterface)
	{
		this->scriptId = scriptId;
		interface = scriptInterface;
	}
	bool setCallbackId(int32_t callbackId, LuaScriptInterface* scriptInterface);

	int32_t getScriptId() const { return scriptId; }
	LuaScriptInterface* getScriptInterface() { return interface; }

	void setTimerEvent() { timerEvent = true; }

	auto getEventInfo() const { return std::make_tuple(scriptId, interface, callbackId, timerEvent); }

	uint32_t addThing(const std::shared_ptr<Thing>& thing);
	void insertItem(uint32_t uid, const std::shared_ptr<Item>& item);

	void setNpc(std::shared_ptr<Npc> npc) { curNpc = std::move(npc); }
	std::shared_ptr<Npc> getNpc() const { return curNpc; }

	std::shared_ptr<Thing> getThingByUID(uint32_t uid);
	std::shared_ptr<Item> getItemByUID(uint32_t uid);
	std::shared_ptr<Container> getContainerByUID(uint32_t uid);
	void removeItemByUID(uint32_t uid);

private:
	LuaScriptInterface* interface;

	// for npc scripts
	std::shared_ptr<Npc> curNpc = nullptr;

	// local item map
	std::unordered_map<uint32_t, std::shared_ptr<Item>> localMap;
	uint32_t lastUID = std::numeric_limits<uint16_t>::max();

	// script file id
	int32_t scriptId;
	int32_t callbackId;
	bool timerEvent;
};

int protectedCall(lua_State* L, int nargs, int nresults);
ScriptEnvironment* getScriptEnv();
bool reserveScriptEnv();
void resetScriptEnv();

} // namespace tfs::lua
