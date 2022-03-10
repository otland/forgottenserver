local config = {
	{chanceFrom = 0, chanceTo = 471, itemId = 2114}, -- piggy bank
	{chanceFrom = 472, chanceTo = 1413, itemId = 2154}, -- yellow gem
	{chanceFrom = 1414, chanceTo = 1988, itemId = 13537}, -- bag of apple slices
	{chanceFrom = 1989, chanceTo = 2616, itemId = 3956}, -- tusk
	{chanceFrom = 2617, chanceTo = 3191, itemId = 5880}, -- iron ore
	{chanceFrom = 3192, chanceTo = 3557, itemId = 5879}, -- spider silk
	{chanceFrom = 3558, chanceTo = 4342, itemId = 5882}, -- red dragon scale
	{chanceFrom = 4343, chanceTo = 4708, itemId = 2165}, -- stealth ring
	{chanceFrom = 4709, chanceTo = 4874, itemId = 25393}, -- folded rift carpet
	{chanceFrom = 4875, chanceTo = 5040, itemId = 26192}, -- folded void carpet
	{chanceFrom = 5041, chanceTo = 6034, itemId = 2079}, -- war horn
	{chanceFrom = 6035, chanceTo = 6819, itemId = 2661}, -- scarf
	{chanceFrom = 6820, chanceTo = 6985, itemId = 9971}, -- gold ingot
	{chanceFrom = 6986, chanceTo = 7151, itemId = 25419}, -- shaggy ogre bag
	{chanceFrom = 7152, chanceTo = 7360, itemId = 2162}, -- magic light wand
	{chanceFrom = 7361, chanceTo = 7526, itemId = 6570}, -- surprise bag
	{chanceFrom = 7527, chanceTo = 7944, itemId = 25387}, -- rift tapestry
	{chanceFrom = 7945, chanceTo = 8624, itemId = 18215}, -- gnomish supply package
	{chanceFrom = 8625, chanceTo = 8938, itemId = 2199}, -- garlic necklace
	{chanceFrom = 8939, chanceTo = 9147, itemId = 2153}, -- violet gem
	{chanceFrom = 9148, chanceTo = 9513, itemId = 9813}, -- slightly rusted legs
	{chanceFrom = 9514, chanceTo = 9879, itemId = 2787, count = 10}, -- white mushroom
	{chanceFrom = 9880, chanceTo = 10000, itemId = 24850, count = 2} -- opal
}

local mysteriousRemains = Action()

function mysteriousRemains.onUse(player, item, fromPosition, target, toPosition, isHotkey)
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

			item:remove(1)
			return true
		end
	end
	return false
end

mysteriousRemains:id(26165)
mysteriousRemains:register()
