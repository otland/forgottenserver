/**
 * The Forgotten Server - a server application for the MMORPG Tibia
 * Copyright (C) 2013  Mark Samman <mark.samman@gmail.com>
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

#include "definitions.h"
#include "npc.h"
#include "game.h"
#include "tools.h"
#include "configmanager.h"
#include "position.h"
#include "spells.h"
#include "player.h"

#include <algorithm>
#include <functional>
#include <string>
#include <sstream>
#include <fstream>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "luascript.h"

extern ConfigManager g_config;
extern Game g_game;
extern Spells* g_spells;

uint32_t Npc::npcAutoID = 0x80000000;
NpcScriptInterface* Npc::m_scriptInterface = NULL;

#ifdef __ENABLE_SERVER_DIAGNOSTIC__
uint32_t Npc::npcCount = 0;
#endif

void Npcs::reload()
{
	const std::map<uint32_t, Npc*>& npcs = g_game.getNpcs();
	for (const auto& it : npcs) {
		it.second->closeAllShopWindows();
	}

	delete Npc::m_scriptInterface;
	Npc::m_scriptInterface = NULL;

	for (const auto& it : npcs) {
		it.second->reload();
	}
}

Npc* Npc::createNpc(const std::string& name)
{
	Npc* npc = new Npc(name);
	if (!npc->load()) {
		delete npc;
		return NULL;
	}
	return npc;
}

Npc::Npc(const std::string& _name) :
	Creature()
{
	m_filename = "data/npc/" + _name + ".xml";
	loaded = false;

	masterRadius = -1;
	masterPos.x = 0;
	masterPos.y = 0;
	masterPos.z = 0;

	m_npcEventHandler = NULL;
	reset();

#ifdef __ENABLE_SERVER_DIAGNOSTIC__
	npcCount++;
#endif
}

Npc::~Npc()
{
	reset();

#ifdef __ENABLE_SERVER_DIAGNOSTIC__
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
	focusCreature = 0;

	delete m_npcEventHandler;
	m_npcEventHandler = NULL;

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
	xmlDocPtr doc = xmlParseFile(filename.c_str());
	if (doc) {
		xmlNodePtr root, p;
		root = xmlDocGetRootElement(doc);

		if (xmlStrcmp(root->name, (const xmlChar*)"npc") != 0) {
			std::cerr << "Malformed XML" << std::endl;
			return false;
		}

		int32_t intValue;
		std::string strValue;

		p = root->children;

		std::string scriptfile;
		if (readXMLString(root, "script", strValue)) {
			scriptfile = strValue;
		}

		if (readXMLString(root, "name", strValue)) {
			name = strValue;
		} else {
			name = "";
		}

		if (readXMLInteger(root, "speed", intValue)) {
			baseSpeed = intValue;
		} else {
			baseSpeed = 100;
		}

		if (readXMLInteger(root, "attackable", intValue)) {
			attackable = (intValue != 0);
		}

		if (readXMLInteger(root, "walkinterval", intValue)) {
			walkTicks = intValue;
		}

		if (readXMLInteger(root, "walkradius", intValue)) {
			masterRadius = intValue;
		}

		if (readXMLInteger(root, "floorchange", intValue)) {
			floorChange = (intValue != 0);
		}

		while (p) {
			if (xmlStrcmp(p->name, (const xmlChar*)"health") == 0) {
				if (readXMLInteger(p, "now", intValue)) {
					health = intValue;
				} else {
					health = 100;
				}

				if (readXMLInteger(p, "max", intValue)) {
					healthMax = intValue;
				} else {
					healthMax = 100;
				}
			} else if (xmlStrcmp(p->name, (const xmlChar*)"look") == 0) {
				if (readXMLInteger(p, "type", intValue)) {
					defaultOutfit.lookType = intValue;

					if (readXMLInteger(p, "head", intValue)) {
						defaultOutfit.lookHead = intValue;
					}

					if (readXMLInteger(p, "body", intValue)) {
						defaultOutfit.lookBody = intValue;
					}

					if (readXMLInteger(p, "legs", intValue)) {
						defaultOutfit.lookLegs = intValue;
					}

					if (readXMLInteger(p, "feet", intValue)) {
						defaultOutfit.lookFeet = intValue;
					}

					if (readXMLInteger(p, "addons", intValue)) {
						defaultOutfit.lookAddons = intValue;
					}
				} else if (readXMLInteger(p, "typeex", intValue)) {
					defaultOutfit.lookTypeEx = intValue;
				}

				if (readXMLInteger(p, "mount", intValue)) {
					defaultOutfit.lookMount = intValue;
				}

				currentOutfit = defaultOutfit;
			} else if (xmlStrcmp(p->name, (const xmlChar*)"parameters") == 0) {
				for (xmlNodePtr q = p->children; q != NULL; q = q->next) {
					if (xmlStrcmp(q->name, (const xmlChar*)"parameter") == 0) {
						std::string paramKey;
						std::string paramValue;

						if (!readXMLString(q, "key", paramKey)) {
							continue;
						}

						if (!readXMLString(q, "value", paramValue)) {
							continue;
						}

						m_parameters[paramKey] = paramValue;
					}
				}
			}
			p = p->next;
		}

		xmlFreeDoc(doc);

		if (!scriptfile.empty()) {
			m_npcEventHandler = new NpcScript(scriptfile, this);
			if (!m_npcEventHandler->isLoaded()) {
				return false;
			}
		}

		return true;
	}

	return false;
}

bool Npc::canSee(const Position& pos) const
{
	if (pos.z != getPosition().z) {
		return false;
	}

	return Creature::canSee(getPosition(), pos, 3, 3);
}

std::string Npc::getDescription(int32_t lookDistance) const
{
	std::ostringstream s;
	s << name << ".";
	return s.str();
}

void Npc::onCreatureAppear(const Creature* creature, bool isLogin)
{
	Creature::onCreatureAppear(creature, isLogin);

	if (creature == this && walkTicks > 0) {
		addEventWalk();
	}

	if (creature == this || creature->getPlayer()) {
		if (m_npcEventHandler) {
			m_npcEventHandler->onCreatureAppear(creature);
		}
	}
}

void Npc::onCreatureDisappear(const Creature* creature, uint32_t stackpos, bool isLogout)
{
	Creature::onCreatureDisappear(creature, stackpos, isLogout);

	if (creature == this) {
		//Close all open shop window's
		closeAllShopWindows();

		if(m_npcEventHandler){
			m_npcEventHandler->onCreatureDisappear(creature);
		}
	} else if (creature->getPlayer()) {
		//only players for script events
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

void Npc::onCreatureSay(const Creature* creature, SpeakClasses type, const std::string& text, Position* pos/* = NULL*/)
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
	g_game.internalCreatureSay(this, SPEAK_SAY, text, false);
}

