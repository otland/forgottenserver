local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_DRAWBLOOD)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_WEAPONTYPE)

function onTargetCreature(creature, target)
	local skill = 0
	for i = SKILL_CLUB, SKILL_AXE do
		if skill < creature:getEffectiveSkillLevel(i) then
			skill = creature:getEffectiveSkillLevel(i)
		end
	end

	local min = (creature:getLevel() * 0.01) + (skill * 0.2) + 2
	local max = (creature:getLevel() * 0.01) + (skill * 0.4) + 2
	local damage = math.random(math.floor(min), math.floor(max))
	creature:addDamageCondition(target, CONDITION_BLEEDING, 1, target:isPlayer() and math.floor(damage / 4 + 0.5) or damage)
	return true
end

combat:setCallback(CALLBACK_PARAM_TARGETCREATURE, "onTargetCreature")

function onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end
