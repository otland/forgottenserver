local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_EARTHDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_CARNIPHILA)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_SMALLEARTH)

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 4.5) + 35
	local max = (level / 5) + (magicLevel * 7.3) + 55
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("Ultimate Terra Strike")
spell:words("exori max tera")
spell:group("attack")
spell:vocation("druid", "elder druid")
spell:id(157)
spell:cooldown(30 * 1000)
spell:groupCooldown(4 * 1000)
spell:level(90)
spell:mana(100)
spell:range(3)
spell:needCasterTargetOrDirection(true)
spell:blockWalls(true)
spell:isPremium(true)
spell:register()
