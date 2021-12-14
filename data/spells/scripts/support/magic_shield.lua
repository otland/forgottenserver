-- Old mana shield
local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)

local condition = Condition(CONDITION_MANASHIELD)
condition:setParameter(CONDITION_PARAM_TICKS, 200000)
combat:addCondition(condition)

-- New mana shield
local combatBreakableManaShield = Combat()
combatBreakableManaShield:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
combatBreakableManaShield:setParameter(COMBAT_PARAM_AGGRESSIVE, false)

local conditionBreakableManaShield = Condition(CONDITION_MANASHIELD_BREAKABLE)
conditionBreakableManaShield:setParameter(CONDITION_PARAM_TICKS, 200000)

function onCastSpell(creature, variant)
	if configManager.getBoolean(configKeys.NEW_MANASHIELD) == true then
		local player = creature:getPlayer()
		if player then
			conditionBreakableManaShield:setParameter(CONDITION_PARAM_MANASHIELD_BREAKABLE, math.min(player:getMaxMana(), 300 + 7.6 * player:getLevel() + 7 * player:getMagicLevel()))
			player:addCondition(conditionBreakableManaShield)
			return combatBreakableManaShield:execute(creature, variant)
		end
	end
	return combat:execute(creature, variant)
end
