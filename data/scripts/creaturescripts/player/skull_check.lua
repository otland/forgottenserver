local config = {
	mode = "new", -- old or new
	whiteSkullTime = 15 * 60 * 1000 -- 15 minutes
}

local newConfig = {
	redDailyLimit = 3,
	redWeeklyLimit = 5,
	redMonthlyLimit = 10,
	redSkullLength = 30 * 24 * 60 * 60, -- 30 days
	blackDailyLimit = 6,
	blackWeeklyLimit = 10,
	blackMonthlyLimit = 20,
	blackSkullLength = 45 * 24 * 60 * 60, -- 45 days
	daysUnjustified = 30
}

local oldConfig = {
	fragTime = 24 * 60 * 60, -- 24h
	killsToRed = 3,
	killsToBlack = 6
}

-- register event in player
local event = CreatureEvent("RegisterSkulls")

function event.onLogin(player)
	player:registerEvent("SkullCheck")
	player:registerEvent("SkullTick")
	return true
end

event:register()

-- event to check when player kill another player
event = CreatureEvent("SkullCheck")

local inFight = Condition(CONDITION_INFIGHT, CONDITIONID_DEFAULT)
inFight:setParameter(CONDITION_PARAM_TICKS, config.whiteSkullTime)

-- New PVP system
local function getUnjustifiedDates(player, offsetTime, days)
	local kills = {}

	local resultId = db.storeQuery("SELECT `time` FROM `player_deaths` WHERE `killed_by` = " .. db.escapeString(player:getName()) .. " AND `unjustified` = 1 AND `time` >= " .. offsetTime - (days * 86400) .. "")
	if resultId then
		repeat
			table.insert(kills, result.getNumber(resultId, "time"))
		until not result.next(resultId)
		result.free(resultId)
	end

	return kills
end

local function updateAttackerSkullNew(attacker, todayKills, weekKills, monthKills)
	local attackerSkull = attacker:getSkull()
	if attackerSkull == SKULL_BLACK then
		attacker:setSkullTime(newConfig.blackSkullLength)
	elseif attackerSkull == SKULL_RED then
		local blackSkullLimitReached = newConfig.blackDailyLimit <= todayKills or newConfig.blackWeeklyLimit <= weekKills or newConfig.blackMonthlyLimit <= monthKills
		if blackSkullLimitReached then
			attacker:setSkull(SKULL_BLACK)
			attacker:setSkullTime(newConfig.blackSkullLength)
		else
			attacker:setSkullTime(newConfig.redSkullLength)
		end
	else
		local redSkullLimitReached = newConfig.redDailyLimit <= todayKills or newConfig.redWeeklyLimit <= weekKills or newConfig.redMonthlyLimit <= monthKills
		if redSkullLimitReached then
			attacker:setSkull(SKULL_RED)
			attacker:setSkullTime(newConfig.redSkullLength)
		end
	end
end

local function updateAttackerNew(attacker)
	local now = os.time()
	local today = now - 86400
	local week = now - (7 * 86400)

	local kills = getUnjustifiedDates(attacker, now, newConfig.daysUnjustified)
	table.insert(kills, now)

	local todayKills = 0
	local weekKills = 0
	local monthKills = #kills

	for _, time in pairs(kills) do
		if time > today then
			todayKills = todayKills + 1
		end

		if time > week then
			weekKills = weekKills + 1
		end
	end

	updateAttackerSkullNew(attacker, todayKills, weekKills, monthKills)
end

-- Old PVP system
local function updateAttackerSkullOld(attacker, skullTime)
	local attackerSkull = attacker:getSkull()
	if attackerSkull == SKULL_BLACK then
		return
	end

	if oldConfig.killsToBlack > 0 then
		local time = (oldConfig.killsToBlack - 1) * oldConfig.fragTime
		if skullTime > time then
			attacker:setSkull(SKULL_BLACK)
		end
	elseif oldConfig.killsToRed > 0 then
		if attackerSkull == SKULL_RED then
			return
		end

		local time = (oldConfig.killsToRed - 1) * oldConfig.fragTime
		if skullTime > time then
			attacker:setSkull(SKULL_RED)
		end
	end
end

local function updateAttackerOld(attacker)
	local skullTime = attacker:getSkullTime() + oldConfig.fragTime
	attacker:setSkullTime(skullTime)
	updateAttackerSkullOld(attacker, skullTime)
end

-- PVP system
local function unjustifiedDead(attacker, target)
	if Game.getWorldType() == WORLD_TYPE_PVP_ENFORCED then
		return
	end

	if target:getSkull() ~= SKULL_NONE then
		return
	end

	if target:hasFlag(PlayerFlag_NotGainInFight) then
		return
	end

	if config.mode == "new" then
		updateAttackerNew(attacker)
	elseif config.mode == "old" then
		updateAttackerOld(attacker)
	end

	attacker:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Warning! The murder of " .. target:getName() .. " was not justified.")
end

function event.onKill(player, target, lastHit)
	local targetPlayer = target:getPlayer()
	if not targetPlayer then
		return true
	end
	
	-- if the battle is in a PVP area, do not drop loot/loss skill from the target
	if targetPlayer:getZone() == ZONE_PVP then
		targetPlayer:setDropLoot(false)
		targetPlayer:setSkillLoss(false)
		return true
	end

	-- self attack
	if player == targetPlayer then
		return true
	end

	if player:getZone() == ZONE_PVP or player:hasFlag(PlayerFlag_NotGainInFight) then
		return true
	end

	-- ignore when target from the same guild or party
	if player:isGuildMate(targetPlayer) or player:isPartner(targetPlayer) then
		return true
	end

	-- check if the player attacked and the target did not fight back.
	if not player:hasAttacked(targetPlayer) or targetPlayer:hasAttacked(player) then
		return true
	end

	if not player:isAtWarAgainst(targetPlayer) then
		unjustifiedDead(player, targetPlayer)
	end

	if lastHit and player:hasCondition(CONDITION_INFIGHT) then
		player:setPzLocked(true)
		player:addCondition(inFight)
	end
	return true
end

event:register()

-- event to check skull ticks
event = CreatureEvent("SkullTick")

local function updatePlayerSkull(player, skullTime)
	if skullTime > 0 then
		return
	end

	if player:hasCondition(CONDITION_INFIGHT) then
		return
	end

	local playerSkull = player:getSkull()
	if playerSkull == SKULL_BLACK or playerSkull == SKULL_RED then
		player:setSkull(SKULL_NONE)
	end
end

function event.onThink(player, interval)
	if Game.getWorldType() == WORLD_TYPE_PVP_ENFORCED then
		return true
	end

	local skullTime = player:getSkullTime() - (interval / 1000)
	player:setSkullTime(skullTime > 0 and skullTime or 0)
	updatePlayerSkull(player, skullTime)
	return true
end

event:register()
