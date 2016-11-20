local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_DRAWBLOOD)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_WEAPONTYPE)

function Creature:getHighestSkillLevel()
	local skillLevel = -1
	for skillType = SKILL_CLUB, SKILL_AXE do
		if skillLevel < self:getEffectiveSkillLevel(skillType) then
			skillLevel = self:getEffectiveSkillLevel(skillType)
		end
	end
	return skillLevel
end

function onTargetCreature(creature, target)
	local skill = creature:getHighestSkillLevel()
	local min = (creature:getLevel() / 80) + (skill * 0.2) + 2
	local max = (creature:getLevel() / 80) + (skill * 0.4) + 2
	local damage = math.random(min, max)
	creature:addDamageCondition(target, CONDITION_BLEEDING, 1, target:isPlayer() and math.floor(damage / 4 + 0.5) or damage)
	return true
end

combat:setCallback(CALLBACK_PARAM_TARGETCREATURE, "onTargetCreature")

function onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end
