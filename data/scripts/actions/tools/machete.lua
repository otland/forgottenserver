local machete = Action()

function machete.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	return onUseMachete(player, item, fromPosition, target, toPosition, isHotkey)
end

machete:id(3308, 3330)
machete:register()
