local function getUnlockedBestiary(player, monsterTypes)
	local unlocked = 0
	for _, mType in pairs(monsterTypes) do
		local amount = mType:getBestiaryKills()[1]
		if player:getStorageValue(PlayerStorageKeys.bestiaryKillsBase + mType:raceId()) >= amount then
			unlocked = unlocked + 1
		end
	end
	return unlocked
end

local handler = PacketHandler(0xE1)

function handler.onReceive(player)
	local bestiaryClasses = Game.getBestiaryClasses()
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
