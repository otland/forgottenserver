function Party:onJoin(player)
	if Event.onPartyJoin then
		return Event.onPartyJoin(self, player)
	end
	return true
end

function Party:onLeave(player)
	if Event.onPartyLeave then
		return Event.onPartyLeave(self, player)
	end
	return true
end

function Party:onDisband()
	if Event.onPartyDisband then
		return Event.onPartyDisband(self)
	end
	return true
end

function Party:onInvite(player)
	if Event.onPartyInvite then
		return Event.onPartyInvite(self, player)
	end
	return true
end

function Party:onRevokeInvitation(player)
	if Event.onPartyRevokeInvitation then
		return Event.onPartyRevokeInvitation(self, player)
	end
	return true
end

function Party:onPassLeadership(player)
	if Event.onPartyPassLeadership then
		return Event.onPartyPassLeadership(self, player)
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
	if Event.onPartyShareExperience then
		return Event.onPartyShareExperience(self, exp, rawExp)
	end
	return exp
end
