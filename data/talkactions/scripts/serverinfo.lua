function onSay(cid, words, param)
	local player = Player(cid)
	player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Server Info:"
					.. "\nExp rate: " .. Game.getExperienceStage(player:getLevel())
					.. "\nSkill rate: " .. configManager.getNumber(configKeys.RATE_SKILL)
					.. "\nMagic rate: " .. configManager.getNumber(configKeys.RATE_MAGIC)
					.. "\nLoot rate: " .. configManager.getNumber(configKeys.RATE_LOOT))
	return false
end
