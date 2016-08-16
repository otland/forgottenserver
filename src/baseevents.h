/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2016  Mark Samman <mark.samman@gmail.com>
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

#ifndef FS_BASEEVENTS_H_9994E32C91CE4D95912A5FDD1F41884A
#define FS_BASEEVENTS_H_9994E32C91CE4D95912A5FDD1F41884A

#include "luascript.h"

class Event
{
	public:
		explicit Event(LuaScriptInterface* interface);
		explicit Event(const Event* copy);
		virtual ~Event() = default;

		virtual bool configureEvent(const pugi::xml_node& node) = 0;

		bool checkScript(const std::string& basePath, const std::string& scriptsName, const std::string& scriptFile) const;
		bool loadScript(const std::string& scriptFile);
		virtual bool loadFunction(const pugi::xml_attribute&) {
			return false;
		}

		bool isScripted() const {
			return scripted;
		}

	protected:
		virtual std::string getScriptEventName() const = 0;

		bool scripted = false;
		int32_t scriptId = 0;
		LuaScriptInterface* scriptInterface = nullptr;
};

class BaseEvents
{
	public:
 		constexpr BaseEvents() = default;
		virtual ~BaseEvents() = default;

		bool loadFromXml();
		bool reload();
		bool isLoaded() const {
			return loaded;
		}

	protected:
		virtual LuaScriptInterface& getScriptInterface() = 0;
		virtual std::string getScriptBaseName() const = 0;
		virtual Event* getEvent(const std::string& nodeName) = 0;
		virtual bool registerEvent(Event* event, const pugi::xml_node& node) = 0;
		virtual void clear() = 0;

		bool loaded = false;
};

class CallBack
{
	public:
		CallBack() = default;

		bool loadCallBack(LuaScriptInterface* interface, const std::string& name);

	protected:
		int32_t scriptId = 0;
		LuaScriptInterface* scriptInterface = nullptr;

		bool loaded = false;

		std::string callbackName;
};

#endif
