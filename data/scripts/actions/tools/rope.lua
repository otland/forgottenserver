local rope = Action()

function rope.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	return onUseRope(player, item, fromPosition, target, toPosition, isHotkey)
end

if Game.getClientVersion().min >= 810 then
	rope:id(7731)
end
rope:id(2120)
rope:register()
