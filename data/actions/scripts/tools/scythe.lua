function onUse(player, item, fromPosition, targetEx, toPosition, isHotkey)
	if targetEx:getId() == 2739 then
		targetEx:transform(2737)
		targetEx:decay()
		Game.createItem(2694, 1, toPosition)
		return true
	end

	return destroyItem(targetEx, toPosition)
end
