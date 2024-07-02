// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_NPC_H
#define FS_NPC_H

#include "creature.h"
#include "luascript.h"

class Npc;
class Player;

class NpcScriptInterface final : public LuaScriptInterface
{
public:
	NpcScriptInterface();

	bool loadNpcLib(const std::string& file);
	int32_t loadFile(const std::string& file, Npc* npc = nullptr);

private:
	void registerFunctions();

	static int luaActionSay(lua_State* L);
	static int luaActionMove(lua_State* L);
	static int luaActionMoveTo(lua_State* L);
	static int luaActionTurn(lua_State* L);
	static int luaActionFollow(lua_State* L);
	static int luagetDistanceTo(lua_State* L);
	static int luaSetNpcFocus(lua_State* L);
	static int luaGetNpcCid(lua_State* L);
	static int luaGetNpcParameter(lua_State* L);
	static int luaOpenShopWindow(lua_State* L);
	static int luaCloseShopWindow(lua_State* L);
	static int luaDoSellItem(lua_State* L);

	// metatable
	static int luaNpcGetParameter(lua_State* L);
	static int luaNpcSetFocus(lua_State* L);

	static int luaNpcOpenShopWindow(lua_State* L);
	static int luaNpcCloseShopWindow(lua_State* L);

private:
	bool initState() override;
	bool closeState() override;

	bool libLoaded;
};

class NpcEventsHandler
{
public:
	NpcEventsHandler(const std::string& file, Npc* npc);
	NpcEventsHandler();
	~NpcEventsHandler();

	void onCreatureAppear(Creature* creature);
	void onCreatureDisappear(Creature* creature);
	void onCreatureMove(Creature* creature, const Position& oldPos, const Position& newPos);
	void onCreatureSay(Creature* creature, SpeakClasses, const std::string& text);
	void onPlayerTrade(Player* player, int32_t callback, uint16_t itemId, uint8_t count, uint16_t amount,
	                   bool ignore = false, bool inBackpacks = false);
	void onPlayerCloseChannel(Player* player);
	void onPlayerEndTrade(Player* player);
	void onThink();
	void onCreatureSight(Creature* creature);
	void onSpeechBubble(Player* player, uint8_t& speechBubble);

	void setNpc(Npc* n) { npc = n; };

	bool isLoaded() const;

	int32_t creatureAppearEvent = -1;
	int32_t creatureDisappearEvent = -1;
	int32_t creatureMoveEvent = -1;
	int32_t creatureSayEvent = -1;
	int32_t playerCloseChannelEvent = -1;
	int32_t playerEndTradeEvent = -1;
	int32_t thinkEvent = -1;
	int32_t creatureSightEvent = -1;
	int32_t speechBubbleEvent = -1;

	std::shared_ptr<NpcScriptInterface> scriptInterface;
	friend class NpcScriptInterface;

private:
	Npc* npc;
	bool loaded = false;
};

class NpcType
{
public:
	NpcType() = default;

	// non-copyable
	NpcType(const NpcType&) = delete;
	NpcType& operator=(const NpcType&) = delete;

	bool loadCallback(NpcScriptInterface* scriptInterface);

	uint8_t speechBubble = SPEECHBUBBLE_NONE;
	uint16_t sightX = 0;
	uint16_t sightY = 0;

	uint32_t walkTicks = 1500;
	uint32_t baseSpeed = 100;

	int32_t masterRadius = 2;
	int32_t health = 1000;
	int32_t healthMax = 1000;

	bool floorChange = false;
	bool attackable = false;
	bool ignoreHeight = false;
	bool loaded = false;
	bool isIdle = true;
	bool pushable = true;

	Outfit_t defaultOutfit;
	Skulls_t skull = SKULL_NONE;

	std::map<std::string, std::string> parameters;

	bool loadFromXml();

	std::string name;
	std::string filename;
	std::string scriptFilename;
	bool fromLua = false;
	std::string eventType;

	std::unique_ptr<NpcEventsHandler> npcEventHandler = std::make_unique<NpcEventsHandler>();
};

