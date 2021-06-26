local config = {
	{chanceFrom = 0, chanceTo = 2500, itemId = 5741}, -- skull helmet
	{chanceFrom = 2501, chanceTo = 5000, itemId = 2160}, -- crystal coin
	{chanceFrom = 5001, chanceTo = 7500, itemId = 2436}, -- skull staff
	{chanceFrom = 7501, chanceTo = 10000, itemId = 9969} -- black skull
}

local belongingsShargon = Action()

function belongingsShargon.onUse(player, item, fromPosition, target, toPosition, isHotkey)
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

belongingsShargon:id(23705)
belongingsShargon:register()
