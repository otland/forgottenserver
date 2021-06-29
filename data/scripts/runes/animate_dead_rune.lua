local animateDeadRune = Spell("rune")

function animateDeadRune.onCastSpell(creature, variant, isHotkey)
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

animateDeadRune:group("support")
animateDeadRune:id(83)
animateDeadRune:name("Animate Dead Rune")
animateDeadRune:runeId(2316)
animateDeadRune:allowFarUse(true)
animateDeadRune:charges(1)
animateDeadRune:level(27)
animateDeadRune:magicLevel(4)
animateDeadRune:cooldown(2000)
animateDeadRune:groupCooldown(2000)
animateDeadRune:isBlocking(true)
animateDeadRune:register()
