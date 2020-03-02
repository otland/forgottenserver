local ferumbrasAmulet = Action()

function ferumbrasAmulet.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if math.random(2) == 1 then
		player:addMana(1000)
	else
		player:addHealth(1000)
	end
	item:transform(25424)
	item:getPosition():sendMagicEffect(CONST_ME_MAGIC_RED)
	item:decay()
	player:say("Magical sparks whirl around the amulet and you suddenly feel refreshed.", TALKTYPE_MONSTER_SAY)
	return true
end

ferumbrasAmulet:id(25423)
ferumbrasAmulet:register()
