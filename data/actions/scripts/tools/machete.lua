function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if target.itemid == 2782 then
		target:transform(2781)
		target:decay()
		return true
	end
	return destroyItem(player, target, toPosition)
end
