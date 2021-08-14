-- Creature
EVENT_CALLBACK_ONCHANGEOUTFIT = 1
EVENT_CALLBACK_ONCHANGEMOUNT = 2
EVENT_CALLBACK_ONAREACOMBAT = 3
EVENT_CALLBACK_ONTARGETCOMBAT = 4
EVENT_CALLBACK_ONHEAR = 5
EVENT_CALLBACK_ONSPAWN = 6
-- Party
EVENT_CALLBACK_ONJOIN = 7
EVENT_CALLBACK_ONLEAVE = 8
EVENT_CALLBACK_ONDISBAND = 9
EVENT_CALLBACK_ONSHAREEXPERIENCE = 10
-- Player
EVENT_CALLBACK_ONBROWSEFIELD = 11
EVENT_CALLBACK_ONLOOK = 12
EVENT_CALLBACK_ONLOOKINBATTLELIST = 13
EVENT_CALLBACK_ONLOOKINTRADE = 14
EVENT_CALLBACK_ONLOOKINSHOP = 15
EVENT_CALLBACK_ONTRADEREQUEST = 16
EVENT_CALLBACK_ONTRADEACCEPT = 17
EVENT_CALLBACK_ONTRADECOMPLETED = 18
EVENT_CALLBACK_ONMOVEITEM = 19
EVENT_CALLBACK_ONITEMMOVED = 20
EVENT_CALLBACK_ONMOVECREATURE = 21
EVENT_CALLBACK_ONREPORTRULEVIOLATION = 22
EVENT_CALLBACK_ONREPORTBUG = 23
EVENT_CALLBACK_ONTURN = 24
EVENT_CALLBACK_ONGAINEXPERIENCE = 25
EVENT_CALLBACK_ONLOSEEXPERIENCE = 26
EVENT_CALLBACK_ONGAINSKILLTRIES = 27
EVENT_CALLBACK_ONWRAPITEM = 28
-- Monster
EVENT_CALLBACK_ONDROPLOOT = 29
-- last (for correct table counting)
EVENT_CALLBACK_LAST = EVENT_CALLBACK_ONDROPLOOT

local callbacks = {
	-- Creature
	["onChangeOutfit"] = EVENT_CALLBACK_ONCHANGEOUTFIT,
	["onChangeMount"] = EVENT_CALLBACK_ONCHANGEMOUNT,
	["onAreaCombat"] = EVENT_CALLBACK_ONAREACOMBAT,
	["onTargetCombat"] = EVENT_CALLBACK_ONTARGETCOMBAT,
	["onHear"] = EVENT_CALLBACK_ONHEAR,
	["onSpawn"] = EVENT_CALLBACK_ONSPAWN,
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
	["onDropLoot"] = EVENT_CALLBACK_ONDROPLOOT
}

local auxargs = {
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
	register = function (self, index)
		if isScriptsInterface() then
			local type, call = rawget(self, "type"), rawget(self, "call")
			if type and call then
				EventCallbackData[type][#EventCallbackData[type] + 1] = {call, tonumber(index) or 0}
				table.sort(EventCallbackData[type], function (a, b) return a[2] < b[2] end)
				return rawset(self, "type", nil) and rawset(self, "call", nil)
			end
			debugPrint("[Warning - EventCallback::register] is need to set up a callback before register.")
		end
	end,
	clear = function (self)
		EventCallbackData = {}
		for i = 1, EVENT_CALLBACK_LAST do
			EventCallbackData[i] = {}
		end
	end
}

setmetatable(EventCallback, {
	__index = function (self) return self end,
	__newindex = function (self, k, v)
		if isScriptsInterface() then
			local ecType = callbacks[k]
			if ecType then
				if type(v) == "function" then
					return rawset(self, "type", ecType) and rawset(self, "call", v)
				end
				debugPrint(string.format("[Warning - EventCallback::%s] a function is expected.", k))
			else
				debugPrint(string.format("[Warning - EventCallback::%s] is not a valid callback.", k))
			end
		end
	end,
	__call = function (self, type, ...)
		local eventTable, ret = EventCallbackData[type]
		local args, events = table.pack(...), #eventTable
		for k, ev in pairs(eventTable) do
			ret = {ev[1](unpack(args))}
			if k == events or (ret[1] ~= nil and (ret[1] == false or table.contains({EVENT_CALLBACK_ONAREACOMBAT, EVENT_CALLBACK_ONTARGETCOMBAT}, type) and ret[1] ~= RETURNVALUE_NOERROR)) then
				return unpack(ret)
			end
			for k, v in pairs(auxargs[type] or {}) do args[k] = ret[v] end
		end
	end
	})

-- can't be overwritten on reloads
EventCallback:clear()
