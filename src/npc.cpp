/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2014  Mark Samman <mark.samman@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "otpch.h"

#include "npc.h"
#include "game.h"
#include "tools.h"
#include "position.h"
#include "player.h"
#include "spawn.h"
#include "pugicast.h"
#include "luascript.h"

extern Game g_game;
extern LuaEnvironment g_luaEnvironment;

enum {
	EVENT_ID_LOADING = 1,
	EVENT_ID_USER = 1000,
};

uint32_t Npc::npcAutoID = 0x80000000;
NpcScriptInterface* Npc::m_scriptInterface = nullptr;

#ifdef ENABLE_SERVER_DIAGNOSTIC
uint32_t Npc::npcCount = 0;
#endif

void Npcs::reload()
{
	const std::map<uint32_t, Npc*>& npcs = g_game.getNpcs();
	for (const auto& it : npcs) {
		it.second->closeAllShopWindows();
	}

	delete Npc::m_scriptInterface;
	Npc::m_scriptInterface = nullptr;

	for (const auto& it : npcs) {
		it.second->reload();
	}
}

Npc* Npc::createNpc(const std::string& name)
{
	Npc* npc = new Npc(name);
	if (!npc->load()) {
		delete npc;
		return nullptr;
	}
	return npc;
}

Npc::Npc(const std::string& _name) :
	Creature(), m_filename("data/npc/" + _name + ".xml")
{
	loaded = false;

	masterRadius = -1;

	m_npcEventHandler = nullptr;
	reset();

#ifdef ENABLE_SERVER_DIAGNOSTIC
	npcCount++;
#endif
}

Npc::~Npc()
{
	reset();

#ifdef ENABLE_SERVER_DIAGNOSTIC
	npcCount--;
#endif
}

void Npc::addList()
{
	g_game.addNpc(this);
}

void Npc::removeList()
{
	g_game.removeNpc(this);
}

bool Npc::load()
{
	if (isLoaded()) {
		return true;
	}

	reset();

	if (!m_scriptInterface) {
		m_scriptInterface = new NpcScriptInterface();
		m_scriptInterface->loadNpcLib("data/npc/lib/npc.lua");
	}

	loaded = loadFromXml(m_filename);
	return isLoaded();
}

void Npc::reset()
{
	loaded = false;
	walkTicks = 1500;
	floorChange = false;
	attackable = false;
	ignoreHeight = true;
	focusCreature = 0;
	speechBubble = SPEECHBUBBLE_NONE;

	delete m_npcEventHandler;
	m_npcEventHandler = nullptr;

	m_parameters.clear();
	shopPlayerSet.clear();
}

void Npc::reload()
{
	reset();
	load();

	// Simulate that the creature is placed on the map again.
	if (m_npcEventHandler) {
		m_npcEventHandler->onCreatureAppear(this);
	}

	if (walkTicks > 0) {
		addEventWalk();
	}
}

