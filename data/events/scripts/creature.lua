function Creature:onChangeOutfit(outfit)
	return true
end

function Creature:onAreaCombat(tile, isAggressive)
	return true
end

function Creature:onTargetCombat(target)
	if target:isPlayer() then
		local protection = loginProtectionTable[target:getGuid()]
		if not protection then
			return true
		end

		if protection >= os.mtime() then
			return false
		end

		loginProtectionTable[target:getGuid()] = nil
    	end
    	return true
end
