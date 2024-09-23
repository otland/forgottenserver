do
	local worldLightLevel = 0
	local worldLightColor = 0

	function Game.getWorldLight() return worldLightLevel, worldLightColor end

	function Game.setWorldLight(color, level)
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
