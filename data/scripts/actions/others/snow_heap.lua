local snowHeap = Action()

function snowHeap.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	Game.createItem(2992, 1, item:getPosition())
	return true
end

snowHeap:id(611)
snowHeap:register()
