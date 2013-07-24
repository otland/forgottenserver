local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
setCombatParam(combat, COMBAT_PARAM_BLOCKARMOR, true)
setCombatParam(combat, COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_WEAPONTYPE)
setCombatParam(combat, COMBAT_PARAM_USECHARGES, true)

function onGetPlayerMinMaxValues(cid, skill, attack, factor)
	local min = skill * attack / 100 + getPlayerLevel(cid) / 5 + 1
	local max = skill * attack / (100/3)  + getPlayerLevel(cid) / 5 + 6
	return -min, -max
end
setCombatCallback(combat, CALLBACK_PARAM_SKILLVALUE, "onGetPlayerMinMaxValues")

function onCastSpell(cid, var)
	return doCombat(cid, combat, var)
end

