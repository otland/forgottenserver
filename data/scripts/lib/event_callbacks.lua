local unpack = unpack
local pack = table.pack

local EventCallbackData, callbacks, updateableParameters, autoID = {}, {}, {}, 0
-- This metatable creates an auto-configuration mechanism to create new types of EventCallbacks
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
	EventCallbackData[autoID] = {maxn = 0}
	EVENT_CALLBACK_LAST = autoID
end})

--@ Definitions of valid EventCallback types to hook according to the given field name
--@ The fields within the assigned table, allow to save arbitrary information
-- Creature
ec.onChangeOutfit = {}
ec.onChangeMount = {}
ec.onAreaCombat = {returnValue=true}
ec.onTargetCombat = {returnValue=true}
ec.onHear = {}
-- Party
ec.onJoin = {}
ec.onLeave = {}
ec.onDisband = {}
ec.onShareExperience = {}
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
ec.onMoveItem = {}
ec.onItemMoved = {}
ec.onMoveCreature = {}
ec.onReportRuleViolation = {}
ec.onReportBug = {}
ec.onTurn = {}
ec.onGainExperience = {[3] = 1}
ec.onLoseExperience = {[2] = 1}
ec.onGainSkillTries = {[3] = 1}
ec.onWrapItem = {}
ec.onInventoryUpdate = {}
-- Monster
ec.onDropLoot = {}
ec.onSpawn = {}

EventCallback = {
	register = function(self, triggerIndex)
		if isScriptsInterface() then
			local eventType = rawget(self, 'eventType')
			local callback = rawget(self, 'callback')
			if not eventType or not callback then
				debugPrint("[Warning - EventCallback::register] need to setup a callback before you can register.")
				return
			end

			local eventData = EventCallbackData[eventType]
			eventData.maxn = #eventData + 1
			eventData[eventData.maxn] = {
				callback = callback,
				triggerIndex = tonumber(triggerIndex) or 0
			}
			table.sort(eventData, function(ecl, ecr) return ecl.triggerIndex < ecr.triggerIndex end)
			self.eventType = nil
			self.callback = nil
		end
	end,

	clear = function(self)
		EventCallbackData = {}
		for i = 1, EVENT_CALLBACK_LAST do
			EventCallbackData[i] = {maxn = 0}
		end
	end
}

setmetatable(EventCallback, {
	__newindex = function(self, key, callback)
		if not isScriptsInterface() then
			return
		end

		local eventType = callbacks[key]
		if not eventType then
			debugPrint(string.format("[Warning - EventCallback::%s] is not a valid callback.", key))
			return
		end

		if type(callback) ~= "function" then
			debugPrint(string.format("[Warning - EventCallback::%s] a function is expected.", key))
			return
		end

		rawset(self, 'eventType', eventType)
		rawset(self, 'callback', callback)
	end,

	__index = function(self, key)
		local callback = callbacks[key]
		if not callback then
			if not isScriptsInterface() then
				return
			end

			return rawget(self, key)
		end

		local eventData = EventCallbackData[callback]
		if eventData.maxn == 0 then
			return
		end

		return function(...)
			local results, args, info = {}, pack(...), callbacks[callback]
			for index = 1, eventData.maxn do
				local event = eventData[index]
				repeat
					results = {event.callback(unpack(args))}
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
					if index == eventData.maxn then
						return unpack(results)
					end
				until true
				-- Update the results for the next call
				for index, value in pairs(updateableParameters[callback]) do
					args[index] = results[value]
				end
			end
		end
	end
})
