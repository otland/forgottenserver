//////////////////////////////////////////////////////////////////////
// The Forgotten Server - a server application for the MMORPG Tibia
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#ifndef __OTSERV_NPC_H__
#define __OTSERV_NPC_H__

#include "creature.h"
#include "luascript.h"

class Npc;
class Player;
class NpcResponse;
struct NpcState;

typedef std::list<Npc*> NpcList;
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

		static void pushState(lua_State* L, NpcState* state);
		static void popState(lua_State* L, NpcState* &state);

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
		static int32_t luaGetNpcState(lua_State* L);
		static int32_t luaSetNpcState(lua_State* L);
		static int32_t luaGetNpcName(lua_State* L);
		static int32_t luaGetNpcParameter(lua_State* L);
		static int32_t luaOpenShopWindow(lua_State* L);
		static int32_t luaCloseShopWindow(lua_State* L);
		static int32_t luaDoSellItem(lua_State* L);

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

enum RespondParam_t {
	RESPOND_DEFAULT      =  0x0000,
	RESPOND_MALE         =  0x0001,
	RESPOND_FEMALE       =  0x0002,
	RESPOND_PZBLOCK      =  0x0004,
	RESPOND_LOWMONEY     =  0x0008,
	RESPOND_NOAMOUNT     =  0x0010,
	RESPOND_LOWAMOUNT    =  0x0020,
	RESPOND_PREMIUM      =  0x0040,
	RESPOND_DRUID        =  0x0080,
	RESPOND_KNIGHT       =  0x0100,
	RESPOND_PALADIN      =  0x0200,
	RESPOND_SORCERER     =  0x0400,
	RESPOND_LOWLEVEL     =  0x0800,
	RESPOND_ENOUGHMONEY  =  0x1000,
	RESPOND_ENOUGHAMOUNT =  0x2000,
	RESPOND_HIGHLEVEL    =  0x4000,
	RESPOND_KNOWSPELL    =  0x8000,
	RESPOND_CANNOTLEARNSPELL = 0x10000,
	RESPOND_PROMOTED     = 0x20000,
	RESPOND_NOTTOPIC     = 0x40000
};

enum ResponseType_t {
	RESPONSE_DEFAULT,
	RESPONSE_SCRIPT
};

enum InteractType_t {
	INTERACT_NONE,
	INTERACT_TEXT,
	INTERACT_EVENT
};

enum ReponseActionParam_t {
	ACTION_NONE,
	ACTION_SETTOPIC,
	ACTION_SETLEVEL,
	ACTION_SETPRICE,
	ACTION_SETBUYPRICE,
	ACTION_SETSELLPRICE,
	ACTION_TAKEMONEY,
	ACTION_GIVEMONEY,
	ACTION_SELLITEM,
	ACTION_BUYITEM,
	ACTION_GIVEITEM,
	ACTION_TAKEITEM,
	ACTION_SETAMOUNT,
	ACTION_SETITEM,
	ACTION_SETSUBTYPE,
	ACTION_SETEFFECT,
	ACTION_SETSPELL,
	ACTION_SETLISTNAME,
	ACTION_SETLISTPNAME,
	ACTION_TEACHSPELL,
	ACTION_SETSTORAGE,
	ACTION_SETTELEPORT,
	ACTION_SCRIPT,
	ACTION_SCRIPTPARAM,
	ACTION_ADDQUEUE,
	ACTION_SETIDLE
};

enum StorageComparision_t {
	STORAGE_LESS,
	STORAGE_LESSOREQUAL,
	STORAGE_EQUAL,
	STORAGE_NOTEQUAL,
	STORAGE_GREATEROREQUAL,
	STORAGE_GREATER
};

struct StorageCondition {
	int32_t id;
	int32_t value;
	StorageComparision_t op;
};

typedef std::vector<StorageCondition> StorageConditions;

enum NpcEvent_t {
	EVENT_NONE,
	EVENT_BUSY,
	EVENT_THINK,
	EVENT_IDLE,
	EVENT_PLAYER_ENTER,
	EVENT_PLAYER_MOVE,
	EVENT_PLAYER_LEAVE,
	EVENT_PLAYER_SHOPSELL,
	EVENT_PLAYER_SHOPBUY,
	EVENT_PLAYER_SHOPCLOSE

	/*
	EVENT_CREATURE_ENTER,
	EVENT_CREATURE_MOVE,
	EVENT_CREATURE_LEAVE,
	*/
};

enum ShopEvent_t {
	SHOPEVENT_SELL,
	SHOPEVENT_BUY,
	SHOPEVENT_CLOSE
};

