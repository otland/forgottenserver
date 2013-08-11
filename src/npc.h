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

#ifndef __OTSERV_NPC_H__
#define __OTSERV_NPC_H__

#include "creature.h"
#include "luascript.h"

#include <set>

class Npc;
class Player;

class Npcs
{
	public:
		Npcs() {}
		~Npcs() {}
		void reload();
};

class NpcScriptInterface : public LuaScriptInterface
{
	public:
		NpcScriptInterface();
		virtual ~NpcScriptInterface();

		bool loadNpcLib(const std::string& file);

	protected:
		virtual void registerFunctions();

		static int32_t luaActionSay(lua_State* L);
		static int32_t luaActionMove(lua_State* L);
		static int32_t luaActionMoveTo(lua_State* L);
		static int32_t luaActionTurn(lua_State* L);
		static int32_t luaActionFollow(lua_State* L);
		static int32_t luaSelfGetPos(lua_State* L);
		static int32_t luagetDistanceTo(lua_State* L);
		static int32_t luaSetNpcFocus(lua_State* L);
		static int32_t luaGetNpcCid(lua_State* L);
		static int32_t luaGetNpcPos(lua_State* L);
		static int32_t luaGetNpcName(lua_State* L);
		static int32_t luaGetNpcParameter(lua_State* L);
		static int32_t luaOpenShopWindow(lua_State* L);
		static int32_t luaCloseShopWindow(lua_State* L);
		static int32_t luaDoSellItem(lua_State* L);

		// metatable
		static int32_t luaNpcGetParameter(lua_State* L);
		static int32_t luaNpcSetFocus(lua_State* L);

		static int32_t luaNpcDoOpenShopWindow(lua_State* L);
		static int32_t luaNpcDoCloseShopWindow(lua_State* L);

	private:
		virtual bool initState();
		virtual bool closeState();

		bool m_libLoaded;
};

class NpcEventsHandler
{
	public:
		NpcEventsHandler(Npc* npc);
		virtual ~NpcEventsHandler();

		virtual void onCreatureAppear(const Creature* creature) {}
		virtual void onCreatureDisappear(const Creature* creature) {}
		virtual void onCreatureMove(const Creature* creature, const Position& oldPos, const Position& newPos) {}
		virtual void onCreatureSay(const Creature* creature, SpeakClasses, const std::string& text, Position* pos = NULL) {}
		virtual void onPlayerTrade(const Player* player, int32_t callback, uint16_t itemid,
		                           uint8_t count, uint8_t amount, bool ignore = false, bool inBackpacks = false) {}
		virtual void onPlayerCloseChannel(const Player* player) {}
		virtual void onPlayerEndTrade(const Player* player) {}
		virtual void onThink() {}

		bool isLoaded() const;

	protected:
		Npc* m_npc;
		bool m_loaded;
};

class NpcScript : public NpcEventsHandler
{
	public:
		NpcScript(const std::string& file, Npc* npc);
		NpcScript(Npc* npc);
		virtual ~NpcScript();

		virtual void onCreatureAppear(const Creature* creature);
		virtual void onCreatureDisappear(const Creature* creature);
		virtual void onCreatureMove(const Creature* creature, const Position& oldPos, const Position& newPos);
		virtual void onCreatureSay(const Creature* creature, SpeakClasses, const std::string& text, Position* pos = NULL);
		virtual void onPlayerTrade(const Player* player, int32_t callback, uint16_t itemid,
		                           uint8_t count, uint8_t amount, bool ignore, bool inBackpacks);
		virtual void onPlayerCloseChannel(const Player* player);
		virtual void onPlayerEndTrade(const Player* player);
		virtual void onThink();

	private:
		NpcScriptInterface* m_scriptInterface;

		int32_t m_onCreatureAppear;
		int32_t m_onCreatureDisappear;
		int32_t m_onCreatureMove;
		int32_t m_onCreatureSay;
		int32_t m_onPlayerCloseChannel;
		int32_t m_onPlayerEndTrade;
		int32_t m_onThink;
};

