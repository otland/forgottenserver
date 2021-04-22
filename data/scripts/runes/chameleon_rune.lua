local condition = Condition(CONDITION_OUTFIT)
condition:setTicks(200000)

local chamaleonRune = Spell("rune")

function chamaleonRune.onCastSpell(creature, variant, isHotkey)
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

chamaleonRune:group("support")
chamaleonRune:id(14)
chamaleonRune:name("Chamaleon Rune")
chamaleonRune:runeId(2291)
chamaleonRune:allowFarUse(true)
chamaleonRune:charges(1)
chamaleonRune:level(27)
chamaleonRune:magicLevel(4)
chamaleonRune:cooldown(2000)
chamaleonRune:groupCooldown(2000)
chamaleonRune:isBlocking(true)
chamaleonRune:isSelfTarget(true)
chamaleonRune:register()
