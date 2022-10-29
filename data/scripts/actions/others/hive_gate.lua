local hiveGateHorizontal = Action()

function hiveGateHorizontal.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local position = player:getPosition()
	if position.y == toPosition.y then
		return false
	end

	toPosition.y = position.y > toPosition.y and toPosition.y - 1 or toPosition.y + 1
	player:teleportTo(toPosition)
	toPosition:sendMagicEffect(CONST_ME_TELEPORT)
	return true
end

hiveGateHorizontal:id(14755, 14756, 14757, 14758, 14759, 14760)
hiveGateHorizontal:register()

local hiveGateVertical = Action()

function hiveGateVertical.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	local position = player:getPosition()
	if position.x == toPosition.x then
		return false
	end

	toPosition.x = position.x > toPosition.x and toPosition.x - 1 or toPosition.x + 1
	player:teleportTo(toPosition)
	toPosition:sendMagicEffect(CONST_ME_TELEPORT)
	return true
end

hiveGateVertical:id(14767, 14768, 14769, 14770, 14771)
hiveGateVertical:register()
