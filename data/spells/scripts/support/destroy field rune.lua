function onCastSpell(creature, var)
	local position = variantToPosition(var)
	local tile = position:getTile()
	local field = tile and tile:getItemByType(ITEM_TYPE_MAGICFIELD)
	if field and isInArray(FIELDS, field:getId()) then
		field:remove()
		position:sendMagicEffect(CONST_ME_POFF)
		return true
	end

	creature:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
	creature:getPosition():sendMagicEffect(CONST_ME_POFF)
	return false
end
