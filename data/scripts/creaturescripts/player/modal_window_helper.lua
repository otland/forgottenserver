local ModalWindowAutoID = 10000

if not __ModalWindow then
	__ModalWindow = ModalWindow
	ModalWindows = {}
end

-- NOTE: As ModalWindow has a real table defined here, I moved the definition of the class to this file

-- This is to allow for proper documentation of the class and its methods, as having it in cpplinter.lua was causing the language server
-- to not be able to find the class and its methods.

---The ModalWindow is a small window that will be shown to the player with the information set for the window 
---<br>
---ModalWindows can be quite useful for having a user input data, be shown some information, etc... 
---<br><br>
---There is a helper function for creating a ModalWindow, which is ModalWindowHelper, found in data\scripts\creaturescripts\player\modal_window_helper.lua
---<br><br>
---A ModalWindow example can be seen in data\scripts\creaturescripts\player\#modal_window_example.lua
---<br>
--- Example usage:
--- ```lua
--- local modalWindow = ModalWindow{
---		title = "Modal Window Helper Example",
---		message = "This is an example of ModalWindowHelper."
---	}
--- modalWindow:addChoice("Up", function(player, button, choice)
--- 	if button.name == "Select" then
---			local pos = player:getPosition()
---			pos:getNextPosition(DIRECTION_NORTH)
---			player:teleportTo(pos, true)
---			modalWindow:sendToPlayer(player) -- This will resend the same modal window to the player again, to be able to interact again with the window
--- 	end
--- end)
--- ```
--- <br>
--- Further definitons of the ModalWindow can be found in 
--- <ol>
--- <li>modal_window_helper.lua</li>
--- <li>#modal_window_example.lua</li>
--- <li>luascript.cpp</li>
--- </ol>
---@class ModalWindow
---*int LuaScriptInterface::luaModalWindowCreate(lua_State* L)
---@source ../src/luascript.cpp:6293
---@field create fun(): ModalWindow
---@field __eq fun(self: ModalWindow, other: ModalWindow): boolean
---@field __gc fun(self: ModalWindow)
---*int LuaScriptInterface::luaModalWindowDelete(lua_State* L)
---@source ../src/luascript.cpp:6305
---@field delete fun(self: ModalWindow)
---*int LuaScriptInterface::luaModalWindowGetId(lua_State* L)
---@source ../src/luascript.cpp:6315
---@field getId fun(self: ModalWindow): number
---*int LuaScriptInterface::luaModalWindowGetTitle(lua_State* L)
---@source ../src/luascript.cpp:6327
---@field getTitle fun(self: ModalWindow): string
---*int LuaScriptInterface::luaModalWindowGetMessage(lua_State* L)
---@source ../src/luascript.cpp:6339
---@field getMessage fun(self: ModalWindow): string
---*int LuaScriptInterface::luaModalWindowSetTitle(lua_State* L)
---@source ../src/luascript.cpp:6351
---@field setTitle fun(self: ModalWindow, title: string)
---*int LuaScriptInterface::luaModalWindowSetMessage(lua_State* L)
---@source ../src/luascript.cpp:6365
---@field setMessage fun(self: ModalWindow, message: string)
---*int LuaScriptInterface::luaModalWindowGetButtonCount(lua_State* L)
---@source ../src/luascript.cpp:6379
---@field getButtonCount fun(self: ModalWindow): number
---*int LuaScriptInterface::luaModalWindowGetChoiceCount(lua_State* L)
---@source ../src/luascript.cpp:6391
---@field getChoiceCount fun(self: ModalWindow): number
---*int LuaScriptInterface::luaModalWindowAddButton(lua_State* L)
---@source ../src/luascript.cpp:6403
---@field addButton fun(self: ModalWindow, buttonId: number, buttonText: string)
---*int LuaScriptInterface::luaModalWindowAddChoice(lua_State* L)
---@source ../src/luascript.cpp:6418
---@field addChoice fun(self: ModalWindow, choiceId: number, choiceText: string)
---*int LuaScriptInterface::luaModalWindowGetDefaultEnterButton(lua_State* L)
---@source ../src/luascript.cpp:6433
---@field getDefaultEnterButton fun(self: ModalWindow): number
---*int LuaScriptInterface::luaModalWindowSetDefaultEnterButton(lua_State* L)
---@source ../src/luascript.cpp:6445
---@field setDefaultEnterButton fun(self: ModalWindow, buttonId: number)
---*int LuaScriptInterface::luaModalWindowGetDefaultEscapeButton(lua_State* L)
---@source ../src/luascript.cpp:6458
---@field getDefaultEscapeButton fun(self: ModalWindow): number
---*int LuaScriptInterface::luaModalWindowSetDefaultEscapeButton(lua_State* L)
---@source ../src/luascript.cpp:6470
---@field setDefaultEscapeButton fun(self: ModalWindow, buttonId: number)
---*int LuaScriptInterface::luaModalWindowHasPriority(lua_State* L)
---@source ../src/luascript.cpp:6483
---@field hasPriority fun(self: ModalWindow): boolean
---*int LuaScriptInterface::luaModalWindowSetPriority(lua_State* L)
---@source ../src/luascript.cpp:6495
---@field setPriority fun(self: ModalWindow, priority: boolean)
---*int LuaScriptInterface::luaModalWindowSendToPlayer(lua_State* L)
---@source ../src/luascript.cpp:6508
---@field sendToPlayer fun(self: ModalWindow, player: Player)
ModalWindow = {}
ModalWindow.__index = ModalWindow

