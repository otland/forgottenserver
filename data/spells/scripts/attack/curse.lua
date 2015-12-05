local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_DEATHDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MORTAREA)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_DEATH)

local condition = Condition(CONDITION_CURSED)
condition:setParameter(CONDITION_PARAM_DELAYED, 1)

local damageTable = {
	{id = 1, 45, 40, 35, 34},
	{id = 2, 33, 32, 31, 30},
	{id = 3, 29, 25, 24},
	{id = 4, 23, 20},
	{id = 5, 19, 15},
	{id = 6, 10},
	{id = 10, 5}
}

for i = 1, #damageTable do
	local t = damageTable[i]
	for j = 1, #t do
		condition:addDamage(t.id, 4000, -t[j])
	end
end

combat:setCondition(condition)

function onCastSpell(creature, var)
	return combat:execute(creature, var)
end
