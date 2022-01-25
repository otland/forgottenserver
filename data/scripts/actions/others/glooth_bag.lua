local config = {
	{chanceFrom = 0, chanceTo = 1875, itemId = 23529, count = 2}, -- glooth spear
	{chanceFrom = 1876, chanceTo = 3418, itemId = 23554}, -- glooth amulet
	{chanceFrom = 3419, chanceTo = 4933, itemId = 23549}, -- glooth club
	{chanceFrom = 4934, chanceTo = 6397, itemId = 23551}, -- glooth axe
	{chanceFrom = 6398, chanceTo = 7829, itemId = 23550}, -- glooth blade
	{chanceFrom = 7830, chanceTo = 8462, itemId = 23666}, -- glooth backpack
	{chanceFrom = 8463, chanceTo = 8975, itemId = 23514, count = 10}, -- glooth sandwich
	{chanceFrom = 8976, chanceTo = 9469, itemId = 23515, count = 10}, -- bowl of glooth soup
	{chanceFrom = 9470, chanceTo = 9922, itemId = 23517, count = 10}, -- glooth steak
	{chanceFrom = 9923, chanceTo = 10000, itemId = 23557} -- control unit
}

local gloothBag = Action()

function gloothBag.onUse(player, item, fromPosition, target, toPosition, isHotkey)
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

gloothBag:id(23574)
gloothBag:register()
