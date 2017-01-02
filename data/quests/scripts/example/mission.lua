function Mission:setDescription(player)
	local state = player:getStorageValue(self:getStorageId())
	if state == 4 then
		-- Here the true is important, it means it will get the description from the XML instead of calling this function again
		-- If not used it would cause a stack overflow!
		return self:getDescription(player, true):format(math.random(1, 100))
	elseif state == 5 then
		return self:getDescription(player, true):format(player:getLevel())
	end

	return self:getDescription(player, true)
end
