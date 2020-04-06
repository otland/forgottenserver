local presents = {
	[6570] = { -- blue present
		{3598, 10}, {6393, 3}, 6279, 6574, 6578, 6575, 6577, 6569, 6576, 6572, 2995
	},
	[6571] = { -- red present
		{3035, 10}, {3035, 10}, {3035, 10}, 3036, 5944, 2993, 6568, 6566, 3386, 3420, 3079, 2995, 2995, 2995, 6392, 6392, 6576, 6576, 6578, 6578, 6574, 6574
	}
}

local surpriseBag = Action()

function surpriseBag.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local count = 1
	local targetItem = presents[item.itemid]
	if not targetItem then
		return true
	end

	local gift = targetItem[math.random(#targetItem)]
	if type(gift) == "table" then
		gift = gift[1]
		count = gift[2]
	end

	player:addItem(gift, count)
	item:remove(1)
	fromPosition:sendMagicEffect(CONST_ME_GIFT_WRAPS)
	return true
end

surpriseBag:id(6570, 6571)
surpriseBag:register()