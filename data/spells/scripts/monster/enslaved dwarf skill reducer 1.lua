local combat = {}

for i = 13, 50 do
combat[i] = createCombatObject()
setCombatParam(combat[i], COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_RED)

local condition = createConditionObject(CONDITION_ATTRIBUTES)
setConditionParam(condition, CONDITION_PARAM_TICKS, 6000)
setConditionParam(condition, CONDITION_PARAM_SKILL_SHIELDPERCENT, i)

local area = createCombatArea(AREA_CIRCLE2X2)
setCombatArea(combat[i], area)
setCombatCondition(combat[i], condition)
end

function onCastSpell(cid, var)
	return doCombat(cid, combat[math.random(13,50)], var)
end