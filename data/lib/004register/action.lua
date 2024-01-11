local ActionRegister = false
do
	local mt = getmetatable(Action)
	local defaultCall = mt.__call

	mt.__call = function(self, params)
		-- we need to make sure that the Action contains a setup table, if not we are using an outdated version
		if not params then
			print(("You are using scripts with an outdated version of revscriptsys (Action)"))
			print(string.format(">> file: %s\n", debug.getinfo(2, "S").source))
			return defaultCall(self)
		end
		-- we are adding the table params with the parameters onto self without calling __newindex
		ActionRegister = params
		return defaultCall(self)
	end
end

-- hooking the callback function to c
-- if not we are just adding it as a regular table index without calling __newindex
do
	local function ActionNewIndex(self, key, value)
		-- we need to make sure that we are pushing something as a callback function
		if type(value) == "function" then
			-- we know now that it is a function and hook it in c
			self:onUse(key, value)

			-- checking for outdated revscriptsys
			if not ActionRegister then
				-- using outdated version, we just gracefully return
				return
			end
			-- now that we know that we have a hooked event we want to pass the params and register

			-- some prior checks to see if there is any type of id set
			local ids = {"id", "aid", "uid"}
			local found = false
			for i = 1, #ids do
				if ActionRegister[ids[i]] then
					found = true
					break
				end
			end

			-- making aware that there is no ids set if that's the case
			if not found then
				print("There is no id/aid/uid set for this callback: ".. key)
				return
			end

			-- we are safe to go now as we are sure that everything is correct
			for func, params in pairs(ActionRegister) do
				if type(params) == "table" then
					self[func](self, unpack(params))
				else
					self[func](self, params)
				end
			end

			-- now we are registering, which frees our userdata
			self:register()
			-- resetting the global variable which holds our parameter table
			ActionRegister = false

			return
		end
		rawset(self, key, value)
	end
	rawgetmetatable("Action").__newindex = ActionNewIndex
end
