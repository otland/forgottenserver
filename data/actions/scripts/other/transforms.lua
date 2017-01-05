local items = {
	[3743] = 4404, [4404] = 3743
}

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	item:transform(items[item:getId()])
	return true
end
