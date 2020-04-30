function onDeath(player, corpse, killer, mostDamage, unjustified, mostDamage_unjustified)
	if player:hasFlag(PlayerFlag_NotGenerateLoot) or player:getVocation():getId() == VOCATION_NONE then
		return true
	end

	local amulet = player:getSlotItem(CONST_SLOT_NECKLACE)
	local isRedOrBlack = table.contains({SKULL_RED, SKULL_BLACK}, player:getSkull())
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

		if not isPlayer or not player:hasBlessing(6) then
			player:removeItem(ITEM_AMULETOFLOSS, 1, -1, false)
		end
	else
		for i = CONST_SLOT_HEAD, CONST_SLOT_AMMO do
			local item = player:getSlotItem(i)
			local lossPercent = player:getLossPercent()
			if item then
				if isRedOrBlack or math.random(item:isContainer() and 100 or 1000) <= lossPercent then
					if (isRedOrBlack or lossPercent ~= 0) and not item:moveTo(corpse) then
						item:remove()
					end
				end
			end
		end
	end

	if not player:getSlotItem(CONST_SLOT_BACKPACK) then
		player:addItem(ITEM_BAG, 1, false, CONST_SLOT_BACKPACK)
	end
	return true
end