struct ResponseAction {
	public:
		ResponseAction() {
			actionType = ACTION_NONE;
			key = 0;
			intValue = 0;
			strValue = "";
			pos.x = 0;
			pos.y = 0;
			pos.z = 0;
		}

		ReponseActionParam_t actionType;
		int32_t key;
		int32_t intValue;
		std::string strValue;
		Position pos;
};

struct ScriptVars {
	ScriptVars() {
		n1 = -1;
		n2 = -1;
		n3 = -1;
		b1 = false;
		b2 = false;
		b3 = false;
		s1 = "";
		s2 = "";
		s3 = "";
	}

	int32_t n1;
	int32_t n2;
	int32_t n3;
	bool b1;
	bool b2;
	bool b3;
	std::string s1;
	std::string s2;
	std::string s3;
};

struct ListItem {
	ListItem() {
		itemId = 0;
		subType = -1;
		sellPrice = 0;
		buyPrice = 0;
		keywords = "";
		name = "";
		pluralName = "";
	}

	int32_t sellPrice;
	int32_t buyPrice;
	int32_t itemId;
	int32_t subType;
	std::string keywords;
	std::string name;
	std::string pluralName;
};


typedef std::list<ResponseAction> ActionList;
typedef std::map<std::string, int32_t> ResponseScriptMap;
typedef std::list<NpcResponse*> ResponseList;

class NpcResponse
{
	public:
		struct ResponseProperties {
			ResponseProperties() {
				topic = -1;
				focusStatus = -1;
				eventType = EVENT_NONE;
				params = 0;
				publicize = true;
				inputList.clear();
				haveItemId = 0;
				dontHaveItemId = 0;
				level = 0;
				storageConditions.clear();
				itemList.clear();
				time = 0;
				singleEvent = false;

				responseType = RESPONSE_DEFAULT;
				output = "";
				knowSpell = "";
				actionList.clear();
				condition = CONDITION_NONE;
				health = -1;
				amount = -1;
			}

			//interact specific
			int32_t topic;
			int32_t focusStatus;
			NpcEvent_t eventType;
			uint32_t params;
			bool publicize;
			std::list<std::string> inputList;
			uint16_t haveItemId;
			uint16_t dontHaveItemId;
			uint32_t level;
			StorageConditions storageConditions;
			std::list<ListItem> itemList;
			uint32_t time;
			bool singleEvent;

			//response specific
			ResponseType_t responseType;
			std::string output;
			std::string knowSpell;
			ActionList actionList;
			ConditionType_t condition;
			int32_t health;
			int32_t amount;
		};

		NpcResponse(const ResponseProperties& _prop,
		            ResponseList _subResponseList,
		            ScriptVars _scriptVars) {
			prop = _prop;
			subResponseList = _subResponseList;
			scriptVars = _scriptVars;
		}

		NpcResponse(NpcResponse& rhs) {
			prop = rhs.prop;
			scriptVars = rhs.scriptVars;

			for (ResponseList::iterator it = rhs.subResponseList.begin(); it != rhs.subResponseList.end(); ++it) {
				NpcResponse* response = new NpcResponse(*(*it));
				subResponseList.push_back(response);
			}
		}

		~NpcResponse() {
			for (ResponseList::iterator it = subResponseList.begin(); it != subResponseList.end(); ++it) {
				delete *it;
			}

			subResponseList.clear();
		}

		uint32_t getParams() const {
			return prop.params;
		}
		std::string getInputText() const {
			return (prop.inputList.empty() ? "" : *prop.inputList.begin());
		}
		int32_t getTopic() const {
			return prop.topic;
		}
		int32_t getFocusState() const {
			return prop.focusStatus;
		}
		int32_t getHaveItemID() const {
			return prop.haveItemId;
		}
		int32_t getDontHaveItemID() const {
			return prop.dontHaveItemId;
		}
		ConditionType_t getCondition() const {
			return prop.condition;
		}
		int32_t getHealth() const {
			return prop.health;
		}
		int32_t getLevel() const {
			return prop.level;
		}
		ResponseType_t getResponseType() const {
			return prop.responseType;
		}
		NpcEvent_t getEventType() const {
			return prop.eventType;
		}
		const std::string& getKnowSpell() const {
			return prop.knowSpell;
		}
		const std::string& getText() const {
			return prop.output;
		}
		int32_t getAmount() const {
			return prop.amount;
		}
		void setAmount(int32_t _amount) {
			prop.amount = _amount;
		}
		bool publicize() const {
			return prop.publicize;
		}
		uint32_t getTime() const {
			return prop.time;
		}
		uint32_t isSingleEvent() const {
			return prop.singleEvent;
		}

