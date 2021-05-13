local northemPike = CreatureEvent("NorthernPikeDeath")

function northemPike.onDeath(creature, corpse, killer, mostDamageKiller, lastHitUnjustified, mostDamageUnjustified)
	if math.random(100) < 11 then
		Game.createMonster("Slippery Northern Pike", creature:getPosition(), false, true)
	end
	return true
end

northemPike:register()