function ModalWindow.new(self, ...)
	if type(self) ~= "table" then
		return __ModalWindow(self, ...)
	end

	self.modalWindowId = 0
	self.buttons = {}
	self.choices = {}
	self.using = 0
	return setmetatable(self, ModalWindow)
end

setmetatable(ModalWindow, {
	__call = function (self, ...)
		return ModalWindow.new(...)
	end
})

function ModalWindow:setTitle(title)
	self.title = tostring(title)
	return true
end

function ModalWindow:setMessage(message)
	self.message = tostring(message)
	return true
end

function ModalWindow:addButton(name, callback)
	if type(name) ~= "string" then
		io.write("ModalWindow:addButton: name must be a string.\n")
		name = tostring(name)
	end

	if self.buttons[name] then
		io.write("ModalWindow: Button with name '" .. name .. "' already exists.\n")
		return false
	end

	local id = #self.buttons + 1
	local button = { id = id, name = name, callback = callback }
	self.buttons[id] = button
	self.buttons[name] = button
	return button
end

function ModalWindow:removeButton(name)
	if type(name) ~= "string" then
		io.write("ModalWindow:removeButton: name must be a string.\n")
		name = tostring(name)
	end

	local button = self.buttons[name]
	if not button then
		io.write("ModalWindow: Button with name '" .. name .. "' does not exist.\n")
		return false
	end

	self.buttons[button.id] = nil
	self.buttons[name] = nil
	return true
end

function ModalWindow:callButton(name, player, button, choice)
	if type(name) ~= "string" then
		io.write("ModalWindow:callButton: name must be a string.\n")
		name = tostring(name)
	end

	local button = self.buttons[name]
	if not button then
		io.write("ModalWindow: Button with name '" .. name .. "' does not exist.\n")
		return false
	end

	if not button.callback then
		io.write("ModalWindow: Button with name '" .. name .. "' has no callback.\n")
		return false
	end
	return button.callback(player, button, choice)
end

function ModalWindow:clearButtons()
	self.buttons = {}
	return true
end

function ModalWindow:setDefaultEnterButton(buttonId)
	self.defaultEnterButton = buttonId
	return true
end

function ModalWindow:setDefaultEscapeButton(buttonId)
	self.defaultEscapeButton = buttonId
	return true
end

