local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ENERGYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ENERGYHIT)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ENERGYBALL)
combat:setParameter(COMBAT_PARAM_CREATEITEM, ITEM_ENERGYFIELD_PVP)

local energyFieldRune = Spell("rune")

function energyFieldRune.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

energyFieldRune:group("attack")
energyFieldRune:id(27)
energyFieldRune:name("Energy Field Rune")
energyFieldRune:runeId(2277)
energyFieldRune:allowFarUse(true)
energyFieldRune:charges(3)
energyFieldRune:level(18)
energyFieldRune:magicLevel(3)
energyFieldRune:cooldown(2000)
energyFieldRune:groupCooldown(2000)
energyFieldRune:isBlocking(true)
energyFieldRune:register()
