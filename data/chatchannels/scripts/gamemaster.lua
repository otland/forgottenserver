function canJoin(cid)
	local player = Player(cid)
	return player:getAccountType() >= ACCOUNT_TYPE_GAMEMASTER
end

function onSpeak(cid, type, message)
	local player = Player(cid)
	local playerAccountType = player:getAccountType()
	if type == TALKTYPE_CHANNEL_Y then
		if playerAccountType == ACCOUNT_TYPE_GOD then
			type = TALKTYPE_CHANNEL_O
		end
	elseif type == TALKTYPE_CHANNEL_O then
		if playerAccountType ~= ACCOUNT_TYPE_GOD then
			type = TALKTYPE_CHANNEL_Y
		end
	elseif type == TALKTYPE_CHANNEL_R1 then
		if playerAccountType ~= ACCOUNT_TYPE_GOD and not getPlayerFlagValue(cid, PlayerFlag_CanTalkRedChannel) then
			type = TALKTYPE_CHANNEL_Y
		end
	end
	return type
end
