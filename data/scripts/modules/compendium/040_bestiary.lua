-- 0x2A add bestiary tracker list
-- 0xE1 request races (declared and linked)
-- 0xE2 request creatures
-- 0xE3 request monster

function Player:sendCharms()
	self:sendResourceBalance(RESOURCE_BANK_BALANCE, self:getBankBalance())
	self:sendResourceBalance(RESOURCE_GOLD_EQUIPPED, self:getMoney())
	
	local response = NetworkMessage()
	response:addByte(COMPENDIUM_RESPONSE_CHARMS)
	response:addU32(0) -- charm points

	local charmCount = 19
	response:addByte(charmCount) -- charm count

	-- charm block
	for i = 0, charmCount-1 do--18 do
	response:addByte(i) -- charmId
	response:addString("name")
	response:addString("description")
	response:addByte(2) -- charm level (0-2)
	response:addU16(10) -- cost in charm points
	response:addByte(1) -- is unlocked
	response:addByte(0) -- is monster assigned
	-- if yes:
	--response:addU16(10) -- raceId
	--response:addU32(10) -- remove cost
	end
	
	response:addByte(255) -- amount of charms you can assign (0-254), 255 - unlimited
	
	-- list of monsters you can assign your charms to
	response:addU16(2) -- raceCount
	response:addU16(10) -- raceId (shows on the list)
	response:addU16(11) -- raceId (shows on the list)
	response:sendToPlayer(self)
end

function Player:sendBestiary()
	local response = NetworkMessage()
	response:addByte(COMPENDIUM_RESPONSE_BESTIARY)
	response:addU16(MONSTERGROUP_LAST)
	for monsterGroupId = MONSTERGROUP_FIRST, MONSTERGROUP_LAST do
		response:addString(monsterGroupNames[monsterGroupId])
		response:addU16(monsterGroupId) -- total
		response:addU16(monsterGroupId) -- known
	end
	response:sendToPlayer(self)
end

-- open bestiary/charms
function onRequestBestiaryCharms(player, recvbyte, networkMessage)
	--player:sendBestiary()
	--player:sendCharms()
	return true
end

local callback = onRequestBestiaryCharms
setPacketEvent(COMPENDIUM_REQUEST_BESTIARY, callback)

-- click on monster category
local callback = function(player, recvbyte, networkMessage)
	local m = NetworkMessage()
	m:addByte(COMPENDIUM_RESPONSE_BESTIARY_SPECIES)
	m:addString("Amphibic") -- category name (unused)

	-- monsters block
	m:addU16(1) -- amount of monsters

	-- monster
	m:addU16(10) -- race
	m:addByte(1) -- progress level(0-4, if > 1, demands occurence byte)
	m:addByte(3) -- enum 3: occurence: very rare
	
	
	m:sendToPlayer(player)
	return true
end
setPacketEvent(COMPENDIUM_REQUEST_BESTIARY_SPECIES, callback)