do
	-- World Light
	-- NOTE: if defaultWorldLight is set to true the world light algorithm will
	-- be handled in the sources. set it to false to avoid conflicts if you wish
	-- to make use of the function Game.setWorldLight(level, color)
	defaultWorldLight = true

	local worldLightLevel = 0
	local worldLightColor = 0

--- Gets the current world light color and level
---@return integer, integer
	function Game.getWorldLight() return worldLightLevel, worldLightColor end

--- Sets the world light to a specific color and level
---@param color any
---@param level any
	function Game.setWorldLight(color, level)
		if not defaultWorldLight then
			return
		end

		local previousColor = worldLightColor
		local previousLevel = worldLightLevel
		worldLightColor = color
		worldLightLevel = level

		if worldLightColor ~= previousColor or worldLightLevel ~= previousLevel then
			for _, player in ipairs(Game.getPlayers()) do
				player:sendWorldLight(worldLightColor, worldLightLevel)
			end
		end
	end
end
