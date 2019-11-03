local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_DRAWBLOOD)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_WEAPONTYPE)

local function getHighestSkillLevel(creature)
	local skillLevel = -1
	for skillType = SKILL_CLUB, SKILL_AXE do
		if skillLevel < creature:getEffectiveSkillLevel(skillType) then
			skillLevel = creature:getEffectiveSkillLevel(skillType)
		end
	end
	return skillLevel
end

function onCastSpell(creature, variant)
	local skill = getHighestSkillLevel(creature)
	local min = (creature:getLevel() / 80) + (skill * 0.2) + 2
	local max = (creature:getLevel() / 80) + (skill * 0.4) + 2
	local damage = math.random(math.floor(min) * 1000, math.floor(max) * 1000) / 1000
	for _, target in ipairs(combat:getTargets(creature, variant)) do
		creature:addDamageCondition(target, CONDITION_BLEEDING, DAMAGELIST_LOGARITHMIC_DAMAGE, target:isPlayer() and damage / 4 or damage)
	end
	return true
end
