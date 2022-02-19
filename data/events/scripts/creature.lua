function Creature:onChangeOutfit(outfit)
	if EventCallback.onChangeMount then
		if not EventCallback.onChangeMount(self, outfit.lookMount) then
			return false
		end
	end
	if EventCallback.onChangeOutfit then
		return EventCallback.onChangeOutfit(self, outfit)
	else
		return true
	end
end

function Creature:onAreaCombat(tile, isAggressive)
	if EventCallback.onAreaCombat then
		return EventCallback.onAreaCombat(self, tile, isAggressive)
	else
		return RETURNVALUE_NOERROR
	end
end

function Creature:onTargetCombat(target)
	if EventCallback.onTargetCombat then
		return EventCallback.onTargetCombat(self, target)
	else
		return RETURNVALUE_NOERROR
	end
end

function Creature:onHear(speaker, words, type)
	if EventCallback.onHear then
		EventCallback.onHear(self, speaker, words, type)
	end
end
