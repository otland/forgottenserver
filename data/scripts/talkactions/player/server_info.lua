local talkaction = TalkAction("!serverinfo")

function talkaction.onSay(player, words, param)
	player:sendTextMessage(MESSAGE_INFO_DESCR, "Server Info:"
					.. "\nExp rate: " .. Game.getExperienceStage(player:getLevel())
					.. "\nSkill rate: " .. configManager.getNumber(configKeys.RATE_SKILL)
					.. "\nMagic rate: " .. configManager.getNumber(configKeys.RATE_MAGIC)
					.. "\nLoot rate: " .. configManager.getNumber(configKeys.RATE_LOOT))
	return false
end

talkaction:register()
