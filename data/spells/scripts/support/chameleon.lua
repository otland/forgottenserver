local condition = Condition(CONDITION_OUTFIT)
condition:setTicks(200000)

function onCastSpell(creature, variant, isHotkey)
	local item = Tile(variant:getPosition()):getTopDownItem()
	if not item or item.itemid == 0 or not isMoveable(item.uid) then
		creature:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		creature:getPosition():sendMagicEffect(CONST_ME_POFF)
		return false
	end

	condition:setOutfit({lookTypeEx = item.itemid})
	creature:addCondition(condition)
	creature:getPosition():sendMagicEffect(CONST_ME_MAGIC_RED)
	return true
end
