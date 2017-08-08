local combat = Combat()
combat:setArea(createCombatArea(AREA_CIRCLE3X3))

local condition = Condition(CONDITION_ATTRIBUTES)
condition:setParameter(CONDITION_PARAM_TICKS, 2 * 60 * 1000)
condition:setParameter(CONDITION_PARAM_SKILL_FIST, 3)
condition:setParameter(CONDITION_PARAM_SKILL_MELEE, 3)
condition:setParameter(CONDITION_PARAM_SKILL_DISTANCE, 3)
condition:setParameter(CONDITION_PARAM_BUFF_SPELL, true)

local positions

function onTargetTile(creature, position)
	positions[#positions + 1] = position
end

combat:setCallback(CALLBACK_PARAM_TARGETTILE, "onTargetTile")

function onCastSpell(creature, variant)
	positions = {}
	return creature:addPartyCondition(combat, variant, positions, condition, 60)
end
