local transformItems = {
	[3743] = 4404, [4404] = 3743
}

local action = Action()

function action.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	item:transform(transformItems[item:getId()])
	return true
end

for key in pairs(transformItems) do
	action:id(key)
end

action:register()
