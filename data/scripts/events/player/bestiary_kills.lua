local BESTIARY_PARTY_KILL_SHARING = true

local function getKillersForBestiary(monster)
	if not BESTIARY_PARTY_KILL_SHARING then
		return monster:getKillers(true)
	end

	local killers = {}
	for _, killer in pairs(monster:getKillers(true)) do
		local party = killer:getParty()
		if party then
			for _, member in pairs({party:getLeader(), unpack(party:getMembers())}) do
				if party:isMemberSharingExp(member) then
					killers[member:getId()] = member
				end
			end
		else
			killers[killer:getId()] = killer
		end
	end
	return killers
end

local event = Event()

event.onCreatureKill = function(self, target)
	if not self:isPlayer() then
		return true
	end

	local monster = target:getMonster()
	if not monster or monster:getMaster() then
		return true
	end

	local raceId = monster:getType():getBestiaryInfo().raceId
	if raceId == 0 then
		return true
	end

	for _, killer in pairs(getKillersForBestiary(monster)) do
		killer:addBestiaryKills(raceId)

		if killer:isBestiaryTracked(raceId) then
			killer:sendTrackedBestiary()
		end
	end
	return true
end

event:register()
