local christmasBundle = Action()

function christmasBundle.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local presents = { -- [christmas bundle item id] = {{reward item id, count}, ...}
		[6507] = { -- red bundle
			{6569, 15}, -- candy
			{2687, 20}, -- cookie
			{2688, 10}, -- candy cane
			{2675, 10}, -- orange
			{2674, 5}, -- red apple
			6501, -- gingerbreadman
			6502, -- christmas wreath
			6490, -- christmas branch
			6504, -- red christmas garland
			6388 -- christmas card
		},
		[6508] = { -- blue bundle
			{6569, 15}, -- candy
			{2687, 20}, -- cookie
			{2688, 10}, -- candy cane
			{2675, 10}, -- orange
			{2674, 5}, -- red apple
			6501, -- gingerbreadman
			6502, -- christmas wreath
			6490, -- christmas branch
			6505, -- blue christmas garland
			6388 -- christmas card
		},
		[6509] = { -- green bundle
			{6569, 15}, -- candy
			{2687, 20}, -- cookie
			{2688, 10}, -- candy cane
			{2675, 10}, -- orange
			{2674, 5}, -- red apple
			6501, -- gingerbreadman
			6502, -- christmas wreath
			6490, -- christmas branch
			6503, -- christmas garland
			6388 -- christmas card
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
