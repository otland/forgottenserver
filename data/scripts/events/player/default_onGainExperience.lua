local soulCondition = Condition(CONDITION_SOUL, CONDITIONID_DEFAULT)
soulCondition:setTicks(4 * 60 * 1000)
soulCondition:setParameter(CONDITION_PARAM_SOULGAIN, 1)

local function useStamina(player)
	local staminaMinutes = player:getStamina()
	if staminaMinutes == 0 then
		return
	end

	local playerId = player:getId()
	if not nextUseStaminaTime[playerId] then
		nextUseStaminaTime[playerId] = 0
	end

	local currentTime = os.time()
	local timePassed = currentTime - nextUseStaminaTime[playerId]
	if timePassed <= 0 then
		return
	end

	if timePassed > 60 then
		if staminaMinutes > 2 then
			staminaMinutes = staminaMinutes - 2
		else
			staminaMinutes = 0
		end
		nextUseStaminaTime[playerId] = currentTime + 120
	else
		staminaMinutes = staminaMinutes - 1
		nextUseStaminaTime[playerId] = currentTime + 60
	end
	player:setStamina(staminaMinutes)
end

local default = Event()

function default.onGainExperience(player, source, exp, rawExp, sendText)
	if not source or source:isPlayer() then
		return exp
	end

	local level = player:getLevel()

	-- Soul regeneration
	local vocation = player:getVocation()
	if player:getSoul() < vocation:getMaxSoul() and exp >= level then
		soulCondition:setParameter(CONDITION_PARAM_SOULTICKS, vocation:getSoulGainTicks() * 1000)
		player:addCondition(soulCondition)
	end

	-- Apply experience stage multiplier
	exp = exp * Game.getExperienceStage(level)

	-- Apply low level bonus
	exp = exp * (1 + player:calculateLowLevelBonus(level) / 100)

	-- Stamina modifier
	if configManager.getBoolean(configKeys.STAMINA_SYSTEM) then
		useStamina(player)

		local staminaMinutes = player:getStamina()
		if staminaMinutes > 2340 and player:isPremium() then
			exp = exp * 1.5
		elseif staminaMinutes <= 840 then
			exp = exp * 0.5
		end
	end
	return exp
end

default:register()

-- For this event, we use the trigger index math.huge so that this event is called last, thus ensuring that the
-- experience message is sent to the client with the correct value.

local message = Event()

function message.onGainExperience(player, source, exp, rawExp, sendText)
	if sendText and exp ~= 0 then
		local pos = player:getPosition()
		local expString = exp .. (exp ~= 1 and " experience points." or " experience point.")
		player:sendTextMessage(MESSAGE_EXPERIENCE, "You gained " .. expString, pos, exp, TEXTCOLOR_WHITE_EXP)

		local spectators = Game.getSpectators(pos, false, true)
		for _, spectator in ipairs(spectators) do
			if spectator ~= player then
				spectator:sendTextMessage(MESSAGE_EXPERIENCE_OTHERS, player:getName() .. " gained " .. expString)
			end
		end
	end
	return exp
end

message:register(math.huge)
