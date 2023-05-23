--[[
	ModalWindowHelper Example
]]--

local moveDirectionTest = {
	["Right"] = function(player)
		local pos = player:getPosition()
		pos:getNextPosition(DIRECTION_EAST)
		player:teleportTo(pos, true)
	end,
	["Left"] = function(player)
		local pos = player:getPosition()
		pos:getNextPosition(DIRECTION_WEST)
		player:teleportTo(pos, true)
	end,
	["Up"] = function(player)
		local pos = player:getPosition()
		pos:getNextPosition(DIRECTION_NORTH)
		player:teleportTo(pos, true)
	end,
	["Down"] = function(player)
		local pos = player:getPosition()
		pos:getNextPosition(DIRECTION_SOUTH)
		player:teleportTo(pos, true)
	end
}

local talkAction = TalkAction("!modalTest")

function talkAction.onSay(player, words, param, type)
	local modalWindow = ModalWindow{
		title = "Modal Window Helper Example",
		message = "This is an example of ModalWindowHelper."
	}
	if param == "choices" then
		for text, callback in pairs(moveDirectionTest) do
			modalWindow:addChoice(text, function(player, button, choice)
				if button.name == "Select" then
					callback(player)
					modalWindow:sendToPlayer(player)
				end
			end)
		end

		modalWindow:addButton("Select")
		modalWindow:addButton("Cancel")
	elseif param == "buttons" then
		for direction, callback in pairs(moveDirectionTest) do
			modalWindow:addButton(direction, function(player, button, choice)
				callback(player)
				modalWindow:sendToPlayer(player)
			end)
		end
	end
	modalWindow:sendToPlayer(player)
	return false
end

--talkAction:accountType(ACCOUNT_TYPE_GOD)
--talkAction:access(true)
talkAction:separator(" ")
talkAction:register()
