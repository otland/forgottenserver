local reloadTypes = {
	["all"] = { targetType = RELOAD_TYPE_ALL, name = "all" },

	["action"] = { targetType = RELOAD_TYPE_ACTIONS, name = "actions" },
	["actions"] = { targetType = RELOAD_TYPE_ACTIONS, name = "actions" },

	["chat"] = { targetType = RELOAD_TYPE_CHAT, name = "chatchannels" },
	["channel"] = { targetType = RELOAD_TYPE_CHAT, name = "chatchannels" },
	["chatchannels"] = { targetType = RELOAD_TYPE_CHAT, name = "chatchannels" },

	["config"] = { targetType = RELOAD_TYPE_CONFIG, name = "config" },
	["configuration"] = { targetType = RELOAD_TYPE_CONFIG, name = "config" },

	["creaturescript"] = { targetType = RELOAD_TYPE_CREATURESCRIPTS, name = "creature scripts" },
	["creaturescripts"] = { targetType = RELOAD_TYPE_CREATURESCRIPTS, name = "creature scripts" },

	["events"] = { targetType = RELOAD_TYPE_EVENTS, name = "events" },

	["global"] = { targetType = RELOAD_TYPE_GLOBAL, name = "global.lua" },

	["globalevent"] = { targetType = RELOAD_TYPE_GLOBALEVENTS, name = "globalevents" },
	["globalevents"] = { targetType = RELOAD_TYPE_GLOBALEVENTS, name = "globalevents" },

	["items"] = { targetType = RELOAD_TYPE_ITEMS, name = "items" },

	["monster"] = { targetType = RELOAD_TYPE_MONSTERS, name = "monsters" },
	["monsters"] = { targetType = RELOAD_TYPE_MONSTERS, name = "monsters" },

	["mount"] = { targetType = RELOAD_TYPE_MOUNTS, name = "mounts" },
	["mounts"] = { targetType = RELOAD_TYPE_MOUNTS, name = "mounts" },

	["move"] = { targetType = RELOAD_TYPE_MOVEMENTS, name = "movements" },
	["movement"] = { targetType = RELOAD_TYPE_MOVEMENTS, name = "movements" },
	["movements"] = { targetType = RELOAD_TYPE_MOVEMENTS, name = "movements" },

	["npc"] = { targetType = RELOAD_TYPE_NPCS, name = "npcs" },
	["npcs"] = { targetType = RELOAD_TYPE_NPCS, name = "npcs" },

	["quest"] = { targetType = RELOAD_TYPE_QUESTS, name = "quests" },
	["quests"] = { targetType = RELOAD_TYPE_QUESTS, name = "quests" },

	["raid"] = { targetType = RELOAD_TYPE_RAIDS, name = "raids" },
	["raids"] = { targetType = RELOAD_TYPE_RAIDS, name = "raids" },

	["spell"] = { targetType = RELOAD_TYPE_SPELLS, name = "spells" },
	["spells"] = { targetType = RELOAD_TYPE_SPELLS, name = "spells" },

	["talk"] = { targetType = RELOAD_TYPE_TALKACTIONS, name = "talk actions" },
	["talkaction"] = { targetType = RELOAD_TYPE_TALKACTIONS, name = "talk actions" },
	["talkactions"] = { targetType = RELOAD_TYPE_TALKACTIONS, name = "talk actions" },

	["weapon"] = { targetType = RELOAD_TYPE_WEAPONS, name = "weapons" },
	["weapons"] = { targetType = RELOAD_TYPE_WEAPONS, name = "weapons" }
}

function onSay(player, words, param)
	if not player:getGroup():getAccess() then
		return true
	end

	if player:getAccountType() < ACCOUNT_TYPE_GOD then
		return false
	end

	logCommand(player, words, param)

	local reloadType = reloadTypes[param and param:lower()]
	if not reloadType then
		player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, "Reload type not found.")
		return false
	end

	Game.reload(reloadType.targetType)
	player:sendTextMessage(MESSAGE_STATUS_CONSOLE_BLUE, string.format("Reloaded %s.", reloadType.name))
	return false
end
