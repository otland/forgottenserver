local scythe = Action()

function scythe.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	return onUseScythe(player, item, fromPosition, target, toPosition, isHotkey) or destroyItem(player, target, toPosition)
end

if Game.getClientVersion().min >= 940 then
	scythe:id(15492)
end
if Game.getClientVersion().min >= 850 then
	scythe:id(10301)
end
if Game.getClientVersion().min >= 750 then
	scythe:id(3963)
end
scythe:id(2550)
scythe:register()
