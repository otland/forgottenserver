function Tile:getTopMoveableItem()
	local tileItems = self:getItems()
	for index, item in pairs(tileItems) do
		if index > 8 then
			return
		end
		
		if item:getType():isMovable() and item:getType():isPickupable() then
			if item:hasAttribute(ITEM_ATTRIBUTE_UNIQUEID) then
				-- quest chest
				return
			end
			
			return item
		end
	end	
end

function Player:inspectItemFromPos(position)
	local tile = Tile(position)
	if not tile then
		self:sendCancelMessage("Sorry, not possible.")
		return
	end
	
	local foundItem = tile:getTopMoveableItem()
	if not foundItem then
		self:sendCancelMessage("Sorry, not possible.")
		return
	end
	
	-- to do: inspection window
	self:popupFYI(foundItem:getId())
end

function Player:inspectItemByClientId(clientId)
	-- to do: get itemType by client id
	self:popupFYI(clientId)
end

function Player:inspectItemByTrade(isCounterOffer, containerPos)
	-- to do: lua player get trade
	self:popupFYI(string.format("%s, %d", tostring(isCounterOffer), containerPos))
end

-- view item
function onRequestItemData(player, recvbyte, networkMessage)
	local inspectionType = networkMessage:getByte()
	if inspectionType == INSPECTION_ITEM_NORMAL then
		local thingPos = networkMessage:getPosition()
		if thingPos.x == CONTAINER_POSITION then
			-- y = slotId or 64+client container order,
			-- z = containerSlot
			player:popupFYI(string.format("%d, %d", thingPos.y, thingPos.z))
		else
			local playerPos = player:getPosition()
			
			if math.abs(thingPos.x - playerPos.x) < 8 and math.abs(thingPos.y - playerPos.y) < 8 then
				player:inspectItemFromPos(thingPos)
			end
		end
		return
	elseif inspectionType == INSPECTION_ITEM_NPCTRADE or inspectionType == INSPECTION_ITEM_COMPENDIUM then
		local clientId = networkMessage:getU16()
		player:inspectItemByClientId(clientId)
	elseif inspectionType == INSPECTION_ITEM_PLAYERTRADE then
		local isCounterOffer = networkMessage:getByte() == 1
		local positionInTradeWindow = networkMessage:getByte()
		player:inspectItemByTrade(isCounterOffer, positionInTradeWindow)
	end
	
	return true
end

local callback = onRequestItemData
setPacketEvent(COMPENDIUM_REQUEST_ITEMDATA, callback)