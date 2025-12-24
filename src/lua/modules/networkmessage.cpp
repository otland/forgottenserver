#include "../../otpch.h"

#include "../../networkmessage.h"

#include "../../item.h"
#include "../../player.h"
#include "../api.h"
#include "../meta.h"
#include "../register.h"
#include "../script.h"

namespace {

int luaNetworkMessageCreate(lua_State* L)
{
	// NetworkMessage()
	tfs::lua::pushUserdata(L, new NetworkMessage);
	tfs::lua::setMetatable(L, -1, "NetworkMessage");
	return 1;
}

int luaNetworkMessageDelete(lua_State* L)
{
	NetworkMessage** messagePtr = tfs::lua::getRawUserdata<NetworkMessage>(L, 1);
	if (messagePtr && *messagePtr) {
		delete *messagePtr;
		*messagePtr = nullptr;
	}
	return 0;
}

int luaNetworkMessageGetByte(lua_State* L)
{
	// networkMessage:getByte()
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		tfs::lua::pushNumber(L, message->getByte());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaNetworkMessageGetU16(lua_State* L)
{
	// networkMessage:getU16()
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		tfs::lua::pushNumber(L, message->get<uint16_t>());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaNetworkMessageGetU32(lua_State* L)
{
	// networkMessage:getU32()
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		tfs::lua::pushNumber(L, message->get<uint32_t>());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaNetworkMessageGetU64(lua_State* L)
{
	// networkMessage:getU64()
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		tfs::lua::pushNumber(L, message->get<uint64_t>());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaNetworkMessageGetString(lua_State* L)
{
	// networkMessage:getString()
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		tfs::lua::pushString(L, message->getString());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaNetworkMessageGetPosition(lua_State* L)
{
	// networkMessage:getPosition()
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		tfs::lua::pushPosition(L, message->getPosition());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaNetworkMessageAddByte(lua_State* L)
{
	// networkMessage:addByte(number)
	uint8_t number = tfs::lua::getNumber<uint8_t>(L, 2);
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->addByte(number);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaNetworkMessageAddU16(lua_State* L)
{
	// networkMessage:addU16(number)
	uint16_t number = tfs::lua::getNumber<uint16_t>(L, 2);
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->add<uint16_t>(number);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaNetworkMessageAddU32(lua_State* L)
{
	// networkMessage:addU32(number)
	uint32_t number = tfs::lua::getNumber<uint32_t>(L, 2);
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->add<uint32_t>(number);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaNetworkMessageAddU64(lua_State* L)
{
	// networkMessage:addU64(number)
	uint64_t number = tfs::lua::getNumber<uint64_t>(L, 2);
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->add<uint64_t>(number);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaNetworkMessageAddString(lua_State* L)
{
	// networkMessage:addString(string)
	const std::string& string = tfs::lua::getString(L, 2);
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->addString(string);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaNetworkMessageAddPosition(lua_State* L)
{
	// networkMessage:addPosition(position)
	const Position& position = tfs::lua::getPosition(L, 2);
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->addPosition(position);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaNetworkMessageAddDouble(lua_State* L)
{
	// networkMessage:addDouble(number)
	double number = tfs::lua::getNumber<double>(L, 2);
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->addDouble(number);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaNetworkMessageAddItem(lua_State* L)
{
	// networkMessage:addItem(item)
	const auto& item = tfs::lua::getSharedPtr<Item>(L, 2);
	if (!item) {
		tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_ITEM_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->addItem(item);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaNetworkMessageAddItemId(lua_State* L)
{
	// networkMessage:addItemId(itemId)
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (!message) {
		lua_pushnil(L);
		return 1;
	}

	uint16_t itemId;
	if (tfs::lua::isNumber(L, 2)) {
		itemId = tfs::lua::getNumber<uint16_t>(L, 2);
	} else {
		itemId = Item::items.getItemIdByName(tfs::lua::getString(L, 2));
		if (itemId == 0) {
			lua_pushnil(L);
			return 1;
		}
	}

	message->addItemId(itemId);
	tfs::lua::pushBoolean(L, true);
	return 1;
}

int luaNetworkMessageReset(lua_State* L)
{
	// networkMessage:reset()
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->reset();
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaNetworkMessageLength(lua_State* L)
{
	// networkMessage:len()
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		tfs::lua::pushNumber(L, message->getLength());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaNetworkMessageSkipBytes(lua_State* L)
{
	// networkMessage:skipBytes(number)
	int16_t number = tfs::lua::getNumber<int16_t>(L, 2);
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (message) {
		message->skipBytes(number);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int luaNetworkMessageSendToPlayer(lua_State* L)
{
	// networkMessage:sendToPlayer(player)
	NetworkMessage* message = tfs::lua::getUserdata<NetworkMessage>(L, 1);
	if (!message) {
		lua_pushnil(L);
		return 1;
	}

	if (const auto& player = tfs::lua::getPlayer(L, 2)) {
		player->sendNetworkMessage(*message);
		tfs::lua::pushBoolean(L, true);
	} else {
		tfs::lua::reportError(L, tfs::lua::getErrorDesc(tfs::lua::LUA_ERROR_PLAYER_NOT_FOUND));
		lua_pushnil(L);
	}
	return 1;
}

} // namespace

void tfs::lua::registerNetworkMessage(LuaScriptInterface& lsi)
{
	lsi.registerClass("NetworkMessage", "", luaNetworkMessageCreate);
	lsi.registerMetaMethod("NetworkMessage", "__eq", tfs::lua::luaUserdataCompare);
	lsi.registerMetaMethod("NetworkMessage", "__gc", luaNetworkMessageDelete);
	lsi.registerMethod("NetworkMessage", "delete", luaNetworkMessageDelete);

	lsi.registerMethod("NetworkMessage", "getByte", luaNetworkMessageGetByte);
	lsi.registerMethod("NetworkMessage", "getU16", luaNetworkMessageGetU16);
	lsi.registerMethod("NetworkMessage", "getU32", luaNetworkMessageGetU32);
	lsi.registerMethod("NetworkMessage", "getU64", luaNetworkMessageGetU64);
	lsi.registerMethod("NetworkMessage", "getString", luaNetworkMessageGetString);
	lsi.registerMethod("NetworkMessage", "getPosition", luaNetworkMessageGetPosition);

	lsi.registerMethod("NetworkMessage", "addByte", luaNetworkMessageAddByte);
	lsi.registerMethod("NetworkMessage", "addU16", luaNetworkMessageAddU16);
	lsi.registerMethod("NetworkMessage", "addU32", luaNetworkMessageAddU32);
	lsi.registerMethod("NetworkMessage", "addU64", luaNetworkMessageAddU64);
	lsi.registerMethod("NetworkMessage", "addString", luaNetworkMessageAddString);
	lsi.registerMethod("NetworkMessage", "addPosition", luaNetworkMessageAddPosition);
	lsi.registerMethod("NetworkMessage", "addDouble", luaNetworkMessageAddDouble);
	lsi.registerMethod("NetworkMessage", "addItem", luaNetworkMessageAddItem);
	lsi.registerMethod("NetworkMessage", "addItemId", luaNetworkMessageAddItemId);

	lsi.registerMethod("NetworkMessage", "reset", luaNetworkMessageReset);
	lsi.registerMethod("NetworkMessage", "len", luaNetworkMessageLength);
	lsi.registerMethod("NetworkMessage", "skipBytes", luaNetworkMessageSkipBytes);
	lsi.registerMethod("NetworkMessage", "sendToPlayer", luaNetworkMessageSendToPlayer);
}
