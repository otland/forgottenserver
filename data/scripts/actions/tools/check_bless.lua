local blessings = {
	[BLESSING_EMBRACE_OF_THE_WORLD] = "Embrace of the World",
	[BLESSING_FIRE_OF_THE_SUNS] = "Fire of the Suns",
	[BLESSING_SPARK_OF_THE_PHOENIX] = "Spark of the Phoenix",
	[BLESSING_WISDOM_OF_SOLITUDE] = "Wisdom of Solitude",
	[BLESSING_SPIRITUAL_SHIELDING] = "Spiritual Shielding",
	[BLESSING_HEART_OF_THE_MOUNTAIN] = "Heart of the Mountain",
	[BLESSING_BLOOD_OF_THE_MOUNTAIN] = "Blood of the Mountain",
	[BLESSING_TWIST_OF_FATE] = "Twist of Fate"
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
