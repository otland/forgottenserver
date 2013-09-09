local statues = {
	[18488] = SKILL_SWORD,
	[18489] = SKILL_AXE,
	[18490] = SKILL_CLUB,
	[18491] = SKILL_DISTANCE,
	[18492] = SKILL_MAGLEVEL
}

function onUse(cid, item, fromPosition, itemEx, toPosition)
	if not isPremium(cid) then
		doPlayerSendDefaultCancel(cid, RETURNVALUE_YOUNEEDPREMIUMACCOUNT)
		return true
	end

	if isPlayerPzLocked(cid) then
		return false
	end

	doPlayerSetOfflineTrainingSkill(cid, statues[item.itemid])
	doRemoveCreature(cid)
	return true
end
