local wildGrowth = {1499, 11099} -- wild growth destroyable by machete
local jungleGrass = { -- grass destroyable by machete
	[2782] = 2781,
	[3985] = 3984,
	[19433] = 19431
}
local groundIds = {354, 355} -- pick usable ground
local sandIds = {231, 9059} -- desert sand
local holeId = { -- usable rope holes, for rope spots see global.lua
	294, 369, 370, 383, 392, 408, 409, 410, 427, 428, 429, 430, 462, 469, 470, 482,
	484, 485, 489, 924, 1369, 3135, 3136, 4835, 4837, 7933, 7938, 8170, 8249, 8250,
	8251, 8252, 8254, 8255, 8256, 8276, 8277, 8279, 8281, 8284, 8285, 8286, 8323,
	8567, 8585, 8595, 8596, 8972, 9606, 9625, 13190, 14461, 19519, 21536, 23713,
	26020
}
local holes = {468, 481, 483, 7932, 23712} -- holes opened by shovel
local fruits = {2673, 2674, 2675, 2676, 2677, 2678, 2679, 2680, 2681, 2682, 2684, 2685, 5097, 8839, 8840, 8841} -- fruits to make decorated cake with knife

function destroyItem(player, target, toPosition)
	if type(target) ~= "userdata" or not target:isItem() then
		return false
	end

	if target:hasAttribute(ITEM_ATTRIBUTE_UNIQUEID) or target:hasAttribute(ITEM_ATTRIBUTE_ACTIONID) then
		return false
	end

	if toPosition.x == CONTAINER_POSITION then
		player:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		return true
	end

	local destroyId = ItemType(target.itemid):getDestroyId()
	if destroyId == 0 then
		return false
	end

	if math.random(7) == 1 then
		local item = Game.createItem(destroyId, 1, toPosition)
		if item then
			item:decay()
		end

		-- Move items outside the container
		if target:isContainer() then
			for i = target:getSize() - 1, 0, -1 do
				local containerItem = target:getItem(i)
				if containerItem then
					containerItem:moveTo(toPosition)
				end
			end
		end

		target:remove(1)
	end

	toPosition:sendMagicEffect(CONST_ME_POFF)
	return true
end

function onUseMachete(player, item, fromPosition, target, toPosition, isHotkey)
	local targetId = target.itemid
	if not targetId then
		return true
	end

	if table.contains(wildGrowth, targetId) then
		toPosition:sendMagicEffect(CONST_ME_POFF)
		target:remove()
		return true
	end

	local grass = jungleGrass[targetId]
	if grass then
		target:transform(grass)
		target:decay()
		return true
	end

	return destroyItem(player, target, toPosition)
end

function onUsePick(player, item, fromPosition, target, toPosition, isHotkey)
	if target.itemid == 11227 then -- shiny stone refining
		local chance = math.random(1, 100)
		if chance == 1 then
			player:addItem(ITEM_CRYSTAL_COIN) -- 1% chance of getting crystal coin
		elseif chance <= 6 then
			player:addItem(ITEM_GOLD_COIN) -- 5% chance of getting gold coin
		elseif chance <= 51 then
			player:addItem(ITEM_PLATINUM_COIN) -- 45% chance of getting platinum coin
		else
			player:addItem(2145) -- 49% chance of getting small diamond
		end
		target:getPosition():sendMagicEffect(CONST_ME_BLOCKHIT)
		target:remove(1)
		return true
	end

	local tile = Tile(toPosition)
	if not tile then
		return false
	end

	local ground = tile:getGround()
	if not ground then
		return false
	end

	if table.contains(groundIds, ground.itemid) and (ground:hasAttribute(ITEM_ATTRIBUTE_UNIQUEID) or ground:hasAttribute(ITEM_ATTRIBUTE_ACTIONID)) then
		ground:transform(392)
		ground:decay()

		toPosition.z = toPosition.z + 1
		tile:relocateTo(toPosition)
	end

	return true
end

function onUseRope(player, item, fromPosition, target, toPosition, isHotkey)
	local tile = Tile(toPosition)
	if not tile then
		return false
	end

	if table.contains(ropeSpots, tile:getGround():getId()) or tile:getItemById(14435) then
		if Tile(toPosition:moveUpstairs()):hasFlag(TILESTATE_PROTECTIONZONE) and player:isPzLocked() then
			player:sendCancelMessage(RETURNVALUE_PLAYERISPZLOCKED)
			return true
		end
		player:teleportTo(toPosition, false)
		return true
	elseif table.contains(holeId, target.itemid) then
		toPosition.z = toPosition.z + 1
		tile = Tile(toPosition)
		if tile then
			local thing = tile:getTopVisibleThing()
			if thing:isPlayer() then
				if Tile(toPosition:moveUpstairs()):hasFlag(TILESTATE_PROTECTIONZONE) and thing:isPzLocked() then
					return false
				end
				return thing:teleportTo(toPosition, false)
			end
			if thing:isItem() and thing:getType():isMovable() then
				return thing:moveTo(toPosition:moveUpstairs())
			end
		end
		player:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		return true
	end
	return false
end

function onUseShovel(player, item, fromPosition, target, toPosition, isHotkey)
	local tile = Tile(toPosition)
	if not tile then
		return false
	end

	local ground = tile:getGround()
	if not ground then
		return false
	end

	local groundId = ground:getId()
	if table.contains(holes, groundId) then
		ground:transform(groundId + 1)
		ground:decay()

		toPosition.z = toPosition.z + 1
		tile:relocateTo(toPosition)
	elseif table.contains(sandIds, groundId) then
		local randomValue = math.random(1, 100)
		if target.actionid == 100 and randomValue <= 20 then
			ground:transform(489)
			ground:decay()
		elseif randomValue == 1 then
			Game.createItem(2159, 1, toPosition)
		elseif randomValue > 95 then
			Game.createMonster("Scarab", toPosition)
		end
		toPosition:sendMagicEffect(CONST_ME_POFF)
	else
		return false
	end

	return true
end

function onUseScythe(player, item, fromPosition, target, toPosition, isHotkey)
	if not table.contains({2550, 10513}, item.itemid) then
		return false
	end

	if target.itemid == 2739 then -- wheat
		target:transform(2737)
		target:decay()
		Game.createItem(2694, 1, toPosition) -- bunch of wheat
		return true
	end
	if target.itemid == 5465 then -- burning sugar cane
		target:transform(5464)
		target:decay()
		Game.createItem(5467, 1, toPosition) -- bunch of sugar cane
		return true
	end
	return destroyItem(player, target, toPosition)
end

function onUseCrowbar(player, item, fromPosition, target, toPosition, isHotkey)
	if not table.contains({2416, 10515}, item.itemid) then
		return false
	end

	return destroyItem(player, target, toPosition)
end

function onUseKitchenKnife(player, item, fromPosition, target, toPosition, isHotkey)
	if not table.contains({2566, 10511, 10515}, item.itemid) then
		return false
	end

	if table.contains(fruits, target.itemid) and player:removeItem(6278, 1) then
		target:remove(1)
		player:addItem(6279, 1)
		player:getPosition():sendMagicEffect(CONST_ME_MAGIC_GREEN)
		return true
	end

	return false
end
