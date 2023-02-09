function Player:onBrowseField(position)
	if EventCallback.onBrowseField then
		return EventCallback.onBrowseField(self, position)
	end
	return true
end

function Player:onLook(thing, position, distance)
	local description = ""
	if EventCallback.onLook then
		description = EventCallback.onLook(self, thing, position, distance, description)
	end

	if description ~= "" then
		self:sendTextMessage(MESSAGE_INFO_DESCR, description)
	end
end

function Player:onLookInBattleList(creature, distance)
	local description = ""
	if EventCallback.onLookInBattleList then
		description = EventCallback.onLookInBattleList(self, creature, distance, description)
	end

	if description ~= "" then
		self:sendTextMessage(MESSAGE_INFO_DESCR, description)
	end
end

function Player:onLookInTrade(partner, item, distance)
	local description = "You see " .. item:getDescription(distance)
	if EventCallback.onLookInTrade then
		description = EventCallback.onLookInTrade(self, partner, item, distance, description)
	end

	if description ~= "" then
		self:sendTextMessage(MESSAGE_INFO_DESCR, description)
	end
end

function Player:onLookInShop(itemType, count)
	local description = "You see "
	if EventCallback.onLookInShop then
		description = EventCallback.onLookInShop(self, itemType, count, description)
	end

	if description ~= "" then
		self:sendTextMessage(MESSAGE_INFO_DESCR, description)
	end
end

function Player:onLookInMarket(itemType)
	if EventCallback.onLookInMarket then
		EventCallback.onLookInMarket(self, itemType)
	end
end

function Player:onMoveItem(item, count, fromPosition, toPosition, fromCylinder, toCylinder)
	if EventCallback.onMoveItem then
		return EventCallback.onMoveItem(self, item, count, fromPosition, toPosition, fromCylinder, toCylinder)
	end
	return RETURNVALUE_NOERROR
end

function Player:onItemMoved(item, count, fromPosition, toPosition, fromCylinder, toCylinder)
	if EventCallback.onItemMoved then
		EventCallback.onItemMoved(self, item, count, fromPosition, toPosition, fromCylinder, toCylinder)
	end
end

function Player:onMoveCreature(creature, fromPosition, toPosition)
	if EventCallback.onMoveCreature then
		return EventCallback.onMoveCreature(self, creature, fromPosition, toPosition)
	end
	return true
end

function Player:onReportRuleViolation(targetName, reportType, reportReason, comment, translation)
	if EventCallback.onReportRuleViolation then
		EventCallback.onReportRuleViolation(self, targetName, reportType, reportReason, comment, translation)
	end
end

function Player:onReportBug(message, position, category)
	if EventCallback.onReportBug then
		return EventCallback.onReportBug(self, message, position, category)
	end
	return true
end

function Player:onTurn(direction)
	if EventCallback.onTurn then
		return EventCallback.onTurn(self, direction)
	end
	return true
end

function Player:onTradeRequest(target, item)
	if EventCallback.onTradeRequest then
		return EventCallback.onTradeRequest(self, target, item)
	end
	return true
end

function Player:onTradeAccept(target, item, targetItem)
	if EventCallback.onTradeAccept then
		return EventCallback.onTradeAccept(self, target, item, targetItem)
	end
	return true
end

function Player:onTradeCompleted(target, item, targetItem, isSuccess)
	if EventCallback.onTradeCompleted then
		EventCallback.onTradeCompleted(self, target, item, targetItem, isSuccess)
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

local soulCondition = Condition(CONDITION_SOUL, CONDITIONID_DEFAULT)
soulCondition:setTicks(4 * 60 * 1000)
soulCondition:setParameter(CONDITION_PARAM_SOULGAIN, 1)

local function useStamina(player)
	local staminaMinutes = player:getStamina()
	if staminaMinutes == 0 then
		return
	end

	local playerId = player:getId()
	if not nextUseStaminaTime[playerId] then
		nextUseStaminaTime[playerId] = 0
	end

	local currentTime = os.time()
	local timePassed = currentTime - nextUseStaminaTime[playerId]
	if timePassed <= 0 then
		return
	end

	if timePassed > 60 then
		if staminaMinutes > 2 then
			staminaMinutes = staminaMinutes - 2
		else
			staminaMinutes = 0
		end
		nextUseStaminaTime[playerId] = currentTime + 120
	else
		staminaMinutes = staminaMinutes - 1
		nextUseStaminaTime[playerId] = currentTime + 60
	end
	player:setStamina(staminaMinutes)
end

function Player:onGainExperience(source, exp, rawExp)
	if not source or source:isPlayer() then
		return exp
	end

	-- Soul regeneration
	local vocation = self:getVocation()
	if self:getSoul() < vocation:getMaxSoul() and exp >= self:getLevel() then
		soulCondition:setParameter(CONDITION_PARAM_SOULTICKS, vocation:getSoulGainTicks() * 1000)
		self:addCondition(soulCondition)
	end

	-- Apply experience stage multiplier
	exp = exp * Game.getExperienceStage(self:getLevel())

	-- Stamina modifier
	if configManager.getBoolean(configKeys.STAMINA_SYSTEM) then
		useStamina(self)

		local staminaMinutes = self:getStamina()
		if staminaMinutes > 2340 and self:isPremium() then
			exp = exp * 1.5
		elseif staminaMinutes <= 840 then
			exp = exp * 0.5
		end
	end

	return EventCallback.onGainExperience and EventCallback.onGainExperience(self, source, exp, rawExp) or exp
end

function Player:onLoseExperience(exp)
	return EventCallback.onLoseExperience and EventCallback.onLoseExperience(self, exp) or exp
end

function Player:onGainSkillTries(skill, tries)
	if not APPLY_SKILL_MULTIPLIER then
		return EventCallback.onGainSkillTries and EventCallback.onGainSkillTries(self, skill, tries) or tries
	end

	if skill == SKILL_MAGLEVEL then
		tries = tries * configManager.getNumber(configKeys.RATE_MAGIC)
		return EventCallback.onGainSkillTries and EventCallback.onGainSkillTries(self, skill, tries) or tries
	end
	tries = tries * configManager.getNumber(configKeys.RATE_SKILL)
	return EventCallback.onGainSkillTries and EventCallback.onGainSkillTries(self, skill, tries) or tries
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

	if not EventCallback.onWrapItem or EventCallback.onWrapItem(self, item) then
		local oldId = item:getId()
		item:remove(1)
		local item = tile:addItem(wrapId)
		if item then
			item:setAttribute("wrapid", oldId)
		end
	end
end

function Player:onInventoryUpdate(item, slot, equip)
	if EventCallback.onInventoryUpdate then
		EventCallback.onInventoryUpdate(self, item, slot, equip)
	end
end
