local handler = PacketHandler(0xCF)

function handler.onReceive(player)
	local blessings = Game.getBlessings()

	local msg = NetworkMessage()

	msg:addByte(0x9B)
	msg:addByte(SERVER_BLESSINGS_COUNT)

	for i = 1, SERVER_BLESSINGS_COUNT do
		msg:addU16(blessings[i])
		msg:addByte(player:hasBlessing(i) and 1 or 0)
		msg:addByte(0) -- store bless
	end

	local premiumReduction = 0
	if player:isPremium() and player:isPromoted() then
		premiumReduction = 30
	end

	msg:addByte(2) -- premium (only work with premium days)
	msg:addByte(premiumReduction) -- exp loss lower

	local lossPercents = player:getLossPercent()
	local expLost = premiumReduction + lossPercents.skills
	msg:addByte(expLost) -- exp skill loss min pvp death
	msg:addByte(expLost) -- exp skill loss max pvp death
	msg:addByte(expLost) -- exp skill pve death
	msg:addByte(lossPercents.container) -- equip container lose pvp death
	msg:addByte(lossPercents.container) -- equip container pve death

	msg:addByte(0) -- ??
	msg:addByte(0) -- ??

	-- History
	local historyAmount = 1
	msg:addByte(historyAmount)

	for i = 1, historyAmount do
		msg:addU32(os.time())
		msg:addByte(0) -- color message (0 - red | 1 = white)
		msg:addString("Blessing Purchased")
	end

	msg:sendToPlayer(player)
	msg:delete()
end

handler:register()
