local handler = PacketHandler(0x1D)

function handler.onReceive(player)
    local msg = NetworkMessage()
    msg:addByte(0x1E)
    msg:sendToPlayer(player)
    msg:delete()
end

handler:register()
