local items = {
	[2114] = {
		failure = {
			item = {id = ITEM_GOLD_COIN, count = 1, effect = CONST_ME_POFF, transform = {id = 2115}},
		},

		success = {
			item = {id = ITEM_PLATINUM_COIN, count = 1, effect = CONST_ME_SOUND_YELLOW}
		}
	}
}

function onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local newItem = math.random(1, 5) == 1 and items[item:getId()].failure or items[item:getId()].success
	if newItem.item.transform then
		item:transform(newItem.item.transform.id)
	end

	player:addItem(newItem.item.id, newItem.item.count)
	item:getPosition():sendMagicEffect(newItem.item.effect)
	return true
end