bool Npc::loadFromXml(const std::string& filename)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename.c_str());
	if (!result) {
		std::cout << "[Error - Npc::loadFromXml] Failed to load " << filename << ": " << result.description() << std::endl;
		return false;
	}

	pugi::xml_node npcNode = doc.child("npc");
	if (!npcNode) {
		std::cout << "[Error - Npc::loadFromXml] Missing npc tag in " << filename << std::endl;
		return false;
	}

	name = npcNode.attribute("name").as_string();
	attackable = npcNode.attribute("attackable").as_bool();
	floorChange = npcNode.attribute("floorchange").as_bool();

	pugi::xml_attribute attr;
	if ((attr = npcNode.attribute("speed"))) {
		baseSpeed = pugi::cast<uint32_t>(attr.value());
	} else {
		baseSpeed = 100;
	}

	if ((attr = npcNode.attribute("walkinterval"))) {
		walkTicks = pugi::cast<uint32_t>(attr.value());
	}

	if ((attr = npcNode.attribute("walkradius"))) {
		masterRadius = pugi::cast<int32_t>(attr.value());
	}

	if ((attr = npcNode.attribute("ignoreheight"))) {
		ignoreHeight = attr.as_bool();
	}

	if ((attr = npcNode.attribute("speechbubble"))) {
		speechBubble = pugi::cast<uint32_t>(attr.value());
	}

	pugi::xml_node healthNode = npcNode.child("health");
	if (healthNode) {
		if ((attr = healthNode.attribute("now"))) {
			health = pugi::cast<int32_t>(attr.value());
		} else {
			health = 100;
		}

		if ((attr = healthNode.attribute("max"))) {
			healthMax = pugi::cast<int32_t>(attr.value());
		} else {
			healthMax = 100;
		}
	}

	pugi::xml_node lookNode = npcNode.child("look");
	if (lookNode) {
		pugi::xml_attribute lookTypeAttribute = lookNode.attribute("type");
		if (lookTypeAttribute) {
			defaultOutfit.lookType = pugi::cast<uint16_t>(lookTypeAttribute.value());
			defaultOutfit.lookHead = pugi::cast<uint16_t>(lookNode.attribute("head").value());
			defaultOutfit.lookBody = pugi::cast<uint16_t>(lookNode.attribute("body").value());
			defaultOutfit.lookLegs = pugi::cast<uint16_t>(lookNode.attribute("legs").value());
			defaultOutfit.lookFeet = pugi::cast<uint16_t>(lookNode.attribute("feet").value());
			defaultOutfit.lookAddons = pugi::cast<uint16_t>(lookNode.attribute("addons").value());
		} else if ((attr = lookNode.attribute("typeex"))) {
			defaultOutfit.lookTypeEx = pugi::cast<uint16_t>(attr.value());
		}
		defaultOutfit.lookMount = pugi::cast<uint16_t>(lookNode.attribute("mount").value());

		currentOutfit = defaultOutfit;
	}

	for (pugi::xml_node parameterNode = npcNode.child("parameters").first_child(); parameterNode; parameterNode = parameterNode.next_sibling()) {
		m_parameters[parameterNode.attribute("key").as_string()] = parameterNode.attribute("value").as_string();
	}

	pugi::xml_attribute scriptFile = npcNode.attribute("script");
	if (scriptFile) {
		m_npcEventHandler = new NpcScript(scriptFile.as_string(), this);
		if (!m_npcEventHandler->isLoaded()) {
			return false;
		}
	}
	return true;
}

bool Npc::canSee(const Position& pos) const
{
	if (pos.z != getPosition().z) {
		return false;
	}
	return Creature::canSee(getPosition(), pos, 3, 3);
}

std::string Npc::getDescription(int32_t) const
{
	std::string descr;
	descr.reserve(name.length() + 1);
	descr.assign(name);
	descr.push_back('.');
	return descr;
}

void Npc::onCreatureAppear(const Creature* creature, bool isLogin)
{
	Creature::onCreatureAppear(creature, isLogin);

	if (creature == this) {
		if (walkTicks > 0) {
			addEventWalk();
		}

		if (m_npcEventHandler) {
			m_npcEventHandler->onCreatureAppear(creature);
		}
	} else if (creature->getPlayer()) {
		if (m_npcEventHandler) {
			m_npcEventHandler->onCreatureAppear(creature);
		}
	}
}

void Npc::onCreatureDisappear(const Creature* creature, uint32_t stackpos, bool isLogout)
{
	Creature::onCreatureDisappear(creature, stackpos, isLogout);

	if (creature == this) {
		closeAllShopWindows();
		if (m_npcEventHandler) {
			m_npcEventHandler->onCreatureDisappear(creature);
		}
	} else if (creature->getPlayer()) {
		if (m_npcEventHandler) {
			m_npcEventHandler->onCreatureDisappear(creature);
		}
	}
}

void Npc::onCreatureMove(const Creature* creature, const Tile* newTile, const Position& newPos,
                         const Tile* oldTile, const Position& oldPos, bool teleport)
{
	Creature::onCreatureMove(creature, newTile, newPos, oldTile, oldPos, teleport);

	if (creature == this || creature->getPlayer()) {
		if (m_npcEventHandler) {
			m_npcEventHandler->onCreatureMove(creature, oldPos, newPos);
		}
	}
}

void Npc::onCreatureSay(const Creature* creature, SpeakClasses type, const std::string& text)
{
	if (creature->getID() == this->getID()) {
		return;
	}

	//only players for script events
	const Player* player = creature->getPlayer();
	if (player) {
		if (m_npcEventHandler) {
			m_npcEventHandler->onCreatureSay(player, type, text);
		}
	}
}

void Npc::onPlayerCloseChannel(const Player* player)
{
	if (m_npcEventHandler) {
		m_npcEventHandler->onPlayerCloseChannel(player);
	}
}

void Npc::onThink(uint32_t interval)
{
	Creature::onThink(interval);

	if (m_npcEventHandler) {
		m_npcEventHandler->onThink();
	}

	if (getTimeSinceLastMove() >= walkTicks) {
		addEventWalk();
	}
}

