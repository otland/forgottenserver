local raids = {}

Raid = setmetatable({
	getRaids = function() return raids end
}, {
	__call = function(self, name)
		local events = {}

		local obj = {
			margin = 0,
			name = name,
			repeats = false,
		}

		function obj:__newindex(key, value)
			if key == "interval" or key == "margin" or key == "repeats" then
				rawset(self, key, value)
			else
				io.write("[Warning] Invalid attribute for raid: " .. key .. ". Ignoring...\n")
			end
		end

		function obj:addEvent(delay, fn)
			events[#events] = { delay = delay, fn = fn }
		end

		function obj:execute()
			for _, event in ipairs(events) do
				addEvent(event.delay, event.fn)
			end
		end

		function obj:register()
			raids[self.name] = self
		end

		function obj:unregister()
			raids[self.name] = nil
		end

		return obj
	end
})
