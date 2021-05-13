local insectoidCell = Action()

function insectoidCell.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if player:getStorageValue(PlayerStorageKeys.insectoidCell) > os.time() then
		player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "The insectoid cell is empty.")
		return true
	end

	player:addItem(15572, 1) -- gooey mass
	player:setStorageValue(PlayerStorageKeys.insectoidCell, os.time() + 7 * 24 * 60 * 60)
	player:sendTextMessage(MESSAGE_EVENT_ADVANCE, "You have found a gooey mass.")
	return true
end

insectoidCell:id(14437, 14438) -- insectoid cell
insectoidCell:register()
