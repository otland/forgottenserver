local CHANNEL_HELP = 7

local muted = createConditionObject(CONDITION_CHANNELMUTEDTICKS)
setConditionParam(muted, CONDITION_PARAM_SUBID, CHANNEL_HELP)
setConditionParam(muted, CONDITION_PARAM_TICKS, 3600000)

function onSpeak(cid, type, message)
	local playerAccountType = getPlayerAccountType(cid)
	if getPlayerLevel(cid) == 1 and playerAccountType == ACCOUNT_TYPE_NORMAL then
		doPlayerSendCancel(cid, "You may not speak into channels as long as you are on level 1.")
		return false
	end

	if getCreatureCondition(cid, CONDITION_CHANNELMUTEDTICKS, CHANNEL_HELP) then
		doPlayerSendCancel(cid, "You are muted from the Help channel for using it inappropriately.")
		return false
	end

	if playerAccountType >= ACCOUNT_TYPE_TUTOR then
		if string.sub(message, 1, 6) == "!mute " then
			local targetName = string.sub(message, 7)
			local target = getPlayerByName(targetName)
			if target ~= false then
				if playerAccountType > getPlayerAccountType(target) then
					if not getCreatureCondition(target, CONDITION_CHANNELMUTEDTICKS, CHANNEL_HELP) then
						doAddCondition(target, muted)
						sendChannelMessage(CHANNEL_HELP, TALKTYPE_CHANNEL_R1, getPlayerName(target) .. " has been muted by " .. getPlayerName(cid) .. " for using Help Channel inappropriately.")
					else
						doPlayerSendCancel(cid, "That player is already muted.")
					end
				else
					doPlayerSendCancel(cid, "You are not authorized to mute that player.")
				end
			else
				doPlayerSendDefaultCancel(cid, RETURNVALUE_PLAYERWITHTHISNAMEISNOTONLINE)
			end
			return false
		elseif string.sub(message, 1, 8) == "!unmute " then
			local targetName = string.sub(message, 9)
			local target = getPlayerByName(targetName)
			if target ~= false then
				if playerAccountType > getPlayerAccountType(target) then
					if getCreatureCondition(target, CONDITION_CHANNELMUTEDTICKS, CHANNEL_HELP) then
						doRemoveCondition(target, CONDITION_CHANNELMUTEDTICKS, CHANNEL_HELP)
						sendChannelMessage(CHANNEL_HELP, TALKTYPE_CHANNEL_R1, getPlayerName(target) .. " has been unmuted by " .. getPlayerName(cid) .. ".")
					else
						doPlayerSendCancel(cid, "That player is not muted.")
					end
				else
					doPlayerSendCancel(cid, "You are not authorized to unmute that player.")
				end
			else
				doPlayerSendDefaultCancel(cid, RETURNVALUE_PLAYERWITHTHISNAMEISNOTONLINE)
			end
			return false
		end
	end

	if type == TALKTYPE_CHANNEL_Y then
		if playerAccountType >= ACCOUNT_TYPE_TUTOR or getPlayerFlagValue(cid, PlayerFlag_TalkOrangeHelpChannel) then
			type = TALKTYPE_CHANNEL_O
		end
	elseif type == TALKTYPE_CHANNEL_O then
		if playerAccountType < ACCOUNT_TYPE_TUTOR and not getPlayerFlagValue(cid, PlayerFlag_TalkOrangeHelpChannel) then
			type = TALKTYPE_CHANNEL_Y
		end
	elseif type == TALKTYPE_CHANNEL_R1 then
		if playerAccountType < ACCOUNT_TYPE_GAMEMASTER and not getPlayerFlagValue(cid, PlayerFlag_CanTalkRedChannel) then
			if playerAccountType >= ACCOUNT_TYPE_TUTOR or getPlayerFlagValue(cid, PlayerFlag_TalkOrangeHelpChannel) then
				type = TALKTYPE_CHANNEL_O
			else
				type = TALKTYPE_CHANNEL_Y
			end
		end
	end
	return type
end
