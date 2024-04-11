local callbacks = {
	[RESOURCE_BANK_BALANCE] = Player.getBankBalance,
	[RESOURCE_GOLD_EQUIPPED] = Player.getMoney,
}

local handler = PacketHandler(0xED)

function handler.onReceive(player, msg)
	local resource = msg:getByte()
	local callback = callbacks[resource]
	if not callback then
		-- print("[Warning - network/request_resource_balance.lua] Unknown resource type " .. string.format("0x%02X", resource))
		return
	end

	player:sendResourceBalance(resource, callback(player))
end

handler:register()
