local handler = PacketHandler(0x1E)

function handler.onReceive(player)
    player:setLastPong(os.time())
end

handler:register()
