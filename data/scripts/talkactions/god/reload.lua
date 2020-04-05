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

local reload = TalkAction("/reload")

function reload.onSay(player, words, param)
	if(not player:getGroup():getAccess()) or player:getAccountType() < ACCOUNT_TYPE_GOD then
		return true
	end

	if(param == "") then
		player:sendCancelMessage("Command param required.")
		return false
	end

	logCommand(player, words, param)

	local reloadType = reloadTypes[param:lower()]
	if not reloadType then
		player:sendCancelMessage("Reload type not found.")
		return false
	end

	Game.reload(reloadType)
	player:sendTextMessage(MESSAGE_INFO_DESCR, string.format("Reloaded %s.", param:lower()))
	return false
end

reload:separator(" ")
reload:register()
