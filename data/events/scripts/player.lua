function Player:onBrowseField(position)
	local onBrowseField = EventCallback.onBrowseField
	if onBrowseField then
		return onBrowseField(self, position)
	end
	return true
end

function Player:onLook(thing, position, distance)
	local description = ""
	local onLook = EventCallback.onLook
	if onLook then
		description = onLook(self, thing, position, distance, description)
	end

	if description ~= "" then
		self:sendTextMessage(MESSAGE_INFO_DESCR, description)
	end
end

function Player:onLookInBattleList(creature, distance)
	local description = ""
	local onLookInBattleList = EventCallback.onLookInBattleList
	if onLookInBattleList then
		description = onLookInBattleList(self, creature, distance, description)
	end

	if description ~= "" then
		self:sendTextMessage(MESSAGE_INFO_DESCR, description)
	end
end

function Player:onLookInTrade(partner, item, distance)
	local description = "You see " .. item:getDescription(distance)
	local onLookInTrade = EventCallback.onLookInTrade
	if onLookInTrade then
		description = onLookInTrade(self, partner, item, distance, description)
	end

	if description ~= "" then
		self:sendTextMessage(MESSAGE_INFO_DESCR, description)
	end
end

function Player:onLookInShop(itemType, count)
	local description = "You see "
	local onLookInShop = EventCallback.onLookInShop
	if onLookInShop then
		description = onLookInShop(self, itemType, count, description)
	end

	if description ~= "" then
		self:sendTextMessage(MESSAGE_INFO_DESCR, description)
	end
end

function Player:onLookInMarket(itemType)
	local onLookInMarket = EventCallback.onLookInMarket
	if onLookInMarket then
		onLookInMarket(self, itemType)
	end
end

function Player:onMoveItem(item, count, fromPosition, toPosition, fromCylinder, toCylinder)
	local onMoveItem = EventCallback.onMoveItem
	if onMoveItem then
		return onMoveItem(self, item, count, fromPosition, toPosition, fromCylinder, toCylinder)
	end
	return RETURNVALUE_NOERROR
end

function Player:onItemMoved(item, count, fromPosition, toPosition, fromCylinder, toCylinder)
	local onItemMoved = EventCallback.onItemMoved
	if onItemMoved then
		onItemMoved(self, item, count, fromPosition, toPosition, fromCylinder, toCylinder)
	end
end

function Player:onMoveCreature(creature, fromPosition, toPosition)
	local onMoveCreature = EventCallback.onMoveCreature
	if onMoveCreature then
		return onMoveCreature(self, creature, fromPosition, toPosition)
	end
	return true
end

function Player:onReportRuleViolation(targetName, reportType, reportReason, comment, translation)
	local onReportRuleViolation = EventCallback.onReportRuleViolation
	if onReportRuleViolation then
		onReportRuleViolation(self, targetName, reportType, reportReason, comment, translation)
	end
end

function Player:onReportBug(message, position, category)
	local onReportBug = EventCallback.onReportBug
	if onReportBug then
		return onReportBug(self, message, position, category)
	end
	return true
end

function Player:onTurn(direction)
	local onTurn = EventCallback.onTurn
	if onTurn then
		return onTurn(self, direction)
	end
	return true
end

function Player:onTradeRequest(target, item)
	local onTradeRequest = EventCallback.onTradeRequest
	if onTradeRequest then
		return onTradeRequest(self, target, item)
	end
	return true
end

function Player:onTradeAccept(target, item, targetItem)
	local onTradeAccept = EventCallback.onTradeAccept
	if onTradeAccept then
		return onTradeAccept(self, target, item, targetItem)
	end
	return true
end

function Player:onTradeCompleted(target, item, targetItem, isSuccess)
	local onTradeCompleted = EventCallback.onTradeCompleted
	if onTradeCompleted then
		onTradeCompleted(self, target, item, targetItem, isSuccess)
	end
end

function Player:onPodiumRequest(item)
	if not item:isPodium() then
		self:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		return
	end

	self:sendEditPodium(item)
end

