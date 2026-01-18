local action = Action()

function action.onUse(player, item, fromPosition, target, toPosition, isHotkey)
    return onUseMachete(player, item, fromPosition, target, toPosition, isHotkey)
end

action:id(2420, 2442)
action:register()
