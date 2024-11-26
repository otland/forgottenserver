local function notifyAllPlayers(guid, status)
	for _, player in ipairs(Game.getPlayers()) do
		if player:getGuid() ~= guid then
			player:notifyVipStatusChange(guid, status)
		end
	end
end

do
	local event = CreatureEvent("OnlineVip")
	
	function event.onLogin(player)
		local playerGuid = player:getGuid()
		notifyAllPlayers(playerGuid, VIPSTATUS_ONLINE)
	
		local accountId = player:getAccountId()
		local resultId = db.storeQuery("SELECT `player_id`, (SELECT `name` FROM `players` WHERE `id` = `player_id`) AS `name`, `description`, `icon`, `notify` FROM `account_viplist` WHERE `account_id` = " .. accountId)
		if not resultId then
			return true
		end
	
		local playerVip = Vip(playerGuid)
		repeat
			local vipGuid = result.getNumber(resultId, "player_id")
			local vipName = result.getString(resultId, "name")
			local description = result.getString(resultId, "description")
			local icon = result.getNumber(resultId, "icon")
			local notify = result.getNumber(resultId, "notify") ~= 0
	
			-- add to player Vip cache
			playerVip:add(vipGuid)
	
			-- calculate the Vip status
			local status
			local vipPlayer = Player(vipGuid)
			if vipPlayer and player:canSeeCreature(vipPlayer) then
				status = VIPSTATUS_ONLINE
			else
				status = VIPSTATUS_OFFLINE
			end
	
			-- send to client
			player:sendVip(vipGuid, vipName, description, icon, notify, status)
		until not result.next(resultId)
		result.free(resultId)
	
		return true
	end
	
	event:register()
end

do
	local event = CreatureEvent("OfflineVip")
	
	function event.onLogout(player)
		local playerGuid = player:getGuid()
		notifyAllPlayers(playerGuid, VIPSTATUS_OFFLINE)
	
		local playerVip = Vip(playerGuid)
		playerVip:clear()
		return true
	end
	
	event:register()
end

