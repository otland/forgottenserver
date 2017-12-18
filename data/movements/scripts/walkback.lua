function onStepIn(creature, item, position, fromPosition)
	if item.uid > 0 and item.uid <= 65535 then
		if position == fromPosition then
			if creature:isPlayer() then
				local temple = creature:getTown():getTemplePosition()
				creature:teleportTo(temple, false)
			else
				creature:remove()	
			end
		else
			creature:teleportTo(fromPosition, false)
		end
	end
	return true
end
