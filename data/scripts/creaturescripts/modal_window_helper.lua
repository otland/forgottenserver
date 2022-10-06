local ModalWindowAutoID = 10000
local buttonOrder = {
    [4] = {2, 3, 4, 1},
    [3] = {2, 3, 1},
    [2] = {2, 1},
    [1] = {1}
}

if not __ModalWindow then
    __ModalWindow = ModalWindow
    ModalWindows = {}
end

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
    self.title = title
end

function ModalWindow:setMessage(message)
    self.message = message
end

function ModalWindow:addButton(name, callback)
    for _, button in pairs(self.buttons) do
        if button.name == name then
            button.callback = callback
            return
        end
    end
    self.buttons[#self.buttons + 1] = {name = name, callback = callback}
end

function ModalWindow:removeButton(name)
    for i = 1, #self.buttons do
        if self.buttons[i].name == name then
            table.remove(self.buttons, i)
            break
        end
    end
end

function ModalWindow:callButton(name, player, button, choice)
    for _, button in pairs(self.buttons) do
        if button.name == name then
            button.callback(player, button, choice)
            return
        end
    end
end

function ModalWindow:clearButtons()
    self.buttons = {}
end

function ModalWindow:setDefaultEnterButton(buttonId)
    self.defaultEnterButton = buttonId
end

function ModalWindow:setDefaultEscapeButton(buttonId)
    self.defaultEscapeButton = buttonId
end

function ModalWindow:setDefaultCallback(callback)
    self.defaultCallback = callback
end

function ModalWindow:addChoice(text, callback)
    self.choices[#self.choices + 1] = {text = text, callback = callback}
end

function ModalWindow:removeChoice(text)
    for i = 1, #self.choices do
        if self.choices[i].text == text then
            table.remove(self.choices, i)
            break
        end
    end
end

function ModalWindow:callChoice(text, player, button, choice)
    for _, choice in pairs(self.choices) do
        if choice.text == text then
            choice.callback(player, button, choice)
            return
        end
    end
end

function ModalWindow:clearChoices()
    self.choices = {}
end

function ModalWindow:clear()
    self.choices = {}
    self.buttons = {}
end

function ModalWindow:setPriority(priority)
    self.priority = priority and true or false
end

function ModalWindow:setId()
    if self.modalWindowId > 0 then
        return self.modalWindowId
    end

    self.modalWindowId = ModalWindowAutoID
    ModalWindowAutoID = ModalWindowAutoID + 1
    return self.modalWindowId
end

function ModalWindow:create()
    local modalWindow = __ModalWindow(self:setId(), self.title, self.message)
    modalWindow:setPriority(self.priority)

    local order = buttonOrder[math.min(#self.buttons, 4)]
    if order then
        for _, i in pairs(order) do
            local button = self.buttons[i]
            modalWindow:addButton(i, button.name)
            button.id = i

            if button.name == self.defaultEnterButton then
                modalWindow:setDefaultEnterButton(i)
            elseif button.name == self.defaultEscapeButton then
                modalWindow:setDefaultEscapeButton(i)
            end
        end
    end

    for i, choice in pairs(self.choices) do
        modalWindow:addChoice(i, choice.text)
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
    local modalWindows = ModalWindows[player:getId()]
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
    end
    return true
end

creatureEvent:register()
