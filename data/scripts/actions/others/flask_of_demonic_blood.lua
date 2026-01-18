local action = Action()

local transform = {7588, 7589}

function action.onUse(player, item, fromPosition, target, toPosition, isHotkey)
    local reward = transform[math.random(#transform)]
    if fromPosition.x == CONTAINER_POSITION then
        local targetContainer = Container(item:getParent().uid)
        targetContainer:addItem(reward, 1)
    else
        Game.createItem(reward, 1, fromPosition)
    end

    item:getPosition():sendMagicEffect(CONST_ME_DRAWBLOOD)
    item:remove(1)
    return true
end

action:id(6558)
action:register()
