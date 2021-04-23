local condition = Condition(CONDITION_OUTFIT)
condition:setTicks(200000)

local chameleonRune = Spell("rune")

function chameleonRune.onCastSpell(creature, variant, isHotkey)
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

chameleonRune:group("support")
chameleonRune:id(14)
chameleonRune:name("Chamaleon Rune")
chameleonRune:runeId(2291)
chameleonRune:allowFarUse(true)
chameleonRune:charges(1)
chameleonRune:level(27)
chameleonRune:magicLevel(4)
chameleonRune:cooldown(2000)
chameleonRune:groupCooldown(2000)
chameleonRune:isBlocking(true)
chameleonRune:isSelfTarget(true)
chameleonRune:register()
