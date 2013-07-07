-- Including the Advanced NPC System
dofile('data/npc/lib/npcsystem/npcsystem.lua')

function getDistanceToCreature(id)
	if id == 0 or id == nil then
		selfGotoIdle()
	end

	local creaturePosition = getCreaturePosition(id)
	cx = creaturePosition.x
	cy = creaturePosition.y
	cz = creaturePosition.z
	if cx == nil then
		return nil
	end

	sx, sy, sz = selfGetPosition()
	return math.max(math.abs(sx - cx), math.abs(sy - cy))
end

function moveToPosition(x,y,z)
	selfMoveTo(x, y, z)
end

function moveToCreature(id)
	if id == 0 or id == nil then
		selfGotoIdle()
	end

	tx, ty, tz = getCreaturePosition(id)
	if tx == nil then
		selfGotoIdle()
	else
		moveToPosition(tx, ty, tz)
	end
end

function selfGotoIdle()
	following = false
	attacking = false
	selfAttackCreature(0)
	target = 0
end

function isPlayerPremiumCallback(cid)
	return isPremium(cid)
end

function msgcontains(message, keyword)
	local message, keyword = message:lower(), keyword:lower()
	if message == keyword then
		return true
	end

	return message:find(keyword) and not message:find('(%w+)' .. keyword)
end

function selfSayChannel(cid, message)
	return selfSay(message, cid, FALSE)
end

function doPosRemoveItem(_itemid, n, position)
	local thing = getThingfromPos({x = position.x, y = position.y, z = position.z, stackpos = 1})
	if thing.itemid == _itemid then
		doRemoveItem(thing.uid, n)
	else
		return false
	end
	return true
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
