local event = Event()

event.onPlayerJoin = function(self)
	local worldTime = Game.getWorldTime()
	self:sendWorldTime(worldTime)

	local worldLightColor, worldLightLevel = Game.getWorldLight()
	self:sendWorldLight(worldLightColor, worldLightLevel)
end

event:register()
