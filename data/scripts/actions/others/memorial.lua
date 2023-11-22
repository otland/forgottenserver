local memorial = Action()

function memorial.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	player:showMemorialInfo()
	return true
end

memorial:id(34174, 34175, 34176, 34177, 34178, 34179)
memorial:register()
