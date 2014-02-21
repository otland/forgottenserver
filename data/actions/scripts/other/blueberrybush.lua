function onUse(cid, item, fromPosition, itemEx, toPosition)
	local ITem = Item(item.uid)
	ITem:transform(2786)
	ITem:decay()
	Game.createItem(2677, 3, fromPosition)
	return true
end
