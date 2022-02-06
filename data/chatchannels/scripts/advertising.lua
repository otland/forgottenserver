function canJoin(player)
	return player:getVocation():getId() ~= VOCATION_NONE or player:getAccountType() >= ACCOUNT_TYPE_SENIORTUTOR
end

local CHANNEL_ADVERTISING = 5

local muted = Condition(CONDITION_CHANNELMUTEDTICKS, CONDITIONID_DEFAULT)
muted:setParameter(CONDITION_PARAM_SUBID, CHANNEL_ADVERTISING)
muted:setParameter(CONDITION_PARAM_TICKS, 120000)

function onSpeak(player, type, message)
	if player:getAccountType() >= ACCOUNT_TYPE_GAMEMASTER then
		if type == TALKTYPE_CHANNEL_Y then
			return TALKTYPE_CHANNEL_O
		end
		return true
	end

	if player:getLevel() < 20 and not player:isPremium() then
		player:sendCancelMessage("You may not speak in this channel unless you have reached level 20 or your account has premium status.")
		return false
	end

	if player:getCondition(CONDITION_CHANNELMUTEDTICKS, CONDITIONID_DEFAULT, CHANNEL_ADVERTISING) then
		player:sendCancelMessage("You may only place one offer in two minutes.")
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

function onJoin(player)
	sendChannelMessage(CHANNEL_ADVERTISING, MESSAGE_GUILD, "Here you can advertise all kinds of things. Among others, you can trade items, advertise ingame events, seek characters for a quest or a hunting group, find members for your guild or look for somebody to help you with something.")
	sendChannelMessage(CHANNEL_ADVERTISING, MESSAGE_GUILD, "It goes without saying that all advertisements must conform to the Rules, e.g. it is illegal to advertise trades including real money.")
	return true
end
