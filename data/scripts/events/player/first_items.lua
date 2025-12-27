local firstItems = {2050, 2382}

local event = Event()

event.onPlayerJoin = function(self)
	if self:getLastLoginSaved() == 0 then
		for i = 1, #firstItems do
			self:addItem(firstItems[i], 1)
		end
		self:addItem(self:getSex() == 0 and 2651 or 2650, 1)
		self:addItem(ITEM_BAG, 1):addItem(2674, 1)
	end
end

event:register()
