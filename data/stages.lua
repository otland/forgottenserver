-- set to true to enable experience stages
stagesEnabled = false

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
	local minlevel, maxlevel, multiplier

	if stage.minlevel then
		minlevel = stage.minlevel
	else
		minlevel = 1
	end

	if stage.maxlevel then
		maxlevel = stage.maxlevel
	else
		maxlevel = 0
		lastStageLevel = minlevel
		useLastStageLevel = true
	end

	if stage.multiplier then
		multiplier = stage.multiplier
	else
		multiplier = 1.0
	end

	if useLastStageLevel then
		stagesMap[lastStageLevel] = multiplier
	else
		for i = stage.minlevel, stage.maxlevel do
			stagesMap[i] = stage.multiplier
		end
	end
end