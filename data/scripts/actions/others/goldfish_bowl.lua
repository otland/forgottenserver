local goldfishBowl = Action()

function goldfishBowl.onUse(player, item, fromPosition, target, toPosition, isHotkey)
	if target:getId() ~= 5554 then -- small fish
		return false
	end

	target:remove(1)
	item:transform(5929) -- goldfish bowl
	player:addAchievement("Silent Pet")
	return true
end

goldfishBowl:id(5928) -- goldfish bowl
goldfishBowl:register()
