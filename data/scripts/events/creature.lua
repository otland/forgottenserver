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
