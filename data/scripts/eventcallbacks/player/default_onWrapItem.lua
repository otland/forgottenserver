local ec = EventCallback

ec.onWrapItem = function(self, item, position)
	local topCylinder = item:getTopParent()
	if not topCylinder then
		return false
	end

	local tile = Tile(topCylinder:getPosition())
	if not tile then
		return false
	end

	if not tile:getHouse() then
		self:sendCancelMessage("You can only wrap and unwrap this item inside a house.")
		return false
	end

	local wrapId = item:getAttribute("wrapid")
	if wrapId == 0 then
		return false
	end
	return true
end
