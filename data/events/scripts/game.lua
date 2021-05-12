function onLuaApiResponse(recvbyte, data)
	print("Lua API recieved recvbyte: ".. recvbyte)
	print(">> ".. data)
	if recvbyte == 102 then
		EventCallbackData = {} 
		for i = 1, EVENT_CALLBACK_LAST do 
			EventCallbackData[i] = {} 
		end
		Game.reload(RELOAD_TYPE_SCRIPTS)
	end
end
