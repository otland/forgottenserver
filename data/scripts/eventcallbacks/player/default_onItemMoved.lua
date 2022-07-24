local ec = EventCallback

ec.onItemMoved = function(self, item, count, fromPosition, toPosition, fromCylinder, toCylinder)
	if item:getId() == 2579 and toPosition.x == 0xFFFF then -- trap
		item:transform(item.itemid - 1) -- closed trap
		self:getPosition():sendMagicEffect(CONST_ME_POFF)
	end
	return true
end

ec:register()
