function Creature:onChangeOutfit(outfit)
	if hasEvent.onChangeMount then
		if not Event.onChangeMount(self, outfit.lookMount) then
			return false
		end
	end
	if hasEvent.onChangeOutfit then
		return Event.onChangeOutfit(self, outfit)
	end
	return true
end

function Creature:onAreaCombat(tile, isAggressive)
	if hasEvent.onAreaCombat then
		return Event.onAreaCombat(self, tile, isAggressive)
	end
	return RETURNVALUE_NOERROR
end

function Creature:onTargetCombat(target)
	if hasEvent.onTargetCombat then
		return Event.onTargetCombat(self, target)
	end
	return RETURNVALUE_NOERROR
end

function Creature:onHear(speaker, words, type)
	if hasEvent.onHear then
		Event.onHear(self, speaker, words, type)
	end
end

function Creature:onChangeZone(fromZone, toZone)
	if hasEvent.onChangeZone then
		Event.onChangeZone(self, fromZone, toZone)
	end
end

function Creature:onUpdateStorage(key, value, oldValue, isSpawn)
	if hasEvent.onUpdateStorage then
		Event.onUpdateStorage(self, key, value, oldValue, isSpawn)
	end
end
