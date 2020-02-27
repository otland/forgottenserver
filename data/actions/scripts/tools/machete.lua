local jungleGrass = { 2782, 3985, 19433 }
local wildGrowth = { 1499, 11099 }

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local targetId = target.itemid
	if table.contains(jungleGrass, targetId) then
		target:transform(targetId == 19433 and 19431 or targetId - 1)
		target:decay()
		return true
	end

	if table.contains(wildGrowth, targetId) then
		toPosition:sendMagicEffect(CONST_ME_POFF)
		target:remove()
		return true
	end
	return destroyItem(player, target, toPosition)
end
