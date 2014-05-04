local dead_human = {
	3058, 3059, 3060, 3061, 3064, 3065, 3066
}
function onCastSpell(creature, var)
	local position = variantToPosition(var)
	local tile = position:getTile()
	local object = tile and tile:getTopVisibleThing()
	if object
		and not(object:isCreature() or tile:hasFlag(TILESTATE_PROTECTIONZONE) or isInArray(dead_human, object:getId()))
		and object:getType():isMovable()
		and object:getUniqueId() > 65535
		and object:getActionId() == 0
	then
		object:remove(1)
		position:sendMagicEffect(CONST_ME_BLOCKHIT)
		return true
	end

	creature:sendCancelMessage(RETURNVALUE_NOTPOSSIBLE)
	creature:getPosition():sendMagicEffect(CONST_ME_POFF)
	return false
end