namespace Npcs {
void load(bool reload = false);
void reload();
void addNpcType(const std::string& name, NpcType* npcType);
void clearNpcTypes();
std::map<const std::string, NpcType*> getNpcTypes();
NpcType* getNpcType(std::string name);
NpcScriptInterface* getScriptInterface();
bool loadNpcs(bool reload);
inline constexpr int32_t ViewportX = Map::maxClientViewportX * 2 + 2;
inline constexpr int32_t ViewportY = Map::maxClientViewportY * 2 + 2;
inline constexpr int32_t EVENT_ID_LOADING = 1;
} // namespace Npcs

class Npc final : public Creature
{
public:
	explicit Npc(const std::string& name);
	~Npc();

	// non-copyable
	Npc(const Npc&) = delete;
	Npc& operator=(const Npc&) = delete;

	using Creature::onWalk;

	Npc* getNpc() override { return this; }
	const Npc* getNpc() const override { return this; }

	bool isPushable() const override { return pushable && walkTicks != 0; }

	void setID() override
	{
		if (id == 0) {
			id = ++npcAutoID;
		}
	}

	void removeList() override;
	void addList() override;

	static Npc* createNpc(const std::string& name);

	bool canSee(const Position& pos) const override;

	bool load();
	void reload();
	void reset(bool reload = false);

	const std::string& getName() const override { return name; }
	void setName(const std::string& n) { name = n; }
	const std::string& getNameDescription() const override { return name; }

	CreatureType_t getType() const override { return CREATURETYPE_NPC; }

	uint8_t getSpeechBubble() const override { return speechBubble; }
	void setSpeechBubble(const uint8_t bubble) { speechBubble = bubble; }

	void doSay(const std::string& text, SpeakClasses talkType = TALKTYPE_SAY);
	void doSayToPlayer(Player* player, const std::string& text);

	bool doMoveTo(const Position& pos, int32_t minTargetDist = 1, int32_t maxTargetDist = 1, bool fullPathSearch = true,
	              bool clearSight = true, int32_t maxSearchDist = 0);

	int32_t getMasterRadius() const { return masterRadius; }
	const Position& getMasterPos() const { return masterPos; }
	void setMasterPos(Position pos, int32_t radius = 1)
	{
		masterPos = pos;
		if (masterRadius == -1) {
			masterRadius = radius;
		}
	}

	void onPlayerCloseChannel(Player* player);
	void onPlayerTrade(Player* player, int32_t callback, uint16_t itemId, uint8_t count, uint16_t amount,
	                   bool ignore = false, bool inBackpacks = false);
	void onPlayerEndTrade(Player* player, int32_t buyCallback, int32_t sellCallback);

	void turnToCreature(Creature* creature);
	void setCreatureFocus(Creature* creature);

	static uint32_t npcAutoID;

	const auto& getSpectators() { return spectators; }

	void loadNpcTypeInfo();

	std::unique_ptr<NpcEventsHandler> npcEventHandler;
	bool fromLua = false;
	NpcType* npcType;
	void closeAllShopWindows();
	void addShopPlayer(Player* player);
	void removeShopPlayer(Player* player);
	std::map<std::string, std::string> parameters;

private:
	void onCreatureAppear(Creature* creature, bool isLogin) override;
	void onRemoveCreature(Creature* creature, bool isLogout) override;
	void onCreatureMove(Creature* creature, const Tile* newTile, const Position& newPos, const Tile* oldTile,
	                    const Position& oldPos, bool teleport) override;

	void onCreatureSay(Creature* creature, SpeakClasses type, const std::string& text) override;
	void onThink(uint32_t interval) override;
	std::string getDescription(int32_t lookDistance) const override;

	bool isImmune(CombatType_t) const override { return !attackable; }
	bool isImmune(ConditionType_t) const override { return !attackable; }
	bool isAttackable() const override { return attackable; }
	bool getNextStep(Direction& dir, uint32_t& flags) override;

	void setIdle(const bool idle);

	bool canWalkTo(const Position& fromPos, Direction dir) const;
	bool getRandomStep(Direction& direction) const;

	std::set<Player*> shopPlayerSet;
	std::set<Player*> spectators;

	std::string name;
	std::string filename;

	Position masterPos;

	uint32_t walkTicks;
	int32_t focusCreature;
	int32_t masterRadius;
	uint16_t sightX;
	uint16_t sightY;

	uint8_t speechBubble;

	bool floorChange;
	bool attackable;
	bool ignoreHeight;
	bool loaded;
	bool isIdle;
	bool pushable;
	std::set<Creature*> spectatorCache;

	friend class NpcType;
};

#endif // FS_NPC_H
