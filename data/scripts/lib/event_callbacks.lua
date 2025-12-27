local unpack = unpack
local pack = table.pack

local EventData, callbacks, updateableParameters, autoID = {}, {}, {}, 0
-- This metatable creates an auto-configuration mechanism to create new types of Events
local ec = setmetatable({}, { __newindex = function(self, key, value)
	autoID = autoID + 1
	callbacks[key] = autoID
	local info, update = {}, {}
	for k, v in pairs(value) do
		if type(k) == "string" then
			info[k] = v
		else
			update[k] = v
		end
	end
	updateableParameters[autoID] = update
	callbacks[autoID] = info
	EventData[autoID] = {maxn = 0}
end})

--@ Definitions of valid Event types to hook according to the given field name
--@ The fields within the assigned table, allow to save arbitrary information
-- Creature
ec.onCreatureChangeOutfit = {}
ec.onCreatureChangeMount = {}
ec.onCreatureAreaCombat = {returnValue=true}
ec.onCreatureTargetCombat = {returnValue=true}
ec.onCreatureHear = {}
ec.onCreatureChangeZone = {}
ec.onCreatureUpdateStorage = {}
ec.onCreatureChangeHealth = {}
ec.onCreatureChangeMana = {}
ec.onCreatureThink = {}
ec.onCreaturePrepareDeath = {}
ec.onCreatureDeath = {}
ec.onCreatureKill = {}
-- Party
ec.onPartyJoin = {}
ec.onPartyLeave = {}
ec.onPartyDisband = {}
ec.onPartyShareExperience = {}
ec.onPartyInvite = {}
ec.onPartyRevokeInvitation = {}
ec.onPartyPassLeadership = {}
-- Player
ec.onPlayerBrowseField = {}
ec.onPlayerLook = {}
ec.onPlayerLookInBattleList = {}
ec.onPlayerLookInTrade = {}
ec.onPlayerLookInShop = {}
ec.onPlayerLookInMarket = {}
ec.onPlayerTradeRequest = {}
ec.onPlayerTradeAccept = {}
ec.onPlayerTradeCompleted = {}
ec.onPlayerMoveItem = {returnValue=true}
ec.onPlayerItemMoved = {}
ec.onPlayerMoveCreature = {}
ec.onPlayerReportRuleViolation = {}
ec.onPlayerRotateItem = {}
ec.onPlayerTurn = {}
ec.onPlayerGainExperience = {[3] = 1}
ec.onPlayerLoseExperience = {[2] = 1}
ec.onPlayerGainSkillTries = {[3] = 1}
ec.onPlayerWrapItem = {}
ec.onPlayerInventoryUpdate = {}
ec.onPlayerSpellCheck = {}
ec.onPlayerLogin = {}
ec.onPlayerJoin = {}
ec.onPlayerLogout = {}
ec.onPlayerReconnect = {}
ec.onPlayerAdvance = {}
ec.onPlayerModalWindow = {}
ec.onPlayerTextEdit = {}
ec.onPlayerExtendedOpcode = {}
-- Monster
ec.onMonsterDropLoot = {}
ec.onMonsterSpawn = {}

local EventMeta = {
	__newindex = function(self, key, callback)
		if not isScriptsInterface() then
			return
		end

		local eventType = callbacks[key]
		if not eventType then
			debugPrint(string.format("[Warning - Event::%s] is not a valid callback.", key))
			return
		end

		if type(callback) ~= "function" then
			debugPrint(string.format("[Warning - Event::%s] a function is expected.", key))
			return
		end

		rawset(self, 'eventType', eventType)
		rawset(self, 'callback', callback)
	end
}

local function register(self, triggerIndex)
	if not isScriptsInterface() then
		return
	end

	local eventType = rawget(self, 'eventType')
	local callback = rawget(self, 'callback')
	if not eventType or not callback then
		debugPrint("[Warning - Event::register] need to setup a callback before you can register.")
		return false
	end

	local events = EventData[eventType]
	events.maxn = #events + 1
	events[events.maxn] = {
		callback = callback,
		triggerIndex = tonumber(triggerIndex) or 0
	}

	table.sort(events, function(ecl, ecr) return ecl.triggerIndex < ecr.triggerIndex end)
	self.eventType = nil
	self.callback = nil
	return true
end

Event = setmetatable({
	clear = function(self)
		EventData = {}
		for i = 1, autoID do
			EventData[i] = {maxn = 0}
		end
	end
}, {
	__call = function(self)
		return setmetatable({register = register}, EventMeta)
	end,

	__index = function(self, key)
		local callback = callbacks[key]
		if not callback then
			return
		end

		local events = EventData[callback]
		local eventsCount = events.maxn
		if eventsCount == 0 then
			return
		end

		local updateableParams = updateableParameters[callback]
		return function(...)
			local results, args, info = {}, pack(...), callbacks[callback]
			for index = 1, eventsCount do
				repeat
					results = {events[index].callback(unpack(args))}
					local output = results[1]
					-- If the call returns nil then we continue with the next call
					if output == nil then
						break
					end
					-- If the call returns false then we exit the loop
					if not output then
						return false
					end
					-- If the call of type returnvalue returns noerror then we continue the loop
					if info.returnValue then
						if output == RETURNVALUE_NOERROR then
							break
						end
						return output
					end
					-- We left the loop why have we reached the end
					if index == eventsCount then
						return unpack(results)
					end
				until true
				-- Update the results for the next call
				for i, value in pairs(updateableParams) do
					args[i] = results[value]
				end
			end
		end
	end
})

-- For compatibility with the previous version.
EventCallback = Event()
