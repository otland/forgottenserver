local function getUnlockedBestiary(player, monsterTypes)
	local count = 0
	for _, monsterType in pairs(monsterTypes) do
		local info = monsterType:getBestiaryInfo()
		if player:getStorageValue(PlayerStorageKeys.bestiaryKillsBase + info.raceId) >= info.prowess then
			count = count + 1
		end
	end
	return count
end

local handler = PacketHandler(0xE1)

function handler.onReceive(player)
	local bestiaryClasses = Game.getBestiary()
	local msg = NetworkMessage()
	msg:addByte(0xD5)
	msg:addU16(#bestiaryClasses)
	for _, class in pairs(bestiaryClasses) do
		msg:addString(class.name)
		msg:addU16(#class.monsterTypes)
		msg:addU16(getUnlockedBestiary(player, class.monsterTypes))
	end

	msg:sendToPlayer(player)
	msg:delete()
end

handler:register()
