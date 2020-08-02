function Creature:onChangeOutfit(outfit)
	return true
end

function Creature:onAreaCombat(tile, isAggressive)
	return RETURNVALUE_NOERROR
end

function Creature:onTargetCombat(target)
	-- No Vocation Combat Config --
	disableNoVocationCombat = true
	-- No Vocation Combat Config --
	
	if disableNoVocationCombat then
		if self:isPlayer() and target:isPlayer() then
			if self:getVocation():getId() == VOCATION_NONE or target:getVocation():getId() == VOCATION_NONE then
				if not self:getGroup():getAccess() then
					return RETURNVALUE_YOUMAYNOTATTACKTHISPLAYER
				end
			end
		end
	end
	
	return RETURNVALUE_NOERROR
end

function Creature:onHear(speaker, words, type)
end
