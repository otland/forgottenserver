local pick = Action()

function pick.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	return onUsePick(player, item, fromPosition, target, toPosition, isHotkey)
end

pick:id(3456)
pick:register()
