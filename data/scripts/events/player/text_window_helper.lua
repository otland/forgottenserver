if not TextWindows then TextWindows = {} end

local textWindow = {}
textWindow.__index = textWindow

function TextWindow(window) return setmetatable(window, textWindow) end

function textWindow.setCallback(window, callback)
	window.callback = callback
	return true
end

function textWindow.setItemId(window, itemId)
	window.itemId = itemId
	return true
end

function textWindow.setText(window, text)
	window.text = text
	return true
end

function textWindow.setCanWrite(window, canWrite)
	window.canWrite = canWrite
	return true
end

function textWindow.setLength(window, length)
	window.length = length
	return true
end

function textWindow.sendToPlayer(window, player)
	window.id = player:showTextDialog(window.itemId, window.text, window.canWrite, window.length)
	local playerGuid = player:getGuid()
	local windows = TextWindows[playerGuid]
	if not windows then
		windows = {}
		TextWindows[playerGuid] = windows
	end

	windows[window.id] = window
	return window.id
end

local event = Event()

event.onPlayerTextEdit = function(self, item, text, windowTextId)
	local playerGuid = self:getGuid()
	local windows = TextWindows[playerGuid]
	if not windows then
		return true
	end

	local window = windows[windowTextId]
	if not window then
		return true
	end

	windows[windowTextId] = nil
	if window.id == windowTextId and window.callback then
		return window.callback(self, item, text)
	end
	return true
end

event:register()

event = Event()

event.onPlayerLogout = function(self)
	TextWindows[self:getGuid()] = nil
	return true
end

event:register()

event = Event()

event.onCreatureDeath = function(self)
	if self:isPlayer() then
		TextWindows[self:getGuid()] = nil
	end
end

event:register()
