do
	local worldTime = 0

	function Game.getWorldTime() return worldTime end

	function Game.setWorldTime(time)
		worldTime = time

		-- quarter-hourly update to client clock near the minimap
		if worldTime % 15 == 0 then
			for _, player in ipairs(Game.getPlayers()) do
				player:sendWorldTime(worldTime)
			end
		end
	end

	function Game.getFormattedWorldTime()
		local worldTime = Game.getWorldTime()
		local hours = math.floor(worldTime / 60)

		local minutes = worldTime % 60
		if minutes < 10 then
			minutes = '0' .. minutes
		end

		minutes = math.floor(minutes)

		return hours .. ':' .. minutes
	end
end
