local event = Event()

event.onPlayerLogout = function(self)
	print(self:getName() .. " has logged out.")
	return true
end

event:register()
