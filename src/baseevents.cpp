/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2014  Mark Samman <mark.samman@gmail.com>
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

#include "baseevents.h"
#include "pugicast.h"

extern LuaEnvironment g_luaEnvironment;

BaseEvents::BaseEvents()
{
	m_loaded = false;
}

bool BaseEvents::loadFromXml()
{
	if (m_loaded) {
		std::cout << "[Error - BaseEvents::loadFromXml] It's already loaded." << std::endl;
		return false;
	}

	std::string scriptsName = getScriptBaseName();
	std::string basePath = "data/" + scriptsName + "/";
	if (getScriptInterface().loadFile(basePath + "lib/" + scriptsName + ".lua") == -1) {
		std::cout << "[Warning - BaseEvents::loadFromXml] Can not load " << scriptsName << " lib/" << scriptsName << ".lua" << std::endl;
	}

	std::string filename = basePath + scriptsName + ".xml";

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename.c_str());
	if (!result) {
		std::cout << "[Error - BaseEvents::loadFromXml] Failed to load " << filename << ": " << result.description() << std::endl;
		return false;
	}

	m_loaded = true;

	for (pugi::xml_node node = doc.child(scriptsName.c_str()).first_child(); node; node = node.next_sibling()) {
		Event* event = getEvent(node.name());
		if (!event) {
			continue;
		}

		if (!event->configureEvent(node)) {
			std::cout << "[Warning - BaseEvents::loadFromXml] Failed to configure event" << std::endl;
			delete event;
			continue;
		}

		bool success;

		pugi::xml_attribute scriptAttribute = node.attribute("script");
		if (scriptAttribute) {
			std::string scriptFile = "scripts/" + std::string(scriptAttribute.as_string());
			success = event->checkScript(basePath, scriptsName, scriptFile) && event->loadScript(basePath + scriptFile);
		} else {
			success = event->loadFunction(node.attribute("function"));
		}

		if (!success || !registerEvent(event, node)) {
			delete event;
		}
	}
	return true;
}

bool BaseEvents::reload()
{
	m_loaded = false;
	clear();
	return loadFromXml();
}

Event::Event(LuaScriptInterface* _interface)
{
	m_scriptInterface = _interface;
	m_scriptId = 0;
	m_scripted = false;
}

Event::Event(const Event* copy)
{
	m_scriptInterface = copy->m_scriptInterface;
	m_scriptId = copy->m_scriptId;
	m_scripted = copy->m_scripted;
}

bool Event::checkScript(const std::string& basePath, const std::string& scriptsName, const std::string& scriptFile)
{
	LuaScriptInterface* testInterface = g_luaEnvironment.getTestInterface();
	testInterface->reInitState();

	if (testInterface->loadFile(std::string(basePath + "lib/" + scriptsName + ".lua")) == -1) {
		std::cout << "[Warning - Event::checkScript] Can not load " << scriptsName << " lib/" << scriptsName << ".lua" << std::endl;
	}

	if (m_scriptId != 0) {
		std::cout << "[Failure - Event::checkScript] scriptid = " << m_scriptId << std::endl;
		return false;
	}

	if (testInterface->loadFile(basePath + scriptFile) == -1) {
		std::cout << "[Warning - Event::checkScript] Can not load script: " << scriptFile << std::endl;
		std::cout << testInterface->getLastLuaError() << std::endl;
		return false;
	}

	int32_t id = testInterface->getEvent(getScriptEventName());
	if (id == -1) {
		std::cout << "[Warning - Event::checkScript] Event " << getScriptEventName() << " not found. " << scriptFile << std::endl;
		return false;
	}
	return true;
}

bool Event::loadScript(const std::string& scriptFile)
{
	if (!m_scriptInterface || m_scriptId != 0) {
		std::cout << "Failure: [Event::loadScript] m_scriptInterface == nullptr. scriptid = " << m_scriptId << std::endl;
		return false;
	}

	if (m_scriptInterface->loadFile(scriptFile) == -1) {
		std::cout << "[Warning - Event::loadScript] Can not load script. " << scriptFile << std::endl;
		std::cout << m_scriptInterface->getLastLuaError() << std::endl;
		return false;
	}

	int32_t id = m_scriptInterface->getEvent(getScriptEventName());
	if (id == -1) {
		std::cout << "[Warning - Event::loadScript] Event " << getScriptEventName() << " not found. " << scriptFile << std::endl;
		return false;
	}

	m_scripted = true;
	m_scriptId = id;
	return true;
}

CallBack::CallBack()
{
	m_scriptId = 0;
	m_scriptInterface = nullptr;
	m_loaded = false;
}

bool CallBack::loadCallBack(LuaScriptInterface* _interface, const std::string& name)
{
	if (!_interface) {
		std::cout << "Failure: [CallBack::loadCallBack] m_scriptInterface == nullptr" << std::endl;
		return false;
	}

	m_scriptInterface = _interface;

	int32_t id = m_scriptInterface->getEvent(name.c_str());
	if (id == -1) {
		std::cout << "[Warning - CallBack::loadCallBack] Event " << name << " not found." << std::endl;
		return false;
	}

	m_callbackName = name;
	m_scriptId = id;
	m_loaded = true;
	return true;
}
