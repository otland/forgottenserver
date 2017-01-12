local items = {
	[2692] = { -- liquids containers
		target = {
			id = {1775, 2005, 2006, 2007, 2008, 2009, 2011, 2012, 2013, 2014, 2015, 2023, 2031, 2032, 2033},
			transform = {subType = 0},
			fluidType = 1 -- water
		},
		add = {id = 2693, count = 1},
	},

	[2694] = { -- millstones
		target = {id = {1381, 1382, 1383, 1384}},
		add = {id = 2692, count = 1}
	}
}

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local newItem, targetId = items[item:getId()], target:getId()
	if not isInArray(newItem.target.id, targetId) then
		return false
	end

	if newItem.target.fluidType and target:getFluidType() ~= newItem.target.fluidType then
		return false
	end

	if newItem.target.transform then
		target:transform(targetId, newItem.target.transform.subType)
	end

	player:addItem(newItem.add.id, newItem.add.count)
	item:remove(1)
	return true
end
