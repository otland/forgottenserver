local combat = createCombatObject()
setCombatParam(combat, COMBAT_PARAM_TYPE, COMBAT_HEALING)
setCombatParam(combat, COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
setCombatParam(combat, COMBAT_PARAM_AGGRESSIVE, 0)
setCombatParam(combat, COMBAT_PARAM_DISPEL, CONDITION_PARALYZE)

local area = createCombatArea(AREA_CIRCLE3X3)
setCombatArea(combat, area)

function onGetFormulaValues(cid, level, maglevel)
min = ((level*0.2) + (maglevel*5.7) + 26)
max = ((level*0.2) + (maglevel*10.43) + 62)
return min, max
end

setCombatCallback(combat, CALLBACK_PARAM_LEVELMAGICVALUE, "onGetFormulaValues")

function onCastSpell(cid, var)
	if getTileInfo(getThingPos(cid)).protection == false then
		return doCombat(cid, combat, var)
	else
		doPlayerSendTextMessage(cid, MESSAGE_STATUS_CONSOLE_BLUE, "You can't cast this spell in protection zone.")
	end
end