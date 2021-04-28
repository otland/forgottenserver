function onCastSpell(creature, variant, isHotkey)
	local target = Creature(variant:getNumber())
	if not target or not target:isMonster() then
		creature:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		creature:getPosition():sendMagicEffect(CONST_ME_POFF)
		return false
	end

	local monsterType = target:getType()
	if not creature:hasFlag(PlayerFlag_CanConvinceAll) then
		if not monsterType:isConvinceable() then
			creature:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
			creature:getPosition():sendMagicEffect(CONST_ME_POFF)
			return false
		end

		if #creature:getSummons() >= 2 then
			creature:sendCancelMessage("You cannot control more creatures.")
			creature:getPosition():sendMagicEffect(CONST_ME_POFF)
			return false
		end
	end

	local manaCost = target:getType():getManaCost()
	if creature:getMana() < manaCost and not creature:hasFlag(PlayerFlag_HasInfiniteMana) then
		creature:sendCancelMessage(RETURNVALUE_NOTENOUGHMANA)
		creature:getPosition():sendMagicEffect(CONST_ME_POFF)
		return false
	end

	creature:addMana(-manaCost)
	creature:addManaSpent(manaCost)
	creature:addSummon(target)
	creature:getPosition():sendMagicEffect(CONST_ME_MAGIC_BLUE)
	return true
end
