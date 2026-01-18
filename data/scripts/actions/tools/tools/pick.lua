local action = Action()

function action.onUse(player, item, fromPosition, target, toPosition, isHotkey)
    return onUsePick(player, item, fromPosition, target, toPosition, isHotkey)
end

action:id(2553)
action:register()
