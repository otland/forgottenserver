function Creature:onChangeOutfit(outfit)
	if EventCallback:has(EVENT_CALLBACK_ONCHANGEMOUNT) then
		if not EventCallback:get(EVENT_CALLBACK_ONCHANGEMOUNT, self, outfit.lookMount) then
			return false
		end
	end
	if EventCallback:has(EVENT_CALLBACK_ONCHANGEOUTFIT) then
		return EventCallback:get(EVENT_CALLBACK_ONCHANGEOUTFIT, self, outfit)
	else
		return true
	end
end

function Creature:onAreaCombat(tile, isAggressive)
	if EventCallback:has(EVENT_CALLBACK_ONAREACOMBAT) then
		return EventCallback:get(EVENT_CALLBACK_ONAREACOMBAT, self, tile, isAggressive)
	else
		return RETURNVALUE_NOERROR
	end
end

function Creature:onTargetCombat(target)
	if EventCallback:has(EVENT_CALLBACK_ONTARGETCOMBAT) then
		return EventCallback:get(EVENT_CALLBACK_ONTARGETCOMBAT, self, target)
	else
		return RETURNVALUE_NOERROR
	end
end

function Creature:onHear(speaker, words, type)
	if EventCallback:has(EVENT_CALLBACK_ONHEAR) then
		EventCallback:get(EVENT_CALLBACK_ONHEAR, self, speaker, words, type)
	end
end
