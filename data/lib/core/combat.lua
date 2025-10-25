---Returns a table with all positions that the combat would hit when executed.
---@param creature Player|Monster
---@param variant Variant
---@return table<number, Position>
function Combat:getPositions(creature, variant)
	local positions = {}
	function onTargetTile(creature, position)
		positions[#positions + 1] = position
	end

	self:setCallback(CALLBACK_PARAM_TARGETTILE, "onTargetTile")
	self:execute(creature, variant)
	return positions
end

---Returns a table of all creatures that the combat would hit when executed.
---@param creature Player|Monster
---@param variant Variant
---@return table<number, Player|Monster>
function Combat:getTargets(creature, variant)
	local targets = {}
	function onTargetCreature(creature, target)
		targets[#targets + 1] = target
	end

	self:setCallback(CALLBACK_PARAM_TARGETCREATURE, "onTargetCreature")
	self:execute(creature, variant)
	return targets
end
