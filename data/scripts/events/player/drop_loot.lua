local event = Event()

event.onCreatureDeath = function(self, corpse, killer, mostDamageKiller, lastHitUnjustified, mostDamageUnjustified)
	if not self:isPlayer() then
		return
	end

	if self:hasFlag(PlayerFlag_NotGenerateLoot) or self:getVocation():getId() == VOCATION_NONE then
		return
	end

	local amulet = self:getSlotItem(CONST_SLOT_NECKLACE)
	local isRedOrBlack = table.contains({SKULL_RED, SKULL_BLACK}, self:getSkull())
	if amulet and amulet.itemid == ITEM_AMULETOFLOSS and not isRedOrBlack then
		local isPlayer = false
		if killer then
			if killer:isPlayer() then
				isPlayer = true
			else
				local master = killer:getMaster()
				if master and master:isPlayer() then
					isPlayer = true
				end
			end
		end

		if not isPlayer or not self:hasBlessing(6) then
			self:removeItem(ITEM_AMULETOFLOSS, 1, -1, false)
		end
	else
		for i = CONST_SLOT_HEAD, CONST_SLOT_AMMO do
			local item = self:getSlotItem(i)
			if item then
				local lossPercent = self:getLossPercent()
				local lossChance = item:isContainer() and lossPercent.container or lossPercent.other

				if isRedOrBlack or (math.random(0, 1000) <= lossChance * 10) then
					if not item:moveTo(corpse) then
						item:remove()
					end
				end
			end
		end
	end

	if not self:getSlotItem(CONST_SLOT_BACKPACK) then
		self:addItem(ITEM_BAG, 1, false, CONST_SLOT_BACKPACK)
	end
end

event:register()
