if not globalBosses then
	globalBosses = {}
end

function Monster.setReward(self, enable)
	if enable then
		if not self:getType():isRewardBoss() then
			error("Rewards can only be enabled to rewards bosses.")
			return false
		end
		globalBosses[self:getId()] = {}
		self:registerEvent("BossDeath")    
		self:registerEvent("BossThink")    
	else
		globalBosses[self:getId()] = nil
		self:unregisterEvent("BossDeath")
		self:unregisterEvent("BossThink")
	end
	return true
end

local function pushValues(buffer, sep, ...)
	local argv = {...}
	local argc = #argv
	for k, v in ipairs(argv) do
		table.insert(buffer, v)
		if k < argc and sep then
			table.insert(buffer, sep)
		end
	end
end

function Item.getNameDescription(self)
	local subType = self:getSubType()
	local itemType = self:getType()

	local buffer = {}

	local name = self:getName() or ''
	if(#name ~= 0) then
		if(itemType:isStackable() and subType > 1) then
			pushValues(buffer, ' ', subType, self:getPluralName())
		else
			local article = self:getArticle() or ''
			pushValues(buffer, ' ', select(#article ~= 0 and 1 or 2, article, name))
		end
	else
		pushValues(buffer, ' ', 'an item of type', self:getId())
	end 

	return table.concat(buffer)
end

function Container.getContentDescription(self, outputBuffer)
	local firstItem = true
	local buffer = outputBuffer or {}
	for i = 1, self:getSize() do
		local item = self:getItem(i - 1)

		if(firstItem) then
			firstItem = false
		else
			table.insert(buffer, ", ")
		end

		table.insert(buffer, item:getNameDescription())
	end

	if firstItem then
		table.insert(buffer, "nothing")
	end

	if not outputBuffer then
		return table.concat(buffer)
	end
end

function Player.getRewardChest(self, autocreate)
	return self:getDepotChest(99, autocreate)
end

function Player.inBossFight(self)
	if not next(globalBosses) then
		return false
	end
	local playerGuid = self:getGuid()

	for _, info in pairs(globalBosses) do
		local stats = info[playerGuid]
		if stats and stats.active then
			return stats
		end
	end
	return false
end

-- by https://otland.net/members/cbrm.25752/ with some modifications
function MonsterType.createLootItem(self, lootBlock, chance, lootTable)
	local lootTable, itemCount = lootTable or {}, 0
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

function MonsterType.getBossReward(self, lootFactor, topScore)
	local result = {}
	if getConfigInfo("rateLoot") > 0 then
		for _, lootBlock in pairs(self:getLoot()) do
			if lootBlock.unique and not topScore then
				--continue
			else
				self:createLootItem(lootBlock, lootFactor, result)
			end
		end
	end
	return result
end