function ModalWindow:setDefaultCallback(callback)
	self.defaultCallback = callback
	return true
end

function ModalWindow:addChoice(text, callback)
	if type(text) ~= "string" then
		io.write("ModalWindow:addChoice: text must be a string.\n")
		text = tostring(text)
	end

	local id = #self.choices + 1
	local choice = { id = id, text = text, callback = callback }
	self.choices[id] = choice
	self.choices[text] = choice
	return choice
end

function ModalWindow:removeChoice(text)
	if type(text) ~= "string" then
		io.write("ModalWindow:removeChoice: text must be a string.\n")
		text = tostring(text)
	end

	local choice = self.choices[text]
	if not choice then
		io.write("ModalWindow: Choice with text '" .. text .. "' does not exist.\n")
		return false
	end

	self.choices[choice.id] = nil
	self.choices[text] = nil
	return true
end

function ModalWindow:callChoice(text, player, button, choice)
	if type(text) ~= "string" then
		io.write("ModalWindow:callChoice: text must be a string.\n")
		text = tostring(text)
	end

	local choice = self.choices[text]
	if not choice then
		io.write("ModalWindow: Choice with text '" .. text .. "' does not exist.\n")
		return false
	end

	if not choice.callback then
		io.write("ModalWindow: Choice with text '" .. text .. "' has no callback.\n")
		return false
	end
	return choice.callback(player, button, choice)
end

function ModalWindow:clearChoices()
	self.choices = {}
	return true
end

function ModalWindow:clear()
	self.choices = {}
	self.buttons = {}
	return true
end

function ModalWindow:setPriority(priority)
	self.priority = priority
	return true
end

function ModalWindow:setId()
	if self.modalWindowId ~= 0 then
		return self.modalWindowId
	end

	self.modalWindowId = ModalWindowAutoID
	ModalWindowAutoID = ModalWindowAutoID + 1
	return self.modalWindowId
end

function ModalWindow:create()
	local modalWindow = __ModalWindow(self:setId(), self.title, self.message)
	modalWindow:setPriority(self.priority and true or false)

	for id = 1, #self.buttons do
		local name = self.buttons[id].name
		modalWindow:addButton(id, name)
		if id == self.defaultEnterButton or name == self.defaultEnterButton then
			modalWindow:setDefaultEnterButton(id - 1)
		elseif id == self.defaultEscapeButton or name == self.defaultEscapeButton then
			modalWindow:setDefaultEscapeButton(id - 1)
		end
	end

	for id = 1, #self.choices do
		modalWindow:addChoice(id, self.choices[id].text)
	end
	return modalWindow
end

function ModalWindow:sendToPlayer(player)
	local modalWindow = self:create()
	local playerId = player:getId()
	ModalWindows[playerId] = ModalWindows[playerId] or {}
	ModalWindows[playerId][self.modalWindowId] = self
	player:registerEvent("modalWindowHelper")
	self.using = self.using + 1
	return modalWindow:sendToPlayer(player)
end

local creatureEvent = CreatureEvent("modalWindowHelper")

function creatureEvent.onModalWindow(player, modalWindowId, buttonId, choiceId)
	local playerId = player:getId()
	local modalWindows = ModalWindows[playerId]
	if not modalWindows then
		return true
	end

	local modalWindow = modalWindows[modalWindowId]
	if not modalWindow then
		return true
	end

	local button = modalWindow.buttons[buttonId] or {}
	local choice = modalWindow.choices[choiceId] or {}
	if button.callback then
		button.callback(player, button, choice)
	elseif choice.callback then
		choice.callback(player, button, choice)
	elseif modalWindow.defaultCallback then
		modalWindow.defaultCallback(player, button, choice)
	end

	modalWindow.using = modalWindow.using - 1
	if modalWindow.using == 0 then
		modalWindows[modalWindowId] = nil
		if not next(modalWindows) then
			ModalWindows[playerId] = nil
		end
	end
	return true
end

creatureEvent:register()
