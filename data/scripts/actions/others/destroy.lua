local destroy = Action()

function destroy.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	return destroyItem(player, target, toPosition)
end

for items = 3264, 3292 do
	destroy:id(items)
end
for items = 3294, 3303 do
	destroy:id(items)
end
for items = 3305, 3307 do
	destroy:id(items)
end
for items = 3309, 3329 do
	destroy:id(items)
end
for items = 3331, 3341 do
	destroy:id(items)
end

destroy:register()