enum ShopEvent_t {
	SHOPEVENT_SELL,
	SHOPEVENT_BUY,
	SHOPEVENT_CLOSE
};

class Npc : public Creature
{
	public:
#ifdef __ENABLE_SERVER_DIAGNOSTIC__
		static uint32_t npcCount;
#endif

		virtual ~Npc();

		virtual Npc* getNpc() {
			return this;
		}
		virtual const Npc* getNpc() const {
			return this;
		}

		virtual bool isPushable() const {
			return walkTicks > 0;
		}

		void setID() {
			if (this->id == 0) {
				this->id = npcAutoID++;
			}
		}

		void removeList();
		void addList();

		static Npc* createNpc(const std::string& name);

		virtual bool canSee(const Position& pos) const;

		bool load();
		void reload();

		virtual const std::string& getName() const {
			return name;
		}
		virtual const std::string& getNameDescription() const {
			return name;
		}

		virtual CreatureType_t getType() const {
			return CREATURETYPE_NPC;
		}

		void doSay(const std::string& text);
		void doSayToPlayer(Player* player, const std::string& text);

		void doMove(Direction dir);
		void doTurn(Direction dir);
		void doMoveTo(const Position& pos);
		bool isLoaded() const {
			return loaded;
		}

		int32_t getMasterRadius() const {
			return masterRadius;
		}
		const Position& getMasterPos() const {
			return masterPos;
		}
		void setMasterPos(const Position& pos, uint32_t radius = 1) {
			masterPos = pos;
			if (masterRadius == -1) {
				masterRadius = radius;
			}
		}

		void onPlayerCloseChannel(const Player* player);
		void onPlayerTrade(Player* player, ShopEvent_t type, int32_t callback, uint16_t itemId,
		                   uint8_t count, uint8_t amount, bool ignore = false, bool inBackpacks = false);
		void onPlayerEndTrade(Player* player, int32_t buyCallback, int32_t sellCallback);

		void turnToCreature(Creature* creature);
		void setCreatureFocus(Creature* creature);

		NpcScriptInterface* getScriptInterface();

	protected:
		Npc(const std::string& _name);

		virtual void onCreatureAppear(const Creature* creature, bool isLogin);
		virtual void onCreatureDisappear(const Creature* creature, uint32_t stackpos, bool isLogout);
		virtual void onCreatureMove(const Creature* creature, const Tile* newTile, const Position& newPos,
		                            const Tile* oldTile, const Position& oldPos, bool teleport);

		virtual void onCreatureSay(const Creature* creature, SpeakClasses type, const std::string& text, Position* pos = NULL);
		virtual void onThink(uint32_t interval);
		virtual std::string getDescription(int32_t lookDistance) const;

		bool isImmune(CombatType_t type) const {
			return true;
		}
		bool isImmune(ConditionType_t type) const {
			return true;
		}
		virtual bool isAttackable() const {
			return attackable;
		}
		virtual bool getNextStep(Direction& dir, uint32_t& flags);

		bool canWalkTo(const Position& fromPos, Direction dir);
		bool getRandomStep(Direction& dir);

		void reset();
		bool loadFromXml(const std::string& name);

		typedef std::map<std::string, std::string> ParametersMap;
		ParametersMap m_parameters;

		uint32_t loadParams(xmlNodePtr node);

		void addShopPlayer(Player* player);
		void removeShopPlayer(Player* player);
		void closeAllShopWindows();

		std::set<Player*> shopPlayerSet;

		std::string name;
		std::string m_filename;

		NpcEventsHandler* m_npcEventHandler;

		uint32_t walkTicks;
		int32_t focusCreature;
		int32_t masterRadius;

		Position masterPos;

		bool floorChange;
		bool attackable;
		bool loaded;

		static uint32_t npcAutoID;

		static NpcScriptInterface* m_scriptInterface;

		friend class Npcs;
		friend class NpcScriptInterface;
};

#endif
