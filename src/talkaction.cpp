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

#include "creature.h"
#include "player.h"
#include "tools.h"

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "talkaction.h"

extern Game g_game;

TalkActions::TalkActions()
	: m_scriptInterface("TalkAction Interface")
{
	m_scriptInterface.initState();
}

TalkActions::~TalkActions()
{
	clear();
}

void TalkActions::clear()
{
	TalkActionList::iterator it = wordsMap.begin();

	while (it != wordsMap.end()) {
		delete it->second;
		wordsMap.erase(it);
		it = wordsMap.begin();
	}

	m_scriptInterface.reInitState();
}

LuaScriptInterface& TalkActions::getScriptInterface()
{
	return m_scriptInterface;
}

std::string TalkActions::getScriptBaseName()
{
	return "talkactions";
}

Event* TalkActions::getEvent(const std::string& nodeName)
{
	if (asLowerCaseString(nodeName) == "talkaction") {
		return new TalkAction(&m_scriptInterface);
	}

	return NULL;
}

bool TalkActions::registerEvent(Event* event, xmlNodePtr p)
{
	TalkAction* talkAction = dynamic_cast<TalkAction*>(event);

	if (!talkAction) {
		return false;
	}

	wordsMap.push_back(std::make_pair(talkAction->getWords(), talkAction));
	return true;
}

TalkActionResult_t TalkActions::playerSaySpell(Player* player, SpeakClasses type, const std::string& words)
{
	if (type != SPEAK_SAY) {
		return TALKACTION_CONTINUE;
	}

	std::string str_words;
	std::string str_param;
	size_t loc = words.find('"', 0);

	if (loc != std::string::npos) {
		str_words = std::string(words, 0, loc);
		str_param = std::string(words, (loc + 1), words.size() - loc - 1);
	} else {
		str_words = words;
		str_param = std::string("");
	}

	trim_left(str_words, " ");
	trim_right(str_words, " ");

	for (const auto& it : wordsMap) {
		if (it.first == str_words) {
			if (it.second->executeSay(player, str_words, str_param)) {
				return TALKACTION_CONTINUE;
			} else {
				return TALKACTION_BREAK;
			}
		}
	}
	return TALKACTION_CONTINUE;
}

TalkAction::TalkAction(LuaScriptInterface* _interface) :
	Event(_interface)
{
	//
}

TalkAction::~TalkAction()
{
	//
}

bool TalkAction::configureEvent(xmlNodePtr p)
{
	std::string strValue;

	if (readXMLString(p, "words", strValue)) {
		m_words = strValue;
	} else {
		std::cout << "Error: [TalkAction::configureEvent] No words for TalkAction or Spell." << std::endl;
		return false;
	}

	return true;
}

std::string TalkAction::getScriptEventName()
{
	return "onSay";
}

bool TalkAction::executeSay(Creature* creature, const std::string& words, const std::string& param)
{
	//onSay(cid, words, param)
	if (!m_scriptInterface->reserveScriptEnv()) {
		std::cout << "[Error - TalkAction::executeSay] Call stack overflow" << std::endl;
		return false;
	}

	ScriptEnvironment* env = m_scriptInterface->getScriptEnv();
	env->setScriptId(m_scriptId, m_scriptInterface);

	lua_State* L = m_scriptInterface->getLuaState();

	m_scriptInterface->pushFunction(m_scriptId);
	lua_pushnumber(L, creature->getID());
	LuaScriptInterface::pushString(L, words);
	LuaScriptInterface::pushString(L, param);

	return m_scriptInterface->callFunction(3);
}
