local watch = Action()

function watch.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	player:sendTextMessage(MESSAGE_INFO_DESCR, "The time is " .. Game.getFormattedWorldTime() .. ".")
	return true
end

watch:id(1728, 1729, 1730, 1731, 2036, 3900, 6092, 7828, 9443, 9444)
watch:register()
