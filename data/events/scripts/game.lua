function onLuaApiResponse(recvbyte, name, data)
	-- saving server
	if recvbyte == 102 then
		saveServer()
	end

	-- cleaning server map
	if recvbyte == 103 then
		cleanMap()
	end

	-- close server
	if recvbyte == 104 then
		Game.setGameState(GAME_STATE_SHUTDOWN)
	end

	-- start raid
	if recvbyte == 105 then
		local returnValue = Game.startRaid(data)
		if returnValue ~= RETURNVALUE_NOERROR then
			print("Raid: ".. Game.getReturnMessage(returnValue))
		else
			print("Raid: ".. data .." started.")
		end
	end

	-- reloading scripting systems
	if recvbyte >= 200 and recvbyte <= 218 then
		if table.contains({200, 215}, recvbyte) then
			EventCallback:clear()
		end
		Game.reload(recvbyte-200)
	end
end
