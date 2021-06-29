local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_EARTHDAMAGE)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ENERGY)
combat:setParameter(COMBAT_PARAM_CREATEITEM, ITEM_POISONFIELD_PVP)

local poisonFieldRune = Spell("rune")

function poisonFieldRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

poisonFieldRune:group("attack")
poisonFieldRune:id(26)
poisonFieldRune:name("Poison Field Rune")
poisonFieldRune:runeId(2285)
poisonFieldRune:allowFarUse(true)
poisonFieldRune:charges(3)
poisonFieldRune:level(14)
poisonFieldRune:magicLevel(0)
poisonFieldRune:cooldown(2000)
poisonFieldRune:groupCooldown(2000)
poisonFieldRune:isBlocking(true)
poisonFieldRune:register()
