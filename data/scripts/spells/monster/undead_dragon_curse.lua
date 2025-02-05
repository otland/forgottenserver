local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_DEATHDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_SMALLCLOUDS)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_DEATH)
combat:setArea(createCombatArea(AREA_SQUAREWAVE7))

local spell = Spell(SPELL_INSTANT)

function spell.onCastSpell(creature, variant)
	local damage = math.random(154, 266)
	for _, target in ipairs(combat:getTargets(creature, variant)) do
		creature:addDamageCondition(target, CONDITION_CURSED, DAMAGELIST_EXPONENTIAL_DAMAGE, damage)
	end
	return true
end

spell:name("undead dragon curse")
spell:words("###10")
spell:needDirection(true)
spell:blockWalls(true)
spell:needLearn(true)
spell:register()
