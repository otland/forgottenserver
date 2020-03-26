local sugarOat = Action()

function sugarOat.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if target.itemid == 2694 then
		if toPosition.x ~= CONTAINER_POSITION then
			Game.createItem(13939, 1, toPosition)
		else
			player:addItem(13939, 1)
			toPosition = player:getPosition()
		end
		toPosition:sendMagicEffect(CONST_ME_MAGIC_BLUE)  
		item:remove(1)
		target:remove(1)
	end
	return true
end

sugarOat:id(5467)
sugarOat:register()
