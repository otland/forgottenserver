local creatureevent = CreatureEvent("ScarabDeath")

function creatureevent.onDeath(creature, corpse, killer, mostDamageKiller, lastHitUnjustified, mostDamageUnjustified)
	if math.random(100) < 10 then
		Game.createMonster("Scorpion", creature:getPosition())
		creature:say("Horestis' curse spawns a vengeful scorpion from the body!", TALKTYPE_MONSTER_SAY)
	end
	return true
end

creatureevent:register()
