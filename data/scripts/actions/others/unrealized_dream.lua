local config = {
	{itemId = 2111, message = "You try to concentrate and your dream comes true. You wished for something cool.", chanceFrom = 0, chanceTo = 1032},
	{itemId = 2223, message = "You try to concentrate and your dream comes true. You knew it would be some rubbish!", chanceFrom = 1033, chanceTo = 2105},
	{itemId = 2114, message = "You try to concentrate and your dream comes true. You just thought about your wealth.", chanceFrom = 2106, chanceTo = 2590},
	{itemId = 2745, message = "You try to concentrate and your dream comes true. You just thought about spring.", chanceFrom = 2591, chanceTo = 3543},
	{itemId = 2072, message = "You try to concentrate and your dream comes true. Well, part of. You thought about getting rich and a pile of loot...", chanceFrom = 3544, chanceTo = 4024},
	{itemId = 9074, message = "You try to concentrate and your dream comes true. You were that curious for the surprise.", chanceFrom = 4025, chanceTo = 4982},
	{itemId = 2657, message = "You try to concentrate and ... Oops, where did that dream come from?", chanceFrom = 4983, chanceTo = 5937},
	{itemId = 22604, message = "You try to concentrate and your dream comes true. You just thought about a true challenge.", chanceFrom = 5938, chanceTo = 6125},
	{itemId = 7735, message = "You try to concentrate and your dream comes true. Somehow you seemed to get distracted when you thought of Ferumbras.", chanceFrom = 6126, chanceTo = 6543},
	{itemId = 5792, message = "You try to concentrate and your dream comes true. You just wondered if you'd be lucky this time.", chanceFrom = 6544, chanceTo = 7520},
	{itemId = 2560, message = "You try to concentrate and your dream comes true. You shouldn't really think about yourself that often.", chanceFrom = 7521, chanceTo = 8475},
	{itemId = 22396, message = "You try to concentrate and your dream comes true. You thought of your last night's dream, You try to concentrate and your dream comes true. You couldn't focus on anything specific.", chanceFrom = 8476, chanceTo = 8603},
	{itemId = 2666, message = "You try to concentrate and your dream comes true. Unfortunately you were subconsciouly thinking about something to eat.", chanceFrom = 8604, chanceTo = 8922},
	{itemId = 5928, message = "You try to concentrate and your dream comes true. You just thought about a loyal companion.", chanceFrom = 8923, chanceTo = 9307},
	{itemId = 22605, message = "You try to concentrate and your dream comes true. You just thought about a true challenge.", chanceFrom = 9308, chanceTo = 9512},
	{itemId = 22606, message = "You try to concentrate and your dream comes true. You just thought about a true challenge.", chanceFrom = 9513, chanceTo = 9715},
	{itemId = 22607, message = "You try to concentrate and your dream comes true. You just thought about a true challenge.", chanceFrom = 9716, chanceTo = 9766},
	{itemId = 2121, message = "You try to concentrate and your dream comes true. You wonder what were you thinking about.", chanceFrom = 9767, chanceTo = 9826},
	{itemId = 2355, message = "You try to concentrate and your dream comes true. You just thought about having a true friend.", chanceFrom = 9827, chanceTo = 9868},
	{itemId = 22609, message = "You try to concentrate and your dream comes true. You wonder what were you thinking about.", chanceFrom = 9869, chanceTo = 9919},
	{itemId = 7459, message = "You try to concentrate and your dream comes true. Sadly you had cold ears just in that moment.", chanceFrom = 9920, chanceTo = 9977},
	{itemId = 13537, message = "You try to concentrate and your dream comes true. You thought about your sore feet.", chanceFrom = 9978, chanceTo = 10000}
}

local unrealizedDream = Action()

function unrealizedDream.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local chance = math.random(0, 10000)
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
