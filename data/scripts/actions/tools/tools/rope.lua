local action = Action()

function action.onUse(player, item, fromPosition, target, toPosition, isHotkey)
    return onUseRope(player, item, fromPosition, target, toPosition, isHotkey)
end

action:id(2120, 7731)
action:register()
