local decayItems = {
	[2041] = 2042, [2042] = 2041, [2044] = 2045, [2045] = 2044, [2047] = 2048,
	[2048] = 2047, [2050] = 2051, [2051] = 2050, [2052] = 2053, [2053] = 2052,
	[2054] = 2055, [2055] = 2054
}

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	item:transform(decayItems[item:getId()])
	item:decay()
	return true
end
