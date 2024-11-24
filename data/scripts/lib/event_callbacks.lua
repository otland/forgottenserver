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
ec.onChangeOutfit = {}
ec.onChangeMount = {}
ec.onAreaCombat = {returnValue=true}
ec.onTargetCombat = {returnValue=true}
ec.onHear = {}
ec.onChangeZone = {}
ec.onUpdateStorage = {}
-- Party
ec.onJoin = {}
ec.onLeave = {}
ec.onDisband = {}
ec.onShareExperience = {}
ec.onInvite = {}
ec.onRevokeInvitation = {}
ec.onPassLeadership = {}
-- Player
ec.onBrowseField = {}
ec.onLook = {[5] = 1}
ec.onLookInBattleList = {[4] = 1}
ec.onLookInTrade = {[5] = 1}
ec.onLookInShop = {[4] = 1}
ec.onLookInMarket = {}
ec.onTradeRequest = {}
ec.onTradeAccept = {}
ec.onTradeCompleted = {}
ec.onMoveItem = {returnValue=true}
ec.onItemMoved = {}
ec.onMoveCreature = {}
ec.onReportRuleViolation = {}
ec.onReportBug = {}
ec.onRotateItem = {}
ec.onTurn = {}
ec.onGainExperience = {[3] = 1}
ec.onLoseExperience = {[2] = 1}
ec.onGainSkillTries = {[3] = 1}
ec.onWrapItem = {}
ec.onInventoryUpdate = {}
ec.onSpellCheck = {}
-- Monster
ec.onDropLoot = {}
ec.onSpawn = {}

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

hasEvent = setmetatable({}, {
	__index = function(self, key)
		local callback = callbacks[key]
		if callback then
			return EventData[callback].maxn > 0
		end
	end
})

-- For compatibility with the previous version.
EventCallback = Event()
