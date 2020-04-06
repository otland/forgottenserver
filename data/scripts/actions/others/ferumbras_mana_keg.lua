local ferumbrasManaKeg = Action()

function ferumbrasManaKeg.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	player:addItem(238, 10)
	item:transform(22770)
	item:getPosition():sendMagicEffect(CONST_ME_MAGIC_RED)
	item:decay()
	player:say("Magical sparks whirl around the keg as you open the spigot and you fill ten empty vials with mana fluid.", TALKTYPE_MONSTER_SAY)
	return true
end

ferumbrasManaKeg:id(22769)
ferumbrasManaKeg:register()
