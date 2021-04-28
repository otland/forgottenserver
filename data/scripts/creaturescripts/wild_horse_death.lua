local wildHorse = CreatureEvent("WildHorseDeath")

function wildHorse.onDeath(creature, corpse, killer, mostDamageKiller, lastHitUnjustified, mostDamageUnjustified)
	creature:say("With its last strength the horse runs to safety.", TALKTYPE_MONSTER_SAY)
	return true
end

wildHorse:register()
