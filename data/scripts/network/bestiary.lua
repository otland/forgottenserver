local handler = PacketHandler(0xE1)

function handler.onReceive(player)
	local bestiaryClasses = Game.getBestiary()
	local msg = NetworkMessage()
	msg:addByte(0xD5)
	msg:addU16(#bestiaryClasses)
	for _, class in pairs(bestiaryClasses) do
		msg:addString(class.name)
		msg:addU16(#class.monsterTypes)
		msg:addU16(player:getUnlockedBestiary(class.monsterTypes))
	end

	msg:sendToPlayer(player)
	msg:delete()
end

handler:register()

local function getProgressStatus(player, info)
	local killAmount = player:getBestiaryKills(info.raceId)
	if killAmount == 0 then
		return 0
	elseif killAmount < info.prowess then
		return 1
	elseif killAmount < info.expertise then
		return 2
	elseif killAmount < info.mastery then
		return 3
	end
	return 4
end

local handler = PacketHandler(0xE2)

function handler.onReceive(player, msg)
	local monsterTypes = {}
	local className = ""
	local search = msg:getByte() == 1
	if search then
		local amount = msg:getU16()
		for i = 1, amount do
			local raceId = msg:getU16()
			local monsterType = MonsterType(raceId)
			if monsterType and player:getBestiaryKills(raceId) > 0 then
				monsterTypes[#monsterTypes + 1] = monsterType
			end
		end
	else
		className = msg:getString()
		local bestiaryClasses = Game.getBestiary()
		for _, class in pairs(bestiaryClasses) do
			if class.name == className then
				monsterTypes = class.monsterTypes
				break
			end
		end
	end

	if #monsterTypes == 0 then
		return
	end

	local response = NetworkMessage()
	response:addByte(0xD6)
	response:addString(className)
	response:addU16(#monsterTypes)

	for _, monsterType in pairs(monsterTypes) do
		local info = monsterType:getBestiaryInfo()
		response:addU16(info.raceId)
		local progress = getProgressStatus(player, info)
		if progress > 0 then
			response:addU16(progress)
		else
			response:addByte(0)
		end
	end

	response:sendToPlayer(player)
	response:delete()
end

handler:register()

local function getDifficulty(chance)
	if chance < 200 then
		return 4
	elseif chance < 1000 then
		return 3
	elseif chance < 5000 then
		return 2
	elseif chance < 25000 then
		return 1
	end
	return 0
end

local bestiaryElements = {
	COMBAT_PHYSICALDAMAGE,
	COMBAT_FIREDAMAGE,
	COMBAT_EARTHDAMAGE,
	COMBAT_ENERGYDAMAGE,
	COMBAT_ICEDAMAGE,
	COMBAT_HOLYDAMAGE,
	COMBAT_DEATHDAMAGE,
	COMBAT_HEALING
}

local function getBestiaryElements(monsterType)
	local elements = monsterType:getElementList()
	local result = {}
	for _, combatType in pairs(bestiaryElements) do
		result[#result + 1] = 100 - (elements[combatType] or 0)
	end
	return result
end

local handler = PacketHandler(0xE3)

function handler.onReceive(player, msg)
	local raceId = msg:getU16()
	local monsterType = MonsterType(raceId)
	if not monsterType then
		return
	end

	local info = monsterType:getBestiaryInfo()
	local kills = player:getBestiaryKills(raceId)
	local progress = getProgressStatus(player, info)

	local response = NetworkMessage()
	response:addByte(0xD7)
	response:addU16(raceId)
	response:addString(info.class)
	response:addByte(progress)
	response:addU32(kills)

	response:addU16(info.prowess)
	response:addU16(info.expertise)
	response:addU16(info.mastery)

	response:addByte(info.stars)
	response:addByte(info.occurrence)

	local loot = monsterType:getLoot()
	response:addByte(#loot)

	for _, lootItem in pairs(loot) do
		local difficulty = getDifficulty(lootItem.chance)
		local knowLoot = difficulty <= progress
		local itemType = ItemType(lootItem.itemId)
		response:addU16(knowLoot and itemType:getClientId() or 0)
		response:addByte(difficulty)
		response:addByte(0) -- 1 = event loot, 0 = normal loot
		if knowLoot then
			response:addString(itemType:getName())
			response:addByte(lootItem.maxCount > 1 and 1 or 0)
		end
	end

	if progress > 1 then
		response:addU16(info.charmPoints)
		response:addByte(monsterType:isHostile() and 2 or 1)
		response:addByte(2)
		response:addU32(monsterType:getMaxHealth())
		response:addU32(monsterType:getExperience())
		response:addU16(monsterType:getBaseSpeed())
		response:addU16(monsterType:getArmor())
	end

	if progress > 2 then
		local elements = getBestiaryElements(monsterType)
		response:addByte(#elements)
		for index, value in pairs(elements) do
			response:addByte(index - 1)
			response:addU16(value)
		end

		response:addU16(1)
		response:addString(info.locations)
	end

	if progress > 3 then
		response:addByte(0)
		response:addByte(1)
	end

	response:sendToPlayer(player)
	response:delete()
end

handler:register()