void Npc::doSayToPlayer(Player* player, const std::string& text)
{
	if (player) {
		player->sendCreatureSay(this, SPEAK_PRIVATE_NP, text);
		player->onCreatureSay(this, SPEAK_PRIVATE_NP, text);
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

void Npc::onPlayerTrade(Player* player, ShopEvent_t type, int32_t callback, uint16_t itemId,
                        uint8_t count, uint8_t amount, bool ignore/* = false*/, bool inBackpacks/* = false*/)
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

	if (!dirList.empty()) {
		std::random_shuffle(dirList.begin(), dirList.end());
		dir = dirList[random_range(0, dirList.size() - 1)];
		return true;
	}

	return false;
}

void Npc::doMoveTo(const Position& target)
{
	std::list<Direction> listDir;
	if (g_game.getPathToEx(this, target, listDir, 1, 1, true, true)) {
		startAutoWalk(listDir);
	}
}

void Npc::turnToCreature(Creature* creature)
{
	const Position& creaturePos = creature->getPosition();
	const Position& myPos = getPosition();
	int32_t dx = Position::getOffsetX(myPos, creaturePos);
	int32_t dy = Position::getOffsetY(myPos, creaturePos);

	float tan;
	if (dx != 0) {
		tan = (float)dy / dx;
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
	for (Player* player : shopPlayerSet) {
		player->closeShopWindow();
	}
	shopPlayerSet.clear();
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
	return LuaScriptInterface::initState();
}

bool NpcScriptInterface::closeState()
{
	m_libLoaded = false;
	return LuaScriptInterface::closeState();
}

bool NpcScriptInterface::loadNpcLib(const std::string& file)
{
	if (m_libLoaded) {
		return true;
	}

	if (loadFile(file) == -1) {
		std::cout << "Warning: [NpcScriptInterface::loadNpcLib] Can not load " << file << std::endl;
		return false;
	}

	m_libLoaded = true;
	return true;
}

void NpcScriptInterface::registerFunctions()
{
	LuaScriptInterface::registerFunctions();

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
}

int32_t NpcScriptInterface::luaSelfGetPos(lua_State* L)
{
	//selfGetPosition()
	ScriptEnvironment* env = getScriptEnv();
	Npc* npc = env->getNpc();

	if (npc) {
		Position pos = npc->getPosition();
		pushPosition(L, pos);
	} else {
		lua_pushnil(L);
	}

	return 1;
}

int32_t NpcScriptInterface::luaActionSay(lua_State* L)
{
	//selfSay(words [[, target], publicize])
	// publicize defaults to true if there is no target, false otherwise
	uint32_t parameters = lua_gettop(L);
	uint32_t target = 0;
	bool publicize = true;

	if (parameters >= 3) {
		publicize = (popNumber(L) == 1);
	}

	if (parameters >= 2) {
		target = popNumber(L);
		if (target != 0) {
			publicize = false;
		}
	}

	std::string text = popString(L);

	Npc* npc = getScriptEnv()->getNpc();
	if (npc) {
		if (publicize) {
			npc->doSay(text);
		} else {
			npc->doSayToPlayer(g_game.getPlayerByID(target), text);
		}
	}

	return 0;
}

int32_t NpcScriptInterface::luaActionMove(lua_State* L)
{
	//selfMove(direction)
	Direction dir = (Direction)popNumber(L);

	Npc* npc = getScriptEnv()->getNpc();
	if (npc) {
		npc->doMove(dir);
	}

	return 0;
}

int32_t NpcScriptInterface::luaActionMoveTo(lua_State* L)
{
	//selfMoveTo(x,y,z)
	Position target;
	target.z = (int32_t)popNumber(L);
	target.y = (int32_t)popNumber(L);
	target.x = (int32_t)popNumber(L);

	Npc* npc = getScriptEnv()->getNpc();
	if (npc) {
		npc->doMoveTo(target);
	}

	return 0;
}

int32_t NpcScriptInterface::luaActionTurn(lua_State* L)
{
	//selfTurn(direction)
	Direction dir = (Direction)popNumber(L);

	ScriptEnvironment* env = getScriptEnv();

	Npc* npc = env->getNpc();
	if (npc) {
		npc->doTurn(dir);
	}

	return 0;
}

int32_t NpcScriptInterface::luaActionFollow(lua_State* L)
{
	//selfFollow(cid)
	uint32_t cid = popNumber(L);

	ScriptEnvironment* env = getScriptEnv();

	Player* player = g_game.getPlayerByID(cid);
	if (cid != 0 && !player) {
		pushBoolean(L, false);
		return 1;
	}

	Npc* npc = env->getNpc();

	if (!npc) {
		pushBoolean(L, false);
		return 1;
	}

	bool result = npc->setFollowCreature(player, true);
	pushBoolean(L, result);
	return 1;
}

int32_t NpcScriptInterface::luagetDistanceTo(lua_State* L)
{
	//getDistanceTo(uid)
	uint32_t uid = popNumber(L);

	ScriptEnvironment* env = getScriptEnv();

	Npc* npc = env->getNpc();
	Thing* thing = env->getThingByUID(uid);

	if (thing && npc) {
		Position thing_pos = thing->getPosition();
		Position npc_pos = npc->getPosition();

		if (npc_pos.z != thing_pos.z) {
			lua_pushnumber(L, -1);
		} else {
			int32_t dist = std::max<int32_t>(Position::getDistanceX(npc_pos, thing_pos), Position::getDistanceY(npc_pos, thing_pos));
			lua_pushnumber(L, dist);
		}
	} else {
		reportErrorFunc(getErrorDesc(LUA_ERROR_THING_NOT_FOUND));
		lua_pushnil(L);
	}

	return 1;
}

int32_t NpcScriptInterface::luaSetNpcFocus(lua_State* L)
{
	//doNpcSetCreatureFocus(cid)
	uint32_t cid = popNumber(L);

	ScriptEnvironment* env = getScriptEnv();

	Npc* npc = env->getNpc();
	if (npc) {
		npc->setCreatureFocus(g_game.getCreatureByID(cid));
	}

	return 0;
}

int32_t NpcScriptInterface::luaGetNpcPos(lua_State* L)
{
	//getNpcPos()
	ScriptEnvironment* env = getScriptEnv();

	Position pos(0, 0, 0);
	uint32_t stackpos = 0;

	Npc* npc = env->getNpc();

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
	ScriptEnvironment* env = getScriptEnv();

	Npc* npc = env->getNpc();
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

	ScriptEnvironment* env = getScriptEnv();

	Npc* npc = env->getNpc();
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
	int32_t buyCallback = -1;
	int32_t sellCallback = -1;
	std::list<ShopInfo> items;
	Player* player = NULL;

	ScriptEnvironment* env = getScriptEnv();
	Npc* npc = env->getNpc();

	if (lua_isfunction(L, -1) == 0) {
		lua_pop(L, 1);    // skip it - use default value
	} else {
		sellCallback = popCallback(L);
	}

	if (lua_isfunction(L, -1) == 0) {
		lua_pop(L, 1);    // skip it - use default value
	} else {
		buyCallback = popCallback(L);
	}

	if (lua_istable(L, -1) == 0) {
		reportError(__FUNCTION__, "item list is not a table.");
		pushBoolean(L, false);
		return 1;
	}

	// first key
	lua_pushnil(L);

	while (lua_next(L, -2) != 0) {
		ShopInfo item;
		item.itemId = getField(L, "id");
		item.subType = getField(L, "subType");

		if (item.subType == 0) {
			item.subType = getField(L, "subtype");
		}

		item.buyPrice = getField(L, "buy");
		item.sellPrice = getField(L, "sell");
		item.realName = getFieldString(L, "name");
		items.push_back(item);

		lua_pop(L, 1);
	}

	lua_pop(L, 1);

	player = g_game.getPlayerByID(popNumber(L));

	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	//Close any eventual other shop window currently open.
	player->closeShopWindow(false);

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
	ScriptEnvironment* env = getScriptEnv();

	Player* player = g_game.getPlayerByID(popNumber(L));

	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Npc* npc = env->getNpc();

	if (!npc) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
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

		player->setShopOwner(NULL, -1, -1);
		npc->removeShopPlayer(player);
	}

	pushBoolean(L, true);
	return 1;
}

int32_t NpcScriptInterface::luaDoSellItem(lua_State* L)
{
	//doSellItem(cid, itemid, amount, <optional> subtype, <optional> actionid, <optional: default: 1> canDropOnMap)
	int32_t parameters = lua_gettop(L);

	bool canDropOnMap = true;

	if (parameters > 5) {
		canDropOnMap = (popNumber(L) == 1);
	}

	uint32_t actionId = 0;

	if (parameters > 4) {
		actionId = popNumber(L);
	}

	uint32_t subType = 1;

	if (parameters > 3) {
		int32_t n = popNumber(L);

		if (n != -1) {
			subType = n;
		}
	}

	uint32_t amount = (uint32_t)popNumber(L);
	uint32_t itemId = (uint32_t)popNumber(L);

	Player* player = g_game.getPlayerByID(popNumber(L));
	if (!player) {
		reportErrorFunc(getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	uint32_t sellCount = 0;

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

			amount = amount - stackCount;
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

	if (m_scriptInterface->reserveScriptEnv()) {
		m_scriptInterface->getScriptEnv()->setNpc(npc);

		if (m_scriptInterface->loadFile(file, npc) == -1) {
			std::cout << "[Warning - NpcScript::NpcScript] Can not load script: " << file << std::endl;
			std::cout << m_scriptInterface->getLastLuaError() << std::endl;
			m_loaded = false;
			m_scriptInterface->releaseScriptEnv();
			return;
		}

		m_scriptInterface->releaseScriptEnv();
	}

	m_onCreatureSay = m_scriptInterface->getEvent("onCreatureSay");
	m_onCreatureDisappear = m_scriptInterface->getEvent("onCreatureDisappear");
	m_onCreatureAppear = m_scriptInterface->getEvent("onCreatureAppear");
	m_onCreatureMove = m_scriptInterface->getEvent("onCreatureMove");
	m_onPlayerCloseChannel = m_scriptInterface->getEvent("onPlayerCloseChannel");
	m_onPlayerEndTrade = m_scriptInterface->getEvent("onPlayerEndTrade");
	m_onThink = m_scriptInterface->getEvent("onThink");
	m_loaded = true;
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
	if (m_scriptInterface->reserveScriptEnv()) {
		ScriptEnvironment* env = m_scriptInterface->getScriptEnv();

		lua_State* L = m_scriptInterface->getLuaState();

		env->setScriptId(m_onCreatureAppear, m_scriptInterface);
		env->setRealPos(m_npc->getPosition());
		env->setNpc(m_npc);

		m_scriptInterface->pushFunction(m_onCreatureAppear);
		lua_pushnumber(L, creature->getID());
		m_scriptInterface->callFunction(1);
		m_scriptInterface->releaseScriptEnv();
	} else {
		std::cout << "[Error - NpcScript::onCreatureAppear] NPC Name: " << m_npc->getName() << " - Call stack overflow" << std::endl;
	}
}

void NpcScript::onCreatureDisappear(const Creature* creature)
{
	if (m_onCreatureDisappear == -1) {
		return;
	}

	//onCreatureDisappear(id)
	if (m_scriptInterface->reserveScriptEnv()) {
		ScriptEnvironment* env = m_scriptInterface->getScriptEnv();

		lua_State* L = m_scriptInterface->getLuaState();

		env->setScriptId(m_onCreatureDisappear, m_scriptInterface);
		env->setRealPos(m_npc->getPosition());
		env->setNpc(m_npc);

		m_scriptInterface->pushFunction(m_onCreatureDisappear);
		lua_pushnumber(L, creature->getID());
		m_scriptInterface->callFunction(1);
		m_scriptInterface->releaseScriptEnv();
	} else {
		std::cout << "[Error - NpcScript::onCreatureDisappear] NPC Name: " << m_npc->getName() << " - Call stack overflow" << std::endl;
	}
}

void NpcScript::onCreatureMove(const Creature* creature, const Position& oldPos, const Position& newPos)
{
	if (m_onCreatureMove == -1) {
		return;
	}

	//onCreatureMove(creature, oldPos, newPos)
	if (m_scriptInterface->reserveScriptEnv()) {
		ScriptEnvironment* env = m_scriptInterface->getScriptEnv();

		lua_State* L = m_scriptInterface->getLuaState();

		env->setScriptId(m_onCreatureMove, m_scriptInterface);
		env->setRealPos(m_npc->getPosition());
		env->setNpc(m_npc);

		m_scriptInterface->pushFunction(m_onCreatureMove);
		lua_pushnumber(L, creature->getID());
		LuaScriptInterface::pushPosition(L, oldPos, 0);
		LuaScriptInterface::pushPosition(L, newPos, 0);
		m_scriptInterface->callFunction(3);
		m_scriptInterface->releaseScriptEnv();
	} else {
		std::cout << "[Error - NpcScript::onCreatureMove] NPC Name: " << m_npc->getName() << " - Call stack overflow" << std::endl;
	}
}

void NpcScript::onCreatureSay(const Creature* creature, SpeakClasses type, const std::string& text, Position* pos/* = NULL*/)
{
	if (m_onCreatureSay == -1) {
		return;
	}

	//onCreatureSay(cid, type, msg)
	if (m_scriptInterface->reserveScriptEnv()) {
		ScriptEnvironment* env = m_scriptInterface->getScriptEnv();

		env->setScriptId(m_onCreatureSay, m_scriptInterface);
		env->setRealPos(m_npc->getPosition());
		env->setNpc(m_npc);

		lua_State* L = m_scriptInterface->getLuaState();
		m_scriptInterface->pushFunction(m_onCreatureSay);
		lua_pushnumber(L, creature->getID());
		lua_pushnumber(L, type);
		LuaScriptInterface::pushString(L, text);
		m_scriptInterface->callFunction(3);
		m_scriptInterface->releaseScriptEnv();
	} else {
		std::cout << "[Error - NpcScript::onCreatureSay] NPC Name: " << m_npc->getName() << " - Call stack overflow" << std::endl;
	}
}

void NpcScript::onPlayerTrade(const Player* player, int32_t callback, uint16_t itemid,
                              uint8_t count, uint8_t amount, bool ignore, bool inBackpacks)
{
	if (callback == -1) {
		return;
	}

	//"onBuy"(cid, itemid, count, amount, ignore, inbackpacks)
	if (m_scriptInterface->reserveScriptEnv()) {
		ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
		env->setScriptId(-1, m_scriptInterface);
		env->setRealPos(m_npc->getPosition());
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
		m_scriptInterface->releaseScriptEnv();
	} else {
		std::cout << "[Error - NpcScript::onPlayerTrade] NPC Name: " << m_npc->getName() << " - Call stack overflow" << std::endl;
	}
}

void NpcScript::onPlayerCloseChannel(const Player* player)
{
	if (m_onPlayerCloseChannel == -1) {
		return;
	}

	//onPlayerCloseChannel(cid)
	if (m_scriptInterface->reserveScriptEnv()) {
		ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
		env->setScriptId(m_onPlayerCloseChannel, m_scriptInterface);
		env->setRealPos(m_npc->getPosition());
		env->setNpc(m_npc);

		lua_State* L = m_scriptInterface->getLuaState();
		m_scriptInterface->pushFunction(m_onPlayerCloseChannel);
		lua_pushnumber(L, player->getID());
		m_scriptInterface->callFunction(1);
		m_scriptInterface->releaseScriptEnv();
	} else {
		std::cout << "[Error - NpcScript::onPlayerCloseChannel] NPC Name: " << m_npc->getName() << " - Call stack overflow" << std::endl;
	}
}

void NpcScript::onPlayerEndTrade(const Player* player)
{
	if (m_onPlayerCloseChannel == -1) {
		return;
	}

	//onPlayerEndTrade(cid)
	if (m_scriptInterface->reserveScriptEnv()) {
		ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
		env->setScriptId(m_onPlayerCloseChannel, m_scriptInterface);
		env->setRealPos(m_npc->getPosition());
		env->setNpc(m_npc);

		lua_State* L = m_scriptInterface->getLuaState();
		m_scriptInterface->pushFunction(m_onPlayerEndTrade);
		lua_pushnumber(L, player->getID());
		m_scriptInterface->callFunction(1);
		m_scriptInterface->releaseScriptEnv();
	} else {
		std::cout << "[Error - NpcScript::onPlayerEndTrade] NPC Name: " << m_npc->getName() << " - Call stack overflow" << std::endl;
	}
}

void NpcScript::onThink()
{
	if (m_onThink == -1) {
		return;
	}

	//onThink()
	if (m_scriptInterface->reserveScriptEnv()) {
		ScriptEnvironment* env = m_scriptInterface->getScriptEnv();

		env->setScriptId(m_onThink, m_scriptInterface);
		env->setRealPos(m_npc->getPosition());
		env->setNpc(m_npc);

		m_scriptInterface->pushFunction(m_onThink);
		m_scriptInterface->callFunction(0);
		m_scriptInterface->releaseScriptEnv();
	} else {
		std::cout << "[Error - NpcScript::onThink] NPC Name: " << m_npc->getName() << " - Call stack overflow" << std::endl;
	}
}
