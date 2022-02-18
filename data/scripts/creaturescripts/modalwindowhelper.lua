if not ModalWindows then
	ModalWindows = {
		eventName = "ModalWindowHelper",
		modalWindowConstructor = ModalWindow,
		baseId = 1000,
		windows = {}
	}
end

local modalWindowHelper = CreatureEvent(ModalWindows.eventName)

function modalWindowHelper.onModalWindow(player, modalWindowId, buttonId, choiceId)
	local modalWindow = ModalWindows.windows[modalWindowId]
	if not modalWindow then return end
	if modalWindow.playerGuid ~= player:getGuid() then return end
	local button = modalWindow.buttons[buttonId]
	local choice = modalWindow.choices[choiceId]
	local buttonCallback = button.callback
	local choiceCallback = choice.callback
	local defaultCallback = modalWindow.defaultCallback
    ModalWindows.windows[modalWindowId] = nil
    local returnValue = true
    if buttonCallback then returnValue = buttonCallback(player, button, choice) end
    if returnValue and choiceCallback then returnValue = choiceCallback(player, button, choice) end
    if returnValue and defaultCallback then defaultCallback(player, button, choice) end
end

modalWindowHelper:register()

local MT = {}
MT.__index = MT

function ModalWindow(modalWindow, ...)
	if type(modalWindow) == 'table' then
		local self = setmetatable(modalWindow, MT)
		self.tmpButtons = {}
		local mt = {
			__index = function ()
				return {}
			end
		}
		self.buttons = setmetatable({}, mt)
		self.choices = setmetatable({}, mt)
		return self
	end

	return ModalWindows.modalWindowConstructor(modalWindow, ...)
end

function MT:setDefaultCallback(callback)
	self.defaultCallback = callback
end

function MT:addButton(text, callback)
	self.tmpButtons[#self.tmpButtons +1] = {text = tostring(text), callback = callback}
end

function MT:addButtons(...)
	for _, text in ipairs({...}) do
		self.tmpButtons[#self.tmpButtons +1] = {text = tostring(text)}
	end
end

function MT:addChoice(text, callback)
	self.choices[#self.choices +1] = {text = tostring(text), callback = callback}
end

function MT:addChoices(...)
	for _, text in ipairs({...}) do
		self.choices[#self.choices +1] = {text = tostring(text)}
	end
end

function MT:setDefaultEnterButton(text)
	self.defaultEnterButton = text
end

function MT:setDefaultEscapeButton(text)
	self.defaultEscapeButton = text
end

function MT:setTitle(title)
	self.title = tostring(title)
end

function MT:setMessage(message)
	self.message = tostring(message)
end

local buttonOrder = {
	[4] = {3, 4, 2, 1},
	[3] = {2, 3, 1},
	[2] = {1, 2},
	[1] = {1}
}
function MT:create(player)
	local playerGuid = player:getGuid()
	local id = ModalWindows.baseId + playerGuid
	self.playerGuid = playerGuid
	ModalWindows.windows[id] = self
	local modalWindow = ModalWindows.modalWindowConstructor(id, self.title, self.message)
	local order = buttonOrder[math.min(#self.tmpButtons, 4)]

	if order then
		for _, i in pairs(order) do
			local button = self.tmpButtons[i]
			modalWindow:addButton(i, button.text)
			button.id = i

			if button.text == self.defaultEnterButton then
				modalWindow:setDefaultEnterButton(i)
			elseif button.text == self.defaultEscapeButton then
				modalWindow:setDefaultEscapeButton(i)
			end
			self.buttons[i] = button
		end

		self.tmpButtons = nil
	end

	for index, choice in pairs(self.choices) do
		modalWindow:addChoice(index, choice.text)
		choice.id = index
	end

	self.modalWindow = modalWindow
end

function MT:sendToPlayer(player)
	if not self.modalWindow then
		self:create(player)
	end

	player:registerEvent(ModalWindows.eventName)
	return self.modalWindow:sendToPlayer(player)
end
