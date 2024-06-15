local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_ENERGYDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_BIGCLOUDS)
combat:setArea(createCombatArea(AREA_CIRCLE6X6))

function onGetFormulaValues(player, level, magicLevel)
	local min = (level / 5) + (magicLevel * 4) + 75
	local max = (level / 5) + (magicLevel * 10) + 150
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:group("attack")
spell:id(119)
spell:name("Rage of the Skies")
spell:words("exevo gran mas vis")
spell:level(55)
spell:mana(600)
spell:isSelfTarget(true)
spell:cooldown(40000)
spell:groupCooldown(4000)
spell:vocation("sorcerer;true", "master sorcerer;true")
spell:register()
