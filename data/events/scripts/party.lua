function Party:onJoin(player)
	return true
end

function Party:onLeave(player)
	return true
end

function Party:onDisband()
	return true
end

function Party:onShareExperience(exp)
	local sharedExperienceMultiplier = 1.20 --20%
	local vocationsIds = {}
	table.insert(vocationsIds, self:getLeader():getVocation():getId())
	for _, member in ipairs(self:getMembers()) do
		if not isInArray(vocationsIds, member:getVocation():getId()) then
			table.insert(vocationsIds, member:getVocation():getId())
		end
	end
	
	local size = #vocationsIds
	if size > 1 then
		sharedExperienceMultiplier = 1.0 + ((size * (10 + (size - 1) * 5)) / 100)
	else
		sharedExperienceMultiplier = 1.20
	end
	
	exp = (exp * sharedExperienceMultiplier) / (self:getMembers() + 1)
	return exp
end
