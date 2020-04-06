local blueberrybush = Action()

function blueberrybush.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	item:transform(3700)
	item:decay()
	Game.createItem(3588, 3, fromPosition)
	return true
end

blueberrybush:id(3699)
blueberrybush:register()
