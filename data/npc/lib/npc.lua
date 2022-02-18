-- Including the Advanced NPC System
dofile('data/npc/lib/npcsystem/npcsystem.lua')

function msgcontains(message, keyword)
	local message, keyword = message:lower(), keyword:lower()
	if message == keyword then
		return true
	end

	return message:find(keyword) and not message:find('(%w+)' .. keyword)
end

function doNpcSellItem(cid, itemid, amount, subType, ignoreCap, inBackpacks, backpack)
	local amount = amount or 1
	local subType = subType or 0
	local item = 0
	if ItemType(itemid):isStackable() then
		if inBackpacks then
			stuff = Game.createItem(backpack, 1)
			item = stuff:addItem(itemid, math.min(100, amount))
		else
			stuff = Game.createItem(itemid, math.min(100, amount))
		end
		return Player(cid):addItemEx(stuff, ignoreCap) ~= RETURNVALUE_NOERROR and 0 or amount, 0
	end

	local a = 0
	if inBackpacks then
		local container, b = Game.createItem(backpack, 1), 1
		for i = 1, amount do
			local item = container:addItem(itemid, subType)
			if table.contains({(ItemType(backpack):getCapacity() * b), amount}, i) then
				if Player(cid):addItemEx(container, ignoreCap) ~= RETURNVALUE_NOERROR then
					b = b - 1
					break
				end

				a = i
				if amount > i then
					container = Game.createItem(backpack, 1)
					b = b + 1
				end
			end
		end
		return a, b
	end

	for i = 1, amount do -- normal method for non-stackable items
		local item = Game.createItem(itemid, subType)
		if Player(cid):addItemEx(item, ignoreCap) ~= RETURNVALUE_NOERROR then
			break
		end
		a = i
	end
	return a, 0
end

local func = function(cid, text, type, e, pcid)
	if Player(pcid):isPlayer() then
		local creature = Creature(cid)
		creature:say(text, type, false, pcid, creature:getPosition())
		e.done = true
	end
end

function doCreatureSayWithDelay(cid, text, type, delay, e, pcid)
	if Player(pcid):isPlayer() then
		e.done = false
		e.event = addEvent(func, delay < 1 and 1000 or delay, cid, text, type, e, pcid)
	end
end

function doPlayerSellItem(cid, itemid, count, cost)
	local player = Player(cid)
	if player:removeItem(itemid, count) then
		if not player:addMoney(cost) then
			error('Could not add money to ' .. player:getName() .. '(' .. cost .. 'gp)')
		end
		return true
	end
	return false
end

function doPlayerBuyItemContainer(cid, containerid, itemid, count, cost, charges)
	local player = Player(cid)
	if not player:removeTotalMoney(cost) then
		return false
	end

	for i = 1, count do
		local container = Game.createItem(containerid, 1)
		for x = 1, ItemType(containerid):getCapacity() do
			container:addItem(itemid, charges)
		end

		if player:addItemEx(container, true) ~= RETURNVALUE_NOERROR then
			return false
		end
	end
	return true
end

function getCount(string)
	local b, e = string:find("%d+")
	local tonumber = tonumber(string:sub(b, e))
	if tonumber > 2 ^ 32 - 1 then
		print("Warning: Casting value to 32bit to prevent crash\n"..debug.traceback())
	end
	return b and e and math.min(2 ^ 32 - 1, tonumber) or -1
end

function Player.getTotalMoney(self)
	return self:getMoney() + self:getBankBalance()
end

function isValidMoney(money)
	return isNumber(money) and money > 0
end

function getMoneyCount(string)
	local b, e = string:find("%d+")
	local tonumber = tonumber(string:sub(b, e))
	if tonumber > 2 ^ 32 - 1 then
		print("Warning: Casting value to 32bit to prevent crash\n"..debug.traceback())
	end
	local money = b and e and math.min(2 ^ 32 - 1, tonumber) or -1
	if isValidMoney(money) then
		return money
	end
	return -1
end

function getMoneyWeight(money)
	local weight, currencyItems = 0, Game.getCurrencyItems()
	for index = #currencyItems, 1, -1 do
		local currency = currencyItems[index]
		local worth = currency:getWorth()
		local currencyCoins = math.floor(money / worth)
		if currencyCoins > 0 then
			money = money - (currencyCoins * worth)
			weight = weight + currency:getWeight(currencyCoins)
		end
	end
	return weight
end
