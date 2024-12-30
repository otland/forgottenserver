local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_HOLYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_HOLYDAMAGE)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_SMALLHOLY)

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 1.9) + 8
	local max = (level / 5) + (magicLevel * 3) + 18
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:group("attack")
spell:id(122)
spell:name("Divine Missile")
spell:words("exori san")
spell:level(40)
spell:mana(20)
spell:range(4)
spell:needCasterTargetOrDirection(true)
spell:blockWalls(true)
spell:cooldown(2000)
spell:groupCooldown(2000)
spell:vocation("paladin;true", "royal paladin;true")
spell:register()
