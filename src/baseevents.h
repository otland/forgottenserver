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

#ifndef __BASEEVENTS_H__
#define __BASEEVENTS_H__

#include "luascript.h"
#include "pugixml.hpp"

class Event;

class BaseEvents
{
	public:
		BaseEvents();
		virtual ~BaseEvents();

		bool loadFromXml();
		bool reload();
		bool isLoaded() const {
			return m_loaded;
		}

	protected:
		virtual LuaScriptInterface& getScriptInterface() = 0;
		virtual std::string getScriptBaseName() = 0;
		virtual Event* getEvent(const std::string& nodeName) = 0;
		virtual bool registerEvent(Event* event, const pugi::xml_node& node) = 0;
		virtual void clear() = 0;

		bool m_loaded;
};

class Event
{
	public:
		Event(LuaScriptInterface* _interface);
		Event(const Event* copy);
		virtual ~Event();

		virtual bool configureEvent(const pugi::xml_node& node) = 0;

		bool checkScript(const std::string& basePath, const std::string& scriptsName, const std::string& scriptFile);
		bool loadScript(const std::string& scriptFile);
		virtual bool loadFunction(const std::string& functionName);

		virtual bool isScripted() {
			return m_scripted;
		}

	protected:
		virtual std::string getScriptEventName() = 0;

		bool m_scripted;
		int32_t m_scriptId;
		LuaScriptInterface* m_scriptInterface;
};


class CallBack
{
	public:
		CallBack();
		virtual ~CallBack();

		bool loadCallBack(LuaScriptInterface* _interface, const std::string& name);

	protected:
		int32_t m_scriptId;
		LuaScriptInterface* m_scriptInterface;

		bool m_loaded;

		std::string m_callbackName;
};

#endif
