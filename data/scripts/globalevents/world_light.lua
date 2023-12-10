local event = GlobalEvent("WorldLight")

local lightConfig = {
	day = 250,
	night = 40
}

local worldConfig = {
	sunrise = 360,
	dayTime = 480,
	sunset = 1080,
	nightTime = 1200
}

local lightChange = {
	sunrise = math.floor((lightConfig.day - lightConfig.night) / (worldConfig.dayTime - worldConfig.sunrise) * 100) / 100,
	sunset = math.floor((lightConfig.day - lightConfig.night) / (worldConfig.nightTime - worldConfig.sunset) * 100) / 100
}

do
	-- load default values
	local defaultColor = 215
	local defaultLevel = lightConfig.day
	Game.setWorldLight(defaultColor, defaultLevel)
end

local function calculateWorldLightLevel()
	local worldTime = Game.getWorldTime()
	if worldTime >= worldConfig.sunrise and worldTime <= worldConfig.dayTime then
		return ((worldConfig.dayTime - worldConfig.sunrise) - (worldConfig.dayTime - worldTime)) * lightChange.sunrise + lightConfig.night
	elseif worldTime >= worldConfig.sunset and worldTime <= worldConfig.nightTime then
		return lightConfig.day - ((worldTime - worldConfig.sunset) * lightChange.sunset)
	elseif worldTime >= worldConfig.nightTime or worldTime < worldConfig.sunrise then
		return lightConfig.night
	else
		return lightConfig.day
	end
end

function event.onTime(interval)
	if not configManager.getBoolean(configKeys.DEFAULT_WORLD_LIGHT) then
		return true
	end

	local worldLightColor, worldLightLevel = Game.getWorldLight()

	local lightLevel = calculateWorldLightLevel()
	if lightLevel ~= worldLightLevel then
		Game.setWorldLight(worldLightColor, lightLevel)
	end
	return true
end

event:interval(10000) -- 10 seconds
event:register()
