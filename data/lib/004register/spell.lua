local SpellRegister = false
do
	local mt = getmetatable(Spell)
	local defaultCall = mt.__call

	mt.__call = function(self, params)
		-- we need to make sure that the Spell contains a setup table, if not we are using an outdated version
		if type(params) == "number" then
			print("You are using scripts with an outdated version of revscriptsys (Spell)")
			print(string.format(">> file: %s\n", debug.getinfo(2, "S").source))
			return defaultCall(self, params)
		end
		-- we are adding the table params with the parameters onto self without calling __newindex
		SpellRegister = params
		return defaultCall(self, params)
	end
end

-- hooking the callback function to c
-- if not we are just adding it as a regular table index without calling __newindex
do
	local function SpellNewIndex(self, key, value)
		-- we need to make sure that we are pushing something as a callback function
		if type(value) == "function" then
			-- we know now that it is a function and hook it in c
			self:onCastSpell(key, value)

			-- checking for outdated revscriptsys
			if not SpellRegister then
	    		-- using outdated version, we just gracefully return
	    		return
			end
			-- now that we know that we have a hooked event we want to pass the params and register

			-- some prior checks to see if there are words set
			if not SpellRegister.name then
				print("There is no name set for this callback: ".. key)
				return
			end

			-- we are safe to go now as we are sure that everything is correct
			for func, params in pairs(SpellRegister) do
				if func == "vocation" then
					if type(params[1]) == "table" then
						for k, v in pairs(params) do
							self:vocation(unpack(v))
						end
					else
						self:vocation(unpack(params))
					end
				else
					if type(params) == "table" then
						self[func](self, unpack(params))
					else
						self[func](self, params)
					end
				end
			end

			-- now we are registering, which frees our userdata
			self:register()
			-- resetting the global variable which holds our parameter table
			SpellRegister = false

			return
		end
		rawset(self, key, value)
	end
	rawgetmetatable("Spell").__newindex = SpellNewIndex
end
