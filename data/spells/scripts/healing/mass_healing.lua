local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
combat:setParameter(COMBAT_PARAM_DISPEL, CONDITION_PARALYZE)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)
combat:setArea(createCombatArea(AREA_CIRCLE3X3))

local healMonsters = false

function onTargetCreature(creature, target)
	local player = creature:getPlayer()
	local min = (player:getLevel() / 5) + (player:getMagicLevel() * 4.6) + 100
	local max = (player:getLevel() / 5) + (player:getMagicLevel() * 9.6) + 125

	if not healMonsters then
		local master = target:getMaster()
		if target:isMonster() and not master or master and master:isMonster() then
			return true
		end
	end

	doTargetCombatHealth(creature:getId(), target, COMBAT_HEALING, min, max, CONST_ME_NONE)
	return true
end

combat:setCallback(CALLBACK_PARAM_TARGETCREATURE, "onTargetCreature")

function onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end
