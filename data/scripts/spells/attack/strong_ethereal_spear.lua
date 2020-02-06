local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_HITAREA)
combat:setParameter(COMBAT_PARAM_DISTANCEEFFECT, CONST_ANI_ETHEREALSPEAR)
combat:setParameter(COMBAT_PARAM_BLOCKARMOR, true)

function onGetFormulaValues(player, skill, attack, factor)
	local distanceSkill = player:getEffectiveSkillLevel(SKILL_DISTANCE)
	local min = (player:getLevel() / 5) + distanceSkill + 7
	local max = (player:getLevel() / 5) + (distanceSkill * 1.5) + 13
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_SKILLVALUE, "onGetFormulaValues")

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("Strong Ethereal Spear")
spell:words("exori gran con")
spell:group("attack")
spell:vocation("paladin", "royal paladin")
spell:id(57)
spell:cooldown(8 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(90)
spell:mana(55)
spell:range(7)
spell:needTarget(true)
spell:blockWalls(true)
spell:isPremium(true)
spell:register()
