local config = {
	{chanceFrom = 0, chanceTo = 38}, -- nothing
	{chanceFrom = 39, chanceTo = 2132, itemId = 2152, count = 2}, -- platinum coin
	{chanceFrom = 2133, chanceTo = 4187, itemId = 15487, count = 10}, -- larvae
	{chanceFrom = 4188, chanceTo = 6040, itemId = 2144, count = 2}, -- black pearl
	{chanceFrom = 6041, chanceTo = 7951, itemId = 7591, count = 2}, -- great health potion
	{chanceFrom = 7952, chanceTo = 9843, itemId = 7590, count = 2}, -- great mana potion
	{chanceFrom = 9844, chanceTo = 9941, itemId = 9971}, -- gold ingot
	{chanceFrom = 9942, chanceTo = 9987, itemId = 15546}, -- four-leaf clover
	{chanceFrom = 9988, chanceTo = 10000, itemId = 15492} -- hive scythe
}

local gooeyMass = Action()

function gooeyMass.onUse(player, item, fromPosition, target, toPosition, isHotkey)
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

			item:getPosition():sendMagicEffect(CONST_ME_HITBYPOISON)
			item:remove(1)
			return true
		end
	end
	return false
end

gooeyMass:id(15572)
gooeyMass:register()
