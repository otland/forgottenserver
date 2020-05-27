function Condition:createDamageList(list, damage, period, rounds)
	if list == DAMAGELIST_EXPONENTIAL_DAMAGE then
		local exponent, value = -10, 0
		while value < damage do
			value = math.floor(10 * math.pow(1.2, exponent) + 0.5)
			self:addDamage(1, period or 4000, -value)

			if value >= damage then
				local permille = math.random(10, 1200) / 1000
				self:addDamage(1, period or 4000, -math.max(1, math.floor(value * permille + 0.5)))
			else
				exponent = exponent + 1
			end
		end
	elseif list == DAMAGELIST_LOGARITHMIC_DAMAGE then
		local n, value = 0, damage
		while value > 0 do
			value = math.floor(damage * math.pow(2.718281828459, -0.05 * n) + 0.5)
			if value ~= 0 then
				self:addDamage(1, period or 4000, -value)
				n = n + 1
			end
		end
	elseif list == DAMAGELIST_VARYING_PERIOD then
		for _ = 1, rounds or 1 do
			self:addDamage(1, math.random(period[1], period[2]) * 1000, -damage)
		end
	elseif list == DAMAGELIST_CONSTANT_PERIOD then
		self:addDamage(rounds or 1, period * 1000, -damage)
	end
end
