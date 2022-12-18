local config = {
	{itemId = 2111, message = "You try to concentrate and your dream comes true. You wished for something cool.", chanceFrom = 1, chanceTo = 100},
	{itemId = 2223, message = "You try to concentrate and your dream comes true. You knew it would be some rubbish!", chanceFrom = 101, chanceTo = 200},
	{itemId = 2114, message = "You try to concentrate and your dream comes true. You just thought about your wealth.", chanceFrom = 201, chanceTo = 300},
	{itemId = 2745, message = "You try to concentrate and your dream comes true. You just thought about spring.", chanceFrom = 301, chanceTo = 400},
	{itemId = 2072, message = "You try to concentrate and your dream comes true. Well, part of. You thought about getting rich and a pile of loot...", chanceFrom = 401, chanceTo = 500},
	{itemId = 9074, message = "You try to concentrate and your dream comes true. You were that curious for the surprise.", chanceFrom = 501, chanceTo = 600},
	{itemId = 2657, message = "You try to concentrate and ... Oops, where did that dream come from?", chanceFrom = 601, chanceTo = 700},
	{itemId = 22604, message = "You try to concentrate and your dream comes true. You just thought about a true challenge.", chanceFrom = 701, chanceTo = 800},
	{itemId = 7735, message = "You try to concentrate and your dream comes true. Somehow you seemed to get distracted when you thought of Ferumbras.", chanceFrom = 801, chanceTo = 900},
	{itemId = 5792, message = "You try to concentrate and your dream comes true. You just wondered if you'd be lucky this time.", chanceFrom = 901, chanceTo = 1000},
	{itemId = 2560, message = "You try to concentrate and your dream comes true. You shouldn't really think about yourself that often.", chanceFrom = 1001, chanceTo = 1100},
	{itemId = 22396, message = "You try to concentrate and your dream comes true. You thought of your last night's dream, You try to concentrate and your dream comes true. You couldn't focus on anything specific.", chanceFrom = 1101, chanceTo = 1200},
	{itemId = 2666, message = "You try to concentrate and your dream comes true. Unfortunately you were subconsciouly thinking about something to eat.", chanceFrom = 1201, chanceTo = 1300},
	{itemId = 5928, message = "You try to concentrate and your dream comes true. You just thought about a loyal companion.", chanceFrom = 1301, chanceTo = 1400},
	{itemId = 22605, message = "You try to concentrate and your dream comes true. You just thought about a true challenge.", chanceFrom = 1401, chanceTo = 1500},
	{itemId = 22606, message = "You try to concentrate and your dream comes true. You just thought about a true challenge.", chanceFrom = 1501, chanceTo = 1600},
	{itemId = 22607, message = "You try to concentrate and your dream comes true. You just thought about a true challenge.", chanceFrom = 1601, chanceTo = 1700},
	{itemId = 2121, message = "You try to concentrate and your dream comes true. You wonder what were you thinking about.", chanceFrom = 1701, chanceTo = 1800},
	{itemId = 2355, message = "You try to concentrate and your dream comes true. You just thought about having a true friend.", chanceFrom = 1801, chanceTo = 1900},
	{itemId = 22609, message = "You try to concentrate and your dream comes true. You wonder what were you thinking about.", chanceFrom = 1901, chanceTo = 2000},
	{itemId = 7459, message = "You try to concentrate and your dream comes true. Sadly you had cold ears just in that moment.", chanceFrom = 2001, chanceTo = 2100},
	{itemId = 13537, message = "You try to concentrate and your dream comes true. You thought about your sore feet.", chanceFrom = 2101, chanceTo = 2200}
}

local unrealizedDream = Action()

function unrealizedDream.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local chance = math.random(0, 2200)
	for i = 1, #config do
		local randomItem = config[i]
		if chance >= randomItem.chanceFrom and chance <= randomItem.chanceTo then
			if randomItem.itemId then
				local gift = randomItem.itemId
				local msg = randomItem.message

				player:addItem(gift, 1)
				player:sendTextMessage(MESSAGE_EVENT_ADVANCE, msg)
			end

			item:remove(1)
			return true
		end
	end
	return false
end

unrealizedDream:id(22598)
unrealizedDream:register()
