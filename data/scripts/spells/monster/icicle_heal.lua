local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, 0)
combat:setArea(createCombatArea(AREA_CIRCLE3X3))

function onTargetCreature(creature, target)
	if target:hasPlayerOwned() then
		return true
	end

	local min = 400
	local max = 600
	doTargetCombat(0, target, COMBAT_HEALING, min, max, CONST_ME_NONE)
	return true
end

combat:setCallback(CALLBACK_PARAM_TARGETCREATURE, "onTargetCreature")

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("icicle heal")
spell:words("###59")
spell:blockWalls(true)
spell:needLearn(true)
spell:isSelfTarget(true)
spell:register()
