local ferumbrasStaff = Action()

function ferumbrasStaff.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if math.random(100) > 20 then
		item:transform(25423)
	else
		item:transform(25422)
	end
	item:getPosition():sendMagicEffect(CONST_ME_MAGIC_RED)
	item:decay()
	player:say("Magical sparks whirl around the staff and suddenly it changes its appearance.", TALKTYPE_MONSTER_SAY)
	return true
end

ferumbrasStaff:id(25420)
ferumbrasStaff:register()
