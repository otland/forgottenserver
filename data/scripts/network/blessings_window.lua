local ec = EventCallback

ec.onUpdateStorage = function(player, key, value, oldValue, isLogin)
	if isLogin then
		return
	end

	local storageBlessing = 10003001
	if key >= (storageBlessing + BLESSING_FIRST) and key <= (storageBlessing + BLESSING_LAST) then
		player:sendBlessings()
	end
end

ec:register()


local function sendBlessingsWindow(player)
	local msg = NetworkMessage()
	msg:addByte(0x9B)

	msg:addByte(BLESSING_LAST + 1)
	for blessingId = BLESSING_FIRST, BLESSING_LAST, 1 do
		msg:addU16(2 ^ blessingId)
		msg:addByte(player:getBlessing(blessingId))
		msg:addByte(0) -- charges bought from store
	end

	local isPromoted = player:isPromoted()
	msg:addByte(isPromoted and 0x01 or 0x00)
	msg:addByte(30) -- fixed value

	local percentReduction = 0
	if isPromoted then
		percentReduction = percentReduction + 30
	end

	local blessings = player:getBlessings(true)
	percentReduction = percentReduction + (#blessings * 8)

	msg:addByte(percentReduction) -- exp/skill min loss pvp death
	msg:addByte(percentReduction + 9) -- exp/skill max loss pvp death
	msg:addByte(percentReduction) -- exp/skill loss pve death

	local equipmentLoss = {100, 70, 45, 25, 10, 0, 0, 0}
	local equipmentPercent = equipmentLoss[#blessings + 1]

	local isBlackOrRedSkull = (player:getSkull() == SKULL_RED or player:getSkull() == SKULL_BLACK)

	if isBlackOrRedSkull then
		equipmentPercent = 100
	end

	msg:addByte(equipmentPercent) -- loss container death pvp & 10% for pve (100pvp => 10pve)
	msg:addByte(equipmentPercent) -- loss equipment death pvp & 10% for pve (100pvp => 10pve)

	msg:addByte(isBlackOrRedSkull and 0x01 or 0x00)

	local amulet = player:getSlotItem(CONST_SLOT_NECKLACE)
	if amulet and amulet:getId() == ITEM_AMULETOFLOSS then
		msg:addByte(0x01)
	else
		msg:addByte(0x00)
	end

	local history = player:getBlessingsHistory()
	msg:addByte(#history)
	for _, entry in ipairs(history) do
		msg:addU32(entry.ts)
		msg:addByte(entry.type)
		msg:addString(entry.event)
	end

	msg:sendToPlayer(player)
	msg:delete()

	return true
end

local handler = PacketHandler(0xCF)

function handler.onReceive(player, msg)
	sendBlessingsWindow(player)
end

handler:register()
