function getDeathList(guid)
	local response = {}
	if not tonumber(guid) then
		return
	end
	
	local resultId = db.storeQuery(
		string.format(
			"SELECT `time`, `level`, `killed_by`, `is_player` FROM `player_deaths` WHERE `player_id` = %d ORDER BY `time` DESC",
			guid
		)
	)
	
	if resultId ~= false then
		repeat		
			response[#response + 1] = {
				msg = string.format("Died at level %d by %s", result.getNumber(resultId, "level"), result.getString(resultId, "killed_by")),
				at = result.getNumber(resultId, "time")
			}
		until not result.next(resultId)
		result.free(resultId)
	end
	return response
end

function getFragList(guid)
	local response = {}
	if not tonumber(guid) then
		return
	end
	
	local player = Player(guid)
	if not player then
		return
	end
	
	local resultId = db.storeQuery(
		'SELECT `players`.`name`, `time`, `unjustified` FROM `player_deaths` LEFT JOIN `players` ON `players`.`id` = `player_deaths`.`player_id` WHERE `killed_by` = ' .. db.escapeString(player:getName()) .. ''
	)

	if resultId ~= false then
		repeat		
			response[#response + 1] = {
				victim = result.getString(resultId, "name"),
				at = result.getNumber(resultId, "time"),
				killType = result.getNumber(resultId, "unjustified")
			}
		until not result.next(resultId)
		result.free(resultId)
	end
	return response
end

function getFragStatus(player)
	local response = {
		unjustified = 0,
		duration = 0,
		totalDuration = 0,
	}
	
	local fragTime = configManager.getNumber(configKeys.FRAG_TIME)
	if fragTime <= 0 then
		return response
	end
	
	local skullTime = player:getSkullTime()
	if skullTime <= 0 then
		return response
	end

	response.unjustified = math.ceil(skullTime / fragTime)
	response.duration = math.floor(skullTime % fragTime)
	response.totalDuration = skullTime
	return response
end

function parseItem(item, response)
	local clientId = item:getType():getClientId()
	if not response[clientId] then
		response[clientId] = 0
	end
	response[clientId] = response[clientId] + item:getCount()
	
	if item:isContainer() then
		for containerIndex, containerItem in pairs(item:getItems()) do
			parseItem(containerItem, response)
		end
	end
end

function Player:getClientInventory(category)
	local response = {}
	local responseCount = 0
	
	if category == COMPENDIUM_PLAYERITEMS_EQUIPPED then
		for slot = CONST_SLOT_HEAD, CONST_SLOT_AMMO do
			local slotItem = self:getSlotItem(slot)
			if slotItem then
				parseItem(slotItem, response)
			end
		end
	elseif category == COMPENDIUM_PLAYERITEMS_PURSE then
		local slotItem = self:getSlotItem(CONST_SLOT_STORE_INBOX)
		if slotItem then
			parseItem(slotItem, response)
		end
	elseif category == COMPENDIUM_PLAYERITEMS_STASH then
		-- not implemented yet
	elseif category == COMPENDIUM_PLAYERITEMS_DEPOT then
		local towns = Game.getTowns()
		for _, town in pairs(towns) do
			local depotBox = self:getDepotChest(town:getId())
			if depotBox then
				for containerIndex, containerItem in pairs(depotBox:getItems()) do
					parseItem(containerItem, response)
				end
			end
		end
	elseif category == COMPENDIUM_PLAYERITEMS_MAILBOX then
		local inbox = self:getInbox()
		if inbox then
			for containerIndex, containerItem in pairs(inbox:getItems()) do
				parseItem(containerItem, response)
			end
		end
	end
	
	for _ in pairs(response) do
		responseCount = responseCount + 1
	end
	
	return response, responseCount
end