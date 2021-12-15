
local condition = Condition(CONDITION_MANASHIELD_BREAKABLE)
condition:setParameter(CONDITION_PARAM_TICKS, 200000)

local magicShieldPotion = Action()

function magicShieldPotion.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if configManager.getBoolean(configKeys.MANASHIELD_BREAKABLE) == true then
		if player:getLevel() >= 14 and (player:getVocation():getId() == 1 or player:getVocation():getId() == 2 or player:getVocation():getId() == 5 or player:getVocation().getId() == 6) then
			condition:setParameter(CONDITION_PARAM_MANASHIELD_BREAKABLE, math.min(player:getMaxMana(), 300 + 7.6 * player:getLevel() + 7 * player:getMagicLevel()))
			player:addCondition(condition)
			player:getPosition():sendMagicEffect(CONST_ME_ENERGYAREA)
			item:remove(1)
		else
			player:say("Only sorcerers and druids of level 14 or above may drink this potion.", TALKTYPE_MONSTER_SAY)
		end
	end
	return true
end

magicShieldPotion:id(38219)
magicShieldPotion:register()