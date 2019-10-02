local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_EARTHDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_CARNIPHILA)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_EARTH)

function onTargetCreature(creature, target)
	local min = (creature:getLevel() / 80) + (creature:getMagicLevel() * 0.55) + 6
	local max = (creature:getLevel() / 80) + (creature:getMagicLevel() * 0.75) + 7
	local damage = math.random(math.floor(min) * 1000, math.floor(max) * 1000) / 1000
	creature:addDamageCondition(target, CONDITION_POISON, DAMAGELIST_LOGARITHMIC_DAMAGE, target:isPlayer() and damage / 2 or damage)
end

combat:setCallback(CALLBACK_PARAM_TARGETCREATURE, "onTargetCreature")

function onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end