		std::string formatResponseString(Creature* creature) const;
		void addAction(ResponseAction action) {
			prop.actionList.push_back(action);
		}
		const std::list<std::string>& getInputList() const {
			return prop.inputList;
		}

		void setResponseList(ResponseList _list) {
			subResponseList.insert(subResponseList.end(), _list.begin(), _list.end());
		}
		const ResponseList& getResponseList() const {
			return subResponseList;
		}

		ActionList::const_iterator getFirstAction() const {
			return prop.actionList.begin();
		}
		ActionList::const_iterator getEndAction() const {
			return prop.actionList.end();
		}

		ResponseProperties prop;
		ResponseList subResponseList;
		ScriptVars scriptVars;
};

struct NpcState {
	uint32_t playerId;
	int32_t topic;
	int32_t focusState;
	bool isQueued;
	int32_t price;
	int32_t sellPrice;
	int32_t buyPrice;
	int32_t amount;
	int32_t itemId;
	int32_t subType;
	bool ignore;
	bool inBackpacks;
	std::string spellName;
	std::string listName;
	std::string listPluralName;
	int32_t level;
	std::string respondToText;
	const NpcResponse* lastResponse;
	const NpcResponse* subResponse;
	uint64_t lastResponseTime;

	//script variables
	ScriptVars scriptVars;

	//Do not forget to update pushState/popState if you add more variables
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
		void doMoveTo(Position pos);
		bool isLoaded() const {
			return loaded;
		}
		virtual void setMasterPos(const Position& pos, uint32_t radius = 1) {
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

		const NpcResponse* getResponse(const ResponseList& list, const Player* player,
		                               NpcState* npcState, const std::string& text,
		                               bool exactMatch = false, NpcEvent_t eventType = EVENT_NONE);
		const NpcResponse* getResponse(const Player* player, NpcState* npcState, const std::string& text, bool checkLastResponse);
		const NpcResponse* getResponse(const Player* player, NpcEvent_t eventType);
		const NpcResponse* getResponse(const Player* player, NpcState* npcState,
		                               NpcEvent_t eventType, const std::string& text, bool checkLastResponse);
		const NpcResponse* getResponse(const Player* player, NpcState* npcState,
		                               NpcEvent_t eventType, bool checkLastResponse);

		int32_t matchKeywords(NpcResponse* response, std::vector<std::string> wordList, bool exactMatch);

		void processResponse(Player* player, NpcState* npcState, const NpcResponse* response, bool delayResponse = false);
		void executeResponse(Player* player, NpcState* npcState, const NpcResponse* response);

		std::string formatResponse(Creature* creature, const NpcState* npcState, const NpcResponse* response) const;

		void onPlayerEnter(Player* player, NpcState* state);
		void onPlayerLeave(Player* player, NpcState* state);

		typedef std::map<std::string, std::string> ParametersMap;
		ParametersMap m_parameters;

		uint32_t loadParams(xmlNodePtr node);
		StorageCondition loadStorageCondition(xmlNodePtr node);
		ResponseList loadInteraction(xmlNodePtr node);

		NpcState* getState(const Player* player, bool makeNew = true);

		void addShopPlayer(Player* player);
		void removeShopPlayer(const Player* player);
		void closeAllShopWindows();
		uint32_t getListItemPrice(uint16_t itemId, ShopEvent_t type);

		std::string name;
		std::string m_filename;
		uint32_t walkTicks;
		bool floorChange;
		bool attackable;
		bool isIdle;
		bool hasUsedIdleReply;
		bool hasBusyReply;
		bool hasScriptedFocus;
		int32_t talkRadius;
		uint32_t idleTimeout;
		uint64_t lastResponseTime;
		bool defaultPublic;
		int32_t focusCreature;

		typedef std::list<Player*> ShopPlayerList;
		ShopPlayerList shopPlayerList;

		typedef std::map<std::string, std::list<ListItem> > ItemListMap;
		ItemListMap itemListMap;

		ResponseScriptMap responseScriptMap;
		ResponseList responseList;

		typedef std::list<NpcState*> StateList;
		StateList stateList;
		NpcEventsHandler* m_npcEventHandler;

		typedef std::list<uint32_t> QueueList;
		QueueList queueList;
		bool loaded;

		static uint32_t npcAutoID;

		static NpcScriptInterface* m_scriptInterface;

		friend class Npcs;
		friend class NpcScriptInterface;
};

#endif
