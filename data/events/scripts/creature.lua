function Creature:onChangeOutfit(outfit)
	if hasEventCallback(EVENT_CALLBACK_ONCHANGEMOUNT) then
		if not EventCallback(EVENT_CALLBACK_ONCHANGEMOUNT, self, outfit.lookMount) then
			return false
		end
	end
	if hasEventCallback(EVENT_CALLBACK_ONCHANGEOUTFIT) then
		return EventCallback(EVENT_CALLBACK_ONCHANGEOUTFIT, self, outfit)
	else
		return true
	end
end

function Creature:onAreaCombat(tile, isAggressive)
	if hasEventCallback(EVENT_CALLBACK_ONAREACOMBAT) then
		return EventCallback(EVENT_CALLBACK_ONAREACOMBAT, self, tile, isAggressive)
	else
		return RETURNVALUE_NOERROR
	end
end

function Creature:onTargetCombat(target)
	if hasEventCallback(EVENT_CALLBACK_ONTARGETCOMBAT) then
		return EventCallback(EVENT_CALLBACK_ONTARGETCOMBAT, self, target)
	else
		return RETURNVALUE_NOERROR
	end
end

function Creature:onHear(speaker, words, type)
	if hasEventCallback(EVENT_CALLBACK_ONHEAR) then
		EventCallback(EVENT_CALLBACK_ONHEAR, self, speaker, words, type)
	end
end
