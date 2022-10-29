local config = {
	{chanceFrom = 0, chanceTo = 1438, itemId = 2787, count = 20}, -- white mushroom
	{chanceFrom = 1439, chanceTo = 2454, itemId = 18397}, -- mushroom pie
	{chanceFrom = 2455, chanceTo = 3259, itemId = 18437, count = 15}, -- envenomed arrow
	{chanceFrom = 3260, chanceTo = 4057, itemId = 18304, count = 15}, -- crystalline arrow
	{chanceFrom = 4058, chanceTo = 4836, itemId = 18457}, -- teleport crystal
	{chanceFrom = 4837, chanceTo = 5443, itemId = 7588, count = 2}, -- strong health potion
	{chanceFrom = 5444, chanceTo = 6024, itemId = 7589, count = 2}, -- strong mana potion
	{chanceFrom = 6025, chanceTo = 6568, itemId = 7618, count = 4}, -- health potion
	{chanceFrom = 6569, chanceTo = 7069, itemId = 7620, count = 4}, -- mana potion
	{chanceFrom = 7070, chanceTo = 7518, itemId = 7590}, -- great mana potion
	{chanceFrom = 7519, chanceTo = 7930, itemId = 7591}, -- great health potion
	{chanceFrom = 7931, chanceTo = 8270, itemId = 7443}, -- bullseye potion
	{chanceFrom = 8271, chanceTo = 8587, itemId = 7439}, -- berserk potion
	{chanceFrom = 8588, chanceTo = 8900, itemId = 5911}, -- red piece of cloth
	{chanceFrom = 8901, chanceTo = 9204, itemId = 2152, count = 5}, -- platinum coin
	{chanceFrom = 9205, chanceTo = 9504, itemId = 7440}, -- mastermind potion
	{chanceFrom = 9505, chanceTo = 9629, itemId = 18455}, -- pet pig
	{chanceFrom = 9630, chanceTo = 9741, itemId = 18522}, -- gnomish voucher type CA2
	{chanceFrom = 9742, chanceTo = 9837, itemId = 18519}, -- gnomish voucher type MA2
	{chanceFrom = 9838, chanceTo = 9890, itemId = 2160}, -- crystal coin
	{chanceFrom = 9891, chanceTo = 9923, itemId = 2156}, -- red gem
	{chanceFrom = 9924, chanceTo = 9956, itemId = 2154}, -- yellow gem
	{chanceFrom = 9957, chanceTo = 9986, itemId = 18509}, -- red teleport crystal
	{chanceFrom = 9987, chanceTo = 9993, itemId = 2158}, -- blue gem
	{chanceFrom = 9994, chanceTo = 10000, itemId = 2155} -- green gem
}

local gnomish = Action()

function gnomish.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local chance = math.random(0, 10000)
	for i = 1, #config do
		local randomItem = config[i]
		if chance >= randomItem.chanceFrom and chance <= randomItem.chanceTo then
			if randomItem.itemId then
				local gift = randomItem.itemId
				local count = randomItem.count or 1
				if type(count) == "table" then
					count = math.random(count[1], count[2])
				end
				player:addItem(gift, count)
			end

			item:getPosition():sendMagicEffect(CONST_ME_CRAPS)
			item:remove(1)
			return true
		end
	end
	return false
end

gnomish:id(18215)
gnomish:register()
