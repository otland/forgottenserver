local callbacks = {}
local getInfo = {}
local updateableParameters = {}
local autoID = 0
local ec = setmetatable({}, {
	__newindex = function (self, n, v)
		autoID = autoID +1
		local name = string.format('EVENT_CALLBACK_%s', n:upper())
		_G[name] = autoID
		callbacks[n] = autoID
		local infos = {}
		local updates = {}
		for key, value in pairs(v) do
			if type(key) == "number" then
				updates[key] = value
			else
				infos[key] = value
			end
		end
		getInfo[autoID] = infos
		updateableParameters[autoID] = updates
		rawset(self, name, autoID)
	end
})

-- Creature
ec.onChangeOutfit = {} -- EVENT_CALLBACK_ONCHANGEOUTFIT
ec.onChangeMount = {} -- EVENT_CALLBACK_ONCHANGEMOUNT
ec.onAreaCombat = {returnValue=true} -- EVENT_CALLBACK_ONAREACOMBAT
ec.onTargetCombat = {returnValue=true} -- EVENT_CALLBACK_ONTARGETCOMBAT
ec.onHear = {} -- EVENT_CALLBACK_ONHEAR
-- Party
ec.onJoin = {} -- EVENT_CALLBACK_ONJOIN
ec.onLeave = {} -- EVENT_CALLBACK_ONLEAVE
ec.onDisband = {} -- EVENT_CALLBACK_ONDISBAND
ec.onShareExperience = {} -- EVENT_CALLBACK_ONSHAREEXPERIENCE
-- Player
ec.onBrowseField = {} -- EVENT_CALLBACK_ONBROWSEFIELD
ec.onLook = {[5]=1} -- EVENT_CALLBACK_ONLOOK
ec.onLookInBattleList = {[4]=1} -- EVENT_CALLBACK_ONLOOKINBATTLELIST
ec.onLookInTrade = {[5]=1} -- EVENT_CALLBACK_ONLOOKINTRADE
ec.onLookInShop = {[4]=1} -- EVENT_CALLBACK_ONLOOKINSHOP
ec.onTradeRequest = {} -- EVENT_CALLBACK_ONTRADEREQUEST
ec.onTradeAccept = {} -- EVENT_CALLBACK_ONTRADEACCEPT
ec.onTradeCompleted = {} -- EVENT_CALLBACK_ONTRADECOMPLETED
ec.onMoveItem = {} -- EVENT_CALLBACK_ONMOVEITEM
ec.onItemMoved = {} -- EVENT_CALLBACK_ONITEMMOVED
ec.onMoveCreature = {} -- EVENT_CALLBACK_ONMOVECREATURE
ec.onReportRuleViolation = {} -- EVENT_CALLBACK_ONREPORTRULEVIOLATION
ec.onReportBug = {} -- EVENT_CALLBACK_ONREPORTBUG
ec.onTurn = {} -- EVENT_CALLBACK_ONTURN
ec.onGainExperience = {[3]=1} -- EVENT_CALLBACK_ONGAINEXPERIENCE
ec.onLoseExperience = {[2]=1} -- EVENT_CALLBACK_ONLOSEEXPERIENCE
ec.onGainSkillTries = {[3]=1} -- EVENT_CALLBACK_ONGAINSKILLTRIES
ec.onWrapItem = {} -- EVENT_CALLBACK_ONWRAPITEM
-- Monster
ec.onDropLoot = {} -- EVENT_CALLBACK_ONDROPLOOT
ec.onSpawn = {} -- EVENT_CALLBACK_ONSPAWN
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
				if getInfo[eventCallback].returnValue then
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
			for index, value in pairs(updateableParameters[eventCallback]) do
				args[index] = results[value]
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
