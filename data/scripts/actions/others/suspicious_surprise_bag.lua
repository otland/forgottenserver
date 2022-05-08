local config = {
	{chanceFrom = 0, chanceTo = 3394}, -- nothing
	{chanceFrom = 3395, chanceTo = 5159, itemId = 1689}, -- yellow pillow
	{chanceFrom = 5160, chanceTo = 6954, itemId = 7735}, -- spellwand
	{chanceFrom = 6955, chanceTo = 8327, itemId = 2114}, -- piggy bank
	{chanceFrom = 8328, chanceTo = 9141, itemId = 6574}, -- bar of chocolate
	{chanceFrom = 9142, chanceTo = 9654, itemId = 6394}, -- cream cake
	{chanceFrom = 9655, chanceTo = 9850, itemId = 7377}, -- ice cream cone
	{chanceFrom = 9851, chanceTo = 9986, itemId = 9074}, -- explosive present
	{chanceFrom = 9987, chanceTo = 10000, itemId = 7487} -- toy mouse
}

local suspiciousSurpriseBag = Action()

function suspiciousSurpriseBag.onUse(player, item, fromPosition, target, toPosition, isHotkey)
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
				item:getPosition():sendMagicEffect(CONST_ME_GIFT_WRAPS)
			else
				item:getPosition():sendMagicEffect(CONST_ME_CAKE)
			end

			item:remove(1)
			return true
		end
	end
	return false
end

suspiciousSurpriseBag:id(9108)
suspiciousSurpriseBag:register()
