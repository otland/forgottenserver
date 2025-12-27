local event = Event()

event.onPlayerLogout = function(self)
	Game.getTrackedQuests()[self:getId()] = nil
	return true
end

event:register()
