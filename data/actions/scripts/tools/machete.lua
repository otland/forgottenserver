local wildGrowth = { 1499, 11099 }
local jungleGrass = {
	[2782] = 2781,
	[3985] = 3984,
	[19433] = 19431
}

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local targetId = target.itemid
	if not targetId then
		return true
	end

	if table.contains(wildGrowth, targetId) then
		toPosition:sendMagicEffect(CONST_ME_POFF)
		target:remove()
		return true
	end

	local grass = jungleGrass[targetId]
	if grass then
		target:transform(grass)
		target:decay()
		return true
	end

	return destroyItem(player, target, toPosition)
end
