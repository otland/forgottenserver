local blessings = {
	"Spiritual Shielding",
	"Embrace of Tibia",
	"Fire of the Suns",
	"Spark of the Phoenix",
	"Wisdom of Solitude",
	"Twist of Fate"
}

local checkBless = Action()

function checkBless.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local result, bless = "Received blessings:"
	for i = 1, #blessings do
		bless = blessings[i]
		result = player:hasBlessing(i) and result .. "\n" .. bless or result
	end

	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, 20 > result:len() and "No blessings received." or result)
	return true
end

checkBless:id(12424, 6561)
checkBless:register()
