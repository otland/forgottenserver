// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_CREATUREEVENT_H
#define FS_CREATUREEVENT_H

#include "baseevents.h"
#include "enums.h"
#include "luascript.h"

class CreatureEvent;
using CreatureEvent_ptr = std::unique_ptr<CreatureEvent>;

enum CreatureEventType_t
{
	CREATURE_EVENT_NONE,
	CREATURE_EVENT_LOGIN,
	CREATURE_EVENT_LOGOUT,
	CREATURE_EVENT_RECONNECT,
	CREATURE_EVENT_THINK,
	CREATURE_EVENT_PREPAREDEATH,
	CREATURE_EVENT_DEATH,
	CREATURE_EVENT_KILL,
	CREATURE_EVENT_ADVANCE,
	CREATURE_EVENT_MODALWINDOW,
	CREATURE_EVENT_TEXTEDIT,
	CREATURE_EVENT_HEALTHCHANGE,
	CREATURE_EVENT_MANACHANGE,
	CREATURE_EVENT_EXTENDED_OPCODE, // otclient additional network opcodes
};

class CreatureEvent final : public Event
{
public:
	explicit CreatureEvent(LuaScriptInterface* interface);

	bool configureEvent(const pugi::xml_node& node) override;

	CreatureEventType_t getEventType() const { return type; }
	void setEventType(CreatureEventType_t eventType) { type = eventType; }
	const std::string& getName() const { return eventName; }
	void setName(const std::string& name) { eventName = name; }
	bool isLoaded() const { return loaded; }
	void setLoaded(bool b) { loaded = b; }

	void clearEvent();
	void copyEvent(CreatureEvent* creatureEvent);

	// scripting
	bool executeOnLogin(const std::shared_ptr<Player>& player) const;
	bool executeOnLogout(const std::shared_ptr<Player>& player) const;
	void executeOnReconnect(const std::shared_ptr<Player>& player) const;
	bool executeOnThink(const std::shared_ptr<Creature>& creature, uint32_t interval);
	bool executeOnPrepareDeath(const std::shared_ptr<Creature>& creature, const std::shared_ptr<Creature>& killer);
	bool executeOnDeath(const std::shared_ptr<Creature>& creature, const std::shared_ptr<Item>& corpse,
	                    const std::shared_ptr<Creature>& killer, const std::shared_ptr<Creature>& mostDamageKiller,
	                    bool lastHitUnjustified, bool mostDamageUnjustified);
	void executeOnKill(const std::shared_ptr<Creature>& creature, const std::shared_ptr<Creature>& target);
	bool executeAdvance(const std::shared_ptr<Player>& player, skills_t, uint32_t, uint32_t);
	void executeModalWindow(const std::shared_ptr<Player>& player, uint32_t modalWindowId, uint8_t buttonId,
	                        uint8_t choiceId);
	bool executeTextEdit(const std::shared_ptr<Player>& player, const std::shared_ptr<Item>& item,
	                     std::string_view text, const uint32_t windowTextId);
	void executeHealthChange(const std::shared_ptr<Creature>& creature, const std::shared_ptr<Creature>& attacker,
	                         CombatDamage& damage);
	void executeManaChange(const std::shared_ptr<Creature>& creature, const std::shared_ptr<Creature>& attacker,
	                       CombatDamage& damage);
	void executeExtendedOpcode(const std::shared_ptr<Player>& player, uint8_t opcode, const std::string& buffer);
	//

private:
	std::string_view getScriptEventName() const override;

	std::string eventName;
	CreatureEventType_t type;
	bool loaded;
};

class CreatureEvents final : public BaseEvents
{
public:
	CreatureEvents();

	// non-copyable
	CreatureEvents(const CreatureEvents&) = delete;
	CreatureEvents& operator=(const CreatureEvents&) = delete;

	// global events
	bool playerLogin(const std::shared_ptr<Player>& player) const;
	bool playerLogout(const std::shared_ptr<Player>& player) const;
	void playerReconnect(const std::shared_ptr<Player>& player) const;
	bool playerAdvance(const std::shared_ptr<Player>& player, skills_t, uint32_t, uint32_t);

	CreatureEvent* getEventByName(const std::string& name, bool forceLoaded = true);

	bool registerLuaEvent(CreatureEvent* event);
	void clear(bool fromLua) override final;

	void removeInvalidEvents();

private:
	LuaScriptInterface& getScriptInterface() override;
	std::string_view getScriptBaseName() const override { return "creaturescripts"; }
	Event_ptr getEvent(const std::string& nodeName) override;
	bool registerEvent(Event_ptr event, const pugi::xml_node& node) override;

	// creature events
	using CreatureEventMap = std::map<std::string, CreatureEvent>;
	CreatureEventMap creatureEvents;

	LuaScriptInterface scriptInterface;
};

#endif // FS_CREATUREEVENT_H
