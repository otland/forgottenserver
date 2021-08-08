function canJoin(player)
	return player:getVocation():getId() ~= VOCATION_NONE or player:getAccountType() >= ACCOUNT_TYPE_SENIORTUTOR
end

local CHANNEL_MAIN_LFG = 3

local muted = Condition(CONDITION_CHANNELMUTEDTICKS, CONDITIONID_DEFAULT)
muted:setParameter(CONDITION_PARAM_SUBID, CHANNEL_MAIN_LFG)
muted:setParameter(CONDITION_PARAM_TICKS, 60000)

function onSpeak(player, type, message)
	if player:getAccountType() >= ACCOUNT_TYPE_GAMEMASTER then
		if type == TALKTYPE_CHANNEL_Y then
			return TALKTYPE_CHANNEL_O
		end
		return true
	end

	if player:getLevel() >= 15 then
		player:sendCancelMessage("You may not speak into this channel as long as you are below level 15.")
		return false
	end

	if player:getCondition(CONDITION_CHANNELMUTEDTICKS, CONDITIONID_DEFAULT, CHANNEL_MAIN_LFG) then
		player:sendCancelMessage("You may only post once per minute.")
		return false
	end
	player:addCondition(muted)

	if type == TALKTYPE_CHANNEL_O then
		if player:getAccountType() < ACCOUNT_TYPE_GAMEMASTER then
			type = TALKTYPE_CHANNEL_Y
		end
	elseif type == TALKTYPE_CHANNEL_R1 then
		if not player:hasFlag(PlayerFlag_CanTalkRedChannel) then
			type = TALKTYPE_CHANNEL_Y
		end
	end
	return type
end
