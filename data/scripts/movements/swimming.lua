local condition = Condition(CONDITION_OUTFIT)
condition:setOutfit({lookType = 267})
condition:setTicks(-1)

-- onStepIn
local swimmingStepIn = MoveEvent()

function swimmingStepIn.onStepIn(creature, item, position, fromPosition)
	if not creature:isPlayer() then
		return false
	end
	
	creature:addCondition(condition)
	return true
end

swimmingStepIn:id(629,630,631,632,633,634)
swimmingStepIn:register()

-- onStepOut
local swimmingStepOut = MoveEvent()

function swimmingStepOut.onStepOut(creature, item, position, fromPosition)
	if not creature:isPlayer() then
		return false
	end
	
	creature:removeCondition(CONDITION_OUTFIT)
	return true
end

swimmingStepOut:id(629,630,631,632,633,634)
swimmingStepOut:register()
