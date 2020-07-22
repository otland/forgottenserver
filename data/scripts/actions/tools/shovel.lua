local shovel = Action()

function shovel.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	return onUseShovel(player, item, fromPosition, target, toPosition, isHotkey)
end

if Game.getClientVersion().min >= 780 then
	shovel:id(5710)
end
shovel:id(2554)
shovel:register()
