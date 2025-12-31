#pragma once

#include "../enums.h"
#include "../position.h"

class AreaCombat;
class Combat;
class Container;
class Creature;
class Spell;
class Item;
class LuaScriptInterface;
class LuaVariant;
class Npc;
class Player;
class Thing;
struct Outfit;

using Combat_ptr = std::shared_ptr<Combat>;

inline constexpr int32_t EVENT_ID_LOADING = 1;
inline constexpr int32_t EVENT_ID_USER = 1000;

struct LuaTimerEventDesc
{
	int32_t scriptId = -1;
	int32_t function = -1;
	std::vector<int32_t> parameters;
	uint32_t eventId = 0;

	LuaTimerEventDesc() = default;
	LuaTimerEventDesc(LuaTimerEventDesc&& other) = default;
};

class LuaScriptInterface
{
public:
	explicit LuaScriptInterface(std::string interfaceName);
	virtual ~LuaScriptInterface();

	// non-copyable
	LuaScriptInterface(const LuaScriptInterface&) = delete;
	LuaScriptInterface& operator=(const LuaScriptInterface&) = delete;

	virtual bool initState();
	bool reInitState();

	int32_t loadFile(const std::string& file, const std::shared_ptr<Npc>& npc = nullptr);

	const std::string& getFileById(int32_t scriptId);
	int32_t getEvent(std::string_view eventName);
	int32_t getEvent();
	int32_t getMetaEvent(const std::string& globalName, const std::string& eventName);

	const std::string& getInterfaceName() const { return interfaceName; }
	const std::string& getLastLuaError() const { return lastLuaError; }

	lua_State* getLuaState() const { return L; }

	bool pushFunction(int32_t functionId);

	bool callFunction(int params);
	void callVoidFunction(int params);

	void registerGlobalMethod(std::string_view functionName, lua_CFunction func);
	void registerGlobalVariable(std::string_view name, lua_Number value);
	void registerGlobalBoolean(std::string_view name, bool value);

	void registerTable(std::string_view tableName);
	void registerClass(std::string_view className, std::string_view baseClass, lua_CFunction newFunction = nullptr);
	void registerMetaMethod(std::string_view className, std::string_view methodName, lua_CFunction func);
	void registerMethod(std::string_view globalName, std::string_view methodName, lua_CFunction func);
	void registerVariable(std::string_view tableName, std::string_view name, lua_Number value);

protected:
	virtual bool closeState();

	void registerFunctions();

	lua_State* L = nullptr;

	int32_t eventTableRef = -1;
	int32_t runningEventId = EVENT_ID_USER;

	// script file cache
	std::map<int32_t, std::string> cacheFiles;

private:
	std::string lastLuaError;
	std::string interfaceName;
	std::string loadingFile;
};

class LuaEnvironment : public LuaScriptInterface
{
public:
	LuaEnvironment();
	~LuaEnvironment();

	// non-copyable
	LuaEnvironment(const LuaEnvironment&) = delete;
	LuaEnvironment& operator=(const LuaEnvironment&) = delete;

	bool initState() override;
	bool reInitState();
	bool closeState() override;

	LuaScriptInterface* getTestInterface();

	Combat_ptr getCombatObject(uint32_t id) const;
	Combat_ptr createCombatObject(LuaScriptInterface* interface);
	void clearCombatObjects(LuaScriptInterface* interface);

	AreaCombat* getAreaObject(uint32_t id) const;
	uint32_t createAreaObject(LuaScriptInterface* interface);
	void clearAreaObjects(LuaScriptInterface* interface);

	void executeTimerEvent(uint32_t eventIndex);

	std::unordered_map<uint32_t, LuaTimerEventDesc> timerEvents;
	uint32_t lastEventTimerId = 1;

private:
	std::unordered_map<uint32_t, Combat_ptr> combatMap;
	std::unordered_map<uint32_t, AreaCombat*> areaMap;

	std::unordered_map<LuaScriptInterface*, std::vector<uint32_t>> combatIdMap;
	std::unordered_map<LuaScriptInterface*, std::vector<uint32_t>> areaIdMap;

	LuaScriptInterface* testInterface = nullptr;

	uint32_t lastCombatId = 0;
	uint32_t lastAreaId = 0;

	friend class LuaScriptInterface;
	friend class CombatSpell;
};
