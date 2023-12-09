local handler = PacketHandler(CLIENT_GREET)

function handler.onReceive(player, msg)
	player:say("hi", TALKTYPE_SAY)
end

handler:register()
