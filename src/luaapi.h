#ifndef TFS_LUAAPI
#define TFS_LUAAPI

#include "luaerror.h"
#include "luavariant.h"
#include "outfit.h"

class Creature;
class Cylinder;
class InstantSpell;
class Player;
class Thing;
struct LootBlock;
struct Mount;

namespace tfs::lua {

enum LuaDataType
{
	LuaData_Unknown,

	LuaData_Item,
	LuaData_Container,
	LuaData_Teleport,
	LuaData_Podium,
	LuaData_Player,
	LuaData_Monster,
	LuaData_Npc,
	LuaData_Tile,
};

template <class T>
inline std::enable_if_t<std::is_enum_v<T>, T> getNumber(lua_State* L, int32_t arg)
{
	return static_cast<T>(static_cast<int64_t>(lua_tonumber(L, arg)));
}

template <class T>
inline std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, T> getNumber(lua_State* L, int32_t arg)
{
	double num = lua_tonumber(L, arg);
	if (num < static_cast<double>(std::numeric_limits<T>::lowest()) ||
	    num > static_cast<double>(std::numeric_limits<T>::max())) {
		reportErrorFunc(L, fmt::format("Argument {} has out-of-range value for {}: {}", arg, typeid(T).name(), num));
	}

	return static_cast<T>(num);
}

template <class T>
inline std::enable_if_t<(std::is_integral_v<T> && std::is_signed_v<T>) || std::is_floating_point_v<T>, T> getNumber(
    lua_State* L, int32_t arg)
{
	double num = lua_tonumber(L, arg);
	if (num < static_cast<double>(std::numeric_limits<T>::lowest()) ||
	    num > static_cast<double>(std::numeric_limits<T>::max())) {
		reportErrorFunc(L, fmt::format("Argument {} has out-of-range value for {}: {}", arg, typeid(T).name(), num));
	}

	return static_cast<T>(num);
}

template <class T>
inline T getNumber(lua_State* L, int32_t arg, T defaultValue)
{
	const auto parameters = lua_gettop(L);
	if (parameters == 0 || arg > parameters) {
		return defaultValue;
	}
	return getNumber<T>(L, arg);
}

bool isNumber(lua_State* L, int32_t arg);

template <class T>
inline T** getRawUserdata(lua_State* L, int32_t arg)
{
	return static_cast<T**>(lua_touserdata(L, arg));
}

template <class T>
inline T* getUserdata(lua_State* L, int32_t arg)
{
	T** userdata = getRawUserdata<T>(L, arg);
	if (!userdata) {
		return nullptr;
	}
	return *userdata;
}

template <class T>
inline void pushUserdata(lua_State* L, T* value)
{
	T** userdata = static_cast<T**>(lua_newuserdata(L, sizeof(T*)));
	*userdata = value;
}

int luaUserdataCompare(lua_State* L);

template <class T>
inline std::shared_ptr<T>& getSharedPtr(lua_State* L, int32_t arg)
{
	return *static_cast<std::shared_ptr<T>*>(lua_touserdata(L, arg));
}

template <class T>
inline void pushSharedPtr(lua_State* L, T value)
{
	new (lua_newuserdata(L, sizeof(T))) T(std::move(value));
}

bool getBoolean(lua_State* L, int32_t arg);
bool getBoolean(lua_State* L, int32_t arg, bool defaultValue);
void pushBoolean(lua_State* L, bool value);

std::string getString(lua_State* L, int32_t arg);
void pushString(lua_State* L, std::string_view value);
std::string popString(lua_State* L);

Position getPosition(lua_State* L, int32_t arg);
std::pair<Position, int32_t> getStackPosition(lua_State* L, int32_t arg);
void pushPosition(lua_State* L, const Position& position, int32_t stackpos = 0);

Outfit_t getOutfit(lua_State* L, int32_t arg);
Outfit getOutfitClass(lua_State* L, int32_t arg);
void pushOutfit(lua_State* L, const Outfit_t& outfit);
void pushOutfitClass(lua_State* L, const Outfit* outfit);

LuaVariant getVariant(lua_State* L, int32_t arg);
void pushVariant(lua_State* L, const LuaVariant& var);

template <typename T>
inline T getField(lua_State* L, int32_t arg, const std::string& key)
{
	lua_getfield(L, arg, key.c_str());
	return getNumber<T>(L, -1);
}

template <typename T, typename... Args>
inline T getField(lua_State* L, int32_t arg, const std::string& key, T&& defaultValue)
{
	lua_getfield(L, arg, key.c_str());
	return getNumber<T>(L, -1, std::forward<T>(defaultValue));
}

std::string getFieldString(lua_State* L, int32_t arg, const std::string& key);
void setField(lua_State* L, const char* index, lua_Number value);
void setField(lua_State* L, const char* index, const std::string& value);

InstantSpell* getInstantSpell(lua_State* L, int32_t arg);
void pushInstantSpell(lua_State* L, const InstantSpell& spell);

Reflect getReflect(lua_State* L, int32_t arg);
void pushReflect(lua_State* L, const Reflect& reflect);

Thing* getThing(lua_State* L, int32_t arg);
void pushThing(lua_State* L, Thing* thing);

void pushCallback(lua_State* L, int32_t callback);
int32_t popCallback(lua_State* L);

Creature* getCreature(lua_State* L, int32_t arg);
Player* getPlayer(lua_State* L, int32_t arg);
LuaDataType getUserdataType(lua_State* L, int32_t arg);
void pushCombatDamage(lua_State* L, const CombatDamage& damage);
void pushMount(lua_State* L, const Mount* mount);
void pushLoot(lua_State* L, const std::vector<LootBlock>& lootList);
void pushCylinder(lua_State* L, Cylinder* cylinder);

} // namespace tfs::lua

#endif
