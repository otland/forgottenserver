local statues = {
	[18488] = SKILL_SWORD,
	[18489] = SKILL_AXE,
	[18490] = SKILL_CLUB,
	[18491] = SKILL_DISTANCE,
	[18492] = SKILL_MAGLEVEL
}

function onUse(cid, item, fromPosition, itemEx, toPosition)
	local skill = statues[item.itemid]
	if not skill then
		return false
	end

	local player = Player(cid)

	if player:getPremiumDays() == 0 then
		player:sendTextMessage(MESSAGE_STATUS_SMALL, Game.getReturnMessage(RETURNVALUE_YOUNEEDPREMIUMACCOUNT))
		return true
	end

	if player:isPzLocked() then
		return false
	end

	doPlayerSetOfflineTrainingSkill(cid, skill)
	player:remove()
	return true
end
