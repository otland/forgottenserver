-- Creature
EVENT_CALLBACK_ONCHANGEOUTFIT = 1
EVENT_CALLBACK_ONAREACOMBAT = 2
EVENT_CALLBACK_ONTARGETCOMBAT = 3
EVENT_CALLBACK_ONHEAR = 4
-- Party
EVENT_CALLBACK_ONJOIN = 5
EVENT_CALLBACK_ONLEAVE = 6
EVENT_CALLBACK_ONDISBAND = 7
EVENT_CALLBACK_ONSHAREEXPERIENCE = 8
-- Player
EVENT_CALLBACK_ONBROWSEFIELD = 9
EVENT_CALLBACK_ONLOOK = 10
EVENT_CALLBACK_ONLOOKINBATTLELIST = 11
EVENT_CALLBACK_ONLOOKINTRADE = 12
EVENT_CALLBACK_ONLOOKINSHOP = 13
EVENT_CALLBACK_ONTRADEREQUEST = 14
EVENT_CALLBACK_ONTRADEACCEPT = 15
EVENT_CALLBACK_ONMOVEITEM = 16
EVENT_CALLBACK_ONITEMMOVED = 17
EVENT_CALLBACK_ONMOVECREATURE = 18
EVENT_CALLBACK_ONREPORTRULEVIOLATION = 19
EVENT_CALLBACK_ONREPORTBUG = 20
EVENT_CALLBACK_ONTURN = 21
EVENT_CALLBACK_ONGAINEXPERIENCE = 22
EVENT_CALLBACK_ONLOSEEXPERIENCE = 23
EVENT_CALLBACK_ONGAINSKILLTRIES = 24
EVENT_CALLBACK_ONWRAPITEM = 25
-- Monster
EVENT_CALLBACK_ONDROPLOOT = 26
EVENT_CALLBACK_ONSPAWN = 27
-- last (for correct table counting)
EVENT_CALLBACK_LAST = EVENT_CALLBACK_ONSPAWN

local callbacks = {
	-- Creature
	["onChangeOutfit"] = EVENT_CALLBACK_ONCHANGEOUTFIT,
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

-- can't be overwritten on /reload global/libs now
if not EventCallbackData then
	EventCallbackData = {}
	for i = 1, EVENT_CALLBACK_LAST do
		EventCallbackData[i] = {}
	end
end
EventCallback = {}
setmetatable(EventCallback,
{
	__index =
	function(self)
		if isScriptsInterface() then
			return self
		else
			return nil
		end
	end,

	__newindex = 
	function(self, key, value)
		if isScriptsInterface() then
			if self[key] then
				local ecd = EventCallbackData
				ecd[callbacks[key]][#ecd[callbacks[key]] + 1] = value
			end
		else
			return nil
		end
	end,

	__call =
	function(self, callbackType, ...)
		local args = {select(2, ...)}
		local obj = (select(1, ...))
		for _, func in pairs(EventCallbackData[callbackType]) do
			args = {func(obj, unpack(args))}
			if #args == 1 and args[1] == false then
				return false
			end
			-- handling for RETURNVALUE type of Events
			if isInArray({EVENT_CALLBACK_ONAREACOMBAT, EVENT_CALLBACK_ONTARGETCOMBAT}, callbackType) then
				if args[1] ~= RETURNVALUE_NOERROR then
					return unpack(args)
				end
			end
		end
		if args[1] == nil then
			return true
		end
		return unpack(args)
	end
})

function hasEventCallback(callbackType)
	if #EventCallbackData[callbackType] == 0 then
		return false
	end
	return true
end

