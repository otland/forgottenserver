function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if target.itemid == 2739 then -- wheat
		target:transform(2737)
		target:decay()
		Game.createItem(2694, 1, toPosition) -- bunch of wheat
		return true
	end
	if target.itemid == 5465 then -- burning sugar cane
		target:transform(5464)
		target:decay()
		Game.createItem(5467, 1, toPosition) -- bunch of sugar cane
		return true
	end
	return destroyItem(player, target, toPosition)
end
