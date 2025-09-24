// Copyright 2023 The Forgotten Server Authors. All rights reserved.
// Use of this source code is governed by the GPL-2.0 License that can be found in the LICENSE file.

#ifndef FS_BASEEVENTS_H
#define FS_BASEEVENTS_H

class LuaScriptInterface;

class Event;
using Event_ptr = std::unique_ptr<Event>;

class Event
{
public:
	explicit Event(LuaScriptInterface* interface);
	virtual ~Event() = default;

	virtual bool configureEvent(const pugi::xml_node& node) = 0;

	bool checkScript(const std::string& basePath, const std::string& scriptsName, const std::string& scriptFile) const;
	bool loadScript(const std::string& scriptFile);
	bool loadCallback();
	virtual bool loadFunction(const pugi::xml_attribute&, bool) { return false; }

	bool isScripted() const { return scripted; }

	bool scripted = false;
	bool fromLua = false;

	int32_t getScriptId() const { return scriptId; }

	void clearScript();

protected:
	virtual std::string_view getScriptEventName() const = 0;

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
	bool isLoaded() const { return loaded; }
	void reInitState(bool fromLua);

private:
	virtual LuaScriptInterface& getScriptInterface() = 0;
	virtual std::string_view getScriptBaseName() const = 0;
	virtual Event_ptr getEvent(const std::string& nodeName) = 0;
	virtual bool registerEvent(Event_ptr event, const pugi::xml_node& node) = 0;
	virtual void clear(bool) = 0;

	bool loaded = false;
};

class CallBack
{
public:
	CallBack() = default;

	bool loadCallBack(LuaScriptInterface* interface, const std::string& name);

	void clearScript();

protected:
	int32_t scriptId = 0;
	LuaScriptInterface* scriptInterface = nullptr;

private:
	bool loaded = false;
};

#endif // FS_BASEEVENTS_H
