function onUse(cid, item, fromPosition, target, toPosition, isHotkey)
	if target.itemid == 2739 then
		local iEx = Item(target.uid)
		iEx:transform(2737)
		iEx:decay()
		Game.createItem(2694, 1, toPosition)
		return true
	end

	return destroyItem(cid, target, toPosition)
end
