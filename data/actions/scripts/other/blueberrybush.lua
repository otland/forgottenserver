function onUse(cid, item, fromPosition, itemEx, toPosition, isHotkey)
	local blueberryBush = Item(item.uid)
	blueberryBush:transform(2786)
	blueberryBush:decay()
	Game.createItem(2677, 3, fromPosition)
	return true
end
