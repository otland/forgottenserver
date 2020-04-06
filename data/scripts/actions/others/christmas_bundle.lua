local presents = { -- [christmas bundle item id] = {{reward item id, count}, ...}
		[6506] = { -- red bundle
			{6569, 15}, -- candy
			{3585, 5}, -- red apple
			{3586, 10}, -- orange
			{3598, 20}, -- cookie
			{3599, 10}, -- candy cane
			6500, -- gingerbreadman
			6501, -- christmas wreath
			6489, -- christmas branch
			6503, -- red christmas garland
			6387 -- christmas card
		},
		[6507] = { -- blue bundle
			{6569, 15}, -- candy
			{3585, 5}, -- red apple
			{3586, 10}, -- orange
			{3598, 20}, -- cookie
			{3599, 10}, -- candy cane
			6500, -- gingerbreadman
			6501, -- christmas wreath
			6489, -- christmas branch
			6505, -- blue christmas garland
			6387 -- christmas card
		},
		[6508] = { -- green bundle
			{6569, 15}, -- candy
			{3585, 5}, -- red apple
			{3586, 10}, -- orange
			{3598, 20}, -- cookie
			{3599, 10}, -- candy cane
			6500, -- gingerbreadman
			6501, -- christmas wreath
			6489, -- christmas branch
			6502, -- christmas garland
			6387 -- christmas card
		}
	}

local christmasBundle = Action()

function christmasBundle.onUse(player, item, fromPosition, target, toPosition, isHotkey)
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

for index, value in pairs(presents) do
	christmasBundle:id(index)
end

christmasBundle:register()
