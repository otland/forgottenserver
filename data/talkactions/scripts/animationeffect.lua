function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	local effect = tonumber(param)
	local orig = player:getPosition()
	local d1, d2 = {z = orig.z}, {z = orig.z}

	d1.x = orig.x - 7
	d2.x = orig.x + 7
	for i = -5, 5 do
		d1.y = orig.y + i
		d2.y = d1.y
		orig:sendDistanceEffect(d1, effect)
		orig:sendDistanceEffect(d2, effect)
	end

	d1.y = orig.y - 5
	d2.y = orig.y + 5
	for i = -6, 6 do
		d1.x = orig.x + i
		d2.x = d1.x
		orig:sendDistanceEffect(d1, effect)
		orig:sendDistanceEffect(d2, effect)
	end
	return false
end
