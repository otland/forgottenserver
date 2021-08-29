/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019  Mark Samman <mark.samman@gmail.com>
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
#include "pugicast.h"

extern Game g_game;
extern LuaEnvironment g_luaEnvironment;

uint32_t Npc::npcAutoID = 0x80000000;
NpcScriptInterface* Npc::scriptInterface = nullptr;

void Npcs::reload()
{
	const std::map<uint32_t, Npc*>& npcs = g_game.getNpcs();
	for (const auto& it : npcs) {
		it.second->closeAllShopWindows();
	}

	delete Npc::scriptInterface;
	Npc::scriptInterface = nullptr;

	for (const auto& it : npcs) {
		it.second->reload();
	}
}

Npc* Npc::createNpc(const std::string& name)
{
	std::unique_ptr<Npc> npc(new Npc(name));
	if (!npc->load()) {
		return nullptr;
	}
	return npc.release();
}

Npc::Npc(const std::string& name) :
	Creature(),
	filename("data/npc/" + name + ".xml"),
	npcEventHandler(nullptr),
	masterRadius(-1),
	loaded(false)
{
	reset();
}

Npc::~Npc()
{
	reset();
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
	if (loaded) {
		return true;
	}

	reset();

	if (!scriptInterface) {
		scriptInterface = new NpcScriptInterface();
		scriptInterface->loadNpcLib("data/npc/lib/npc.lua");
	}

	loaded = loadFromXml();
	return loaded;
}

void Npc::reset()
{
	loaded = false;
	isIdle = true;
	walkTicks = 1500;
	pushable = true;
	floorChange = false;
	attackable = false;
	ignoreHeight = false;
	focusCreature = 0;
	speechBubble = SPEECHBUBBLE_NONE;

	delete npcEventHandler;
	npcEventHandler = nullptr;

	parameters.clear();
	shopPlayerSet.clear();
	spectators.clear();
}

void Npc::reload()
{
	reset();
	load();

	SpectatorVec players;
	g_game.map.getSpectators(players, getPosition(), true, true);
	for (const auto& player : players) {
		spectators.insert(player->getPlayer());
	}

	const bool hasSpectators = !spectators.empty();
	setIdle(!hasSpectators);

	if (hasSpectators && walkTicks > 0) {
		addEventWalk();
	}

	// Simulate that the creature is placed on the map again.
	if (npcEventHandler) {
		npcEventHandler->onCreatureAppear(this);
	}
}

