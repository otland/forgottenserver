local christmasBundle = Action()

function christmasBundle.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local presents = { -- [christmas bundle item id] = {{reward item id, count}, ...}
		[6507] = { -- red bundle
			{6569, 15}, {2687, 20}, {2688, 10}, {2675, 10}, {2674, 5}, 6501, 6502, 6490, 6504, 6388
		},
		[6508] = { -- blue bundle
			{6569, 15}, {2687, 20}, {2688, 10}, {2675, 10}, {2674, 5}, 6501, 6502, 6490, 6505, 6388
		},
		[6509] = { -- green bundle
			{6569, 15}, {2687, 20}, {2688, 10}, {2675, 10}, {2674, 5}, 6501, 6502, 6490, 6503, 6388
		}
	}

	local targetItem = presents[item.itemid]
	if not targetItem then
		return true
	end

	local rewards = {}
	while #rewards < 7 do
		local count = 1
		local rand = math.random(#targetItem)
		local gift = targetItem[rand]
		if type(gift) == "table" then
			gift, count = unpack(gift)
		end
		rewards[#rewards + 1] = {gift, count}
		table.remove(targetItem, rand)
	end

	for i = 1, #rewards do
		player:addItem(unpack(rewards[i]))
	end
	item:remove(1)
	player:getPosition():sendMagicEffect(CONST_ME_GIFT_WRAPS)
	return true
end

christmasBundle:id(6507,6508,6509)
christmasBundle:register()
