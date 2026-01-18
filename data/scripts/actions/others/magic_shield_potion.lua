local action = Action()

local vocations = {VOCATION_SORCERER, VOCATION_DRUID, VOCATION_MASTER_SORCERER, VOCATION_ELDER_DRUID}
local minLevel = 14
local description = "Only sorcerers and druids of level 14 or above may drink this potion."

local manaShield = Condition(CONDITION_MANASHIELD_BREAKABLE)
manaShield:setParameter(CONDITION_PARAM_TICKS, 3 * 60 * 1000)

function action.onUse(player, item, fromPosition, target, toPosition, isHotkey)
    if type(target) == "userdata" and not target:isPlayer() then
        return false
    end

    if not player:getGroup():getAccess() and (minLevel and player:getLevel() < minLevel or vocations and
        not table.contains(vocations, player:getVocation():getId())) then
        player:say(description, TALKTYPE_POTION)
        return true
    end

    manaShield:setParameter(CONDITION_PARAM_MANASHIELD_BREAKABLE,
        math.min(player:getMaxMana(), 300 + 7.6 * player:getLevel() + 7 * player:getMagicLevel()))
    player:addCondition(manaShield)
    player:getPosition():sendMagicEffect(CONST_ME_ENERGYAREA)

    if configManager.getBoolean(configKeys.REMOVE_POTION_CHARGES) then
        item:remove(1)
    end

    return true
end

action:id(38219)
action:register()
