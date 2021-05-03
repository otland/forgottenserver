local machines = {
	[5469] = 5513,
	[5470] = 5514
}

local sugarOat = Action()

function sugarOat.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if target:getId() == 2694 then -- bunch of wheat
		if toPosition.x ~= CONTAINER_POSITION then
			Game.createItem(13939, 1, toPosition) -- sugar oat
		else
			player:addItem(13939, 1) -- sugar oat
			toPosition = player:getPosition()
		end
		toPosition:sendMagicEffect(CONST_ME_MAGIC_BLUE)
		item:remove(1)
		target:remove(1)
	else
		local machine = machines[target:getId()]
		if machine then
			target:transform(machine)
			target:decay()
			item:remove(1)
			player:getPosition():sendMagicEffect(CONST_ME_MAGIC_RED)
			player:addAchievementProgress("Homebrewed", 50)
		end
	end
	return true
end

sugarOat:id(5467) -- bunch of sugar cane
sugarOat:register()
