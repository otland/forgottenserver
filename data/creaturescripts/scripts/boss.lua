local function pushSeparated(buffer, sep, ...)
	local argv = {...}
	local argc = #argv
	for k, v in ipairs(argv) do
		table.insert(buffer, v)
		if k < argc and sep then
			table.insert(buffer, sep)
		end
	end
end

local function insertItems(buffer, info, parent, items)
	local start = info.running
	for _, item in ipairs(items) do
		if _ ~= 1 or parent > 100 then
			table.insert(buffer, ",")
		end
		info.running = info.running + 1
		table.insert(buffer, "(")        
		pushSeparated(buffer, ",", info.playerGuid, parent, info.running, item:getId(), item:getSubType(), db.escapeBlob(item:serializeAttributes()))
		table.insert(buffer, ")")

		if item:isContainer() then
			local size = item:getSize()
			if size > 0 then              
				local subItems = {}
				for i = 1, size do
					table.insert(subItems, item:getItem(i - 1))
				end

				insertItems(buffer, info, info.running, subItems)
			end
		end
	end
	return info.running - start
end

local function insertRewardItems(playerGuid, timestamp, itemList)
	db.asyncStoreQuery('SELECT `pid`, `sid` FROM `player_rewards` WHERE player_id = ' .. playerGuid .. ' ORDER BY `sid` ASC;', 
		function(query)
			local lastReward = 0
			local lastStoreId   
			if(query) then             
				repeat
					local sid = result.getDataInt(query, 'sid')
					local pid = result.getDataInt(query, 'pid')

					if pid < 100 then
						lastReward = pid
					end
					lastStoreId = sid
				until not result.next(query)
			end

			local buffer = {'INSERT INTO `player_rewards` (`player_id`, `pid`, `sid`, `itemtype`, `count`, `attributes`) VALUES'}

			--reward bag
			local info = {
				playerGuid = playerGuid,
				running = lastStoreId or 100
			}

			local bag = Game.createItem(ITEM_REWARD_CONTAINER)
			bag:setAttribute(ITEM_ATTRIBUTE_DATE, timestamp)
			if itemList then
				for _, p in ipairs(itemList) do
					bag:addItem(p[1], p[2])
				end
			end

			local total = insertItems(buffer, info, lastReward + 1, {bag})
			table.insert(buffer, ";")

			if total ~= 0 then
				db.query(table.concat(buffer))
			end
		end
	)
end

local function getPlayerStats(bossId, playerGuid, autocreate)
	local ret = globalBosses[bossId][playerGuid]
	if not ret and autocreate then
		ret = {
			bossId = bossId, 
			damageIn = 0, -- damage taken from the boss
			healing = 0, -- healing (other players) done
		}
		globalBosses[bossId][playerGuid] = ret
		return ret
	end
	return ret
end

function onDeath(creature, corpse, killer, mostDamageKiller, lastHitUnjustified, mostDamageUnjustified)
	local monsterType = creature:getType()
	if monsterType:isRewardBoss() then -- Make sure it is a boss
		local bossId = creature:getId()
		local timestamp = os.time()

		local totalDamageOut, totalDamageIn, totalHealing = 0.1, 0.1, 0.1 -- avoid dividing by zero

		local scores = {}
		local info = globalBosses[bossId]
		local damageMap = creature:getDamageMap()

		for guid, stats in pairs(info) do
			local player = Player(stats.playerId)
			local part = damageMap[stats.playerId]
			local damageOut, damageIn, healing = (stats.damageOut or 0) + (part and part.total or 0), stats.damageIn or 0, stats.healing or 0

			totalDamageOut = totalDamageOut + damageOut
			totalDamageIn = totalDamageIn + damageIn
			totalHealing = totalHealing + healing

			table.insert(scores, {
				player = player, 
				guid = guid,
				damageOut = damageOut, 
				damageIn = damageIn,
				healing = healing,
			})            
		end

		local participants = 0
		for _, con in ipairs(scores) do
			local score = (con.damageOut / totalDamageOut) + (con.damageIn / totalDamageIn) + (con.healing / totalHealing)
			con.score = score / 3 -- normalize to 0-1
			if score ~= 0 then
				participants = participants + 1
			end
		end
		table.sort(scores, function(a, b) return a.score > b.score end)

		local expectedScore = 1 / participants

		for _, con in ipairs(scores) do
			local reward, stamina -- ignoring stamina for now because I heard you receive rewards even when it's depleted   
			if con.player then   
				reward = con.player:getReward(timestamp, true)
				stamina = con.player:getStamina()
			else
				stamina = con.stamina or 0
			end

			local playerLoot
			if --[[stamina > 840 and]] con.score ~= 0 then
				local lootFactor = 1
				lootFactor = lootFactor / participants ^ (1 / 3) -- tone down the loot a notch if there are many participants
				lootFactor = lootFactor * (1 + lootFactor) ^ (con.score / expectedScore) -- increase the loot multiplicatively by how many times the player surpassed the expected score
				playerLoot = monsterType:getBossReward(lootFactor, _ == 1)

				if con.player then
					for _, p in ipairs(playerLoot) do
						reward:addItem(p[1], p[2])
					end
				end
			end

			if con.player then
				local lootMessage = {"The following items are available in your reward chest: "}

				if --[[stamina > 840]]true then
					reward:getContentDescription(lootMessage)
				else
					table.insert(lootMessage, 'nothing (due to low stamina)')
				end
				table.insert(lootMessage, ".")
				con.player:sendTextMessage(MESSAGE_EVENT_ADVANCE, table.concat(lootMessage))
			else
				insertRewardItems(con.guid, timestamp, playerLoot)
			end
		end

		globalBosses[bossId] = nil
	end
	return true
end

function onThink(creature, interval)
	local bossId = creature:getId()
	local info = globalBosses[bossId]
	-- Reset all players' status
	for _, player in pairs(info) do
		player.active = false
	end
	-- Set all players in boss' target list as active in the fight
	local targets = creature:getTargetList()
	for _, target in ipairs(targets) do
		if target:isPlayer() then
			local stats = getPlayerStats(bossId, target:getGuid(), true)
			stats.playerId = target:getId() -- Update player id
			stats.active = true            
		end
	end
end

function onHealthChange(creature, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)
	if not next(globalBosses) then
		return primaryDamage, primaryType, secondaryDamage, secondaryType
	end

	if not creature or not attacker then
		return primaryDamage, primaryType, secondaryDamage, secondaryType
	end

	local stats = creature:inBossFight()
	if not stats then
		return primaryDamage, primaryType, secondaryDamage, secondaryType
	end

	local creatureId, attackerId = creature:getId(), attacker:getId()
	stats.playerId = creatureId -- Update player id

	-- Account for healing of others active in the boss fight
	if primaryType == COMBAT_HEALING and attacker:isPlayer() and attackerId ~= creatureId then
		local healerStats = getPlayerStats(stats.bossId, attacker:getGuid(), true)
		healerStats.active = true
		healerStats.playerId = attackerId -- Update player id
		healerStats.healing = healerStats.healing + primaryDamage
	elseif stats.bossId == attackerId then
		-- Account for damage taken from the boss
		stats.damageIn = stats.damageIn + primaryDamage
	end
	return primaryDamage, primaryType, secondaryDamage, secondaryType
end
