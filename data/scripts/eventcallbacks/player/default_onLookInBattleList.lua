local ec = EventCallback

ec.onLookInBattleList = function(self, creature, distance)
	local description = "You see " .. creature:getDescription(distance)
	if self:getGroup():getAccess() then
		local str = "%s\nHealth: %d / %d"
		if creature:isPlayer() and creature:getMaxMana() > 0 then
			str = string.format("%s, Mana: %d / %d", str, creature:getMana(), creature:getMaxMana())
		end
		description = string.format(str, description, creature:getHealth(), creature:getMaxHealth()) .. "."

		local position = creature:getPosition()
		description = string.format(
			"%s\nPosition: %d, %d, %d",
			description, position.x, position.y, position.z
		)

		local outfit = creature:getOutfit()
		if outfit.lookTypeEx == 0 then
			description = string.format("%s\nOutfit: %d", description, outfit.lookType)
			if outfit.lookHead > 0 or outfit.lookBody > 0 or outfit.lookLegs > 0 or outfit.lookFeet > 0 then
				description = string.format("%s\nColors: %d, %d, %d, %d", description, outfit.lookHead, outfit.lookBody, outfit.lookLegs, outfit.lookFeet)
			end
			
			if outfit.lookAddons > 0 then
				description = string.format("%s\nAddons: %d", description, outfit.lookAddons)
			end
		else
			local itemType = ItemType(outfit.lookTypeEx)
			local clientId = 0
			if itemType then
				clientId = itemType:getClientId()
			end
			
			description = string.format("%s\nItem ID: %d\nClient ID: %d", description, outfit.lookTypeEx, clientId)
		end
		
		if outfit.lookMount ~= 0 then
			description = string.format("%s\nMount: %d", description, outfit.lookMount)
			if outfit.lookMountHead > 0 or outfit.lookMountBody > 0 or outfit.lookMountLegs > 0 or outfit.lookMountFeet > 0 then
				description = string.format("%s\nColors: %d, %d, %d, %d", description, outfit.lookMountHead, outfit.lookMountBody, outfit.lookMountLegs, outfit.lookMountFeet)
			end
		end
		
		description = string.format("%s\ncid: %s", description, creature:getId())
		
		if creature:isPlayer() then
			description = string.format("%s\nGUID: %s", description, creature:getGuid())
			description = string.format("%s\nIP: %s", description, Game.convertIpToString(creature:getIp()))
		end
	end
	return description
end

ec:register()
