-- Creature
EVENT_CALLBACK_ONCHANGEOUTFIT = 1
EVENT_CALLBACK_ONCHANGEMOUNT = 2
EVENT_CALLBACK_ONAREACOMBAT = 3
EVENT_CALLBACK_ONTARGETCOMBAT = 4
EVENT_CALLBACK_ONHEAR = 5
-- Party
EVENT_CALLBACK_ONJOIN = 6
EVENT_CALLBACK_ONLEAVE = 7
EVENT_CALLBACK_ONDISBAND = 8
EVENT_CALLBACK_ONSHAREEXPERIENCE = 9
-- Player
EVENT_CALLBACK_ONBROWSEFIELD = 10
EVENT_CALLBACK_ONLOOK = 11
EVENT_CALLBACK_ONLOOKINBATTLELIST = 12
EVENT_CALLBACK_ONLOOKINTRADE = 13
EVENT_CALLBACK_ONLOOKINSHOP = 14
EVENT_CALLBACK_ONTRADEREQUEST = 15
EVENT_CALLBACK_ONTRADEACCEPT = 16
EVENT_CALLBACK_ONTRADECOMPLETED = 17
EVENT_CALLBACK_ONMOVEITEM = 18
EVENT_CALLBACK_ONITEMMOVED = 19
EVENT_CALLBACK_ONMOVECREATURE = 20
EVENT_CALLBACK_ONREPORTRULEVIOLATION = 21
EVENT_CALLBACK_ONREPORTBUG = 22
EVENT_CALLBACK_ONTURN = 23
EVENT_CALLBACK_ONGAINEXPERIENCE = 24
EVENT_CALLBACK_ONLOSEEXPERIENCE = 25
EVENT_CALLBACK_ONGAINSKILLTRIES = 26
EVENT_CALLBACK_ONWRAPITEM = 27
-- Monster
EVENT_CALLBACK_ONDROPLOOT = 28
EVENT_CALLBACK_ONSPAWN = 29
-- last (for correct table counting)
EVENT_CALLBACK_LAST = EVENT_CALLBACK_ONSPAWN

local callbacks = {
	-- Creature
	["onChangeOutfit"] = EVENT_CALLBACK_ONCHANGEOUTFIT,
	["onChangeMount"] = EVENT_CALLBACK_ONCHANGEMOUNT,
	["onAreaCombat"] = EVENT_CALLBACK_ONAREACOMBAT,
	["onTargetCombat"] = EVENT_CALLBACK_ONTARGETCOMBAT,
	["onHear"] = EVENT_CALLBACK_ONHEAR,
	-- Party
	["onJoin"] = EVENT_CALLBACK_ONJOIN,
	["onLeave"] = EVENT_CALLBACK_ONLEAVE,
	["onDisband"] = EVENT_CALLBACK_ONDISBAND,
	["onShareExperience"] = EVENT_CALLBACK_ONSHAREEXPERIENCE,
	-- Player
	["onBrowseField"] = EVENT_CALLBACK_ONBROWSEFIELD,
	["onLook"] = EVENT_CALLBACK_ONLOOK,
	["onLookInBattleList"] = EVENT_CALLBACK_ONLOOKINBATTLELIST,
	["onLookInTrade"] = EVENT_CALLBACK_ONLOOKINTRADE,
	["onLookInShop"] = EVENT_CALLBACK_ONLOOKINSHOP,
	["onTradeRequest"] = EVENT_CALLBACK_ONTRADEREQUEST,
	["onTradeAccept"] = EVENT_CALLBACK_ONTRADEACCEPT,
	["onTradeCompleted"] = EVENT_CALLBACK_ONTRADECOMPLETED,
	["onMoveItem"] = EVENT_CALLBACK_ONMOVEITEM,
	["onItemMoved"] = EVENT_CALLBACK_ONITEMMOVED,
	["onMoveCreature"] = EVENT_CALLBACK_ONMOVECREATURE,
	["onReportRuleViolation"] = EVENT_CALLBACK_ONREPORTRULEVIOLATION,
	["onReportBug"] = EVENT_CALLBACK_ONREPORTBUG,
	["onTurn"] = EVENT_CALLBACK_ONTURN,
	["onGainExperience"] = EVENT_CALLBACK_ONGAINEXPERIENCE,
	["onLoseExperience"] = EVENT_CALLBACK_ONLOSEEXPERIENCE,
	["onGainSkillTries"] = EVENT_CALLBACK_ONGAINSKILLTRIES,
	["onWrapItem"] = EVENT_CALLBACK_ONWRAPITEM,
	-- Monster
	["onDropLoot"] = EVENT_CALLBACK_ONDROPLOOT,
	["onSpawn"] = EVENT_CALLBACK_ONSPAWN
}

local updateableParameters = {
	[EVENT_CALLBACK_ONLOOK] = {[5] = 1},
	[EVENT_CALLBACK_ONLOOKINBATTLELIST] = {[4] = 1},
	[EVENT_CALLBACK_ONLOOKINTRADE] = {[5] = 1},
	[EVENT_CALLBACK_ONLOOKINSHOP] = {[4] = 1},
	[EVENT_CALLBACK_ONGAINEXPERIENCE] = {[3] = 1},
	[EVENT_CALLBACK_ONLOSEEXPERIENCE] = {[2] = 1},
	[EVENT_CALLBACK_ONGAINSKILLTRIES] = {[3] = 1}
}

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
				if table.contains({EVENT_CALLBACK_ONAREACOMBAT, EVENT_CALLBACK_ONTARGETCOMBAT}, eventCallback) then
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
