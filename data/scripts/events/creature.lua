function Creature:onChangeOutfit(outfit)
	if Event.onCreatureChangeMount and not Event.onCreatureChangeMount(self, outfit.lookMount) then
		return false
	end
	if Event.onCreatureChangeOutfit and not Event.onCreatureChangeOutfit(self, outfit) then
		return false
	end
	return true
end

function Creature:onAreaCombat(tile, isAggressive)
	if Event.onCreatureAreaCombat then
		return Event.onCreatureAreaCombat(self, tile, isAggressive)
	end
	return RETURNVALUE_NOERROR
end

function Creature:onTargetCombat(target)
	if Event.onCreatureTargetCombat then
		return Event.onCreatureTargetCombat(self, target)
	end
	return RETURNVALUE_NOERROR
end

function Creature:onHear(speaker, words, type)
	if Event.onCreatureHear then
		Event.onCreatureHear(self, speaker, words, type)
	end
end

function Creature:onChangeZone(fromZone, toZone)
	if Event.onCreatureChangeZone then
		Event.onCreatureChangeZone(self, fromZone, toZone)
	end
end

function Creature:onUpdateStorage(key, value, oldValue, isSpawn)
	if Event.onCreatureUpdateStorage then
		Event.onCreatureUpdateStorage(self, key, value, oldValue, isSpawn)
	end
end

function Creature:onChangeHealth(attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)
	if Event.onCreatureChangeHealth then
		return Event.onCreatureChangeHealth(self, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)
	end
	return primaryDamage, primaryType, secondaryDamage, secondaryType
end

function Creature:onChangeMana(attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)
	if Event.onCreatureChangeMana then
		return Event.onCreatureChangeMana(self, attacker, primaryDamage, primaryType, secondaryDamage, secondaryType, origin)
	end
	return primaryDamage, primaryType, secondaryDamage, secondaryType
end

function Creature:onThink(interval)
	if Event.onCreatureThink then
		Event.onCreatureThink(self, interval)
	end
end

function Creature:onPrepareDeath(killer)
	if Event.onCreaturePrepareDeath then
		return Event.onCreaturePrepareDeath(self, killer)
	end
	return true
end

function Creature:onDeath(corpse, killer, mostDamageKiller, lastHitUnjustified, mostDamageUnjustified)
	if Event.onCreatureDeath then
		Event.onCreatureDeath(self, corpse, killer, mostDamageKiller, lastHitUnjustified, mostDamageUnjustified)
	end
end

function Creature:onKill(target)
	if Event.onCreatureKill then
		Event.onCreatureKill(self, target)
	end
end
