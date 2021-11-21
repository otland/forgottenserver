function NetworkMessage:addOutfit(outfit, addMount)
	self:addU16(outfit.lookType)
	if outfit.lookType == 0 then
		local itemType = ItemType(outfit.lookTypeEx)
		self:addU16(itemType and itemType:getClientId() or 0)
	else
		self:addByte(outfit.lookHead)
		self:addByte(outfit.lookBody)
		self:addByte(outfit.lookLegs)
		self:addByte(outfit.lookFeet)
		self:addByte(outfit.lookAddons)
	end

	if addMount then
		self:addU16(outfit.lookMount)
		if outfit.lookMount ~= 0 then
			self:addByte(outfit.lookMountHead or 0)
			self:addByte(outfit.lookMountBody or 0)
			self:addByte(outfit.lookMountLegs or 0)
			self:addByte(outfit.lookMountFeet or 0)
		end
	end
end

COMPENDIUM_PLAYER_BASEINFORMATION = 0
COMPENDIUM_PLAYER_GENERAL = 1
COMPENDIUM_PLAYER_COMBAT = 2
COMPENDIUM_PLAYER_DEATHS = 3
COMPENDIUM_PLAYER_PVPKILLS = 4
COMPENDIUM_PLAYER_ACHIEVEMENTS = 5
COMPENDIUM_PLAYER_INVENTORY = 6
COMPENDIUM_PLAYER_COSMETICS = 7
COMPENDIUM_PLAYER_STORE = 8
COMPENDIUM_PLAYER_INSPECTION = 9
COMPENDIUM_PLAYER_BADGES = 10
COMPENDIUM_PLAYER_TITLES = 11

COMPENDIUM_KILLTYPE_JUSTIFIED = 0
COMPENDIUM_KILLTYPE_UNJUSTIFIED = 1
COMPENDIUM_KILLTYPE_GUILDWAR = 2
COMPENDIUM_KILLTYPE_ASSISTED = 3
COMPENDIUM_KILLTYPE_ARENA = 4

COMPENDIUMERROR_PLAYER_NODATA = 1
COMPENDIUMERROR_PLAYER_CHARDATA_DENIED = 2
COMPENDIUMERROR_PLAYER_INSPECTION_DENIED = 3

function sendCompendiumError(player, infoType, errorCode)
	local msg = NetworkMessage();
	msg:addByte(0xDA)
	msg:addByte(infoType)
	msg:addByte(errorCode)
	msg:sendToPlayer(player)
end

function sendCompendiumPlayerInfo(player, creatureId, infoType, entriesPerPage, page)
	-- to do: implement permission logic for inspecting other creatures
	if player:getId() ~= creatureId then
		sendCompendiumError(player, infoType, COMPENDIUMERROR_PLAYER_CHARDATA_DENIED)
		return
	end

	local creature = Creature(creatureId)
	if not creature or creature:isRemoved() then
		sendCompendiumError(player, infoType, COMPENDIUMERROR_PLAYER_NODATA)
		return
	end

	local creatureType = "unknown"
	if creature:isPlayer() then
		if creature:getGroup():getAccess() then
			creatureType = creature:getGroup():getName()
		else
			creatureType = creature:getVocation():getName()
		end
	elseif creature:isMonster() then
		creatureType = "monster"
	elseif creature:isNpc() then
		creatureType = "Non-Player Character"	
	end
	
	local response = NetworkMessage()
	if infoType == COMPENDIUM_PLAYER_BASEINFORMATION then
		response:addByte(0xDA)
		response:addByte(COMPENDIUM_PLAYER_BASEINFORMATION)
		response:addByte(0x00)
		response:addString(creature:getName())
		response:addString(creatureType:gsub("^%l", string.upper))
		response:addU16(creature:isPlayer() and creature:getLevel() or 1)
		response:addOutfit(creature:getOutfit(), false)
		response:addByte(0x01) -- stamina?
		response:addByte(0x01) -- enable store summary & character titles?
		response:addString("") -- character title
		response:sendToPlayer(player)
		return
	elseif infoType == COMPENDIUM_PLAYER_GENERAL then
		return
	elseif infoType == COMPENDIUM_PLAYER_COMBAT then
		return
	elseif infoType == COMPENDIUM_PLAYER_DEATHS then
		return
	elseif infoType == COMPENDIUM_PLAYER_PVPKILLS then
		return
	elseif infoType == COMPENDIUM_PLAYER_ACHIEVEMENTS then
		return
	elseif infoType == COMPENDIUM_PLAYER_INVENTORY then
		return
	elseif infoType == COMPENDIUM_PLAYER_COSMETICS then
		return
	elseif infoType == COMPENDIUM_PLAYER_STORE then
		return
	elseif infoType == COMPENDIUM_PLAYER_INSPECTION then
		return
	elseif infoType == COMPENDIUM_PLAYER_BADGES then
		return
	elseif infoType == COMPENDIUM_PLAYER_TITLES then
		return
	end

	sendCompendiumError(player, infoType, COMPENDIUMERROR_PLAYER_NODATA)
end

local callback = function(player, recvbyte, networkMessage)
	local playerId = networkMessage:getU32()
	if playerId == 0 then
		playerId = player:getId()
	end
	
	local infoType = networkMessage:getByte()
	local entriesPerPage = 5
	local currentPage = 1
	
	if (infoType == COMPENDIUM_PLAYER_DEATHS or infoType == COMPENDIUM_PLAYER_PVPKILLS) then
		entriesPerPage = math.min(30, math.max(5, networkMessage:getU16()))
		currentPage = math.max(1, networkMessage:getU16());
	end
	
	sendCompendiumPlayerInfo(player, playerId, infoType, entriesPerPage, currentPage)
	return true
end

setPacketEvent(0xE5, callback)