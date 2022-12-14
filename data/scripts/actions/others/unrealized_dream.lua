local config = {
	{chanceFrom = 1, chanceTo = 2},
	{chanceFrom = 3, chanceTo = 2167, itemId = 13537},
	{chanceFrom = 2168, chanceTo = 4243, itemId = 7459},
	{chanceFrom = 4244, chanceTo = 6196, itemId = 22609},
	{chanceFrom = 6197, chanceTo = 8149, itemId = 2355},
	{chanceFrom = 8150, chanceTo = 9823, itemId = 32220},
	{chanceFrom = 9824, chanceTo = 9923, itemId = 22607},
	{chanceFrom = 9924, chanceTo = 9990, itemId = 22606},
	{chanceFrom = 9924, chanceTo = 9990, itemId = 22605},
	{chanceFrom = 9991, chanceTo = 10001, itemId = 22604},
	{chanceFrom = 9991, chanceTo = 10001, itemId = 5929},
	{chanceFrom = 9991, chanceTo = 10001, itemId = 2666},
	{chanceFrom = 9991, chanceTo = 10001, itemId = 22396},
	{chanceFrom = 9991, chanceTo = 10001, itemId = 2560},
	{chanceFrom = 9991, chanceTo = 10001, itemId = 7735},
	{chanceFrom = 9991, chanceTo = 10001, itemId = 2657},
	{chanceFrom = 9991, chanceTo = 10001, itemId = 2072},
	{chanceFrom = 9991, chanceTo = 10001, itemId = 2745},
	{chanceFrom = 9991, chanceTo = 10001, itemId = 2114},
	{chanceFrom = 9991, chanceTo = 10001, itemId = 2223},
	{chanceFrom = 9991, chanceTo = 10001, itemId = 2111}
}

local unrealizedDream = Action()

function unrealizedDream.onUse(player, item, fromPosition, target, toPosition, isHotkey)
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

			item:getPosition():sendMagicEffect(CONST_ME_GIFT_WRAPS)
			item:remove(1)
			return true
		end
	end
	return false
end

unrealizedDream:id(22598)
unrealizedDream:register()
