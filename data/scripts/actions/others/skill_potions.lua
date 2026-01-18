local action = Action()

local berserk = Condition(CONDITION_ATTRIBUTES)
berserk:setParameter(CONDITION_PARAM_TICKS, 10 * 60 * 1000)
berserk:setParameter(CONDITION_PARAM_SKILL_MELEE, 5)
berserk:setParameter(CONDITION_PARAM_SKILL_SHIELD, -10)
berserk:setParameter(CONDITION_PARAM_BUFF_SPELL, true)

local mastermind = Condition(CONDITION_ATTRIBUTES)
mastermind:setParameter(CONDITION_PARAM_TICKS, 10 * 60 * 1000)
mastermind:setParameter(CONDITION_PARAM_STAT_MAGICPOINTS, 3)
mastermind:setParameter(CONDITION_PARAM_BUFF_SPELL, true)

local bullseye = Condition(CONDITION_ATTRIBUTES)
bullseye:setParameter(CONDITION_PARAM_TICKS, 10 * 60 * 1000)
bullseye:setParameter(CONDITION_PARAM_SKILL_DISTANCE, 5)
bullseye:setParameter(CONDITION_PARAM_SKILL_SHIELD, -10)
bullseye:setParameter(CONDITION_PARAM_BUFF_SPELL, true)

local potions = {
    [7439] = { -- berserk potion
        condition = berserk,
        vocations = {VOCATION_KNIGHT, VOCATION_ELITE_KNIGHT},
        effect = CONST_ME_MAGIC_RED,
        description = "Only knights may drink this potion.",
        text = "You feel stronger."
    },
    [7440] = { -- mastermind potion
        condition = mastermind,
        vocations = {VOCATION_SORCERER, VOCATION_DRUID, VOCATION_MASTER_SORCERER, VOCATION_ELDER_DRUID},
        effect = CONST_ME_MAGIC_BLUE,
        description = "Only sorcerers and druids may drink this potion.",
        text = "You feel smarter."
    },
    [7443] = { -- bullseye potion
        condition = bullseye,
        vocations = {VOCATION_PALADIN, VOCATION_ROYAL_PALADIN},
        effect = CONST_ME_MAGIC_GREEN,
        description = "Only paladins may drink this potion.",
        text = "You feel more accurate."
    }
}

function action.onUse(player, item, fromPosition, target, toPosition, isHotkey)
    if type(target) == "userdata" and not target:isPlayer() then
        return false
    end

    local potion = potions[item:getId()]
    if not player:getGroup():getAccess() and (potion.level and player:getLevel() < potion.level or potion.vocations and
        not table.contains(potion.vocations, player:getVocation():getId())) then
        player:say(potion.description, TALKTYPE_POTION)
        return true
    end

    player:addCondition(potion.condition)
    player:say(potion.text, TALKTYPE_POTION)
    player:getPosition():sendMagicEffect(potion.effect)

    if configManager.getBoolean(configKeys.REMOVE_POTION_CHARGES) then
        item:remove(1)
    end

    return true
end

for k, _ in pairs(potions) do
    action:id(k)
end
action:register()
