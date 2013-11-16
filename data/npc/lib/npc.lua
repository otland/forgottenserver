-- Including the Advanced NPC System
dofile('data/npc/lib/npcsystem/npcsystem.lua')

isPlayerPremiumCallback = isPremium

function msgcontains(message, keyword)
	local message, keyword = message:lower(), keyword:lower()
	if message == keyword then
		return true
	end

	return message:find(keyword) and not message:find('(%w+)' .. keyword)
end

function doNpcSellItem(cid, itemid, amount, subType, ignoreCap, inBackpacks, backpack)
	local amount, subType, ignoreCap, item = amount or 1, subType or 0, ignoreCap and TRUE or FALSE, 0
	ignoreCap = FALSE
	if isItemStackable(itemid) then
		if(inBackpacks) then
			stuff = doCreateItemEx(backpack, 1)
			item = doAddContainerItem(stuff, itemid, math.min(100, amount))
		else
			stuff = doCreateItemEx(itemid, math.min(100, amount))
		end
		return doPlayerAddItemEx(cid, stuff, ignoreCap) ~= RETURNVALUE_NOERROR and 0 or amount, 0
	end

	local a = 0
	if(inBackpacks) then
		local container, b = doCreateItemEx(backpack, 1), 1
		for i = 1, amount do
			local item = doAddContainerItem(container, itemid, subType)
			if(isInArray({(getContainerCapById(backpack) * b), amount}, i) == TRUE) then
				if(doPlayerAddItemEx(cid, container, ignoreCap) ~= RETURNVALUE_NOERROR) then
					b = b - 1 --
					break
				end
				a = i -- a = a + i
				if(amount > i) then
					container = doCreateItemEx(backpack, 1)
					b = b + 1
				end
			end
		end
		return a, b
	end

	for i = 1, amount do -- normal method for non-stackable items
		local item = doCreateItemEx(itemid, subType)
		if(doPlayerAddItemEx(cid, item, ignoreCap) ~= RETURNVALUE_NOERROR) then
			break
		end
		a = i
	end
	return a, 0
end

local func = function(pars)
	if isPlayer(pars.pcid) == TRUE then
		doCreatureSay(pars.cid, pars.text, pars.type, false, pars.pcid, getCreaturePosition(pars.cid))
		pars.e.done = TRUE
	end
end

function doCreatureSayWithDelay(cid, text, type, delay, e, pcid)
	if isPlayer(pcid) == TRUE then
		e.done = FALSE
		e.event = addEvent(func, delay < 1 and 1000 or delay, {cid=cid, text=text, type=type, e=e, pcid=pcid})
	end
end

function doPlayerTakeItem(cid, itemid, count)
	if getPlayerItemCount(cid,itemid) < count then
		return LUA_ERROR
	end

	while count > 0 do
		local tempcount = 0
		if isItemStackable(itemid) then
			tempcount = math.min (100, count)
		else
			tempcount = 1
		end

		local ret = doPlayerRemoveItem(cid, itemid, tempcount)
		if ret ~= LUA_ERROR then
			count = count - tempcount
		else
			return LUA_ERROR
		end
	end

	if count ~= 0 then
		return LUA_ERROR
	end
	return LUA_NO_ERROR
end

function doPlayerSellItem(cid, itemid, count, cost)
	if doPlayerTakeItem(cid, itemid, count) == LUA_NO_ERROR then
		if not doPlayerAddMoney(cid, cost) then
			error('Could not add money to ' .. getPlayerName(cid) .. '(' .. cost .. 'gp)')
		end
		return LUA_NO_ERROR
	end
	return LUA_ERROR
end

function doPlayerBuyItemContainer(cid, containerid, itemid, count, cost, charges)
	if not doPlayerRemoveMoney(cid, cost) then
		return LUA_ERROR
	end

	for i = 1, count do
		local container = doCreateItemEx(containerid, 1)
		for x = 1, getContainerCapById(containerid) do
			doAddContainerItem(container, itemid, charges)
		end

		if doPlayerAddItemEx(cid, container, true) ~= RETURNVALUE_NOERROR then
			return LUA_ERROR
		end
	end
	return LUA_NO_ERROR
end

function getCount(string)
	local b, e = string:find("%d+")
	return b and e and tonumber(string:sub(b, e)) or -1
end
