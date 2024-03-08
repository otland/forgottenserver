local MoveEventRegister = false
local eventList = {["onStepIn"] = "stepin", ["onStepOut"] = "stepout", ["onEquip"] = "equip", ["onDeEquip"] = "deequip", ["onAddItem"] = "additem", ["onRemoveItem"] = "removeitem"}
do
	local mt = getmetatable(MoveEvent)
	local defaultCall = mt.__call

	mt.__call = function(self, params)
		-- we need to make sure that the MoveEvent contains a setup table, if not we are using an outdated version
		if not params then
			print("You are using scripts with an outdated version of revscriptsys (MoveEvent)")
			print(string.format(">> file: %s\n", debug.getinfo(2, "S").source))
			return defaultCall(self)
		end
		-- we are adding the table params with the parameters onto self without calling __newindex
		MoveEventRegister = params
		return defaultCall(self)
	end
end

-- hooking the callback function to c
-- if not we are just adding it as a regular table index without calling __newindex
do
	local function MoveEventNewIndex(self, key, value)
		-- we need to make sure that we are pushing something as a callback function
		if type(value) == "function" then
			-- we know now that it is a function and hook it in c
			-- checking for outdated revscriptsys
			if MoveEventRegister then
				-- looking if event has been set so we can directly hook
				if MoveEventRegister.event then
					-- eventType is set through our config table
					self:event(MoveEventRegister.event)
					self[MoveEventRegister.event](self, key, value)
				else
					-- we get the type through the hook name
					for k, v in pairs(eventList) do
						if key:lower():find(k:lower()) then
							-- found the right event and hook it
							self:event(v)
							self[k](self, key, value)
						end
					end
				end
			else
				self:event(eventList[key])
				self[key](self, key, value)
				-- using outdated version, we just gracefully return
				return
			end
			-- now that we know that we have a hooked event we want to pass the params and register

			-- some prior checks to see if there are ids/uids/aids/pos set
			local checks = {"id", "aid", "uid", "pos"}
			local found = false
			for i = 1, #checks do
				if MoveEventRegister[checks[i]] then
					found = true
					break
				end
			end

			-- making aware that there is no ids set if that's the case
			if not found then
				print("There is no id/aid/uid/pos set for this callback: ".. key)
				return
			end

			-- we are safe to go now as we are sure that everything is correct
			for func, params in pairs(MoveEventRegister) do
				if type(params) == "table" then
					self[func](self, unpack(params))
				else
					self[func](self, params)
				end
			end

			-- now we are registering, which frees our userdata
			alreadyRegistered = true
			self:register()
			-- resetting the global variable which holds our parameter table
			MoveEventRegister = false

			return
		end
		rawset(self, key, value)
	end
	rawgetmetatable("MoveEvent").__newindex = MoveEventNewIndex
end
