function onUse(cid, item, fromPosition, itemEx, toPosition)
	if not isPremium(cid) then
		doPlayerSendDefaultCancel(cid, RETURNVALUE_YOUNEEDPREMIUMACCOUNT)
		return true
	end

	if isPlayerPzLocked(cid) then
		return false
	end

	if item.itemid == 18492 then
		doPlayerSetOfflineTrainingSkill(cid, SKILL_MAGLEVEL)
	elseif item.itemid == 18491 then
		doPlayerSetOfflineTrainingSkill(cid, SKILL_DISTANCE)
	elseif item.itemid == 18490 then
		doPlayerSetOfflineTrainingSkill(cid, SKILL_CLUB)
	elseif item.itemid == 18489 then
		doPlayerSetOfflineTrainingSkill(cid, SKILL_AXE)
	elseif item.itemid == 18488 then
		doPlayerSetOfflineTrainingSkill(cid, SKILL_SWORD)
	end

	doRemoveCreature(cid)
	return true
end
