local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_FIREDAMAGE)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_FIRE)
combat:setParameter(COMBAT_PARAM_CREATEITEM, ITEM_FIREFIELD_PVP_FULL)
combat:setArea(createCombatArea(AREA_WALLFIELD, AREADIAGONAL_WALLFIELD))

local spell = Spell("rune")

function spell.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

spell:name("Fire Wall Rune")
spell:group("attack")
spell:id(28)
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(33)
spell:magicLevel(6)
spell:runeId(2303)
spell:charges(4)
spell:isBlocking(true)
spell:allowFarUse(true)
spell:register()
