local spell = Spell(SPELL_RUNE)

function spell.onCastSpell(creature, variant, isHotkey)
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

spell:group("support")
spell:id(30)
spell:name("Destroy Field Rune")
spell:runeId(2261)
spell:allowFarUse(true)
spell:charges(3)
spell:runeLevel(17)
spell:runeMagicLevel(3)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:isAggressive(false)
spell:range(5)
spell:register()
