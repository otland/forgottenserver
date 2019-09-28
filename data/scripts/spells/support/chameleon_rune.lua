local condition = Condition(CONDITION_OUTFIT)
condition:setTicks((3 * 60 + 20) * 1000)

local spell = Spell("rune")

function spell.onCastSpell(creature, variant, isHotkey)
	local position, item = variant:getPosition()
	if position.x == CONTAINER_POSITION then
		local container = creature:getContainerById(position.y - 64)
		if container then
			item = container:getItem(position.z)
		else
			item = creature:getSlotItem(position.y)
		end
	else
		item = Tile(position):getTopDownItem()
	end

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

spell:name("Chameleon Rune")
spell:group("support")
spell:id(14)
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(27)
spell:magicLevel(4)
spell:runeId(2291)
spell:charges(1)
spell:isSelfTarget(true)
spell:isBlocking(true)
spell:isAggressive(false)
spell:allowFarUse(true)
spell:register()
