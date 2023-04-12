local function getKillersForBestiary(monster)
	if not configManager.getBoolean(configKeys.BESTIARY_PARTY_KILL_SHARING) then
		local mostDamagePlayer = monster:getMostDamagePlayer()
		return mostDamagePlayer and {mostDamagePlayer} or {}
	end

	local killers = {}
	for _, killer in pairs(monster:getKillers()) do
		if killer:isPlayer() then
			local party = killer:getParty()
			if party then
				for _, member in pairs({party:getLeader(), unpack(party:getMembers())}) do
					if party:isMemberSharingExp(member) then
						killers[#killers + 1] = member
					end
				end
			else
				killers[killers + 1] = killer
			end
		end
	end
	return killers
end

local creatureEvent = CreatureEvent("BestiaryKills")

function creatureEvent.onKill(player, target)
	local monster = target:getMonster()
	if not monster then
		return true
	end

	local raceId = monster:getType():getBestiaryInfo().raceId
	if raceId == 0 then
		return true
	end

	for _, killer in pairs(getKillersForBestiary(monster)) do
		killer:addBestiaryKills(raceId)
	end
	return true
end

creatureEvent:register()
