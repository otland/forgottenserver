local config = {
	{chanceFrom = 0, chanceTo = 1500, itemId = 6541, count = 10}, -- coloured egg
	{chanceFrom = 1501, chanceTo = 3000, itemId = 6542, count = 10}, -- coloured egg
	{chanceFrom = 3001, chanceTo = 4500, itemId = 6543, count = 10}, -- coloured egg
	{chanceFrom = 4501, chanceTo = 6000, itemId = 6544, count = 10}, -- coloured egg
	{chanceFrom = 6001, chanceTo = 7500, itemId = 6545, count = 10}, -- coloured egg
	{chanceFrom = 7501, chanceTo = 8550, itemId = 6569, count = 10}, -- candy
	{chanceFrom = 8551, chanceTo = 9550, itemId = 6574}, -- bar of chocolate
	{chanceFrom = 9551, chanceTo = 9850, itemId = 4850}, -- hydra egg
	{chanceFrom = 9851, chanceTo = 9950, itemId = 6570}, -- blue surprise bag
	{chanceFrom = 9951, chanceTo = 9990, itemId = 6571}, -- red surprise bag
	{chanceFrom = 9991, chanceTo = 10000, itemId = 11400} -- phoenix egg
}

local surpriseNest = Action()

function surpriseNest.onUse(player, item, fromPosition, target, toPosition, isHotkey)
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

surpriseNest:id(16102)
surpriseNest:register()
