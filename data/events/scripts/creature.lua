function Creature:onChangeOutfit(outfit)
	return true
end

function Creature:onAreaCombat(tile, isAggressive)
	return RETURNVALUE_NOERROR
end

function Creature:onTargetCombat(target)
	return RETURNVALUE_NOERROR
end

function Creature:onHear(speaker, words, type)
end
