local config = {
	{chanceFrom = 0, chanceTo = 4760, itemId = 6576}, -- fireworks rocket
	{chanceFrom = 4761, chanceTo = 6841, itemId = 6394}, -- cream cake
	{chanceFrom = 6842, chanceTo = 7975, itemId = 6574}, -- bar of chocolate
	{chanceFrom = 7976, chanceTo = 9007, itemId = 6578}, -- party hat
	{chanceFrom = 9008, chanceTo = 9519, itemId = 2114}, -- piggy bank
	{chanceFrom = 9520, chanceTo = 9692, itemId = 2153}, -- violet gem
	{chanceFrom = 9693, chanceTo = 9850, itemId = 5944}, -- soul orb
	{chanceFrom = 9851, chanceTo = 9885, itemId = 2156}, -- red gem
	{chanceFrom = 9886, chanceTo = 9907, itemId = 2112}, -- teddy bear
	{chanceFrom = 9908, chanceTo = 9925, itemId = 2520}, -- demon shield
	{chanceFrom = 9926, chanceTo = 9944, itemId = 6568}, -- panda teddy
	{chanceFrom = 9945, chanceTo = 9959, itemId = 2195}, -- boots of haste
	{chanceFrom = 9960, chanceTo = 9974, itemId = 2492}, -- dragon scale mail
	{chanceFrom = 9975, chanceTo = 9986, itemId = 2498}, -- royal helmet
	{chanceFrom = 9987, chanceTo = 9995, itemId = 2173}, -- amulet of loss
	{chanceFrom = 9996, chanceTo = 10000, itemId = 6566}, -- stuffed dragon
}

local redSurpriseBag = Action()

function redSurpriseBag.onUse(player, item, fromPosition, target, toPosition, isHotkey)
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

redSurpriseBag:id(6571)
redSurpriseBag:register()
