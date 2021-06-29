local destroyFieldRune = Spell("rune")

function destroyFieldRune.onCastSpell(creature, variant, isHotkey)
	local position = variant:getPosition()
	local tile = Tile(position)
	local field = tile and tile:getItemByType(ITEM_TYPE_MAGICFIELD)
	if field and table.contains(FIELDS, field:getId()) then
		field:remove()
		position:sendMagicEffect(CONST_ME_POFF)
		return true
	end

	creature:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
	creature:getPosition():sendMagicEffect(CONST_ME_POFF)
	return false
end

destroyFieldRune:group("support")
destroyFieldRune:id(30)
destroyFieldRune:name("Destroy Field Rune")
destroyFieldRune:runeId(2261)
destroyFieldRune:allowFarUse(true)
destroyFieldRune:charges(3)
destroyFieldRune:level(17)
destroyFieldRune:magicLevel(3)
destroyFieldRune:cooldown(2000)
destroyFieldRune:groupCooldown(2000)
destroyFieldRune:isAggressive(false)
destroyFieldRune:range(5)
destroyFieldRune:register()
