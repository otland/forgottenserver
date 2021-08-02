local blessings = {
	"Spiritual Shielding",
	"Embrace of the World",
	"Fire of the Suns",
	"Spark of the Phoenix",
	"Wisdom of Solitude",
	"Twist of Fate"
}

local checkBless = Action()

function checkBless.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local message = {"Received blessings:"}
	for i, blessing in pairs(blessings) do
		if player:hasBlessing(i) then
			message[#message + 1] = blessing
		end
	end

	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, #message == 1 and "No blessings received." or table.concat(message, '\n'))
	return true
end

checkBless:id(12424, 6561)
checkBless:register()
