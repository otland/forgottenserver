local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_DRAWBLOOD)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_WEAPONTYPE)
combat:setParameter(COMBAT_PARAM_TARGETCASTERORTOPMOST, 1)
combat:setParameter(COMBAT_PARAM_BLOCKARMOR, 1)
combat:setParameter(COMBAT_PARAM_USECHARGES, 1)

local condition = Condition(CONDITION_BLEEDING)
condition:setParameter(CONDITION_PARAM_DELAYED, 1)

local damageTable = {
	{id = 1, 45, 43, 41, 39, 37, 35, 33, 31, 29, 27, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16},
	{id = 2, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6},
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
