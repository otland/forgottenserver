local action = Action()

function action.onUse(player, item, fromPosition, target, toPosition, isHotkey)
    return onUseShovel(player, item, fromPosition, target, toPosition, isHotkey)
end

action:id(2554, 5710)
action:register()
