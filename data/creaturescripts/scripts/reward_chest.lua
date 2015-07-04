REWARD_CHEST = {
	BOSSES = {"bibby bloodbath", "chizzoron the distorter", "ferumbras",
			"furyosa", "gaz'haragoth", "ghazbaran", "hirintror",
			"jaul", "mad mage", "mawhawk", "morgaroth", "obujos",
			"ocyakao", "omrafir", "orshabaal", "raging mage",
			"tanjis", "the mutated pumpkin", "the pale count",
			"the welter", "tyrn", "white pale", "zulazza the corruptor",
			"zushuka"},
	LOOT = {},			
	DECAY = {},
	DEPOT_ID = 99,
	CONTAINER = 21518,
	ENCODE = true,			--encode stored rewards on db?
	EXPIRE = 7*24*60*60, 	--days to erase stored rewards
	STORAGE = 18394, 		--player storage for chest exhaustion
	EXHAUST = 5*60, 		--time allowed to use again the chest
}

-- Lua 5.1+ base64 v3.0 (c) 2009 by Alex Kloss <alexthkloss@web.de>
-- licensed under the terms of the LGPL2

-- character table string
local enc = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/'

function string.encode(str)
    return ((str:gsub('.', function(x) 
        local r,enc='',x:byte()
        for i=8,1,-1 do r=r..(enc%2^i-enc%2^(i-1)>0 and '1' or '0') end
        return r;
    end)..'0000'):gsub('%d%d%d?%d?%d?%d?', function(x)
        if (#x < 6) then return '' end
        local c=0
        for i=1,6 do c=c+(x:sub(i,i)=='1' and 2^(6-i) or 0) end
        return enc:sub(c+1,c+1)
    end)..({ '', '==', '=' })[#str%3+1])
end

function string.decode(str)
    str = string.gsub(str, '[^'.. enc ..'=]', '')
    return (str:gsub('.', function(x)
        if (x == '=') then return '' end
        local r,f='',(enc:find(x)-1)
        for i=6,1,-1 do r=r..(f%2^i-f%2^(i-1)>0 and '1' or '0') end
        return r;
    end):gsub('%d%d%d?%d?%d?%d?%d?%d?', function(x)
        if (#x ~= 8) then return '' end
        local c=0
        for i=1,8 do c=c+(x:sub(i,i)=='1' and 2^(8-i) or 0) end
        return string.char(c)
    end))
end

function table.find(t, value, sensitive)
    local sensitive = sensitive or true
    if(not sensitive and type(value) == 'string') then
        for i, v in pairs(t) do
            if(type(v) == 'string') then
                if(v:lower() == value:lower()) then
                    return i
                end
            end
        end
        return nil
    end
    for i, v in pairs(t) do
        if(v == value) then
            return i
        end
    end
    return nil
end

function table.serialize(x, recur)
	local t = type(x)
	recur = recur or {}
	if(t == nil) then
		return "nil"
	elseif(t == "string") then
		return string.format("%q", x)
	elseif(t == "number") then
		return tostring(x)
	elseif(t == "boolean") then
		return x and "true" or "false"
	elseif(getmetatable(x)) then
		error("Can not serialize a table that has a metatable associated with it.")
	elseif(t == "table") then
		if(table.find(recur, x)) then
			error("Can not serialize recursive tables.")
		end
		table.insert(recur, x)
		local s = "{"
		for k, v in pairs(x) do
			s = s .. "[" .. table.serialize(k, recur) .. "]" .. " = " .. table.serialize(v, recur) .. ", "
		end
		return s:sub(0, s:len() - 2) .. "}"
	end
	error("Can not serialize value of type '" .. t .. "'.")
end

function table.unserialize(str)
    return loadstring('return ' .. str)()
end

function serialize(t)
	return REWARD_CHEST.ENCODE and string.encode(table.serialize(t)) or table.serialize(t)
end

function unserialize(str)
	return REWARD_CHEST.ENCODE and table.unserialize(string.decode(str)) or table.unserialize(str)
end

function addContainerItems(container, items)
    for k, v in pairs(items) do
        if ItemType(k):isContainer() then
            local newBag = Container(doCreateItemEx(k, 1))
            addContainerItems(newBag, v)
            container:addItemEx(newBag)
        else
            container:addItem(v[1], v[2])
        end
    end
end

function MonsterType.createLootItem(self, lootBlock, chance)
	local lootTable, itemCount = {}, 0
	local randvalue = math.random(0, 100000) / (getConfigInfo("rateLoot") * chance)
	if randvalue < lootBlock.chance then
		if (ItemType(lootBlock.itemId):isStackable()) then
			itemCount = randvalue % lootBlock.maxCount + 1
		else
			itemCount = 1
		end
	end

	while itemCount > 0 do
		local n = math.min(itemCount, 100)
		itemCount = itemCount - n
		table.insert(lootTable, {lootBlock.itemId, n})
	end

	return lootTable
end

function MonsterType.getBossReward(self, chance)
	local result = {}
	if getConfigInfo("rateLoot") > 0 then
		for _, loot in pairs(self:getLoot()) do
			local itemList = self:createLootItem(loot, chance)
			if itemList then
				for _, item in ipairs(itemList) do
					table.insert(result, item)
				end
			end
		end
	end
	return result
end

function getDecayTime(id, name)
	local decayTime = 0
	do
		local o = io.open('data/items/items.xml','r')
		file = o:read('*a')
		o:close()
	end
	local function sumDecayTime(corpse)
		for attr in file:gmatch('<item.-id="' .. corpse ..'"(.-)</item>') do
			local decayTo = attr:match('<attribute.-key="decayTo".-value="(.-)".-/>')
			local duration = attr:match('<attribute.-key="duration".-value="(.-)".-/>')
			decayTime = decayTime + duration
			if decayTime > 3000 then
				print("[Warning - RewardChest] Decay time for corpse of " .. name .." is infinite - using 3000 instead")
				return 3000
			end			
			if tonumber(decayTo) > 0 then
				sumDecayTime(decayTo)
			end
			break
		end
	end
	sumDecayTime(id)
	return decayTime
end

function loadCorpses()
	for _, name in ipairs(REWARD_CHEST.BOSSES) do
		if MonsterType(name) ~= nil then
			REWARD_CHEST.DECAY[name] = getDecayTime(MonsterType(name):getCorpseId(), name)
		end
	end
end
addEvent(loadCorpses, 0)

function getPlayerByGUID(guid)
	for _, player in ipairs(Game.getPlayers()) do
		if guid == player:getGuid() then
			return player
		end
	end
	return nil
end

function Player.addReward(self, reward, time, id)
    local LootBag = Container(doCreateItemEx(REWARD_CHEST.CONTAINER, 1))
    LootBag:setAttribute('text', time)
    addContainerItems(LootBag, reward)
    if id then
    	db.query('DELETE FROM player_rewardchest WHERE id = ' .. id .. ';')
    end
    return self:getDepotChest(REWARD_CHEST.DEPOT_ID, true):addItemEx(LootBag)
end

function doSaveReward(uid, name)
	for GUID, items in pairs(REWARD_CHEST.LOOT[uid]) do
		local player = getPlayerByGUID(GUID)
		if player ~= nil then
			player:addReward(items, os.time())
			player:sendTextMessage(MESSAGE_INFO_DESCR, 'Your reward container from ' .. name .. ' was moved to your reward chest.')
		else
			db.query('INSERT INTO player_rewardchest VALUES (NULL, ' .. GUID ..', "' .. serialize(items) ..'", ' .. os.time() ..');')
		end
	end
	REWARD_CHEST.LOOT[uid] = nil
end

function Player.updateRewardChest(self)
	db.query('DELETE FROM player_rewardchest WHERE TIME_TO_SEC(TIMEDIFF(NOW(), FROM_UNIXTIME(date))) >= '.. REWARD_CHEST.EXPIRE ..';')
	local Query = db.storeQuery('SELECT id, reward, date FROM player_rewardchest WHERE player_id = '.. self:getGuid() ..';')
	if Query ~= false then
		repeat
			local rewardBag = unserialize(result.getDataString(Query, 'reward'))
			self:addReward(rewardBag, result.getDataInt(Query, 'date'), result.getDataInt(Query, 'id'))
		until not result.next(Query)
		result.free(Query)
	end
	local depotChest = self:getDepotChest(REWARD_CHEST.DEPOT_ID, true)
	for index = (depotChest:getSize() - 1), 0, -1 do
	    local container = depotChest:getItem(index)
	    if (container:getAttribute('text') ~= nil) and (tonumber(container:getAttribute('text')) + REWARD_CHEST.EXPIRE < os.time()) then
	    	container:remove()
	    end
	end
	return self:setExhaustion(REWARD_CHEST.STORAGE, REWARD_CHEST.EXHAUST)
end

function string.diff(self)
    local format = {
        {'day', self / 60 / 60 / 24},
        {'hour', self / 60 / 60 % 24},
        {'minute', self / 60 % 60},
        {'second', self % 60}
    }

    local out = {}
    for k, t in ipairs(format) do
        local v = math.floor(t[2])
        if(v > 0) then
            table.insert(out, (k < #format and (#out > 0 and ', ' or '') or ' and ') .. v .. ' ' .. t[1] .. (v ~= 1 and 's' or ''))
        end
    end
    local ret = table.concat(out)
    if ret:len() < 16 and ret:find('second') then
        local a, b = ret:find(' and ')
        ret = ret:sub(b+1)
    end
    return ret
end

function doBossReward(monster, corpse)
    REWARD_CHEST.LOOT[corpse:getUniqueId()] = {}
    corpse:setAttribute('aid', 21584)
    corpse:setAttribute('text', corpse:getUniqueId())
    for killer, damage in pairs(monster:getDamageMap()) do
    	local player, str = Player(killer), 'Loot of ' .. MonsterType(monster:getName()):getNameDescription() .. ': '
    	if player ~= nil then
    		local rewardBag = doCreateItemEx(REWARD_CHEST.CONTAINER)
    		if player:getStamina() > 840 then
        		local loot = MonsterType(monster:getName()):getBossReward(damage.total/monster:getMaxHealth())
        		if #loot > 0 then
        			addContainerItems(Container(rewardBag), loot)
        			REWARD_CHEST.LOOT[corpse:getUniqueId()][player:getGuid()] = loot
        			str = str .. Container(rewardBag):getContentDescription()
        		else
        			str = str .. 'nothing'
        		end
        	else
        		str = str .. 'nothing (due to low stamina)'
        	end
        	player:sendTextMessage(MESSAGE_INFO_DESCR, str .. '.')
        end
    end
    addEvent(doSaveReward, REWARD_CHEST.DECAY[monster:getName():lower()]*1000, corpse:getUniqueId(), MonsterType(monster:getName()):getNameDescription())
end

function onKill(creature, target)
    if (Monster(target) ~= nil) and isInArray(REWARD_CHEST.BOSSES, target:getName():lower()) then
    	local corpse = Item(doCreateItem(MonsterType(target:getName()):getCorpseId(), 1, target:getPosition()))
    	corpse:decay()
    	target:setDropLoot(false)
    	doBossReward(target, corpse)
    end
end