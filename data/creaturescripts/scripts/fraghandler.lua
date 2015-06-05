function onDeath(player, corpse, lasthitkiller, mostdamagekiller, lasthitunjustified, mostdamageunjustified)
	if getPlayerFlagValue(player, PlayerFlag_NotGainInFight) or player == lasthitkiller or Game.getWorldType() == WORLD_TYPE_PVP_ENFORCED then
		return true
	end

	if not lasthitkiller:isPlayer() then
		return true
	end

	if player:getSkull() ~= SKULL_NONE then
		return true
	end

	local playerParty = player:getParty() or 0
	if playerParty ~= 0 then
		local killerParty = lasthitkiller:getParty() or 0
		if killerParty ~= 0 then
			if playerParty == killerParty then
				return true
			end
		end
	end

	if player:getTile():hasFlag(TILESTATE_PVPZONE) then
		return true
	end

	local playerGuild = player:getGuild()
	playerGuild = playerGuild and playerGuild:getId()
	if playerGuild ~= 0 then
		local killerGuild = lasthitkiller:getGuild()
		killerGuild = killerGuild and killerGuild:getId() or 0
		if killerGuild ~= 0 then
			if playerGuild == killerGuild then
				return true
			else
				if isInWar(player:getId(), lasthitkiller:getId()) then
					return true
				end
			end
		end
	end

	lasthitkiller:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Warning! The murder of " .. player:getName() .. " was not justified.")
	lasthitkiller:setSkullTime(lasthitkiller:getSkullTime() + configManager.getNumber(configKeys.FRAG_TIME))

	if lasthitkiller:getSkull() ~= SKULL_BLACK then
		if configManager.getNumber(configKeys.KILLS_TO_BLACK) ~= 0 and (lasthitkiller:getSkullTime() > (configManager.getNumber(configKeys.KILLS_TO_BLACK) - 1) * configManager.getNumber(configKeys.FRAG_TIME)) then
			lasthitkiller:setSkull(SKULL_BLACK)
		elseif lasthitkiller:getSkull() ~= SKULL_RED and configManager.getNumber(configKeys.KILLS_TO_RED) ~= 0 and (lasthitkiller:getSkullTime() > (configManager.getNumber(configKeys.KILLS_TO_RED) - 1) * configManager.getNumber(configKeys.FRAG_TIME)) then
			lasthitkiller:setSkull(SKULL_RED)
		end
	end
	return true
end
