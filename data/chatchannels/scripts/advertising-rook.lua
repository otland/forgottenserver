function canJoin(cid)
	local player = Player(cid) return player:getVocation() == VOCATION_NONE or player:getAccountType() >= ACCOUNT_TYPE_SENIORTUTOR
end

local CHANNEL_ADVERTISING_ROOK = 6

function onSpeak(cid, type, message)
	local player = Player(cid)
	if player:getAccountType() >= ACCOUNT_TYPE_GAMEMASTER then
		if type == TALKTYPE_CHANNEL_Y then
			return TALKTYPE_CHANNEL_O
		end
		return true
	end

	if player:getLevel() == 1 then
		player:sendCancelMessage("You may not speak into channels as long as you are on level 1.")
		return false
	end

	local muted = player:getCondition(CONDITION_CHANNELMUTEDTICKS, CONDITIONID_DEFAULT, CHANNEL_ADVERTISING_ROOK)
	if not muted then
		if not getPlayerFlagValue(cid, PlayerFlag_CannotBeMuted) then
			muted = Condition(CONDITION_CHANNELMUTEDTICKS, CONDITIONID_DEFAULT)
			muted:setParameter(CONDITION_PARAM_SUBID, CHANNEL_ADVERTISING_ROOK)
			muted:setParameter(CONDITION_PARAM_TICKS, 120000)
			player:addCondition(muted)
		end
	else
		player:sendCancelMessage("You may only place one offer in two minutes.")
		return false
	end

	if type == TALKTYPE_CHANNEL_O then
		if player:getAccountType() < ACCOUNT_TYPE_GAMEMASTER then
			type = TALKTYPE_CHANNEL_Y
		end
	elseif type == TALKTYPE_CHANNEL_R1 then
		if not getPlayerFlagValue(cid, PlayerFlag_CanTalkRedChannel) then
			type = TALKTYPE_CHANNEL_Y
		end
	end
	return type
end
