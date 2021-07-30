local drunk = Condition(CONDITION_DRUNK)
drunk:setParameter(CONDITION_PARAM_TICKS, 5000)

local peppermoonBells = Action()

function peppermoonBells.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if item.itemid == 23461 then
		player:say("This flower has no pollen left. It will take a little before it has new pollen.", TALKTYPE_MONSTER_SAY, false, player, item:getPosition())
		return true
	else
	if math.random(5) == 1 then
		player:say("Achoo! You spill the pollen into your bag.", TALKTYPE_MONSTER_SAY, false, player, item:getPosition())
		player:addCondition(drunk)
		item:getPosition():sendMagicEffect(CONST_ME_STUN)
	else
		player:say("You collected the smelly pollen into a bag.", TALKTYPE_MONSTER_SAY, false, player, item:getPosition())
		item:getPosition():sendMagicEffect(CONST_ME_MAGIC_BLUE)
	end
	player:addItem(23460, 1) -- blue pollen
	item:transform(23461)
	item:decay()
	return true
	end
end

peppermoonBells:id(23459, 23461)
peppermoonBells:register()
