local statues = {
	[18488] = SKILL_SWORD,
	[18489] = SKILL_AXE,
	[18490] = SKILL_CLUB,
	[18491] = SKILL_DISTANCE,
	[18492] = SKILL_MAGLEVEL
}

function onUse(player, item, fromPosition, targetEx, toPosition, isHotkey)
	local skill = statues[item:getId()]
	if not skill then
		return false
	end

	if player:getPremiumDays() == 0 then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_YOUNEEDPREMIUMACCOUNT))
		return true
	end

	if player:isPzLocked() then
		return false
	end

	doPlayerSetOfflineTrainingSkill(player, skill)
	player:remove()
	return true
end
