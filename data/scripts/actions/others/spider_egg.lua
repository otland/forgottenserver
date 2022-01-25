local spiderEgg = Action()

function spiderEgg.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local chance = math.random(100)
	if chance >= 50 and chance < 83 then
		Game.createMonster("Spider", fromPosition)
	elseif chance >= 83 and chance < 97 then
		Game.createMonster("Poison Spider", fromPosition)
	elseif chance >= 97 and chance < 100 then
		Game.createMonster("Tarantula", fromPosition)
	else
		item:getPosition():sendMagicEffect(CONST_ME_POFF)
	end
	item:transform(7536)
	item:decay()
	return true
end

spiderEgg:id(7537)
spiderEgg:register()
