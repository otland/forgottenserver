function Combat:getPositions(creature, variant)
	local positions = {}
	self:setCallback(CALLBACK_PARAM_TARGETTILE, function(creature, position)
		positions[#positions + 1] = position
	end)
	self:execute(creature, variant)
	return positions
end

function Combat:getTargets(creature, variant)
	local targets = {}
	self:setCallback(CALLBACK_PARAM_TARGETCREATURE, function(creature, target)
		targets[#targets + 1] = target
	end)
	self:execute(creature, variant)
	return targets
end
