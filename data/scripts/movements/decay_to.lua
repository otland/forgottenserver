local decayItems = {
	[293] = 294,
	[475] = 476,
	[1066] = 1067
}

local decayTo = MoveEvent()

function decayTo.onStepIn(creature, item, position, fromPosition)
	item:transform(decayItems[item.itemid])
	return true
end

for k, v in pairs(decayItems) do
	decayTo:id(k)
end

decayTo:register()
