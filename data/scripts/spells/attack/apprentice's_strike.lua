local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_FIREDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_FIREATTACK)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_FIRE)

function onGetFormulaValues(player, level, magicLevel)
	return -10, -20
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("Apprentice's Strike")
spell:words("exori min flam")
spell:group("attack")
spell:vocation("druid", "elder druid", "sorcerer", "master sorcerer")
spell:id(169)
spell:cooldown(2 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(8)
spell:mana(6)
spell:range(3)
spell:needCasterTargetOrDirection(true)
spell:blockWalls(true)
spell:register()
