local config = {
	{chanceFrom = 0, chanceTo = 1046, itemId = 6569, count = 3}, -- candy
	{chanceFrom = 1047, chanceTo = 1997, itemId = 6280}, -- party cake
	{chanceFrom = 1998, chanceTo = 2962, itemId = 6576}, -- fireworks rocket
	{chanceFrom = 2963, chanceTo = 3872, itemId = 2114}, -- piggy bank
	{chanceFrom = 3873, chanceTo = 4782, itemId = 6572}, -- party trumpet
	{chanceFrom = 4783, chanceTo = 5842, itemId = 6575}, -- red balloons
	{chanceFrom = 5843, chanceTo = 6725, itemId = 6578}, -- party hat
	{chanceFrom = 6726, chanceTo = 7649, itemId = 6394}, -- cream cake
	{chanceFrom = 7650, chanceTo = 8478, itemId = 6577}, -- green balloons
	{chanceFrom = 8479, chanceTo = 9212, itemId = 2687, count = 10}, -- cookie
	{chanceFrom = 9213, chanceTo = 10000, itemId = 6574} -- bar of chocolate
}

local blueSurpriseBag = Action()

function blueSurpriseBag.onUse(player, item, fromPosition, target, toPosition, isHotkey)
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

blueSurpriseBag:id(6570)
blueSurpriseBag:register()
