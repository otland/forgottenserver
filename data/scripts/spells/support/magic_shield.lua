-- Old mana shield
local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)

local condition = Condition(CONDITION_MANASHIELD)
condition:setParameter(CONDITION_PARAM_TICKS, 200 * 1000)
combat:addCondition(condition)

-- New mana shield
local combatBreakableManaShield = Combat()
combatBreakableManaShield:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_BLUE)
combatBreakableManaShield:setParameter(COMBAT_PARAM_AGGRESSIVE, false)

local conditionBreakableManaShield = Condition(CONDITION_MANASHIELD_BREAKABLE)
conditionBreakableManaShield:setParameter(CONDITION_PARAM_TICKS, 180 * 1000)

local magicShield = Spell(SPELL_INSTANT)

function magicShield.onCastSpell(creature, variant)
	if configManager.getBoolean(configKeys.MANASHIELD_BREAKABLE) then
		local player = creature:getPlayer()
		if player then
			conditionBreakableManaShield:setParameter(CONDITION_PARAM_MANASHIELD_BREAKABLE, math.min(player:getMaxMana(), 300 + 7.6 * player:getLevel() + 7 * player:getMagicLevel()))
			player:addCondition(conditionBreakableManaShield)
			return combatBreakableManaShield:execute(creature, variant)
		end
	end
	return combat:execute(creature, variant)
end

magicShield:name("Magic Shield")
magicShield:id(44)
magicShield:words("utamo vita")
magicShield:level(14)
magicShield:mana(50)
magicShield:group("support")
magicShield:isAggressive(false)
magicShield:isSelfTarget(true)
if configManager.getBoolean(configKeys.MANASHIELD_BREAKABLE) then
	magicShield:cooldown(14000)
else
	magicShield:cooldown(2000)
end
magicShield:groupCooldown(2000)
magicShield:needLearn(false)
magicShield:vocation("druid;true", "elder druid;true", "sorcerer;true", "master sorcerer;true")
magicShield:register()
