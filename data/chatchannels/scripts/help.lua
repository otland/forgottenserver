local CHANNEL_HELP = 7

local muted = createConditionObject(CONDITION_CHANNELMUTEDTICKS)
setConditionParam(muted, CONDITION_PARAM_SUBID, CHANNEL_HELP)
setConditionParam(muted, CONDITION_PARAM_TICKS, 3600000)

function onSpeak(cid, type, message)
	if getCreatureCondition(cid, CONDITION_CHANNELMUTEDTICKS, CHANNEL_HELP) then
		doPlayerSendCancel(cid, "You are muted from the Help channel for using it inappropriately.")
		return false
	end

	local playerAccountType = getPlayerAccountType(cid)
	if playerAccountType >= ACCOUNT_TYPE_TUTOR then
		if string.sub(message, 1, 6) == "!mute " then
			local targetName = string.sub(message, 7)
			local target = getPlayerByName(targetName)
			if target ~= false and playerAccountType > getPlayerAccountType(target) and not getCreatureCondition(target, CONDITION_CHANNELMUTEDTICKS, CHANNEL_HELP) then
				doAddCondition(target, muted)
				sendChannelMessage(CHANNEL_HELP, SPEAK_CHANNEL_R1, getPlayerName(target) .. " has been muted by " .. getPlayerName(cid) .. " for using Help Channel inappropriately.")
			end
		elseif string.sub(message, 1, 8) == "!unmute " then
			local targetName = string.sub(message, 9)
			local target = getPlayerByName(targetName)
			if target ~= false and playerAccountType > getPlayerAccountType(target) and getCreatureCondition(target, CONDITION_CHANNELMUTEDTICKS, CHANNEL_HELP) then
				doRemoveCondition(target, CONDITION_CHANNELMUTEDTICKS, CHANNEL_HELP)
				sendChannelMessage(CHANNEL_HELP, SPEAK_CHANNEL_R1, getPlayerName(target) .. " has been unmuted by " .. getPlayerName(cid) .. ".")
			end
		end
	end
	return true
end
