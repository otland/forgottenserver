// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#include "otpch.h"

#include "npc.h"

#include "game.h"
#include "pugicast.h"
#include "spectators.h"

extern Game g_game;
extern LuaEnvironment g_luaEnvironment;

uint32_t Npc::npcAutoID = 0x20000000;

void Npcs::reload()
{
	const std::map<uint32_t, std::shared_ptr<Npc>>& npcs = g_game.getNpcs();
	for (const auto& it : npcs) {
		it.second->closeAllShopWindows();
	}

	for (const auto& it : npcs) {
		it.second->reload();
	}
}

std::shared_ptr<Npc> Npc::createNpc(const std::string& name)
{
	auto npc = std::make_shared<Npc>(name);
	if (!npc->load()) {
		return nullptr;
	}
	return npc;
}

Npc::Npc(const std::string& name) : Creature(), filename("data/npc/" + name + ".xml"), masterRadius(-1), loaded(false)
{
	reset();
}

void Npc::addList() { g_game.addNpc(getNpc()); }

void Npc::removeList() { g_game.removeNpc(getNpc()); }

bool Npc::load()
{
	if (loaded) {
		return true;
	}

	reset();

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

	npcEventHandler.reset();

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
		assert(player->getPlayer() != nullptr);
		spectators.insert(std::static_pointer_cast<Player>(player));
	}

	const bool hasSpectators = !spectators.empty();
	setIdle(!hasSpectators);

	if (hasSpectators && walkTicks > 0) {
		addEventWalk();
	}

	// Simulate that the creature is placed on the map again.
	if (npcEventHandler) {
		npcEventHandler->onCreatureAppear(getNpc());
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
		setSkull(getSkullType(boost::algorithm::to_lower_copy<std::string>(attr.as_string())));
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
			std::cout << "[Warning - Npc::loadFromXml] Health now is greater than health max in " << filename
			          << std::endl;
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
		auto handler = std::make_unique<NpcEventsHandler>(scriptFile.as_string(), getNpc());
		if (!handler->isLoaded()) {
			return false;
		}
		npcEventHandler = std::move(handler);
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

void Npc::goToFollowCreature()
{
	if (!followCreature) {
		return;
	}

	FindPathParams fpp;
	getPathSearchParams(followCreature, fpp);
	updateFollowCreaturePath(fpp);
}

void Npc::onCreatureAppear(const std::shared_ptr<Creature>& creature, bool isLogin)
{
	Creature::onCreatureAppear(creature, isLogin);

	if (creature.get() == this) {
		SpectatorVec players;
		g_game.map.getSpectators(players, getPosition(), true, true);
		for (const auto& player : players) {
			assert(player->getPlayer() != nullptr);
			spectators.insert(std::static_pointer_cast<Player>(player));
		}

		const bool hasSpectators = !spectators.empty();
		setIdle(!hasSpectators);

		if (hasSpectators && walkTicks > 0) {
			addEventWalk();
		}

		if (npcEventHandler) {
			npcEventHandler->onCreatureAppear(creature);
		}
	} else if (const auto& player = creature->getPlayer()) {
		if (npcEventHandler) {
			npcEventHandler->onCreatureAppear(creature);
		}

		spectators.insert(player);
		setIdle(false);
	}
}

void Npc::onRemoveCreature(const std::shared_ptr<Creature>& creature, bool isLogout)
{
	Creature::onRemoveCreature(creature, isLogout);

	if (creature.get() == this) {
		closeAllShopWindows();
		if (npcEventHandler) {
			npcEventHandler->onCreatureDisappear(creature);
		}
	} else if (const auto& player = creature->getPlayer()) {
		if (npcEventHandler) {
			npcEventHandler->onCreatureDisappear(creature);
		}

		spectators.erase(player);
		setIdle(spectators.empty());
	}
}

void Npc::onCreatureMove(const std::shared_ptr<Creature>& creature, const std::shared_ptr<const Tile>& newTile,
                         const Position& newPos, const std::shared_ptr<const Tile>& oldTile, const Position& oldPos,
                         bool teleport)
{
	Creature::onCreatureMove(creature, newTile, newPos, oldTile, oldPos, teleport);

	if (creature.get() == this || creature->getPlayer()) {
		if (npcEventHandler) {
			npcEventHandler->onCreatureMove(creature, oldPos, newPos);
		}

		if (creature.get() != this) {
			const auto& player = creature->getPlayer();

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

void Npc::onCreatureSay(const std::shared_ptr<Creature>& creature, SpeakClasses type, const std::string& text)
{
	if (creature.get() == this) {
		return;
	}

	// only players for script events
	const auto& player = creature->getPlayer();
	if (player) {
		if (npcEventHandler) {
			npcEventHandler->onCreatureSay(player, type, text);
		}
	}
}

void Npc::onPlayerCloseChannel(const std::shared_ptr<Player>& player)
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

void Npc::doSay(const std::string& text) { g_game.internalCreatureSay(getNpc(), TALKTYPE_SAY, text, false); }

void Npc::doSayToPlayer(const std::shared_ptr<Player>& player, const std::string& text)
{
	if (player) {
		player->sendCreatureSay(getNpc(), TALKTYPE_PRIVATE_NP, text);
		player->onCreatureSay(getNpc(), TALKTYPE_PRIVATE_NP, text);
	}
}

void Npc::onPlayerTrade(const std::shared_ptr<Player>& player, int32_t callback, uint16_t itemId, uint8_t count,
                        uint16_t amount, bool ignore /* = false*/, bool inBackpacks /* = false*/)
{
	if (npcEventHandler) {
		npcEventHandler->onPlayerTrade(player, callback, itemId, count, amount, ignore, inBackpacks);
	}
	player->sendSaleItemList();
}

void Npc::onPlayerEndTrade(const std::shared_ptr<Player>& player, int32_t buyCallback, int32_t sellCallback)
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

	if (isRemoved() || isDead()) {
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

	auto tile = g_game.map.getTile(toPos);
	if (!tile || tile->queryAdd(0, getNpc(), 1, 0) != RETURNVALUE_NOERROR) {
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

bool Npc::getRandomStep(Direction& direction) const
{
	const Position& creaturePos = getPosition();
	for (Direction dir : getShuffleDirections()) {
		if (canWalkTo(creaturePos, dir)) {
			direction = dir;
			return true;
		}
	}
	return false;
}

bool Npc::doMoveTo(const Position& pos, int32_t minTargetDist /* = 1*/, int32_t maxTargetDist /* = 1*/,
                   bool fullPathSearch /* = true*/, bool clearSight /* = true*/, int32_t maxSearchDist /* = 0*/)
{
	listWalkDir.clear();
	if (getPathTo(pos, listWalkDir, minTargetDist, maxTargetDist, fullPathSearch, clearSight, maxSearchDist)) {
		startAutoWalk();
		return true;
	}
	return false;
}

void Npc::turnToCreature(const std::shared_ptr<Creature>& creature)
{
	const Position& creaturePos = creature->getPosition();
	const Position& myPos = getPosition();
	const auto dx = myPos.getOffsetX(creaturePos);
	const auto dy = myPos.getOffsetY(creaturePos);

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
	g_game.internalCreatureTurn(getNpc(), dir);
}

void Npc::setCreatureFocus(const std::shared_ptr<Creature>& creature)
{
	if (creature) {
		focusCreature = creature->getID();
		turnToCreature(creature);
	} else {
		focusCreature = 0;
	}
}

void Npc::addShopPlayer(const std::shared_ptr<Player>& player) { shopPlayerSet.insert(player); }

void Npc::removeShopPlayer(const std::shared_ptr<Player>& player) { shopPlayerSet.erase(player); }

void Npc::closeAllShopWindows()
{
	while (!shopPlayerSet.empty()) {
		auto player = *shopPlayerSet.begin();
		if (!player->closeShopWindow()) {
			removeShopPlayer(player);
		}
	}
}

NpcScriptInterface::NpcScriptInterface() : LuaScriptInterface("Npc interface")
{
	libLoaded = false;
	initState();
}

bool NpcScriptInterface::initState()
{
	L = g_luaEnvironment.getLuaState();
	if (!L) {
		return false;
	}

	registerFunctions();

	lua_newtable(L);
	eventTableRef = luaL_ref(L, LUA_REGISTRYINDEX);
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
	// npc exclusive functions
	lua_register(L, "selfSay", NpcScriptInterface::luaActionSay);
	lua_register(L, "selfMove", NpcScriptInterface::luaActionMove);
	lua_register(L, "selfMoveTo", NpcScriptInterface::luaActionMoveTo);
	lua_register(L, "selfTurn", NpcScriptInterface::luaActionTurn);
	lua_register(L, "selfFollow", NpcScriptInterface::luaActionFollow);
	lua_register(L, "getDistanceTo", NpcScriptInterface::luagetDistanceTo);
	lua_register(L, "doNpcSetCreatureFocus", NpcScriptInterface::luaSetNpcFocus);
	lua_register(L, "getNpcCid", NpcScriptInterface::luaGetNpcCid);
	lua_register(L, "getNpcParameter", NpcScriptInterface::luaGetNpcParameter);
	lua_register(L, "openShopWindow", NpcScriptInterface::luaOpenShopWindow);
	lua_register(L, "closeShopWindow", NpcScriptInterface::luaCloseShopWindow);
	lua_register(L, "doSellItem", NpcScriptInterface::luaDoSellItem);

	// metatable
	tfs::lua::registerMethod(L, "Npc", "getParameter", NpcScriptInterface::luaNpcGetParameter);
	tfs::lua::registerMethod(L, "Npc", "setFocus", NpcScriptInterface::luaNpcSetFocus);

	tfs::lua::registerMethod(L, "Npc", "openShopWindow", NpcScriptInterface::luaNpcOpenShopWindow);
	tfs::lua::registerMethod(L, "Npc", "closeShopWindow", NpcScriptInterface::luaNpcCloseShopWindow);
}

int NpcScriptInterface::luaActionSay(lua_State* L)
{
	// selfSay(words[, target])
	const auto& npc = tfs::lua::getScriptEnv()->getNpc();
	if (!npc) {
		return 0;
	}

	const std::string& text = tfs::lua::getString(L, 1);
	if (lua_gettop(L) >= 2) {
		auto target = tfs::lua::getPlayer(L, 2);
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
	// selfMove(direction)
	const auto& npc = tfs::lua::getScriptEnv()->getNpc();
	if (npc) {
		g_game.internalMoveCreature(npc, tfs::lua::getNumber<Direction>(L, 1));
	}
	return 0;
}

int NpcScriptInterface::luaActionMoveTo(lua_State* L)
{
	// selfMoveTo(x, y, z[, minTargetDist = 1[, maxTargetDist = 1[, fullPathSearch = true[, clearSight = true[,
	// maxSearchDist = 0]]]]]) selfMoveTo(position[, minTargetDist = 1[, maxTargetDist = 1[, fullPathSearch = true[,
	// clearSight = true[, maxSearchDist = 0]]]]])
	const auto& npc = tfs::lua::getScriptEnv()->getNpc();
	if (!npc) {
		return 0;
	}

	Position position;
	int32_t argsStart = 2;
	if (lua_istable(L, 1)) {
		position = tfs::lua::getPosition(L, 1);
	} else {
		position.x = tfs::lua::getNumber<uint16_t>(L, 1);
		position.y = tfs::lua::getNumber<uint16_t>(L, 2);
		position.z = tfs::lua::getNumber<uint8_t>(L, 3);
		argsStart = 4;
	}

	tfs::lua::pushBoolean(
	    L,
	    npc->doMoveTo(position, tfs::lua::getNumber<int32_t>(L, argsStart, 1),
	                  tfs::lua::getNumber<int32_t>(L, argsStart + 1, 1), tfs::lua::getBoolean(L, argsStart + 2, true),
	                  tfs::lua::getBoolean(L, argsStart + 3, true), tfs::lua::getNumber<int32_t>(L, argsStart + 4, 0)));
	return 1;
}

int NpcScriptInterface::luaActionTurn(lua_State* L)
{
	// selfTurn(direction)
	const auto& npc = tfs::lua::getScriptEnv()->getNpc();
	if (npc) {
		g_game.internalCreatureTurn(npc, tfs::lua::getNumber<Direction>(L, 1));
	}
	return 0;
}

int NpcScriptInterface::luaActionFollow(lua_State* L)
{
	// selfFollow(player)
	const auto& npc = tfs::lua::getScriptEnv()->getNpc();
	if (!npc) {
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	auto followedPlayer = tfs::lua::getPlayer(L, 1);
	if (followedPlayer) {
		npc->setFollowCreature(followedPlayer);
		tfs::lua::pushBoolean(L, npc->canFollowCreature(followedPlayer));
	} else {
		npc->removeFollowCreature();
		tfs::lua::pushBoolean(L, true);
	}
	return 1;
}

int NpcScriptInterface::luagetDistanceTo(lua_State* L)
{
	// getDistanceTo(uid)
	ScriptEnvironment* env = tfs::lua::getScriptEnv();

	const auto& npc = env->getNpc();
	if (!npc) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_THING_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	uint32_t uid = tfs::lua::getNumber<uint32_t>(L, -1);

	auto thing = env->getThingByUID(uid);
	if (!thing) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_THING_NOT_FOUND));
		lua_pushnil(L);
		return 1;
	}

	const Position& thingPos = thing->getPosition();
	const Position& npcPos = npc->getPosition();
	if (npcPos.z != thingPos.z) {
		lua_pushnumber(L, -1);
	} else {
		lua_pushnumber(L, std::max(npcPos.getDistanceX(thingPos), npcPos.getDistanceY(thingPos)));
	}
	return 1;
}

int NpcScriptInterface::luaSetNpcFocus(lua_State* L)
{
	// doNpcSetCreatureFocus(cid)
	const auto& npc = tfs::lua::getScriptEnv()->getNpc();
	if (npc) {
		npc->setCreatureFocus(tfs::lua::getCreature(L, -1));
	}
	return 0;
}

int NpcScriptInterface::luaGetNpcCid(lua_State* L)
{
	// getNpcCid()
	const auto& npc = tfs::lua::getScriptEnv()->getNpc();
	if (npc) {
		lua_pushnumber(L, npc->getID());
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int NpcScriptInterface::luaGetNpcParameter(lua_State* L)
{
	// getNpcParameter(paramKey)
	const auto& npc = tfs::lua::getScriptEnv()->getNpc();
	if (!npc) {
		lua_pushnil(L);
		return 1;
	}

	std::string paramKey = tfs::lua::getString(L, -1);

	auto it = npc->parameters.find(paramKey);
	if (it != npc->parameters.end()) {
		tfs::lua::pushString(L, it->second);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int NpcScriptInterface::luaOpenShopWindow(lua_State* L)
{
	// openShopWindow(cid, items, onBuy callback, onSell callback)
	int32_t sellCallback;
	if (!lua_isfunction(L, -1)) {
		sellCallback = -1;
		lua_pop(L, 1); // skip it - use default value
	} else {
		sellCallback = tfs::lua::popCallback(L);
	}

	int32_t buyCallback;
	if (!lua_isfunction(L, -1)) {
		buyCallback = -1;
		lua_pop(L, 1); // skip it - use default value
	} else {
		buyCallback = tfs::lua::popCallback(L);
	}

	if (!lua_istable(L, -1)) {
		reportErrorFunc(L, "item list is not a table.");
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	std::list<ShopInfo> items;
	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		const auto tableIndex = lua_gettop(L);
		ShopInfo item;

		item.itemId = tfs::lua::getField<uint32_t>(L, tableIndex, "id");
		item.subType = tfs::lua::getField<int32_t>(L, tableIndex, "subType");
		if (item.subType == 0) {
			item.subType = tfs::lua::getField<int32_t>(L, tableIndex, "subtype");
			lua_pop(L, 1);
		}

		item.buyPrice = tfs::lua::getField<int64_t>(L, tableIndex, "buy");
		item.sellPrice = tfs::lua::getField<int64_t>(L, tableIndex, "sell");
		item.realName = tfs::lua::getFieldString(L, tableIndex, "name");

		items.push_back(item);
		lua_pop(L, 6);
	}
	lua_pop(L, 1);

	auto player = tfs::lua::getPlayer(L, -1);
	if (!player) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	// Close any eventual other shop window currently open.
	player->closeShopWindow(false);

	const auto& npc = tfs::lua::getScriptEnv()->getNpc();
	if (!npc) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	npc->addShopPlayer(player);
	player->setShopOwner(npc, buyCallback, sellCallback);
	player->openShopWindow(npc, items);

	tfs::lua::pushBoolean(L, true);
	return 1;
}

int NpcScriptInterface::luaCloseShopWindow(lua_State* L)
{
	// closeShopWindow(cid)
	const auto& npc = tfs::lua::getScriptEnv()->getNpc();
	if (!npc) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	auto player = tfs::lua::getPlayer(L, 1);
	if (!player) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	int32_t buyCallback;
	int32_t sellCallback;

	auto merchant = player->getShopOwner(buyCallback, sellCallback);

	// Check if we actually have a shop window with this player.
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

	tfs::lua::pushBoolean(L, true);
	return 1;
}

int NpcScriptInterface::luaDoSellItem(lua_State* L)
{
	// doSellItem(cid, itemid, amount, <optional> subtype, <optional> actionid, <optional: default: 1> canDropOnMap)
	auto player = tfs::lua::getPlayer(L, 1);
	if (!player) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	uint32_t sellCount = 0;

	uint32_t itemId = tfs::lua::getNumber<uint32_t>(L, 2);
	uint32_t amount = tfs::lua::getNumber<uint32_t>(L, 3);
	uint32_t subType;

	int32_t n = tfs::lua::getNumber<int32_t>(L, 4, -1);
	if (n != -1) {
		subType = n;
	} else {
		subType = 1;
	}

	uint32_t actionId = tfs::lua::getNumber<uint32_t>(L, 5, 0);
	bool canDropOnMap = tfs::lua::getBoolean(L, 6, true);

	const ItemType& it = Item::items[itemId];
	if (it.stackable) {
		while (amount > 0) {
			int32_t stackCount = std::min<int32_t>(ITEM_STACK_SIZE, amount);
			auto item = Item::CreateItem(it.id, stackCount);
			if (item && actionId != 0) {
				item->setActionId(actionId);
			}

			if (g_game.internalPlayerAddItem(player, item, canDropOnMap) != RETURNVALUE_NOERROR) {
				lua_pushnumber(L, sellCount);
				return 1;
			}

			amount -= stackCount;
			sellCount += stackCount;
		}
	} else {
		for (uint32_t i = 0; i < amount; ++i) {
			auto item = Item::CreateItem(it.id, subType);
			if (item && actionId != 0) {
				item->setActionId(actionId);
			}

			if (g_game.internalPlayerAddItem(player, item, canDropOnMap) != RETURNVALUE_NOERROR) {
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
	const std::string& key = tfs::lua::getString(L, 2);
	auto npc = tfs::lua::getSharedPtr<Npc>(L, 1);
	if (npc) {
		auto it = npc->parameters.find(key);
		if (it != npc->parameters.end()) {
			tfs::lua::pushString(L, it->second);
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
	const std::shared_ptr<Creature>& creature = tfs::lua::getCreature(L, 2);
	auto npc = tfs::lua::getSharedPtr<Npc>(L, 1);
	if (npc) {
		npc->setCreatureFocus(creature);
		tfs::lua::pushBoolean(L, true);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int NpcScriptInterface::luaNpcOpenShopWindow(lua_State* L)
{
	// npc:openShopWindow(cid, items, buyCallback, sellCallback)
	if (!lua_istable(L, 3)) {
		reportErrorFunc(L, "item list is not a table.");
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	auto player = tfs::lua::getPlayer(L, 2);
	if (!player) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	auto npc = tfs::lua::getSharedPtr<Npc>(L, 1);
	if (!npc) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	int32_t sellCallback = -1;
	if (lua_isfunction(L, 5)) {
		sellCallback = luaL_ref(L, LUA_REGISTRYINDEX);
	}

	int32_t buyCallback = -1;
	if (lua_isfunction(L, 4)) {
		buyCallback = luaL_ref(L, LUA_REGISTRYINDEX);
	}

	std::list<ShopInfo> items;

	lua_pushnil(L);
	while (lua_next(L, 3) != 0) {
		const auto tableIndex = lua_gettop(L);
		ShopInfo item;

		item.itemId = tfs::lua::getField<uint32_t>(L, tableIndex, "id");
		item.subType = tfs::lua::getField<int32_t>(L, tableIndex, "subType");
		if (item.subType == 0) {
			item.subType = tfs::lua::getField<int32_t>(L, tableIndex, "subtype");
			lua_pop(L, 1);
		}

		item.buyPrice = tfs::lua::getField<int64_t>(L, tableIndex, "buy");
		item.sellPrice = tfs::lua::getField<int64_t>(L, tableIndex, "sell");
		item.realName = tfs::lua::getFieldString(L, tableIndex, "name");

		items.push_back(item);
		lua_pop(L, 6);
	}
	lua_pop(L, 1);

	player->closeShopWindow(false);
	npc->addShopPlayer(player);

	player->setShopOwner(npc, buyCallback, sellCallback);
	player->openShopWindow(npc, items);

	tfs::lua::pushBoolean(L, true);
	return 1;
}

int NpcScriptInterface::luaNpcCloseShopWindow(lua_State* L)
{
	// npc:closeShopWindow(player)
	auto player = tfs::lua::getPlayer(L, 2);
	if (!player) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_PLAYER_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	auto npc = tfs::lua::getSharedPtr<Npc>(L, 1);
	if (!npc) {
		reportErrorFunc(L, tfs::lua::getErrorDesc(LUA_ERROR_CREATURE_NOT_FOUND));
		tfs::lua::pushBoolean(L, false);
		return 1;
	}

	int32_t buyCallback;
	int32_t sellCallback;

	auto merchant = player->getShopOwner(buyCallback, sellCallback);
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

	tfs::lua::pushBoolean(L, true);
	return 1;
}

NpcEventsHandler::NpcEventsHandler(const std::string& file, std::shared_ptr<Npc> npc) :
    scriptInterface{std::make_unique<NpcScriptInterface>()}, npc{std::move(npc)}
{
	if (!scriptInterface->loadNpcLib("data/npc/lib/npc.lua")) {
		std::cout << "[Warning - NpcLib::NpcLib] Can not load lib: " << file << std::endl;
		std::cout << scriptInterface->getLastLuaError() << std::endl;
		return;
	}

	loaded = scriptInterface->loadFile("data/npc/scripts/" + file, this->npc) == 0;
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

bool NpcEventsHandler::isLoaded() const { return loaded; }

void NpcEventsHandler::onCreatureAppear(const std::shared_ptr<Creature>& creature)
{
	if (creatureAppearEvent == -1) {
		return;
	}

	// onCreatureAppear(creature)
	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onCreatureAppear] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(creatureAppearEvent, scriptInterface.get());
	env->setNpc(npc);

	lua_State* L = scriptInterface->getLuaState();
	scriptInterface->pushFunction(creatureAppearEvent);
	tfs::lua::pushSharedPtr(L, creature);
	tfs::lua::setCreatureMetatable(L, -1, creature);
	scriptInterface->callFunction(1);
}

void NpcEventsHandler::onCreatureDisappear(const std::shared_ptr<Creature>& creature)
{
	if (creatureDisappearEvent == -1) {
		return;
	}

	// onCreatureDisappear(creature)
	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onCreatureDisappear] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(creatureDisappearEvent, scriptInterface.get());
	env->setNpc(npc);

	lua_State* L = scriptInterface->getLuaState();
	scriptInterface->pushFunction(creatureDisappearEvent);
	tfs::lua::pushSharedPtr(L, creature);
	tfs::lua::setCreatureMetatable(L, -1, creature);
	scriptInterface->callFunction(1);
}

void NpcEventsHandler::onCreatureMove(const std::shared_ptr<Creature>& creature, const Position& oldPos,
                                      const Position& newPos)
{
	if (creatureMoveEvent == -1) {
		return;
	}

	// onCreatureMove(creature, oldPos, newPos)
	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onCreatureMove] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(creatureMoveEvent, scriptInterface.get());
	env->setNpc(npc);

	lua_State* L = scriptInterface->getLuaState();
	scriptInterface->pushFunction(creatureMoveEvent);
	tfs::lua::pushSharedPtr(L, creature);
	tfs::lua::setCreatureMetatable(L, -1, creature);
	tfs::lua::pushPosition(L, oldPos);
	tfs::lua::pushPosition(L, newPos);
	scriptInterface->callFunction(3);
}

void NpcEventsHandler::onCreatureSay(const std::shared_ptr<Creature>& creature, SpeakClasses type,
                                     const std::string& text)
{
	if (creatureSayEvent == -1) {
		return;
	}

	// onCreatureSay(creature, type, msg)
	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onCreatureSay] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(creatureSayEvent, scriptInterface.get());
	env->setNpc(npc);

	lua_State* L = scriptInterface->getLuaState();
	scriptInterface->pushFunction(creatureSayEvent);
	tfs::lua::pushSharedPtr(L, creature);
	tfs::lua::setCreatureMetatable(L, -1, creature);
	lua_pushnumber(L, type);
	tfs::lua::pushString(L, text);
	scriptInterface->callFunction(3);
}

void NpcEventsHandler::onPlayerTrade(const std::shared_ptr<Player>& player, int32_t callback, uint16_t itemId,
                                     uint8_t count, uint16_t amount, bool ignore, bool inBackpacks)
{
	if (callback == -1) {
		return;
	}

	// onBuy(player, itemid, count, amount, ignore, inbackpacks)
	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onPlayerTrade] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(-1, scriptInterface.get());
	env->setNpc(npc);

	lua_State* L = scriptInterface->getLuaState();
	tfs::lua::pushCallback(L, callback);
	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");
	lua_pushnumber(L, itemId);
	lua_pushnumber(L, count);
	lua_pushnumber(L, amount);
	tfs::lua::pushBoolean(L, ignore);
	tfs::lua::pushBoolean(L, inBackpacks);
	scriptInterface->callFunction(6);
}

void NpcEventsHandler::onPlayerCloseChannel(const std::shared_ptr<Player>& player)
{
	if (playerCloseChannelEvent == -1) {
		return;
	}

	// onPlayerCloseChannel(player)
	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onPlayerCloseChannel] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerCloseChannelEvent, scriptInterface.get());
	env->setNpc(npc);

	lua_State* L = scriptInterface->getLuaState();
	scriptInterface->pushFunction(playerCloseChannelEvent);
	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");
	scriptInterface->callFunction(1);
}

void NpcEventsHandler::onPlayerEndTrade(const std::shared_ptr<Player>& player)
{
	if (playerEndTradeEvent == -1) {
		return;
	}

	// onPlayerEndTrade(player)
	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onPlayerEndTrade] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(playerEndTradeEvent, scriptInterface.get());
	env->setNpc(npc);

	lua_State* L = scriptInterface->getLuaState();
	scriptInterface->pushFunction(playerEndTradeEvent);
	tfs::lua::pushSharedPtr(L, player);
	tfs::lua::setMetatable(L, -1, "Player");
	scriptInterface->callFunction(1);
}

void NpcEventsHandler::onThink()
{
	if (thinkEvent == -1) {
		return;
	}

	// onThink()
	if (!tfs::lua::reserveScriptEnv()) {
		std::cout << "[Error - NpcScript::onThink] Call stack overflow" << std::endl;
		return;
	}

	ScriptEnvironment* env = tfs::lua::getScriptEnv();
	env->setScriptId(thinkEvent, scriptInterface.get());
	env->setNpc(npc);

	scriptInterface->pushFunction(thinkEvent);
	scriptInterface->callFunction(0);
}
