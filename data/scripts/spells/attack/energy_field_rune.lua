local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ENERGYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ENERGYHIT)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ENERGYBALL)
combat:setParameter(COMBAT_PARAM_CREATEITEM, ITEM_ENERGYFIELD_PVP)

local spell = Spell("rune")

function spell.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

spell:name("Energy Field Rune")
spell:group("attack")
spell:id(27)
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(18)
spell:magicLevel(3)
spell:runeId(2277)
spell:charges(3)
spell:isBlocking(true)
spell:allowFarUse(true)
spell:register()
