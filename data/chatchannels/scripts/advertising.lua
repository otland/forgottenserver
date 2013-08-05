function canJoin(cid)
	return getPlayerVocation(cid) ~= VOCATION_NONE or getPlayerAccountType(cid) >= ACCOUNT_TYPE_SENIOR_TUTOR
end

local CHANNEL_ADVERTISING = 5

local muted = createConditionObject(CONDITION_CHANNELMUTEDTICKS)
setConditionParam(muted, CONDITION_PARAM_SUBID, CHANNEL_ADVERTISING)
setConditionParam(muted, CONDITION_PARAM_TICKS, 120000)

function onSpeak(cid, type, message)
	if getPlayerAccountType(cid) >= ACCOUNT_TYPE_GAMEMASTER then
		return true
	end

	if getCreatureCondition(cid, CONDITION_CHANNELMUTEDTICKS, CHANNEL_ADVERTISING) then
		doPlayerSendCancel(cid, "You may only place one offer in two minutes.")
		return false
	end
	doAddCondition(cid, muted)
	return true
end
