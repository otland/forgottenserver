local spell = Spell(SPELL_RUNE)

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

spell:group("support")
spell:id(83)
spell:name("Animate Dead Rune")
spell:runeId(2316)
spell:allowFarUse(true)
spell:charges(1)
spell:runeLevel(27)
spell:runeMagicLevel(4)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:isBlocking(true, false)
spell:register()
