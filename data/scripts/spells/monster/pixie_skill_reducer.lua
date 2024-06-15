local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_FAEEXPLOSION)

local condition = Condition(CONDITION_ATTRIBUTES)
condition:setParameter(CONDITION_PARAM_TICKS, 6000)
condition:setParameter(CONDITION_PARAM_SKILL_DISTANCEPERCENT, 30)
condition:setParameter(CONDITION_PARAM_SKILL_SHIELDPERCENT, 30)
condition:setParameter(CONDITION_PARAM_SKILL_MELEEPERCENT, 30)

local area = createCombatArea(AREA_CIRCLE2X2)

combat:setArea(area)
combat:addCondition(condition)

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("pixie skill reducer")
spell:words("###72")
spell:blockWalls(true)
spell:needLearn(true)
spell:register()
