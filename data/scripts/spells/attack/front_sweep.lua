local combat = Combat()
combat:setParameter(COMBAT_PARAM_TYPE, COMBAT_PHYSICALDAMAGE)
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_HITAREA)
combat:setParameter(COMBAT_PARAM_BLOCKARMOR, true)
combat:setParameter(COMBAT_PARAM_USECHARGES, true)
combat:setArea(createCombatArea(AREA_WAVE6, AREADIAGONAL_WAVE6))

function onGetFormulaValues(player, skill, attack, factor)
	local min = (player:getLevel() / 5) + (skill * attack * 0.04) + 11
	local max = (player:getLevel() / 5) + (skill * attack * 0.08) + 21
	return -min, -max
end

combat:setCallback(CALLBACK_PARAM_SKILLVALUE, "onGetFormulaValues")

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
	return combat:execute(creature, variant)
end

spell:name("Front Sweep")
spell:words("exori min")
spell:group("attack")
spell:vocation("knight", "elite knight")
spell:id(59)
spell:cooldown(6 * 1000)
spell:groupCooldown(2 * 1000)
spell:level(70)
spell:mana(200)
spell:needWeapon(true)
spell:needDirection(true)
spell:isPremium(true)
spell:register()
