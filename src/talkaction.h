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

#ifndef __TALKACTION_H__
#define __TALKACTION_H__

#include <list>
#include <string>
#include "luascript.h"
#include "baseevents.h"
#include "const.h"

enum TalkActionResult_t {
	TALKACTION_CONTINUE,
	TALKACTION_BREAK,
	TALKACTION_FAILED
};

class TalkAction;

class TalkActions : public BaseEvents
{
	public:
		TalkActions();
		virtual ~TalkActions();

		TalkActionResult_t playerSaySpell(Player* player, SpeakClasses type, const std::string& words);

	protected:
		virtual LuaScriptInterface& getScriptInterface();
		virtual std::string getScriptBaseName();
		virtual Event* getEvent(const std::string& nodeName);
		virtual bool registerEvent(Event* event, xmlNodePtr p);
		virtual void clear();

		typedef std::list< std::pair<std::string, TalkAction* > > TalkActionList;
		TalkActionList wordsMap;

		LuaScriptInterface m_scriptInterface;
};

class TalkAction : public Event
{
	public:
		TalkAction(LuaScriptInterface* _interface);
		virtual ~TalkAction();

		virtual bool configureEvent(xmlNodePtr p);

		std::string getWords() const {
			return m_words;
		}

		//scripting
		int32_t executeSay(Creature* creature, const std::string& words, const std::string& param);
		//

	protected:
		virtual std::string getScriptEventName();

		std::string m_words;
};

#endif
