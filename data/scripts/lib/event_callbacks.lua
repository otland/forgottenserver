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

local auxargs = {
	[EVENT_CALLBACK_ONGAINEXPERIENCE] = {[3] = 1},
	[EVENT_CALLBACK_ONLOSEEXPERIENCE] = {[2] = 1},
	[EVENT_CALLBACK_ONGAINSKILLTRIES] = {[3] = 1}
}

EventCallback = {
	__index = function (self, k)
		if isScriptsInterface() then
			return self
		else
			return nil
		end
	end,
	__newindex = function (ev, k, v)
		if isScriptsInterface() then
			local type = callbacks[k]
			if type then
				rawset(ev, "type", type)
				rawset(ev, "call", v)
			else
				return nil
			end
		end
	end,
	register = function (ev, index)
		if ev.registered then
			debugPrint("[EventCallback - Warning] The event was ignored, because it is already registered.")
			return nil
		end
		if ev.type then
			local doSort = type(index) == "number"
			if doSort then
				rawset(ev, 'index', index)
			end
			rawset(ev, 'registered', true)
			EventCallback.data[ev.type][#EventCallback.data[ev.type] + 1] = ev
			if doSort and #EventCallback.data[ev.type] > 1 then
				table.sort(EventCallback.data[ev.type], function (ea, eb) return ea.index < eb.index end)
			end
		end
	end,
	get = function (self, type, ...)
		local args = table.pack(...)
		local eventTable, ret = self.data[type]
		local events = #eventTable
		for k, ev in pairs(eventTable) do
			ret = {ev.call(unpack(args))}
			if k == events or (ret[1] ~= nil and (ret[1] == false or table.contains({EVENT_CALLBACK_ONAREACOMBAT, EVENT_CALLBACK_ONTARGETCOMBAT}, type) and ret[1] ~= RETURNVALUE_NOERROR)) then
				break
			end
			for k, v in pairs(auxargs[type] or {}) do
				args[k] = ret[v]
			end
		end
		return unpack(ret)
	end,
	clear = function (self)
		self.data = {}
		for i = 1, EVENT_CALLBACK_LAST do
			self.data[i] = {}
		end
	end,
	has = function (self, type)
		return #self.data[type] > 0
	end
}
EventCallback.__index = EventCallback

setmetatable(EventCallback, {
	__call = function (self)
		if isScriptsInterface() then
			local ev = {}
			ev.index = math.huge
			setmetatable(ev, EventCallback)
			return ev
		else
			return nil
		end
	end
})

-- can't be overwritten on reloads
if not EventCallback.data then
	EventCallback:clear()
end
