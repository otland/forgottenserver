local function getLootDifficulty(chance)
	for i = 0, 4 do
		if chance < 200 * 5 ^ i then
			return 4 - i
		end
	end
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
	local monsterKills = monsterType:getBestiaryKills()
	local step = kills == 0 and 0 or 4
	if kills ~= 0 then
		for i, amount in pairs({0, unpack(monsterKills)}) do
			step = kills >= amount and i or step
		end
	end

	local response = NetworkMessage()
	response:addByte(0xD7)
	response:addU16(raceId)
	response:addString(info.class)
	response:addByte(step)
	response:addU32(kills)

	for _, amount in pairs(monsterKills) do
		response:addU16(amount)
	end

	response:addByte(info.difficulty)
	response:addByte(info.occurrence)

	local loot = monsterType:getLoot()
	response:addByte(#loot)

	for _, lootItem in pairs(loot) do
		local lootDifficulty = getLootDifficulty(lootItem.chance)
		local knowLoot = lootDifficulty <= step
		local itemType = ItemType(lootItem.itemId)
		response:addU16(knowLoot and itemType:getClientId() or 0)
		response:addByte(lootDifficulty)
		response:addByte(0) -- 1 = event loot, 0 = normal loot
		if knowLoot then
			response:addString(itemType:getName())
			response:addByte(lootItem.maxCount > 1 and 1 or 0)
		end
	end

	if step > 1 then
		response:addU16(monsterType:getBestiaryCharmPoints())
		response:addByte(monsterType:isHostile() and 2 or 1)
		response:addByte(2)
		response:addU32(monsterType:getMaxHealth())
		response:addU32(monsterType:getExperience())
		response:addU16(monsterType:getBaseSpeed())
		response:addU16(monsterType:getArmor())
	end

	if step > 2 then
		local elements = getBestiaryElements(monsterType)
		response:addByte(#elements)
		for index, value in pairs(elements) do
			response:addByte(index - 1)
			response:addU16(value)
		end

		response:addU16(1)
		response:addString(info.locations)
	end

	if step > 3 then
		response:addByte(0)
		response:addByte(1)
	end

	response:sendToPlayer(player)
	response:delete()
end

handler:register()
