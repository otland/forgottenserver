local handler = PacketHandler(0x1E)

function handler.onReceive(player)
    player:setLastPong(os.mtime())
end

handler:register()
