local event = Event()

event.onPlayerLogin = function(self)
	print(self:getName() .. " has logged in.")
	return true
end

event:register()
