local creatureevent = CreatureEvent("GiantSpiderWyda")

function creatureevent.onDeath(creature, corpse, killer, mostDamageKiller, lastHitUnjustified, mostDamageUnjustified)
	creature:say("It seems this was just an illusion.", TALKTYPE_MONSTER_SAY)
	if mostdamagekiller:isPlayer() then
		mostdamagekiller:addAchievement("Someone's Bored")
	end
	return true
end

creatureevent:register()
