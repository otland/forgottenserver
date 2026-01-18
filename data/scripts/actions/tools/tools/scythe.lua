local action = Action()

function action.onUse(player, item, fromPosition, target, toPosition, isHotkey)
    return onUseScythe(player, item, fromPosition, target, toPosition, isHotkey)
end

action:id(2550)
action:register()
