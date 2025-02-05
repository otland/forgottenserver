local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ICEDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ICEATTACK)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_SMALLICE)

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 1.4) + 8
	local max = (level / 5) + (magicLevel * 2.2) + 14
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:group("attack")
spell:id(112)
spell:name("Ice Strike")
spell:words("exori frigo")
spell:level(15)
spell:mana(20)
spell:range(3)
spell:needCasterTargetOrDirection(true)
spell:blockWalls(true)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("sorcerer;true", "master sorcerer;true", "druid;true", "elder druid;true")
spell:register()
