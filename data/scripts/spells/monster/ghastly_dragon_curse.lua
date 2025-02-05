local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_DEATHDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_SMALLCLOUDS)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_DEATH)

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	local damage = math.random(128, 954)
	for _, target in ipairs(combat:getTargets(creature, variant)) do
		creature:addDamageCondition(target, CONDITION_CURSED, DAMAGELIST_EXPONENTIAL_DAMAGE, damage)
	end
	return true
end

spell:name("ghastly dragon curse")
spell:words("###1")
spell:blockWalls(true)
spell:needTarget(true)
spell:needLearn(true)
spell:register()
