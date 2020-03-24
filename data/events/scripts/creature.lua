function Creature:onChangeOutfit(outfit)
	if hasEventCallback(EVENT_CALLBACK_ONCHANGEOUTFIT) then return EventCallback(EVENT_CALLBACK_ONCHANGEOUTFIT, self, outfit) else return true end
end

function Creature:onAreaCombat(tile, isAggressive)
	return hasEventCallback(EVENT_CALLBACK_ONAREACOMBAT) and EventCallback(EVENT_CALLBACK_ONAREACOMBAT, self, tile, isAggressive) or RETURNVALUE_NOERROR
end

function Creature:onTargetCombat(target)
	return hasEventCallback(EVENT_CALLBACK_ONTARGETCOMBAT) and EventCallback(EVENT_CALLBACK_ONTARGETCOMBAT, self, target) or RETURNVALUE_NOERROR
end

function Creature:onHear(speaker, words, type)
	EventCallback(EVENT_CALLBACK_ONHEAR, self, speaker, words, type)
end