void Npc::doSay(const std::string& text)
{
	g_game.internalCreatureSay(this, TALKTYPE_SAY, text, false);
}

void Npc::doSayToPlayer(Player* player, const std::string& text)
{
	if (player) {
		player->sendCreatureSay(this, TALKTYPE_PRIVATE_NP, text);
		player->onCreatureSay(this, TALKTYPE_PRIVATE_NP, text);
	}
}

void Npc::doMove(Direction dir)
{
	g_game.internalMoveCreature(this, dir);
}

void Npc::doTurn(Direction dir)
{
	g_game.internalCreatureTurn(this, dir);
}

void Npc::onPlayerTrade(Player* player, int32_t callback, uint16_t itemId, uint8_t count,
                        uint8_t amount, bool ignore/* = false*/, bool inBackpacks/* = false*/)
{
	if (m_npcEventHandler) {
		m_npcEventHandler->onPlayerTrade(player, callback, itemId, count, amount, ignore, inBackpacks);
	}
	player->sendSaleItemList();
}

void Npc::onPlayerEndTrade(Player* player, int32_t buyCallback, int32_t sellCallback)
{
	lua_State* L = getScriptInterface()->getLuaState();

	if (buyCallback != -1) {
		luaL_unref(L, LUA_REGISTRYINDEX, buyCallback);
	}

	if (sellCallback != -1) {
		luaL_unref(L, LUA_REGISTRYINDEX, sellCallback);
	}

	removeShopPlayer(player);

	if (m_npcEventHandler) {
		m_npcEventHandler->onPlayerEndTrade(player);
	}
}

bool Npc::getNextStep(Direction& dir, uint32_t& flags)
{
	if (Creature::getNextStep(dir, flags)) {
		return true;
	}

	if (walkTicks <= 0) {
		return false;
	}

	if (focusCreature != 0) {
		return false;
	}

	if (getTimeSinceLastMove() < walkTicks) {
		return false;
	}

	return getRandomStep(dir);
}

bool Npc::canWalkTo(const Position& fromPos, Direction dir)
{
	if (masterRadius == 0) {
		return false;
	}

	Position toPos = getNextPosition(dir, fromPos);
	if (!Spawns::getInstance()->isInZone(masterPos, masterRadius, toPos)) {
		return false;
	}

	Tile* tile = g_game.getTile(toPos.x, toPos.y, toPos.z);
	if (!tile || tile->__queryAdd(0, this, 1, 0) != RET_NOERROR) {
		return false;
	}

	if (!floorChange && (tile->floorChange() || tile->getTeleportItem())) {
		return false;
	}

	if (!ignoreHeight && tile->hasHeight(1)) {
		return false;
	}

	return true;
}

bool Npc::getRandomStep(Direction& dir)
{
	std::vector<Direction> dirList;
	const Position& creaturePos = getPosition();

	if (canWalkTo(creaturePos, NORTH)) {
		dirList.push_back(NORTH);
	}

	if (canWalkTo(creaturePos, SOUTH)) {
		dirList.push_back(SOUTH);
	}

	if (canWalkTo(creaturePos, EAST)) {
		dirList.push_back(EAST);
	}

	if (canWalkTo(creaturePos, WEST)) {
		dirList.push_back(WEST);
	}

	if (dirList.empty()) {
		return false;
	}

	dir = dirList[uniform_random(0, dirList.size() - 1)];
	return true;
}

void Npc::doMoveTo(const Position& target)
{
	std::list<Direction> listDir;
	if (getPathTo(target, listDir, 1, 1, true, true)) {
		startAutoWalk(listDir);
	}
}

void Npc::turnToCreature(Creature* creature)
{
	const Position& creaturePos = creature->getPosition();
	const Position& myPos = getPosition();
	const auto dx = Position::getOffsetX(myPos, creaturePos);
	const auto dy = Position::getOffsetY(myPos, creaturePos);

	float tan;
	if (dx != 0) {
		tan = static_cast<float>(dy) / dx;
	} else {
		tan = 10;
	}

	Direction dir;
	if (std::abs(tan) < 1) {
		if (dx > 0) {
			dir = WEST;
		} else {
			dir = EAST;
		}
	} else {
		if (dy > 0) {
			dir = NORTH;
		} else {
			dir = SOUTH;
		}
	}
	g_game.internalCreatureTurn(this, dir);
}

