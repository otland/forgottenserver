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

#ifndef __OTSERV_CREATUREEVENT_H__
#define __OTSERV_CREATUREEVENT_H__

#include "luascript.h"
#include "baseevents.h"
#include "enums.h"

enum CreatureEventType_t {
	CREATURE_EVENT_NONE,
	CREATURE_EVENT_LOGIN,
	CREATURE_EVENT_LOGOUT,
	CREATURE_EVENT_THINK,
	CREATURE_EVENT_PREPAREDEATH,
	CREATURE_EVENT_DEATH,
	CREATURE_EVENT_KILL,
	CREATURE_EVENT_ADVANCE,
	CREATURE_EVENT_MODALWINDOW,
	CREATURE_EVENT_TEXTEDIT
};

class CreatureEvent;

class CreatureEvents : public BaseEvents
{
	public:
		CreatureEvents();
		virtual ~CreatureEvents();

		// global events
		bool playerLogin(Player* player);
		bool playerLogout(Player* player);
		bool playerAdvance(Player* player, skills_t, uint32_t, uint32_t);

		CreatureEvent* getEventByName(const std::string& name, bool forceLoaded = true);

	protected:
		virtual LuaScriptInterface& getScriptInterface();
		virtual std::string getScriptBaseName();
		virtual Event* getEvent(const std::string& nodeName);
		virtual bool registerEvent(Event* event, xmlNodePtr p);
		virtual void clear();

		//creature events
		typedef std::map<std::string, CreatureEvent*> CreatureEventList;
		CreatureEventList m_creatureEvents;

		LuaScriptInterface m_scriptInterface;
};

class CreatureEvent : public Event
{
	public:
		CreatureEvent(LuaScriptInterface* _interface);
		virtual ~CreatureEvent() {}

		virtual bool configureEvent(xmlNodePtr p);

		CreatureEventType_t getEventType() const {
			return m_type;
		}
		const std::string& getName() const {
			return m_eventName;
		}
		bool isLoaded() const {
			return m_isLoaded;
		}

		void clearEvent();
		void copyEvent(CreatureEvent* creatureEvent);

		//scripting
		uint32_t executeOnLogin(Player* player);
		uint32_t executeOnLogout(Player* player);
		uint32_t executeOnThink(Creature* creature, uint32_t interval);
		uint32_t executeOnPrepareDeath(Creature* creature, Creature* killer);
		uint32_t executeOnDeath(Creature* creature, Item* corpse, Creature* killer, Creature* mostDamageKiller, bool lastHitUnjustified, bool mostDamageUnjustified);
		uint32_t executeOnKill(Creature* creature, Creature* target);
		uint32_t executeAdvance(Player* player, skills_t, uint32_t, uint32_t);
		uint32_t executeModalWindow(Player* player, uint32_t modalWindowId, uint8_t buttonId, uint8_t choiceId);
		uint32_t executeTextEdit(Player* player, Item* item, const std::string& text);
		//

	protected:
		virtual std::string getScriptEventName();

		std::string m_eventName;
		CreatureEventType_t m_type;
		bool m_isLoaded;
};

#endif
