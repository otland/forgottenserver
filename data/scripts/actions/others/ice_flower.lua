local iceFlower = Action()

function iceFlower.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if math.random(5) == 1 then
		player:addItem(15271, 1) -- ice flower seeds
		player:addAchievementProgress("Ice Harvester", 10)
	end
	item:transform(15270) -- harvested ice flower
	item:decay()
	return true
end

iceFlower:id(15269)
iceFlower:register()
