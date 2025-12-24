function Creature:onChangeOutfit(outfit)
	if Event.onChangeMount and not Event.onChangeMount(self, outfit.lookMount) then
		return false
	end
	if Event.onChangeOutfit and not Event.onChangeOutfit(self, outfit) then
		return false
	end
	return true
end

function Creature:onAreaCombat(tile, isAggressive)
	if Event.onAreaCombat then
		return Event.onAreaCombat(self, tile, isAggressive)
	end
	return RETURNVALUE_NOERROR
end

function Creature:onTargetCombat(target)
	if Event.onTargetCombat then
		return Event.onTargetCombat(self, target)
	end
	return RETURNVALUE_NOERROR
end

function Creature:onHear(speaker, words, type)
	if Event.onHear then
		Event.onHear(self, speaker, words, type)
	end
end

function Creature:onChangeZone(fromZone, toZone)
	if Event.onChangeZone then
		Event.onChangeZone(self, fromZone, toZone)
	end
end

function Creature:onUpdateStorage(key, value, oldValue, isSpawn)
	if Event.onUpdateStorage then
		Event.onUpdateStorage(self, key, value, oldValue, isSpawn)
	end
end
