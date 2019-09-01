local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
combat:setParameter(COMBAT_PARAM_DISPEL, CONDITION_PARALYZE)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)
combat:setArea(createCombatArea(AREA_CIRCLE3X3))

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	local min = (creature:getLevel() / 5) + (creature:getMagicLevel() * 4.6) + 100
	local max = (creature:getLevel() / 5) + (creature:getMagicLevel() * 9.6) + 125
	for _, target in ipairs(combat:getTargets(creature, variant)) do
		local master = target:getMaster()
		if target:isPlayer() or master and master:isPlayer() then
			doTargetCombatHealth(0, target, COMBAT_HEALING, min, max)
		end
	end
	return true
end

spell:name("Mass Healing")
spell:words("exura gran mas res")
spell:group("healing")
spell:vocation("druid", "elder druid")
spell:id(82)
spell:cooldown(2 * 1000)
spell:groupCooldown(1 * 1000)
spell:level(36)
spell:mana(150)
spell:isAggressive(false)
spell:isPremium(true)
spell:register()
