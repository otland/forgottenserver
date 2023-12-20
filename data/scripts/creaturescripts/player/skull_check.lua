local config = {
	fragTime = 24 * 60 * 60, -- in seconds
	whiteSkullTime = 15 * 60, -- in seconds
    killsToRed = 3,
    killsToBlack = 6,
}

-- register event in player
local event = CreatureEvent("RegisterSkullCheck")

function event.onLogin(player)
	player:registerEvent("SkullCheck")
	return true
end

event:register()

-- event to check when player kill another player
event = CreatureEvent("SkullCheck")

local inFight = Condition(CONDITION_INFIGHT, CONDITIONID_DEFAULT)
inFight:setParameter(CONDITION_PARAM_TICKS, config.whiteSkullTime * 1000)

local function updateAttackerSkull(attacker, skullTime)
	local attackerSkull = attacker:getSkull()
	if attackerSkull == SKULL_BLACK then
		return
	end

	if config.killsToBlack > 0 then
		local time = (config.killsToBlack - 1) * config.fragTime
		if skullTime > time then
			attacker:setSkull(SKULL_BLACK)
		end
	elseif config.killsToRed > 0 then
		if attackerSkull == SKULL_RED then
			return
		end

		local time = (config.killsToRed - 1) * config.fragTime
		if skullTime > time then
			attacker:setSkull(SKULL_RED)
		end
	end
end

local function unjustifiedDead(attacker, target)
    if g_game.getWorldType() == WORLD_TYPE_PVP_ENFORCED then
        return
    end

	if target:getSkull() ~= SKULL_NONE then
		return
	end

	if target:hasFlag(PlayerFlag_NotGainInFight) then
		return
	end

	local skullTime = player:getSkullTime() + config.fragTime
	player:setSkullTime(skullTime)
	updateAttackerSkull(attacker, skullTime)

	attacker:sendTextMessage(MESSAGE_EVENT_ADVANCE, "Warning! The murder of " .. target:getName() .. " was not justified.");
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

	if not player:isInWar(targetPlayer) then
		unjustifiedDead(player, targetPlayer)
	end

	if lastHit and player:hasCondition(CONDITION_INFIGHT) then
		player:setPzLocked(true)
		player:addCondition(inFight)
	end
	return true
end

event:register()
