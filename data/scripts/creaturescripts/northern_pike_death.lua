local creatureevent = CreatureEvent("NorthernPikeDeath")

function creatureevent.onDeath(creature, corpse, lasthitkiller, mostdamagekiller, lasthitunjustified, mostdamageunjustified)
	if math.random(100) < 11 then
		Game.createMonster("Slippery Northern Pike", creature:getPosition(), false, true)
	end
	return true
end

creatureevent:register()
