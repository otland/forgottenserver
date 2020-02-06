local spell = Spell("rune")

function spell.onCastSpell(creature, variant, isHotkey)
	local position = variant:getPosition()
	local tile = Tile(position)
	if tile and creature:getSkull() ~= SKULL_BLACK then
		local corpse = tile:getTopDownItem()
		if corpse then
			local itemType = corpse:getType()
			if itemType:isCorpse() and itemType:isMovable() then
				local monster = Game.createMonster("Skeleton", position)
				if monster then
					corpse:remove()
					creature:addSummon(monster)
					position:sendMagicEffect(CONST_ME_MAGIC_BLUE)
					return true
				end
			end
		end
	end

	creature:getPosition():sendMagicEffect(CONST_ME_POFF)
	creature:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
	return false
end

spell:name("Animate Dead Rune")
spell:group("support")
spell:id(83)
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(27)
spell:magicLevel(4)
spell:runeId(2316)
spell:charges(1)
spell:isBlocking(true)
spell:isAggressive(false)
spell:allowFarUse(true)
spell:register()
