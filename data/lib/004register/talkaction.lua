local TalkActionRegister = false
do
	local mt = getmetatable(TalkAction)
	local defaultCall = mt.__call

	mt.__call = function(self, params, ...)
		-- we need to make sure that the TalkAction contains a setup table, if not we are using an outdated version
		if type(params) == "string" then
			print("You are using scripts with an outdated version of revscriptsys (TalkAction)")
			print(string.format(">> file: %s\n", debug.getinfo(2, "S").source))
			return defaultCall(self, params, ...)
		end
		-- we are adding the table params with the parameters onto self without calling __newindex
		TalkActionRegister = params
		return defaultCall(self)
	end
end

-- hooking the callback function to c
-- if not we are just adding it as a regular table index without calling __newindex
do
	local function TalkActionNewIndex(self, key, value)
		-- we need to make sure that we are pushing something as a callback function
		if type(value) == "function" then
			-- we know now that it is a function and hook it in c
			self:onSay(key, value)

			-- checking for outdated revscriptsys
			if not TalkActionRegister then
	    		-- using outdated version, we just gracefully return
	    		return
			end
			-- now that we know that we have a hooked event we want to pass the params and register

			-- some prior checks to see if there are words set
			if not TalkActionRegister.word then
				print("There is no word set for this callback: ".. key)
				return
			end

			-- we are safe to go now as we are sure that everything is correct
			for func, params in pairs(TalkActionRegister) do
				if type(params) == "table" then
					self[func](self, unpack(params))
				else
					self[func](self, params)
				end
			end

			-- now we are registering, which frees our userdata
			self:register()
			-- resetting the global variable which holds our parameter table
			TalkActionRegister = false

			return
		end
		rawset(self, key, value)
	end
	rawgetmetatable("TalkAction").__newindex = TalkActionNewIndex
end
