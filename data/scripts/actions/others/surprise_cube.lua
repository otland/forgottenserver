local config = {
	{chanceFrom = 0, chanceTo = 1450, itemId = 2152, count = 5}, -- platinum coin
	{chanceFrom = 1451, chanceTo = 2850, itemId = 18421}, -- green crystal fragment
	{chanceFrom = 2851, chanceTo = 3950, itemId = 18419}, -- cyan crystal fragment
	{chanceFrom = 3951, chanceTo = 5050, itemId = 18420}, -- red crystal fragment
	{chanceFrom = 5051, chanceTo = 6050, itemId = 2675, count = 20}, -- orange
	{chanceFrom = 6051, chanceTo = 6850, itemId = 18413}, -- blue crystal shard
	{chanceFrom = 6851, chanceTo = 7450, itemId = 18415}, -- green crystal shard
	{chanceFrom = 7451, chanceTo = 8050, itemId = 2169}, -- time ring
	{chanceFrom = 8051, chanceTo = 8450, itemId = 2213}, -- dwarven ring
	{chanceFrom = 8451, chanceTo = 8750, itemId = 2167}, -- energy ring
	{chanceFrom = 8751, chanceTo = 9050, itemId = 2165}, -- stealth ring
	{chanceFrom = 9051, chanceTo = 9350, itemId = 7440}, -- mastermind potion
	{chanceFrom = 9351, chanceTo = 9550, itemId = 2214}, -- ring of healing
	{chanceFrom = 9551, chanceTo = 9750, itemId = 7439}, -- berserk potion
	{chanceFrom = 9751, chanceTo = 9850, itemId = 18414}, -- violet crystal shard
	{chanceFrom = 9851, chanceTo = 9950, itemId = 7443}, -- bullseye potion
	{chanceFrom = 9951, chanceTo = 9960, itemId = 26145}, -- brown pit demon
	{chanceFrom = 9961, chanceTo = 9970, itemId = 26146}, -- green pit demon
	{chanceFrom = 9971, chanceTo = 9980, itemId = 26147}, -- blue pit demon
	{chanceFrom = 9981, chanceTo = 9990, itemId = 26148}, -- black pit demon
	{chanceFrom = 9991, chanceTo = 10000, itemId = 26149} -- red pit demon
}

local surpriseCube = Action()

function surpriseCube.onUse(player, item, fromPosition, target, toPosition, isHotkey)
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

surpriseCube:id(26144)
surpriseCube:register()
