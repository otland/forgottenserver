local holes = {468, 481, 483}
function onUse(player, item, fromPosition, targetEx, toPosition, isHotkey)
	if isInArray(holes, targetEx:getId()) then
		targetEx:transform(targetEx:getId() + 1)
		targetEx:decay()
	elseif targetEx:getId() == 231 then
		local rand = math.random(1, 100)
		if rand == 1 then
			Game.createItem(2159, 1, toPosition)
		elseif rand > 95 then
			Game.createMonster("Scarab", toPosition)
		end
		toPosition:sendMagicEffect(CONST_ME_POFF)
	else
		return false
	end

	return true
end
