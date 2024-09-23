local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ENERGYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ENERGYAREA)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ENERGY)

function onGetFormulaValues(player, level, magicLevel)
	return -3, -7
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell(SPELL_RUNE)

function spell.onCastSpell(creature, variant, isHotkey)
	return combat:execute(creature, variant)
end

spell:group("attack")
spell:id(7)
spell:name("Lightest Magic Missile Rune")
spell:runeId(19392)
spell:allowFarUse(true)
spell:charges(10)
spell:runeLevel(1)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:runeMagicLevel(0)
spell:needTarget(true)
spell:isBlocking(true, false)
spell:register()
