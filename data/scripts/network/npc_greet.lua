local handler = PacketHandler(0xEE)

function handler.onReceive(player, msg)
	player:say("hi", TALKTYPE_SAY)
end

handler:register()
