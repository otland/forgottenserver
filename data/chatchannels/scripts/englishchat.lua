function onSpeak(player, type, message)
	local playerAccountType = player:getAccountType()
	if player:getLevel() == 1 and playerAccountType < ACCOUNT_TYPE_GAMEMASTER then
		player:sendCancelMessage("You may not speak into channels as long as you are on level 1.")
		return false
	end

	if type == TALKTYPE_CHANNEL_Y then
		if playerAccountType >= ACCOUNT_TYPE_GAMEMASTER then
			type = TALKTYPE_CHANNEL_O
		end
	elseif type == TALKTYPE_CHANNEL_O then
		if playerAccountType < ACCOUNT_TYPE_GAMEMASTER then
			type = TALKTYPE_CHANNEL_Y
		end
	elseif type == TALKTYPE_CHANNEL_R1 then
		if playerAccountType < ACCOUNT_TYPE_GAMEMASTER and not getPlayerFlagValue(player, PlayerFlag_CanTalkRedChannel) then
			type = TALKTYPE_CHANNEL_Y
		end
	end
	return type
end
