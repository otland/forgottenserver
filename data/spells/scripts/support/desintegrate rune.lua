local dead_human = {
	3058, 3059, 3060, 3061, 3064, 3065, 3066
}

function onCastSpell(creature, var, isHotkey)
	local position = Variant.getPosition(var)
	local tile = Tile(position)
	local targetItem = tile and tile:getTopVisibleThing()

	if targetItem and not targetItem:isCreature() then
		local desintegrate = false
		while not desintegrate
			and not isInArray(dead_human, targetItem:getId())
			and targetItem:getType():isMovable()
			and targetItem:getUniqueId() > 65535
			and targetItem:getActionId() == 0
		do
			targetItem:remove()
			desintegrate = true
			targetItem = tile:getTopVisibleThing()
		end

		if desintegrate then
			position:sendMagicEffect(CONST_ME_BLOCKHIT)
			return true
		end
	end

	creature:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
	creature:getPosition():sendMagicEffect(CONST_ME_POFF)
	return false
end
