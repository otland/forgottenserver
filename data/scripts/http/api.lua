local reloadTypes = {
	["all"] = RELOAD_TYPE_ALL,

	["action"] = RELOAD_TYPE_ACTIONS,
	["actions"] = RELOAD_TYPE_ACTIONS,

	["chat"] = RELOAD_TYPE_CHAT,
	["channel"] = RELOAD_TYPE_CHAT,
	["chatchannels"] = RELOAD_TYPE_CHAT,

	["config"] = RELOAD_TYPE_CONFIG,
	["configuration"] = RELOAD_TYPE_CONFIG,

	["creaturescript"] = RELOAD_TYPE_CREATURESCRIPTS,
	["creaturescripts"] = RELOAD_TYPE_CREATURESCRIPTS,

	["events"] = RELOAD_TYPE_EVENTS,

	["global"] = RELOAD_TYPE_GLOBAL,

	["globalevent"] = RELOAD_TYPE_GLOBALEVENTS,
	["globalevents"] = RELOAD_TYPE_GLOBALEVENTS,

	["items"] = RELOAD_TYPE_ITEMS,

	["monster"] = RELOAD_TYPE_MONSTERS,
	["monsters"] = RELOAD_TYPE_MONSTERS,

	["mount"] = RELOAD_TYPE_MOUNTS,
	["mounts"] = RELOAD_TYPE_MOUNTS,

	["move"] = RELOAD_TYPE_MOVEMENTS,
	["movement"] = RELOAD_TYPE_MOVEMENTS,
	["movements"] = RELOAD_TYPE_MOVEMENTS,

	["npc"] = RELOAD_TYPE_NPCS,
	["npcs"] = RELOAD_TYPE_NPCS,

	["quest"] = RELOAD_TYPE_QUESTS,
	["quests"] = RELOAD_TYPE_QUESTS,

	["raid"] = RELOAD_TYPE_RAIDS,
	["raids"] = RELOAD_TYPE_RAIDS,

	["spell"] = RELOAD_TYPE_SPELLS,
	["spells"] =  RELOAD_TYPE_SPELLS,

	["talk"] = RELOAD_TYPE_TALKACTIONS,
	["talkaction"] = RELOAD_TYPE_TALKACTIONS,
	["talkactions"] = RELOAD_TYPE_TALKACTIONS,

	["weapon"] = RELOAD_TYPE_WEAPONS,
	["weapons"] = RELOAD_TYPE_WEAPONS,

	["scripts"] = RELOAD_TYPE_SCRIPTS,
	["libs"] = RELOAD_TYPE_GLOBAL
}

local api = GlobalEvent("API")

function api.onHttpRequest(name, data)
	
	-- reload
	if data.reload then
		local reloadType = reloadTypes[data.reload:lower()]
		if not reloadType then
			-- WIP
			-- http:sendCallbackMessage("API", "Reload type not found.")
			return
		end

		-- need to clear EventCallback.data or we end up having duplicated events on /reload scripts
		if table.contains({RELOAD_TYPE_SCRIPTS, RELOAD_TYPE_ALL}, reloadType) then
			EventCallback:clear()
		end

		Game.reload(reloadType)
		print(string.format("Reloaded %s.", data.reload:lower()))
		-- WIP
		-- http:sendCallbackMessage("API", string.format("Reloaded %s.", name:lower()))
	end

	-- server save
	if data.saveServer then
		saveServer()
	end

	-- clean server
	if data.cleanServer then
		cleanMap()
	end

	-- close server
	if data.closeServer then
		Game.setGameState(GAME_STATE_SHUTDOWN)
	end

	-- start raid
	if data.raid then
		local returnValue = Game.startRaid(data.raid)
		if returnValue ~= RETURNVALUE_NOERROR then
			print("Raid: ".. Game.getReturnMessage(returnValue))
		else
			print("Raid: ".. data.raid .." started.")
		end
	end

	return true
end

api:register()
