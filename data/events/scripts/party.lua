function Party:onJoin(player)
	if EventCallback.onJoin then
		return EventCallback.onJoin(self, player)
	end
	return true
end

function Party:onLeave(player)
	if EventCallback.onLeave then
		return EventCallback.onLeave(self, player)
	end
	return true
end

function Party:onDisband()
	if EventCallback.onDisband then
		return EventCallback.onDisband(self)
	end
	return true
end

function Party:onShareExperience(exp)
	local sharedExperienceMultiplier = 1.20 --20%
	local vocationsIds = {}
	local rawExp = exp

	local vocationId = self:getLeader():getVocation():getBase():getId()
	if vocationId ~= VOCATION_NONE then
		table.insert(vocationsIds, vocationId)
	end

	for _, member in ipairs(self:getMembers()) do
		vocationId = member:getVocation():getBase():getId()
		if not table.contains(vocationsIds, vocationId) and vocationId ~= VOCATION_NONE then
			table.insert(vocationsIds, vocationId)
		end
	end

	local size = #vocationsIds
	if size > 1 then
		sharedExperienceMultiplier = 1.0 + ((size * (5 * (size - 1) + 10)) / 100)
	end

	exp = math.ceil((exp * sharedExperienceMultiplier) / (#self:getMembers() + 1))
	return EventCallback.onShareExperience and EventCallback.onShareExperience(self, exp, rawExp) or exp
end
