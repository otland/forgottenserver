-- registerEnum mechanism
local callbacks, updateableParameters, isReturnValue, autoID = {}, {}, {}, 0
local registerEnum = setmetatable({}, {
	__newindex = function (self, globalVariableName, enum)
		autoID = autoID +1
		_G[globalVariableName] = autoID
		callbacks[enum[1]] = autoID
		isReturnValue[autoID] = enum.returnValue
		if enum.update then
			updateableParameters[autoID] = enum.update
		end
	end
})

-- update: this makes the system update the parameter [n] with the result in the given index, example: [3]=1
-- returnValue: this determines that the value returned by the function is a RETURNVALUE_...

-- Creature
registerEnum.EVENT_CALLBACK_ONCHANGEOUTFIT = { "onChangeOutfit" }
registerEnum.EVENT_CALLBACK_ONCHANGEMOUNT = { "onChangeMount" }
registerEnum.EVENT_CALLBACK_ONAREACOMBAT = { "onAreaCombat", returnValue=true }
registerEnum.EVENT_CALLBACK_ONTARGETCOMBAT = { "onTargetCombat", returnValue=true }
registerEnum.EVENT_CALLBACK_ONHEAR = { "onHear" }
-- Party
registerEnum.EVENT_CALLBACK_ONJOIN = { "onJoin" }
registerEnum.EVENT_CALLBACK_ONLEAVE = { "onLeave" }
registerEnum.EVENT_CALLBACK_ONDISBAND = { "onDisband" }
registerEnum.EVENT_CALLBACK_ONSHAREEXPERIENCE = { "onShareExperience" }
-- Player
registerEnum.EVENT_CALLBACK_ONBROWSEFIELD = { "onBrowseField" }
registerEnum.EVENT_CALLBACK_ONLOOK = { "onLook", update={[5]=1} }
registerEnum.EVENT_CALLBACK_ONLOOKINBATTLELIST = { "onLookInBattleList", update={[4]=1} }
registerEnum.EVENT_CALLBACK_ONLOOKINTRADE = { "onLookInTrade", update={[5]=1} }
registerEnum.EVENT_CALLBACK_ONLOOKINSHOP = { "onLookInShop", update={[4]=1} }
registerEnum.EVENT_CALLBACK_ONTRADEREQUEST = { "onTradeRequest" }
registerEnum.EVENT_CALLBACK_ONTRADEACCEPT = { "onTradeAccept" }
registerEnum.EVENT_CALLBACK_ONTRADECOMPLETED = { "onTradeCompleted" }
registerEnum.EVENT_CALLBACK_ONMOVEITEM = { "onMoveItem" }
registerEnum.EVENT_CALLBACK_ONITEMMOVED = { "onItemMoved" }
registerEnum.EVENT_CALLBACK_ONMOVECREATURE = { "onMoveCreature" }
registerEnum.EVENT_CALLBACK_ONREPORTRULEVIOLATION = { "onReportRuleViolation" }
registerEnum.EVENT_CALLBACK_ONREPORTBUG = { "onReportBug" }
registerEnum.EVENT_CALLBACK_ONTURN = { "onTurn" }
registerEnum.EVENT_CALLBACK_ONGAINEXPERIENCE = { "onGainExperience", update={[3]=1} }
registerEnum.EVENT_CALLBACK_ONLOSEEXPERIENCE = { "onLoseExperience", update={[2]=1} }
registerEnum.EVENT_CALLBACK_ONGAINSKILLTRIES = { "onGainSkillTries", update={[3]=1} }
registerEnum.EVENT_CALLBACK_ONWRAPITEM = { "onWrapItem" }
-- Monster
registerEnum.EVENT_CALLBACK_ONDROPLOOT = { "onDropLoot" }
registerEnum.EVENT_CALLBACK_ONSPAWN = { "onSpawn" }
-- last (for correct table counting)
EVENT_CALLBACK_LAST = EVENT_CALLBACK_ONSPAWN

EventCallbackData = {}
hasEventCallback = function (type)
	return #EventCallbackData[type] > 0
end

EventCallback = {
	register = function (self, triggerIndex)
		if isScriptsInterface() then
			local eventType = rawget(self, 'eventType')
			local callback = rawget(self, 'callback')
			if not eventType or not callback then
				debugPrint("[Warning - EventCallback::register] need to setup a callback before you can register.")
				return
			end

			local eventData = EventCallbackData[eventType]
			table.insert(eventData, {
				callback = callback,
				triggerIndex = tonumber(triggerIndex) or 0
			})
			table.sort(eventData, function (ecl, ecr) return ecl.triggerIndex < ecr.triggerIndex end)
			rawset(self, 'eventType', nil)
			rawset(self, 'callback', nil)
			return true
		end
	end,

	clear = function (self)
		EventCallbackData = {}
		for i = 1, EVENT_CALLBACK_LAST do
			EventCallbackData[i] = {}
		end
		return true
	end
}

setmetatable(EventCallback, {
	__index = function (self, key)
		return nil
	end,

	__call = function (self, eventCallback, ...)
		local eventData = EventCallbackData[eventCallback]
		local results = {}
		local eventDataCount = #eventData
		local args = table.pack(...)
		for index, event in pairs(eventData) do
			repeat
				results = {event.callback(unpack(args))}
				local output = results[1]
				-- If the call returns nil then we continue with the next call
				if output == nil then
					break
				end
				-- If the call returns false then we exit the loop
				if output == false then
					return false
				end
				-- If the call of type returnvalue returns noerror then we continue the loop
				if isReturnValue[eventCallback] then
					if output == RETURNVALUE_NOERROR then
						break
					end
					
					return output
				end
				-- We left the loop why have we reached the end
				if index == eventDataCount then
					return unpack(results)
				end
			until true

			-- Update the results for the next call
			local parameters = updateableParameters[eventCallback]
			if parameters then
				for index, value in pairs(parameters) do
					args[index] = results[value]
				end
			end
		end
	end,

	__newindex = function (self, key, callback)
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
	end
})

-- can't be overwritten on reloads
EventCallback:clear()
