--[[
	those events will be used by the example monster
	You don't need to insert all event functions in here, just those which you need, rest can be erased
]]
function onThink(interval)
	return print("I'm thinking!")
end

function onCreatureAppear(creature)
	return print("Hello")
end

function onCreatureDisappear(creature)
	return print("Bye Bye")
end

function onCreatureMove(self, creature, oldPosition, newPosition)
	return print("I'm walking")
end

function onCreatureSay(self, creature, type, message)
	return print("I'm talking")
end
