local machete = Action()

function machete.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	return onUseMachete(player, item, fromPosition, target, toPosition, isHotkey) or destroyItem(player, target, toPosition)
end

if Game.getClientVersion().min >= 730 then
	machete:id(2442)
end
machete:id(2420)
machete:register()
