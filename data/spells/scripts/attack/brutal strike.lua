local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
setCombatParam(combat, COMBAT_PARAM_BLOCKARMOR, 1)
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_HITAREA)
setCombatParam(combat, COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_WEAPONTYPE)
setCombatParam(combat, COMBAT_PARAM_USECHARGES, 1)

function onGetPlayerMinMaxValues(cid, skill, attack, factor)
	local min = skill * attack / (100/2) + getPlayerLevel(cid) / 5 + 1
	local max = skill * attack / (100/4)  + getPlayerLevel(cid) / 5 + 6
	return -min, -max
end
setCombatCallback(combat, CALLBACK_PARAM_SKILLVALUE, "onGetPlayerMinMaxValues")

function onCastSpell(cid, var)
	return doCombat(cid, combat, var)
end

