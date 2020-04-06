local statues = {
	[16198] = SKILL_SWORD,
	[16199] = SKILL_AXE,
	[16200] = SKILL_CLUB,
	[16201] = SKILL_DISTANCE,
	[16202] = SKILL_MAGLEVEL
}

local skillTrainer = Action()

function skillTrainer.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local skill = statues[item:getId()]
	if not player:isPremium() then
		player:sendCancelMessage(RETURNVALUE_YOUNEEDPREMIUMACCOUNT)
		return true
	end

	if player:isPzLocked() then
		return false
	end

	player:setOfflineTrainingSkill(skill)
	player:remove()
	return true
end

for index, value in pairs(statues) do
	skillTrainer:id(index)
end

skillTrainer:register()
