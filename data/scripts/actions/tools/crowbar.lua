local crowbar = Action()

function crowbar.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	return onUseCrowbar(player, item, fromPosition, target, toPosition, isHotkey)
end

crowbar:id(3304)
crowbar:register()
