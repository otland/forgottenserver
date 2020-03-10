local ferumbrasManaKeg = Action()

function ferumbrasManaKeg.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	player:addItem(7590, 10)
	item:transform(25426)
	item:getPosition():sendMagicEffect(CONST_ME_MAGIC_RED)
	item:decay()
	player:say("Magical sparks whirl around the keg as you open the spigot and you fill ten empty vials with mana fluid.", TALKTYPE_MONSTER_SAY)
	return true
end

ferumbrasManaKeg:id(25425)
ferumbrasManaKeg:register()
