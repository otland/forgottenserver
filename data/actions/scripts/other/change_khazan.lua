local config = {
	[ITEM_KHAZAN_GEM] = {changeTo = ITEM_KHAZAN_GEM_CLUSTER},
	[ITEM_KHAZAN_GEM_CLUSTER] = {changeBack = ITEM_KHAZAN_GEM, changeTo = ITEM_KHAZAN_TOKEN},
	[ITEM_KHAZAN_TOKEN] = {changeBack = ITEM_KHAZAN_GEM_CLUSTER}
}

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local token = config[item:getId()]
	if token.changeTo and item.type == 100 then
		item:remove()
		player:addItem(token.changeTo, 1)
	elseif token.changeBack then
		item:remove(1)
		player:addItem(token.changeBack, 100)
	else
		return false
	end
	return true
end
