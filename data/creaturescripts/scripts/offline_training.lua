function onLogin(player)
	local lastLogout = player:getLastLogout()
	local offlineTime = lastLogout ~= 0 and math.min(os.time() - lastLogout, 86400 * 21) or 0
	local offlineTrainingSkill = player:getOfflineTrainingSkill()
	if offlineTrainingSkill == -1 then
		player:addOfflineTrainingTime(offlineTime * 1000)
		return true
	end

	player:setOfflineTrainingSkill(-1)

	if offlineTime < 600 then
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You must be logged out for more than 10 minutes to start offline training.")
		return true
	end

	local trainingTime = math.max(0, math.min(offlineTime, math.min(43200, player:getOfflineTrainingTime() / 1000)))
	player:removeOfflineTrainingTime(trainingTime * 1000)

	local remainder = offlineTime - trainingTime
	if remainder > 0 then
		player:addOfflineTrainingTime(remainder * 1000)
	end

	if trainingTime < 60 then
		return true
	end

	local text = "During your absence you trained for"
	local hours = math.floor(trainingTime / 3600)
	if hours > 1 then
		text = string.format("%s %d hours", text, hours)
	elseif hours == 1 then
		text = string.format("%s 1 hour", text)
	end

	local minutes = math.floor((trainingTime % 3600) / 60)
	if minutes ~= 0 then
		if hours ~= 0 then
			text = string.format("%s and", text)
		end

		if minutes > 1 then
			text = string.format("%s %d minutes", text, minutes)
		else
			text = string.format("%s 1 minute", text)
		end
	end

	text = string.format("%s.", text)
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, text)

	local vocation = player:getVocation()
	local promotion = vocation:getPromotion()
	local topVocation = not promotion and vocation or promotion

	local updateSkills = false
	if table.contains({SKILL_CLUB, SKILL_SWORD, SKILL_AXE, SKILL_DISTANCE}, offlineTrainingSkill) then
		local modifier = topVocation:getAttackSpeed() / 1000
		updateSkills = player:addOfflineTrainingTries(offlineTrainingSkill, (trainingTime / modifier) / (offlineTrainingSkill == SKILL_DISTANCE and 4 or 2))
	elseif offlineTrainingSkill == SKILL_MAGLEVEL then
		local gainTicks = topVocation:getManaGainTicks() * 2
		if gainTicks == 0 then
			gainTicks = 1
		end

		updateSkills = player:addOfflineTrainingTries(SKILL_MAGLEVEL, trainingTime * (vocation:getManaGainAmount() / gainTicks))
	end

	if updateSkills then
		player:addOfflineTrainingTries(SKILL_SHIELD, trainingTime / 4)
	end

	return true
end
