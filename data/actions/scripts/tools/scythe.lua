function onUse(cid, item, fromPosition, target, toPosition, isHotkey)
	if target.itemid == 2739 then
		target:transform(2737)
		target:decay()
		Game.createItem(2694, 1, toPosition)
		return true
	end
	return destroyItem(cid, target, toPosition)
end
