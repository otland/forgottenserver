local action = Action()

function action.onUse(player, item, fromPosition, target, toPosition, isHotkey)
    return onUseCrowbar(player, item, fromPosition, target, toPosition, isHotkey)
end

action:id(2416)
action:register()
