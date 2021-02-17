function Party:onJoin(player)
	if EventCallback:has(EVENT_CALLBACK_ONJOIN) then
		return EventCallback:get(EVENT_CALLBACK_ONJOIN, self, player)
	else
		return true
	end
end

function Party:onLeave(player)
	if EventCallback:has(EVENT_CALLBACK_ONLEAVE) then
		return EventCallback:get(EVENT_CALLBACK_ONLEAVE, self, player)
	else
		return true
	end
end

function Party:onDisband()
	if EventCallback:has(EVENT_CALLBACK_ONDISBAND) then
		return EventCallback:get(EVENT_CALLBACK_ONDISBAND, self)
	else
		return true
	end
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

	exp = (exp * sharedExperienceMultiplier) / (#self:getMembers() + 1)
	return EventCallback:has(EVENT_CALLBACK_ONSHAREEXPERIENCE) and EventCallback:get(EVENT_CALLBACK_ONSHAREEXPERIENCE, self, exp, rawExp) or exp
end
