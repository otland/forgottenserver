function onLuaApiResponse(recvbyte, name, data)
	-- saving server
	if recvbyte == 102 then
		saveServer()
		return
	end

	-- cleaning server map
	if recvbyte == 103 then
		cleanMap()
		return
	end

	-- close server
	if recvbyte == 104 then
		Game.setGameState(GAME_STATE_SHUTDOWN)
		return
	end

	-- start raid
	if recvbyte == 105 then
		local returnValue = Game.startRaid(data)
		if returnValue ~= RETURNVALUE_NOERROR then
			print("Raid: ".. Game.getReturnMessage(returnValue))
		else
			print("Raid: ".. data .." started.")
		end
		return
	end

	-- reloading scripts
	if recvbyte == 200 then
		EventCallback:clear()
		Game.reload(RELOAD_TYPE_SCRIPTS)
		print("reloaded scripts successfuly")
		return
	end

	print("[lua protocol api] unhandled packet recieved!")
	print(">> recvbyte: ".. recvbyte)
	print(">> name: ".. name)
	print(">> data: ".. data)
end
