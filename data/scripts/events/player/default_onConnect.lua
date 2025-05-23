local event = Event()

event.onConnect = function(self, msg)

	return msg
end

event:register()