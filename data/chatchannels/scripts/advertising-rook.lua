function canJoin(cid)
	return getPlayerVocation(cid) == VOCATION_NONE or getPlayerAccountType(cid) >= ACCOUNT_TYPE_SENIORTUTOR
end

local CHANNEL_ADVERTISING_ROOK = 6

local muted = createConditionObject(CONDITION_CHANNELMUTEDTICKS)
setConditionParam(muted, CONDITION_PARAM_SUBID, CHANNEL_ADVERTISING_ROOK)
setConditionParam(muted, CONDITION_PARAM_TICKS, 120000)

function onSpeak(cid, type, message)
	if getPlayerAccountType(cid) >= ACCOUNT_TYPE_GAMEMASTER then
		if type == TALKTYPE_CHANNEL_Y then
			return TALKTYPE_CHANNEL_O
		end
		return true
	end

	if getPlayerLevel(cid) == 1 then
		doPlayerSendCancel(cid, "You may not speak into channels as long as you are on level 1.")
		return false
	end

	if getCreatureCondition(cid, CONDITION_CHANNELMUTEDTICKS, CHANNEL_ADVERTISING_ROOK) then
		doPlayerSendCancel(cid, "You may only place one offer in two minutes.")
		return false
	end
	doAddCondition(cid, muted)

	if type == TALKTYPE_CHANNEL_O then
		if getPlayerAccountType(cid) < ACCOUNT_TYPE_GAMEMASTER then
			type = TALKTYPE_CHANNEL_Y
		end
	elseif type == TALKTYPE_CHANNEL_R1 then
		if not getPlayerFlagValue(cid, PlayerFlag_CanTalkRedChannel) then
			type = TALKTYPE_CHANNEL_Y
		end
	end
	return type
end