void Npc::setCreatureFocus(Creature* creature)
{
	if (creature) {
		focusCreature = creature->getID();
		turnToCreature(creature);
	} else {
		focusCreature = 0;
	}
}

void Npc::addShopPlayer(Player* player)
{
	shopPlayerSet.insert(player);
}

void Npc::removeShopPlayer(Player* player)
{
	shopPlayerSet.erase(player);
}

void Npc::closeAllShopWindows()
{
	while (!shopPlayerSet.empty()) {
		Player* player = *shopPlayerSet.begin();
		if (!player->closeShopWindow()) {
			removeShopPlayer(player);
		}
	}
}

NpcScriptInterface* Npc::getScriptInterface()
{
	return m_scriptInterface;
}

NpcScriptInterface::NpcScriptInterface() :
	LuaScriptInterface("Npc interface")
{
	m_libLoaded = false;
	initState();
}

NpcScriptInterface::~NpcScriptInterface()
{
	//
}

bool NpcScriptInterface::initState()
{
	m_luaState = g_luaEnvironment.getLuaState();
	if (!m_luaState) {
		return false;
	}

	registerFunctions();

	lua_newtable(m_luaState);
	m_eventTableRef = luaL_ref(m_luaState, LUA_REGISTRYINDEX);
	m_runningEventId = EVENT_ID_USER;
	return true;
}

bool NpcScriptInterface::closeState()
{
	m_libLoaded = false;
	LuaScriptInterface::closeState();
	return true;
}

bool NpcScriptInterface::loadNpcLib(const std::string& file)
{
	if (m_libLoaded) {
		return true;
	}

	if (loadFile(file) == -1) {
		std::cout << "[Warning - NpcScriptInterface::loadNpcLib] Can not load " << file << std::endl;
		return false;
	}

	m_libLoaded = true;
	return true;
}

void NpcScriptInterface::registerFunctions()
{
	//npc exclusive functions
	lua_register(m_luaState, "selfSay", NpcScriptInterface::luaActionSay);
	lua_register(m_luaState, "selfMove", NpcScriptInterface::luaActionMove);
	lua_register(m_luaState, "selfMoveTo", NpcScriptInterface::luaActionMoveTo);
	lua_register(m_luaState, "selfTurn", NpcScriptInterface::luaActionTurn);
	lua_register(m_luaState, "selfFollow", NpcScriptInterface::luaActionFollow);
	lua_register(m_luaState, "selfGetPosition", NpcScriptInterface::luaSelfGetPos);
	lua_register(m_luaState, "getDistanceTo", NpcScriptInterface::luagetDistanceTo);
	lua_register(m_luaState, "doNpcSetCreatureFocus", NpcScriptInterface::luaSetNpcFocus);
	lua_register(m_luaState, "getNpcCid", NpcScriptInterface::luaGetNpcCid);
	lua_register(m_luaState, "getNpcPos", NpcScriptInterface::luaGetNpcPos);
	lua_register(m_luaState, "getNpcName", NpcScriptInterface::luaGetNpcName);
	lua_register(m_luaState, "getNpcParameter", NpcScriptInterface::luaGetNpcParameter);
	lua_register(m_luaState, "openShopWindow", NpcScriptInterface::luaOpenShopWindow);
	lua_register(m_luaState, "closeShopWindow", NpcScriptInterface::luaCloseShopWindow);
	lua_register(m_luaState, "doSellItem", NpcScriptInterface::luaDoSellItem);

	// metatable
	registerMethod("Npc", "getParameter", NpcScriptInterface::luaNpcGetParameter);
	registerMethod("Npc", "setFocus", NpcScriptInterface::luaNpcSetFocus);

	registerMethod("Npc", "openShopWindow", NpcScriptInterface::luaNpcOpenShopWindow);
	registerMethod("Npc", "closeShopWindow", NpcScriptInterface::luaNpcCloseShopWindow);
}

