local roastedMeat = Action()

function roastedMeat.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if target.itemid == 1423 then -- campfire
		item:transform(24843) -- roasted meat
		toPosition:sendMagicEffect(CONST_ME_HITBYFIRE)
	end
	return true
end

roastedMeat:id(24842)
roastedMeat:register()
