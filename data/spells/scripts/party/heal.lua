local combat = Combat()
combat:setArea(createCombatArea(AREA_CIRCLE3X3))

local condition = Condition(CONDITION_REGENERATION)
condition:setParameter(CONDITION_PARAM_TICKS, 2 * 60 * 1000)
condition:setParameter(CONDITION_PARAM_HEALTHGAIN, 20)
condition:setParameter(CONDITION_PARAM_HEALTHTICKS, 2 * 1000)
condition:setParameter(CONDITION_PARAM_BUFF_SPELL, true)

local positions

function onTargetTile(creature, position)
	positions[#positions + 1] = position
end

combat:setCallback(CALLBACK_PARAM_TARGETTILE, "onTargetTile")

function onCastSpell(creature, variant)
	positions = {}
	return creature:addPartyCondition(combat, variant, positions, condition, 120)
end

