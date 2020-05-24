experienceStages = {
	{minlevel = 1, maxlevel = 8, multiplier = 7},
	{minlevel = 9, maxlevel = 20, multiplier = 6},
	{minlevel = 21, maxlevel = 50, multiplier = 5},
	{minlevel = 51, maxlevel = 100, multiplier = 4},
	{minlevel = 101, multiplier = 3}
}

stagesMap = {}
useLastStageLevel = false
lastStageLevel = 0

for _, stage in ipairs(experienceStages) do
	local minlevel = stage.minlevel or 1
	local maxlevel = stage.maxlevel or 0
	local multiplier = stage.multiplier or 1.0

	if not stage.maxlevel then
		lastStageLevel = minlevel
		useLastStageLevel = true
	end

	if not useLastStageLevel then
		for i = stage.minlevel, stage.maxlevel do
			stagesMap[i] = stage.multiplier
		end
	else
		stagesMap[lastStageLevel] = multiplier
	end
end