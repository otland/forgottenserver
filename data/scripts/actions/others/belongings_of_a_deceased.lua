local config = {
	{chanceFrom = 0, chanceTo = 216}, -- nothing
	{chanceFrom = 217, chanceTo = 1659, itemId = 2238}, -- worn leather boots
	{chanceFrom = 1660, chanceTo = 3069, itemId = 2237}, -- dirty cape
	{chanceFrom = 3070, chanceTo = 4465, itemId = 2115}, -- broken piggy bank
	{chanceFrom = 4466, chanceTo = 5025, itemId = 2148, count = 6}, -- gold coin
	{chanceFrom = 5026, chanceTo = 5546, itemId = 2787}, -- white mushroom
	{chanceFrom = 5547, chanceTo = 6025, itemId = 5890}, -- chicken feather
	{chanceFrom = 6026, chanceTo = 6513, itemId = 3976, count = 4}, -- worm
	{chanceFrom = 6514, chanceTo = 6999, itemId = 2695}, -- egg
	{chanceFrom = 7000, chanceTo = 7398, itemId = 5899}, -- turtle shell
	{chanceFrom = 7399, chanceTo = 7806, itemId = 5894}, -- bat wing
	{chanceFrom = 7807, chanceTo = 8191, itemId = 2403}, -- knife
	{chanceFrom = 8192, chanceTo = 8560, itemId = 10606}, -- bunch of troll hair
	{chanceFrom = 8561, chanceTo = 8915, itemId = 8859}, -- spider fangs
	{chanceFrom = 8916, chanceTo = 9211, itemId = 5902}, -- honeycomb
	{chanceFrom = 9212, chanceTo = 9344, itemId = 2661}, -- scarf
	{chanceFrom = 9345, chanceTo = 9440, itemId = 2199}, -- garlic necklace
	{chanceFrom = 8441, chanceTo = 9525, itemId = 13926}, -- plague bell
	{chanceFrom = 9526, chanceTo = 9598, itemId = 2143}, -- white pearl
	{chanceFrom = 9599, chanceTo = 9664, itemId = 5879}, -- spider silk
	{chanceFrom = 9665, chanceTo = 9728, itemId = 2114}, -- piggy bank
	{chanceFrom = 9729, chanceTo = 9792, itemId = 13925}, -- plague mask
	{chanceFrom = 9793, chanceTo = 9842, itemId = 10563}, -- book of prayers
	{chanceFrom = 9843, chanceTo = 9886, itemId = 2110}, -- doll
	{chanceFrom = 9887, chanceTo = 9929, itemId = 5880}, -- iron ore
	{chanceFrom = 9930, chanceTo = 9968, itemId = 5895}, -- fish fin
	{chanceFrom = 9969, chanceTo = 9995, itemId = 13508}, -- slug drug
	{chanceFrom = 9996, chanceTo = 9997, itemId = 2195}, -- boots of haste
	{chanceFrom = 9998, chanceTo = 9999, itemId = 5885}, -- flask of warrior's sweat
	{chanceFrom = 9999, chanceTo = 10000, itemId = 5804} -- nose ring
}

local belongings = Action()

function belongings.onUse(player, item, fromPosition, target, toPosition, isHotkey)
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

				local itemType = ItemType(gift)
				player:say("You found " .. (count > 1 and count or (itemType:getArticle() ~= "" and itemType:getArticle() or "")) .. " " .. (count > 1 and itemType:getPluralName() or itemType:getName()) .. " in the bag.", TALKTYPE_MONSTER_SAY)
			else
				player:say("You found nothing useful.", TALKTYPE_MONSTER_SAY)
			end

			item:getPosition():sendMagicEffect(CONST_ME_POFF)
			item:remove(1)
			return true
		end
	end
	return false
end

belongings:id(13670)
belongings:register()
