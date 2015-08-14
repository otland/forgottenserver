local reloadTypes = {
	["action"] = { targetType = RELOAD_TYPE_ACTIONS, name = "actions"},
	["actions"] = { targetType = RELOAD_TYPE_ACTIONS, name = "actions"},

	["config"] = { targetType = RELOAD_TYPE_CONFIG, name = "config"},
	["configuration"] = { targetType = RELOAD_TYPE_CONFIG, name = "config"},

	["creaturescript"] = { targetType = RELOAD_TYPE_CREATURESCRIPTS, name = "creature scripts"},
	["creaturescripts"] = { targetType = RELOAD_TYPE_CREATURESCRIPTS, name = "creature scripts"},

	["monster"] = { targetType = RELOAD_TYPE_MONSTERS, name = "monsters"},
	["monsters"] = { targetType = RELOAD_TYPE_MONSTERS, name = "monsters"},

	["move"] = { targetType = RELOAD_TYPE_MOVEMENTS, name = "movements"},
	["movement"] = { targetType = RELOAD_TYPE_MOVEMENTS, name = "movements"},
	["movements"] = { targetType = RELOAD_TYPE_MOVEMENTS, name = "movements"},

	["npc"] = { targetType = RELOAD_TYPE_NPCS, name = "npcs"},
	["npcs"] = { targetType = RELOAD_TYPE_NPCS, name = "npcs"},

	["raid"] = { targetType = RELOAD_TYPE_RAIDS, name = "raids"},
	["raids"] = { targetType = RELOAD_TYPE_RAIDS, name = "raids"},

	["spell"] = { targetType = RELOAD_TYPE_SPELLS, name = "spells"},
	["spells"] = { targetType = RELOAD_TYPE_SPELLS, name = "spells"},

	["talk"] = { targetType = RELOAD_TYPE_TALKCTIONS, name = "talk actions"},
	["talkaction"] = { targetType = RELOAD_TYPE_TALKCTIONS, name = "talk actions"},
	["talkactions"] = { targetType = RELOAD_TYPE_TALKCTIONS, name = "talk actions"},

	["items"] = { targetType = RELOAD_TYPE_ITEMS, name = "items"},

	["weapon"] = { targetType = RELOAD_TYPE_WEAPONS, name = "weapons"},
	["weapons"] = { targetType = RELOAD_TYPE_WEAPONS, name = "weapons"},

	["quest"] = { targetType = RELOAD_TYPE_QUESTS, name = "quests"},
	["quests"] = { targetType = RELOAD_TYPE_QUESTS, name = "quests"},

	["mount"] = { targetType = RELOAD_TYPE_MOUNTS, name = "mounts"},
	["mounts"] = { targetType = RELOAD_TYPE_MOUNTS, name = "mounts"},

	["globalevent"] = { targetType = RELOAD_TYPE_GLOBALEVENTS, name = "globalevents"},
	["globalevents"] = { targetType = RELOAD_TYPE_GLOBALEVENTS, name = "globalevents"},

	["events"] = { targetType = RELOAD_TYPE_EVENTS, name = "events"},

	["chat"] = { targetType = RELOAD_TYPE_CHAT, name = "chatchannels"},
	["channel"] = { targetType = RELOAD_TYPE_CHAT, name = "chatchannels"},
	["chatchannels"] = { targetType = RELOAD_TYPE_CHAT, name = "chatchannels"},

	["global"] = { targetType = RELOAD_TYPE_GLOBAL, name = "global.lua"},

	["command"] = { targetType = RELOAD_TYPE_COMMANDS, name = "commands"},
	["commands"] = { targetType = RELOAD_TYPE_COMMANDS, name = "commands"}
}

function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	if player:getAccountType() < ACCOUNT_TYPE_GAMEMASTER then
		return false
	end

	local reloadType = reloadTypes[param and param:lower()]
	if not reloadType then
		player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Reload type not found.")
		return false
	end

	Game.reload(reloadType.targetType)
	player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, string.format("Reloaded %s.", reloadType.name))
	return true
end
