function Creature:onChangeOutfit(outfit)
	return true
end

function Creature:onAreaCombat(tile, isAggressive)
	return true
end

function Creature:onTargetCombat(target)
	if target:isPlayer() then
		if target:getStorageValue(1000) >= os.time() then -- Login Protection Time
			return false
		end
	end
	return true
end
