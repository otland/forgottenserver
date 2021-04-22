local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_FIREDAMAGE)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_FIRE)
combat:setParameter(COMBAT_PARAM_CREATEITEM, ITEM_FIREFIELD_PVP_FULL)

local fireFieldRune = Spell("rune")

function fireFieldRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

fireFieldRune:group("attack")
fireFieldRune:id(25)
fireFieldRune:name("Fire Field Rune")
fireFieldRune:runeId(2301)
fireFieldRune:allowFarUse(true)
fireFieldRune:charges(3)
fireFieldRune:level(15)
fireFieldRune:magicLevel(1)
fireFieldRune:cooldown(2000)
fireFieldRune:groupCooldown(2000)
fireFieldRune:isBlocking(true)
fireFieldRune:register()
