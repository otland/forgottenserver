-- TODO: Rewrite this script using fluidtypes from LIQUIDS doc-file,
--      and correct itemid's to recieve the liquids.

local drunk = Condition(CONDITION_DRUNK)
    drunk:setParameter(CONDITION_PARAM_TICKS, 60000)

local poison = Condition(CONDITION_POISON)
    poison:setParameter(CONDITION_PARAM_DELAYED, true) -- Condition will delay the first damage from when it's added
    poison:setParameter(CONDITION_PARAM_MINVALUE, -50) -- Minimum damage the condition can do at total
    poison:setParameter(CONDITION_PARAM_MAXVALUE, -120) -- Maximum damage
    poison:setParameter(CONDITION_PARAM_STARTVALUE, -5) -- The damage the condition will do on the first hit
    poison:setParameter(CONDITION_PARAM_TICKINTERVAL, 4000) -- Delay between damages
    poison:setParameter(CONDITION_PARAM_FORCEUPDATE, true) -- Re-update condition when adding it(ie. min/max value)

local fluidType = {3, 4, 5, 7, 10, 11, 13, 15, 19}
local fluidMessage = {"Aah...", "Urgh!", "Mmmh.", "Aaaah...", "Aaaah...", "Urgh!", "Urgh!", "Aah...", "Urgh!"}

function onUse(cid, item, fromPosition, itemEx, toPosition)
    if ItemType(itemEx.itemid):isFluidContainer() and itemEx.type == 0 then
        Item(itemEx.uid):transform(itemEx.itemid, item.type)
        Item(item.uid):transform(item.itemid, 0)
        return true
    end
    if ItemType(itemEx.itemid):isFluidContainer() and item.type == 0 then
        Item(itemEx.uid):transform(itemEx.itemid, 0)
        Item(item.uid):transform(item.itemid, itemEx.type)
        return true
    end

    if itemEx.itemid == 1 then
        if item.type == 0 then
            Player(cid):sendTextMessage(MESSAGE_STATUS_SMALL, "It is empty.")
        elseif itemEx.uid == cid then
            Item(item.uid):transform(item.itemid, 0)
            if item.type == 3 or item.type == 15 then
                doTargetCombatCondition(0, cid, drunk, CONST_ME_NONE)
            elseif item.type == 4 then
                doTargetCombatCondition(0, cid, poison, CONST_ME_NONE)
            elseif item.type == 7 then
                Player(cid):addMana(math.random(50, 150))
                fromPosition:sendMagicEffect(CONST_ME_MAGIC_BLUE)
            elseif item.type == 10 then
                Player(cid):addHealth(60)
                fromPosition:sendMagicEffect(CONST_ME_MAGIC_BLUE)
            end
            for i = 0, #fluidType do
                if item.type == fluidType[i] then
                    Player(cid):say(fluidMessage[i], TALKTYPE_ORANGE_1)
                    return true
                end
            end
            Player(cid):say("Gulp.", TALKTYPE_ORANGE_1)
        else
            Item(item.uid):transform(item.itemid, 0)
            Game.createItem(2016, item.type, toPosition):decay()
        end
    else
        local fluidSource = ItemType(itemEx.itemid):getFluidSource()
        if fluidSource ~= 0 then
            Item(item.uid):transform(item.itemid, fluidSource)
        elseif item.type == 0 then
            Player(cid):sendTextMessage(MESSAGE_STATUS_SMALL, "It is empty.")
        else
            if toPosition.x == CONTAINER_POSITION then
                toPosition = Player(cid):getPosition()
            end
            Item(item.uid):transform(item.itemid, 0)
            Game.createItem(2016, item.type, toPosition):decay()
        end
    end

    return true
end