int32_t NpcScriptInterface::luaSelfGetPos(lua_State* L)
{
	//selfGetPosition()
	Npc* npc = getScriptEnv()->getNpc();
	if (npc) {
		pushPosition(L, npc->getPosition());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t NpcScriptInterface::luaActionSay(lua_State* L)
{
	//selfSay(words[, target])
	Npc* npc = getScriptEnv()->getNpc();
	if (!npc) {
		return 0;
	}

	const std::string& text = getString(L, 1);
	if (lua_gettop(L) >= 2) {
		Player* target = getPlayer(L, 2);
		if (target) {
			npc->doSayToPlayer(target, text);
			return 0;
		}
	}

	npc->doSay(text);
	return 0;
}

int32_t NpcScriptInterface::luaActionMove(lua_State* L)
{
	//selfMove(direction)
	Npc* npc = getScriptEnv()->getNpc();
	if (npc) {
		npc->doMove(static_cast<Direction>(getNumber<uint32_t>(L, 1)));
	}
	return 0;
}

int32_t NpcScriptInterface::luaActionMoveTo(lua_State* L)
{
	//selfMoveTo(x,y,z)
	Npc* npc = getScriptEnv()->getNpc();
	if (!npc) {
		return 0;
	}

	npc->doMoveTo(Position(
		getNumber<uint16_t>(L, 1),
		getNumber<uint16_t>(L, 2),
		getNumber<uint8_t>(L, 3)
	));
	return 0;
}

int32_t NpcScriptInterface::luaActionTurn(lua_State* L)
{
	//selfTurn(direction)
	Npc* npc = getScriptEnv()->getNpc();
	if (npc) {
		npc->doTurn(static_cast<Direction>(getNumber<uint32_t>(L, 1)));
	}
	return 0;
}

int32_t NpcScriptInterface::luaActionFollow(lua_State* L)
{
	//selfFollow(player)
	Npc* npc = getScriptEnv()->getNpc();
	if (!npc) {
		pushBoolean(L, false);
		return 1;
	}

	pushBoolean(L, npc->setFollowCreature(getPlayer(L, 1)));
	return 1;
}

int32_t NpcScriptInterface::luagetDistanceTo(lua_State* L)
{
	//getDistanceTo(uid)
	uint32_t uid = popNumber<uint32_t>(L);

	ScriptEnvironment* env = getScriptEnv();

	Npc* npc = env->getNpc();
	if (!npc) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_THING_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	Thing* thing = env->getThingByUID(uid);
	if (!thing) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_THING_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	const Position& thingPos = thing->getPosition();
	const Position& npcPos = npc->getPosition();
	if (npcPos.z != thingPos.z) {
		lua_pushnumber(L, -1);
	} else {
		int32_t dist = std::max<int32_t>(Position::getDistanceX(npcPos, thingPos), Position::getDistanceY(npcPos, thingPos));
		lua_pushnumber(L, dist);
	}
	return 1;
}

int32_t NpcScriptInterface::luaSetNpcFocus(lua_State* L)
{
	//doNpcSetCreatureFocus(cid)
	Npc* npc = getScriptEnv()->getNpc();
	if (npc) {
		npc->setCreatureFocus(getCreature(L, -1));
	}
	return 0;
}

int32_t NpcScriptInterface::luaGetNpcPos(lua_State* L)
{
	//getNpcPos()
	Position pos;
	uint32_t stackpos = 0;

	Npc* npc = getScriptEnv()->getNpc();
	if (npc) {
		pos = npc->getPosition();
		stackpos = npc->getParent()->__getIndexOfThing(npc);
	}

	pushPosition(L, pos, stackpos);
	return 1;
}

int32_t NpcScriptInterface::luaGetNpcCid(lua_State* L)
{
	//getNpcCid()
	Npc* npc = getScriptEnv()->getNpc();
	if (npc) {
		lua_pushnumber(L, npc->getID());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t NpcScriptInterface::luaGetNpcName(lua_State* L)
{
	//getNpcName()
	Npc* npc = getScriptEnv()->getNpc();
	if (npc) {
		LuaScriptInterface::pushString(L, npc->getName());
	} else {
		pushBoolean(L, false);
	}
	return 1;
}

int32_t NpcScriptInterface::luaGetNpcParameter(lua_State* L)
{
	//getNpcParameter(paramKey)
	std::string paramKey = popString(L);

	Npc* npc = getScriptEnv()->getNpc();
	if (npc) {
		auto it = npc->m_parameters.find(paramKey);
		if (it != npc->m_parameters.end()) {
			LuaScriptInterface::pushString(L, it->second);
		} else {
			lua_pushnil(L);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t NpcScriptInterface::luaOpenShopWindow(lua_State* L)
{
	//openShopWindow(cid, items, onBuy callback, onSell callback)
	int32_t sellCallback;
	if (lua_isfunction(L, -1) == 0) {
		sellCallback = -1;
		lua_pop(L, 1);    // skip it - use default value
	} else {
		sellCallback = popCallback(L);
	}

	int32_t buyCallback;
	if (lua_isfunction(L, -1) == 0) {
		buyCallback = -1;
		lua_pop(L, 1);    // skip it - use default value
	} else {
		buyCallback = popCallback(L);
	}

	if (lua_istable(L, -1) == 0) {
		reportError(__FUNCTION__, "item list is not a table.");
		pushBoolean(L, false);
		return 1;
	}

	std::list<ShopInfo> items;
	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		ShopInfo item;
		item.itemId = popField<uint32_t>(L, "id");
		item.subType = popField<int32_t>(L, "subType");
		if (item.subType == 0) {
			item.subType = popField<int32_t>(L, "subtype");
		}

		item.buyPrice = popField<uint32_t>(L, "buy");
		item.sellPrice = popField<uint32_t>(L, "sell");
		item.realName = popFieldString(L, "name");

		items.push_back(item);
		lua_pop(L, 1);
	}
	lua_pop(L, 1);

	Player* player = getPlayer(L, -1);
	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	//Close any eventual other shop window currently open.
	player->closeShopWindow(false);

	Npc* npc = getScriptEnv()->getNpc();
	if (!npc) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	npc->addShopPlayer(player);
	player->setShopOwner(npc, buyCallback, sellCallback);
	player->openShopWindow(npc, items);

	pushBoolean(L, true);
	return 1;
}

int32_t NpcScriptInterface::luaCloseShopWindow(lua_State* L)
{
	//closeShopWindow(cid)
	Npc* npc = getScriptEnv()->getNpc();
	if (!npc) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Player* player = getPlayer(L, 1);
	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	int32_t buyCallback;
	int32_t sellCallback;

	Npc* merchant = player->getShopOwner(buyCallback, sellCallback);

	//Check if we actually have a shop window with this player.
	if (merchant == npc) {
		player->sendCloseShop();

		if (buyCallback != -1) {
			luaL_unref(L, LUA_REGISTRYINDEX, buyCallback);
		}

		if (sellCallback != -1) {
			luaL_unref(L, LUA_REGISTRYINDEX, sellCallback);
		}

		player->setShopOwner(nullptr, -1, -1);
		npc->removeShopPlayer(player);
	}

	pushBoolean(L, true);
	return 1;
}

int32_t NpcScriptInterface::luaDoSellItem(lua_State* L)
{
	//doSellItem(cid, itemid, amount, <optional> subtype, <optional> actionid, <optional: default: 1> canDropOnMap)
	Player* player = getPlayer(L, 1);
	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	uint32_t sellCount = 0;

	uint32_t itemId = getNumber<uint32_t>(L, 2);
	uint32_t amount = getNumber<uint32_t>(L, 3);
	uint32_t subType;

	int32_t n = getNumber<int32_t>(L, 4, -1);
	if (n != -1) {
		subType = n;
	} else {
		subType = 1;
	}

	uint32_t actionId = getNumber<uint32_t>(L, 5, 0);
	bool canDropOnMap = getBoolean(L, 6, true);

	const ItemType& it = Item::items[itemId];
	if (it.stackable) {
		while (amount > 0) {
			int32_t stackCount = std::min<int32_t>(100, amount);
			Item* item = Item::CreateItem(it.id, stackCount);
			if (item && actionId != 0) {
				item->setActionId(actionId);
			}

			if (g_game.internalPlayerAddItem(player, item, canDropOnMap) != RET_NOERROR) {
				delete item;
				lua_pushnumber(L, sellCount);
				return 1;
			}

			amount -= stackCount;
			sellCount += stackCount;
		}
	} else {
		for (uint32_t i = 0; i < amount; ++i) {
			Item* item = Item::CreateItem(it.id, subType);
			if (item && actionId != 0) {
				item->setActionId(actionId);
			}

			if (g_game.internalPlayerAddItem(player, item, canDropOnMap) != RET_NOERROR) {
				delete item;
				lua_pushnumber(L, sellCount);
				return 1;
			}

			++sellCount;
		}
	}

	lua_pushnumber(L, sellCount);
	return 1;
}

int32_t NpcScriptInterface::luaNpcGetParameter(lua_State* L)
{
	// npc:getParameter(key)
	const std::string& key = getString(L, 2);
	Npc* npc = getUserdata<Npc>(L, 1);
	if (npc) {
		auto it = npc->m_parameters.find(key);
		if (it != npc->m_parameters.end()) {
			pushString(L, it->second);
		} else {
			lua_pushnil(L);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t NpcScriptInterface::luaNpcSetFocus(lua_State* L)
{
	// npc:setFocus(creature)
	Creature* creature = getCreature(L, 2);
	Npc* npc = getUserdata<Npc>(L, 1);
	if (npc) {
		npc->setCreatureFocus(creature);
		pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int32_t NpcScriptInterface::luaNpcOpenShopWindow(lua_State* L)
{
	// npc:openShopWindow(cid, items, buyCallback, sellCallback)
	if (!isTable(L, 3)) {
		reportErrorFunc("item list is not a table.");
		pushBoolean(L, false);
		return 1;
	}

	Player* player = getPlayer(L, 2);
	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Npc* npc = getUserdata<Npc>(L, 1);
	if (!npc) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	int32_t sellCallback = -1;
	if (LuaScriptInterface::isFunction(L, 5)) {
		sellCallback = luaL_ref(L, LUA_REGISTRYINDEX);
	}

	int32_t buyCallback = -1;
	if (LuaScriptInterface::isFunction(L, 4)) {
		buyCallback = luaL_ref(L, LUA_REGISTRYINDEX);
	}

	std::list<ShopInfo> items;

	lua_pushnil(L);
	while (lua_next(L, 3) != 0) {
		ShopInfo item;
		item.itemId = popField<uint32_t>(L, "id");
		item.subType = popField<int32_t>(L, "subType");
		if (item.subType == 0) {
			item.subType = popField<int32_t>(L, "subtype");
		}

		item.buyPrice = popField<uint32_t>(L, "buy");
		item.sellPrice = popField<uint32_t>(L, "sell");
		item.realName = popFieldString(L, "name");

		items.push_back(item);
		lua_pop(L, 1);
	}
	lua_pop(L, 1);

	player->closeShopWindow(false);
	npc->addShopPlayer(player);

	player->setShopOwner(npc, buyCallback, sellCallback);
	player->openShopWindow(npc, items);

	pushBoolean(L, true);
	return 1;
}

int32_t NpcScriptInterface::luaNpcCloseShopWindow(lua_State* L)
{
	// npc:closeShopWindow(player)
	Player* player = getPlayer(L, 2);
	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Npc* npc = getUserdata<Npc>(L, 1);
	if (!npc) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	int32_t buyCallback;
	int32_t sellCallback;

	Npc* merchant = player->getShopOwner(buyCallback, sellCallback);
	if (merchant == npc) {
		player->sendCloseShop();
		if (buyCallback != -1) {
			luaL_unref(L, LUA_REGISTRYINDEX, buyCallback);
		}

		if (sellCallback != -1) {
			luaL_unref(L, LUA_REGISTRYINDEX, sellCallback);
		}

		player->setShopOwner(nullptr, -1, -1);
		npc->removeShopPlayer(player);
	}

	pushBoolean(L, true);
	return 1;
}

NpcEventsHandler::NpcEventsHandler(Npc* npc)
{
	m_npc = npc;
	m_loaded = false;
}

NpcEventsHandler::~NpcEventsHandler()
{
	//
}

bool NpcEventsHandler::isLoaded() const
{
	return m_loaded;
}

NpcScript::NpcScript(const std::string& file, Npc* npc) :
	NpcEventsHandler(npc)
{
	m_scriptInterface = npc->getScriptInterface();
	m_loaded = m_scriptInterface->loadFile("data/npc/scripts/" + file, npc) == 0;
	if (!m_loaded) {
		std::cout << "[Warning - NpcScript::NpcScript] Can not load script: " << file << std::endl;
		std::cout << m_scriptInterface->getLastLuaError() << std::endl;
	} else {
		m_onCreatureSay = m_scriptInterface->getEvent("onCreatureSay");
		m_onCreatureDisappear = m_scriptInterface->getEvent("onCreatureDisappear");
		m_onCreatureAppear = m_scriptInterface->getEvent("onCreatureAppear");
		m_onCreatureMove = m_scriptInterface->getEvent("onCreatureMove");
		m_onPlayerCloseChannel = m_scriptInterface->getEvent("onPlayerCloseChannel");
		m_onPlayerEndTrade = m_scriptInterface->getEvent("onPlayerEndTrade");
		m_onThink = m_scriptInterface->getEvent("onThink");
	}
}

NpcScript::~NpcScript()
{
	//
}

void NpcScript::onCreatureAppear(const Creature* creature)
{
	if (m_onCreatureAppear == -1) {
		return;
	}

	//onCreatureAppear(creature)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onCreature] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_onCreatureAppear, m_scriptInterface);
	env->setNpc(m_npc);

	lua_State* L = m_scriptInterface->getLuaState();
	m_scriptInterface->pushFunction(m_onCreatureAppear);
	lua_pushnumber(L, creature->getID());
	m_scriptInterface->callFunction(1);
}

void NpcScript::onCreatureDisappear(const Creature* creature)
{
	if (m_onCreatureDisappear == -1) {
		return;
	}

	//onCreatureDisappear(id)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onCreatureDisappear] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_onCreatureDisappear, m_scriptInterface);
	env->setNpc(m_npc);

	lua_State* L = m_scriptInterface->getLuaState();
	m_scriptInterface->pushFunction(m_onCreatureDisappear);
	lua_pushnumber(L, creature->getID());
	m_scriptInterface->callFunction(1);
}

void NpcScript::onCreatureMove(const Creature* creature, const Position& oldPos, const Position& newPos)
{
	if (m_onCreatureMove == -1) {
		return;
	}

	//onCreatureMove(creature, oldPos, newPos)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onCreatureMove] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_onCreatureMove, m_scriptInterface);
	env->setNpc(m_npc);

	lua_State* L = m_scriptInterface->getLuaState();
	m_scriptInterface->pushFunction(m_onCreatureMove);
	lua_pushnumber(L, creature->getID());
	LuaScriptInterface::pushPosition(L, oldPos);
	LuaScriptInterface::pushPosition(L, newPos);
	m_scriptInterface->callFunction(3);
}

void NpcScript::onCreatureSay(const Creature* creature, SpeakClasses type, const std::string& text)
{
	if (m_onCreatureSay == -1) {
		return;
	}

	//onCreatureSay(cid, type, msg)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onCreatureSay] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_onCreatureSay, m_scriptInterface);
	env->setNpc(m_npc);

	lua_State* L = m_scriptInterface->getLuaState();
	m_scriptInterface->pushFunction(m_onCreatureSay);
	lua_pushnumber(L, creature->getID());
	lua_pushnumber(L, type);
	LuaScriptInterface::pushString(L, text);
	m_scriptInterface->callFunction(3);
}

void NpcScript::onPlayerTrade(const Player* player, int32_t callback, uint16_t itemid,
                              uint8_t count, uint8_t amount, bool ignore, bool inBackpacks)
{
	if (callback == -1) {
		return;
	}

	//"onBuy"(cid, itemid, count, amount, ignore, inbackpacks)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onPlayerTrade] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(-1, m_scriptInterface);
	env->setNpc(m_npc);

	lua_State* L = m_scriptInterface->getLuaState();
	LuaScriptInterface::pushCallback(L, callback);
	lua_pushnumber(L, player->getID());
	lua_pushnumber(L, itemid);
	lua_pushnumber(L, count);
	lua_pushnumber(L, amount);
	LuaScriptInterface::pushBoolean(L, ignore);
	LuaScriptInterface::pushBoolean(L, inBackpacks);
	m_scriptInterface->callFunction(6);
}

void NpcScript::onPlayerCloseChannel(const Player* player)
{
	if (m_onPlayerCloseChannel == -1) {
		return;
	}

	//onPlayerCloseChannel(cid)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onPlayerCloseChannel] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_onPlayerCloseChannel, m_scriptInterface);
	env->setNpc(m_npc);

	lua_State* L = m_scriptInterface->getLuaState();
	m_scriptInterface->pushFunction(m_onPlayerCloseChannel);
	lua_pushnumber(L, player->getID());
	m_scriptInterface->callFunction(1);
}

void NpcScript::onPlayerEndTrade(const Player* player)
{
	if (m_onPlayerEndTrade == -1) {
		return;
	}

	//onPlayerEndTrade(cid)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onPlayerEndTrade] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_onPlayerEndTrade, m_scriptInterface);
	env->setNpc(m_npc);

	lua_State* L = m_scriptInterface->getLuaState();
	m_scriptInterface->pushFunction(m_onPlayerEndTrade);
	lua_pushnumber(L, player->getID());
	m_scriptInterface->callFunction(1);
}

void NpcScript::onThink()
{
	if (m_onThink == -1) {
		return;
	}

	//onThink()
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onThink] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_onThink, m_scriptInterface);
	env->setNpc(m_npc);

	m_scriptInterface->pushFunction(m_onThink);
	m_scriptInterface->callFunction(0);
}
