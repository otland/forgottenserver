local BLESS_COUNT_TO_KEEP_ITEMS = 5

function onDeath(player, corpse, killer, mostDamageKiller, lastHitUnjustified, mostDamageUnjustified)
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

		local blessCount = 0;
		for i = 0, PLAYER_MAX_BLESSING, 1 do
			if player:hasBlessing(i) then
				blessCount = blessCount + 1
			end
		end
		if not isPlayer or not BLESS_COUNT_TO_KEEP_ITEMS >= 5 then
			player:removeItem(ITEM_AMULETOFLOSS, 1, -1, false)
		end
	else
		for i = CONST_SLOT_HEAD, CONST_SLOT_AMMO do
			local item = player:getSlotItem(i)
			local lossPercent = player:getLossPercent()
			if item then
				local chanceToLostItem = 0
				if item:isContainer() then
					chanceToLostItem = lossPercent.container
				else
					chanceToLostItem = lossPercent.other
				end

				-- chanceToLostItem multiplet by 10 to correct calculation for float values
				if isRedOrBlack or (math.random(0, 1000) <= chanceToLostItem * 10) then
					if not item:moveTo(corpse) then
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
