local config = {
	{chanceFrom = 0, chanceTo = 2000, itemId = 2158}, -- blue gem
	{chanceFrom = 2001, chanceTo = 4000, itemId = 2160}, -- crystal coin
	{chanceFrom = 4001, chanceTo = 6000, itemId = 9971}, -- gold ingot
	{chanceFrom = 6001, chanceTo = 8000, itemId = 2155}, -- green gem
	{chanceFrom = 8001, chanceTo = 10000, itemId = 2443} -- ravager's axe
}

local belongingsRavager = Action()

function belongingsRavager.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local chance = math.random(0, 10000)
	for i = 1, #config do
		local randomItem = config[i]
		if chance >= randomItem.chanceFrom and chance <= randomItem.chanceTo then
			local gift = randomItem.itemId
			local count = randomItem.count or 1
			if type(count) == "table" then
				count = math.random(count[1], count[2])
			end
			player:addItem(gift, count)

			local itemType = ItemType(gift)
			player:say("You found " .. (count > 1 and count or (itemType:getArticle() ~= "" and itemType:getArticle() or "")) .. " " .. (count > 1 and itemType:getPluralName() or itemType:getName()) .. " in the bag.", TALKTYPE_MONSTER_SAY)

			item:getPosition():sendMagicEffect(CONST_ME_POFF)
			item:remove(1)
			return true
		end
	end
	return false
end

belongingsRavager:id(23704)
belongingsRavager:register()
