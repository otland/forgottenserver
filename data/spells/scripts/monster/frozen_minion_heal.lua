local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, 0)
combat:setArea(createCombatArea(AREA_CIRCLE2X2))

local function onTargetCreature(creature, target)
	local min = 100
	local max = 200
	local master = target:getMaster()
	if target:isPlayer() and not master or master and master:isPlayer() then
		return true
	end

	doTargetCombat(0, target, COMBAT_HEALING, min, max, CONST_ME_NONE)
	return true
end

combat:setCallback(CALLBACK_PARAM_TARGETCREATURE, onTargetCreature)

function onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end