bool Npc::loadFromXml()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename.c_str());
	if (!result) {
		printXMLError("Error - Npc::loadFromXml", filename, result);
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

	if ((attr = npcNode.attribute("pushable"))) {
		pushable = attr.as_bool();
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

	if ((attr = npcNode.attribute("skull"))) {
		setSkull(getSkullType(asLowerCaseString(attr.as_string())));
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

		if (health > healthMax) {
			health = healthMax;
			std::cout << "[Warning - Npc::loadFromXml] Health now is greater than health max in " << filename << std::endl;
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

	for (auto parameterNode : npcNode.child("parameters").children()) {
		parameters[parameterNode.attribute("key").as_string()] = parameterNode.attribute("value").as_string();
	}

	pugi::xml_attribute scriptFile = npcNode.attribute("script");
	if (scriptFile) {
		npcEventHandler = new NpcEventsHandler(scriptFile.as_string(), this);
		if (!npcEventHandler->isLoaded()) {
			delete npcEventHandler;
			npcEventHandler = nullptr;
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

void Npc::onCreatureAppear(Creature* creature, bool isLogin)
{
	Creature::onCreatureAppear(creature, isLogin);

	if (creature == this) {
		SpectatorVec players;
		g_game.map.getSpectators(players, getPosition(), true, true);
		for (const auto& player : players) {
			spectators.insert(player->getPlayer());
		}

		const bool hasSpectators = !spectators.empty();
		setIdle(!hasSpectators);

		if (hasSpectators && walkTicks > 0) {
			addEventWalk();
		}

		if (npcEventHandler) {
			npcEventHandler->onCreatureAppear(creature);
		}
	} else if (Player* player = creature->getPlayer()) {
		if (npcEventHandler) {
			npcEventHandler->onCreatureAppear(creature);
		}

		spectators.insert(player);
		setIdle(false);
	}
}

void Npc::onRemoveCreature(Creature* creature, bool isLogout)
{
	Creature::onRemoveCreature(creature, isLogout);

	if (creature == this) {
		closeAllShopWindows();
		if (npcEventHandler) {
			npcEventHandler->onCreatureDisappear(creature);
		}
	} else if (Player* player = creature->getPlayer()) {
		if (npcEventHandler) {
			npcEventHandler->onCreatureDisappear(creature);
		}

		spectators.erase(player);
		setIdle(spectators.empty());
	}
}

void Npc::onCreatureMove(Creature* creature, const Tile* newTile, const Position& newPos,
                         const Tile* oldTile, const Position& oldPos, bool teleport)
{
	Creature::onCreatureMove(creature, newTile, newPos, oldTile, oldPos, teleport);

	if (creature == this || creature->getPlayer()) {
		if (npcEventHandler) {
			npcEventHandler->onCreatureMove(creature, oldPos, newPos);
		}

		if (creature != this) {
			Player* player = creature->getPlayer();

			// if player is now in range, add to spectators list, otherwise erase
			if (player->canSee(position)) {
				spectators.insert(player);
			} else {
				spectators.erase(player);
			}

			setIdle(spectators.empty());
		}
	}
}

void Npc::onCreatureSay(Creature* creature, SpeakClasses type, const std::string& text)
{
	if (creature == this) {
		return;
	}

	//only players for script events
	Player* player = creature->getPlayer();
	if (player) {
		if (npcEventHandler) {
			npcEventHandler->onCreatureSay(player, type, text);
		}
	}
}

void Npc::onPlayerCloseChannel(Player* player)
{
	if (npcEventHandler) {
		npcEventHandler->onPlayerCloseChannel(player);
	}
}

void Npc::onThink(uint32_t interval)
{
	Creature::onThink(interval);

	if (npcEventHandler) {
		npcEventHandler->onThink();
	}

	if (!isIdle && getTimeSinceLastMove() >= walkTicks) {
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

void Npc::onPlayerTrade(Player* player, int32_t callback, uint16_t itemId, uint8_t count,
                        uint8_t amount, bool ignore/* = false*/, bool inBackpacks/* = false*/)
{
	if (npcEventHandler) {
		npcEventHandler->onPlayerTrade(player, callback, itemId, count, amount, ignore, inBackpacks);
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

	if (npcEventHandler) {
		npcEventHandler->onPlayerEndTrade(player);
	}
}

bool Npc::getNextStep(Direction& dir, uint32_t& flags)
{
	if (Creature::getNextStep(dir, flags)) {
		return true;
	}

	if (walkTicks == 0) {
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

void Npc::setIdle(const bool idle)
{
	if (idle == isIdle) {
		return;
	}

	if (isRemoved() || getHealth() <= 0) {
		return;
	}

	isIdle = idle;

	if (isIdle) {
		onIdleStatus();
	}
}

bool Npc::canWalkTo(const Position& fromPos, Direction dir) const
{
	if (masterRadius == 0) {
		return false;
	}

	Position toPos = getNextPosition(dir, fromPos);
	if (!Spawns::isInZone(masterPos, masterRadius, toPos)) {
		return false;
	}

	Tile* tile = g_game.map.getTile(toPos);
	if (!tile || tile->queryAdd(0, *this, 1, 0) != RETURNVALUE_NOERROR) {
		return false;
	}

	if (!floorChange && (tile->hasFlag(TILESTATE_FLOORCHANGE) || tile->getTeleportItem())) {
		return false;
	}

	if (!ignoreHeight && tile->hasHeight(1)) {
		return false;
	}

	return true;
}

bool Npc::getRandomStep(Direction& dir) const
{
	std::vector<Direction> dirList;
	const Position& creaturePos = getPosition();

	if (canWalkTo(creaturePos, DIRECTION_NORTH)) {
		dirList.push_back(DIRECTION_NORTH);
	}

	if (canWalkTo(creaturePos, DIRECTION_SOUTH)) {
		dirList.push_back(DIRECTION_SOUTH);
	}

	if (canWalkTo(creaturePos, DIRECTION_EAST)) {
		dirList.push_back(DIRECTION_EAST);
	}

	if (canWalkTo(creaturePos, DIRECTION_WEST)) {
		dirList.push_back(DIRECTION_WEST);
	}

	if (dirList.empty()) {
		return false;
	}

	dir = dirList[uniform_random(0, dirList.size() - 1)];
	return true;
}

bool Npc::doMoveTo(const Position& pos, int32_t minTargetDist/* = 1*/, int32_t maxTargetDist/* = 1*/,
                   bool fullPathSearch/* = true*/, bool clearSight/* = true*/, int32_t maxSearchDist/* = 0*/)
{
	listWalkDir.clear();
	if (getPathTo(pos, listWalkDir, minTargetDist, maxTargetDist, fullPathSearch, clearSight, maxSearchDist)) {
		startAutoWalk();
		return true;
	}
	return false;
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
			dir = DIRECTION_WEST;
		} else {
			dir = DIRECTION_EAST;
		}
	} else {
		if (dy > 0) {
			dir = DIRECTION_NORTH;
		} else {
			dir = DIRECTION_SOUTH;
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
	return scriptInterface;
}

NpcScriptInterface::NpcScriptInterface() :
	LuaScriptInterface("Npc interface")
{
	libLoaded = false;
	initState();
}

bool NpcScriptInterface::initState()
{
	luaState = g_luaEnvironment.getLuaState();
	if (!luaState) {
		return false;
	}

	registerFunctions();

	lua_newtable(luaState);
	eventTableRef = luaL_ref(luaState, LUA_REGISTRYINDEX);
	runningEventId = EVENT_ID_USER;
	return true;
}

bool NpcScriptInterface::closeState()
{
	libLoaded = false;
	LuaScriptInterface::closeState();
	return true;
}

bool NpcScriptInterface::loadNpcLib(const std::string& file)
{
	if (libLoaded) {
		return true;
	}

	if (loadFile(file) == -1) {
		std::cout << "[Warning - NpcScriptInterface::loadNpcLib] Can not load " << file << std::endl;
		return false;
	}

	libLoaded = true;
	return true;
}

void NpcScriptInterface::registerFunctions()
{
	//npc exclusive functions
	lua_register(luaState, "selfSay", NpcScriptInterface::luaActionSay);
	lua_register(luaState, "selfMove", NpcScriptInterface::luaActionMove);
	lua_register(luaState, "selfMoveTo", NpcScriptInterface::luaActionMoveTo);
	lua_register(luaState, "selfTurn", NpcScriptInterface::luaActionTurn);
	lua_register(luaState, "selfFollow", NpcScriptInterface::luaActionFollow);
	lua_register(luaState, "getDistanceTo", NpcScriptInterface::luagetDistanceTo);
	lua_register(luaState, "doNpcSetCreatureFocus", NpcScriptInterface::luaSetNpcFocus);
	lua_register(luaState, "getNpcCid", NpcScriptInterface::luaGetNpcCid);
	lua_register(luaState, "getNpcParameter", NpcScriptInterface::luaGetNpcParameter);
	lua_register(luaState, "openShopWindow", NpcScriptInterface::luaOpenShopWindow);
	lua_register(luaState, "closeShopWindow", NpcScriptInterface::luaCloseShopWindow);
	lua_register(luaState, "doSellItem", NpcScriptInterface::luaDoSellItem);

	// metatable
	registerMethod("Npc", "getParameter", NpcScriptInterface::luaNpcGetParameter);
	registerMethod("Npc", "setFocus", NpcScriptInterface::luaNpcSetFocus);

	registerMethod("Npc", "openShopWindow", NpcScriptInterface::luaNpcOpenShopWindow);
	registerMethod("Npc", "closeShopWindow", NpcScriptInterface::luaNpcCloseShopWindow);
}

int NpcScriptInterface::luaActionSay(lua_State* L)
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

int NpcScriptInterface::luaActionMove(lua_State* L)
{
	//selfMove(direction)
	Npc* npc = getScriptEnv()->getNpc();
	if (npc) {
		g_game.internalMoveCreature(npc, getNumber<Direction>(L, 1));
	}
	return 0;
}

int NpcScriptInterface::luaActionMoveTo(lua_State* L)
{
	//selfMoveTo(x, y, z[, minTargetDist = 1[, maxTargetDist = 1[, fullPathSearch = true[, clearSight = true[, maxSearchDist = 0]]]]])
	//selfMoveTo(position[, minTargetDist = 1[, maxTargetDist = 1[, fullPathSearch = true[, clearSight = true[, maxSearchDist = 0]]]]])
	Npc* npc = getScriptEnv()->getNpc();
	if (!npc) {
		return 0;
	}

	Position position;
	int32_t argsStart = 2;
	if (isTable(L, 1)) {
		position = getPosition(L, 1);
	} else {
		position.x = getNumber<uint16_t>(L, 1);
		position.y = getNumber<uint16_t>(L, 2);
		position.z = getNumber<uint8_t>(L, 3);
		argsStart = 4;
	}

	pushBoolean(L, npc->doMoveTo(
		position,
		getNumber<int32_t>(L, argsStart, 1),
		getNumber<int32_t>(L, argsStart + 1, 1),
		getBoolean(L, argsStart + 2, true),
		getBoolean(L, argsStart + 3, true),
		getNumber<int32_t>(L, argsStart + 4, 0)
	));
	return 1;
}

int NpcScriptInterface::luaActionTurn(lua_State* L)
{
	//selfTurn(direction)
	Npc* npc = getScriptEnv()->getNpc();
	if (npc) {
		g_game.internalCreatureTurn(npc, getNumber<Direction>(L, 1));
	}
	return 0;
}

int NpcScriptInterface::luaActionFollow(lua_State* L)
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

int NpcScriptInterface::luagetDistanceTo(lua_State* L)
{
	//getDistanceTo(uid)
	ScriptEnvironment* env = getScriptEnv();

	Npc* npc = env->getNpc();
	if (!npc) {
		reportErrorFunc(L, getErrorDesc(LUA_ERROR_THING_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	uint32_t uid = getNumber<uint32_t>(L, -1);

	Thing* thing = env->getThingByUID(uid);
	if (!thing) {
		reportErrorFunc(L, getErrorDesc(LUA_ERROR_THING_NOT_FOUND));
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

int NpcScriptInterface::luaSetNpcFocus(lua_State* L)
{
	//doNpcSetCreatureFocus(cid)
	Npc* npc = getScriptEnv()->getNpc();
	if (npc) {
		npc->setCreatureFocus(getCreature(L, -1));
	}
	return 0;
}

int NpcScriptInterface::luaGetNpcCid(lua_State* L)
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

int NpcScriptInterface::luaGetNpcParameter(lua_State* L)
{
	//getNpcParameter(paramKey)
	Npc* npc = getScriptEnv()->getNpc();
	if (!npc) {
		lua_pushnil(L);
		return 1;
	}

	std::string paramKey = getString(L, -1);

	auto it = npc->parameters.find(paramKey);
	if (it != npc->parameters.end()) {
		LuaScriptInterface::pushString(L, it->second);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int NpcScriptInterface::luaOpenShopWindow(lua_State* L)
{
	//openShopWindow(cid, items, onBuy callback, onSell callback)
	int32_t sellCallback;
	if (lua_isfunction(L, -1) == 0) {
		sellCallback = -1;
		lua_pop(L, 1); // skip it - use default value
	} else {
		sellCallback = popCallback(L);
	}

	int32_t buyCallback;
	if (lua_isfunction(L, -1) == 0) {
		buyCallback = -1;
		lua_pop(L, 1); // skip it - use default value
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
		const auto tableIndex = lua_gettop(L);
		ShopInfo item;

		item.itemId = getField<uint32_t>(L, tableIndex, "id");
		item.subType = getField<int32_t>(L, tableIndex, "subType");
		if (item.subType == 0) {
			item.subType = getField<int32_t>(L, tableIndex, "subtype");
			lua_pop(L, 1);
		}

		item.buyPrice = getField<uint32_t>(L, tableIndex, "buy");
		item.sellPrice = getField<uint32_t>(L, tableIndex, "sell");
		item.realName = getFieldString(L, tableIndex, "name");

		items.push_back(item);
		lua_pop(L, 6);
	}
	lua_pop(L, 1);

	Player* player = getPlayer(L, -1);
	if (!player) {
		reportErrorFunc(L, getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	//Close any eventual other shop window currently open.
	player->closeShopWindow(false);

	Npc* npc = getScriptEnv()->getNpc();
	if (!npc) {
		reportErrorFunc(L, getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	npc->addShopPlayer(player);
	player->setShopOwner(npc, buyCallback, sellCallback);
	player->openShopWindow(npc, items);

	pushBoolean(L, true);
	return 1;
}

int NpcScriptInterface::luaCloseShopWindow(lua_State* L)
{
	//closeShopWindow(cid)
	Npc* npc = getScriptEnv()->getNpc();
	if (!npc) {
		reportErrorFunc(L, getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Player* player = getPlayer(L, 1);
	if (!player) {
		reportErrorFunc(L, getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
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

int NpcScriptInterface::luaDoSellItem(lua_State* L)
{
	//doSellItem(cid, itemid, amount, <optional> subtype, <optional> actionid, <optional: default: 1> canDropOnMap)
	Player* player = getPlayer(L, 1);
	if (!player) {
		reportErrorFunc(L, getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
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

			if (g_game.internalPlayerAddItem(player, item, canDropOnMap) != RETURNVALUE_NOERROR) {
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

			if (g_game.internalPlayerAddItem(player, item, canDropOnMap) != RETURNVALUE_NOERROR) {
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

int NpcScriptInterface::luaNpcGetParameter(lua_State* L)
{
	// npc:getParameter(key)
	const std::string& key = getString(L, 2);
	Npc* npc = getUserdata<Npc>(L, 1);
	if (npc) {
		auto it = npc->parameters.find(key);
		if (it != npc->parameters.end()) {
			pushString(L, it->second);
		} else {
			lua_pushnil(L);
		}
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int NpcScriptInterface::luaNpcSetFocus(lua_State* L)
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

int NpcScriptInterface::luaNpcOpenShopWindow(lua_State* L)
{
	// npc:openShopWindow(cid, items, buyCallback, sellCallback)
	if (!isTable(L, 3)) {
		reportErrorFunc(L, "item list is not a table.");
		pushBoolean(L, false);
		return 1;
	}

	Player* player = getPlayer(L, 2);
	if (!player) {
		reportErrorFunc(L, getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Npc* npc = getUserdata<Npc>(L, 1);
	if (!npc) {
		reportErrorFunc(L, getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
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
		const auto tableIndex = lua_gettop(L);
		ShopInfo item;

		item.itemId = getField<uint32_t>(L, tableIndex, "id");
		item.subType = getField<int32_t>(L, tableIndex, "subType");
		if (item.subType == 0) {
			item.subType = getField<int32_t>(L, tableIndex, "subtype");
			lua_pop(L, 1);
		}

		item.buyPrice = getField<uint32_t>(L, tableIndex, "buy");
		item.sellPrice = getField<uint32_t>(L, tableIndex, "sell");
		item.realName = getFieldString(L, tableIndex, "name");

		items.push_back(item);
		lua_pop(L, 6);
	}
	lua_pop(L, 1);

	player->closeShopWindow(false);
	npc->addShopPlayer(player);

	player->setShopOwner(npc, buyCallback, sellCallback);
	player->openShopWindow(npc, items);

	pushBoolean(L, true);
	return 1;
}

int NpcScriptInterface::luaNpcCloseShopWindow(lua_State* L)
{
	// npc:closeShopWindow(player)
	Player* player = getPlayer(L, 2);
	if (!player) {
		reportErrorFunc(L, getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		pushBoolean(L, false);
		return 1;
	}

	Npc* npc = getUserdata<Npc>(L, 1);
	if (!npc) {
		reportErrorFunc(L, getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
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

NpcEventsHandler::NpcEventsHandler(const std::string& file, Npc* npc) :
	npc(npc), scriptInterface(npc->getScriptInterface())
{
	loaded = scriptInterface->loadFile("data/npc/scripts/" + file, npc) == 0;
	if (!loaded) {
		std::cout << "[Warning - NpcScript::NpcScript] Can not load script: " << file << std::endl;
		std::cout << scriptInterface->getLastLuaError() << std::endl;
	} else {
		creatureSayEvent = scriptInterface->getEvent("onCreatureSay");
		creatureDisappearEvent = scriptInterface->getEvent("onCreatureDisappear");
		creatureAppearEvent = scriptInterface->getEvent("onCreatureAppear");
		creatureMoveEvent = scriptInterface->getEvent("onCreatureMove");
		playerCloseChannelEvent = scriptInterface->getEvent("onPlayerCloseChannel");
		playerEndTradeEvent = scriptInterface->getEvent("onPlayerEndTrade");
		thinkEvent = scriptInterface->getEvent("onThink");
	}
}

bool NpcEventsHandler::isLoaded() const
{
	return loaded;
}

void NpcEventsHandler::onCreatureAppear(Creature* creature)
{
	if (creatureAppearEvent == -1) {
		return;
	}

	//onCreatureAppear(creature)
	if (!scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onCreatureAppear] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = scriptInterface->getScriptEnv();
	env->setScriptId(creatureAppearEvent, scriptInterface);
	env->setNpc(npc);

	lua_State* L = scriptInterface->getLuaState();
	scriptInterface->pushFunction(creatureAppearEvent);
	LuaScriptInterface::pushUserdata<Creature>(L, creature);
	LuaScriptInterface::setCreatureMetatable(L, -1, creature);
	scriptInterface->callFunction(1);
}

void NpcEventsHandler::onCreatureDisappear(Creature* creature)
{
	if (creatureDisappearEvent == -1) {
		return;
	}

	//onCreatureDisappear(creature)
	if (!scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onCreatureDisappear] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = scriptInterface->getScriptEnv();
	env->setScriptId(creatureDisappearEvent, scriptInterface);
	env->setNpc(npc);

	lua_State* L = scriptInterface->getLuaState();
	scriptInterface->pushFunction(creatureDisappearEvent);
	LuaScriptInterface::pushUserdata<Creature>(L, creature);
	LuaScriptInterface::setCreatureMetatable(L, -1, creature);
	scriptInterface->callFunction(1);
}

void NpcEventsHandler::onCreatureMove(Creature* creature, const Position& oldPos, const Position& newPos)
{
	if (creatureMoveEvent == -1) {
		return;
	}

	//onCreatureMove(creature, oldPos, newPos)
	if (!scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onCreatureMove] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = scriptInterface->getScriptEnv();
	env->setScriptId(creatureMoveEvent, scriptInterface);
	env->setNpc(npc);

	lua_State* L = scriptInterface->getLuaState();
	scriptInterface->pushFunction(creatureMoveEvent);
	LuaScriptInterface::pushUserdata<Creature>(L, creature);
	LuaScriptInterface::setCreatureMetatable(L, -1, creature);
	LuaScriptInterface::pushPosition(L, oldPos);
	LuaScriptInterface::pushPosition(L, newPos);
	scriptInterface->callFunction(3);
}

void NpcEventsHandler::onCreatureSay(Creature* creature, SpeakClasses type, const std::string& text)
{
	if (creatureSayEvent == -1) {
		return;
	}

	//onCreatureSay(creature, type, msg)
	if (!scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onCreatureSay] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = scriptInterface->getScriptEnv();
	env->setScriptId(creatureSayEvent, scriptInterface);
	env->setNpc(npc);

	lua_State* L = scriptInterface->getLuaState();
	scriptInterface->pushFunction(creatureSayEvent);
	LuaScriptInterface::pushUserdata<Creature>(L, creature);
	LuaScriptInterface::setCreatureMetatable(L, -1, creature);
	lua_pushnumber(L, type);
	LuaScriptInterface::pushString(L, text);
	scriptInterface->callFunction(3);
}

void NpcEventsHandler::onPlayerTrade(Player* player, int32_t callback, uint16_t itemId,
                              uint8_t count, uint8_t amount, bool ignore, bool inBackpacks)
{
	if (callback == -1) {
		return;
	}

	//onBuy(player, itemid, count, amount, ignore, inbackpacks)
	if (!scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onPlayerTrade] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = scriptInterface->getScriptEnv();
	env->setScriptId(-1, scriptInterface);
	env->setNpc(npc);

	lua_State* L = scriptInterface->getLuaState();
	LuaScriptInterface::pushCallback(L, callback);
	LuaScriptInterface::pushUserdata<Player>(L, player);
	LuaScriptInterface::setMetatable(L, -1, "Player");
	lua_pushnumber(L, itemId);
	lua_pushnumber(L, count);
	lua_pushnumber(L, amount);
	LuaScriptInterface::pushBoolean(L, ignore);
	LuaScriptInterface::pushBoolean(L, inBackpacks);
	scriptInterface->callFunction(6);
}

void NpcEventsHandler::onPlayerCloseChannel(Player* player)
{
	if (playerCloseChannelEvent == -1) {
		return;
	}

	//onPlayerCloseChannel(player)
	if (!scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onPlayerCloseChannel] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = scriptInterface->getScriptEnv();
	env->setScriptId(playerCloseChannelEvent, scriptInterface);
	env->setNpc(npc);

	lua_State* L = scriptInterface->getLuaState();
	scriptInterface->pushFunction(playerCloseChannelEvent);
	LuaScriptInterface::pushUserdata<Player>(L, player);
	LuaScriptInterface::setMetatable(L, -1, "Player");
	scriptInterface->callFunction(1);
}

void NpcEventsHandler::onPlayerEndTrade(Player* player)
{
	if (playerEndTradeEvent == -1) {
		return;
	}

	//onPlayerEndTrade(player)
	if (!scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onPlayerEndTrade] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = scriptInterface->getScriptEnv();
	env->setScriptId(playerEndTradeEvent, scriptInterface);
	env->setNpc(npc);

	lua_State* L = scriptInterface->getLuaState();
	scriptInterface->pushFunction(playerEndTradeEvent);
	LuaScriptInterface::pushUserdata<Player>(L, player);
	LuaScriptInterface::setMetatable(L, -1, "Player");
	scriptInterface->callFunction(1);
}

void NpcEventsHandler::onThink()
{
	if (thinkEvent == -1) {
		return;
	}

	//onThink()
	if (!scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onThink] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = scriptInterface->getScriptEnv();
	env->setScriptId(thinkEvent, scriptInterface);
	env->setNpc(npc);

	scriptInterface->pushFunction(thinkEvent);
	scriptInterface->callFunction(0);
}
