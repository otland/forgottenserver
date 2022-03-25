PacketHandlers = {}

function PacketHandler(packetType)
	return {
		register = function(self)
			if isScriptsInterface() then
				local callback = rawget(self, 'onReceive')
				if not callback then
					debugPrint("[Warning - PacketHandler::register] need to setup a callback before you can register.")
					return
				end

				if type(callback) ~= "function" then
					debugPrint(string.format("[Warning - PacketHandler::onReceive] a function is expected."))
					return
				end

				PacketHandlers[packetType] = callback
			end
		end,

		clear = function(self)
			if PacketHandlers[packetType] == self.callback then
				PacketHandlers[packetType] = nil
			end
		end,
	}
end
