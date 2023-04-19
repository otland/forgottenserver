function Party:onJoin(player)
	if hasEvent.onJoin then
		return Event.onJoin(self, player)
	end
	return true
end

function Party:onLeave(player)
	if hasEvent.onLeave then
		return Event.onLeave(self, player)
	end
	return true
end

function Party:onDisband()
	if hasEvent.onDisband then
		return Event.onDisband(self)
	end
	return true
end

function Party:onInvite(player)
	if hasEvent.onInvite then
		return Event.onInvite(self, player)
	end
	return true
end

function Party:onRevokeInvitation(player)
	if hasEvent.onRevokeInvitation then
		return Event.onRevokeInvitation(self, player)
	end
	return true
end

function Party:onPassLeadership(player)
	if hasEvent.onPassLeadership then
		return Event.onPassLeadership(self, player)
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
	return hasEvent.onShareExperience and Event.onShareExperience(self, exp, rawExp) or exp
end