function Player:onPodiumEdit(item, outfit, direction, isVisible)
	if not item:isPodium() then
		self:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
		return
	end

	if not self:getGroup():getAccess() then
		-- check if the player is in melee range
		if getDistanceBetween(self:getPosition(), item:getPosition()) > 1 then
			self:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
			return
		end

		-- reset outfit if unable to wear
		if not self:canWearOutfit(outfit.lookType, outfit.lookAddons) then
			outfit.lookType = 0
		end

		-- reset mount if unable to ride
		local mount = Game.getMountIdByLookType(outfit.lookMount)
		if not (mount and self:hasMount(mount)) then
			outfit.lookMount = 0
		end
	end

	local podiumOutfit = item:getOutfit()
	local playerOutfit = self:getOutfit()

	-- use player outfit if podium is empty
	if podiumOutfit.lookType == 0 then
		podiumOutfit.lookType = playerOutfit.lookType
		podiumOutfit.lookHead = playerOutfit.lookHead
		podiumOutfit.lookBody = playerOutfit.lookBody
		podiumOutfit.lookLegs = playerOutfit.lookLegs
		podiumOutfit.lookFeet = playerOutfit.lookFeet
		podiumOutfit.lookAddons = playerOutfit.lookAddons
	end

	-- set player mount colors podium is empty
	if podiumOutfit.lookMount == 0 then
		podiumOutfit.lookMount = playerOutfit.lookMount
		podiumOutfit.lookMountHead = playerOutfit.lookMountHead
		podiumOutfit.lookMountBody = playerOutfit.lookMountBody
		podiumOutfit.lookMountLegs = playerOutfit.lookMountLegs
		podiumOutfit.lookMountFeet = playerOutfit.lookMountFeet
	end

	-- "outfit" box checked
	if outfit.lookType ~= 0 then
		podiumOutfit.lookType = outfit.lookType
		podiumOutfit.lookHead = outfit.lookHead
		podiumOutfit.lookBody = outfit.lookBody
		podiumOutfit.lookLegs = outfit.lookLegs
		podiumOutfit.lookFeet = outfit.lookFeet
		podiumOutfit.lookAddons = outfit.lookAddons
	end

	-- "mount" box checked
	if outfit.lookMount ~= 0 then
		podiumOutfit.lookMount = outfit.lookMount
		podiumOutfit.lookMountHead = outfit.lookMountHead
		podiumOutfit.lookMountBody = outfit.lookMountBody
		podiumOutfit.lookMountLegs = outfit.lookMountLegs
		podiumOutfit.lookMountFeet = outfit.lookMountFeet
	end

	-- prevent invisible podium state
	if outfit.lookType == 0 and outfit.lookMount == 0 then
		isVisible = true
	end

	-- save player choices
	item:setFlag(PODIUM_SHOW_PLATFORM, isVisible)
	item:setFlag(PODIUM_SHOW_OUTFIT, outfit.lookType ~= 0)
	item:setFlag(PODIUM_SHOW_MOUNT, outfit.lookMount ~= 0)
	item:setDirection(direction < DIRECTION_NORTHEAST and direction or DIRECTION_SOUTH)
	item:setOutfit(podiumOutfit)
end

function Player:onGainExperience(source, exp, rawExp, sendText)
	local onGainExperience = EventCallback.onGainExperience
	return onGainExperience and onGainExperience(self, source, exp, rawExp, sendText) or exp
end

function Player:onLoseExperience(exp)
	local onLoseExperience = EventCallback.onLoseExperience
	return onLoseExperience and onLoseExperience(self, exp) or exp
end

function Player:onGainSkillTries(skill, tries)
	local onGainSkillTries = EventCallback.onGainSkillTries
	if not APPLY_SKILL_MULTIPLIER then
		return onGainSkillTries and onGainSkillTries(self, skill, tries) or tries
	end

	if skill == SKILL_MAGLEVEL then
		tries = tries * configManager.getNumber(configKeys.RATE_MAGIC)
		return onGainSkillTries and onGainSkillTries(self, skill, tries) or tries
	end
	tries = tries * configManager.getNumber(configKeys.RATE_SKILL)
	return onGainSkillTries and onGainSkillTries(self, skill, tries) or tries
end

function Player:onWrapItem(item)
	local topCylinder = item:getTopParent()
	if not topCylinder then
		return
	end

	local tile = Tile(topCylinder:getPosition())
	if not tile then
		return
	end

	local house = tile:getHouse()
	if not house then
		self:sendCancelMessage("You can only wrap and unwrap this item inside a house.")
		return
	end

	if house ~= self:getHouse() and not string.find(house:getAccessList(SUBOWNER_LIST):lower(), "%f[%a]" .. self:getName():lower() .. "%f[%A]") then
		self:sendCancelMessage("You cannot wrap or unwrap items from a house, which you are only guest to.")
		return
	end

	local wrapId = item:getAttribute("wrapid")
	if wrapId == 0 then
		return
	end

	local onWrapItem = EventCallback.onWrapItem
	if not onWrapItem or onWrapItem(self, item) then
		local oldId = item:getId()
		item:remove(1)
		local item = tile:addItem(wrapId)
		if item then
			item:setAttribute("wrapid", oldId)
		end
	end
end

function Player:onInventoryUpdate(item, slot, equip)
	local onInventoryUpdate = EventCallback.onInventoryUpdate
	if onInventoryUpdate then
		onInventoryUpdate(self, item, slot, equip)
	end
end

function Player:onNetworkMessage(recvByte, msg)
	local handler = PacketHandlers[recvByte]
	if not handler then
		--io.write(string.format("Player: %s sent an unknown packet header: 0x%02X with %d bytes!\n", self:getName(), recvByte, msg:len()))
		return
	end

	handler(self, msg)
end
