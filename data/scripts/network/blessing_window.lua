local handler = PacketHandler(0xCF)
local totalBlessings = 5

function handler.onReceive(player)
	local msg = NetworkMessage()
	msg:addByte(0x9B)

	msg:addByte(totalBlessings)

	for bless = 1, totalBlessings do
		msg:addU16(2 ^ bless)
		if player:hasBlessing(bless) then
			msg:addByte(1)
		else
			msg:addByte(0)
		end

		msg:addByte(0) -- store bless
	end

	local premiumReduction = 0
	if player:isPremium() and player:isPromoted() then
		premiumReduction = 30
	end

	msg:addByte(2) -- premium (only work with premium days)
	msg:addByte(premiumReduction) -- exp loss lower

	local lossPercent = player:getLossPercent()
	local expLost = premiumReduction + lossPercent.skills
	msg:addByte(expLost) -- exp skill loss min pvp death
	msg:addByte(expLost) -- exp skill loss max pvp death
	msg:addByte(expLost) -- exp skill pve death
	msg:addByte(lossPercent.container) -- equip container lose pvp death
	msg:addByte(lossPercent.container) -- equip container pve death

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
