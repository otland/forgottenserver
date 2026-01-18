local action = Action()

function action.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if item:getActionId() == actionIds.drawWell then
		fromPosition.z = fromPosition.z + 1
		player:teleportTo(fromPosition, false)
	end
	return true
end

action:id(1369)
action:register()
