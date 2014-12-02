function onUse(player, item, fromPosition, targetEx, toPosition, isHotkey)
	if (targetEx:getUniqueId() <= 65535 or targetEx:getActionId() > 0) and (targetEx:getId() == 354 or targetEx:getId() == 355) then
		targetEx:transform(392)
		targetEx:decay()
		toPosition:sendMagicEffect(CONST_ME_POFF)
		return true
	end

	return false
end
