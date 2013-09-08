local function isPlayerInArea(fromPos, toPos)
	for _x = fromPos.x, toPos.x do
		for _y = fromPos.y, toPos.y do
			for _z = fromPos.z, toPos.z do
				creature = getTopCreature({x = _x, y = _y, z = _z})
					if (isPlayer(creature.uid)) then
					return true
				end
			end
		end
	end
	return false
end

local function doRemoveMonsterFromArea(fromPos, toPos)
	for _x = fromPos.x, toPos.x do
		for _y = fromPos.y, toPos.y do
			for _z = fromPos.z, toPos.z do
				creature = getTopCreature({x = _x, y = _y, z = _z})
				if (isMonster(creature.uid)) then
					doRemoveCreature(creature.uid)
				end
			end
		end
	end
	return false
end

local Area_fromPos = {x = 33219, y = 31657, z = 13} --top left of the room
local Area_toPos = {x = 33224, y = 31661, z = 13} --bottom right of the room

local players_pos =  {
	{x = 33225, y = 31671, z = 13, stackpos = STACKPOS_TOP_CREATURE},
	{x = 33224, y = 31671, z = 13, stackpos = STACKPOS_TOP_CREATURE},
	{x = 33223, y = 31671, z = 13, stackpos = STACKPOS_TOP_CREATURE},
	{x = 33222, y = 31671, z = 13, stackpos = STACKPOS_TOP_CREATURE}
}

local new_player_pos = {
	{x = 33222, y = 31659, z = 13},
	{x = 32221, y = 31659, z = 13},
	{x = 32220, y = 31659, z = 13},
	{x = 32219, y = 31659, z = 13}
}

local demonPos = {
	{x = 33219, y = 31657, z = 13},
	{x = 33221, y = 31657, z = 13},
	{x = 33223, y = 31659, z = 13},
	{x = 33224, y = 31659, z = 13},
	{x = 33220, y = 31661, z = 13},
	{x = 33222, y = 31661, z = 13}
}

local config = {
	min_level = 100, --min player level to make the quest
	min_players = 4, --min players needed to make the quest
	once_per_restart = true --global like real tibia, once per "day"/"restart"
}

function onUse(cid, item, fromPosition, itemEx, toPosition)
	local player = {}
	summon_demons = true
	if (item.itemid == 1946) then
		if (config.once_per_restart) then
			if (getGlobalStorageValue(10000) == 1) then
				return (doPlayerSendCancel(cid, "Someone has already made the quest today."))
			end
		end
		for i = 1,4 do
			player[i] = getTopCreature(players_pos).uid
			if (isPlayer(player[i])) then
				if (getThingfromPos(player[i]).itemid >= config.min_players) then
					if(getPlayerLevel(player[i]) >= config.min_level) then
						if (not isPlayerInArea(Area_fromPos, Area_toPos)) then
							doTeleportThing(player[i], new_player_pos)
							doSendMagicEffect(new_player_pos, CONST_ME_TELEPORT)
							doSendMagicEffect(players_pos, CONST_ME_POFF)
							doSetCreatureDirection(player[i], EAST)
						if (config.once_per_restart) then
							setGlobalStorageValue(10000, 1)
						end
					else
						return (doPlayerSendCancel(cid, "A team is already inside the quest room."))
					end
				else
					return (doPlayerSendCancel(cid, "All players must be above level "..config.min_level.."."))
				end
			else
				return (doPlayerSendCancel(cid, "You need "..config.min_players.." players."))
			end
		end
	end
	if(summon_demons) then
		doRemoveMonsterFromArea(Area_fromPos, Area_toPos)
		for d = 1, 6 do
			doSummonCreature("Demon", demonPos[d])
		end
	end
	summon_demons = false
	doTransformItem(item.uid, 1945)
	elseif (item.itemid == 1945) then
		doTransformItem(item.uid, 1946)
	end
	return true
end
