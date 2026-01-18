#include "../otpch.h"

#include "api.h"

#include "../game.h"
#include "../monster.h"
#include "../monsters.h"
#include "../mounts.h"
#include "../npc.h"
#include "../podium.h"
#include "../spells.h"
#include "../teleport.h"
#include "env.h"
#include "meta.h"

#include <string>

extern Game g_game;
extern Spells* g_spells;

namespace tfs::lua {

bool isNumber(lua_State* L, int32_t arg) { return lua_type(L, arg) == LUA_TNUMBER; }

int luaUserdataCompare(lua_State* L)
{
	// userdataA == userdataB
	pushBoolean(L, getUserdata<void>(L, 1) == getUserdata<void>(L, 2));
	return 1;
}

bool getBoolean(lua_State* L, int32_t arg) { return lua_toboolean(L, arg) != 0; }
bool getBoolean(lua_State* L, int32_t arg, bool defaultValue)
{
	if (lua_isboolean(L, arg) == 0) {
		return defaultValue;
	}
	return lua_toboolean(L, arg) != 0;
}

void setField(lua_State* L, const char* index, lua_Number value)
{
	pushNumber(L, value);
	lua_setfield(L, -2, index);
}

void setField(lua_State* L, const char* index, std::string_view value)
{
	pushString(L, value);
	lua_setfield(L, -2, index);
}

void pushVariant(lua_State* L, const LuaVariant& var)
{
	lua_createtable(L, 0, 2);
	setField(L, "type", var.type());
	switch (var.type()) {
		case VARIANT_NUMBER:
			setField(L, "number", var.getNumber());
			break;
		case VARIANT_STRING:
			setField(L, "string", var.getString());
			break;
		case VARIANT_TARGETPOSITION:
			pushPosition(L, var.getTargetPosition());
			lua_setfield(L, -2, "pos");
			break;
		case VARIANT_POSITION: {
			pushPosition(L, var.getPosition());
			lua_setfield(L, -2, "pos");
			break;
		}
		default:
			break;
	}
	setMetatable(L, -1, "Variant");
}

void pushThing(lua_State* L, const std::shared_ptr<Thing>& thing)
{
	if (!thing) {
		lua_createtable(L, 0, 4);
		setField(L, "uid", 0);
		setField(L, "itemid", 0);
		setField(L, "actionid", 0);
		setField(L, "type", 0);
		return;
	}

	if (const auto& item = thing->asItem()) {
		pushSharedPtr(L, item);
		setItemMetatable(L, -1, item);
	} else if (const auto& creature = thing->asCreature()) {
		pushSharedPtr(L, creature);
		setCreatureMetatable(L, -1, creature);
	} else if (const auto& tile = thing->asTile()) {
		pushSharedPtr(L, tile);
		setMetatable(L, -1, "Tile");
	} else {
		lua_pushnil(L);
	}
}

void pushString(lua_State* L, std::string_view value) { lua_pushlstring(L, value.data(), value.size()); }
void pushCallback(lua_State* L, int32_t callback) { lua_rawgeti(L, LUA_REGISTRYINDEX, callback); }

std::string popString(lua_State* L)
{
	if (lua_gettop(L) == 0) {
		return std::string();
	}

	std::string str = getString(L, -1);
	lua_pop(L, 1);
	return str;
}

int32_t popCallback(lua_State* L) { return luaL_ref(L, LUA_REGISTRYINDEX); }

// Get
std::string getString(lua_State* L, int32_t arg)
{
	size_t len;
	const char* data = lua_tolstring(L, arg, &len);
	if (!data || len == 0) {
		return {};
	}
	return {data, len};
}

Position getPosition(lua_State* L, int32_t arg, int32_t& stackpos)
{
	Position position{
	    getField<uint16_t>(L, arg, "x"),
	    getField<uint16_t>(L, arg, "y"),
	    getField<uint8_t>(L, arg, "z"),
	};

	lua_getfield(L, arg, "stackpos");
	if (lua_isnil(L, -1) == 1) {
		stackpos = 0;
	} else {
		stackpos = getNumber<int32_t>(L, -1);
	}

	lua_pop(L, 4);
	return position;
}

Position getPosition(lua_State* L, int32_t arg)
{
	Position position{
	    getField<uint16_t>(L, arg, "x"),
	    getField<uint16_t>(L, arg, "y"),
	    getField<uint8_t>(L, arg, "z"),
	};

	lua_pop(L, 3);
	return position;
}

Outfit_t getOutfit(lua_State* L, int32_t arg)
{
	Outfit_t outfit{
	    .lookType = getField<uint16_t>(L, arg, "lookType"),
	    .lookTypeEx = getField<uint16_t>(L, arg, "lookTypeEx"),

	    .lookHead = getField<uint8_t>(L, arg, "lookHead"),
	    .lookBody = getField<uint8_t>(L, arg, "lookBody"),
	    .lookLegs = getField<uint8_t>(L, arg, "lookLegs"),
	    .lookFeet = getField<uint8_t>(L, arg, "lookFeet"),
	    .lookAddons = getField<uint8_t>(L, arg, "lookAddons"),

	    .lookMount = getField<uint16_t>(L, arg, "lookMount"),
	    .lookMountHead = getField<uint8_t>(L, arg, "lookMountHead"),
	    .lookMountBody = getField<uint8_t>(L, arg, "lookMountBody"),
	    .lookMountLegs = getField<uint8_t>(L, arg, "lookMountLegs"),
	    .lookMountFeet = getField<uint8_t>(L, arg, "lookMountFeet"),
	};

	lua_pop(L, 12);
	return outfit;
}

Outfit getOutfitClass(lua_State* L, int32_t arg)
{
	Outfit outfit{
	    .name = getFieldString(L, arg, "name"),
	    .lookType = getField<uint16_t>(L, arg, "lookType"),
	    .premium = getField<uint8_t>(L, arg, "premium") == 1,
	    .unlocked = getField<uint8_t>(L, arg, "unlocked") == 1,
	};

	lua_pop(L, 4);
	return outfit;
}

LuaVariant getVariant(lua_State* L, int32_t arg)
{
	LuaVariant var;
	switch (getField<LuaVariantType_t>(L, arg, "type")) {
		case VARIANT_NUMBER: {
			var.setNumber(getField<uint32_t>(L, arg, "number"));
			lua_pop(L, 2);
			break;
		}

		case VARIANT_STRING: {
			var.setString(getFieldString(L, arg, "string"));
			lua_pop(L, 2);
			break;
		}

		case VARIANT_POSITION:
			lua_getfield(L, arg, "pos");
			var.setPosition(getPosition(L, lua_gettop(L)));
			lua_pop(L, 2);
			break;

		case VARIANT_TARGETPOSITION: {
			lua_getfield(L, arg, "pos");
			var.setTargetPosition(getPosition(L, lua_gettop(L)));
			lua_pop(L, 2);
			break;
		}

		default: {
			var = {};
			lua_pop(L, 1);
			break;
		}
	}
	return var;
}

std::shared_ptr<Thing> getThing(lua_State* L, int32_t arg)
{
	std::shared_ptr<Thing> thing = nullptr;
	if (lua_getmetatable(L, arg) != 0) {
		lua_rawgeti(L, -1, 't');
		switch (getNumber<uint32_t>(L, -1)) {
			case LuaData_Item:
				thing = std::static_pointer_cast<Thing>(getSharedPtr<Item>(L, arg));
				break;
			case LuaData_Container:
				thing = std::static_pointer_cast<Thing>(getSharedPtr<Container>(L, arg));
				break;
			case LuaData_Teleport:
				thing = std::static_pointer_cast<Thing>(getSharedPtr<Teleport>(L, arg));
				break;
			case LuaData_Podium:
				thing = std::static_pointer_cast<Thing>(getSharedPtr<Podium>(L, arg));
				break;
			case LuaData_Player:
				thing = std::static_pointer_cast<Thing>(getSharedPtr<Player>(L, arg));
				break;
			case LuaData_Monster:
				thing = std::static_pointer_cast<Thing>(getSharedPtr<Monster>(L, arg));
				break;
			case LuaData_Npc:
				thing = std::static_pointer_cast<Thing>(getSharedPtr<Npc>(L, arg));
				break;
		}
		lua_pop(L, 2);
	} else {
		thing = getScriptEnv()->getThingByUID(getNumber<uint32_t>(L, arg));
	}
	return thing;
}

std::shared_ptr<Creature> getCreature(lua_State* L, int32_t arg)
{
	if (lua_isuserdata(L, arg)) {
		return getSharedPtr<Creature>(L, arg);
	} else if (lua_isnil(L, arg)) {
		return nullptr;
	}
	return g_game.getCreatureByID(getNumber<uint32_t>(L, arg));
}

std::shared_ptr<Player> getPlayer(lua_State* L, int32_t arg)
{
	if (lua_isuserdata(L, arg)) {
		return getSharedPtr<Player>(L, arg);
	}
	return g_game.getPlayerByID(getNumber<uint32_t>(L, arg));
}

std::string getFieldString(lua_State* L, int32_t arg, const std::string_view key)
{
	lua_getfield(L, arg, key.data());
	return getString(L, -1);
}

LuaDataType getUserdataType(lua_State* L, int32_t arg)
{
	if (lua_getmetatable(L, arg) == 0) {
		return LuaData_Unknown;
	}
	lua_rawgeti(L, -1, 't');

	LuaDataType type = getNumber<LuaDataType>(L, -1);
	lua_pop(L, 2);

	return type;
}

void pushBoolean(lua_State* L, bool value) { lua_pushboolean(L, value ? 1 : 0); }

void pushSpell(lua_State* L, const Spell& spell)
{
	lua_createtable(L, 0, 5);
	setField(L, "name", spell.getName());
	setField(L, "level", spell.getLevel());
	setField(L, "mlevel", spell.getMagicLevel());
	setField(L, "mana", spell.getMana());
	setField(L, "manapercent", spell.getManaPercent());
	setMetatable(L, -1, "Spell");
}

void pushPosition(lua_State* L, const Position& position, int32_t stackpos /* = 0*/)
{
	lua_createtable(L, 0, 4);
	setField(L, "x", position.x);
	setField(L, "y", position.y);
	setField(L, "z", position.z);
	setField(L, "stackpos", stackpos);
	setMetatable(L, -1, "Position");
}

void pushOutfit(lua_State* L, const Outfit_t& outfit)
{
	lua_createtable(L, 0, 12);
	setField(L, "lookType", outfit.lookType);
	setField(L, "lookTypeEx", outfit.lookTypeEx);
	setField(L, "lookHead", outfit.lookHead);
	setField(L, "lookBody", outfit.lookBody);
	setField(L, "lookLegs", outfit.lookLegs);
	setField(L, "lookFeet", outfit.lookFeet);
	setField(L, "lookAddons", outfit.lookAddons);
	setField(L, "lookMount", outfit.lookMount);
	setField(L, "lookMountHead", outfit.lookMountHead);
	setField(L, "lookMountBody", outfit.lookMountBody);
	setField(L, "lookMountLegs", outfit.lookMountLegs);
	setField(L, "lookMountFeet", outfit.lookMountFeet);
}

void pushOutfit(lua_State* L, const Outfit* outfit)
{
	lua_createtable(L, 0, 4);
	setField(L, "lookType", outfit->lookType);
	setField(L, "name", outfit->name);
	setField(L, "premium", outfit->premium);
	setField(L, "unlocked", outfit->unlocked);
	setMetatable(L, -1, "Outfit");
}

void pushLoot(lua_State* L, const std::vector<LootBlock>& lootList)
{
	lua_createtable(L, lootList.size(), 0);

	int index = 0;
	for (const auto& lootBlock : lootList) {
		lua_createtable(L, 0, 7);

		setField(L, "itemId", lootBlock.id);
		setField(L, "chance", lootBlock.chance);
		setField(L, "subType", lootBlock.subType);
		setField(L, "maxCount", lootBlock.countmax);
		setField(L, "actionId", lootBlock.actionId);
		setField(L, "text", lootBlock.text);

		pushLoot(L, lootBlock.childLoot);
		lua_setfield(L, -2, "childLoot");

		lua_rawseti(L, -2, ++index);
	}
}

void pushTown(lua_State* L, const Town& town)
{
	lua_createtable(L, 0, 3);
	setField(L, "id", town.id);
	setField(L, "name", town.name);
	pushPosition(L, town.templePosition);
	lua_setfield(L, -2, "templePosition");
	lua_getglobal(L, "Town");
	lua_setmetatable(L, -2);
}

void pushParty(lua_State* L, const std::shared_ptr<Party>& party)
{
	pushSharedPtr(L, party);
	setMetatable(L, -1, "Party");
}

void pushItemType(lua_State* L, const ItemType* itemType)
{
	pushUserdata(L, itemType);
	setMetatable(L, -1, "ItemType");
}

void pushNetworkMessage(lua_State* L, NetworkMessage* msg)
{
	pushUserdata(L, msg);
	setMetatable(L, -1, "NetworkMessage");
}

} // namespace tfs::lua
