function Creature:onChangeOutfit(outfit)
	if EventCallback.onChangeMount then
		if not EventCallback.onChangeMount(self, outfit.lookMount) then
			return false
		end
	end
	if EventCallback.onChangeOutfit then
		return EventCallback.onChangeOutfit(self, outfit)
	end
	return true
end

function Creature:onAreaCombat(tile, isAggressive)
	if EventCallback.onAreaCombat then
		return EventCallback.onAreaCombat(self, tile, isAggressive)
	end
	return RETURNVALUE_NOERROR
end

function Creature:onTargetCombat(target)
	if EventCallback.onTargetCombat then
		return EventCallback.onTargetCombat(self, target)
	end
	return RETURNVALUE_NOERROR
end

function Creature:onHear(speaker, words, type)
	if EventCallback.onHear then
		EventCallback.onHear(self, speaker, words, type)
	end
end
