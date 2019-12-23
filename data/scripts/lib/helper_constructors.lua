local classes = {Action, CreatureEvent, Spell, TalkAction, MoveEvent, GlobalEvent, Weapon}

for _, class in ipairs(classes) do
	local MT = getmetatable(class)
	local DefaultConstructor = MT.__call

	MT.__call = function(self, def, ...)
		-- Backwards compatibility for default obj() constructor
		if type(def) ~= "table" then
			return DefaultConstructor(self, def, ...)
		end

		local obj = nil
		if def.init then
			obj = DefaultConstructor(self, unpack(def.init))
		else
			obj = DefaultConstructor(self)
		end

		-- Call each method from definition table with the value as params
		local hasCallback = false

		for methodName, value in pairs(def) do
			-- Strictly check if a correct callback is passed
			if methodName:sub(1, 2) == "on" and type(value) == "function" and rawget(class, methodName) then
				hasCallback = true
			end 

			if methodName ~= "register" then
				local method = rawget(self, methodName)
				if method then
					if type(value) == "table" then
						method(obj, unpack(value))
					else
						method(obj, value)
					end
				end
			end
		end

		-- Only register if callback has already been defined, otherwise defining afterwards will not work
		if def.register then
			if not hasCallback then
				print("Warning: Event not registered due to there being no callback.")
			else
				obj:register()
			end
		end

		return obj
	end
end
