local watches = {2036, 1728, 1729, 1730, 1731}
local action = Action()

function action.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	player:sendTextMessage(MESSAGE_INFO_DESCR, "The time is " .. getFormattedWorldTime() .. ".")
	return true
end

action:id(table.unpack(watches))
action:register()
