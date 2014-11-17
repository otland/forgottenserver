function onUse(player, item, fromPosition, targetEx, toPosition, isHotkey)
	if targetEx:getId() == 2782 then
		targetEx:transform(2781)
		targetEx:decay()
		return true
	end

	return destroyItem(targetEx, toPosition)
end
