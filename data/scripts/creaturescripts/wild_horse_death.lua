local creatureevent = CreatureEvent("WildHorseDeath")

function creatureevent.onDeath(creature, corpse, lasthitkiller, mostdamagekiller, lasthitunjustified, mostdamageunjustified)
	creature:say("With its last strength the horse runs to safety.", TALKTYPE_MONSTER_SAY)
	return true
end

creatureevent:register()
