local classes = {Action, CreatureEvent, Spell, TalkAction, MoveEvent, GlobalEvent, Weapon}

for _, class in ipairs(classes) do
	local MT = getmetatable(class)
	local DefaultConstructor = MT.__call

	MT.__call = function(self, def, ...)
		local obj = DefaultConstructor(self, def, ...)

		-- Backwards compatibility for default obj() constructor
		if type(def) ~= "table" then
			return obj
		end

		-- Call each method from definition table with the value as params
		local hasCallback = false

		for f, args in pairs(def) do
			-- Strictly check if a correct callback is passed
			if f:sub(1, 2) == "on" and type(args) == "function" and rawget(class, f) then
				hasCallback = true
			end 

			if f ~= "register" then
				local method = self[f]
				if method then
					if type(args) == "table" then
						method(obj, unpack(args))
					else
						method(obj, args)
					end
				end
			end
		end

		-- Only register if callback has already been defined, otherwise defining afterwards will not work
		if def.register then
			if not hasCallback then
				print("Warning: Registering an event with no callback.")
			else
				obj:register()
			end
		end

		return obj
	end
end
