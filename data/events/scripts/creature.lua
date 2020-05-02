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
	if words:find("fuck") then
		local filtered = string.gsub(words, "(fuck)", "****")
		speaker:say(filtered, type, false, self)
		return false
	end
	
	return true
end
