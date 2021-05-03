function onStepOut(creature, item, position, fromPosition)
	if not creature:isPlayer() or creature:isInGhostMode() then
		return true
	end

	if item:getId() == 670 then -- snow
		item:transform(6594)
	else
		item:transform(item:getId() + 15)
	end

	creature:addAchievementProgress("Snowbunny", 10000)
	item:decay()
	return true
end
