local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_FIREDAMAGE)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_FIRE)
combat:setParameter(COMBAT_PARAM_CREATEITEM, ITEM_FIREFIELD_PVP_FULL)
combat:setArea(createCombatArea(AREA_WALLFIELD, AREADIAGONAL_WALLFIELD))

local spell = Spell(SPELL_RUNE)

function spell.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

spell:group("attack")
spell:id(28)
spell:name("Fire Wall Rune")
spell:runeId(2303)
spell:allowFarUse(true)
spell:charges(4)
spell:runeLevel(33)
spell:runeMagicLevel(6)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:isBlocking(true, false)
spell:register()
